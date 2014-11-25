#include <exec/lists.h>


//
//
//
struct Node * RemTail(struct List *list)
{
	if (!list)
		return NULL;

    // Get the last node of the list
	struct Node * node = list->TailPred->Pred ? list->TailPred : NULL;
    if (node)
    {
		node->Pred->Succ = node->Succ;
		node->Succ->Pred = node->Pred;
    }

    return node;
}



