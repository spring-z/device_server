#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "LibEpoll.h"
#include "Libloger.h"







int CreatEpollSet(int maxEvents)
{
	int epollFd;
	
	epollFd = epoll_create(maxEvents);
	if(epollFd < 0)
	{
		error("epoll creat error:");
		return -1;
	}
	
	
}








