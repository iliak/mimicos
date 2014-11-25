#include <stdio.h>
#include <ctype.h>



/*
* Convert a string to an unsigned long integer.
*
*/
ULONG strtoul(STRPTR nptr, STRPTR * endptr, LONG base)
{
	const char *s = nptr;
	ULONG acc;
	LONG c;
	ULONG cutoff;
	LONG neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do 
	{
		c = *s++;
	} while(isspace(c));
	
	if(c == '-')
	{
		neg = 1;
		c = *s++;
	} 
	else if(c == '+')
		c = *s++;
	if((base == 0 || base == 16) && c == '0' &&(*s == 'x' || *s == 'X')) 
	{
		c = s[1];
		s += 2;
		base = 16;
	} 
	else if((base == 0 || base == 2) && c == '0' &&(*s == 'b' || *s == 'B')) 
	{
		c = s[1];
		s += 2;
		base = 2;
	}

	if(base == 0)
		base = c == '0' ? 8 : 10;

	cutoff =(unsigned long)ULONG_MAX /(unsigned long)base;
	cutlim =(unsigned long)ULONG_MAX %(unsigned long)base;

	for(acc = 0, any = 0;; c = *s++) 
	{
		if(isdigit(c))
			c -= '0';
		else if(isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if(c >= base)
			break;
		if(any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
			any = -1;
		else 
		{
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if(any < 0)
	{
		acc = ULONG_MAX;
//		errno = ERANGE;
	} 
	else if(neg)
		acc = -acc;

	if(endptr != 0)
		*endptr =(char *)(any ? s - 1 : nptr);

	return(acc);
}

