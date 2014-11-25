#ifndef GDT_H_INCLUDED
#define GDT_H_INCLUDED
//****************************************************************************
//**
//**    gdt.h
//**
//**	global descriptor table(gdt) for i86 processors. This handles
//**	the basic memory map for the system and permission levels
//**
//**	The system software should have a gdt set up prior to this being
//**	used. This sets up a basic gdt interface that can be managed through
//**	the HAL
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[gdt.h] platform not implimented. Define ARCH_X86 for HAL"
#endif


#include <exec/types.h>



// maximum amount of descriptors allowed
#define MAX_DESCRIPTORS					6

/***	 gdt descriptor access bit flags.	***/

// set access bit
#define I86_GDT_DESC_ACCESS				0x01			//0000 0001

// descriptor is readable and writable. default: read only
#define I86_GDT_DESC_READWRITE			0x02			//0000 0010

// set expansion direction bit
#define I86_GDT_DESC_EXPANSION			0x04			//0000 0100

// executable code segment. Default: data segment
#define I86_GDT_DESC_EXEC_CODE			0x08			//0000 1000

// set code or data descriptor. defult: system defined descriptor
#define I86_GDT_DESC_CODEDATA			0x10			//0001 0000

// set dpl bits
#define I86_GDT_DESC_DPL				0x60			//0110 0000

// set "in memory" bit
#define I86_GDT_DESC_MEMORY				0x80			//1000 0000

/**	gdt descriptor grandularity bit flags	***/

// masks out limitHi(High 4 bits of limit)
#define I86_GDT_GRAND_LIMITHI_MASK		0x0F			//0000 1111

// set os defined bit
#define I86_GDT_GRAND_OS				0x10			//0001 0000

// set if 32bit. default: 16 bit
#define I86_GDT_GRAND_32BIT				0x40			//0100 0000

// 4k grandularity. default: none
#define I86_GDT_GRAND_4K				0x80			//1000 0000



#ifdef _MSC_VER
#pragma pack(push, 1)
#endif
//
// gdt descriptor. A gdt descriptor defines the properties of a specific
// memory block and permissions.
struct gdt_descriptor 
{
	// bits 0-15 of segment limit
	UWORD		limit;

	// bits 0-23 of base address
	UWORD		baseLo;
	UBYTE		baseMid;

	// descriptor access flags
	UBYTE		flags;

	UBYTE		grand;

	// bits 24-32 of base address
	UBYTE		baseHi;
};

#ifdef _MSC_VER
#pragma pack(pop)
#endif



// Setup a descriptor in the Global Descriptor Table
void gdt_set_descriptor(ULONG i, UQUAD base, UQUAD limit, UBYTE access, UBYTE grand);

// returns descritor
struct gdt_descriptor * i86_gdt_get_descriptor(LONG i);

// initializes gdt
LONG i86_gdt_init(void);

#endif
