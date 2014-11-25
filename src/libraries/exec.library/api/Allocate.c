#include <exec/exec.h>
#include <exec/kernel.h>
//#include <exec/macros.h>
#include <proto\exec.h>


//
// Allocate some memory from a Memory Header
//
APTR Allocate(struct MemHeader *header, ULONG size)
{

	if (header == NULL || header->First == NULL || size <= 0)
	{
		kprintf("KERNEL", "Allocate() : One argument is invalid or NULL : header: %#010p, size: %i\n", header, size);
		return NULL;
	}

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


			kprintf("EXEC", "Memory allocated at %#010P (%i octets)\n", next, size);

			return next;
		}

		// Go to next chunk 
		mc = next;
		next = mc->Next;
	}

	kprintf("EXEC", "Allocate() failed !\n");

	return NULL;
}

