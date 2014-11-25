#include <exec\exec.h>
#include <proto\exec.h>
#include <exec\kernel.h>

struct Library * OpenLibrary(STRPTR name, ULONG version)
{
	if (!name)
		return NULL;


	return NULL;
}