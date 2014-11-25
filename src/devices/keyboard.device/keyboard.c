//****************************************************************************
//**
//**    kybrd.cpp
//**		-Keyboard driver
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

// http://www.win.tue.nl/~aeb/linux/kbd/scancodes.html
// http://philipstorr.id.au/pcbook/book3/scancode.htm
// http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html <= keyboard scan codes

#include <string.h>
#include <ctype.h>
#include <hal.h>
#include <devices/keyboard.h>
#include <exec/kernel.h>

#include <exec/console.h>


//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

// keyboard encoder ------------------------------------------

enum KYBRD_ENCODER_IO
{
	KYBRD_ENC_INPUT_BUF = 0x60,
	KYBRD_ENC_CMD_REG = 0x60
};

enum KYBRD_ENC_CMDS
{
	KYBRD_ENC_CMD_SET_LED = 0xED,
	KYBRD_ENC_CMD_ECHO = 0xEE,
	KYBRD_ENC_CMD_SCAN_CODE_SET = 0xF0,
	KYBRD_ENC_CMD_ID = 0xF2,
	KYBRD_ENC_CMD_AUTODELAY = 0xF3,
	KYBRD_ENC_CMD_ENABLE = 0xF4,
	KYBRD_ENC_CMD_RESETWAIT = 0xF5,
	KYBRD_ENC_CMD_RESETSCAN = 0xF6,
	KYBRD_ENC_CMD_ALL_AUTO = 0xF7,
	KYBRD_ENC_CMD_ALL_MAKEBREAK = 0xF8,
	KYBRD_ENC_CMD_ALL_MAKEONLY = 0xF9,
	KYBRD_ENC_CMD_ALL_MAKEBREAK_AUTO = 0xFA,
	KYBRD_ENC_CMD_SINGLE_AUTOREPEAT = 0xFB,
	KYBRD_ENC_CMD_SINGLE_MAKEBREAK = 0xFC,
	KYBRD_ENC_CMD_SINGLE_BREAKONLY = 0xFD,
	KYBRD_ENC_CMD_RESEND = 0xFE,
	KYBRD_ENC_CMD_RESET = 0xFF
};

// keyboard controller ---------------------------------------

enum KYBRD_CTRL_IO
{
	KYBRD_CTRL_STATS_REG = 0x64,
	KYBRD_CTRL_CMD_REG = 0x64
};

enum KYBRD_CTRL_STATS_MASK
{

	KYBRD_CTRL_STATS_MASK_OUT_BUF = 1,		//00000001
	KYBRD_CTRL_STATS_MASK_IN_BUF = 2,		//00000010
	KYBRD_CTRL_STATS_MASK_SYSTEM = 4,		//00000100
	KYBRD_CTRL_STATS_MASK_CMD_DATA = 8,		//00001000
	KYBRD_CTRL_STATS_MASK_LOCKED = 0x10,	//00010000
	KYBRD_CTRL_STATS_MASK_AUX_BUF = 0x20,	//00100000
	KYBRD_CTRL_STATS_MASK_TIMEOUT = 0x40,	//01000000
	KYBRD_CTRL_STATS_MASK_PARITY = 0x80	//10000000
};

enum KYBRD_CTRL_CMDS
{

	KYBRD_CTRL_CMD_READ = 0x20,
	KYBRD_CTRL_CMD_WRITE = 0x60,
	KYBRD_CTRL_CMD_SELF_TEST = 0xAA,
	KYBRD_CTRL_CMD_INTERFACE_TEST = 0xAB,
	KYBRD_CTRL_CMD_DISABLE = 0xAD,
	KYBRD_CTRL_CMD_ENABLE = 0xAE,
	KYBRD_CTRL_CMD_READ_IN_PORT = 0xC0,
	KYBRD_CTRL_CMD_READ_OUT_PORT = 0xD0,
	KYBRD_CTRL_CMD_WRITE_OUT_PORT = 0xD1,
	KYBRD_CTRL_CMD_READ_TEST_INPUTS = 0xE0,
	KYBRD_CTRL_CMD_SYSTEM_RESET = 0xFE,
	KYBRD_CTRL_CMD_MOUSE_DISABLE = 0xA7,
	KYBRD_CTRL_CMD_MOUSE_ENABLE = 0xA8,
	KYBRD_CTRL_CMD_MOUSE_PORT_TEST = 0xA9,
	KYBRD_CTRL_CMD_MOUSE_WRITE = 0xD4
};

