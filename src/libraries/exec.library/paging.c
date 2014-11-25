//** 
//** The *virtual memory manager* (VMM), which manages the mappings between virtual and physical addresses.
//**
//**

#include <string.h>
#include <exec\execbase.h>
#include <exec/kernel.h>

#include "paging.h"
#include "pmm.h"
#include "kheap.h"


// page table represents 4mb address space
#define PTABLE_ADDR_SPACE_SIZE 0x00400000

// directory table represents 4gb address space
#define DTABLE_ADDR_SPACE_SIZE 0x100000000

// page sizes are 4k
#define PAGE_SIZE 4096

//* The corresponding mask 
#define PAGE_MASK  ((1<<12) - 1)



// Helper macro to compute the index in the PD for the given virtual address
#define virt_to_pd_index(vaddr) (((ULONG)(vaddr)) >> 22)


// Helper macro to compute the index in the PT for the given virtual address
#define virt_to_pt_index(vaddr)		( (((ULONG)(vaddr)) >> 12) & 0x3ff )


// Helper macro to compute the offset in the page for the given virtual address
#define virt_to_page_offset(vaddr) (((ULONG)(vaddr)) & PAGE_MASK)





//
// Convert the virtual address into a page table
//
__inline pt_entry* paging_pte_lookup(struct ptable * p, vir_addr addr)
{
	if (p)
		return &p->m_entries[PAGE_TABLE_INDEX(addr)];

	return NULL;
}


//
// Convert the virtual address into a page directory 
//
__inline pd_entry* paging_pde_lookup(struct pdirectory* p, vir_addr addr)
{
	if (p)
		return &p->m_entries[PAGE_TABLE_INDEX(addr)];

	return NULL;
}


//
// Set the page directory
//
__inline BOOL paging_set_pdirectory(struct pdirectory* dir)
{
	if (!dir)
		return FALSE;

	SysBase.DirectoryTable = dir;
	paging_set_PDBR(SysBase.PageDirectory);

	return TRUE;
}


//
//Invalidates (flushes) the translation lookaside buffer (TLB)
//entry specified with the source operand
//
void paging_flush_tlb(vir_addr addr)
{
#ifdef _MSC_VER
	_asm
	{
		cli
		invlpg	addr
		sti
	}
#endif
}



//
// allocates a page in physical memory for a Page Table
// (Map a virtual page to a physical adress)
//
BOOL paging_alloc_page(pt_entry* e)
{
	// allocate a free physical frame
	phy_addr p = pmm_alloc_page();
	if (!p)
		return FALSE;

	// map the virtual adress to the phyical adress
	pte_set_frame(e, p);

	// Marks the page present
	pte_set_attrib(e, I86_PTE_PRESENT);

	//but doesnt set WRITE flag...

	return TRUE;
}


//
// frees a page in physical memory
//
void paging_free_page(pt_entry* e)
{
	void* p = (void*)pte_pfn(*e);
	if (!p)
		return;

	// Release physical memory frame
	pmm_free_page(p);

	// Mark the entry as not present
	pte_del_attrib(e, I86_PTE_PRESENT);
}


//
// Map any physical address to a virtual address
//
void paging_map_page(APTR phys, APTR virt)
{
	#ifdef _VERBOSE
	kprintf("PAGING", "Mapping phys: %#010P to virt: %#010P\n", phys, virt);
	#endif

	// get page directory
	pd_entry* pde = &SysBase.DirectoryTable->m_entries[PAGE_DIRECTORY_INDEX((ULONG)virt)];

	// Page directory not present in memory
	if ((*pde & I86_PDE_PRESENT) != I86_PDE_PRESENT)
	{
		#ifdef	_VERBOSE
		kprintf("PAGING", "Page %i not present, allocating it... \n", PAGE_DIRECTORY_INDEX((ULONG)virt));
		#endif

		// so allocate it
		struct ptable * table = (struct ptable*)pmm_alloc_page();
		if (!table)
			return;

		// clear it first
		#ifdef _VERBOSE
		kprintf(NULL, "clear it...");
		#endif
		memset(table, 0, sizeof(struct ptable));

		// create a new entry
		pd_entry* entry = &SysBase.DirectoryTable->m_entries[PAGE_DIRECTORY_INDEX((ULONG)virt)];

		// map in the table
		pde_set_attrib(entry, I86_PDE_PRESENT | I86_PDE_WRITABLE);
		pde_set_frame(entry, (phy_addr)table);
		#ifdef _VERBOSE
		kprintf(NULL, "attribs set...");
		#endif
	}

	// get table
	struct ptable * table = (struct ptable *)PAGE_GET_PHY_ADDRESS(pde);

	// get page
	pt_entry* page = &table->m_entries[PAGE_TABLE_INDEX((ULONG)virt)];

	// map it in
	pte_set_frame(page, (phy_addr)phys);
	pte_set_attrib(page, I86_PTE_PRESENT | I86_PTE_WRITABLE);
}


