#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// machine check
void interrupt _cdecl machine_check_abort(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Machine Check");
}



