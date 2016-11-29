#ifndef _LIBFDMANAGER_H_
#define _LIBFDMANAGER_H_


typedef struct _tag_FdSetNode_t FdSetNode_t;
typedef struct _tag_FdSet_t FdSet_t;

typedef struct {
	int fd;
	int alive;
	unsigned long uid;
	FdSetNode_t* next;
} _tag_FdSetNode_t;

typedef struct {
	FdSetNode_t* head;
	int nodeNum;
} _tag_FdSet_t;



FdSet_t* FdSet_Creat(void);
void FdSet_Clear(FdSet_t* set);
void FdSet_Update(FdSet_t* set,int fd);
int FdSet_AddNode(FdSet_t* set,int fd);
int FdSet_DeleteNode(FdSet_t* set,int fd);
int FdSet_Destroy(FdSet_t* set,int fd);



#endif