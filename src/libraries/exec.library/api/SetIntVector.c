#include <exec/interrupts.h>
#include <Hal.h>

#include <exec\kernel.h>

//
// This function provides a mechanism for setting the system interrupt vectors.
//
// number : Interrupt number
// handler : An InterruptHandler structure containing the handler's entry point
//
void SetIntVector(ULONG number, InterruptHandler handler)
{
	kprintf("EXEC", "Set interrupt vector %2i to %#010P\n", number, handler);

	setvect(number, handler, 0);
}