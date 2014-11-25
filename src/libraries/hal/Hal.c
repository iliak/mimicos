
//****************************************************************************
//**
//**    Hal.cpp
//**		Hardware Abstraction Layer for i86 architecture
//**
//**	The Hardware Abstraction Layer(HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[hal.cpp for i86] requires i86 architecture. Define ARCH_X86"
#endif

#include <exec\kernel.h>
#include <Hal.h>
#include "cpu.h"
#include "pic.h"
#include "pit.h"
#include "idt.h"


// initialize hardware devices
VOID _cdecl hal_initialize()
{

	kprintf("HAL", "init()\n");

	// disable hardware interrupts
	disable();

	// initialize motherboard controllers and system timer
	i86_cpu_initialize();
	pic_initialize(0x20, 0x28);
	pit_initialize();
	pit_start_counter(100, I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);

	// enable interrupts
	//enable();
}

// shutdown hardware devices
LONG _cdecl hal_shutdown()
{
	// shutdown system resources
	i86_cpu_shutdown();
	return 0;
}


// generate i86 interrupt request
void _cdecl geninterrupt(LONG n)
{
#ifdef _MSC_VER
	_asm
	{
		mov al, byte ptr[n]
		mov byte ptr[genint + 1], al
		jmp genint
		genint :
		int 0	// above code modifies the 0 to int number to generate
	}
#endif
}

// notifies hal interrupt is done
void _cdecl	interruptdone(ULONG intno)
{
	// insure its a valid hardware irq
	if (intno > 16)
		return;

	// test if we need to send end-of-interrupt to second pic
	if (intno >= 8)
		pic_send_command(I86_PIC_OCW2_MASK_EOI, 1);

	// always send end-of-interrupt to primary pic
	pic_send_command(I86_PIC_OCW2_MASK_EOI, 0);
}


// output sound to speaker
void _cdecl	sound(ULONG frequency)
{
	// sets frequency for speaker. frequency of 0 disables speaker
	outportb(0x61, 3 | (UBYTE)(frequency << 2));
}






// read byte from device using port mapped io
UBYTE _cdecl inportb(UWORD portid)
{
#ifdef _MSC_VER
	_asm
	{
		mov		dx, word ptr[portid]
		in		al, dx
		mov		byte ptr[portid], al
	}
#endif
	return (UBYTE)portid;
}

// write byte to device through port mapped io
void _cdecl outportb(UWORD portid, UBYTE value)
{
#ifdef _MSC_VER
	_asm 
	{
		mov		al, byte ptr[value]
		mov		dx, word ptr[portid]
		out		dx, al
	}
#endif
}


// read word from device using port mapped io
UWORD _cdecl inportw(UWORD portid)
{
	UWORD ret = 0;
	_asm 
	{
		mov		dx, word ptr[portid]
		in		eax, dx
		mov		ret, ax
	}
	return ret;

}

// write word to device through port mapped io
void _cdecl outportw(UWORD portid, ULONG value)
{
	_asm
	{
		push	eax
		mov		eax, value
		mov		dx, word ptr[portid]
		out		dx, eax
		pop		eax
	}
}





// 
void _cdecl outportl(UWORD portid, ULONG value)
{
#ifdef _MSC_VER
	_asm 
	{
		push	eax
		mov		eax, long ptr[value]
		mov		dx, word ptr[portid]
		out		dx, eax
		pop		eax
	}
#endif
}


// read long from device using port mapped io
ULONG _cdecl inportl(UWORD portid)
{
#ifdef _MSC_VER
	_asm
	{
		mov		dx, word ptr[portid]
		in		eax, dx
		mov		long ptr[portid], eax
	}
#endif
	return(ULONG)portid;
}



// enable all hardware interrupts
void _cdecl enable()
{
#ifdef _DEBUG
	kprintf("HAL", "enable interrupts\n");
#endif

#ifdef _MSC_VER
	_asm sti
#endif
}


// disable all hardware interrupts
void _cdecl disable()
{
#ifdef _DEBUG
	kprintf("HAL", "disable interrupts\n");
#endif

#ifdef _MSC_VER
	_asm cli
#endif
}

// halt processor
void _cdecl halt()
{
#ifdef _DEBUG
	kprintf("HAL", "Halting processor\n");
#endif
#ifdef _MSC_VER
	_asm hlt
#endif
}



// sets new interrupt vector
// void _cdecl setvect(int intno, void(_cdecl far &vect)())
void _cdecl	setvect(LONG intno, InterruptHandler handler, UWORD flags)
//void _cdecl setvect(LONG intno, void(_cdecl far &handler)(), UWORD flags)
{
	// install interrupt handler! This overwrites prev interrupt descriptor
	i86_install_ir(intno, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32 | flags, 0x8, handler);
}


// returns current interrupt vector
//void(_cdecl far * _cdecl	getvect(LONG intno))()
InterruptHandler getvect(LONG number)
{
	// get the descriptor from the idt
	struct idt_descriptor* desc = i86_get_ir(number);
	if (!desc)
		return 0;

	// get address of interrupt handler
	ULONG addr = desc->baseLo | (desc->baseHi << 16);

	// return interrupt handler
	return (InterruptHandler)addr;
}


// returns cpu vender
const char* _cdecl get_cpu_vender()
{
	return i86_cpu_get_vender();
}


// returns current tick count(only for demo)
ULONG _cdecl get_tick_count()
{
	return pit_get_tick_count();
}




//
// Switch to user mode
// physic: 0x7e70, virtual: 0xC0004E7A
void enter_usermode(void)
{
	kbreak();

	_asm
	{
		cli
		mov ax, 0x23			// user mode data selector is 0x20 (GDT entry 3).Also sets RPL to 3
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax

		push 0x23				// SS, notice it uses same selector as above
		push esp				// ESP
		pushfd					// EFLAGS

		pop eax
		or eax, 0x200			// enable IF in EFLAGS
		push eax

		push 0x1b				// CS, user mode code selector is 0x18.With RPL 3 this is 0x1b
		lea eax, [a]			// EIP first
		push eax


		iretd

		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
		xchg bx, bx
	a :
		add esp, 4				// fix stack
	}
}

