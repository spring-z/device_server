#include <stdio.h>
#include <stdlib.h>
#include "LibSocket.h"







FdSet_t g_fdManagerSet;






void TcpServer_ListenWork(int listen_fd)
{
	EpollSet_t* epollSet;
	int fds;
	int i;
	
	if((epollSet = CreatEpollSet(500)) == NULL)
	{
		error("creat EPOLL SET error!");	
		return -1;
	}

	g_fdManagerSet = FdSet_Creat();
	if(g_fdManagerSet == NULL)
	{
		error("creat fd manager set error!");	
		return -1;
	}
	
	if(EpollSetAddFd(epollSet,listen_fd) == -1)
		return -1;
	
	while(1)
	{
		fds = EpollSetWait(epollSet);
		if(fds < 0)
		{
			perror("wait error:");
			return -1;
		}
		for(i=0; i<fds; i++)
		{
			if(epollSet->events[i].data.fd == listen_fd)
			{
				sock_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);
				if(sock_fd < 0)
				{
					perror("accept error:");
					continue;
				}
			    printf("accept sock:%d\n",sock_fd);	
				EpollSetAddFd(epollSet,sock_fd);
				
				continue;
			}
			else
			{	
				int bytes;
				bytes = recv(epollSet->events[i].data.fd, msg_buff.buff, MAX_BUFF_LEN, 0);
				if(bytes <= 0)
				{
					EpollSetDeleteFd(epollSet, epollSet->events[i].data.fd);
					close(epollSet->events[i].data.fd);
					continue;
				}
				else
				{
					printf("%s\n",(char*)msg_buff.buff);
					msg_buff.msg_type = 1;
					printf("send %s, type = %ld, len = %ld\n", msg_buff.buff,msg_buff.msg_type,sizeof(struct msg_st));
					MsgQueueSend(msgId,&msg_buff,sizeof(struct msg_st));
				}
			}
			
		}
	}
	
	
	
	
	
	
	
	
}







