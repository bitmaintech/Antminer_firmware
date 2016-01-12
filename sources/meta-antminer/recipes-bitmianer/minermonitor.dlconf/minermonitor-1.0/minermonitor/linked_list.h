#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED
//
#include "send_pkg.h"
#include "debug.h"
#define MAXQSIZE 20// The maximum length

typedef struct PKG_Format * Item;//Define the type of data items
typedef struct Node * PNode;//The pointer of the node
typedef struct Node//The struct of the node
{
    Item item;
    PNode next;

}Node;

typedef  PNode Position;
typedef  PNode List;

List L_cmd;
int L_cmd_len;
//Generate an empty list
List MakeEmpty(List L);

//Determines if the list is empty
int IsEmpty(List L);

//Determine whether the end nodes
int IsLast(Position P);

//Find a node
Position Find(Item X,List L);

//Delete a node
void Delete(Item X,List L);

//Find out the previous node
Position FindPrevious(Item X,List L);

//Find the next node
Position FindNext(Item X,List L);

//Insert a node at the P position in the list
void Insert(Item X,List L,Position P);

//Delete the list and all the node in it
void DeleteList(List L);

//Get the position of the head node
Position Header(List L);

//Get the first data node of the list
Position First(List L);

//Get the next node
Position Advance(Position P);

//Get the data of the node
Item Retrieve(Position P);

#endif // LINKED_LIST_H_INCLUDED
