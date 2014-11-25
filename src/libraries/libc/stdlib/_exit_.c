#include "stdlib.h"



// function pointer table to global deinitializer table //
extern _PVFV * pf_atexitlist;

// Maximum entries allowed in table //
extern unsigned max_atexitlist_entries;

// Current amount of entries in table //
extern unsigned cur_atexitlist_entries;




/*
===================================
	Shutdown the CRT, and execute all global dtors.
===================================
*/
void _cdecl _Exit_()
{

	// Go through the list, and execute all global exit routines
	while(cur_atexitlist_entries--)
	{

			// execute function
			(*(--pf_atexitlist))();
	}
}

