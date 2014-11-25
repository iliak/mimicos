#ifndef _STRING_H
#define _STRING_H

#include <exec/types.h>


//
// String manipulation
//
char *	strcpy(char *s1, STRPTR s2);
char *	strncpy(char *dest, STRPTR src, ULONG count);
char *	strcat(char *dest, STRPTR src);
char *	strncat(char *dest, STRPTR src, ULONG count);
ULONG	strxfrm(char *dest, STRPTR src, ULONG n);


//
// String examination
//
unsigned int	strlen(STRPTR str);
ULONG	strnlen(STRPTR s, ULONG maxlen);
int		strcmp(STRPTR str1, STRPTR str2);
int		strncmp(STRPTR cs, STRPTR *ct, ULONG count);
LONG	strcoll(STRPTR s1, STRPTR s2);
char *	strchr(STRPTR str, LONG character);
char *	strrchr(STRPTR s, int c);
ULONG	strspn(STRPTR s1, STRPTR s2);
ULONG	strcspn(STRPTR s1, STRPTR s2);
char *	strpbrk(STRPTR s1, STRPTR s2);
char *	strstr(STRPTR s1, STRPTR s2);
char *	strtok(char * str, STRPTR delim);

//
// Memory manipulation
//
APTR	memcpy(APTR dest, const APTR src, ULONG count);
APTR	memset(APTR dest, ULONG val, ULONG count);
UWORD *	memsetw(UWORD *dest, UWORD val, ULONG count);
APTR	memmove(APTR dest, const APTR src, ULONG count);
int		memcmp(APTR cs, APTR ct, ULONG count);
APTR	memchr(const APTR s, LONG c, ULONG n);

#endif
