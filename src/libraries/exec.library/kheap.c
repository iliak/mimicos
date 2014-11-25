#include <exec/execbase.h>
#include <exec/kernel.h>
#include <exec/alerts.h>
#include <proto/exec.h>
#include "paging.h"

#include "kheap.h"
#include "pmm.h"





// http://www.jamesmolloy.co.uk/tutorial_html/7.-The%20Heap.html

APTR kmalloc(ULONG sz);
ULONG contract(ULONG new_size, Heap *heap);
void free(APTR p, Heap *heap);
void expand(ULONG new_size, Heap *heap);





//
// Create the kernel heap
//
VOID kheap_create(VOID)
{
	// Check page alignment
	if (KHEAP_BASE % 0x1000 != 0)
		kprintf("KHEAP", "Kernel base not page aligned !\n");

	if (KHEAP_TOP % 0x1000 != 0)
		kprintf("KHEAP", "Kernel top not page aligned !\n");


	Heap * heap = &SysBase.KernelHeap;
	heap->Base = KHEAP_BASE;
	heap->Top = KHEAP_TOP;
	heap->Allocated = KHEAP_START_SIZE;
	kprintf("KHEAP", "Kernel heap initialized at [%#010P - %#010P] with an initial size of %i octets (%i Ko)\n", heap->Base, heap->Top, heap->Allocated, heap->Allocated / 1024);

	// Map pages
	for (int virt = KHEAP_BASE; virt < KHEAP_BASE + KHEAP_START_SIZE; virt += PAGE_SIZE)
	{
		phy_addr phys = pmm_alloc_page();
		paging_map_page(phys, virt);

		//ULONG reverse = paging_vir_to_phy(virt);
		//kprintf("", "Mapping (virt)%#010P to (phys)%#010P, reverse => %#010P\n", virt, phys, reverse);
	}



	// Mark the whole heap as free
	heap_header * header = heap->Base;
	header->size = heap->Allocated;
	header->magic = HEAP_MAGIC;
	header->used = FALSE;
	insert_ordered_array((APTR)header, &heap->Index);
}


//
// Dump the content of a heap
//
VOID kheap_dump()
{
	Heap *heap = &SysBase.KernelHeap;

	kprintf("KHEAP", "Dumping the kernel heap [%#010P - %#010P] with a size of %i octets (%i Ko)\n", heap->Base, heap->Top, heap->Allocated, heap->Allocated / 1024);

	ULONG chunkid = 0;
	ULONG total_size = 0;
	heap_header *header = heap->Base;
	while (header < heap->Base + heap->Allocated)
	{
		kprintf("", "#%3i Base:%#010P  (Magic ok: %-3s) Size:%9i  Used: %i\n", chunkid, header, header->magic == HEAP_MAGIC ? "yes" : "no", header->size, header->used);

		total_size += header->size;

		header = (heap_header *)((ULONG)header + header->size);
		chunkid++;
	}

	kprintf("", "Total found size : %i octets (should be %i octets)\n", total_size, heap->Allocated);
}


