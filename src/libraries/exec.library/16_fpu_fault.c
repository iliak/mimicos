#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>

// Floating Point Unit(FPU) error
void interrupt _cdecl fpu_fault(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "FPU Fault");

}



