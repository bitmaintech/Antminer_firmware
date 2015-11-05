#ifndef _REV_QUEUE_H
#define _REV_QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include "send_pkg.h"
#include "debug.h"
#define MAXQSIZE 9// 最大队列长度(对于循环队列，最大队列长度要减1)
typedef struct
{
	PKG_Format *base;	// 初始化的动态分配存储空间，相当于一个数组头
	// 头指针,若队列不空,指向队列头元素，相当于一个下标
	int front;
	// 尾指针,若队列不空,指向队列尾元素的下一个位置，相当于一个下标
	int rear;
}SqQueue;//空队列的标志是队头队尾指针都相同
SqQueue Q_cmd;
SqQueue Q_respond;


int InitQueue(SqQueue *Q,SqQueue *Q_respond);
int DestroyQueue(SqQueue *Q);
int ClearQueue(SqQueue *Q);
int QueueEmpty(SqQueue Q);
int QueueLength(SqQueue Q);
int GetHead(SqQueue Q,PKG_Format *e);
int EnQueue(SqQueue *Q,PKG_Format e);
int DeQueue(SqQueue *Q,PKG_Format *e);
int QueueTraverse(SqQueue Q,void(*vi)(PKG_Format));
void visit(PKG_Format i);
int FUL_Queue(SqQueue *Q);
int NUL_Queue(SqQueue *Q);

#endif
