#ifndef _LIBMSGQUEUE_H_
#define _LIBMSGQUEUE_H_


int CreatMessageQueue(key_t msgKey);
int MsgQueueSend(int msgid, void* msg, size_t msgLen);
int MsgQueueRecv(int msgid, void* msg, size_t msgLen, long int msgType);
int MsgQueueDelete(int msgid);



#endif