//
//
APTR alloc(ULONG size, UBYTE page_align, Heap *heap)
{
	// Make sure we take the size of header/footer into account.
	ULONG new_size = size + sizeof(heap_header) + sizeof(heap_footer);

	// Find the smallest hole that will fit.
	LONG iterator = find_smallest_hole(new_size, page_align, heap);

	if (iterator == -1) // If we didn't find a suitable hole
	{
		// Save some previous data.
		ULONG old_length = heap->Allocated - heap->Base;
		ULONG old_end_address = heap->Allocated;

		// We need to allocate some more space.
		expand(old_length + new_size, heap);
		ULONG new_length = heap->Allocated - heap->Base;

		// Find the endmost header. (Not endmost in size, but in location).
		iterator = 0;
		// Vars to hold the index of, and value of, the endmost header found so far.
		LONG idx = -1;
		ULONG value = 0x0;
		while (iterator < heap->Index.size)
		{
			ULONG tmp = (ULONG)lookup_ordered_array(iterator, &heap->Index);
			if (tmp > value)
			{
				value = tmp;
				idx = iterator;
			}
			iterator++;
		}

		// If we didn't find ANY headers, we need to add one.
		if (idx == -1)
		{
			heap_header *header = (heap_header *)old_end_address;
			header->magic = HEAP_MAGIC;
			header->size = new_length - old_length;
			header->used = FALSE;
			heap_footer *footer = (heap_footer *)(old_end_address + header->size - sizeof(heap_footer));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
			insert_ordered_array((void*)header, &heap->Index);
		}
		else
		{
			// The last header needs adjusting.
			heap_header *header = lookup_ordered_array(idx, &heap->Index);
			header->size += new_length - old_length;
			// Rewrite the footer.
			heap_footer *footer = (heap_footer *)((ULONG)header + header->size - sizeof(heap_footer));
			footer->header = header;
			footer->magic = HEAP_MAGIC;
		}
		// We now have enough space. Recurse, and call the function again.
		return alloc(size, page_align, heap);
	}

	heap_header *orig_hole_header = (heap_header *)lookup_ordered_array(iterator, &heap->Index);
	ULONG orig_hole_pos = (ULONG)orig_hole_header;
	ULONG orig_hole_size = orig_hole_header->size;
	// Here we work out if we should split the hole we found into two parts.
	// Is the original hole size - requested hole size less than the overhead for adding a new hole?
	if (orig_hole_size - new_size < sizeof(heap_header) + sizeof(heap_footer))
	{
		// Then just increase the requested size to the size of the hole we found.
		size += orig_hole_size - new_size;
		new_size = orig_hole_size;
	}

	// If we need to page-align the data, do it now and make a new hole in front of our block.
	if (page_align && orig_hole_pos & 0xFFFFF000)
	{
		ULONG new_location = orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - sizeof(heap_header);
		heap_header *hole_header = (heap_header *)orig_hole_pos;
		hole_header->size = 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - sizeof(heap_header);
		hole_header->magic = HEAP_MAGIC;
		hole_header->used = FALSE;
		heap_footer *hole_footer = (heap_footer *)((ULONG)new_location - sizeof(heap_footer));
		hole_footer->magic = HEAP_MAGIC;
		hole_footer->header = hole_header;
		orig_hole_pos = new_location;
		orig_hole_size = orig_hole_size - hole_header->size;
	}
	else
	{
		// Else we don't need this hole any more, delete it from the index.
		remove_ordered_array(iterator, &heap->Index);
	}

	// Overwrite the original header...
	heap_header *block_header = (heap_header *)orig_hole_pos;
	block_header->magic = HEAP_MAGIC;
	block_header->used = TRUE;
	block_header->size = new_size;
	// ...And the footer
	heap_footer *block_footer = (heap_footer *)(orig_hole_pos + sizeof(heap_header) + size);
	block_footer->magic = HEAP_MAGIC;
	block_footer->header = block_header;

	// We may need to write a new hole after the allocated block.
	// We do this only if the new hole would have positive size...
	if (orig_hole_size - new_size > 0)
	{
		heap_header *hole_header = (heap_header *)(orig_hole_pos + sizeof(heap_header) + size + sizeof(heap_footer));
		hole_header->magic = HEAP_MAGIC;
		hole_header->used = FALSE;
		hole_header->size = orig_hole_size - new_size;
		heap_footer *hole_footer = (heap_footer *)((ULONG)hole_header + orig_hole_size - new_size - sizeof(heap_footer));
		if ((ULONG)hole_footer < heap->Allocated)
		{
			hole_footer->magic = HEAP_MAGIC;
			hole_footer->header = hole_header;
		}
		// Put the new hole in the index;
		insert_ordered_array((void*)hole_header, &heap->Index);
	}

	// ...And we're done!
	return (void *)((ULONG)block_header + sizeof(heap_header));
}


//
//
APTR kmalloc_int(ULONG sz, int align, ULONG *phys)
{
	Heap * heap = &SysBase.KernelHeap;
	//if (kheap != 0)
	//{
	APTR addr = alloc(sz, (UBYTE)align, heap);
	if (phys != 0)
	{
		//page_t *page = get_page((ULONG)addr, 0, kernel_directory);
		//*phys = page->frame * 0x1000 + (ULONG)addr & 0xFFF;
	}

	kprintf("KHEAP", "Allocating %i octets at %#010P\n", sz, addr);

	return addr;
	//}
	//else
	//{
	//	if (align == 1 && (placement_address & 0xFFFFF000))
	//	{
	//		// Align the placement address;
	//		placement_address &= 0xFFFFF000;
	//		placement_address += 0x1000;
	//	}
	//	if (phys)
	//	{
	//		*phys = placement_address;
	//	}
	//	ULONG tmp = placement_address;
	//	placement_address += sz;
	//	return tmp;
	//	}
}


//
//
void kfree(APTR p)
{
	free(p, &SysBase.KernelHeap);
}


APTR kmalloc(ULONG size)
{

	return kmalloc_int(size, 0, 0);
}

//
//
static void expand(ULONG new_size, Heap *heap)
{
	Alert(AN_ExecLib, "Need to expand kernel heap !!");
	/*
		// Sanity check.
		ASSERT(new_size > heap->Top - heap->Base);

		// Get the nearest following page boundary.
		if (new_size & 0xFFFFF000 != 0)
		{
		new_size &= 0xFFFFF000;
		new_size += 0x1000;
		}

		// Make sure we are not overreaching ourselves.
		ASSERT(heap->Base + new_size <= heap->max_address);

		// This should always be on a page boundary.
		ULONG old_size = heap->Allocated - heap->Base;

		ULONG i = old_size;
		while (i < new_size)
		{
		alloc_frame(get_page(heap->Base + i, 1, kernel_directory),
		(heap->supervisor) ? 1 : 0, (heap->readonly) ? 0 : 1);
		i += 0x1000;  // page size
		}
		heap->Allocated = heap->Base + new_size;
		*/
}


