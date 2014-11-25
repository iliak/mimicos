#include <stdio.h>

// convert string to int
LONG atoi(const char * str)
{
	return(LONG) strtol(str, 0, 10);
}

