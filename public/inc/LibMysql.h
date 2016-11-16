#ifndef _LIBMYSQL_H_
#define _LIBMYSQL_H_


typedef struct 
{
	MYSQL conn;
	pthread_mutex_t  conn_lock;
	int is_buzy;
} dbConnNode_t;



typedef struct 
{
	dbConnNode* conn_node_info;
	int conn_num;
	pthread_mutex_t  conn_lock;
	
} dbConnPool_t;



dbConnPool_t* CreatMysqlConnPool(int conn_num);
dbConnNode_t* GetMysqlConnNode(void);
int CheckMysqlConnNodeActive(dbConnNode_t* conn_node);
int ReConnectMysqlConnNode(dbConnNode_t* conn_node);
int DeleteMysqlConnNode(dbConnPool_t* conn_pool);
int DestoyedMysqlConnPool(dbConnPool_t* conn_pool);



#endif

