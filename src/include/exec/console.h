#ifndef _CONSOLE_H
#define _CONSOLE_H
//****************************************************************************
//**
//**    console.h
//**    - Provides display capabilities for displaying text in text mode
//**
//****************************************************************************

#include <exec/types.h>

#define	CONSOLE_HEIGHT					25
#define CONSOLE_WIDTH					80

#define CONSOLE_COLOR_BLACK				0x0
#define CONSOLE_COLOR_BLUE				0x1
#define CONSOLE_COLOR_GREEN				0x2
#define CONSOLE_COLOR_CYAN				0x3
#define CONSOLE_COLOR_RED				0x4
#define CONSOLE_COLOR_MAGENTA			0x5
#define CONSOLE_COLOR_BROWN				0x6
#define CONSOLE_COLOR_LIGHT_GRAY		0x7
#define CONSOLE_COLOR_DARK_GRAY			0x8
#define CONSOLE_COLOR_LIGHT_BLUE		0x9
#define CONSOLE_COLOR_LIGHT_GREEN		0xA
#define CONSOLE_COLOR_LIGHT_CYAN		0xB
#define CONSOLE_COLOR_LIGHT_RED			0xC
#define CONSOLE_COLOR_LIGHT_MAGENTA		0xD
#define CONSOLE_COLOR_YELLOW			0xE
#define CONSOLE_COLOR_WHITE				0xF

#define	CONSOLE_COLOR_BLINK				0x8


enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};



void	console_clear(const UBYTE c);
void	console_print(STRPTR);
LONG	console_printf(STRPTR, ...);
ULONG	console_setcolor(UBYTE fore, UBYTE back);
void	console_goto(ULONG x, ULONG y);
void	console_getcursor(ULONG * x, ULONG * y);
void	console_setcursor(LONG x, LONG y);

#endif
