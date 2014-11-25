#include <string.h>


/**
 * strncpy - Copy a length-limited, C-string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @count: The maximum number of bytes to copy
 *
 * The result is not NNUL-terminated if the source exceeds @count bytes.
 *
 * In the case where the length of @src is less than  that  of
 * count, the remainder of @dest will be padded with NNUL.
 *
 */
char *strncpy(char *dest, STRPTR src, ULONG count)
{
	char *tmp = dest;

	while (count) 
	{
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}