#ifndef _KERNEL_H
#define _KERNEL_H
///***************************************************************************
///*
///*    kernel.h
///*    - Provides access to low level kernel functions
///*
///***************************************************************************

#include <exec/types.h>
#include <string.h>



LONG	kprintf(STRPTR, STRPTR, ...);
//void	kernel_panic(STRPTR fmt, ...);



/*


*/
struct eflag
{
	int _cf : 1;			// Carry Flag: Set by arithmetic instructions which generate either a carry or borrow. Set when an operation generates a carry to or a borrow from a destination operand.
	int _pf : 1;			// Parity flag: Set by most CPU instructions if the least significant (aka the low-order bits) of the destination operand contain an even number of 1's. 
	int _af : 1;			// Auxiliary Carry Flag: Set if there is a carry or borrow involving bit 4 of EAX. Set when a CPU instruction generates a carry to or a borrow from the low-order 4 bits of an operand. This flag is used for binary 
	int _zf : 1;			// Zero Flag: Set by most instructions if the result an operation is binary zero. 
	int _sf : 1;			// Sign Flag: Most operations set this bit the same as the most significant bit (aka high-order bit) of the result. 0 is positive, 1 is negative. 
	int _tf : 1;			// Trap Flag: (sometimes named a Trace Flag.) Permits single stepping of programs. After executing a single instruction, the processor generates an internal exception 1. When Trap Flag is set by a program, the processor generates a single-step interrupt after each instruction. A debugging program can use this feature to execute a program one instruction at a time. 
	int _if : 1;			// Interrupt Enable Flag: when set, the processor recognizes external interrupts on the INTR pin. When set, interrupts are recognized and acted on as they are received. The bit can be cleared to turn off interrupt processing temporarily. 
	int _df : 1;			// Direction Flag: Set and cleared using the STD and CLD instructions. It is used in string processing. When set to 1, string operations process down from high addresses to low addresses. If cleared, string operations process up from low addresses to high addresses. 
	int _of : 1;			// Overflow Flag: Most arithmetic instructions set this bit, indicating that the result was too large to fit in the destination. When set, it indicates that the result of an operation is too large or too small to fit in the destination operand. 
	int _iopl : 1;			// Input/Output privilege level flags: Used in protected mode to generate four levels of security. 
	int _nt : 1;			// Nested Task Flag: Used in protected mode. When set, it indicates that one system task has invoked another via a CALL Instruction, rather than a JMP. 
	int _rf : 1;			// Resume Flag: Used by the debug registers DR6 and DR7. It enables you to turn off certain exceptions while debugging code. 
	int _vm : 1;			// Virtual 8086 Mode flag: Permits 80386 to behave like a high speed 8086. 
	int _ac : 1;			// AC : Alignment Check. Set if alignment checking of memory references is done.
	int _vif : 1;			// VIF : Virtual Interrupt Flag. Virtual image of IF.
	int _vip : 1;			// VIP : Virtual Interrupt Pending flag. Set if an interrupt is pending.
	int _id : 1;			// ID : Identification Flag. Support for CPUID instruction if can be set.
};


/*
// this struct defines the way the registers are stored on the stack during a system call.
struct x86_reg
{
	ULONG	ebx;			 // Base Register
	ULONG	ecx;			 // Counter Register
	ULONG	edx;			 // Data Register
	ULONG	esi;			 // Source Index
	ULONG	edi;			 // Destination Index
	ULONG	ebp;			 // Base Pointer
	ULONG	eax;			 // Accumulator Register
	ULONG	esp;			 // Stack Pointer Register
	ULONG	eflags;			 // Status Register
	ULONG	ds;				 //
	ULONG	es;				 //
	ULONG	fs;				 //
	ULONG	gs;				 //
	//ULONG orig_eax;		 //
	ULONG	eip;			 //
	ULONG	cs;				 //
	ULONG	ss;				 //
	ULONG	cr0, cr2, cr3, cr4;
	ULONG	gtdr, idtr, ldtr, tr;

};

//
//
//
#define save_regs(regs)	_asm				\
{											\
	_asm mov dword ptr[regs].ebx, ebx		\
	_asm mov dword ptr[regs].ecx, ecx		\
	_asm mov dword ptr[regs].edx, edx		\
	_asm mov dword ptr[regs].esi, esi		\
	_asm mov dword ptr[regs].edi, edi		\
	_asm mov dword ptr[regs].ebp, ebp		\
	_asm mov dword ptr[regs].eax, eax		\
	_asm mov dword ptr[regs].esp, esp		\
	_asm mov word ptr[regs].ss, ss			\
	_asm mov word ptr[regs].cs, cs			\
	_asm mov word ptr[regs].ds, ds			\
	_asm mov word ptr[regs].es, es			\
	_asm pushfd								\
	_asm pop dword ptr[regs].eflags			\
	_asm push eax							\
	_asm mov eax, cr0						\
	_asm mov dword ptr[regs].cr0, eax		\
	_asm mov eax, cr2						\
	_asm mov dword ptr[regs].cr2, eax		\
	_asm mov eax, cr3						\
	_asm mov dword ptr[regs].cr3, eax		\
	_asm pop eax							\
}
*/
#define kbreak() _asm 	  \
{						  \
	_asm xchg bx, bx	  \
}						  

#endif
