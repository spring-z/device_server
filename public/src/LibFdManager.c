#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "LibFdManager.h"



static FdSetNode_t* SearchFdNode(FdSet_t* set,int fd);


FdSet_t* FdSet_Creat(void)
{
	FdSet_t* fd_set;
	fd_set = (FdSet_t*)malloc(sizeof(FdSet_t));
	if(NULL == fd_set)
	{
		fd_set->head = NULL;
		fd_set->node_num = 0;
	}
		
	return fd_set;
}


void FdSet_Clear(FdSet_t* set)
{
	time_t t;
	FdSetNode_t* node = set->head;
	if(node == NULL)
		return;

	if((time(&t) - node->alive_time) > FD_RECYCLE_TIME)
	{
		free(node);
		set->head = NULL;
		set->node_num--;
	}

	if(node->next != NULL)
	{
		do
		{
			if((time(&t) - node->next->alive_time) > FD_RECYCLE_TIME)
			{
				FdSetNode_t* temp;
				temp = node->next->next;
				free(node->next);
				node->next = temp;
				set->node_num--;
			}
			else
			{
				node->next = node->next->next;
			}
		}
		while(node->next != NULL);
	}
}

void FdSet_Update(FdSet_t* set,int fd)
{
	time_t t;
	FdSetNode_t* node = SearchFdNode(set, fd);
	if(node != NULL)
	{
		node->alive_time = time(&t);
	}
}


int FdSet_AddNode(FdSet_t* set,int fd, unsigned long uid)
{
	time_t t;
	FdSetNode_t* node = SearchFdNode(set, fd);
	if(node != NULL)
	{
		node->alive_time = time(&t);
		node->uid = uid;
	}
	else
	{
		node = (FdSetNode_t*)malloc(sizeof(FdSetNode_t));
		if(node != NULL)
		{
			node->fd = fd;
			node->alive_time = time(&t);
			node->uid = uid;
			node->next = NULL;
			if(set->head == NULL)
			{
				set->head = node;
				set->tail = node;
				set->node_num++;
			}
			else
			{
				set->tail->next = node;
				set->tail = node;
				set->node_num++;
			}
		}
		else
		{
			return -1;
		}
	}
	
	return 1;
}


void FdSet_DeleteNode(FdSet_t* set,int fd)
{
	FdSetNode_t* node = set->head;
	if(node == NULL)
		return;

	if(node->fd == fd)
	{
		free(node);
		set->head = NULL;
		set->node_num--;
	}

	if(node->next != NULL)
	{
		do
		{
			if(node->next->fd == fd)
			{
				FdSetNode_t* temp;
				temp = node->next->next;
				free(node->next);
				node->next = temp;
				set->node_num--;
			}
			else
			{
				node->next = node->next->next;
			}
		}
		while(node->next != NULL);
	}
}


void FdSet_Destroy(FdSet_t* set,int fd)
{
	FdSetNode_t* free_node = set->head;
	FdSetNode_t* node = free_node->next;
	
	if(node == NULL)
	{
		free(set);
		set = NULL;
		return;
	}

	if(free_node == NULL)
	{
		free(set->head);
		free(set);
		set = NULL;
		return;
	}
	
	while(node != NULL)
	{
		free(free_node);
		free_node = node;
		node = node->next;
	}
	
	free(free_node);
	free(set);
	set=NULL;
}


static FdSetNode_t* SearchFdNode(FdSet_t* set,int fd)
{
	FdSetNode_t* ret = set->head;
	while(ret != NULL)
	{
		if(ret->fd == fd)
			break;
		ret = ret->next;
	}
	
	return ret;
}
















