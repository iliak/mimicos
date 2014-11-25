#include <string.h>


// compare two strings
#pragma function(strcmp)

/**
*
*
*/
LONG strcmp(STRPTR str1, STRPTR str2)
{
	LONG res = 0;
	while (!(res = *(UBYTE *)str1 - *(UBYTE*)str2) && *str2)
		++str1, ++str2;

	if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;

	return res;
}

