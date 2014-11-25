#ifndef	EXEC_DEVICES_H
#define	EXEC_DEVICES_H
//
//
//	Include file for use by Exec device drivers
//
//

#include <exec/libraries.h>
#include <exec/ports.h>
#include <exec/IO.H>


// ***** Device *****************************************************

struct Device
{
	struct Library		Library;
	ULONG				(*callback)(struct IORequest *);			// Callback function
};


// ***** Unit *******************************************************

struct Unit
{
	struct MsgPort	MsgPort;			// queue for unprocessed messages instance of msgport is recommended
	UBYTE			flags;             //
	UBYTE			pad;               //
	UWORD			OpenCnt;			// number of active opens
};


#define UNITF_ACTIVE	(1<<0)
#define UNITF_INTASK	(1<<1)

#endif	

