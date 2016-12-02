#include <stdio.h>
#include "TcpServerConfig.h"
#include "LibLoger.h"
#include "inirw.h"
#include "thread_pool.h"
#include "LinkQueue.h"
#include "LibMsgQueue.h"
#include "LibMysql.h"
#include "TcpServerWork.h"

LinkQueue *g_tcpDataQueue;
tp_thread_pool *g_threadPool;
dbConnPool_t *g_mysqlConnPool;
int g_MsgQueueID;



int main()
{
	int listenPort;
	int listenFd;
	int minThreadNum;
	int maxThreadNum;
	int mysqlConnNum;
	
	info("server setup\n");
	info("server read config file...\n");
	if(0 == iniFileLoad("config.ini"))
	{
		error("read config file error!");	
		return -1;
	}
	
	listenPort = iniGetInt("tcp_sever","listen_port",0);
	if(listenPort == 0)
	{
		error("invaid listen port!");	
		return -1;
	}
	
	maxThreadNum = iniGetInt("tcp_sever","max_thread",0);
	minThreadNum = iniGetInt("tcp_sever","min_thread",0);
	mysqlConnNum = iniGetInt("tcp_sever","max_mysql_conn",0);
	if((maxThreadNum == 0)||(minThreadNum == 0)||(mysqlConnNum == 0))
	{
		error("invaid thread num!");	
		return -1;
	}
	
	iniFileFree();
	
	info("server creat socket...\n");
	listenFd = StartupTcpSocket(listenPort);
	if(-1 == listenFd)
	{
		error("start socket error!");	
		return -1;
	}
	
	info("server creat threadpool...\n");
	g_threadPool = creat_thread_pool(minThreadNum, maxThreadNum);
	if(NULL == g_threadPool)
	{
		error("creat thread pool error!");	
		return -1;
	}
	
	info("server creat queue...\n");
	g_tcpDataQueue = LinkQueue_Create();
	if(NULL == g_tcpDataQueue)
	{
		error("creat data queue error!");	
		return -1;
	}
	
	info("server creat msgueue...\n");
	g_MsgQueueID = CreatMessageQueue(MSGQUEUE_KEY);
	if(-1 == g_MsgQueueID)
	{
		error("creat msg queue error!");	
		return -1;
	}
	
	info("server creat mysql connect pool...\n");
	g_mysqlConnPool = CreatMysqlConnPool(mysqlConnNum);
	if(NULL == g_mysqlConnPool)
	{
		error("creat mysql connect pool error!");	
		return -1;
	}	
	
	
	g_threadPool->init(g_threadPool);
	g_threadPool->process_job(g_threadPool,TcpServer_ListenWork,(void*)listenFd);
	g_threadPool->process_job(g_threadPool,TcpServer_HandleWork,NULL);
	
	while(1)
	{
		sleep(1);
	}
	
	error("tcp_sever going to exit!\n");
	g_threadPool->close(g_threadPool);
	
	return 0;
}



























