
#include <exec/kernel.h>
#include <Hal.h>
#include <string.h>

#include "kserial.h"


//
// http://wiki.osdev.org/Serial_Ports
//

BOOL	isSerialInit = FALSE;

//
//	serial irq handler
//
void _cdecl kserial_irq()
{

	_asm add esp, 12
	_asm pushad
	_asm cli

	kprintf("KSERIAL", "serial_irq\n");


	// tell hal we are done
	interruptdone(SERIAL1_IRQ);


	_asm sti
	_asm popad
	_asm iretd
}


//
//
//
void kserial_init()
{
	if (isSerialInit)
		return;


	outportb(SERIAL_IO_PORT1 + 1, 0x00);    // Disable all interrupts
	outportb(SERIAL_IO_PORT1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outportb(SERIAL_IO_PORT1 + 0, 0x02);    // Set divisor to 3 (lo byte) 38400 baud
	outportb(SERIAL_IO_PORT1 + 1, 0x00);    //                  (hi byte)
	outportb(SERIAL_IO_PORT1 + 3, 0x03);    // 8 bits, no parity, one stop bit
	outportb(SERIAL_IO_PORT1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outportb(SERIAL_IO_PORT1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set



	// Install our interrupt handler(irq 1 uses interrupt 33)
	setvect(SERIAL1_IRQ, kserial_irq, 0);

	isSerialInit = TRUE;
	kprintf("KSERIAL", "init()#####################################################################################################\n");

}


//
//
//
UBYTE kserial_received()
{
	return inportb(SERIAL_IO_PORT1 + 5) & 1;
}


//
//
//
char kserial_read()
{
	if (!isSerialInit)
		return NULL;

	while (kserial_received() == 0)
	{

	}

	return inportb(SERIAL_IO_PORT1);
}


//
//
//
int kserial_is_transmit_empty()
{
	if (!isSerialInit)
		return NULL;

	return inportb(SERIAL_IO_PORT1 + 5) & 0x20;
}


//
//
//
void kserial_write(char a)
{
	if (!isSerialInit)
		return;

	while (kserial_is_transmit_empty() == 0)
	{

	}

	outportb(SERIAL_IO_PORT1, a);
}


//
// Prints some text on the serial port
//
void kserial_print(STRPTR str)
{
	if (!str || !isSerialInit)
		return;

	ULONG i = 0;
	for (i = 0; i < strlen(str); i++)
	{
		kserial_write(str[i]);
	}

}

