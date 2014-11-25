#ifndef EXECBASE_H
#define EXECBASE_H


#include <exec\lists.h>
#include <exec\memory.h>
//#include <exec\kslab.h>

#define KERNEL_VBASE		(0xC0000000)			// Kernel virtual memory base (3gb)
#define KERNEL_PBASE		(0x00100000)			// Kernel physical memory base (1mb)

#define	EXEC_VERSION			1
#define	EXEC_REVISION			0

//
// Virtual memory management
//




//
//
//
struct ExecBase
{
	UWORD	SoftVer;					// Release number
	UWORD	SoftRev;					// Revision number

	struct List MemList;				// 
	struct List ResourceList;			// 
	struct List DeviceList;				// List of loaded devices
	struct List IntrList;				// 
	struct List LibList;				// 
	struct List PortList;				// 
	struct List TaskReady;				// Tasks that are ready to run
	struct List TaskWait;				// Tasks that wait for some event
	struct List SemaphoreList;			// 

	// Kernel
	ULONG	KernelVBase;				// Virtual base address of the kernel
	ULONG	KernelPBase;				// Phisical base address of the kernel
	ULONG	KernelSize;					// Size of the kernel

	// Physical memory management
	ULONG	MemorySize;					// size of physical memory
	ULONG	MemoryUsedPages;			// Number of blocks currently in use
	ULONG	MemoryMaxPages;				// Maximum number of memory pages
	ULONG *	MemoryMap;					// memory map bit array. Each bit represents a memory page

	Heap	KernelHeap;					// Kernel heap base

	// Virtual memory management
	phy_addr			PageDirectory;		// Page Directory base register
	struct pdirectory *	DirectoryTable;
	struct List			FreeVMemRange;		// Range of sorted free virtual memory 
	struct List			UsedVMemRange;		// Range of sorted used virtual memory 
	//kslab_cache *	kmem_range_cache;
	//List			SlabCacheList;			// List of slab caches		http://www.secretmango.com/jimb/Whitepapers/slabs/slab.html
};

extern struct ExecBase	SysBase;

#endif
