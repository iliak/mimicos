#include <exec\kernel.h>
#include <graphics\gfxbase.h>


#include <proto\graphics.h>
#include <proto\intuition.h>


#include "bochs_vbe.h"

VOID graphics_init(VOID)
{
	kprintf("GFX", "graphics_init()\n");


	ULONG ret = BgaReadRegister(VBE_DISPI_INDEX_ID);
	kprintf("GFX", "Current VBE version = 0x%x\n", ret);


}