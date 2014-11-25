#include <string.h>
#include <exec/lists.h>


//
// Find a system list node with a given name
//
struct Node *FindName(struct List *list, STRPTR name)
{
	if (!list || !name)
		return NULL;


	// Look through the list
	struct Node *node = list->Head;

	while (node->Succ != NULL)
	{
		if (!strcmp(node->Name, name))
			break;

		node = node->Succ;
	}

	return node;
}
