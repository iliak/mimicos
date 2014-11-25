#ifndef	SLAB_H
#define	SLAB_H

#ifdef USE_SLAB

#include <exec/types.h>
#include <exec/kmem.h>
#include <exec/errors.h>
#include <exec/memory.h>


// Dimensioning constants 
#define NB_PAGES_IN_SLAB_OF_CACHES 1
#define NB_PAGES_IN_SLAB_OF_RANGES 1


//
// Flags for kmem_cache_alloc()
// 
#define SLAB_ALLOC_ATOMIC	(1<<0)			// Allocation should either succeed or fail, without blocking




#define SLAB_CREATE_MAP		(1<<0)				// The slabs should be initially mapped in physical memory
#define SLAB_CREATE_ZERO	(1<<1)				// The object should always be set to zero at allocation (implies SLAB_CREATE_MAP)



// Opaque data structure that defines a Cache of slabs
struct slab_cache;

// Opaque data structure that defines a slab. Exported only to kmem_vmm.h
struct	slab;





// 
// @note this function MAY block (involved allocations are not atomic)
// @param name must remain valid during the whole cache's life (shallow copy) !
// @param flags An or-ed combination of the SLAB_CREATE_* flags
// 
slab_cache * slab_cache_create(const char* name,
		      ULONG object_size,
		      ULONG pages_per_slab,
		      ULONG min_free_objects,
		      ULONG cache_flags);

ret_t	slab_cache_destroy(slab_cache *cache);


// 
// Allocate an object from the given cache.
// 
// @param alloc_flags An or-ed combination of the SLAB_ALLOC_* flags
// 
vir_addr slab_cache_alloc(slab_cache *cache, ULONG flags);
slab_cache * slab_cache_prepare(vir_addr kernel_core_base,
			     vir_addr		kernel_core_top,
			     ULONG			sizeof_struct_range,
			     slab **		first_struct_slab_of_caches,
			     vir_addr *		first_slab_of_caches_base,
			     ULONG *		first_slab_of_caches_nb_pages,
			     slab **		first_struct_slab_of_ranges,
			     vir_addr *		first_slab_of_ranges_base,
			     ULONG *		first_slab_of_ranges_nb_pages);


// 
// Initialize the slab cache of slab caches, and prepare the cache of
// kmem_range for kmem_vmm.
// 
// @param kernel_core_base The virtual address of the first byte used
// by the kernel code/data
// 
// @param kernel_core_top The virtual address of the first byte after
// the kernel code/data.
// 
// @param sizeof_struct_range the size of the objects (aka "struct
// sos_kmem_vmm_ranges") to be allocated in the cache of ranges
// 
// @param first_struct_slab_of_caches (output value) the virtual
// address of the first slab structure that gets allocated for the
// cache of caches. The function actually manually allocate the first
// slab of the cache of caches because of a chicken-and-egg thing. The
// address of the slab is used by the kmem_vmm_setup routine to
// finalize the allocation of the slab, in order for it to behave like
// a real slab afterwards.
// 
// @param first_slab_of_caches_base (output value) the virtual address
// of the slab associated to the slab structure.
// 
// @param first_slab_of_caches_nb_pages (output value) the number of
// (virtual) pages used by the first slab of the cache of caches.
// 
// @param first_struct_slab_of_ranges (output value) the virtual address
// of the first slab that gets allocated for the cache of ranges. Same
// explanation as above.
// 
// @param first_slab_of_ranges_base (output value) the virtual address
// of the slab associated to the slab structure.
// 
// @param first_slab_of_ranges_nb_pages (output value) the number of
// (virtual) pages used by the first slab of the cache of ranges.
// 
// @return the cache of kmem_range immediatly usable
slab_cache * slab_setup_prepare(vir_addr kernel_core_base,
			     vir_addr kernel_core_top,
			     ULONG  sizeof_struct_range,
			     slab **first_struct_slab_of_caches,
			     vir_addr*first_slab_of_caches_base,
			     ULONG *first_slab_of_caches_nb_pages,
			     slab **first_struct_slab_of_ranges,
			     vir_addr *first_slab_of_ranges_base,
			     ULONG *first_slab_of_ranges_nb_pages);

// 
// Update the configuration of the cache subsystem once the vmm
// subsystem has been fully initialized
ret_t slab_setup_commit(slab *first_struct_slab_of_caches, kmem_range *first_range_of_caches,
			    slab *first_struct_slab_of_ranges, kmem_range *first_range_of_ranges);




// 
// Free an object (assumed to be already allocated and not already
// free) at the given virtual address.
// 
ret_t slab_cache_free(vir_addr vaddr);
















#endif

#endif