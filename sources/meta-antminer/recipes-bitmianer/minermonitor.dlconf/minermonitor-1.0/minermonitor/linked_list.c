#include<malloc.h>
#include<stdlib.h>
#include"linked_list.h"


/*
List MakeEmpty(List L)
parameter
L :the list
return
return the list generated
function
Generate an empty list
*/
List MakeEmpty(List L)
{
    Position temp;
    temp = malloc(sizeof(Node));
    if(temp==NULL)
        exit(0);
    temp->next = NULL;
    L_cmd_len = 0;
    L_cmd = temp;
    return L_cmd;
}

/*
int IsEmpty(List L)
parameter
L :the list
return
if the list is null return 1,else return 0
function
Determines if the list is empty
*/
int IsEmpty(List L)
{
    return L->next == NULL;
}
/*
int IsLast(Position P)
parameter
P :the position
return
if it's the last node return 1 ,else return 0
function
Determine whether the end nodes
*/
int IsLast(Position P)
{
    return P->next == NULL;
}

/*
Position Find(Item X,List L)
parameter
X :the item
L :the list
return
return the first node which item is X if exist
function
Find a node which item is X
*/
Position Find(Item X,List L)
{
    Position P;
    P = L->next;//P->item != X
    while( P!=NULL && strcmp(P->item->pkg_nonce,X->pkg_nonce) != 0 )
    {
        P = P->next;
    }
    return P;
}
/*
void Delete(Item X,List L)
parameter
X :the item
L :the list
return
none
function
Delete the node which item is same as X
*/
void Delete(Item X,List L)
{
    Position P,temp;
    P = FindPrevious(X,L);
    if(!IsLast(P))
    {
        temp = P->next;
        P->next = temp->next;

        DEBUG_printf("delete one cmd:%s,nonce:%s.\n",temp->item->pkg_cmd,temp->item->pkg_nonce);
        free(temp->item);
        free(temp);
        L_cmd_len --;
    }
    else
        DEBUG_printf("there is no cmd for this response.\n");
}
/*
Position FindPrevious(Item X,List L)
parameter
X :the item
L :the list
return
return the previous node if X is exist in the list
function
Find out the previous node
*/
Position FindPrevious(Item X,List L)
{
    Position P;
    P = L;//P->next->item != X
    while(P->next!=NULL && strcmp(P->next->item->pkg_nonce,X->pkg_nonce) != 0)
        P = P->next;
    return P;
}
/*
Position FindNext(Item X,List L)
parameter
X :the item
L :the list
return
return the next node if the list is exist
function
Find the next node
*/
Position FindNext(Item X,List L)
{
    Position P;
    P = L;
    while(P!=NULL && P->item != X)
        P = P->next;
    return P;
}
/*
void Insert(Item X,List L,Position P)
parameter
X :the item
L :the list
return
none
function
Insert a node at the P position in the list
*/
void Insert(Item X,List L,Position P)
{
    Position temp;
    temp = malloc(sizeof(Node));
    if(temp==NULL)
        exit(0);

    PKG_Format *tempdata = (PKG_Format*)malloc(sizeof(PKG_Format));
//
    strcpy((*tempdata).pkg_cmd,X->pkg_cmd);
    strcpy((*tempdata).pkg_nonce,X->pkg_nonce);
    temp->item = tempdata;
    temp->next = P->next;
    P->next = temp;
    L_cmd_len++;
    if(L_cmd_len > MAXQSIZE)
    {
        printf("L_cmd is full!\n");
    }
}
/*
void DeleteList(List L)
parameter
L :the list
return
none
function
Delete the list and all the node in it
*/
void DeleteList(List L)
{
    Position P,temp;
    P = L->next;
    L->next = NULL;
    while( P!=NULL)
    {
        temp = P->next;
        free(P->item);
        free(P);
         P = temp;
    }
    free(L);
}
/*
Position Header(List L)
parameter
L :the list
return
return the position of the head node
function
Get the position of the head node
*/
Position Header(List L)
{
    return L;
}
/*
Position First(List L)
parameter
L :the list
return
return the first data node if the list is not null
function
Get the first data node of the list
*/
Position First(List L)
{
    if(L->next!=NULL)
    return L->next;
}
/*
Position Advance(Position P)
parameter
P :current position
return
the position of the next node if exist,
function
Get the position of the next node
*/
Position Advance(Position P)
{
    if(P!=NULL)
    return P->next;
}
/*
Item Retrieve(Position P)
parameter
P :the location of current node
return
the value of it's item if P is not null
function
Get the data of the node
*/
Item Retrieve(Position P)
{
    if(P!=NULL)
    return P->item;
}
