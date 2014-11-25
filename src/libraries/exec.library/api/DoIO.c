
#include <exec\exec.h>
#include <exec\kernel.h>

#include <proto\exec.h>

//
// perform an I/O command and wait for completion
//
BYTE DoIO(struct IORequest *io)
{
	if (!io || io->io_Device == NULL)
		return -1;

	io->io_Flags = IOF_QUICK;
	io->io_Message.Node.Type = 0;

	BeginIO(io);

	if (!io->io_Flags & IOF_QUICK)
		WaitIO(io);

	return io->io_Error;
}