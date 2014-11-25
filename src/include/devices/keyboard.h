
#ifndef _KKYBRD_DRIVER_H
#define _KKYBRD_DRIVER_H
//****************************************************************************
//**
//**    driver.h
//**
//****************************************************************************

//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <exec/types.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

enum KEYCODE 
{

// Alphanumeric keys ////////////////
	KEY_SPACE             = ' ',
	KEY_0                 = '0',
	KEY_1                 = '1',
	KEY_2                 = '2',
	KEY_3                 = '3',
	KEY_4                 = '4',
	KEY_5                 = '5',
	KEY_6                 = '6',
	KEY_7                 = '7',
	KEY_8                 = '8',
	KEY_9                 = '9',

	KEY_a                 = 'a',
	KEY_b                 = 'b',
	KEY_c                 = 'c',
	KEY_d                 = 'd',
	KEY_e                 = 'e',
	KEY_f                 = 'f',
	KEY_g                 = 'g',
	KEY_h                 = 'h',
	KEY_i                 = 'i',
	KEY_j                 = 'j',
	KEY_k                 = 'k',
	KEY_l                 = 'l',
	KEY_m                 = 'm',
	KEY_n                 = 'n',
	KEY_o                 = 'o',
	KEY_p                 = 'p',
	KEY_q                 = 'q',
	KEY_r                 = 'r',
	KEY_s                 = 's',
	KEY_t                 = 't',
	KEY_u                 = 'u',
	KEY_v                 = 'v',
	KEY_w                 = 'w',
	KEY_x                 = 'x',
	KEY_y                 = 'y',
	KEY_z                 = 'z',

	KEY_A                 = 'A',
	KEY_B                 = 'B',
	KEY_C                 = 'C',
	KEY_D                 = 'D',
	KEY_E                 = 'E',
	KEY_F                 = 'F',
	KEY_G                 = 'G',
	KEY_H                 = 'H',
	KEY_I                 = 'I',
	KEY_J                 = 'J',
	KEY_K                 = 'K',
	KEY_L                 = 'L',
	KEY_M                 = 'M',
	KEY_N                 = 'N',
	KEY_O                 = 'O',
	KEY_P                 = 'P',
	KEY_Q                 = 'Q',
	KEY_R                 = 'R',
	KEY_S                 = 'S',
	KEY_T                 = 'T',
	KEY_U                 = 'U',
	KEY_V                 = 'V',
	KEY_W                 = 'W',
	KEY_X                 = 'X',
	KEY_Y                 = 'Y',
	KEY_Z                 = 'Z',

	KEY_RETURN            = '\r',
	KEY_ESCAPE            = 0x1001,
	KEY_BACKSPACE         = '\b',

// Arrow keys ////////////////////////
	KEY_UP                = 0x1100,
	KEY_DOWN              = 0x1101,
	KEY_LEFT              = 0x1102,
	KEY_RIGHT             = 0x1103,

// Function keys /////////////////////
	KEY_F1                = 0x1201,
	KEY_F2                = 0x1202,
	KEY_F3                = 0x1203,
	KEY_F4                = 0x1204,
	KEY_F5                = 0x1205,
	KEY_F6                = 0x1206,
	KEY_F7                = 0x1207,
	KEY_F8                = 0x1208,
	KEY_F9                = 0x1209,
	KEY_F10               = 0x120a,
	KEY_F11               = 0x120b,
	KEY_F12               = 0x120b,
	KEY_F13               = 0x120c,
	KEY_F14               = 0x120d,
	KEY_F15               = 0x120e,

	KEY_DOT               = '.',
	KEY_COMMA             = ',',
	KEY_COLON             = ':',
	KEY_SEMICOLON         = ';',
	KEY_SLASH             = '/',
	KEY_BACKSLASH         = '\\',
	KEY_PLUS              = '+',
	KEY_MINUS             = '-',
	KEY_ASTERISK          = '*',
	KEY_EXCLAMATION       = '!',
	KEY_QUESTION          = '?',
	KEY_QUOTEDOUBLE       = '\"',
	KEY_QUOTE             = '\'',
	KEY_EQUAL             = '=',
	KEY_HASH              = '#',
	KEY_PERCENT           = '%',
	KEY_AMPERSAND         = '&',
	KEY_UNDERSCORE        = '_',
	KEY_LEFTPARENTHESIS   = '(',
	KEY_RIGHTPARENTHESIS  = ')',
	KEY_LEFTBRACKET       = '[',
	KEY_RIGHTBRACKET      = ']',
	KEY_LEFTCURL          = '{',
	KEY_RIGHTCURL         = '}',
	KEY_DOLLAR            = '$',
	KEY_POUND             = '£',
	KEY_EURO              = '$',
	KEY_LESS              = '<',
	KEY_GREATER           = '>',
	KEY_BAR               = '|',
	KEY_GRAVE             = '`',
	KEY_TILDE             = '~',
	KEY_AT                = '@',
	KEY_CARRET            = '^',

// Numeric keypad //////////////////////