// scan error codes ------------------------------------------

enum KYBRD_ERROR
{

	KYBRD_ERR_BUF_OVERRUN = 0,
	KYBRD_ERR_ID_RET = 0x83AB,
	KYBRD_ERR_BAT = 0xAA,	//note: can also be L. shift key make code
	KYBRD_ERR_ECHO_RET = 0xEE,
	KYBRD_ERR_ACK = 0xFA,
	KYBRD_ERR_BAT_FAILED = 0xFC,
	KYBRD_ERR_DIAG_FAILED = 0xFD,
	KYBRD_ERR_RESEND_CMD = 0xFE,
	KYBRD_ERR_KEY = 0xFF
};

//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES(AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

// current scan code
static char _scancode;

// lock keys
static BOOL _numlock, _scrolllock, _capslock;

// shift, alt, and ctrl keys current state
static BOOL _shift, _alt, _ctrl;

// set if keyboard error
static int _kkybrd_error = 0;

// set if the Basic Assurance Test(BAT) failed
static BOOL _kkybrd_bat_res = FALSE;

// set if diagnostics failed
static BOOL _kkybrd_diag_res = FALSE;

// set if system should resend last command
static BOOL _kkybrd_resend_res = FALSE;

// set if keyboard is disabled
static BOOL _kkybrd_disable = FALSE;

// original xt scan code set. Array index==make code
// change what keys the scan code corrospond to if your scan code set is different
static int _kkybrd_scancode_usT[] =
{
	// key				scancode
	KEY_UNKNOWN,		//0x00
	KEY_ESCAPE,			//0x01
	KEY_1,				//0x02
	KEY_2,				//0x03
	KEY_3,				//0x04
	KEY_4,				//0x05
	KEY_5,				//0x06
	KEY_6,				//0x07
	KEY_7,				//0x08
	KEY_8,				//0x09
	KEY_9,				//0x0a
	KEY_0,				//0x0b
	KEY_MINUS,			//0x0c
	KEY_EQUAL,			//0x0d
	KEY_BACKSPACE,		//0x0e
	KEY_TAB,			//0x0f
	KEY_Q,				//0x10
	KEY_W,				//0x11
	KEY_E,				//0x12
	KEY_R,				//0x13
	KEY_T,				//0x14
	KEY_Y,				//0x15
	KEY_U,				//0x16
	KEY_I,				//0x17
	KEY_O,				//0x18
	KEY_P,				//0x19
	KEY_LEFTBRACKET,	//0x1a
	KEY_RIGHTBRACKET,	//0x1b
	KEY_RETURN,			//0x1c
	KEY_LCTRL,			//0x1d
	KEY_A,				//0x1e
	KEY_S,				//0x1f
	KEY_D,				//0x20
	KEY_F,				//0x21
	KEY_G,				//0x22
	KEY_H,				//0x23
	KEY_J,				//0x24
	KEY_K,				//0x25
	KEY_L,				//0x26
	KEY_SEMICOLON,		//0x27
	KEY_QUOTE,			//0x28
	KEY_GRAVE,			//0x29
	KEY_LSHIFT,			//0x2a
	KEY_BACKSLASH,		//0x2b
	KEY_Z,				//0x2c
	KEY_X,				//0x2d
	KEY_C,				//0x2e
	KEY_V,				//0x2f
	KEY_B,				//0x30
	KEY_N,				//0x31
	KEY_M,				//0x32
	KEY_COMMA,			//0x33
	KEY_DOT,			//0x34
	KEY_SLASH,			//0x35
	KEY_RSHIFT,			//0x36
	KEY_KP_ASTERISK,	//0x37
	KEY_RALT,			//0x38
	KEY_SPACE,			//0x39
	KEY_CAPSLOCK,		//0x3a
	KEY_F1,				//0x3b
	KEY_F2,				//0x3c
	KEY_F3,				//0x3d
	KEY_F4,				//0x3e
	KEY_F5,				//0x3f
	KEY_F6,				//0x40
	KEY_F7,				//0x41
	KEY_F8,				//0x42
	KEY_F9,				//0x43
	KEY_F10,			//0x44
	KEY_KP_NUMLOCK,		//0x45
	KEY_SCROLLLOCK,		//0x46
	KEY_HOME,			//0x47
	KEY_KP_8,			//0x48	//keypad up arrow
	KEY_PAGEUP,			//0x49
	KEY_KP_2,			//0x50	//keypad down arrow
	KEY_KP_3,			//0x51	//keypad page down
	KEY_KP_0,			//0x52	//keypad insert key
	KEY_KP_DECIMAL,		//0x53	//keypad delete key
	KEY_UNKNOWN,		//0x54
	KEY_UNKNOWN,		//0x55
	KEY_UNKNOWN,		//0x56
	KEY_F11,			//0x57
	KEY_F12				//0x58
};

