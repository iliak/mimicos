
//****************************************************************************
//**
//**    tss.h
//**
//**	Task State Segment
//**
//****************************************************************************


#include <string.h>
#include <exec/kernel.h>
#include "gdt.h"
#include "tss.h"


static struct tss_entry TSS;

void install_tsr(UWORD sel);


void flush_tss(UWORD sel)
{

	_asm
	{
		cli
		mov eax, 0x2b
		ltr eax
		sti
	}

	//	_asm ltr [sel]
}



//
//
//
void tss_set_stack(UWORD kernelSS, UWORD kernelESP)
{
	TSS.ss0 = kernelSS;
	TSS.esp0 = kernelESP;
}


//
//
//
void install_tss(ULONG idx, UWORD kernelSS, UWORD kernelESP)
{
	kprintf("TSS", "Installing TSS\n");

	// install TSS descriptor
	ULONG base = (ULONG)&TSS;

	// install descriptor
	gdt_set_descriptor(idx, base, base + sizeof(struct tss_entry),
		I86_GDT_DESC_ACCESS | I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_DPL | I86_GDT_DESC_MEMORY,
		0);

	// initialize TSS
	memset((void*)&TSS, 0, sizeof(struct tss_entry));

	// set stack and segments
	TSS.ss0 = kernelSS;
	TSS.esp0 = kernelESP;
	TSS.cs = 0x0b;
	TSS.ss = 0x13;
	TSS.es = 0x13;
	TSS.ds = 0x13;
	TSS.fs = 0x13;
	TSS.gs = 0x13;

	// flush tss
	flush_tss(idx * sizeof(struct gdt_descriptor));
}

