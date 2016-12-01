#ifndef _LIBFDMANAGER_H_
#define _LIBFDMANAGER_H_


#define FD_RECYCLE_TIME		21600	


typedef struct _tag_FdSetNode_t FdSetNode_t;
typedef struct _tag_FdSet_t FdSet_t;

struct _tag_FdSetNode_t{
	int fd;
	unsigned long alive_time;
	unsigned long uid;
	FdSetNode_t* next;
};

struct _tag_FdSet_t{
	FdSetNode_t* head;
	FdSetNode_t* tail;
	int node_num;
};



FdSet_t* FdSet_Creat(void);
void FdSet_Clear(FdSet_t* set);
void FdSet_Update(FdSet_t* set,int fd);
int FdSet_AddNode(FdSet_t* set,int fd, unsigned long uid);
void FdSet_DeleteNode(FdSet_t* set,int fd);
void FdSet_Destroy(FdSet_t* set,int fd);



#endif