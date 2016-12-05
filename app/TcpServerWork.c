#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "LibSocket.h"
#include "LibEpoll.h"
#include "LibFdManager.h"
#include "LibLoger.h"
#include "LinkQueue.h"
#include "thread_pool.h"
#include "DTL645.h"
#include "ProtocolHandle.h"


#define MAX_BUFF_SIZE	1024
typedef struct
{
	int len; 
	uint8_t buff[MAX_BUFF_SIZE];
} DataBuff_t;



extern LinkQueue *g_tcpDataQueue;
extern tp_thread_pool *g_threadPool;
extern int g_MsgQueueID;
FdSet_t* g_fdManagerSet;


 

//thread tcp listen
void TcpServer_ListenWork(int listen_fd)
{
	EpollSet_t* epollSet;
	int fds;
	int i;
	
	info("listen_work thread creat Epoll set\n");
	if((epollSet = CreatEpollSet(500)) == NULL)
	{
		error("creat EPOLL SET error!");	
		return;// -1;
	}

	info("listen_work thread creat fd set\n");
	g_fdManagerSet = FdSet_Creat();
	if(g_fdManagerSet == NULL)
	{
		error("creat fd manager set error!");	
		return;// -1;
	}
	
	if(EpollSetAddFd(epollSet,listen_fd) == -1)
		return;// -1;
	
	while(1)
	{
		fds = EpollSetWait(epollSet);
		if(fds < 0)
		{
			error("wait error, return num = %d!",fds);//-----------------
			//return -1;
		}
		for(i=0; i<fds; i++)
		{
			if(epollSet->events[i].data.fd == listen_fd)
			{
				int sock_fd;
				sock_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);
				if(sock_fd < 0)
				{
					perror("accept error:");
					continue;
				}
			    info("accept sock:%d\n",sock_fd);	
				EpollSetAddFd(epollSet,sock_fd);
				
				if(-1 == FdSet_AddNode(g_fdManagerSet, sock_fd,1))
				{
					error("FdSet_AddNode add fd error\n");
				}
				continue;
			}
			else
			{	
				DataBuff_t* dataBuff;
				dataBuff = (DataBuff_t*)malloc(sizeof(DataBuff_t));
				if(dataBuff != NULL)
				{
					dataBuff->len = recv(epollSet->events[i].data.fd, dataBuff->buff, MAX_BUFF_SIZE, 0);
					if(dataBuff->len <= 0)
					{
						EpollSetDeleteFd(epollSet, epollSet->events[i].data.fd);
						info("remove fd %d from g_fdManagerSet!\n",epollSet->events[i].data.fd);
						FdSet_DeleteNode(g_fdManagerSet,epollSet->events[i].data.fd);
						if(dataBuff->len == 0)
						{
							info("fd %d close connect!\n",epollSet->events[i].data.fd);
							close(epollSet->events[i].data.fd);
						}
						else
						{
							error("fd %d appear error!\n",epollSet->events[i].data.fd);
						}
			
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

//
/* thread data handle
 * 解析在TCP层之上封装的协议，获取数据内容
 * 		格式：DTL645
 */
void TcpServer_HandleWork(int m)
{
	while(1)
	{
		if(LinkQueue_Length(g_tcpDataQueue))
		{
			DataBuff_t* dataBuff;
			dataBuff = (DataBuff_t*)LinkQueue_Retrieve(g_tcpDataQueue);
			if(dataBuff != NULL)
			{
				int pos = 0;
				info("recv: ");
				info_hex_set(dataBuff->buff,dataBuff->len);
				info("\n");
				do
				{
					DTL645Item_t* DTL645Item = (DTL645Item_t*)malloc(sizeof(DTL645Item_t));
					if(DTL645Item != NULL)
					{
						pos = DecodeDTL645Frame(dataBuff->buff + pos, dataBuff->len, DTL645Item);
						if(pos < 0)
						{
							error("DecodeDTL645Frame error!\n");
							debug("free DTL645Item!\n");
							free(DTL645Item);
							break;
						}
						
						/*解析在DTL645协议之上封装的协议：应用层协议*/
						debug("going to process protocol!\n");
						g_threadPool->process_job(g_threadPool,ProtocolHandle,(void*)DTL645Item);		//第二次接收数据卡死在这里
						//ProtocolHandle(DTL645Item);
					}
				}
				while((dataBuff->len - pos) > 0);
				debug("free dataBuff!\n");
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




