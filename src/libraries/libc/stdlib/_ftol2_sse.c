#include "stdlib.h"


/*
===========================
Dynamic initializer sections
===========================
*/


// Standard C++ Runtime(STD CRT) __xc_a points to beginning of initializer table
#pragma data_seg(".CRT$XCA")
_PVFV __xc_a[] = {0};

// Standard C++ Runtime(STD CRT) __xc_z points to end of initializer table
#pragma data_seg(".CRT$XCZ")
_PVFV __xc_z[] = {0};

// Select the default data segment again(.data) for the rest of the unit
#pragma data_seg()

// Now, move the CRT data into .data section so we can read/write to it
#pragma comment(linker, "/merge:.CRT=.data")


// Current amount of entries in table
unsigned cur_atexitlist_entries = 0;

// function pointer table to global deinitializer table
_PVFV * pf_atexitlist = 0;

// Maximum entries allowed in table
unsigned max_atexitlist_entries = 32;




//! required by MSVC++ runtime for floating point operations (Must be 1)
int _fltused = 1;


// called by MSVC++ to convert a float to a long
long __declspec(naked) _ftol2_sse()
{

	int a;
	_asm {
		fistp[a]
		mov	ebx, a
		ret
	}
}

