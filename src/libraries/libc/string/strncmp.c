#include <string.h>

/**
 * strncmp - Compare two length-limited strings
 * cs: One string
 * ct: Another string
 * count: The maximum number of bytes to compare
 */
LONG strncmp(STRPTR cs, STRPTR *ct, ULONG count)
{
	UBYTE c1, c2;

	while (count)
	{
		c1 = *cs++;
		c2 = (UBYTE)*ct++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		
		if (!c1)
			break;
		
		count--;
	}
	return 0;
}