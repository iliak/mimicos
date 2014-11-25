#include <string.h>
#include <kserial.h>
#include <hal.h>






//
// Displays a character
//
void kputc(UBYTE c)
{
	// Do not output \0
	if (c == 0)
		return;

	// Debug Bochs
	outportb(BOCHSPORT, c);

	// Debug serial port
	kserial_write(c);
}

