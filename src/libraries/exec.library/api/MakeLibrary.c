#include <exec\exec.h>
#include <proto\exec.h>


struct Library * MakeLibrary(const char *funcInit, const char *structInit, ULONG(*CONST libInit)(), ULONG dataSize, ULONG segList)
//struct Library * MakeLibrary(APTR vector, struct InitStruct *structure, APTR init, ULONG size, ULONG seglist)
{

	return NULL;
}