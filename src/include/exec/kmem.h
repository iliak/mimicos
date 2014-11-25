#ifndef KMEM_H
#define	KMEM_H

#include <exec/errors.h>
#include <exec/macros.h>
#include <exec/memory.h>
#include <exec/kslab.h>


// The base and top virtual addresses covered by the kernel allocator
#define KMEM_VMM_BASE		0x4000					// 16kB
#define KMEM_VMM_TOP		PAGING_MIRROR_VADDR		// 1GB - 4MB


// 
// Flags for kmem_vmm_new_range and kmem_vmm_alloc
// 
#define KMEM_VMM_MAP		(1<<0)					// Physical pages should be immediately mapped
#define KMEM_VMM_ATOMIC		(1<<1)					// Allocation should either success or fail, without blocking


// The base and top virtual addresses covered by the kernel allocator
#define KMEM_VMM_BASE		0x4000					// 16kB
#define KMEM_VMM_TOP		PAGING_MIRROR_VADDR		// 1GB - 4MB




struct kmem_range;





/*
kmem_range *	create_range(BOOL, vir_addr, vir_addr, kslab *);
kmem_range *	kmem_new_range(ULONG, ULONG, vir_addr *);
ret_t			kmem_setup(vir_addr, vir_addr, vir_addr, vir_addr);
*/

#endif