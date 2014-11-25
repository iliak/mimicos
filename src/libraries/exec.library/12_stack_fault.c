#include <exec/types.h>
#include <hal.h>
#include "exception.h"
#include <exec\kernel.h>
#include <proto\exec.h>

// stack fault
void interrupt _cdecl stack_fault(ULONG cs,ULONG err, ULONG eip, ULONG eflags)
{
	intstart();
	kprintf("EXEC", "Stack fault");

	Alert(0L, "Stack fault");
}



