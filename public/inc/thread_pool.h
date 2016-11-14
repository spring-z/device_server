#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#ifndef TPBOOL
typedef int TPBOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_TH_NUM 10
#define MIN_TH_NUM 2

#define BUSY_THRESHOLD 0.5	//(busy thread)/(all thread threshold)
#define MANAGE_INTERVAL 1	//tp manage thread sleep interval

typedef void* tp_work_desc;
typedef void* (*tp_work)(void *);

typedef struct tp_thread_info_s tp_thread_info;
typedef struct tp_thread_pool_s tp_thread_pool;

//thread info
struct tp_thread_info_s{
	pthread_t		thread_id;	//thread id num
	TPBOOL  		is_busy;	//thread status:true-busy;flase-idle
	pthread_cond_t  thread_cond;	
	pthread_mutex_t	thread_lock;
	tp_work			th_work;
	tp_work_desc	th_job;
	TPBOOL			exit;
	TPBOOL			is_wait; // CAUTION锛氬湪鏈皟鐢╬thread_cond_wait鏃堕�杩噋thread_cond_signal鍙戦�淇″彿浼氶�鎴愪俊鍙蜂涪澶憋紒
};

//main thread pool struct
struct tp_thread_pool_s{
	TPBOOL (*init)(tp_thread_pool *this);
	void (*close)(tp_thread_pool *this);
	void (*process_job)(tp_thread_pool *this, tp_work worker, tp_work_desc job);
	int  (*get_thread_by_id)(tp_thread_pool *this, pthread_t id);
	TPBOOL (*add_thread)(tp_thread_pool *this);
	TPBOOL (*delete_thread)(tp_thread_pool *this);
	int (*get_tp_status)(tp_thread_pool *this);
	
	int min_th_num;		//min thread number in the pool
	int cur_th_num;		//current thread number in the pool
	int max_th_num;         //max thread number in the pool
	pthread_mutex_t tp_lock;
	pthread_t manage_thread_id;	//manage thread id num
	tp_thread_info *thread_info;	//work thread relative thread info
};

tp_thread_pool *creat_thread_pool(int min_num, int max_num);

