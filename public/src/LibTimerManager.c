#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>  
#include <sys/time.h>  
#include <signal.h>  
#include "LibTimerManager.h"




#define TimerMalloc		malloc
#define TimerFree		free


/*********************************************************************
 * typedef 
 */
 
typedef struct
{
	TimerEvent_t* header;
	int timer_num;
} TimerHead_t;

/*********************************************************************
 * macro 
 */


/*********************************************************************
 * variables 
 */

static TimerHead_t timerListHead;

/*********************************************************************
 * functions 
 */
static int TimerExist( TimerEvent_t *timer);




void TimerUpdate(unsigned short msec)
{
	TimerEvent_t* current = timerListHead.header;
	
	while(current != NULL)
	{
		if(current->open == true)
		{
			current->tick += msec;
			if(current->tick >= current->reload)
			{
				current->callback();
				current->tick = 0;
				if(current->interval == false)
					current->open = false;
			}
		}
		current = current->next;
	}
}



void TimerManagerInit(void)
{
	timerListHead.timer_num = 0;
	timerListHead.header = NULL;
}


int AddTimer(TimerEvent_t* timer)
{
	TimerEvent_t* current = timerListHead.header;
	
	HAL_DISABLE_INTERRUPTS();

	if((timer == NULL) || (TimerExist(timer)))
	{
		HAL_ENABLE_INTERRUPTS();
		return 0;
	}

	timer->open = false;
	timer->interval = false;
	timer->tick = 0;
	timer->reload = 0;
	timer->callback = NULL;
	timer->next = NULL;

	if(current == NULL)
		timerListHead.header = timer;
	else
	{
		while(current->next != NULL)
		{
			current = current->next;
		}
		
		timerListHead.timer_num++;
		current->next = timer;
	}
	
	HAL_ENABLE_INTERRUPTS();
	return 1;
}

void SetTimer(TimerEvent_t* timer, unsigned long timeout, bool is_interval, void (*callback)(void))
{
	HAL_DISABLE_INTERRUPTS();
	
	timer->interval = is_interval;
	timer->reload = timeout;
	timer->callback = callback;
	timer->tick = 0;
	timer->open = true;
	
	HAL_ENABLE_INTERRUPTS();
}

void StopTimer(TimerEvent_t *timer)
{
	timer->open = false;
}


void ResetTimer(TimerEvent_t *timer)
{
	timer->tick = 0;
}


void DeleteTimer(TimerEvent_t *timer)
{
	TimerEvent_t* current = timerListHead.header;
	
	HAL_DISABLE_INTERRUPTS();
	
	if((timer == NULL) || (current == NULL))
	{
		HAL_ENABLE_INTERRUPTS();
		return;
	}
	
	//delete timer
	if(current == timer)
	{
		timerListHead.header = current->next;
	}
	else
	{
		while(current->next != NULL)
		{
			if(current->next == timer)
				current->next = current->next->next;
		}
	}
	
	HAL_ENABLE_INTERRUPTS();
}


static int TimerExist( TimerEvent_t *timer )
{
	TimerEvent_t* current = timerListHead.header;
	
	while(current != NULL)
	{
		if(current == timer)
			return 1;
		current = current->next;
	}	
	
	return 0;
}














