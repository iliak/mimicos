#ifndef	EXEC_MEMORY_H
#define	EXEC_MEMORY_H

//
//
//	Definitions and structures used by the memory allocation system
//
// http://wiki.amigaos.net/index.php/Exec_Memory_Allocation

#include <exec/macros.h>
#include <exec/nodes.h>
#include <exec\ordered_array.h>



// i86 architecture defines 1024 entries per table--do not change
#define PAGES_PER_TABLE					1024
#define PAGES_PER_DIR					1024
#define PAGE_DIRECTORY_INDEX(x)			(((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x)				(((x) >> 12) & 0x3ff)
#define PAGE_GET_PHY_ADDRESS(x)			(*x & ~0xfff)




//
// Memory pointer types
//
typedef	ULONG phy_addr;					// Physical address
typedef ULONG vir_addr;					// A Virtual Address
typedef ULONG pt_entry;					// A Page Table Entry
typedef ULONG pd_entry;					// A Page Directery Entry



//
// Address Range Types
//
enum MemRangeType
{
	AddressRangeMemory = 1L,		// This range is available RAM usable by the operating system.
	AddressRangeReserved = 2L,		// This range of addresses is in use or reserved by the system
									// and must not be used by the operating system.
	AddressRangeACPI = 3L,			// ACPI Reclaim Memory. This range is available RAM usable by
									// the OS after it reads the ACPI tables.
	AddressRangeNVS = 4L,			// ACPI NVS Memory. This range of addresses is in use or reserve
									// by the system and must not be used by the operating system.
									// This range is required to be saved and restored across an NVS sleep.
};



//
// Description of a block of memory available in the system pool
//
/*struct MemRegion
{
	struct Node			Node;				// Pointer to the next memory region
	ULONG				Base;				// base address
	ULONG				Size;				// length(in bytes)
	enum MemRangeType	Type;				//
	STRPTR				Name;				// Name of the block
};
*/
struct MemoryRegion
{
	ULONG			BaseL;			// base address
	ULONG			BaseH;			//
	ULONG			LengthL;		// length(in bytes)
	ULONG			LengthH;		//
	UWORD			Type;			// Region type
	UWORD			acpi;			// Extended
	ULONG			padding;		// Padding to 24 bytes length
};








#pragma pack(push, 1)
typedef struct page_table_entry
{
	ULONG present : 1;		// Page is resident in memory and not swapped out
	ULONG rw : 1;			// Set if the page may be written to
	ULONG user : 1;			// Set if the page is accessible from user space
	ULONG accessed : 1;		// Set if the page is accessed
	ULONG dirty : 1;		// Set if the page is written to
	ULONG unused : 7;		// Amalgamation of unused and reserved bits
	ULONG address : 20;		// Frame address (shifted right 12 bits)
} page_table_entry;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct page_directory_entry
{
	ULONG present : 1;				// Page is resident in memory and not swapped out
	ULONG rw : 1;					// Set if the page may be written to
	ULONG user : 1;					// Set if the page is accessible from user space
	ULONG pwt : 1;					// write-through
	ULONG pcd : 1;					// Page-level cache disable
	ULONG accessed : 1;				// Accessed; indicates whether software has accessed the 4-MByte page referenced by this entry
	ULONG dirty : 1;				// Dirty; indicates whether software has written to the 4-MByte page referenced by this entry
	ULONG pagesize : 1;				// Page size; must be 1 (otherwise, this entry references a page table)
	ULONG global : 1;				// determines whether the translation is global
	ULONG ignored : 3;				// Ignored
	ULONG address : 20;				// physical address of the 4-MByte page referenced by this entry
} page_directory_entry;
#pragma pack(pop)




// Page Table
struct ptable
{
	pt_entry m_entries[PAGES_PER_TABLE];
};


// Page Directory
struct pdirectory
{
	pd_entry m_entries[PAGES_PER_DIR];
};









////////////////////////////////////////////////////////////////////////////////////////
// Heap management
//

#define	MEMORY_CHUNK_MAGIC		0xCAFEBABE			// Magic for MemHeader & MemFooter
#define MEMORY_BLOCK_FREE		0					// Memory block is free of use
#define MEMORY_BLOCK_USED		1					// Memory block is used by a task


