//****************************************************************************
//**
//**    pit.cpp
//**		8253 Programmable Interval Timer
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "idt.h"
#include "pit.h"
#include "pic.h"
#include <hal.h>

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

//-----------------------------------------------
//	Controller Registers
//-----------------------------------------------

#define		I86_PIT_REG_COUNTER0		0x40
#define		I86_PIT_REG_COUNTER1		0x41
#define		I86_PIT_REG_COUNTER2		0x42
#define		I86_PIT_REG_COMMAND			0x43



// Global Tick count
static volatile ULONG			_pit_ticks = 0;

// Test if pit is initialized
static BOOL						_pit_bIsInit = FALSE;



// pit timer interrupt handler
void _cdecl pit_irq();


//	pit timer interrupt handler
void _cdecl pit_irq()
{
	_asm add esp, 12
	_asm pushad

	// increment tick count
	_pit_ticks++;

	//kprintf("PIT", "0x%08X\n", _pit_ticks);

	// tell hal we are done
	interruptdone(0);

	_asm popad
	_asm iretd
}


// Sets new pit tick count and returns prev. value
ULONG pit_set_tick_count(ULONG i)
{
	ULONG ret = _pit_ticks;
	_pit_ticks = i;
	return ret;
}


// returns current tick count
ULONG pit_get_tick_count()
{
	return _pit_ticks;
}


// send command to pit
void pit_send_command(UBYTE cmd)
{
	outportb(I86_PIT_REG_COMMAND, cmd);
}


// send data to a counter
void pit_send_data(UWORD data, UBYTE counter)
{
	UBYTE port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	outportb(port, (UBYTE)data);
}


// read data from counter
UBYTE pit_read_data(UWORD counter)
{
	UBYTE port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	return inportb(port);
}


// starts a counter
void pit_start_counter(ULONG freq, UBYTE counter, UBYTE mode)
{
	if (freq == 0)
		return;

	kprintf("PIT", "Starting counter %i @ %iHz\n", counter, freq);

	UWORD divisor = (UWORD)(1193181 / (UWORD)freq);

	// send operational command
	UBYTE ocw = 0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | counter;
	pit_send_command(ocw);

	// set frequency rate
	pit_send_data(divisor & 0xff, 0);
	pit_send_data((divisor >> 8) & 0xff, 0);

	// reset tick count
	_pit_ticks = 0;
}


// initialize minidriver
void _cdecl pit_initialize()
{
	kprintf("PIT", "init()\n");

	// Install our interrupt handler(irq 0 uses interrupt 32)
	setvect(32, pit_irq, 0);

	// we are initialized
	_pit_bIsInit = TRUE;
}


// test if pit interface is initialized
BOOL _cdecl pit_is_initialized()
{

	return _pit_bIsInit;
}

