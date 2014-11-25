#include <string.h>
#include <klog.h>

//
// Displays a string
//
void kputs(STRPTR str)
{
	if (!str)
		return;

	// Displays a string
	for (ULONG i = 0; i < strlen(str); i++)
		kputc(str[i]);
}

