#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>

// bounds check
void interrupt _cdecl bounds_check_fault(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Bounds check fault");
}



