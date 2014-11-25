#include "stdlib.h"



// function pointer table to global deinitializer table //
extern _PVFV * pf_atexitlist;

// Maximum entries allowed in table //
extern unsigned max_atexitlist_entries;

// Current amount of entries in table //
extern unsigned cur_atexitlist_entries;




/*
===================================
	Initialize the deinitializer function ptr table
===================================
*/
void __cdecl _atexit_init(void)
{
    max_atexitlist_entries = 32;

	// Warning: Normally, the STDC will dynamically allocate this. Because we have no memory manager, just choose
	// a base address that you will never use for now
 //   pf_atexitlist =(_PVFV *)0x500000;

	pf_atexitlist =(_PVFV *)0x5000;
}
