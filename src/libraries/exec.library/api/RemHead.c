#include <exec/lists.h>

//
//
//
struct Node *RemHead(struct List *list)
{
	if (!list)
		return NULL;

    // Get the address of the first node or NULL
	struct  Node * node = list->Head->Succ;
    if (node)
    {
		node->Pred = (struct Node *)list;
		node = list->Head;
		list->Head = node->Succ;
    }

    return node;
}

