#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>

// invalid opcode / instruction
void interrupt _cdecl invalid_opcode_fault(ULONG cs, ULONG eip, ULONG eflags)
{

	intstart();
	Alert(0L, "Invalid opcode");

}




