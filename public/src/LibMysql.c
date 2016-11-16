#include <stdio.h>




static InitMysqlPoolNode(dbConnNode_t* conn_node, char* host, char* username, char* password, char* db_name)
{
	if(mysql_init(&(conn_node->conn)) == NULL)
	{
		
		
	}
	
}





dbConnPool_t* CreatMysqlConnPool(int conn_num)
{
	
	dbConnPool_t* conn_pool = malloc(sizeof(dbConnPool_t));
	if(conn_pool == NULL)
	{
		error("CreatMysqlConnPool error");
		return NULL;
	}

	conn_pool->conn_num = conn_num;
	conn_pool->conn_node_info = malloc(conn_num*sizeof(dbConnNode_t));
	if(conn_pool->conn_node_info == NULL)
	{
		free(conn_pool);
		error("CreatMysqlConnPool Node error");
		return NULL;
	}
	
	
	
	
	
	
	return conn_pool;
}


dbConnNode_t* GetMysqlConnNode(void);






















