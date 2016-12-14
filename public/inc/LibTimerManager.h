#ifndef __LIBTIMERMANAGER_H__
#define __LIBTIMERMANAGER_H__



typedef  unsigned char bool;

#ifndef true
#define true	1
#endif
#ifndef false
#define false	0
#endif


typedef struct _tag_TimerEvent
{
	bool open;
	unsigned long timeout;	// < 2^31
	void (*callback)(void);
	struct _tag_TimerEvent *next;
} TimerEvent_t;



#define HAL_DISABLE_INTERRUPTS()	
#define HAL_ENABLE_INTERRUPTS()	


#define time_after(unknown,known) ((long)(known) - (long)(unknown)<0)
#define time_before(unkonwn,known) ((long)(unknown) - (long)(known)<0)
#define time_after_eq(unknown,known) ((long)(unknown) - (long)(known)>=0)
#define time_before_eq(unknown,known) ((long)(known) -(long)(unknown)>=0)



void TimerUpdate(unsigned short msec);

void TimerManagerInit(void);

int AddTimer(TimerEvent_t* timer);

void SetTimer(TimerEvent_t* timer, unsigned long timeout, void (*callback)(void));

void StopTimer(TimerEvent_t *timer);

void StartTimer(TimerEvent_t *timer);

void DeleteTimer(TimerEvent_t *timer);


#endif






