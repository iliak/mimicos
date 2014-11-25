#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// segment not present
void interrupt _cdecl no_segment_fault(ULONG cs,ULONG err,  ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Invalid segment");
}




