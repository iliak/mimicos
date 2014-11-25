#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// overflow
void interrupt _cdecl overflow_trap(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Overflow trap");
}



