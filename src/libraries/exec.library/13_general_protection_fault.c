#include <exec/types.h>
#include <hal.h>
#include <exec/console.h>
#include <exec\kernel.h>

#include <proto\exec.h>

#include "exception.h"

// general protection fault
void interrupt _cdecl general_protection_fault(ULONG cs,ULONG err, ULONG eip, ULONG eflags)
{

	intstart();
	
	console_printf("General protection fault !!\n");
	kprintf(NULL, "General protection fault !!\n");


	// Bit 0
	if((err && 0x1) == 1)
		console_printf("page was present\n");
	else
		console_printf("page was not present\n");

	// Bit 1
	if((err && 0x2) == 1)
		console_printf("was a read\n");
	else
		console_printf("was a write\n");

	// Bit 2
	if((err && 0x4) == 1)
		console_printf("ring 0\n");
	else
		console_printf("ring 3\n");

	// Bit 3
	if((err && 0x8) == 1)
		console_printf("did not occure because reserved bits were written over\n");
	else
		console_printf("occured because reserved bits were written over\n");

	// Bit 4
	if((err && 0x10) == 1)
		console_printf("did not occure during an instruction fetch\n");
	else
		console_printf("occure during an instruction fetch\n");


	Alert(0L, "General Protection Fault");

}



