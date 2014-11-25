#ifndef _PIT_H_INCLUDED
# define _PIT_H_INCLUDED
//****************************************************************************
//**
//**	pit.h
//**		8253 Programmable Interval Timer
//**
//****************************************************************************

#ifndef ARCH_X86
#error "[pit.h] platform not implimented. Define ARCH_X86 for HAL"
#endif


#include <exec/types.h>


//-----------------------------------------------
//	Operational Command Bit masks
//-----------------------------------------------

#define		I86_PIT_OCW_MASK_BINCOUNT		0x01	//00000001
#define		I86_PIT_OCW_MASK_MODE			0x0E	//00001110
#define		I86_PIT_OCW_MASK_RL				0x30	//00110000
#define		I86_PIT_OCW_MASK_COUNTER		0xC0	//11000000

//-----------------------------------------------
//	Operational Command control bits
//-----------------------------------------------

// Use when setting binary count mode
#define		I86_PIT_OCW_BINCOUNT_BINARY		0
#define		I86_PIT_OCW_BINCOUNT_BCD		1

// Use when setting counter mode
#define		I86_PIT_OCW_MODE_TERMINALCOUNT	0x0		//0000
#define		I86_PIT_OCW_MODE_ONESHOT		0x2		//0010
#define		I86_PIT_OCW_MODE_RATEGEN		0x4		//0100
#define		I86_PIT_OCW_MODE_SQUAREWAVEGEN	0x6		//0110
#define		I86_PIT_OCW_MODE_SOFTWARETRIG	0x8		//1000
#define		I86_PIT_OCW_MODE_HARDWARETRIG	0xA		//1010

// Use when setting data transfer
#define		I86_PIT_OCW_RL_LATCH			0x00	//000000
#define		I86_PIT_OCW_RL_LSBONLY			0x10	//010000
#define		I86_PIT_OCW_RL_MSBONLY			0x20	//100000
#define		I86_PIT_OCW_RL_DATA				0x30	//110000

// Use when setting the counter we are working with
#define		I86_PIT_OCW_COUNTER_0			0x00	//00000000
#define		I86_PIT_OCW_COUNTER_1			0x40	//01000000
#define		I86_PIT_OCW_COUNTER_2			0x80	//10000000



// send operational command to pit. Set up command by using the operational
// command bit masks and setting them with the control bits. Shouldn't need to use
// this outside the interface
extern void pit_send_command(UBYTE cmd);

// write data byte to a counter
extern void pit_send_data(UWORD data, UBYTE counter);

// reads data from a counter
extern UBYTE pit_read_data(UWORD counter);

// Sets new pit tick count and returns prev. value
extern ULONG pit_set_tick_count(ULONG i);

// returns current tick count
extern ULONG pit_get_tick_count();

// starts a counter. Counter continues until another call to this routine
extern void pit_start_counter(ULONG freq, UBYTE counter, UBYTE mode);

// Initialize minidriver
extern void _cdecl pit_initialize();

// Test if interface is initialized
extern BOOL _cdecl pit_is_initialized();

#endif
