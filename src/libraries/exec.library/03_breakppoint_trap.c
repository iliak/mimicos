#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>

// breakpoint hit
void interrupt _cdecl breakpoint_trap(ULONG cs,ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Breakpoint trap");
}




