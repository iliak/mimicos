#include <exec/types.h>
#include <hal.h>
#include <exec\kernel.h>
#include <proto\exec.h>

#include "exception.h"

// invalid Task State Segment(TSS)
void interrupt _cdecl invalid_tss_fault(ULONG cs,ULONG err,  ULONG eip, ULONG eflags) 
{



	intstart();
	kprintf("EXEC", "Invalid TSS !");
	Alert(0L, "Invalid TSS");
}



