#include <string.h>


// copies count bytes from src to dest
/**
* Copy one area of memory to another
* @dest: Where to copy to
* @src: Where to copy from
* @count: The size of the area.
*
*/

#pragma function(memcpy)

APTR	memcpy(APTR dest, const APTR src, ULONG count)
{
	const char *sp = (const char *)src;
	char *dp = (char *)dest;
	for (; count != 0; count--) *dp++ = *sp++;
	return dest;
}

