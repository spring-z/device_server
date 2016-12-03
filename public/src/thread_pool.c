#include "thread_pool.h"
#include "LibLoger.h"


static void *tp_work_thread(void *pthread);
static void tp_manage_thread(void *pthread);

static TPBOOL tp_init(tp_thread_pool *this);
static void tp_close(tp_thread_pool *this);
static void tp_process_job(tp_thread_pool *this, tp_work worker,
		tp_work_desc job);
static int tp_get_thread_by_id(tp_thread_pool *this, pthread_t id);
static TPBOOL tp_add_thread(tp_thread_pool *this);
static TPBOOL tp_delete_thread(tp_thread_pool *this);
static int tp_get_tp_status(tp_thread_pool *this);

/**
 * user interface. creat thread pool.
 * para:
 * 	num: min thread number to be created in the pool
 * return:
 * 	thread pool struct instance be created successfully
 */
tp_thread_pool *creat_thread_pool(int min_num, int max_num) {
	tp_thread_pool *this;
	this = (tp_thread_pool*) malloc(sizeof(tp_thread_pool));
	if (this == NULL) {
		error("FILE:%s LINE:%d malloc thread pool error", __FILE__, __LINE__);
		exit(-1);
	}

	//init member function ponter
	this->init = tp_init;
	this->close = tp_close;
	this->process_job = tp_process_job;
	this->get_thread_by_id = tp_get_thread_by_id;
	this->add_thread = tp_add_thread;
	this->delete_thread = tp_delete_thread;
	this->get_tp_status = tp_get_tp_status;

	//init member var
	this->min_th_num = min_num;
	this->cur_th_num = this->min_th_num;
	this->max_th_num = max_num;
	pthread_mutex_init(&this->tp_lock, NULL);

	//malloc mem for num thread info struct
	this->thread_info = (tp_thread_info*) malloc(
			sizeof(tp_thread_info) * this->max_th_num);

	return this;
}

/**
 * member function reality. thread pool init function.
 * para:
 * 	this: thread pool struct instance ponter
 * return:
 * 	true: successful; false: failed
 */
TPBOOL tp_init(tp_thread_pool *this) {
	int i, num = this->min_th_num;
	int err;

	//creat work thread and init work thread info
	for (i = 0; i < num; i++) {
		pthread_cond_init(&this->thread_info[i].thread_cond, NULL);
		pthread_mutex_init(&this->thread_info[i].thread_lock, NULL);

		err = pthread_create(&this->thread_info[i].thread_id, NULL,
				tp_work_thread, &this->thread_info[i]);
		if (0 != err) {
			error("tp_init: creat work thread failed\n");
			return FALSE;
		}
		info("tp_init: creat work thread 0x%X\n",
				(int)this->thread_info[i].thread_id);
	}

	//creat manage thread
	err = pthread_create(&this->manage_thread_id, NULL, tp_manage_thread, this);
	if (0 != err) {
		info("tp_init: creat manage thread failed\n");
		return FALSE;
	}
	info("tp_init: creat manage thread 0x%X\n", (int)this->manage_thread_id);

	return TRUE;
}

/**
 * member function reality. thread pool entirely close function.
 * para:
 * 	this: thread pool struct instance ponter
 * return:
 */
void tp_close(tp_thread_pool *this) {
	int i;
	void *status;

	//close work thread
	for (i = 0; i < this->cur_th_num; i++) {
		if (pthread_kill(this->thread_info[i].thread_id, 0) != ESRCH) {
			pthread_kill(this->thread_info[i].thread_id, SIGQUIT);
			pthread_join(this->thread_info[i].thread_id, &status);
			pthread_mutex_destroy(&this->thread_info[i].thread_lock);
			pthread_cond_destroy(&this->thread_info[i].thread_cond);
			info("tp_close: kill work thread 0x%X\n",
					(int)this->thread_info[i].thread_id);
		}
	}

	if (pthread_kill(this->manage_thread_id, 0) != ESRCH) {
		//close manage thread
		pthread_kill(this->manage_thread_id, SIGQUIT);
		pthread_join(this->manage_thread_id, &status);
		pthread_mutex_destroy(&this->tp_lock);
		info("tp_close: kill manage thread 0x%X\n", (int)this->manage_thread_id);
	}

	//free thread struct
	free(this->thread_info);
}

