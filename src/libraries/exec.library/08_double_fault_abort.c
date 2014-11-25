#include <exec/types.h>
#include <hal.h>
#include <proto\exec.h>
#include "exception.h"

// double fault
void interrupt _cdecl double_fault_abort(ULONG cs, ULONG err, ULONG eip, ULONG eflags) 
{
	intstart();

	Alert(0L, "Double fault");
}




