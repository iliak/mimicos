// Node.cpp : définit le point d'entrée pour l'application console.
//


// http://web.archive.org/web/20121008090116/http://www.jamesmolloy.co.uk/tutorial_html/7.-The%20Heap.html
// http://g.oswego.edu/dl/html/malloc.html

#include <exec/exec.h>
#include <proto/exec.h>


// Our nodes 
struct Node node1 =
{
    NULL, NULL,    // No predecessor and successor, yet 
    NT_UNKNOWN, 0, // Unknown type, priority ignored 
    "First node"   // Name of the node 
};

struct Node node2 =
{
    NULL, NULL,
    NT_UNKNOWN, 0,
    "Second node"
};

struct Node node3 =
{
    NULL, NULL,
    NT_UNKNOWN, 0,
    "Third node"
};

// http://amiga.sourceforge.net/amigadevhelp/phpwebdev.php?keyword=NewList&funcgroup=AmigaOS&action=Search

int main(int argc, char *argv[])
{
	struct List list;
    NewList(&list);
	
	
	struct Node *node;


	// Test if list is empty
	if (list.TailPred == (struct Node *)&list)
	{
		int i = 0;
		i = 1;
	}



    // Add the first two nodes at the end of the list. 
    AddTail(&list, &node1);
    AddTail(&list, &node2);

    // Insert the third node after the first node. 
    Insert(&list, &node3, &node1);

    // Find the second node 
    node = FindName(&list, "Second node");
    if (node)
        Remove(node);

    return 0;
}