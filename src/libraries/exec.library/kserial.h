#ifndef KSERIAL_H
#define KSERIAL_H

#define	BOCHSPORT					0xE9				//

#define SERIAL1_IRQ	4
#define	SERIAL2_IRQ	3


#define SERIAL_IO_PORT1 0x3f8   // COM1
#define SERIAL_IO_PORT2 0x2f8   // COM2
#define SERIAL_IO_PORT3 0x3e8   // COM3
#define SERIAL_IO_PORT4 0x2e8   // COM4




void	kserial_init();
UBYTE	kserial_received();
char	kserial_read();
void	kserial_write(char a);
int		kserial_is_transmit_empty();
void	kserial_print(const char *);



#endif