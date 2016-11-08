#include <stdio.h>
#include "LibLoger.h"
#include <sys/ipc.h>
//#include <sys/msg.h>

/*
	不同进程使用同一个key创建消息队列；
 msg exp:
	struct msg_st {
	  long int msg_type;
	  char text[BUFSIZE];
	};
 */ 


int CreatMessageQueue(key_t msgKey)
{
	int msgid = -1;
	msgid = msgget(msgKey, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		error("msg queue creat error:");
	}
	return msgid;
}


int MsgQueueSend(int msgid, void* msg, size_t msgLen)
{
	if (msgsnd(msgid, msg, msgLen - sizeof(long), 0) == -1) 
	{
		error("msg send error:");
		return -1;
	}
	return 1;
}

/* 
 * msgType : if 0: 顺序接收
 * msgLen : 接收数据长度，不包含长整型msgType长度
 *
 */
int MsgQueueRecv(int msgid, void* msg, size_t msgLen, long int msgType)
{
	if (msgrcv(msgid, msg, msgLen - sizeof(long), msgType, 0) == -1) 
    {
      error("msg queue recv error:");
      return -1;
    }
	return 1;
}

int MsgQueueDelete(int msgid)
{
  if (msgctl(msgid, IPC_RMID, 0) == -1) 
  {
    error("msg queue delete error:");
    return -1;
  }
  return 1;
}


