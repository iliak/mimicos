#include <exec\kernel.h>

#include <string.h>

#include <proto\exec.h>
#include <proto\intuition.h>
#include <proto\graphics.h>


//
// Open an Intuition screen
//
struct Screen *  OpenScreen(struct NewScreen *newScreen)
{
#ifdef _VERBOSE
	kprintf("INTUIT", "OpenScreen()\n");
#endif

	if (newScreen == NULL)
		return NULL;


	kprintf("INTUIT", "OpenScreen() : width: %i height: %i depth: %i\n", newScreen->Width, newScreen->Height, newScreen->Depth);


	// Allocate enough space for a Screen structure
	kprintf("INTUIT", "Allocating Screen structure: %i octets\n", sizeof(struct Screen));
	//struct Screen *screen = (struct Screen *)AllocMem(sizeof(struct Screen), MEMF_CLEAR);
	struct Screen *screen = kmalloc(sizeof(struct Screen));
	if (screen == NULL)
	{
		kprintf("INTUIT", "OpenScreen(): Failed to allocate a new Screen structure.\n");
		return NULL;
	}
	memset(screen, 0, sizeof(struct Screen));

	// Allocate Video buffer
	ULONG size = newScreen->Width * newScreen->Height * (newScreen->Depth) / 8;
	kprintf("INTUIT", "Allocating ViewPort: %i octets\n", size);

	//screen->ViewPort.Data = AllocMem(size, MEMF_CLEAR);
	screen->ViewPort.Data = kmalloc(size);
	if (screen->ViewPort.Data == NULL)
	{
		kprintf("INTUIT", "OpenScreen(): Failed to allocate ViewPort data for a new Screen structure.\n");
		kfree(screen);
		return NULL;
	}
	screen->ViewPort.Width = newScreen->Width;
	screen->ViewPort.Height = newScreen->Height;
	screen->ViewPort.Depth = newScreen->Depth;

	ClearScreen();

	// Brings the screen to the front
	ScreenToFront(screen);


	return screen;
}