#ifndef	EXEC_NODES_H
#define	EXEC_NODES_H
//
//	Nodes & Node type identifiers.
//
//

#include <exec/types.h>



//
//  List Node Structure.  Each member in a list starts with a Node
struct Node 
{
    struct Node *	Succ;				// Pointer to next (successor) 
    struct Node *	Pred;				// Pointer to previous (predecessor) 
    UBYTE			Type;				// Type of the node
    BYTE			Pri;				// Priority, for sorting (+127 (highest) to -128 (lowest))
    STRPTR			Name;				// ID string, null terminated 
};	// Note: word aligned 



// minimal node -- no type checking possible 
struct MinNode 
{
	struct MinNode *	Succ;
	struct MinNode *	Pred;
};


//----- Node Types for LN_TYPE -----
#define NT_UNKNOWN			0		//
#define NT_TASK				1		// Exec task 
#define NT_INTERRUPT		2		//
#define NT_DEVICE			3		//
#define NT_MSGPORT			4		//
#define NT_MESSAGE			5		// Indicates message currently pending 
#define NT_FREEMSG			6		//
#define NT_REPLYMSG			7		// Message has been replied 
#define NT_RESOURCE			8		//
#define NT_LIBRARY			9		//
#define NT_MEMORY			10		//
#define NT_SOFTINT			11		// Internal flag used by SoftInits 
#define NT_FONT				12		//
#define NT_PROCESS			13		// AmigaDOS Process 
#define NT_SEMAPHORE		14		//
#define NT_SIGNALSEM		15		// signal semaphores 
#define NT_BOOTNODE			16		//
#define NT_KICKMEM			17		//
#define NT_GRAPHICS			18		//
#define NT_DEATHMESSAGE		19		//
#define	NT_SLAB				20		// Slab
#define	NT_FREE_RANGE		21		// Free range list of virtual memory
#define	NT_USED_RANGE		22		// Used range list of virtual memory
									//
#define NT_USER				254		// User node types work down from here 
#define NT_EXTENDED			255		//

#endif


