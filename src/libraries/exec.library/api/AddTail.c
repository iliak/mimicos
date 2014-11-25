
#include <exec/lists.h>


//
//
//
void AddTail(struct List *list, struct Node *node)
{
	if (!list || !node)
		return;

	//Make the node point to the head of the list. Our predecessor is the
	//previous last node of the list.
    node->Succ	       = (struct Node *)&list->Tail;
    node->Pred	       = list->TailPred;

	//Now we are the last now. Make the old last node point to us
	//and the pointer to the last node, too.
    list->TailPred->Succ = node;
    list->TailPred	     = node;

}
