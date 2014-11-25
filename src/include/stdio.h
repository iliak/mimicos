#ifndef _STDIO_H
#define _STDIO_H

#include <stdarg.h>
#include <exec\types.h>


LONG	printf(STRPTR format, ...);
LONG	nprintf(ULONG size, STRPTR format, ...);
LONG	nvprintf(ULONG size, STRPTR format, va_list ap);

LONG	vsprintf(char *str, STRPTR format, va_list ap);

/*
LONG vprintf(const char *format, va_list ap);
LONG vsnprintf(char *str, size_t size, const char *format, va_list ap);
LONG vasprintf(char **ret, const char *format, va_list ap);
*/




LONG	strtol(STRPTR nptr, STRPTR * endptr, LONG base);
ULONG	strtoul(STRPTR nptr, STRPTR * endptr, LONG base);
LONG	atoi(STRPTR str);


#endif
