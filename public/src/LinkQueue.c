#include <stdlib.h>
#include "LinkQueue.h"

typedef struct _tag_LinkQueueNode TLinkQueueNode;
struct _tag_LinkQueueNode
{
    TLinkQueueNode* next;
    void* item;
};

typedef struct _tag_LinkQueue
{
    TLinkQueueNode* front;
    TLinkQueueNode* rear;
    int length;
} TLinkQueue;


//创建链表队列
//输入：无
//输出：链表队列头指针
LinkQueue* LinkQueue_Create() // O(1)
{
    TLinkQueue* ret = (TLinkQueue*)malloc(sizeof(TLinkQueue));
    
    if( ret != NULL )
    {
        ret->front = NULL;
        ret->rear = NULL;
        ret->length = 0;
    }
    
    return ret;
}

//销毁链表队列
//输入：链表队列头指针
//输出：空
void LinkQueue_Destroy(LinkQueue* queue) // O(n)
{
    LinkQueue_Clear(queue);
    free(queue);
}

//清空链表队列
//输入：链表队列头指针
//输出：空
void LinkQueue_Clear(LinkQueue* queue) // O(n)
{
    while( LinkQueue_Length(queue) > 0 )
    {
        LinkQueue_Retrieve(queue);
    }
}

//往队列加入一个节点
//输入：链表队列头指针，节点指针，节点长度
//输出：0:1
int LinkQueue_Append(LinkQueue* queue, void* item) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    TLinkQueueNode* node = (TLinkQueueNode*)malloc(sizeof(TLinkQueueNode));
    int ret = (sQueue != NULL ) && (item != NULL) && (node != NULL);
    
    if( ret )
    {
        node->item = item;
        
        if( sQueue->length > 0 )
        {
            sQueue->rear->next = node;
            sQueue->rear = node;
            node->next = NULL;
        }
        else
        {
            sQueue->front = node;
            sQueue->rear = node;
            node->next = NULL;
        }
        
        sQueue->length++;
    }
    
    if( !ret )
    {
        free(node);
    }
    
    return ret;
}



//往队列插队一个节点
//输入：链表队列头指针，节点指针，节点长度
//输出：0:1
int LinkQueue_Jump(LinkQueue* queue, void* item) // O(1)
{
	TLinkQueue* sQueue = (TLinkQueue*)queue;
	TLinkQueueNode* node = (TLinkQueueNode*)malloc(sizeof(TLinkQueueNode));

    int ret = (sQueue != NULL ) && (item != NULL) && (node != NULL);

    if( ret )
    {
        node->item = item;

        if( sQueue->length > 0 )
        {
            node->next = sQueue->front;
            sQueue->front = node;
        }
        else
        {
            sQueue->front = node;
            sQueue->rear = node;
            node->next = NULL;
        }

        sQueue->length++;

    }

    if( !ret )
    {
        free(node);
    }
    return ret;
}





//从队列读取一个节点
//输入：链表队列头指针
//输出：0,1
void* LinkQueue_Retrieve(LinkQueue* queue) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    TLinkQueueNode* node = NULL;
    void* ret = NULL;
    
    if( (sQueue != NULL) && (sQueue->length > 0) )
    {
        node = sQueue->front;
        
        sQueue->front = node->next;
        
        ret = node->item;
        
        free(node);
        
        sQueue->length--;
        
        if( sQueue->length == 0 )
        {
            sQueue->front = NULL;
            sQueue->rear = NULL;
        }
    }
    
    return ret;
}

// 获取队列头指针
// 输入：队列指针
// 输出：头指针，错误NULL
void* LinkQueue_Header(LinkQueue* queue) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    void* ret = NULL;
    
    if( (sQueue != NULL) && (sQueue->length > 0) )
    {
        ret = sQueue->front->item;
    }
    
    return ret;
}

// 获取队列长度
// 输入：队列指针
// 输出：长度，错误NULL
int LinkQueue_Length(LinkQueue* queue) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    int ret = 0;
    
    if( sQueue != NULL )
    {
        ret = sQueue->length;
    }
    
    return ret;
}




