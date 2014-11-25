#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>

//
// alignment check
//
void interrupt _cdecl alignment_check_fault(ULONG cs, ULONG err, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Alignment Check");
}



