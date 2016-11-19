#ifndef _LIBMYSQL_H_
#define _LIBMYSQL_H_


#include <pthread.h>
#include <mysql/mysql.h>

#define HOST_IP			"115.28.146.59"
#define USERNAME		"root"
#define PASSWORD		"Hqu123456"
#define DB_NAME			"rssicap"



typedef struct 
{
	MYSQL conn;
	pthread_mutex_t  conn_lock;
	int is_buzy;
} dbConnNode_t;



typedef struct 
{
	dbConnNode_t* conn_node_arry;
	int conn_num;
	pthread_mutex_t  pool_lock;
	
} dbConnPool_t;



dbConnPool_t* CreatMysqlConnPool(int conn_num);
dbConnNode_t* GetMysqlConnNode(dbConnPool_t* conn_pool);
void ReleaseMysqlConnNode(dbConnNode_t* conn_node);

int ReConnectMysqlConnNode(dbConnNode_t* conn_node);
void DestoyedMysqlConnPool(dbConnPool_t* conn_pool);

int MysqlExcuteQuery(dbConnNode_t* conn_node, char* query_str,...);

#endif

