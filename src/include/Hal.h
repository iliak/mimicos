#ifndef _HAL_H
#define _HAL_H
//****************************************************************************
//**
//**    Hal.h
//**		Hardware Abstraction Layer Interface
//**
//**	The Hardware Abstraction Layer(HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//**	All routines and types are declared extern and must be defined within
//**	external libraries to define specific hal implimentations.
//**
//****************************************************************************

#ifndef ARCH_X86
#pragma error "HAL not implimented for this platform"
#endif

#include <exec/types.h>
#include <exec/interrupts.h>

#ifdef _MSC_VER
#define interrupt __declspec(naked)
#else
#define interrupt
#endif
#define far
#define near

VOID		_cdecl		hal_initialize();									// initialize hardware abstraction layer
LONG		_cdecl		hal_shutdown();										// shutdown hardware abstraction layer
void		_cdecl		enable();											// enables hardware device interrupts
void		_cdecl		halt();												// halt the processor			
void		_cdecl		disable();											// disables hardware device interrupts
void		_cdecl		geninterrupt(LONG n);								// generates interrupt	
UBYTE		_cdecl		inportb(UWORD id);									// reads byte from hardware device port
void		_cdecl		outportb(UWORD id, UBYTE value);					// writes byte to hardware port
UWORD		_cdecl		inportw(UWORD id);									// reads word from hardware device port
void		_cdecl		outportw(UWORD id, UWORD value);					// writes long to hardware port
ULONG		_cdecl		inportl(UWORD id);									// reads long from hardware device port
void		_cdecl		outportl(UWORD id, ULONG value);					// writes word to hardware port
void		_cdecl		setvect(LONG intno, InterruptHandler handler, UWORD flags);		// sets new interrupt vector
void		(_cdecl	far * _cdecl	getvect(LONG intno))( );				// returns current interrupt at interrupt vector
void		_cdecl		interruptdone(ULONG intno);							// notifies hal the interrupt is done
void		_cdecl		sound(ULONG frequency);								// generates sound
const char*	_cdecl		get_cpu_vender();									// returns cpu vender
ULONG		_cdecl		get_tick_count();									// returns current tick count(Only for demo)


void enter_usermode(void);



//
// Insert a magic breakpoint in the code (BOCHS)
//
//static inline void kbreak(void)
//{
//	_asm xchg bx, bx
//}
#define kbreak() _asm 	  \
{						  \
	_asm xchg bx, bx	  \
}						  

//
// Cause a breakpoint in the code (BOCHS)
//
static __inline void bochsbreak(void)
{
	outportw(0x8A00, 0x8A00);
	outportw(0x8A00, 0x08AE0);
}


#endif
