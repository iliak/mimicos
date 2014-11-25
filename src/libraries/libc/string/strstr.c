#include <string.h>

/*
* The strstr() function locates the ﬁrst occurrence in the string pointed to by s1 of the sequence of characters
* (excluding the terminating null character) in the string pointed to by s2. 
*
*
*/
char * strstr(STRPTR s1, STRPTR s2)
{
	ULONG n = strlen(s2);
	while (*s1)
		if (!memcmp((APTR)s1++, (APTR)s2, n))
			return (char *)(s1 - 1);
	return 0;
}