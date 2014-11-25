#include <stdlib.h>



char tbuf[32];
char bchars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };


//
//
//
void itoa(ULONG i, ULONG base, char* buf)
{
	LONG pos = 0;
	LONG opos = 0;
	LONG top = 0;

	// Invalid base format
	if (i == 0 || base > 16)
	{
		buf[0] = '0';
		buf[1] = 0;
		return;
	}

	while (i != 0)
	{
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top = pos--;

	for (opos = 0; opos < top; pos--, opos++)
	{
		buf[opos] = tbuf[pos];
	}

	// NULL the end
	buf[opos] = 0;
}



