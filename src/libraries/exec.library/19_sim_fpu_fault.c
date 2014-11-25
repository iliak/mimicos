#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>

// Floating Point Unit(FPU) Single Instruction Multiple Data(SIMD) error
void interrupt _cdecl simd_fpu_fault(ULONG cs, ULONG eip, ULONG eflags) 
{

	intstart();
	Alert(0L, "FPU SIMD fault");
}



