#include <string.h>

/*
*
* The strchr() function locates the ﬁrst occurrence of c (converted to a char) in the string pointed to by s.
* The terminating null character is considered to be part of the string. 
*
*
*/
char* strchr(STRPTR str, LONG character) 
{
	do 
	{
		if(*str == character)
			return(char*)str;
	}
	while(*str++);

	return 0;
}

