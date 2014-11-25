#include <stdio.h>
#include <hal.h>
#include <exec/exec.h>
#include <exec/kernel.h>
#include <exec\cpu.h>
#include <proto/exec.h>
//#include <macros.h>

#include "exception.h"

STRPTR arg[] =
{
	"Kernel tried to",
	"User tried to",
	"read a",
	"write to a",
	"non-present page entry",
	"page and caused a protection fault",
};


struct x86_reg regs;

// page fault
void _cdecl page_fault(ULONG err, ULONG eflags, ULONG cs, ULONG eip)
{
	save_regs(regs);

	APTR faultAddr = 0;

	// Get the faulting address from CR2 register
	_asm
	{
		cli
		sub	ebp, 4
		mov eax, cr2
		mov	[faultAddr], eax
	}

	kprintf(NULL, "\n");
	kprintf("INT14", "err=%#010X, eflags=%#010X, CS=%#010X, EIP=%#010X\n", err, eflags, cs, eip);
	kprintf("", "Fault address %#010P\n", faultAddr);

	dump_regs(&regs);

	// While not handling this interrupt...
	Alert(AT_DeadEnd | AN_PageFault, "page_fault : %s %s %s", arg[CHECK_BIT(err, 2) ? 1 : 0], arg[CHECK_BIT(err, 1) ? 3 : 2], arg[CHECK_BIT(err, 0) ? 5 : 4]);


	Disable();
	Halt();

	_asm
	{
		popad
		sti
		iretd
	}

}
