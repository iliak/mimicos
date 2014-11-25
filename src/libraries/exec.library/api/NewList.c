#include <exec/lists.h>

//
//
//
void NewList(struct List *list)
{
	if (!list)
		return;

	list->Head = (struct Node *)&list->Tail;
	list->Tail = NULL;
	list->TailPred = (struct Node *)&list->Head;
	list->Type = 0;
	list->pad = 0;
}

/*
//
//
//
void NewList(struct MinList *list)
{
	if (!list)
		return;

	list->Head = list->Tail;
	list->Tail = NULL;
	list->TailPred = list->Head;
}
*/
