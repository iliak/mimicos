#ifndef	EXEC_SEMAPHORES_H
#define	EXEC_SEMAPHORES_H
//
//	Definitions for locking functions.
//
//

#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/ports.h>
#include <exec/tasks.h>


// ***** SignalSemaphore ********************************************

// Private structure used by ObtainSemaphore() 
struct SemaphoreRequest
{
	struct MinNode		Link;
	struct Task *		Waiter;
};

// Signal Semaphore data structure 
struct SignalSemaphore
{
	struct Node				Link;
	WORD					NestCount;
	struct MinList			WaitQueue;
	struct SemaphoreRequest	MultipleLink;
	struct Task *			Owner;
	WORD					QueueCount;
};

// ***** Semaphore procure message (for use in V39 Procure/Vacate) ***
struct SemaphoreMessage
{
	struct Message				Message;
	struct SignalSemaphore	*	Semaphore;
};

#define	SM_SHARED		(1L)
#define	SM_EXCLUSIVE	(0L)








// ***** Semaphore (Old Procure/Vacate type, not reliable) **********

struct Semaphore	// Do not use these semaphores! 
{
	struct MsgPort	MsgPort;
	WORD			Bids;
};

#define sm_LockMsg mp_SigTask

#endif
