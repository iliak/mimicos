//****************************************************************************
//**
//** The Physical Memory Manager (PMM), which knows which areas of memory are 
//** free to use and which are allocated.
//**
//****************************************************************************

#include <string.h>
//#include <exec/kernel.h>
#include <exec/execbase.h>
#include "pmm.h"
#include <proto/exec.h>

__inline void mmap_set(ULONG bit);				// set any bit(frame) within the memory map bit array
__inline void mmap_unset(ULONG bit);			// unset any bit(frame) within the memory map bit array
__inline BOOL mmap_test(ULONG bit);				// test any bit(frame) within the memory map bit array
LONG mmap_first_free(void);						// finds first free frame in the bit array and returns its index
LONG mmap_first_free_s(ULONG size);				// finds first free "size" number of frames and returns its index




// Name for the different types of memory regions
const char * MemoryTypes[] =
{
	{"Available"},				//type 1
	{"Reserved"},				//type 2
	{"ACPI Reclaim"},			//type 3
	{"ACPI NVS Memory"},		//type 4
	{"Unknown"}
};

/*
//
//
//
struct MemReg
{
	ULONG			BaseL;			// base address
	ULONG			BaseH;			//
	ULONG			LengthL;		// length(in bytes)
	ULONG			LengthH;		//
	UWORD			Type;			// Region type
	UWORD			acpi;			// Extended
	ULONG			padding;		// Padding to 24 bytes length
};
*/





// set any bit(frame) within the memory map bit array
__inline void mmap_set(ULONG bit)
{
	SysBase.MemoryMap[bit / 32] |= (1 << (bit % 32));
}


// unset any bit(frame) within the memory map bit array
__inline void mmap_unset(ULONG bit)
{
	SysBase.MemoryMap[bit / 32] &= ~(1 << (bit % 32));
}


// test if any bit(frame) is set within the memory map bit array
__inline BOOL mmap_test(ULONG bit)
{
	return SysBase.MemoryMap[bit / 32] & (1 << (bit % 32));
}


// finds first free frame in the bit array and returns its index
LONG mmap_first_free(void)
{
	// find the first free bit
	//for(ULONG i=0; i< pmm_get_block_count(); i++)
	//for (ULONG i = 0; i < SysBase.MemoryFreePages; i++)
	for (ULONG i = 0; i < PMM_MEMORY_MAP_SIZE; i++)
	{
		if (SysBase.MemoryMap[i] != 0xffffffff)
		{
			for (int j = 0; j < 32; j++) 				// test each bit in the dword
			{
				int bit = 1 << j;
				if (!(SysBase.MemoryMap[i] & bit))
					return i * 32 + j;
			}
		}
	}

	return -1;
}


// finds first free "size" number of frames and returns its index
LONG mmap_first_free_s(ULONG size)
{
	if (size == 0)
		return -1;

	if (size == 1)
		return mmap_first_free();

	//for(ULONG i=0; i<pmm_get_block_count(); i++)
	//for (ULONG i = 0; i < SysBase.MemoryFreePages; i++)
	for (ULONG i = 0; i < PMM_MEMORY_MAP_SIZE; i++)
	{
		if (SysBase.MemoryMap[i] != 0xffffffff)
		{
			for (int j = 0; j < 32; j++)	// test each bit in the dword
			{
				int bit = 1 << j;
				if (!(SysBase.MemoryMap[i] & bit))
				{
					int startingBit = i * 32;
					startingBit += bit;		//get the free bit in the dword at index i

					ULONG free = 0; //loop through each bit to see if its enough space
					for (ULONG count = 0; count <= size; count++)
					{
						if (!mmap_test(startingBit + count))
							free++;	// this bit is clear(free frame)

						if (free == size)
							return i * 4 * 8 + j; //free count==size needed; return index
					}
				}
			}
		}
	}
	return -1;
}