//
// Initialize virtual memmory manager
//
BOOL paging_init(void)
{
	kprintf("PAGING", "init()\n");

	// allocate default page table
	struct ptable * table = (struct ptable*)pmm_alloc_page();
	if (!table)
	{
		kprintf("PAGING", "Failed to allocate default page table !\n");
		return FALSE;
	}
	//memset(table, 0, sizeof(struct ptable));
	memset(table, 0, PAGE_SIZE);


	// allocates 3gb page table
	struct ptable* table2 = (struct ptable*)pmm_alloc_page();
	if (!table2)
	{
		kprintf("PAGING", "Failed to allocate 3gb page table !\n");
		return FALSE;
	}
	memset(table2, 0, PAGE_SIZE);



	// 0->4mb are identity mapped
	for (ULONG i = 0, frame = 0x00000000, virt = 0x00000000; i < 1024; i++, frame += 4096, virt += 4096)
	{
		// create a new page
		pt_entry page = 0;
		pte_set_attrib(&page, I86_PTE_PRESENT | I86_PTE_USER);
		pte_set_frame(&page, frame);

		// ...and add it to the page table
		table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
	}

	// map physic 1mb to virtual 3gb (where we are at)
	for (ULONG i = 0, frame = KERNEL_PBASE, virt = KERNEL_VBASE; i < 1024; i++, frame += 4096, virt += 4096)
	//	for(int i=0, frame=0x100000, virt=0xc0000000; i<1024; i++, frame+=4096, virt+=4096) 
	{
		// create a new page
		pt_entry page = 0;
		pte_set_attrib(&page, I86_PTE_PRESENT | I86_PTE_USER);
		pte_set_frame(&page, frame);

		// ...and add it to the page table
		table->m_entries[PAGE_TABLE_INDEX(virt)] = page;

		//kprintf("PAGING", "frame: 0x%08X, page: 0x%08X\n", frame, page);
	}






/*
	// VBE mapping
	//#define VBE_DISPI_LFB_PHYSICAL_ADDRESS	0xE0000000
	for (int i = 0, frame = 0xE0000000, virt = 0xE00000; i < 512; i++, frame += 4096, virt += 4096)
	{
		// create a new page
		pt_entry page = 0;
		pte_set_attrib(&page, I86_PTE_PRESENT);
		pte_set_attrib(&page, I86_PTE_USER);
		pte_set_frame(&page, frame);

		// ...and add it to the page table
		table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
	}
	// END VBE
*/







	// create default directory table
	struct pdirectory*   dir = (struct pdirectory*)pmm_alloc_pages(3);
	if (!dir)
	{
		kprintf("PAGING", "Failed to allocate default directory table !\n");
		return FALSE;
	}

	// clear directory table and set it as current
	memset(dir, 0, sizeof(struct pdirectory));

	// get first entry in dir table and set it up to point to our table
	pd_entry* entry = &dir->m_entries[PAGE_DIRECTORY_INDEX(0xC0000000)];
	pde_set_attrib(entry, I86_PDE_PRESENT | I86_PDE_WRITABLE);
	pde_set_frame(entry, (phy_addr)table);

	pd_entry* entry2 = &dir->m_entries[PAGE_DIRECTORY_INDEX(0x00000000)];
	pde_set_attrib(entry2, I86_PDE_PRESENT | I86_PDE_WRITABLE);
	pde_set_frame(entry2, (phy_addr)table2);

	// store current PDBR
	SysBase.PageDirectory = (phy_addr)&dir->m_entries;

	// switch to our page directory
	paging_set_pdirectory(dir);

	// enable paging
	paging_enable(TRUE);



	////
	// Create the kernel heap
	kheap_create();


	return TRUE;
}



//
// Enable or disable paging
//
void paging_enable(BOOL b)
{
	kprintf("PAGING", "%s\n", b == TRUE ? "Enabled" : "Disabled");

#ifdef _MSC_VER
	_asm
	{
		mov	eax, cr0
		cmp	[b], 1
		je	enable
		jmp disable
	enable :
		or	eax, 0x80000000		//set bit 31
		mov	cr0, eax
		jmp done
	disable :
		and eax, 0x7FFFFFFF		//clear bit 31
		mov	cr0, eax
	done :
	}
#endif
}


//
// Paging activated ?
//
BOOL paging_is_enabled(void)
{
	ULONG res = 0;

#ifdef _MSC_VER
	_asm
	{
		mov	eax, cr0
		mov[res], eax
	}
#endif

	return(res & 0x80000000) ? FALSE : TRUE;
}


//
// Set the page directory base register 
//
void paging_set_PDBR(phy_addr addr)
{
#ifdef _MSC_VER
	_asm
	{
		mov	eax, [addr]
		mov	cr3, eax		// PDBR is cr3 register in i86
	}
#endif
}


//
// Get the page directory base register 
//
phy_addr paging_get_PDBR(void)
{
#ifdef _MSC_VER
	_asm
	{
		mov	eax, cr3
		ret
	}
#endif
}

//
//
//
phy_addr paging_vir_to_phy(vir_addr virt)
{
	// get page directory
	pd_entry* pde = &SysBase.DirectoryTable->m_entries[PAGE_DIRECTORY_INDEX((ULONG)virt)];

	// Page directory not present in memory
	if ((*pde & I86_PDE_PRESENT) != I86_PDE_PRESENT)
		return NULL;

	// get table
	struct ptable * table = (struct ptable *)PAGE_GET_PHY_ADDRESS(pde);

	// get page
	pt_entry* page = &table->m_entries[PAGE_TABLE_INDEX((ULONG)virt)];

	return pte_pfn(*page);
}


