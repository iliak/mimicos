#include <exec/exec.h>
#include <proto\exec.h>


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
	start->Size = p4 - (UBYTE *)start;
	header->Free += size;
	return;

}