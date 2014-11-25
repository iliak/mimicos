#ifndef	EXEC_ERRORS_H
#define	EXEC_ERRORS_H




// Values of the error codes 
#define RET_OK			0				// No error 
#define RET_EINVAL		1				// Invalid argument 
#define RET_ENOSUP		2				// Operation not supported 
#define RET_ENOMEM		3				// No available memory 
#define RET_EBUSY		4				// Object or device still in use 
#define RET_EFATAL		255				// Internal fatal error 


// Return type
typedef int ret_t;





#define IOERR_OPENFAIL	 (-1) // device/unit failed to open 
#define IOERR_ABORTED	 (-2) // request terminated early [after AbortIO()] 
#define IOERR_NOCMD		 (-3) // command not supported by device 
#define IOERR_BADLENGTH	 (-4) // not a valid length (usually IO_LENGTH) 
#define IOERR_BADADDRESS (-5) // invalid address (misaligned or bad range) 
#define IOERR_UNITBUSY	 (-6) // device opens ok, but requested unit is busy 
#define IOERR_SELFTEST	 (-7) // hardware failed self-test 

#endif
