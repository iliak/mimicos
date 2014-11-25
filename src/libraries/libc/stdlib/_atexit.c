#include "stdlib.h"



// function pointer table to global deinitializer table //
extern _PVFV * pf_atexitlist;

// Maximum entries allowed in table //
extern unsigned max_atexitlist_entries;

// Current amount of entries in table //
extern unsigned cur_atexitlist_entries;


/*
===================================
	Add entry into atexit deinitialzer table. Called by MSVC++ code.
===================================
*/
int __cdecl _atexit(_PVFV fn)
{
	// Insure we have enough free space
	if(cur_atexitlist_entries >= max_atexitlist_entries)
		return 1;
	else
	{
		// Add the exit routine
		*(pf_atexitlist++) = fn;
		cur_atexitlist_entries++;
	}

	return 0;
}

