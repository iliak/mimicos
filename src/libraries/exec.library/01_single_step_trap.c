#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// single step
void interrupt _cdecl single_step_trap(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Single step");
}
