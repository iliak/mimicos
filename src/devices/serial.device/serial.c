#include <exec\kernel.h>


//
//Serial Device Commands and Functions
//
//	Command 				Command Operation
//	CMD_CLEAR 				Reset the serial port’s read buffer pointers.
//	CMD_FLUSH 				Purge all queued requests for the serial device(does not affect active requests).
//	CMD_READ 				Read a stream of characters from the serial port buffer.The number of characters can be specified or a termination character(s) used.
//	CMD_RESET 				Reset the serial port to its initialized state.All active and queued I / O requests will be aborted and the current buffer will be released.
//	CMD_START 				Restart all paused I / O over the serial port.Also sends an “xON”.
//	CMD_STOP 				Pause all active I / O over the serial port.Also sends an “xOFF”.
//	CMD_WRITE 				Write out a stream of characters to the serial port.The number of characters can be specified or a NULL - terminated string can be sent.
//	SDCMD_BREAK 			Send a break signal out the serial port.May be done immediately or queued.Duration of the break (in microseconds) can be set by the application.
//	SDCMD_QUERY 			Return the status of the serial port lines and registers, and the number of bytes in the serial port’s read buffer.
//	SDCMD_SETPARAMS 		Set the parameters of the serial port.This ranges from baud rate to number of microseconds a break will last.



VOID serial_init(VOID)
{
	kprintf("SERIAL", "serial_init()\n");

}