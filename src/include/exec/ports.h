#ifndef	EXEC_PORTS_H
#define	EXEC_PORTS_H
//
//
//	Message ports and Messages.
//
//

#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/tasks.h>


// ***** MsgPort ****************************************************

struct MsgPort
{
	struct Node	Node;
	UBYTE		Flags;
	UBYTE		SigBit;				// signal bit number	
	void   *	SigTask;				// object to be signalled 
	struct List	MsgList;				// message linked list	
};

#define mp_SoftInt mp_SigTask		// Alias 

// mp_Flags: Port arrival actions (PutMsg) 
#define PF_ACTION	3				// Mask 
#define PA_SIGNAL	0				// Signal task in mp_SigTask 
#define PA_SOFTINT	1				// Signal SoftInt in mp_SoftInt/mp_SigTask 
#define PA_IGNORE	2				// Ignore arrival 


// ***** Message ****************************************************

struct Message
{
	struct Node			Node;
	struct MsgPort *	ReplyPort;  		// message reply port 
	UWORD   			Length;		   	// total message length, in bytes 
	// (include the size of the Message 
	// structure in the length) 
};

#endif	// EXEC_PORTS_H 