static int _kkybrd_scancode_fr[][3] =
{
	// normal key			Shift				Alt					scancode
	{KEY_UNKNOWN,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x00
	{KEY_ESCAPE,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x01 Escape
	{KEY_AMPERSAND,			KEY_1,				KEY_UNKNOWN},		//0x02
	{KEY_EACUTE,			KEY_2,				KEY_TILDE},			//0x03
	{KEY_QUOTEDOUBLE,		KEY_3,				KEY_HASH},			//0x04
	{KEY_QUOTE,				KEY_4,				KEY_LEFTCURL},		//0x05
	{KEY_LEFTPARENTHESIS,	KEY_5,				KEY_LEFTBRACKET},	//0x06
	{KEY_MINUS,				KEY_6,				KEY_BAR},			//0x07
	{KEY_EGRAVE,			KEY_7,				KEY_GRAVE},			//0x08
	{KEY_UNDERSCORE,		KEY_8,				KEY_BACKSLASH},		//0x09
	{KEY_CEDILLA,			KEY_9,				KEY_CARRET},		//0x0a
	{KEY_AGRAVE,			KEY_0,				KEY_AT},			//0x0b
	{KEY_RIGHTPARENTHESIS,	KEY_DEGREE,			KEY_RIGHTBRACKET},	//0x0c
	{KEY_EQUAL,				KEY_PLUS,			KEY_RIGHTCURL},		//0x0d
	{KEY_BACKSPACE,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x0e
	{KEY_TAB,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x0f
	{KEY_a,					KEY_A,				KEY_UNKNOWN},		//0x10
	{KEY_z,					KEY_Z,				KEY_UNKNOWN},		//0x11
	{KEY_e,					KEY_E,				KEY_UNKNOWN},		//0x12
	{KEY_r,					KEY_R,				KEY_UNKNOWN},		//0x13
	{KEY_t,					KEY_T,				KEY_UNKNOWN},		//0x14
	{KEY_y,					KEY_Y,				KEY_UNKNOWN},		//0x15
	{KEY_u,					KEY_U,				KEY_UNKNOWN},		//0x16
	{KEY_i,					KEY_I,				KEY_UNKNOWN},		//0x17
	{KEY_o,					KEY_O,				KEY_UNKNOWN},		//0x18
	{KEY_p,					KEY_P,				KEY_UNKNOWN},		//0x19
	{KEY_CARRET,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x1a
	{KEY_DOLLAR,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x1b
	{KEY_RETURN,			KEY_RETURN,			KEY_RETURN},		//0x1c
	{KEY_LCTRL,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x1d
	{KEY_q,					KEY_Q,				KEY_UNKNOWN},		//0x1e
	{KEY_s,					KEY_S,				KEY_UNKNOWN},		//0x1f
	{KEY_d,					KEY_D,				KEY_UNKNOWN},		//0x20
	{KEY_f,					KEY_F,				KEY_UNKNOWN},		//0x21
	{KEY_g,					KEY_G,				KEY_UNKNOWN},		//0x22
	{KEY_h,					KEY_H,				KEY_UNKNOWN},		//0x23
	{KEY_j,					KEY_J,				KEY_UNKNOWN},		//0x24
	{KEY_k,					KEY_K,				KEY_UNKNOWN},		//0x25
	{KEY_l,					KEY_L,				KEY_UNKNOWN},		//0x26
	{KEY_m,					KEY_M,				KEY_UNKNOWN},		//0x27
	{KEY_UGRAVE,			KEY_PERCENT,		KEY_UNKNOWN},		//0x28
	{KEY_SQUARE,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x29
	{KEY_LSHIFT,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x2a
	{KEY_ASTERISK,			KEY_MICRO,			KEY_UNKNOWN},		//0x2b
	{KEY_w,					KEY_W,				KEY_UNKNOWN},		//0x2c
	{KEY_x,					KEY_X,				KEY_UNKNOWN},		//0x2d
	{KEY_c,					KEY_C,				KEY_UNKNOWN},		//0x2e
	{KEY_v,					KEY_V,				KEY_UNKNOWN},		//0x2f
	{KEY_b,					KEY_B,				KEY_UNKNOWN},		//0x30
	{KEY_n,					KEY_N,				KEY_UNKNOWN},		//0x31
	{KEY_COMMA,				KEY_QUESTION,		KEY_UNKNOWN},		//0x32
	{KEY_SEMICOLON,			KEY_DOT,			KEY_UNKNOWN},		//0x33
	{KEY_COLON,				KEY_SLASH,			KEY_UNKNOWN},		//0x34
	{KEY_EXCLAMATION,		KEY_SECTION,		KEY_UNKNOWN},		//0x35
	{KEY_RSHIFT,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x36
	{KEY_KP_ASTERISK,		KEY_UNKNOWN,		KEY_UNKNOWN},		//0x37
	{KEY_RALT,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x38
	{KEY_SPACE,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x39
	{KEY_CAPSLOCK,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x3a
	{KEY_F1,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x3b
	{KEY_F2,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x3c
	{KEY_F3,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x3d
	{KEY_F4,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x3e
	{KEY_F5,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x3f
	{KEY_F6,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x40
	{KEY_F7,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x41
	{KEY_F8,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x42
	{KEY_F9,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x43
	{KEY_F10,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x44
	{KEY_KP_NUMLOCK,		KEY_UNKNOWN,		KEY_UNKNOWN},		//0x45
	{KEY_SCROLLLOCK,		KEY_UNKNOWN,		KEY_UNKNOWN},		//0x46
	{KEY_HOME,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x47
	{KEY_END,				KEY_1,				KEY_UNKNOWN},		//0x48
	{KEY_DOWN,				KEY_2,				KEY_UNKNOWN},		//0x49
	{KEY_PAGEDOWN,			KEY_3,				KEY_UNKNOWN},		//0x4a
	{KEY_LEFT,				KEY_4,				KEY_UNKNOWN},		//0x4b
	{KEY_UNKNOWN,			KEY_5,				KEY_UNKNOWN},		//0x4c
	{KEY_RIGHT,				KEY_6,				KEY_UNKNOWN},		//0x4d
	{KEY_HOME,				KEY_7,				KEY_UNKNOWN},		//0x4e
	{KEY_UP,				KEY_8,				KEY_UNKNOWN},		//0x4f
	{KEY_PAGEUP,			KEY_9,				KEY_UNKNOWN},		//0x50
	{KEY_KP_3,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x51
	{KEY_KP_0,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x52
	{KEY_KP_DECIMAL,		KEY_UNKNOWN,		KEY_UNKNOWN},		//0x53
	{KEY_UNKNOWN,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x54
	{KEY_UNKNOWN,			KEY_UNKNOWN,		KEY_UNKNOWN},		//0x55
	{KEY_LESS,				KEY_GREATER,		KEY_UNKNOWN},		//0x56
	{KEY_F11,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x57
	{KEY_F12,				KEY_UNKNOWN,		KEY_UNKNOWN},		//0x58


};

// invalid scan code. Used to indicate the last scan code is not to be reused
const int INVALID_SCANCODE = 0;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

UBYTE		kybrd_ctrl_read_status();
void		kybrd_ctrl_send_cmd(UBYTE);
UBYTE		kybrd_enc_read_buf();
void		kybrd_enc_send_cmd(UBYTE);
void _cdecl i86_kybrd_irq();

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

// read status from keyboard controller
UBYTE kybrd_ctrl_read_status()
{

	return inportb(KYBRD_CTRL_STATS_REG);
}

// send command byte to keyboard controller
void kybrd_ctrl_send_cmd(UBYTE cmd)
{

	// wait for kkybrd controller input buffer to be clear
	while (TRUE)
		if ((kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;

	outportb(KYBRD_CTRL_CMD_REG, cmd);
}

// read keyboard encoder buffer
UBYTE kybrd_enc_read_buf()
{

	return inportb(KYBRD_ENC_INPUT_BUF);
}

// send command byte to keyboard encoder
void kybrd_enc_send_cmd(UBYTE cmd)
{

	// wait for kkybrd controller input buffer to be clear
	while (TRUE)
		if ((kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
			break;

	// send command byte to kybrd encoder
	outportb(KYBRD_ENC_CMD_REG, cmd);
}

//	keyboard interrupt handler
void _cdecl i86_kybrd_irq()
{
	_asm add esp, 12
	_asm pushad
	_asm cli

	static BOOL _extended = FALSE;

	int code = 0;

	// read scan code only if the kkybrd controller output buffer is full(scan code is in it)
	if (kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF)
	{
		// read the scan code
		code = kybrd_enc_read_buf();

		//DebugGotoXY(0, 0); 	kprintf(" ->0x%x<- ", code);

		// is this an extended code? If so, set it and return
		if (code == 0xE0 || code == 0xE1)
			_extended = TRUE;
		else
		{
			// either the second byte of an extended scan code or a single byte scan code
			_extended = FALSE;

			// test if this is a break code(Original XT Scan Code Set specific)
			if (code & 0x80)	//test bit 7
			{
				// convert the break code into its make code equivelant
				code -= 0x80;

				// grab the key
				int key = _kkybrd_scancode_fr[code][0];

				// test if a special key has been released & set it
				switch (key)
				{
				case KEY_LCTRL:
				case KEY_RCTRL:
					_ctrl = FALSE;
					break;

				case KEY_LSHIFT:
				case KEY_RSHIFT:
					_shift = FALSE;
					break;

				case KEY_LALT:
				case KEY_RALT:
					_alt = FALSE;
					break;
				}
			}
			else
			{
				// this is a make code - set the scan code
				_scancode = code;

				// grab the key
				int key = _kkybrd_scancode_fr[code][0];

				// test if user is holding down any special keys & set it
				switch (key)
				{

				case KEY_LCTRL:
				case KEY_RCTRL:
					_ctrl = TRUE;
					break;

				case KEY_LSHIFT:
				case KEY_RSHIFT:
					_shift = TRUE;
					break;

				case KEY_LALT:
				case KEY_RALT:
					_alt = TRUE;
					break;

				case KEY_CAPSLOCK:
					_capslock = (_capslock) ? FALSE : TRUE;
					kkybrd_set_leds(_numlock, _capslock, _scrolllock);
					break;

				case KEY_KP_NUMLOCK:
					_numlock = (_numlock) ? FALSE : TRUE;
					kkybrd_set_leds(_numlock, _capslock, _scrolllock);
					break;

				case KEY_SCROLLLOCK:
					_scrolllock = (_scrolllock) ? FALSE : TRUE;
					kkybrd_set_leds(_numlock, _capslock, _scrolllock);
					break;
				}
			}
		}

		// watch for errors
		switch (code)
		{

		case KYBRD_ERR_BAT_FAILED:
			_kkybrd_bat_res = FALSE;
			break;

		case KYBRD_ERR_DIAG_FAILED:
			_kkybrd_diag_res = FALSE;
			break;

		case KYBRD_ERR_RESEND_CMD:
			_kkybrd_resend_res = TRUE;
			break;
		}
	}

	// tell hal we are done
	interruptdone(0);

	// return from interrupt handler
	_asm sti
	_asm popad
	_asm iretd
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

// returns scroll lock state
BOOL	kkybrd_get_scroll_lock()
{

	return _scrolllock;
}

// returns num lock state
BOOL	kkybrd_get_numlock()
{

	return _numlock;
}

// returns caps lock state
BOOL	kkybrd_get_capslock()
{

	return _capslock;
}

// returns status of control key
BOOL	kkybrd_get_ctrl()
{

	return _ctrl;
}

// returns status of alt key
BOOL	kkybrd_get_alt()
{

	return _alt;
}

// returns status of shift key
BOOL	kkybrd_get_shift()
{

	return _shift;
}

// tells driver to ignore last resend request
void	kkybrd_ignore_resend()
{

	_kkybrd_resend_res = FALSE;
}

// return if system should redo last commands
BOOL	kkybrd_check_resend()
{

	return _kkybrd_resend_res;
}

// return diagnostics test result
BOOL	kkybrd_get_diagnostic_res()
{

	return _kkybrd_diag_res;
}

// return BAT test result
BOOL	kkybrd_get_bat_res()
{

	return _kkybrd_bat_res;
}

// return last scan code
UBYTE kkybrd_get_last_scan()
{
	return _scancode;
}

// sets leds
void kkybrd_set_leds(BOOL num, BOOL caps, BOOL scroll)
{
	UBYTE data = 0;

	// set or clear the bit
	data = (scroll) ? (data | 1) : (data & 1);
	data = (num) ? (num | 2) : (num & 2);
	data = (caps) ? (num | 4) : (num & 4);

	// send the command -- update keyboard Light Emetting Diods(LEDs)
	kybrd_enc_send_cmd(KYBRD_ENC_CMD_SET_LED);
	kybrd_enc_send_cmd(data);
}

// get last key stroke
enum KEYCODE kkybrd_get_last_key()
{
	UBYTE pos = 0;
	if (_shift)
		pos = 1;
	else if (_alt)
		pos = 2;

	return(_scancode != INVALID_SCANCODE) ? ((enum KEYCODE)_kkybrd_scancode_fr[_scancode][pos]) : (KEY_UNKNOWN);
}

// discards last scan
void kkybrd_discard_last_key()
{
	_scancode = INVALID_SCANCODE;
}


// disables the keyboard
void kkybrd_disable()
{

	kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_DISABLE);
	_kkybrd_disable = TRUE;
}

// enables the keyboard
void kkybrd_enable()
{

	kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_ENABLE);
	_kkybrd_disable = FALSE;
}

// returns TRUE if keyboard is disabled
BOOL kkybrd_is_disabled()
{

	return _kkybrd_disable;
}

// reset the system
void kkybrd_reset_system()
{
	// writes 11111110 to the output port(sets reset system line low)
	kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_WRITE_OUT_PORT);
	kybrd_enc_send_cmd(0xfe);
}

// run self test
BOOL kkybrd_self_test()
{

	// send command
	kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_SELF_TEST);

	// wait for output buffer to be full
	while (1)
		if (kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF)
			break;

	// if output buffer == 0x55, test passed
	return(kybrd_enc_read_buf() == 0x55) ? TRUE : FALSE;
}

//
// prepares driver for use
//
void kkybrd_init(int irq)
{
	kprintf("KEYBRD", "init()\n");

	// Install our interrupt handler(irq 1 uses interrupt 33)
	setvect(irq, i86_kybrd_irq, 0);

	// assume BAT test is good. If there is a problem, the IRQ handler where catch the error
	_kkybrd_bat_res = TRUE;
	_scancode = 0;

	// set lock keys and led lights
	_numlock = _scrolllock = _capslock = FALSE;
	kkybrd_set_leds(FALSE, FALSE, FALSE);

	// shift, ctrl, and alt keys
	_shift = _alt = _ctrl = FALSE;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[kybrd.cpp]
//**
//****************************************************************************
