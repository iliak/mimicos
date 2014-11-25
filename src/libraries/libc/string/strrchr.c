#include <string.h>



/*
* The strrchr() function locates the last occurrence of c (converted to a char) in the string pointed to by s.
* The terminating null character is considered to be part of the string.
*
*
*
*/
char * strrchr(STRPTR s, int c)
{
	char* ret = 0;
	do
	{
		if (*s == (char)c)
			ret = (char *)s;
	}
	while (*s++);

	return ret;
}