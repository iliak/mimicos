#include <exec/lists.h>


void AddHead(struct List *list, struct Node *node)
{
	if (!list || !node)
		return;

	// Make the node point to the old first node in the list and to the head of the list.
    node->Succ	   = list->Head;
    node->Pred	   = (struct Node *)&list->Head;


	//New we come before the old first node which must now point to us
	//and the same applies to the pointer to-the-first-node in the
	//head of the list.
    list->Head->Pred = node;
    list->Head	     = node;
}

