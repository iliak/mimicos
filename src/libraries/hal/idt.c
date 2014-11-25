//****************************************************************************
//**
//**    Idt.cpp
//**		Interrupt Descriptor Table. The IDT is responsible for providing
//**	the interface for managing interrupts, installing, setting, requesting,
//**	generating, and interrupt callback managing.
//**
//****************************************************************************

// Master PIC
// 
// IRQ 0 — system timer(cannot be changed);
// IRQ 1 — keyboard controller(cannot be changed);
// IRQ 2 — cascaded signals from IRQs 8–15;
// — any devices configured to use IRQ 2 will actually be using IRQ 9
// 
// IRQ 3 — serial port controller for serial port 2 (shared with serial port 4, if present);
// IRQ 4 — serial port controller for serial port 1 (shared with serial port 3, if present);
// IRQ 5 — parallel port 2 and 3  or  sound card;
// IRQ 6 — floppy disk controller;
// IRQ 7 — parallel port 1. It is used for printers or for any parallel port if a printer is not present.It can also be potentially be shared with a secondary sound card with careful management of the port.
// 
// 
// Slave PIC
// 
// IRQ 8 — real - time clock
// IRQ 9 — Advanced Configuration and Power Interface system control interrupt on Intel chipsets.[1] Other chipset manufacturers might use another interrupt for this purpose, or make it available for the use of peripherals.
// 
// — any devices configured to use IRQ 2 will actually be using IRQ 9
// 
// IRQ 10 — The Interrupt is left open for the use of peripherals.open interrupt / available  or  SCSI  or  NIC;
// IRQ 11 — The Interrupt is left open for the use of peripherals.open interrupt / available  or  SCSI  or  NIC;
// IRQ 12 — mouse on PS / 2 connector;
// IRQ 13 — CPU co - processor  or  integrated floating point unit  or  inter - processor interrupt(use depends on OS);
// IRQ 14 — primary ATA channel;
// IRQ 15 — secondary ATA channel;


#include <string.h>
#include <hal.h>
#include <exec\kernel.h>
#include <exec/console.h>
#include <exec\cpu.h>


#include "idt.h"



#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

// describes the structure for the processors idtr register
struct idtr
{
	// size of the interrupt descriptor table(idt)
	UWORD		limit;

	// base address of idt
	ULONG		base;
};

#ifdef _MSC_VER
#pragma pack(pop, 1)
#endif



// interrupt descriptor table
static struct idt_descriptor	_idt[I86_MAX_INTERRUPTS];

// idtr structure used to help define the cpu's idtr register
static struct idtr				_idtr;


static void idt_install();						// installs idtr into processors idtr register
static void i86_default_handler();				// default int handler used to catch unregistered interrupts


// installs idtr into processors idtr register
static void idt_install()
{
#ifdef _MSC_VER
	_asm lidt	[_idtr]
#endif
}


struct x86_reg regs;

// default handler to catch unhandled system interrupts.
static void i86_default_handler()
{
	save_regs(regs);

	_asm
	{
		cli
		sub		ebp, 4
	}


	// clear interrupts to prevent double fault
	disable();

	// print debug message and halt
#ifdef DEBUG
	kprintf("IDT", "i86_default_handler: Unhandled Exception :\n");
	dump_regs(&regs);
	console_setcolor(0x1, 0xe);
	console_printf("*** [i86 Hal] i86_default_handler: Unhandled Exception");
#endif

	halt();

	for (;;);
}


// returns interrupt descriptor
struct idt_descriptor* i86_get_ir(ULONG i)
{
	if (i > I86_MAX_INTERRUPTS)
		return 0;

	return &_idt[i];
}


// installs a new interrupt handler
LONG i86_install_ir(ULONG index, UWORD flags, UWORD sel, InterruptHandler irq)
{
	if (index > I86_MAX_INTERRUPTS || !irq)
	{
		kprintf("IDT", "ERROR: index (%i) is too high ! Max value is %i. (FILE: %s, LINE: %i)\n", index, I86_MAX_INTERRUPTS, __FILE__, __LINE__);
		return 0;
	}

	// get base address of interrupt handler
	UQUAD		uiBase = (UQUAD)&(*irq);

	// store base address into idt
	_idt[index].baseLo = (UWORD)(uiBase & 0xffff);
	_idt[index].baseHi = (UWORD)((uiBase >> 16) & 0xffff);
	_idt[index].reserved = 0;
	_idt[index].flags = (UBYTE)(flags);
	_idt[index].selector = sel;

	return	0;
}


// initialize idt
// codeSel = offset within the GDT
LONG i86_idt_init(UWORD codeSel)
{
	kprintf("IDT", "init()\n");

	// set up idtr for processor
	_idtr.limit = sizeof(struct idt_descriptor) * I86_MAX_INTERRUPTS - 1;
	_idtr.base = (ULONG)&_idt[0];

	// null out the idt
	memset((APTR)&_idt[0], 0x0, sizeof(struct idt_descriptor) * I86_MAX_INTERRUPTS - 1);

	// register default handlers
	for (int i = 0; i < I86_MAX_INTERRUPTS; i++)
		i86_install_ir(i, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
			codeSel, (InterruptHandler)i86_default_handler);

	// install our idt
	idt_install();

	return 0;
}