/**
 * member function reality. main interface opened.
 * after getting own worker and job, user may use the function to process the task.
 * para:
 * 	this: thread pool struct instance ponter
 *	worker: user task reality.
 *	job: user task para
 * return:
 */

#define TP_THREAD_IS_WAIT(idx) \
do \
{ \
	while(1) \
	{ \
		pthread_mutex_lock(&this->thread_info[idx].thread_lock); \
		if(this->thread_info[idx].is_wait) \
		{ \
			pthread_mutex_unlock(&this->thread_info[idx].thread_lock); \
			break; \
		} \
		pthread_mutex_unlock(&this->thread_info[idx].thread_lock); \
		sleep(1); \
	} \
}while(0)

void tp_process_job(tp_thread_pool *this, tp_work worker, tp_work_desc job) {
	int i;
	//int tmpid;
	TPBOOL res;

	//fill this->thread_info's relative work key
	for (i = 0; i < this->cur_th_num; i++) {
		pthread_mutex_lock(&this->thread_info[i].thread_lock);
		if (!this->thread_info[i].is_busy) {
			//info("tp_process_job: %d thread idle, thread id is %ld\n", i, this->thread_info[i].thread_id);
			//thread state be set busy before work
			this->thread_info[i].is_busy = TRUE;
			pthread_mutex_unlock(&this->thread_info[i].thread_lock);
			this->thread_info[i].th_work = worker;
			this->thread_info[i].th_job = job;

			//info("tp_process_job: informing idle working thread %d, thread id is %ld\n", i, this->thread_info[i].thread_id);
			TP_THREAD_IS_WAIT(i);
			pthread_cond_signal(&this->thread_info[i].thread_cond);
			return;
		} else {
			pthread_mutex_unlock(&this->thread_info[i].thread_lock);
		}
	}			//end of for

				//if all current thread are busy, new thread is created here
	//info("if all current thread are busy, new thread is created here");
	pthread_mutex_lock(&this->tp_lock);
	if ((res = this->add_thread(this))) {
		i = this->cur_th_num - 1;
		//tmpid = this->thread_info[i].thread_id;
		this->thread_info[i].th_work = worker;
		this->thread_info[i].th_job = job;
	}
	pthread_mutex_unlock(&this->tp_lock);

	if (res) {
		TP_THREAD_IS_WAIT(i);
		pthread_cond_signal(&this->thread_info[i].thread_cond);
	}
	info("if all current thread are busy, new thread is created here------------OK");
	return;
}

/**
 * member function reality. get real thread by thread id num.
 * para:
 * 	this: thread pool struct instance ponter
 *	id: thread id num
 * return:
 * 	seq num in thread info struct array
 */
int tp_get_thread_by_id(tp_thread_pool *this, pthread_t id) {
	int i;

	for (i = 0; i < this->cur_th_num; i++) {
		if (id == this->thread_info[i].thread_id) {
			return i;
		}
	}

	return -1;
}

/**
 * member function reality. add new thread into the pool.
 * para:
 * 	this: thread pool struct instance ponter
 * return:
 * 	true: successful; false: failed
 */
static TPBOOL tp_add_thread(tp_thread_pool *this) {
	int err;
	tp_thread_info *new_thread;

	if (this->max_th_num <= this->cur_th_num) {
		info("Thread pool full \n");
		return FALSE;
	}

	//malloc new thread info struct
	new_thread = &this->thread_info[this->cur_th_num];

	//init new thread's cond & mutex
	pthread_cond_init(&new_thread->thread_cond, NULL);
	pthread_mutex_init(&new_thread->thread_lock, NULL);

	//init status is busy
	new_thread->is_busy = TRUE;
	new_thread->exit = FALSE;
	new_thread->is_wait = FALSE;

	err = pthread_create(&new_thread->thread_id, NULL, tp_work_thread,
			new_thread);
	if (0 != err) {
		pthread_mutex_destroy(&new_thread->thread_lock);
		pthread_cond_destroy(&new_thread->thread_cond);
		new_thread->is_busy = FALSE;
		info("ERROR:Create thread.\n");
		return FALSE;
	}

	//add current thread number in the pool.
	this->cur_th_num++;

	//info("Creat work thread %d;current threads number is %d.\n", this->thread_info[this->cur_th_num-1].thread_id,this->cur_th_num);

	return TRUE;
}

/**
 * member function reality. delete idle thread in the pool.
 * only delete last idle thread in the pool.
 * para:
 * 	this: thread pool struct instance ponter
 * return:
 * 	true: successful; false: failed
 */
