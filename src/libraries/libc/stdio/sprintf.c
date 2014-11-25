#include <stdio.h>


/*
*
*
*
*/
LONG sprintf(char *buf, STRPTR fmt, ...)
{
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);

	return n;
}



