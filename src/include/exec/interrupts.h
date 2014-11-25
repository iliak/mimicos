#ifndef	EXEC_INTERRUPTS_H
#define	EXEC_INTERRUPTS_H
//
//	Callback structures used by hardware & software interrupts
//

#include <exec/nodes.h>
#include <exec/lists.h>

// Interrupt handler prototype
typedef void(_cdecl * InterruptHandler)(void);


//
//
//
struct Interrupt
{
	struct Node	is_Node;
	APTR		is_Data;		    // server data segment  
	VOID(*is_Code)();	    // server code entry    
};


//
//
//
struct IntVector
{		// For EXEC use ONLY! 
	APTR			iv_Data;
	VOID			(*iv_Code)();
	struct Node *	iv_Node;
};



//
//
//
struct SoftIntList
{		// For EXEC use ONLY! 
	struct List	sh_List;
	UWORD		sh_Pad;
};

#define SIH_PRIMASK (0xf0)

// this is a fake INT definition, used only for AddIntServer and the like 
#define INTB_NMI	15
#define INTF_NMI	(1L<<15)




#endif
