#ifndef TSS_H_INCLUDED
#define TSS_H_INCLUDED
//****************************************************************************
//**
//**    tss.h
//**
//**	Task State Segment
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[tss.h] platform not implimented. Define ARCH_X86 for HAL"
#endif


#include <exec\types.h>


#ifdef _MSC_VER
#pragma pack (push, 1)
#endif

struct tss_entry 
{
	ULONG prevTss;
	ULONG esp0;
	ULONG ss0;
	ULONG esp1;
	ULONG ss1;
	ULONG esp2;
	ULONG ss2;
	ULONG cr3;
	ULONG eip;
	ULONG eflags;
	ULONG eax;
	ULONG ecx;
	ULONG edx;
	ULONG ebx;
	ULONG esp;
	ULONG ebp;
	ULONG esi;
	ULONG edi;
	ULONG es;
	ULONG cs;
	ULONG ss;
	ULONG ds;
	ULONG fs;
	ULONG gs;
	ULONG ldt;
	UWORD trap;
	UWORD iomap;
};

#ifdef _MSC_VER
#pragma pack (pop, 1)
#endif


void tss_set_stack(UWORD kernelSS, UWORD kernelESP);
void install_tss(ULONG sel, UWORD kernelSS, UWORD kernelESP);


#endif
