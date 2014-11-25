#include <exec\kernel.h>
#include <intuition\intuition.h>

#include <proto\exec.h>
#include <proto\intuition.h>

#include <string.h>

//
//
//
ULONG OpenWorkbench(void)
{

	kprintf("INTUIT", "OpenWorkBench()\n");


	struct NewScreen * ns = kmalloc(sizeof(struct NewScreen));
	memset(ns, 0, sizeof(struct NewScreen));
	ns->Width = 1024;
	ns->Height = 768;
	ns->Depth = 32;

	struct Screen *screen = OpenScreen(ns);
	if (screen == NULL)
	{
		kprintf("INTUIT", "Failed to open Workbench !\n");
		return NULL;
	}

	return screen;
}