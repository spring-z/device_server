#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "LibEpoll.h"
#include "Libloger.h"


EpollSet_t* CreatEpollSet(int maxEvents)
{
	EpollSet_t* epollSet = (EpollSet_t*)malloc(sizeof(EpollSet_t));
	if(epollSet != NULL)
	{
		epollSet->events = (struct epoll_event*)malloc(sizeof(struct epoll_event));
		if(epollSet->events == NULL)
		{
			error("epoll events malloc error:");
			free(epollSet)
			epollSet = NULL;
			return NULL;
		}
		
		epollSet->epoll_fd = epoll_create(maxEvents);
		if(epollSet->epoll_fd < 0)
		{
			error("epoll creat error:");
			free(epollSet)
			epollSet = NULL;
			return NULL;
		}
		
		epollSet->max_events = maxEvents;
	}
	
	
	return epollSet;
}


int EpollSetAddFd(EpollSet_t* epollSet, int fd)
{
	struct epoll_event ev;
	
	ev.data.fd = fd;	
	ev.events = EPOLLIN;	//epoll watch data in
	if(epoll_ctl(epollSet->epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
	{
		error("epoll_ctl add error:");
		return -1;
	}
	
	return 1;
}


int EpollSetDeleteFd(EpollSet_t* epollSet, int fd)
{
	if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0)
	{
		error("epoll_ctl del error:");
		return -1;
	}
	
	close(fd);
	return 1;
}




int EpollSetWait(EpollSet_t* epollSet)
{
	return epoll_wait(epollSet->epoll_fd, epollSet->events, epollSet->max_events, -1);
}



void EpollSetDestroyed(EpollSet_t* epollSet)
{
	free(epollSet);
	epollSet = NULL;
}














