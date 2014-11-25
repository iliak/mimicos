#include <libraries/expansion.h>
#include <libraries/expansionbase.h>
#include <exec\kernel.h>

#include "pci.h"

#include <proto\expansion.h>


struct ExpansionBase ExpansionBase;


VOID expansion_init(VOID)
{
	kprintf("EXPANS", "expansion_init()\n");

	ExpansionBase.LibNode.IdString = EXPANSIONNAME;


	// Scan PCI bus for all devices
	FindConfigDev(NULL, -1, -1);

	//pci_scan();

}