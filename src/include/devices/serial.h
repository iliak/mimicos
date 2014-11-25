#ifndef SERIAL_H
#define	SERIAL_H

#include <exec/io.h>


/* status of serial port, as follows:
*		   BIT	ACTIVE	FUNCTION
*		    0	 ---	reserved
*		    1	 ---	reserved
*		    2	 high	Connected to parallel "select" on the A1000.
*				Connected to both the parallel "select" and
*				serial "ring indicator" pins on the A500
*				& A2000.  Take care when making cables.
*		    3	 low	Data Set Ready
*		    4	 low	Clear To Send
*		    5	 low	Carrier Detect
*		    6	 low	Ready To Send
*		    7	 low	Data Terminal Ready
*		    8	 high	read overrun
*		    9	 high	break sent
*		   10	 high	break received
*		   11	 high	transmit x-OFFed
*		   12	 high	receive x-OFFed
*		13-15		reserved
*/

#define   SDCMD_QUERY			CMD_NONSTD	/* $09 */
#define   SDCMD_BREAK	       (CMD_NONSTD+1)	/* $0A */
#define   SDCMD_SETPARAMS      (CMD_NONSTD+2)	/* $0B */


#endif // !SERIAL_H