//
//
static ULONG contract(ULONG new_size, Heap *heap)
{
	Alert(AN_ExecLib, "Need to contract kernel heap !!");
	return heap->Allocated;

	/*
		// Sanity check.
		//ASSERT(new_size < heap->Allocated - heap->Base);

		// Get the nearest following page boundary.
		if (new_size & 0x1000)
		{
		new_size &= 0x1000;
		new_size += 0x1000;
		}

		// Don't contract too far!
		if (new_size < KHEAP_MINSIZE)
		new_size = KHEAP_MINSIZE;

		ULONG old_size = heap->Allocated - heap->Base;
		ULONG i = old_size - 0x1000;
		while (new_size < i)
		{
		free_frame(get_page(heap->Base + i, 0, kernel_directory));
		i -= 0x1000;
		}

		heap->Allocated = heap->Base + new_size;
		return new_size;
		*/
}


//
//
static LONG find_smallest_hole(ULONG size, UBYTE page_align, Heap *heap)
{
	// Find the smallest hole that will fit.
	ULONG iterator = 0;
	while (iterator < heap->Index.size)
	{
		heap_header *header = (heap_header *)lookup_ordered_array(iterator, &heap->Index);
		// If the user has requested the memory be page-aligned
		if (page_align > 0)
		{
			// Page-align the starting point of this header.
			ULONG location = (ULONG)header;
			LONG offset = 0;
			if (((location + sizeof(heap_header)) & 0xFFFFF000) != 0)
				offset = 0x1000 /* page size */ - (location + sizeof(heap_header)) % 0x1000;
			LONG hole_size = (LONG)header->size - offset;
			// Can we fit now?
			if (hole_size >= (LONG)size)
				break;
		}
		else if (header->size >= size)
			break;
		iterator++;
	}
	// Why did the loop exit?
	if (iterator == heap->Index.size)
		return -1; // We got to the end and didn't find anything.
	else
		return iterator;
}


//
//
static BYTE heap_header_less_than(void*a, void *b)
{
	return (((heap_header*)a)->size < ((heap_header*)b)->size) ? 1 : 0;
}


//
//
void free(APTR p, Heap *heap)
{
	// Exit gracefully for null pointers.
	if (p == 0)
		return;

	// Get the header and footer associated with this pointer.
	heap_header *header = (heap_header*)((ULONG)p - sizeof(heap_header));
	heap_footer *footer = (heap_footer*)((ULONG)header + header->size - sizeof(heap_footer));

	// Sanity checks.
	//	ASSERT(header->magic == HEAP_MAGIC);
	//	ASSERT(footer->magic == HEAP_MAGIC);

	// Make us a hole.
	header->used = FALSE;

	// Do we want to add this header into the 'free holes' index?
	char do_add = 1;

	// Unify left
	// If the thing immediately to the left of us is a footer...
	heap_footer *test_footer = (heap_footer*)((ULONG)header - sizeof(heap_footer));
	if (test_footer->magic == HEAP_MAGIC &&
		test_footer->header->used == FALSE)
	{
		ULONG cache_size = header->size; // Cache our current size.
		header = test_footer->header;     // Rewrite our header with the new one.
		footer->header = header;          // Rewrite our footer to point to the new header.
		header->size += cache_size;       // Change the size.
		do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
	}

	// Unify right
	// If the thing immediately to the right of us is a header...
	heap_header *test_header = (heap_header*)((ULONG)footer + sizeof(heap_footer));
	if (test_header->magic == HEAP_MAGIC && !test_header->used)
	{
		header->size += test_header->size; // Increase our size.
		test_footer = (heap_footer*)((ULONG)test_header + // Rewrite it's footer to point to our header.
			test_header->size - sizeof(heap_footer));
		footer = test_footer;
		// Find and remove this header from the index.
		ULONG iterator = 0;
		while ((iterator < heap->Index.size) &&
			(lookup_ordered_array(iterator, &heap->Index) != (void*)test_header))
			iterator++;

		// Make sure we actually found the item.
		//	ASSERT(iterator < heap->Index.size);
		// Remove it.
		remove_ordered_array(iterator, &heap->Index);
	}

	// If the footer location is the end address, we can contract.
	if ((ULONG)footer + sizeof(heap_footer) == heap->Allocated)
	{
		ULONG old_length = heap->Allocated - heap->Base;
		ULONG new_length = contract((ULONG)header - heap->Base, heap);
		// Check how big we will be after resizing.
		if (header->size - (old_length - new_length) > 0)
		{
			// We will still exist, so resize us.
			header->size -= old_length - new_length;
			footer = (heap_footer*)((ULONG)header + header->size - sizeof(heap_footer));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
		}
		else
		{
			// We will no longer exist :(. Remove us from the index.
			ULONG iterator = 0;
			while ((iterator < heap->Index.size) &&
				(lookup_ordered_array(iterator, &heap->Index) != (APTR)test_header))
				iterator++;
			// If we didn't find ourselves, we have nothing to remove.
			if (iterator < heap->Index.size)
				remove_ordered_array(iterator, &heap->Index);
		}
	}

	// If required, add us to the index.
	if (do_add == 1)
		insert_ordered_array((void*)header, &heap->Index);

}
