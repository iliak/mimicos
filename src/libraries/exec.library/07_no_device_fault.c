#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// device not available
void interrupt _cdecl no_device_fault(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Device not found");
}



