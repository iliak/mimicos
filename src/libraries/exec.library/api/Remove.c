#include <exec/lists.h>


void Remove(struct Node *node)
{
	if (!node)
		return;

	node->Pred->Succ = node->Succ;
    node->Succ->Pred = node->Pred;
}

