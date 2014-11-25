#include "stdlib.h"



// function pointer table to global deinitializer table //
extern _PVFV * pf_atexitlist;

// Maximum entries allowed in table //
extern unsigned max_atexitlist_entries;

// Current amount of entries in table //
extern unsigned cur_atexitlist_entries;



/*
===========================
	Initialize global initializaters(Global constructs, et al)
===========================
*/
void __cdecl _initterm(_PVFV * pfbegin, _PVFV * pfend)
{
	// Go through each initializer
    while(pfbegin < pfend)
    {
	  // Execute the global initializer
      if(*pfbegin != 0)
       (**pfbegin)();

	    // Go to next initializer inside the initializer table
        ++pfbegin;
    }
}

