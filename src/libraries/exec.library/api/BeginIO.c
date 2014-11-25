
#include <exec\exec.h>
#include <exec\kernel.h>

#include <proto\exec.h>

//
// initiate asynchronous device I/O
//
void BeginIO(struct IORequest *io)
{
	if (!io)
		return;

}