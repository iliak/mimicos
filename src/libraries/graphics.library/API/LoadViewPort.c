#include <exec\kernel.h>

#include <graphics\view.h>

#include "../bochs_vbe.h"
#include "libraries\exec.library\memory\paging.h"
//
//
//
VOID LoadViewPort(struct ViewPort *vp)
{
	kprintf("GFX", "LoadViewPort()\n");

	if (vp == NULL)
		return;
/*
	kprintf("GFX", "vp->Data %#010P\n", vp->Data);
	for (ULONG i = 0; i < vp->Width * vp->Height * vp->Depth; i += 1024)
	{
		ULONG virt = 0xE0000000 + i;
		ULONG phys = paging_vir_to_phy((UBYTE *)vp->Data + i);

		//kprintf("", "Paging (virt)%#010P to (phys)%#010P...", virt, phys);
		//paging_map_page(phys, paging_vir_to_phy((UBYTE)vp->Data + virt));
		paging_map_page(phys, virt);
		//kprintf(NULL, "Ok\n");
	}

	kprintf("", "done\n");
*/
	BgaSetVideoMode(vp->Width, vp->Height, vp->Depth, TRUE, FALSE);

	ULONG * mem = 0xE0000000;
	mem[1] = 0xFFFFFFFF;

}
