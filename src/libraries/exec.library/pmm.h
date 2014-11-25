#ifndef MMNGR_PHYS_H
#define MMNGR_PHYS_H
//****************************************************************************
//**
//**    mmngr_phys.cpp
//**		-Physical Memory Manager
//**
//****************************************************************************

#include <exec/types.h>
#include <exec/bootinfo.h>
#include <exec/memory.h>


// 8 pages per byte
#define PMM_BLOCKS_PER_BYTE 8

// Page size (4k)
#define PAGE_SIZE	4096

// Page alignment
#define PAGE_ALIGN	PAGE_SIZE


// Size of the memory map  (128Ko)
#define PMM_MEMORY_MAP_SIZE			0xFFFFFFFF / PAGE_SIZE / PMM_BLOCKS_PER_BYTE

// Page alignment
#define PAGE_ALIGN_INF(val)			ALIGN_INF((val), PAGE_SIZE)
#define PAGE_ALIGN_SUP(val)			ALIGN_SUP((val), PAGE_SIZE)
#define IS_PAGE_ALIGNED(val)		IS_ALIGNED((val), PAGE_SIZE)



// 
// This is the reserved physical interval for the x86 video memory and BIOS area. 
#define BIOS_VIDEO_START 0x0A0000
#define BIOS_VIDEO_END   0x100000


//
// Protos
//
void		pmm_init(struct multiboot_info *, phy_addr);		// initialize the physical memory manager
void		pmm_enable_region(phy_addr base, ULONG length);		// enables a physical memory region for use
void		pmm_disable_region(phy_addr base, ULONG length);	// disables a physical memory region as in use
phy_addr	pmm_alloc_page(void);								// allocates a single physical memory page
void		pmm_free_page(APTR);								// releases a memory page
APTR		pmm_alloc_pages(ULONG count);						// allocates physical pages of memory
void		pmm_free_pages(APTR, ULONG);						// frees page of memory
ULONG		pmm_get_free_page_count(void);						// returns number of pages not in use
ULONG		pmm_get_page_size(void);							// returns default memory page size in bytes


#endif
