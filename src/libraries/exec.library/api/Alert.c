
#include <stdio.h>
#include <proto/exec.h>
#include <exec/kernel.h>
#include <exec/alerts.h>
#include <exec/console.h>


VOID Alert(ULONG alert, STRPTR fmt, ...)
{
	char buffer[1024] = {0};

	va_list	args;
	va_start(args, fmt);
	vsprintf(&buffer, fmt, args);
	va_end(args);

	kprintf("ALERT", "GURU MEDITATION %#010X : \"%s\"\n", alert, &buffer);



	console_clear(0x07);
	console_setcolor(CONSOLE_COLOR_BLACK | CONSOLE_COLOR_BLINK, CONSOLE_COLOR_RED);
	console_printf("################################################################################");
	console_printf("#                                                                              #");
	console_printf("#                      GURU MEDITATION %#010X                              #", alert);
	console_printf("#                                                                              #");
	console_printf("################################################################################\n");


	console_setcolor(CONSOLE_COLOR_BLACK, CONSOLE_COLOR_WHITE);
	console_printf("Message: %s\n", &buffer);

	if (alert & AT_DeadEnd)		console_printf("AT_DeadEnd	\n");
	if (alert & AT_Recovery)	console_printf("AT_Recovery\n");

	if (alert & AG_MakeLib)		console_printf("AG_MakeLib	\n");
	if (alert & AG_OpenLib)		console_printf("AG_OpenLib	\n");
	if (alert & AG_OpenDev)		console_printf("AG_OpenDev	\n");
	if (alert & AG_OpenRes)		console_printf("AG_OpenRes	\n");
	if (alert & AG_IOError)		console_printf("AG_IOError	\n");
	if (alert & AG_NoSignal)	console_printf("AG_NoSignal	\n");
	if (alert & AG_BadParm)		console_printf("AG_BadParm	\n");
	if (alert & AG_CloseLib)	console_printf("AG_CloseLib	\n");
	if (alert & AG_CloseDev)	console_printf("AG_CloseDev	\n");
	if (alert & AG_ProcCreate)	console_printf("AG_ProcCreate\n");

	Disable();
	Halt();
}