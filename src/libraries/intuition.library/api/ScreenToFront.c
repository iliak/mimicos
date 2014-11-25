#include <exec\kernel.h>
#include <intuition\screens.h>

#include <proto\graphics.h>

//
// Make the specified screen the frontmost.
//
VOID ScreenToFront(struct Screen *screen)
{
	kprintf("INTUIT", "ScreenToFront()\n");

	if (screen == NULL)
		return;

	LoadViewPort(&screen->ViewPort);

}