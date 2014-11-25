//****************************************************************************
//**
//**    TextMode.cpp
//**
//****************************************************************************

#include <exec/console.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <hal.h>

#include <proto\exec.h>


//Bit 7	Blinking
//Bits 6 - 4	Background color
//Bit 3	Bright
//Bit3 2 - 0	Foreground color

void console_putc(UBYTE c);

//static const ULONG VGA_WIDTH = 80;
//static const ULONG VGA_HEIGHT = 25;

static UWORD* const VGA_MEMORY = (UWORD*)0xB8000;

// video memory
//UWORD *video_memory = (UWORD *)0xB8000;


// current position
UBYTE cursor_x = 0;
UBYTE cursor_y = 0;

// current color
UBYTE _color = 0;


#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif

//
// Updates hardware cursor
//
void console_setcursor(LONG x, LONG y)
{

	// get location
	UWORD cursorLocation = y * CONSOLE_WIDTH + x;

	// send location to vga controller to set cursor
	//Disable();
	outportb(0x3D4, 14);
	outportb(0x3D5, cursorLocation >> 8); // Send the high byte.
	outportb(0x3D4, 15);
	outportb(0x3D5, cursorLocation);      // Send the low byte.
	//Enable();

}


//
// Scroll the console
//
void console_scroll()
{

	if (cursor_y >= CONSOLE_HEIGHT)
	{
		UWORD attribute = _color << 8;

		// move current display up one line
		for (int i = 0; i < (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH; i++)
			VGA_MEMORY[i] = VGA_MEMORY[i + CONSOLE_WIDTH];

		// clear the bottom line
		for (int i = (CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH; i < CONSOLE_HEIGHT * CONSOLE_WIDTH; i++)
			VGA_MEMORY[i] = attribute | ' ';

		cursor_y = CONSOLE_HEIGHT - 1;
	}
}



// Displays a character
void console_putc(UBYTE c)
{

	UWORD attribute = _color << 8;

	// backspace character
	if (c == 0x08 && cursor_x)
		cursor_x--;

	// tab character
	else if (c == 0x09)
		cursor_x = (cursor_x + 8) & ~(8 - 1);

	// carriage return
	else if (c == '\r')
		cursor_x = 0;

	// new line
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}

	// printable characters
	else if (c >= ' ')
	{
		// display character on screen
		UWORD* location = VGA_MEMORY + (cursor_y * CONSOLE_WIDTH + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	// if we are at edge of row, go to new line
	if (cursor_x >= CONSOLE_WIDTH)
	{
		cursor_x = 0;
		cursor_y++;
	}

	// if we are at the last line, scroll up
	if (cursor_y >= CONSOLE_HEIGHT)
		console_scroll();

	// update hardware cursor
	console_setcursor(cursor_x, cursor_y);
}



//
// Sets new font color
//
ULONG console_setcolor(UBYTE fore, UBYTE back)
{

	ULONG t = _color;
	_color = fore << 4 | back & 0xf;
	return t;
}


//
// Sets new position
//
void console_goto(ULONG x, ULONG y)
{

	if (cursor_x <= CONSOLE_WIDTH)
		cursor_x = x;

	if (cursor_y <= CONSOLE_HEIGHT)
		cursor_y = y;

	// update hardware cursor to new position
	console_setcursor(cursor_x, cursor_y);
}


//
// returns position
//
void console_getcursor(ULONG * x, ULONG * y)
{
	if (x == 0 || y == 0)
		return;

	*x = cursor_x;
	*y = cursor_y;
}


//
// Clear screen
//
void console_clear(const UBYTE c)
{
	// clear video memory by writing space characters to it
	for (int i = 0; i < CONSOLE_HEIGHT * CONSOLE_WIDTH; i++)
		VGA_MEMORY[i] = ' ' | (c << 8);

	// move position back to start
	console_goto(0, 0);
}


//
// Prints a message to the console
//
void console_print(STRPTR str)
{
	if (!str)
		return;

	ULONG i = 0;
	for (i = 0; i < strlen(str); i++)
	{
		console_putc(str[i]);
	}

}


//
// Displays a formatted string
//
LONG console_printf(STRPTR fmt, ...)
{
	if (!fmt)
		return 0;

	va_list args;
	char buffer[1024];
	
	va_start(args, fmt);
	LONG len = vsprintf(buffer, fmt, args);
	va_end(args);

	console_print(buffer);
	
	return len;
}


