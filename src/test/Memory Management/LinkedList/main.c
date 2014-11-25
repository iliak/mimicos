//
// An exemple of dynamic memory manager with linked lists
//

#include <string.h>
//#include <macros.h>
#include <exec/exec.h>


#define HEAP_SIZE		200
char data[HEAP_SIZE];

struct MemHeader Heap;




void Alert(ULONG number, STRPTR msg)
{


}

void Forbid()
{

}


void Permit()
{


}


//
//
//
APTR Allocate(struct MemHeader *header, ULONG size)
{
	if (header == NULL || header->First == NULL || size <= 0)
		return NULL;

	// Round to the nearest ULONG above
	size = ROUNDUP(size, sizeof(ULONG));


	struct MemChunk *mc = (struct MemChunk *)&header->First;
	struct MemChunk *next = mc->Next;

	// List empty ?
	if (next == NULL)
		return NULL;

	// Follow the list
	while (next)
	{
		// Check if current block is large enough
		if (next->Size >= size)
		{
			// Fits exactly. Just relink the list.
			if (next->Size == size)
			{
				mc->Next = next->Next;
			}
			else
			{
				// Split the current chunk and return the first bytes.
				mc->Next = (struct MemChunk *)((UBYTE *)next + size);
				mc = mc->Next;
				mc->Next = next->Next;
				mc->Size = next->Size - size;
			}


			// Resize the free space in the heder
			header->Free -= size;

			return next;
		}

		// Go to next chunk 
		mc = next;
		next = mc->Next;
	}


	return NULL;
}


//
//
//
void Deallocate(struct MemHeader *header, APTR mem, ULONG size)
{
	if (header == NULL || mem == NULL || size <= 0)
		return;

	struct MemChunk *node = NULL, *next = NULL, *start = NULL;
	UBYTE * p4 = NULL;

	node = (struct MemChunk *)&header->First;			// 
	next = header->First;						// 

	// Start and end(+1) of the block
	start = (struct MemChunk *)mem;					// Start of the chunk to free
	p4 = (UBYTE *)start + size;					// End of the chunk to free

	// No chunk in list? Just insert the current one and return.
	if (next == NULL)
	{
		start->Size = size;
		start->Next = NULL;
		node->Next = start;
		header->Free += size;

		return;
	}




	do
	{
		// Do some constistency checks:
		// 1. All MemChunks must be aligned to sizeof(MemChunk).
		// 2. The end (+1) of the current MemChunk must be lower than the start of the next one.
		if (((ULONG)next | next->Size) & (sizeof(struct MemChunk) - 1)
			|| ((UBYTE *)next + next->Size >= (UBYTE *)next->Next && next->Next != NULL))
		{
			Alert(AN_MemCorrupt | AT_DeadEnd, "");
		}

		// Found a block with a higher address?
		if (next >= start)
		{
			// If the memory to be freed overlaps with the current block something must be wrong.
			if (p4 > (UBYTE *)next)
			{
				Alert(AN_FreeTwice, "");
				return;
			}

			// End the loop with next non-zero
			break;
		}

		// goto next block 
		node = next;
		next = next->Next;

		// If the loop ends with p2 zero add it at the end.
	} while (next != NULL);


	// If there was a previous block merge with it.
	if (node != (struct MemChunk *)&header->First)
	{
		// Check if they overlap. 
		if ((UBYTE *)node + node->Size > (UBYTE *)start)
		{
			Alert(AN_FreeTwice, "");
			return;
		}
		// Merge if possible 
		if ((UBYTE *)node + node->Size == (UBYTE *)start)
			start = node;
		else
			// Not possible to merge 
			node->Next = start;
	}
	else
	{
		//There was no previous block. Just insert the memory at
		//the start of the list.
		node->Next = start;
	}

	// Try to merge with next block (if there is one ;-) ). 
	if (p4 == (UBYTE *)next && next != NULL)
	{
		// Overlap checking already done. Doing it here after
		// the list potentially changed would be a bad idea.
		
		p4 += next->Size;
		next = next->Next;
	}
	// relink the list and return. 
	start->Next = next;
	start->Size= p4 - (UBYTE *)start;
	header->Free += size;
	return;

}


