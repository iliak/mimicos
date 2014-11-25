#include <string.h>

//
// The memset function copies the value of c (converted to an UBYTE) into each of the ﬁrst n characters of the object pointed to by s. 
//
#pragma function(memset)
//#pragma intrinsic(memset)

APTR __cdecl memset(void *dest, ULONG val, ULONG count)
{
	UBYTE * p = (UBYTE *)dest;
	while (count--)
		*p++ = (UBYTE)val;
	return dest;

}


