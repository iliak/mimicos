#include <string.h>

//
// The strlen() function shall compute the number of bytes in the string to which s points, not including the terminating null byte.
//
#pragma function(strlen)

ULONG strlen(STRPTR str)
{
	STRPTR p = str;

	while (*str) 
		++str;
	return str - p;

}
