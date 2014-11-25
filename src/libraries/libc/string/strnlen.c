#include <string.h>

/*
* Find the length of S, but scan at most MAXLEN characters.
*
*/
ULONG strnlen(STRPTR s, ULONG maxlen)
{
	STRPTR e;
	ULONG n;

	for (e = s, n = 0; *e && n < maxlen; e++, n++)
		;
	return n;
}