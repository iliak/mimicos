#include <exec/lists.h>


void Enqueue(struct List *list, struct Node *node)
{
	if (!list || !node)
		return;

	struct Node *next = NULL;

	// Look through the list 
	for (next = list->Head->Succ ? list->Head : NULL;							// Find the head node
		next;
		next = (next && next->Succ && next->Succ->Succ) ? next->Succ : NULL)	// Find the next node
	{

		// if the NEXT node has lower prio than the new node, insert us before the next node
		if (node->Pri >= next->Pri)
		{
			// Same as insert but insert before instead of insert behind 
			node->Succ = next;
			node->Pred = next->Pred;

			next->Pred->Succ = node;
			next->Pred = node;

			// Done. We cannot simly break the loop because of the AddTail() below.	    
			return;
		}
	}

	//If no nodes were in the list or our node has the lowest prio, we add it as last node
	node->Succ = (struct Node *)&list->Tail;
	node->Pred = list->TailPred;

	list->TailPred->Succ = node;
	list->TailPred = node;

}


