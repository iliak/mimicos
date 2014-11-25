
// ****************************************************************************
// **
// **    paging_pde.cpp
// **		-Page Directory Entries(PDE). This provides an abstract interface
// **	to aid in management of PDEs.
// **
// ****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "pde.h"



__inline void pde_set_attrib(pd_entry* e, ULONG attrib) 
{
	*e |= attrib;
}

__inline void pde_clear_attrib(pd_entry* e, ULONG attrib)
{
	*e &= ~attrib;
}

__inline void pde_set_frame(pd_entry* e, phy_addr addr)
{
	*e =(*e & ~I86_PDE_FRAME) | addr;
}

__inline BOOL pde_is_present(pd_entry e) 
{
	return e & I86_PDE_PRESENT;
}

__inline BOOL pde_is_writable(pd_entry e)
{
	return e & I86_PDE_WRITABLE;
}

__inline phy_addr pde_pfn(pd_entry e)
{
	return e & I86_PDE_FRAME;
}

__inline BOOL pde_is_user(pd_entry e)
{
	return e & I86_PDE_USER;
}

__inline BOOL pde_is_4mb(pd_entry e)
{
	return e & I86_PDE_4MB;
}

__inline void pde_enable_global(pd_entry e) 
{

}