//
//
//
struct MemChunk
{
	struct MemChunk *	Next;		// Pointer to the next chunk
	ULONG				Size;		// Size of the chunk
};



#define	HEAP_MAGIC			0xD15EA5E						// Header a of heap structure


//
// Size information for a hole/block
//
typedef struct
{
	ULONG			magic;			// Magic number, used for error checking and identification.
	ULONG			size;			// size of the block, including the end footer.
	ULONG			used;			// 
	ULONG			padding;		//
} heap_header;

typedef struct
{
	ULONG			magic;			// Magic number, same as in heap_header.
	heap_header *	header;			// Pointer to the block header.
} heap_footer;

typedef struct
{
	ordered_array	Index;
	ULONG			Base;			// The start of our allocated space.
	ULONG			Allocated;		// The end of our allocated space. May be expanded up to max_address.
	ULONG			Top;			// The maximum address the heap can be expanded to.
	//ULONG			Supervisor : 1;	// Should extra pages requested by us be mapped as supervisor-only?
	//ULONG			ReadOnly : 1;	// Should extra pages requested by us be mapped as read-only?
} Heap;








//////////////////////////////////////////////////////////////////////////////////////
// Memory header informations
//
struct MemHeader
{
	struct Node		Node;			//
	ULONG			Attributes;		//
	struct MemChunk	*First;			// pointer to the first MemChunk structure
	APTR			Lower;			// lowest address within the memory block
	APTR			Upper;			// highest address within the memory block
	ULONG			Free;			// total free space
};


/* Total size of struct MemChunk, including padding */
//#define MEMCHUNK_TOTAL  (sizeof(struct MemChunk))
/* Total size of struct MemHeader, including padding */
//#define MEMHEADER_TOTAL (ROUNDUP(sizeof(MemHeader), MEMCHUNK_TOTAL))


/*
//
// After a chunk of memory
//
struct MemFooter
{
ULONG		Magic;		// Magic number
MemHeader *	Header;		// Pointer to the header.
};
*/





//----- Memory Requirement Types ---------------------------
#define MEMF_PRIVATE	(1L<<1)		// Allocate from the task private heap. This memory will not be visible to any other address space. 
#define MEMF_SHARED		(1L<<2)		// Allocate from the system shared heap. This memory can be shared between all address spaces 
// and will always appear at the same address in any address space
#define MEMF_EXECUTABLE	(1L<<3)		// Allocate memory that is marked executable
#define MEMF_CLEAR		(1L<<10)	// AllocMem: NULL out area before return 

#define MEMF_LARGEST	(1L<<20)	// AvailMem: return the largest chunk size 
#define MEMF_TOTAL		(1L<<21)	// AvailMem: return total size of memory 

#define	MEMF_NO_EXPUNGE	(1L<<31)	 //AllocMem: Do not cause expunge on failure 


//----- Memory Attributes Tags ---------------------------
#define MEMT_LOCK		(1L<<1)		// After allocating memory, lock the associated pages in memory. 
// This will prevent the pages from being moved, swapped out or otherwise being made unavailable
#define MEMT_ALIGNMENT	(1L<<2)		// Define an alignment constraint for the allocated memory block. The returned memory block 
// will be aligned to the given size (in bytes).
// It's virtual address will be at least a multiple of the AVT_Alignment value. 
#define	MEMT_CLEAR		(1L<<3)		// Clear the newly allocated memory with the given byte value.
// If this tag is not given the memory block is not cleared. 

//----- Current alignment rules for memory blocks -----
#define MEM_BLOCKSIZE	(8L)
#define MEM_BLOCKMASK	(MEM_BLOCKSIZE-1)


//
/// **** MemHandlerData *********************************************
// Note:  This structure is *READ ONLY* and only EXEC can create it!
struct MemHandlerData
{
	ULONG	RequestSize;	// Requested allocation size 
	ULONG	RequestFlags;	// Requested allocation flags 
	ULONG	Flags;			// Flags (see below) 
};

#define	MEMHF_RECYCLE	(1L<<0)	// 0==First time, 1==recycle 

/// **** Low Memory handler return values **************************
#define	MEM_DID_NOTHING	(0)		// Nothing we could do... 
#define	MEM_ALL_DONE	(-1)	// We did all we could do 
#define	MEM_TRY_AGAIN	(1)		// We did some, try the allocation again 






#endif

