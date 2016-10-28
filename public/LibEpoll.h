#ifndef _LIBEPOLL_H_
#define _LIBEPOLL_H_


#typedef struct {
	int epoll_fd;
	int max_events;
	struct epoll_event* events;
} EpollSet_t;

EpollSet_t* CreatEpollSet(int maxEvents);
int EpollSetAddFd(EpollSet_t* epollSet, int fd);
int EpollSetDeleteFd(EpollSet_t* epollSet, int fd);
int EpollSetWait(EpollSet_t* epollSet);




#endif




