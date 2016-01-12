#include "rev_queue.h"

int InitQueue(SqQueue *Q,SqQueue *Q_respond)
{
	(*Q).base=(PKG_Format *)malloc(MAXQSIZE*sizeof(PKG_Format));
	if(!(*Q).base)
		exit(0);
	(*Q).front=(*Q).rear=0;

	(*Q_respond).base=(PKG_Format *)malloc(MAXQSIZE*sizeof(PKG_Format));
	if(!(*Q_respond).base)
		exit(0);
	(*Q_respond).front=(*Q_respond).rear=0;
	return 1;
}

int DestroyQueue(SqQueue *Q)
{
	if((*Q).base)
		free((*Q).base);
	(*Q).base=NULL;
	(*Q).front=(*Q).rear=0;
	return 1;
}

int ClearQueue(SqQueue *Q)
{
	(*Q).front=(*Q).rear=0;
	return 1;
}

int QueueEmpty(SqQueue Q)
{
	if(Q.front==Q.rear)
		return 1;
	else
		return 0;
}

int QueueLength(SqQueue Q)
{
	return(Q.rear-Q.front+MAXQSIZE)%MAXQSIZE;
}

int GetHead(SqQueue Q,PKG_Format *e)
{
	if(Q.front==Q.rear)
		return 0;
	*e=*(Q.base+Q.front);
	return 1;
}

int EnQueue(SqQueue *Q,PKG_Format e)
{
	if(((*Q).rear+1)%MAXQSIZE==(*Q).front)
		return 0;
	(*Q).base[(*Q).rear]=e;
	(*Q).rear=((*Q).rear+1)%MAXQSIZE;
	return 1;
}

int DeQueue(SqQueue *Q,PKG_Format *e)
{
	if((*Q).front==(*Q).rear)
		return 0;
	*e=(*Q).base[(*Q).front];
	(*Q).front=((*Q).front+1)%MAXQSIZE;
	return 1;
}

int QueueTraverse(SqQueue Q,void(*vi)(PKG_Format))
{
	int i;
	i=Q.front;
	while(i != Q.rear)
	{
		vi(*(Q.base+i));
		i=(i+1) % MAXQSIZE;
	}
	DEBUG_printf("\n");
	return 1;
}


void visit(PKG_Format i)
{
	DEBUG_printf("first_2byte=%s  ser_cmd=%s ser_len=%s body=%s\n",i.pkg_head ,i.pkg_cmd,i.pkg_body_len,i.pkg_body);
}
int FUL_Queue(SqQueue *Q)
{
	if(((*Q).rear+1)%MAXQSIZE==(*Q).front)
	return 0;
	else
	return 1;
}

int NUL_Queue(SqQueue *Q)
{
	if((*Q).front==(*Q).rear)
	return 0;
	else
	return 1;
}



