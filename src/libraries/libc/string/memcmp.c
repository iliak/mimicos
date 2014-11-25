#include <string.h>


/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
#pragma function(memcmp)

ULONG memcmp(APTR cs, APTR ct, ULONG count)
{
	UBYTE *su1, *su2;
	int res = 0;

	for (su1 = (UBYTE *) cs, su2 = (UBYTE *) ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}