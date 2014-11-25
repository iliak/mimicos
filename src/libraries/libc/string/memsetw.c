#include <string.h>


// sets count bytes of dest to val
UWORD * memsetw(UWORD *dest, UWORD val, ULONG count)
{
    UWORD *temp =(UWORD *)dest;
    for(; count != 0; count--)
		*temp++ = val;

	return dest;
}

