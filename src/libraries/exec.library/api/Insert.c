#include <exec/lists.h>

//
//
//
void Insert(struct List *list, struct Node *node, struct Node *pred)
{
	if (!list || !node)
		return;

    if (pred)
    {
		// Our successor is the successor of the node we add ourselves
		// behind and our predecessor is just the node itself.
		node->Succ = pred->Succ;
		node->Pred = pred;

		//We are the predecessor of the successor of our predecessor
		pred->Succ->Pred = node;
		pred->Succ = node;
    }
    else
    {
		// add at the top of the list. 
		node->Succ	       = list->Head;
		node->Pred	       = (struct Node *)&list->Head;
		list->Head->Pred   = node;
		list->Head	       = node;
    }
}


