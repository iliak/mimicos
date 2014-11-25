#include <Hal.h>

#include "bochs_vbe.h"


// http://qlibdos32.sourceforge.net/tutor/tutor-vesa.php
// http://brokenthorn.com/Resources/OSDevVid2.html
// http://stackoverflow.com/questions/16997250/bochs-with-graphics


//
//
//
UWORD BgaReadRegister(UWORD IndexValue)
{
	outportw(VBE_DISPI_IOPORT_INDEX, IndexValue);
	return inportw(VBE_DISPI_IOPORT_DATA);
}


//
//
//
void BgaWriteRegister(UWORD IndexValue, UWORD DataValue)
{
	outportw(VBE_DISPI_IOPORT_INDEX, IndexValue);
	outportw(VBE_DISPI_IOPORT_DATA, DataValue);
}


//
//
//
BOOL BgaIsAvailable(void)
{
	return (BgaReadRegister(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
}

//
//
//
void BgaSetVideoMode(UWORD Width, UWORD Height, UWORD BitDepth, BOOL UseLinearFrameBuffer, BOOL ClearVideoMemory)
{
	BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
	BgaWriteRegister(VBE_DISPI_INDEX_XRES, Width);
	BgaWriteRegister(VBE_DISPI_INDEX_YRES, Height);
	BgaWriteRegister(VBE_DISPI_INDEX_BPP, BitDepth);
	BgaWriteRegister(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
		(UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) |
		(ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));
}


//
//
//
void BgaSetBank(UWORD BankNumber)
{
	BgaWriteRegister(VBE_DISPI_INDEX_BANK, BankNumber);
}


//
//
//
VOID BgaGetControllerInfo(struct VbeInfoBlock *info)
{
	if (info == NULL)
		return;
}
