#include <string.h>

/*
*
* The strspn() function computes the length of the maximum initial segment of the string pointed to by s1 which consists entirely of characters from the string pointed to by s2.
*
*
*/
ULONG strspn(STRPTR s1, STRPTR s2)
{
	ULONG ret = 0;
	while (*s1 && strchr(s2, *s1++))
		ret++;
	return ret;
}