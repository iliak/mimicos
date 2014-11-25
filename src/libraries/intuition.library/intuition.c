#include <exec\kernel.h>


#include <proto\intuition.h>


VOID intuition_init(VOID)
{
	kprintf("INTUIT", "intuition_init()\n");


	OpenWorkbench();
}