#include <string.h>
#include <klog.h>
#include <stdio.h>

//
// Write to the console as well as to the logging facility
//
LONG kprintf(STRPTR tag, STRPTR fmt, ...)
{
	// No text to print
	if (fmt == NULL)
		return 0;

	// Prints the tag
	ULONG i = 0;
	if (tag != NULL)
	{
		kputs("[");
		if (tag)
		{
			for (i = 0; i < KERNEL_PRINT_TAG_LENGTH; i++)
			{
				if (tag[i] == 0)
					break;
				kputc(tag[i]);
			}
		}
		while (i++ < KERNEL_PRINT_TAG_LENGTH)
			kputc(' ');

		kputs("] ");
	}

	va_list args;
	char buffer[1024];
	int len;

	//printf("[%6s] ", tag);
	//kputs(buffer);

	va_start(args, fmt);
	len = vsprintf(buffer, fmt, args);
	va_end(args);
	kputs(buffer);
	return i + len;
}



