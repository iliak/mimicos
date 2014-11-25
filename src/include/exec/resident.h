#ifndef	EXEC_RESIDENT_H
#define	EXEC_RESIDENT_H
//
//
//	Resident/ROMTag stuff.	Used to identify and initialize code modules.
//
//

#include <exec/types.h>


struct Resident 
{
    UWORD rt_MatchWord;				// word to match on (ILLEGAL)	
    struct Resident *rt_MatchTag;	// pointer to the above	
    APTR  rt_EndSkip;				// address to continue scan	
    UBYTE rt_Flags;					// various tag flags		
    UBYTE rt_Version;				// release version number	
    UBYTE rt_Type;					// type of module (NT_XXXXXX)	
    BYTE  rt_Pri;					// initialization priority 
    char  *rt_Name;					// pointer to node name	
    char  *rt_IdString;				// pointer to identification string 
    APTR  rt_Init;					// pointer to init code	
};

#define RTC_MATCHWORD	0x4AFC	// The 68000 "ILLEGAL" instruction 

#define RTF_AUTOINIT	(1<<7)	// rt_Init points to data structure 
#define RTF_AFTERDOS	(1<<2)
#define RTF_SINGLETASK	(1<<1)
#define RTF_COLDSTART	(1<<0)

// Compatibility: (obsolete) 
// #define RTM_WHEN	   3 
#define RTW_NEVER		0
#define RTW_COLDSTART	1

#endif	