//
// Physical Memmory Management initialize
//
void pmm_init(struct multiboot_info *mbi, phy_addr bitmapBase)
{
	kprintf("PMM", "init()\n");

	// Find the total amount of memory
	//struct multiboot_mmap_entry *entry = mbi->mmap_addr;
	//ULONG memsize = 0;
	//UBYTE i = 0;
	//while ((APTR)entry < (mbi->mmap_addr + mbi->mmap_length))
	//{
	//	memsize += (ULONG)(entry->len & 0xffffffff);

	//	entry = (struct multiboot_mmap_entry *) ((ULONG)entry + entry->size + sizeof(entry->size));
	//	i++;
	//}

	//SysBase.MemoryPageCount = (memsize * 1024) / PAGE_SIZE;
	//SysBase.MemoryUsedPages = SysBase.MemoryPageCount;

	SysBase.MemoryMaxPages = 0;				// No memory page available
	SysBase.MemoryUsedPages = 0;			// No used page block


	// By default, all memory is in use
	SysBase.MemoryMap = (ULONG *)bitmapBase;
	//memset(SysBase.MemoryMap, 0xFFFFFFFF, SysBase.MemoryPageCount / PMM_BLOCKS_PER_BYTE);
	memset(SysBase.MemoryMap, 0xFF, PMM_MEMORY_MAP_SIZE);




	// Add memory regions to the system pool
	struct multiboot_mmap_entry *entry = mbi->mmap_addr;
	UBYTE i = 0;
	while ((APTR)entry < (mbi->mmap_addr + mbi->mmap_length))
	{
		AddMemList((ULONG)(entry->addr & 0xffffffff), (ULONG)(entry->len & 0xffffffff), entry->type, entry->type < 4 ? MemoryTypes[entry->type] : "unknown !");

		entry = (struct multiboot_mmap_entry *) ((ULONG)entry + entry->size + sizeof(entry->size));
		i++;
	}

	//TODO: Marquer la zone occupée par le kernel et le tableau de 128Ko Memorymap comme occupée !

}



//
// Marks a physical memory region as free
//
void pmm_enable_region(phy_addr base, ULONG size)
{
	int align = base / PAGE_SIZE;
	int blocks = size / PAGE_SIZE;

	for (; blocks > 0; blocks--)
	{
		mmap_unset(align++);
		SysBase.MemoryMaxPages++;
	}

#ifdef _VERBOSE
	kprintf("PMM", "Enabling memory region [%#010P - %#010P] (%i Ko)\n", base, base + size, size / 1024);
#endif
	mmap_set(0);	//first block is always set. This insures allocs cant be 0
}


//
// Marks a physical memory region as used
//
void pmm_disable_region(phy_addr base, ULONG size)
{
	int align = base / PAGE_SIZE;
	int blocks = size / PAGE_SIZE;

	for (; blocks > 0; blocks--)
	{
		mmap_set(align++);
		SysBase.MemoryMaxPages--;
	}

#ifdef _DEBUG
	//kprintf("PMM", "Disabling memory region [%#010P - %#010P] (%i Ko)\n", base, base + size, size / 1024);
#endif
	mmap_set(0);	//first block is always set. This insures allocs cant be 0
}


//
// Mark next free block of memory as allocated
//
phy_addr pmm_alloc_page(void)
{
	if (pmm_get_free_page_count() <= 0)
		return NULL;	//out of memory

	int frame = mmap_first_free();
	if (frame == -1)
		return NULL;	//out of memory

	mmap_set(frame);

	phy_addr addr = frame * PAGE_SIZE;
	SysBase.MemoryUsedPages++;

	return addr;
}

//
// Mark a block of memory as free
//
void	pmm_free_page(APTR p)
{

	phy_addr addr = (phy_addr)p;
	int frame = addr / PAGE_SIZE;

	mmap_unset(frame);

	SysBase.MemoryUsedPages--;
}

//
// allocates contiguous pages of memory
//
APTR	pmm_alloc_pages(ULONG count)
{
	if (pmm_get_free_page_count() <= count)
		return NULL;	//not enough space

	int frame = mmap_first_free_s(count);
	if (frame == -1)
		return NULL;	//not enough space

	for (ULONG i = 0; i < count; i++)
		mmap_set(frame + i);

	phy_addr addr = frame * PAGE_SIZE;
	SysBase.MemoryUsedPages += count;

	return (APTR)addr;
}


//
// Mark several blocks of memory as allocated
//
void pmm_free_pages(APTR p, ULONG count)
{
	phy_addr addr = (phy_addr)p;
	int frame = addr / PAGE_SIZE;

	for (ULONG i = 0; i < count; i++)
		mmap_unset(frame + i);

	SysBase.MemoryUsedPages -= count;
}

//
//
//
ULONG pmm_get_free_page_count(void)
{
	return SysBase.MemoryMaxPages - SysBase.MemoryUsedPages;
}

//
//
//
ULONG pmm_get_page_size(void)
{
	return PAGE_SIZE;
}

