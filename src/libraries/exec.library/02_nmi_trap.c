#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// non maskable interrupt trap
void interrupt _cdecl nmi_trap(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "NMI trap");
}