	KEY_KP_0              = '0',
	KEY_KP_1              = '1',
	KEY_KP_2              = '2',
	KEY_KP_3              = '3',
	KEY_KP_4              = '4',
	KEY_KP_5              = '5',
	KEY_KP_6              = '6',
	KEY_KP_7              = '7',
	KEY_KP_8              = '8',
	KEY_KP_9              = '9',
	KEY_KP_PLUS           = '+',
	KEY_KP_MINUS          = '-',
	KEY_KP_DECIMAL        = '.',
	KEY_KP_DIVIDE         = '/',
	KEY_KP_ASTERISK       = '*',
	KEY_KP_NUMLOCK        = 0x300f,
	KEY_KP_ENTER          = 0x3010,

	KEY_TAB               = 0x4000,
	KEY_CAPSLOCK          = 0x4001,

// Symbols ////////////////////////////////
	KEY_EACUTE				= 0x82,		// é
	KEY_EGRAVE				= 0x8A,		// è
	KEY_CEDILLA				= 0x87,		// ç
	KEY_AGRAVE				= 0x85,		// à
	KEY_UGRAVE				= 0x97,		// ù
	KEY_SECTION				= 0xE1,		// §
	KEY_DEGREE				= 0xF8,		// °
	KEY_MICRO				= 0xE6,		// µ
	KEY_SQUARE				= 0xFD,		// ²

// Modify keys ////////////////////////////
	KEY_LSHIFT            = 0x4002,
	KEY_LCTRL             = 0x4003,
	KEY_LALT              = 0x4004,
	KEY_LWIN              = 0x4005,
	KEY_RSHIFT            = 0x4006,
	KEY_RCTRL             = 0x4007,
	KEY_RALT              = 0x4008,
	KEY_RWIN              = 0x4009,

	KEY_INSERT            = 0x400a,
	KEY_DELETE            = 0x400b,
	KEY_HOME              = 0x400c,
	KEY_END               = 0x400d,
	KEY_PAGEUP            = 0x400e,
	KEY_PAGEDOWN          = 0x400f,
	KEY_SCROLLLOCK        = 0x4010,
	KEY_PAUSE             = 0x4011,

	KEY_UNKNOWN				= 0xff,
	KEY_NUMKEYCODES
};

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! returns status of lock keys
extern BOOL		kkybrd_get_scroll_lock();
extern BOOL		kkybrd_get_numlock();
extern BOOL		kkybrd_get_capslock();

//! returns status of special keys
extern BOOL		kkybrd_get_alt();
extern BOOL		kkybrd_get_ctrl();
extern BOOL		kkybrd_get_shift();

//! resend last command
extern void		kkybrd_ignore_resend();
extern BOOL		kkybrd_check_resend();

//! returns status of tests. kkybrd_self_test runs the test
extern BOOL		kkybrd_get_diagnostic_res();
extern BOOL		kkybrd_get_bat_res();
extern BOOL		kkybrd_self_test();

//! returns last scan code, last keystroke
extern UBYTE	kkybrd_get_last_scan();
extern enum KEYCODE	kkybrd_get_last_key();
extern void		kkybrd_discard_last_key();

//! updates LEDs
extern void		kkybrd_set_leds (BOOL num, BOOL caps, BOOL scroll);

//! keyboard enable / disable
extern void		kkybrd_disable();
extern void		kkybrd_enable();
extern BOOL		kkybrd_is_disabled();

//! reset system
extern void		kkybrd_reset_system();

//! install keyboard
extern void		kkybrd_init(int);

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [kybrd.h]
//**
//****************************************************************************

#endif
