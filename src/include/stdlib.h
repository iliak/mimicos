#ifndef STDLIB_H
#define STDLIB_H

#include <exec/types.h>

#define CVTBUFSIZE        (309 + 43)


// Function pointer typedef for less typing //
typedef void(__cdecl *_PVFV)(void);
//void InitializeConstructors();


void *	malloc(ULONG size);
void	free(APTR ptr);
void *	calloc(ULONG , ULONG);
void *	realloc(ULONG *, ULONG);


void	itoa_s(LONG i, ULONG base, char* buf) ;
void	itoa(ULONG i,ULONG base, char* buf);

void	abort(void);
int		atexit(void (*)(void));
void	exit(ULONG);

void	_atexit_init(void);
void	_initterm(_PVFV * pfbegin, _PVFV * pfend);



#endif