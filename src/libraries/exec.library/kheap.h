#ifndef HEAP_H
#define	HEAP_H

#include <exec\types.h>
//#include "ordered_array.h"

// http://www.brokenthorn.com/forums/viewtopic.php?f=16&t=2874



#define KHEAP_BASE			0xD0000000						// Kernel heap virtual start address
#define KHEAP_TOP			0xE0000000						// Kernel heap virtual end address
#define KHEAP_MINSIZE		0x00100000						// Kernel minimum heap size (1Mo)
#define KHEAP_MAXSIZE		(KHEAP_TOP - KHEAP_BASE)		// Kernel maximum heap size (256Mo)
#define KHEAP_START_SIZE	0x00400000						// Kernel initial heap size (4Mo)
#define KHEAP_INC			0x00080000						// Size of each increment (512Ko)

#define UHEAP_BASE			0xB0000000						// User heap virtual start address
#define UHEAP_TOP			0xC0000000						// Kernel heap virtual end address
#define UHEAP_MAXSIZE		(UHEAP_TOP - UHEAP_BASE)
#define UHEAP_INC			0x00100000						// Size of each increment (512Ko)



VOID kheap_create(VOID);

VOID kheap_dump(VOID);


//
// Allocate kernel memory
//
APTR kmalloc(ULONG size);

// 
// Free allocated memory
// 
VOID kfree(APTR vaddr);

#endif 
