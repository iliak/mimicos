#ifndef _MMNGR_VIRT_PDE_H
#define _MMNGR_VIRT_PDE_H
//****************************************************************************
//**
//**    paging_pde.h
//**		-Page Directory Entries(PDE). This provides an abstract interface
//**	to aid in management of PDEs.
//**
//****************************************************************************



#include <exec/types.h>
#include <exec/memory.h>


// this format is defined by the i86 architecture--be careful if you modify it
enum PAGE_PDE_FLAGS 
{

	I86_PDE_PRESENT			=	0x00000001,			//0000000000000000000000000000001
	I86_PDE_WRITABLE		=	0x00000002,			//0000000000000000000000000000010
	I86_PDE_USER			=	0x00000004,			//0000000000000000000000000000100
	I86_PDE_PWT				=	0x00000008,			//0000000000000000000000000001000
	I86_PDE_PCD				=	0x00000010,			//0000000000000000000000000010000
	I86_PDE_ACCESSED		=	0x00000020,			//0000000000000000000000000100000
	I86_PDE_DIRTY			=	0x00000040,			//0000000000000000000000001000000
	I86_PDE_4MB				=	0x00000080,			//0000000000000000000000010000000
	I86_PDE_CPU_GLOBAL		=	0x00000100,			//0000000000000000000000100000000
	I86_PDE_LV4_GLOBAL		=	0x00000200,			//0000000000000000000001000000000
   	I86_PDE_FRAME			=	0x7FFFF000 			//1111111111111111111000000000000
};



extern void			pde_set_attrib(pd_entry* e, ULONG attrib);					// sets a flag in the page table entry
extern void			pde_clear_attrib(pd_entry* e, ULONG attrib);				// clears a flag in the page table entry
extern void			pde_set_frame(pd_entry*, phy_addr);							// sets a frame to page table entry
extern BOOL			pde_is_present(pd_entry e);									// test if page is present
extern BOOL			pde_is_user(pd_entry);										// test if directory is user mode
extern BOOL			pde_is_4mb(pd_entry);										// test if directory contains 4mb pages
extern BOOL			pde_is_writable(pd_entry e);								// test if page is writable
extern phy_addr		pde_pfn(pd_entry e);										// get page table entry frame address
extern void			pde_enable_global(pd_entry e);								// enable global pages


#endif
