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
	bool interval;
	unsigned long tick;
	unsigned long reload;
	void (*callback)(void);
	struct _tag_TimerEvent *next;
} TimerEvent_t;



#define HAL_DISABLE_INTERRUPTS()	
#define HAL_ENABLE_INTERRUPTS()	



void TimerOnIRQ(int m);

void TimerManagerInit(void);

int AddTimer(TimerEvent_t* timer);

void SetTimer(TimerEvent_t* timer, unsigned long timeout, bool is_interval, void (*callback)(void));

void StopTimer(TimerEvent_t *timer);

void ResetTimer(TimerEvent_t *timer);

void DeleteTimer(TimerEvent_t *timer);


#endif






