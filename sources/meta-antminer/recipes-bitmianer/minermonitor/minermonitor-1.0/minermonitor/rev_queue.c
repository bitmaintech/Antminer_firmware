#include "rev_queue.h"

// 构造一个空队列Q
int InitQueue(SqQueue *Q,SqQueue *Q_respond)
{
	//给队头队尾指针分配空间，并置空
	(*Q).base=(PKG_Format *)malloc(MAXQSIZE*sizeof(PKG_Format));
	//这里的Q.base相当于一个数组头
	if(!(*Q).base) // 存储分配失败
		exit(0);
	(*Q).front=(*Q).rear=0;	//下标初始化为0

	//给队头队尾指针分配空间，并置空
	(*Q_respond).base=(PKG_Format *)malloc(MAXQSIZE*sizeof(PKG_Format));
	//这里的Q.base相当于一个数组头
	if(!(*Q_respond).base) // 存储分配失败
		exit(0);
	(*Q_respond).front=(*Q_respond).rear=0;	//下标初始化为0
	return 1;
}

// 销毁队列Q,Q不再存在
int DestroyQueue(SqQueue *Q)
{
	if((*Q).base)
		free((*Q).base);
	(*Q).base=NULL;
	(*Q).front=(*Q).rear=0;	//空队列的标志是队头队尾指针都相同，且为0
	return 1;
}

// 将Q清为空队列
int ClearQueue(SqQueue *Q)
{
	(*Q).front=(*Q).rear=0;	//空队列的标志是队头队尾指针都相同，且为0
	return 1;
}

// 若队列Q为空队列,则返回1,否则返回0
int QueueEmpty(SqQueue Q)
{
	if(Q.front==Q.rear) // 队列空的标志
		return 1;
	else
		return 0;
}

// 返回Q的元素个数,即队列的长度
int QueueLength(SqQueue Q)
{
	return(Q.rear-Q.front+MAXQSIZE)%MAXQSIZE;
}

// 若队列不空,则用e返回Q的队头元素,并返回1,否则返回0
int GetHead(SqQueue Q,PKG_Format *e)
{
	if(Q.front==Q.rear) // 队列空
		return 0;
	// *(Q.base+Q.front)相当于Q.base[Q.front],即Q.base是数
	// 组头，表示第Q.front个元素
	*e=*(Q.base+Q.front);
	return 1;
}

// 插入元素e为Q的新的队尾元素
int EnQueue(SqQueue *Q,PKG_Format e)
{
	if(((*Q).rear+1)%MAXQSIZE==(*Q).front) // 队列满
		return 0;
	(*Q).base[(*Q).rear]=e;
	(*Q).rear=((*Q).rear+1)%MAXQSIZE;
	return 1;
}

// 若队列不空,则删除Q的队头元素,用e返回其值,并返回1;否则返回0
int DeQueue(SqQueue *Q,PKG_Format *e)
{
	if((*Q).front==(*Q).rear) // 队列空
		return 0;
	*e=(*Q).base[(*Q).front];
	(*Q).front=((*Q).front+1)%MAXQSIZE;
	return 1;
}

// 从队头到队尾依次对队列Q中每个元素调用函数vi()
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
	if(((*Q).rear+1)%MAXQSIZE==(*Q).front) // 队列满
	return 0;
	else
	return 1;
}

int NUL_Queue(SqQueue *Q)
{
	if((*Q).front==(*Q).rear) // 队列空
	return 0;
	else
	return 1; //
}