//
//
//
APTR AllocMem(ULONG size, ULONG attributes)
{
	if (size == 0)
		return NULL;


	Forbid();
	APTR mem = Allocate(&Heap, size);
	Permit();


	if (mem == NULL)
		return NULL;


	if (attributes || MEMF_CLEAR)
	{
		memset(mem, attributes, size);
	}


	return mem;
}


//
//
//
void FreeMem(APTR ptr, ULONG size)
{
	if (ptr == NULL)
		return;


	memset(ptr, 0xFFFFFFFF, size);

	Deallocate(&Heap, ptr, size);


	/*
		// Point to the node to free up.
		MemLinkType *node = (MemLinkType *)((char *)ptr - sizeof(MemLinkType));


		// Some form of error checking should go here to ensure they are giving you a valid memory address.


		// Find out the previous free node
		MemLinkType *free = FreePool.Next;
		while ((free->Next) && (free < node))
		{
		free = free->Next;
		}
		MemLinkType *prev = free->Prev; // this will be handy later

		// If memory isn’t corrupted, we should be there!
		// We could just stick the link in order right here,
		// but that doesn’t help us cleanup if we can.
		// First let’s see if we can combine this newly freed link
		// with the one before it. This means we would simply add our
		// size + 16 to the previous link’s size and this link would
		// disappear. But we can’t add our size to FreePool. He’s a dummy
		if (free->Prev != &FreePool) // Can’t be FreePool
		{
		if ((char *)node == ((char *)prev + (prev->Size + sizeof(MemLinkType))))
		{
		// add our size and link size (16), then go away!
		prev->Size += node->Size + sizeof(MemLinkType);

		memset((char *)node - sizeof(MemLinkType), 0, node->Size);
		return;
		}
		}

		// If we got here, we couldn’t be combined with the previous.
		// In this case, we must insert ourselves in the free list
		prev->Next = node;
		free->Prev = node;
		node->Next = free;
		node->Prev = prev;
		//node->Owner = osowner;

		// Now we’ll try to combine Next with us!
		if (((char *)node + node->Size + sizeof(MemLinkType)) == (char *)free)
		{
		// We can combine them. pNext will go away!
		node->Size += free->Size + sizeof(MemLinkType);
		node->Next = free->Next;
		node = free->Next;

		if (node) // the next could have been the last!
		node->Prev = node; // fix backlink
		}

		memset((char *)node + sizeof(MemLinkType), 0xCC, node->Size);

		// If we didn’t combine the new link with the last one,
		// we just leave it were we inserted it and report no error
		return;
		*/
}



//
//
//
void InitMemMgmt(char * base, ULONG size)
{
	// Clear the heap...
	memset(base, 0xAAAAAAAA, size);

	struct MemChunk * mc = (struct MemChunk *)base;

	Heap.Node.Type = NT_MEMORY;
	Heap.Node.Succ = NULL;
	Heap.Node.Pred = NULL;
	Heap.Node.Name = "Heap";
	Heap.Node.Pri = 0;

	Heap.First = mc;
	Heap.Lower = mc;
	Heap.Upper = (APTR)((ULONG)mc + size);
	Heap.Free = size;

	mc->Next = NULL;
	mc->Size = size;



}



//
//
//
int main(void)
{
	InitMemMgmt(data, HEAP_SIZE);


	APTR t1 = NULL,
		t2 = NULL,
		t3 = NULL,
		t4 = NULL;

	t1 = AllocMem(12, 0x11);
	t2 = AllocMem(12, 0x22);
	t3 = AllocMem(12, 0x33);
	t4 = AllocMem(12, 0x44);

	FreeMem(t3, 12);
	FreeMem(t2, 12);
	FreeMem(t1, 12);
	FreeMem(t4, 12);

	return 0;
}

