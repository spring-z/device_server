#include <stdio.h>
#include "LibMsgQueue.h"



#define MSGQUEUE_KEY 718
#define MAX_BUFF_LEN 1024

struct msg_st {
  long int msg_type;
  char buff[MAX_BUFF_LEN];
} msg_buff;


int msgId;

int main()
{
	
	if((msgId = CreatMessageQueue(MSGQUEUE_KEY)) == -1)
		return -1;
		
	while(1)
	{
		MsgQueueRecv(msgId,&msg_buff,sizeof(struct msg_st),1);
		printf("%s\n",msg_buff.buff);
	}


}







