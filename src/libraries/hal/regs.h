#ifndef _REGS_H_INCLUDED
# define _REGS_H_INCLUDED
//****************************************************************************
//**
//**    regs.h
//**
//**	processor register structures and declarations. This interface abstracts
//**	register names behind a common, portable interface
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[regs.h] platform not implimented. Define ARCH_X86 for HAL"
#endif


#include <exec/types.h>


// 32 bit registers
struct _R32BIT
{
    ULONG eax, ebx, ecx, edx, esi, edi, ebp, esp, eflags;
    UBYTE cflag;
};

// 16 bit registers
struct _R16BIT 
{
    UWORD ax, bx, cx, dx, si, di, bp, sp, es, cs, ss, ds, flags;
    UBYTE cflag;
};

// 16 bit registers expressed in 32 bit registers
struct _R16BIT32 
{
    UWORD ax, axh, bx, bxh, cx, cxh, dx, dxh;
	UWORD si, di, bp, sp, es, cs, ss, ds, flags;
	UBYTE cflags;
};

// 8 bit registers
struct _R8BIT 
{
    UBYTE al, ah, bl, bh, cl, ch, dl, dh;
};

// 8 bit registers expressed in 32 bit registers
struct _R8BIT32 
{
    UBYTE al, ah; UWORD axh; 
	UBYTE bl, bh; UWORD bxh; 
	UBYTE cl, ch; UWORD cxh; 
	UBYTE dl, dh; UWORD dxh; 
};

// 8 and 16 bit registers union
union _INTR16 
{
    struct _R16BIT x;
    struct _R8BIT h;
};

// 32 bit, 16 bit and 8 bit registers union
union _INTR32 
{
	struct _R32BIT x;
	struct _R16BIT32 l;
	struct _R8BIT32 h;
};


#endif