static TPBOOL tp_delete_thread(tp_thread_pool *this) {
	void* status;
	int idx = this->cur_th_num - 1;
	TPBOOL res;

	//current thread num can't < min thread num
	if (this->cur_th_num <= this->min_th_num) {
		//info("current thread num can't < min thread num\n");
		return FALSE;
	}
	// check thread status
	pthread_mutex_lock(&this->thread_info[idx].thread_lock);
	//if last thread is busy, do nothing
	if (this->thread_info[idx].is_busy) {
		info("last thread is busy, do nothing.worker=%p,job=%p\n",
				this->thread_info[idx].th_work, this->thread_info[idx].th_job);
		res = FALSE;
		pthread_mutex_unlock(&this->thread_info[idx].thread_lock);
	} else {
		this->thread_info[idx].is_busy = TRUE;

		pthread_mutex_lock(&this->tp_lock);
		pthread_mutex_unlock(&this->thread_info[idx].thread_lock);
		//after deleting idle thread, current thread num -1
		this->cur_th_num--;

		//kill the idle thread and free info struct
		this->thread_info[idx].exit = 1;
		pthread_cond_signal(&this->thread_info[idx].thread_cond);
		pthread_join(this->thread_info[idx].thread_id, &status);

		pthread_mutex_destroy(&this->thread_info[idx].thread_lock);
		pthread_cond_destroy(&this->thread_info[idx].thread_cond);
		info("Delete thread.index = %d\n", idx);
		pthread_mutex_unlock(&this->tp_lock);
		res = TRUE;
	}

	return res;
}

/**
 * member function reality. get current thread pool status:idle, normal, busy, .etc.
 * para:
 * 	this: thread pool struct instance ponter
 * return:
 * 	0: idle; 1: normal or busy(don't process)
 */
static int tp_get_tp_status(tp_thread_pool *this) {
	float busy_num = 0.0;
	int i;

	//get busy thread number
	for (i = 0; i < this->cur_th_num; i++) {
		if (this->thread_info[i].is_busy) {
			busy_num++;
		}
	}

	//0.2? or other num?
	busy_num = busy_num / (this->cur_th_num);

	//info("Thread pool busy status = %f.Current thread number = %d\n", busy_num,
	//		this->cur_th_num);

	if (busy_num < BUSY_THRESHOLD) {
		return 0;			//idle status
	} else {
		return 1;			//busy or normal status
	}
}

void *tp_thread_exit() {
	pthread_exit(NULL);
}

void handle_quit(int signo) {
	pthread_t curid;			//current thread id

	//get current thread id
	curid = pthread_self();

	info("Handle sig %d,thread id = 0x%X \n", signo, (int)curid);
	tp_thread_exit();
}

/**
 * internal interface. real work thread.
 * para:
 * 	pthread: thread pool struct ponter
 * return:
 */
static void *tp_work_thread(void *pthread) {
	tp_thread_info *th = (tp_thread_info*) pthread;	//main thread pool struct instance

	signal(SIGQUIT, handle_quit);
	//wait cond for processing real job.
	while ( TRUE) {
		pthread_mutex_lock(&th->thread_lock);
		th->is_wait = TRUE;
		pthread_cond_wait(&th->thread_cond, &th->thread_lock);
		th->is_wait = FALSE;
		pthread_mutex_unlock(&th->thread_lock);

		//info("%d thread do work!\n", pthread_self());

		if (NULL != th->th_work) {
			th->th_work(th->th_job);
		}

		//thread state be set idle after work
		pthread_mutex_lock(&th->thread_lock);
		th->is_busy = FALSE;
		th->th_work = NULL;
		pthread_mutex_unlock(&th->thread_lock);

		if (th->exit) {
			return NULL;
		}
		//info("%d thread do work over!,nseq = %d\n", pthread_self(),nseq);
	}
}

/**
 * internal interface. manage thread pool to delete idle thread.
 * para:
 * 	pthread: thread pool struct ponter
 * return:
 */
static void tp_manage_thread(void *pthread) {
	tp_thread_pool *this = (tp_thread_pool*) pthread;//main thread pool struct instance

	signal(SIGQUIT, handle_quit);

	sleep(MANAGE_INTERVAL);

	do {
		while (this->get_tp_status(this) == 0) {
			if (!this->delete_thread(this)) {
				break;
			}
		}
		sleep(MANAGE_INTERVAL);
	} while (TRUE);
	
}

