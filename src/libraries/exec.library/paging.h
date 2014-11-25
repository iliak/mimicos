#ifndef PAGING_H
#define PAGING_H
//****************************************************************************
//**
//**    mmngr_virtual.h
//**		-Virtual Memory Manager
//**
//****************************************************************************


#include <exec/types.h>
#include <exec/memory.h>

#include "pte.h"
#include "pde.h"
#include "kheap.h"







typedef struct page
{
	ULONG present : 1;		// Page present in memory
	ULONG rw : 1;			// Read-only if clear, readwrite if set
	ULONG user : 1;			// Supervisor level only if clear
	ULONG accessed : 1;		// Has the page been accessed since last refresh?
	ULONG dirty : 1;		// Has the page been written to since last refresh?
	ULONG unused : 7;		// Amalgamation of unused and reserved bits
	ULONG frame : 20;		// Frame address (shifted right 12 bits)
} page_t;


typedef struct page_table
{
	page_t pages[1024];
} page_table_t;


typedef struct page_directory
{
	/**
	Array of pointers to pagetables.
	**/
	page_table_t *tables[1024];
	/**
	Array of pointers to the pagetables above, but gives their *physical*
	location, for loading into the CR3 register.
	**/
	ULONG tablesPhysical[1024];

	/**
	The physical address of tablesPhysical. This comes into play
	when we get our kernel heap allocated and the directory
	may be in a different location in virtual memory.
	**/
	ULONG physicalAddr;
} page_directory_t;

BOOL			paging_alloc_page(pt_entry *);									// allocates a page in physical memory														
void			paging_flush_tlb(vir_addr addr);								// flushes a cached translation lookaside buffer(TLB) entry	
void			paging_free_page(pt_entry * e);									// frees a page in physical memory															
BOOL			paging_init(VOID);												// initialize the memory manager															
void			paging_map_page(APTR phys, APTR virt);							// maps phys to virtual address																
pd_entry *		paging_pde_lookup(struct pdirectory * p, vir_addr addr);		// get directory entry from directory table		
pt_entry *		paging_pte_lookup(struct ptable * p, vir_addr addr);			// get page entry from page table	
BOOL			paging_set_pdirectory(struct pdirectory *);						// switch to a new page directory

void			paging_enable(BOOL);											// enable or disable paging
BOOL			paging_is_enabled(void);										// test if paging is enabled
void			paging_set_PDBR(phy_addr);										// loads the page directory base register(PDBR)
phy_addr		paging_get_PDBR(void);											// get PDBR physical address

phy_addr		virt_to_phys(vir_addr vaddr);									// Return the physical address of the given virtual address
//vir_addr		phys_to_virt(phy_addr);											// Return the virtual address of the given physical address

//extern pdirectory*	paging_get_directory			(void);								// get current page directory																
//extern void			paging_ptable_clear				(ptable* p);						// clears a page table	
//extern ULONG			paging_ptable_virt_to_index		(vir_addr addr);					// convert virtual address to page table index	
//extern ULONG			paging_pdirectory_virt_to_index	(vir_addr addr);					// convert virtual address to page directory index	
//extern void			paging_pdirectory_clear			(pdirectory* dir);					// clears a page directory table	



//extern int			paging_createPageTable			(pdirectory* dir, ULONG virt, ULONG flags);				// 
//extern void			paging_mapPhysicalAddress		(pdirectory* dir, ULONG virt, ULONG phys, ULONG flags);	// 
//extern void			paging_unmapPageTable			(pdirectory* dir, ULONG virt);							// 
//extern void			paging_unmapPhysicalAddress		(pdirectory* dir, ULONG virt);							// 
//extern pdirectory*	paging_createAddressSpace		(void);													// 
//extern APTR			paging_getPhysicalAddress		(pdirectory* dir, ULONG virt);							// 
//
#endif
