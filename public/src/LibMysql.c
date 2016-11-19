#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdarg.h>
#include "LibMysql.h"
#include "LibLoger.h"



static int InitMysqlPoolNode(dbConnNode_t* conn_node, char* host, char* username, char* password, char* db_name)
{
	char time_out;
	
	if(mysql_init(&(conn_node->conn)) == NULL)
	{
		error("mysql init error!\n");
		return -1;
	}
	
	if(mysql_real_connect(&(conn_node->conn),host,username,password,db_name,0,NULL,0) == NULL)
	{
		error("mysql connect error!\n");
		return -1;
	}
	time_out = 3;
	mysql_options(&(conn_node->conn), MYSQL_OPT_RECONNECT, (char *)&time_out);
	
	return 1;
}


dbConnPool_t* CreatMysqlConnPool(int conn_num)
{
	int i;
	dbConnPool_t* conn_pool = (dbConnPool_t*)malloc(sizeof(dbConnPool_t));
	if(conn_pool == NULL)
	{
		error("CreatMysqlConnPool error!\n");
		return NULL;
	}

	conn_pool->conn_num = conn_num;
	conn_pool->conn_node_arry = (dbConnNode_t*)malloc(conn_pool->conn_num*sizeof(dbConnNode_t));
	if(conn_pool->conn_node_arry == NULL)
	{
		free(conn_pool);
		error("CreatMysqlConnPool Node error!\n");
		return NULL;
	}
	pthread_mutex_init(&conn_pool->pool_lock, NULL);
	
	
	for(i=0;i<conn_pool->conn_num;i++)
	{
		info("init mysql connect pool the %d node",i);
		pthread_mutex_init(&conn_pool->conn_node_arry[i].conn_lock, NULL);
		if(InitMysqlPoolNode(&conn_pool->conn_node_arry[i],HOST_IP,USERNAME,PASSWORD,DB_NAME) == -1)
		{
			int j;
			error("init node %d error! ready to close the connected\n", i);
			for(j=0;j<i;j++)
			{
				info("close the %d connected!\n", j);
				mysql_close(&(conn_pool->conn_node_arry[j].conn)); 
			}
			info("close done and cleanning malloc\n");
			free(conn_pool->conn_node_arry);
			conn_pool->conn_node_arry = NULL;
			free(conn_pool);
			conn_pool = NULL;
			return NULL;
		}
		info("init the %d connnet ok!\n", i);
	}
	
	info("creat mysql connect pool done!\n");
	
	return conn_pool;
}


dbConnNode_t* GetMysqlConnNode(dbConnPool_t* conn_pool)
{
	int search_index = 0;
	int i;
	srand((int) time(0));
	search_index = rand() % conn_pool->conn_num;
	
	pthread_mutex_lock(&conn_pool->pool_lock);
	
	for(i = 0;i < conn_pool->conn_num; i++)
	{
		if((pthread_mutex_trylock(&conn_pool->conn_node_arry[search_index].conn_lock) == 0)||
			(conn_pool->conn_node_arry[search_index].is_buzy == 0))
		{
			if(0 != mysql_ping(&(conn_pool->conn_node_arry[search_index].conn)))
			{
				error("mysql ping error!\n");
				ReleaseMysqlConnNode(&(conn_pool->conn_node_arry[search_index]));
			}
			else
			{
				if(conn_pool->conn_node_arry[search_index].is_buzy == 0)
				{
					conn_pool->conn_node_arry[search_index].is_buzy = 1;
					info("get mysql connect %d!\n", search_index);
					
					pthread_mutex_unlock(&conn_pool->pool_lock);
					return &(conn_pool->conn_node_arry[search_index]);
				}
				else
				{
					error("mysql connect %d is busy but never lock!\n",search_index);
				}
			}
		}
		
		if((++search_index) >= conn_pool->conn_num)
			search_index = 0;
	}

	error("mysql connect pool used out!\n");
	pthread_mutex_unlock(&conn_pool->pool_lock);
	return NULL;
}


void ReleaseMysqlConnNode(dbConnNode_t* conn_node)
{
	pthread_mutex_unlock(&conn_node->conn_lock);
	conn_node->is_buzy = 0;
}


//不需要，已设置为自动重连
int ReConnectMysqlConnNode(dbConnNode_t* conn_node)
{
	return 1;
}

void DestoyedMysqlConnPool(dbConnPool_t* conn_pool)
{
	int i;
	info("destroyed the mysql conn pool!\n");
	pthread_mutex_lock(&conn_pool->pool_lock);
	for(i = 0; i < conn_pool->conn_num; i++)
	{
		pthread_mutex_lock(&(conn_pool->conn_node_arry[i].conn_lock));
		info("close the %d connected!\n", i);
		mysql_close(&(conn_pool->conn_node_arry[i].conn)); 
		pthread_mutex_unlock(&(conn_pool->conn_node_arry[i].conn_lock));
	}
	info("close done and cleanning malloc\n");
	
	pthread_mutex_unlock(&conn_pool->pool_lock);
	free(conn_pool->conn_node_arry);
	conn_pool->conn_node_arry = NULL;
	free(conn_pool);
	conn_pool = NULL;
}



int MysqlExcuteQuery(dbConnNode_t* conn_node, char* query_str,...)
{
	char sql[1024];
	va_list args;  
    va_start(args,query_str);  
    vsprintf(sql,query_str,args);  
    va_end(args);  
	info("excute sql: %s\n",sql);
	if(mysql_real_query(&conn_node->conn,sql,strlen(sql)))
	{
		error("mysql query error!\n");
		return -1;
	}
	
	return 1;
}








