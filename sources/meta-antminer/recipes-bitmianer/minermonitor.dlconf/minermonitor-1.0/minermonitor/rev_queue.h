#ifndef _REV_QUEUE_H
#define _REV_QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include "send_pkg.h"
#include "debug.h"
#define MAXQSIZE 9
typedef struct
{
	PKG_Format *base;
	int front;
	int rear;
}SqQueue;
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
