#include <stdio.h>
#include <stdlib.h>
#include "LibSocket.h"
#include "LinkQueue.h"


#define MAX_BUFF_SIZE	1024
typedef struct
{
	int len; 
	char* buff[MAX_BUFF_SIZE];
} DataBuffItem_t;



extern LinkQueue *g_tcpDataQueue;
extern tp_thread_pool *g_threadPool;
extern int g_MsgQueueID;
FdSet_t g_fdManagerSet;


 

//thread tcp listen
void TcpServer_ListenWork(int listen_fd)
{
	EpollSet_t* epollSet;
	char tcpRecvBuff[1024];
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
				
				if(-1 == FdSet_AddNode(g_fdManagerSet, sock_fd))
				{
					free(fdNode);
					error("FdSet_AddNode add fd error\n");
				}
				continue;
			}
			else
			{	
				DataBuffItem_t* dataBuff;
				dataBuff = (DataBuffItem_t*)malloc(sizeof(DataBuffItem_t));
				if(dataBuff != NULL)
				{
					dataBuff->len = recv(epollSet->events[i].data.fd, dataBuff->buff, MAX_BUFF_SIZE, 0);
					if(dataBuff->len <= 0)
					{
						EpollSetDeleteFd(epollSet, epollSet->events[i].data.fd);
						FdSet_DeleteNode(g_fdManagerSet,epollSet->events[i].data.fd);
						close(epollSet->events[i].data.fd);
						continue;
					}
					else
					{
						FdSet_Update(g_fdManagerSet,epollSet->events[i].data.fd);
						if(0 == LinkQueue_Append(g_tcpDataQueue, (uint8_t*)dataBuff))
						{
							error("dataBuff append error!\n");
							free(dataBuff);
						}
					}
				}
				else
				{
					error("dataBuff malloc error!\n");
				}
			}
			
		}
	}
	
	error("TcpServer_ListenWork exiting....!\n");
	
}

//thread data handle
void TcpServer_HandleWork(int m)
{
	while(1)
	{
		if(LinkQueue_Length(g_tcpDataQueue))
		{
			DataBuffItem_t* dataBuff;
			dataBuff = (DataBuffItem_t*)LinkQueue_Retrieve(g_tcpDataQueue)
			if(dataBuff != NULL)
			{
				DTL654Item_t* DTL654Item;
				int pos;
				do
				{
					pos = DecodeDTL654Frame(dataBuff->buff, dataBuff->len, DTL654Item);
					if(pos < 0)
					{
						error("DecodeDTL654Frame error!\n");
						free(dataBuff);
						break;
					}
					ProtocolHandle(DTL654Item);
					free(DTL654Item->DTL654FrameData);
				}
				while(pos > 0);
				
				free(dataBuff);
			}
			else
			{
				error("LinkQueue_Retrieve g_tcpDataQueue error!\n");
			}
		}
		else
		{
			sleep(1);
		}
	}

}




