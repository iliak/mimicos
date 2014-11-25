#include <exec/types.h>
#include <hal.h>
#include "exception.h"

#include <proto\exec.h>


// divide by 0 fault
void _cdecl divide_by_zero_fault(ULONG eflags, ULONG cs, ULONG eip, ULONG other)
{
	_asm 
	{
		cli
		add esp, 12
		pushad
	}

//	kernel_panic("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] other: 0x%x", cs, eip, eflags, other);
	Alert(0L, "Divide by 0");
}
