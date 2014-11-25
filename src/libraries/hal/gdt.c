//****************************************************************************
//**
//**    gdt.cpp
//**
//**	global descriptor table(gdt) for i86 processors. This handles
//**	the basic memory map for the system and permission levels
//**
//****************************************************************************

#include "gdt.h"
#include <string.h>

#include <exec\kernel.h>


#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

// processor gdtr register points to base of gdt. This helps
// us set up the pointer
struct gdtr
{
	UWORD	m_limit;		// size of gdt
	ULONG	m_base;			// base address of gdt
};

#ifdef _MSC_VER
#pragma pack(pop, 1)
#endif



// global descriptor table is an array of descriptors
static struct gdt_descriptor	_gdt[MAX_DESCRIPTORS];

// gdtr data
static struct gdtr				_gdtr;



//
// install gdtr
//
static void gdt_install()
{
	kprintf("EXEC", "gdt_install()\n");

	kbreak();

#ifdef _MSC_VER
	_asm 
	{
		lgdt	[_gdtr]

		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax

		; push the address on the stack
		push 0x08
		mov eax, offset flush2
		push eax

		; ret use the previous pushed address
		_emit 0xCB ; => far return

	flush2:
	}
#endif
}



//
// Setup a descriptor in the Global Descriptor Table
//
void gdt_set_descriptor(ULONG index, UQUAD base, UQUAD limit, UBYTE access, UBYTE grand)
{
#ifdef _DEBUG
	kprintf("GDT", "gdt_set_descriptor index: %i, base: %#010X, limit: %#010X, access: %08b, grand: %08b \n", index, (ULONG)base, (ULONG)limit, access, grand);
#endif // DEBUG


	if (index > MAX_DESCRIPTORS)
	{
		kprintf("GDT", "ERROR: index (%i) is too high ! Max value is %i (FILE: %s, LINE: %i).\n", index, MAX_DESCRIPTORS, __FILE__, __LINE__);
		return;
	}

	struct gdt_descriptor *gdt = &_gdt[index];

	// set limit and base addresses
	gdt->baseLo = (UWORD)(base & 0xffff);
	gdt->baseMid = (UBYTE)((base >> 16) & 0xff);
	gdt->baseHi = (UBYTE)((base >> 24) & 0xff);
	gdt->limit = (UWORD)(limit & 0xffff);

	// set flags and grandularity bytes
	gdt->flags = access;
	gdt->grand = (UBYTE)((limit >> 16) & 0x0f);
	gdt->grand |= grand & 0xf0;

}


// returns descriptor in gdt
struct gdt_descriptor* i86_gdt_get_descriptor(LONG index)
{
	if (index > MAX_DESCRIPTORS)
	{
		return NULL;
	}

	return &_gdt[index];
}


// initialize gdt
LONG i86_gdt_init(void)
{
	kprintf("GDT", "init()\n");

	// set up gdtr
	_gdtr.m_limit = (sizeof(struct gdt_descriptor) * MAX_DESCRIPTORS) - 1;
	_gdtr.m_base = (ULONG)&_gdt[0];

	// set null descriptor
	gdt_set_descriptor(0, 0, 0, 0, 0);

	// set default code descriptor
	gdt_set_descriptor(1, 0, 0xffffffff,
		I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	// set default data descriptor
	gdt_set_descriptor(2, 0, 0xffffffff,
		I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);



	// set default user mode code descriptor
	gdt_set_descriptor(3, 0, 0xffffffff,
		I86_GDT_DESC_READWRITE | I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY | I86_GDT_DESC_DPL,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	// set default user mode data descriptor
	gdt_set_descriptor(4, 0, 0xffffffff,
		I86_GDT_DESC_READWRITE | I86_GDT_DESC_CODEDATA | I86_GDT_DESC_MEMORY | I86_GDT_DESC_DPL,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);



	// install gdtr
	gdt_install();

	return 0;
}


