/******************************************************************************
   main.cpp

   Memory map:

   - Stage 1:
   0x7c00 : Bootloader start
   -Stage 2:
   0x500 :	Base
   0x1000:	Memory map

   - EXEC: (see common.inc)
   Physical :	0x0000 3000
   Virtual:		0xC000 0000 (3Go)
   ******************************************************************************/
#include <exec/bootinfo.h>
#include <hal.h>
//#include <string.h>
//#include <stdio.h>
#include <stdlib.h>

#include <proto\exec.h>
#include <proto\intuition.h>
#include <proto\expansion.h>
#include <proto\graphics.h>

#include <exec/exec.h>
#include <exec/kernel.h>
#include <exec/console.h>
#include <exec/devices.h>
#include <exec/cpu.h>
#include <devices/keyboard.h>
#include <devices/trackdisk.h>
//#include <devices/serial.h>

#include <intuition\intuition.h>
#include <libraries\expansion.h>
#include <graphics\gfxbase.h>

#include "../hal/tss.h"
#include "../hal/idt.h"
#include "exception.h"
#include "pmm.h"
#include "paging.h"
#include "kserial.h"
//#include "smp.h"
//#include "memory/kheap.h"


// http://0vercl0k.tuxfamily.org/bl0g/?tag=os-dev
// http://a.michelizza.free.fr/pmwiki.php?n=TutoOS.TutoOS
// http://www-igm.univ-mlv.fr/~dr/XPOSE2004/bfournier/
// http://sos.enix.org/fr/PagePrincipale
// http://www.nondot.org/sabre/os/articles
// http://sos.enix.org/fr/SOSDownload#TOC_0_2_25
// http://www.internals.com/articles/protmode/protmode.htm
// 



#define	STACK_SIZE                    0x00004000    



struct Device SerialDevice;


//
// Execbase
//
struct ExecBase	SysBase;




//
// wait for key stroke
//
enum KEYCODE	getch(VOID)
{
	enum KEYCODE key = KEY_UNKNOWN;

	// wait for a keypress
	while (key == KEY_UNKNOWN)
		key = kkybrd_get_last_key();

	// discard last keypress(we handled it) and return
	kkybrd_discard_last_key();
	return key;
}


//
// sleeps a little bit. This uses the HALs get_tick_count() which in turn uses the PIT
//
VOID sleep(ULONG ms)
{
	static ULONG ticks = 0;
	ticks = ms + get_tick_count();
	while (ticks > get_tick_count())
	{

	}
}


//
// gets next command
//
VOID get_cmd(char* buf, int n)
{
	console_printf("\nCommand> ");

	enum KEYCODE key = KEY_UNKNOWN;
	BOOL	BufChar;

	// get command string
	int i = 0;
	while (i < n)
	{

		// buffer the next char
		BufChar = TRUE;

		// grab next char
		key = getch();

		// end of command if enter is pressed
		if (key == KEY_RETURN)
			break;

		// backspace
		if (key == KEY_BACKSPACE)
		{

			// dont buffer this char
			BufChar = FALSE;

			if (i > 0)
			{
				// go back one char
				ULONG y, x;
				console_getcursor(&x, &y);
				if (x > 0)
					console_goto(--x, y);
				else
				{
					// x is already 0, so go back one line
					y--;
					x = CONSOLE_WIDTH;
				}

				// erase the character from display
				console_printf(" ");
				console_goto(x, y);

				// go back one char in cmd buf
				i--;
			}
		}

		// only add the char if it is to be buffered
		if (BufChar && key != 0)
		{
			console_printf("%c", key);
			buf[i++] = key;
		}

		// wait for next key. You may need to adjust this to suite your needs
		sleep(20);
	}

	// null terminate the string
	buf[i] = '\0';
}


//
// read sector command
//
VOID cmd_read_sect(void)
{
	ULONG sectornum = 0;
	char sectornumbuf[4];
	UBYTE* sector = 0;

	console_printf("\n\rPlease type in the sector number [0 is default] >");
	//get_cmd(sectornumbuf, 3);
	//sectornum = atoi(sectornumbuf);
	sectornum = 0;

	console_printf("\n\rSector %i contents:\n\n\r", sectornum);

	// read sector from disk
	sector = flpydsk_read_sector(sectornum);

	// display sector
	if (sector != 0)
	{
		int i = 0;
		for (int c = 0; c < 4; c++)
		{
			for (int j = 0; j < 128; j++)
			{
				console_printf("%x ", sector[i + j]);
			}
			i += 128;

			console_printf("\n\rPress any key to continue\n\r");
			getch();
		}
	}
	else
		console_printf("\n\r*** Error reading sector from disk");

	console_printf("\n\rDone.");
}


//
// CPUID test
//
VOID cpuid(VOID)
{
	char	vendor[32] = {0};
	ULONG	b = 0, c = 0, d = 0;
	ULONG	maxid = 0;


#ifdef _MSC_VER
	_asm
	{
		mov		eax, 0		; Vendor ID string
		cpuid
		mov		dword ptr[vendor], ebx
		mov		dword ptr[vendor + 4], edx
		mov		dword ptr[vendor + 8], ecx
		mov		dword ptr[maxid], eax;
	}
#endif

	console_printf("\nmaxid: %i - vendor: \"%s\"\n", maxid, &vendor);

	c = 0;
	d = 0;
#ifdef _MSC_VER
	_asm
	{
		mov		eax, 1			; Feature information
		cpuid
		mov		c, ecx
		mov		d, edx
	}
#endif
	console_printf("cpuid: 1 - ecx: 0x%08X => %032b\n", c, c);
	console_printf("cpuid: 1 - edx: 0x%08X => %032b\n", d, d);

}



//
// our simple command parser
//
BOOL run_cmd(STRPTR cmd_buf)
{

	if (strcmp(cmd_buf, "a") == 0)
	{
		for (int i = 1; i < 255; i++)
			console_printf("%03i % 1c - ", i, i);

		console_printf("\n");
	}

	// exit command
	else if (strcmp(cmd_buf, "exit") == 0)
	{
		return TRUE;
	}

	// Dump heap
	else if (strcmp(cmd_buf, "kheap") == 0)
	{
		kheap_dump();
	}

	// cpuid
	else if (strcmp(cmd_buf, "cpuid") == 0)
	{
		cpuid();
	}

	// Print exec version
	else if (strcmp(cmd_buf, "version") == 0)
	{
		console_printf("\nVersion %i\n", SysBase.SoftVer);
	}

	// Print exec version
	else if (strcmp(cmd_buf, "fault") == 0)
	{

		// Test pour la mémoire paginée
		UBYTE *mem = (UBYTE *)0x00580000;
		LONG h = mem[0];
		mem[0] = 0x12345678;
		console_printf("mem: %i\n", mem[0]);

	}
	else if (strcmp(cmd_buf, "user") == 0)
	{
		kbreak();

		int stack = 0;
		_asm mov[stack], esp


		extern void tss_set_stack(UWORD, UWORD);
		tss_set_stack(0x10, stack);

		enter_usermode();

		char testStr[] = "\n\rWe are inside of your computer...";

		//! call OS-print message
		_asm
		{
			xor eax, eax
			lea ebx, [testStr]
			int 0x80
		}

		//! cant do CLI+HLT here, so loop instead
		ULONG a = 0;
		while (1)
		{
			a++;
		}
	}


	// Generate a division by zero
	else if (strcmp(cmd_buf, "divide") == 0)
	{
		console_printf("\nDivision by zero...\n");
		float i = 0.0;
		i = 1;
		console_printf("\ni = %i", i);
		i--;
		float u = 10.0 / i;
		console_printf(" - u = %i\n", u);

		Cause(9);
	}

	// reset command
	else if (strcmp(cmd_buf, "reboot") == 0)
	{
		console_printf("\nReboot NOW !");
		kkybrd_reset_system();
		console_printf(" Or not...\n");
	}

	// clear screen
	else if (strcmp(cmd_buf, "cls") == 0)
	{
		console_clear(0x17);
	}

	// help
	else if (strcmp(cmd_buf, "help") == 0)
	{
		console_printf("\nSupported commands:\n");
		console_printf(" - exit: quits and halts the system\n");
		console_printf(" - cls: clears the display\n");
		console_printf(" - help: displays this message\n");
		console_printf(" - version: displays OS version\n");
	}

	// read sector
	else if (strcmp(cmd_buf, "read") == 0)
	{
		cmd_read_sect();
	}

	else if (strcmp(cmd_buf, "video") == 0)
	{
		kprintf("KERNEL", "Switching to mode 13h !!!\n");
		_asm
		{
			mov AH, 0h
			mov AL, 13h
			int 10h
		}


	}
	// invalid command
	else
	{
		console_printf("\nUnkown command");
	}

	return FALSE;
}


//
// Main loop
//
void kernel_loop(void)
{
	kprintf("EXEC", "loop()\n");

	// command buffer
	char	cmd_buf[100];

	for (;;)
	{

		// get command
		get_cmd(cmd_buf, 98);

		// run command
		if (run_cmd(cmd_buf))
			break;
	}
}



void* _syscalls[] =
{
	console_printf
};



//
//
//
_declspec(naked) void syscall_dispatcher(void)
{

	static int index = 0;
	_asm mov[index], eax

	//! bounds check
	if (index >= 1)
		_asm iretd

	//! get service
	static void* fnct = NULL;
	fnct = _syscalls[index];

	//! and call service
	_asm
	{
		push edi
			push esi
			push edx
			push ecx
			push ebx
			call fnct
			add esp, 20
			iretd
	}
}







//
// Initialize memory
//
VOID memory_init(struct multiboot_info *mbi)
{
	SysBase.MemorySize = 0;
	SysBase.MemoryMaxPages = 0;
	SysBase.MemoryUsedPages = 0;
	kprintf("EXEC", "KernelVBase : 0x%08X - KernelPBase : 0x%08X - kernelsize: %i Ko\n", SysBase.KernelVBase, SysBase.KernelPBase, SysBase.KernelSize / 1024);

	// Info are present ?
	if ((mbi->flags & MULTIBOOT_INFO_MEM_MAP) != MULTIBOOT_INFO_MEM_MAP)
	{
		Alert(AT_DeadEnd | AG_NoMemory, "No memory information found while booting !!");
	}
	

	//kprintf("EXEC", "Dumping memory map :\n");
	//struct multiboot_mmap_entry *entry = mbi->mmap_addr;
	//UBYTE i = 0;
	//while ((APTR)entry < (mbi->mmap_addr + mbi->mmap_length))
	//{
	//	kprintf("", "[mmap %d] [0x%08X - 0x%08X] length: % 9i Ko, type: %s\n",
	//		i,
	//		(ULONG)(entry->addr & 0xffffffff),
	//		(ULONG)(entry->addr & 0xffffffff) + (ULONG)(entry->len & 0xffffffff),
	//		(ULONG)(entry->len & 0xffffffff) / 1024,
	//		entry->type == 0x1 ? "available" : "reserved");

	//	entry = (struct multiboot_mmap_entry *) ((ULONG)entry + entry->size + sizeof(entry->size));
	//	i++;
	//}

	



	// Physical memory manager
	pmm_init(mbi, KERNEL_PBASE + SysBase.KernelSize);
	kprintf("EXEC", "Total memory size: %i Ko\n", SysBase.MemorySize / 1024);
	kprintf("EXEC", "MemoryMap @ %#010P with a size of %i octets\n", SysBase.MemoryMap, PMM_MEMORY_MAP_SIZE);
	kprintf("EXEC", "MemoryMaxPages:  %10i\n", SysBase.MemoryMaxPages);
	kprintf("EXEC", "MemoryUsedPages: %10u\n", SysBase.MemoryUsedPages);


	// Virtual memory manager
	paging_init();


	//kheap_dump();
	APTR mem0 = kmalloc(16);			//kprintf("", " 0 %#010P\n", mem0);
	APTR mem1 = kmalloc(16);			//kprintf("", " 1 %#010P\n", mem1);
	APTR mem2 = kmalloc(16);			//kprintf("", " 2 %#010P\n", mem2);
	APTR mem3 = kmalloc(16);			//kprintf("", " 3 %#010P\n", mem3);
	//APTR mem4 = kmalloc(16 * 4096);		//kprintf("", " 4 %#010P\n", mem4);
	//APTR mem5 = kmalloc(16);	kprintf("", " 5 %#010P\n", mem5);
	//APTR mem6 = kmalloc(16);	kprintf("", " 6 %#010P\n", mem6);
	//APTR mem7 = kmalloc(16);	kprintf("", " 7 %#010P\n", mem7);
	//APTR mem8 = kmalloc(16);	kprintf("", " 8 %#010P\n", mem8);
	//APTR mem9 = kmalloc(16);	kprintf("", " 9 %#010P\n", mem9);

	//APTR mem10 = kmalloc(16);	kprintf("", "10 %#010P\n", mem10);


	kheap_dump();
}


//
// Initialize the kernel
//
BOOL kernel_init(struct multiboot_info* bootinfo)
{
	kprintf("EXEC", "init()\n");

	SysBase.KernelVBase = KERNEL_VBASE;
	SysBase.KernelPBase = KERNEL_PBASE;
	SysBase.SoftVer = EXEC_VERSION;
	SysBase.SoftRev = EXEC_REVISION;
	NewList(&SysBase.MemList);
	NewList(&SysBase.ResourceList);
	NewList(&SysBase.DeviceList);
	NewList(&SysBase.IntrList);
	NewList(&SysBase.LibList);;
	NewList(&SysBase.PortList);
	NewList(&SysBase.TaskReady);
	NewList(&SysBase.TaskWait);
	SysBase.MemList.Type = NT_MEMORY;
	SysBase.ResourceList.Type = NT_RESOURCE;
	SysBase.DeviceList.Type = NT_DEVICE;
	SysBase.IntrList.Type = NT_INTERRUPT;
	SysBase.LibList.Type = NT_LIBRARY;
	SysBase.PortList.Type = NT_MSGPORT;
	SysBase.TaskReady.Type = NT_TASK;
	SysBase.TaskWait.Type = NT_TASK;


	hal_initialize();

	// clear and init display
	console_clear(0x13);
	console_goto(0, 0);
	console_setcolor(0x1, 0x7);

	console_printf("MimicOS Kernel is starting up...\n");

	memory_init(bootinfo);

	kprintf("EXEC", "Interrupt handlers()...\n");
	SetIntVector(0, (InterruptHandler)divide_by_zero_fault);
	SetIntVector(1, (InterruptHandler)single_step_trap);
	SetIntVector(2, (InterruptHandler)nmi_trap);
	SetIntVector(3, (InterruptHandler)breakpoint_trap);
	SetIntVector(4, (InterruptHandler)overflow_trap);
	SetIntVector(5, (InterruptHandler)bounds_check_fault);
	SetIntVector(6, (InterruptHandler)invalid_opcode_fault);
	SetIntVector(7, (InterruptHandler)no_device_fault);
	SetIntVector(8, (InterruptHandler)double_fault_abort);
	SetIntVector(10, (InterruptHandler)invalid_tss_fault);
	SetIntVector(11, (InterruptHandler)no_segment_fault);
	SetIntVector(12, (InterruptHandler)stack_fault);
	SetIntVector(13, (InterruptHandler)general_protection_fault);
	SetIntVector(14, (InterruptHandler)page_fault);
	SetIntVector(16, (InterruptHandler)fpu_fault);
	SetIntVector(17, (InterruptHandler)alignment_check_fault);
	SetIntVector(18, (InterruptHandler)machine_check_abort);
	SetIntVector(19, (InterruptHandler)simd_fpu_fault);


	kprintf("EXEC", "bootinfo:                     %#010P\n",	bootinfo);
	kprintf("EXEC", "bootinfo->flags:              0x%.8X\n",	bootinfo->flags);
	kprintf("EXEC", "bootinfo->mem_upper:          %10i\n",		bootinfo->mem_upper);
	kprintf("EXEC", "bootinfo->mem_lower:          %10i\n",		bootinfo->mem_lower);
	kprintf("EXEC", "bootinfo->bootDevice:         %#010P\n",	bootinfo->boot_device);
	kprintf("EXEC", "bootinfo->cmdLine:            \"%s\"\n",	bootinfo->cmdline);
	kprintf("EXEC", "bootinfo->modsCount:          %10i\n",		bootinfo->mods_count);
	kprintf("EXEC", "bootinfo->modsAddr:           %#010P\n",	bootinfo->mods_addr);
	kprintf("EXEC", "bootinfo->u.aout_sym.addr     %#010P\n",	bootinfo->u.aout_sym.addr);
	kprintf("EXEC", "bootinfo->u.aout_sym.reserved %10i\n",		bootinfo->u.aout_sym.reserved);
	kprintf("EXEC", "bootinfo->u.aout_sym.strsize  %10i\n",		bootinfo->u.aout_sym.strsize);
	kprintf("EXEC", "bootinfo->u.aout_sym.tabsize  %10i\n",		bootinfo->u.aout_sym.tabsize);
	kprintf("EXEC", "bootinfo->mmap_length:        %10i\n",		bootinfo->mmap_length);
	kprintf("EXEC", "bootinfo->mmap_addr:          %#010P\n",	bootinfo->mmap_addr);
	kprintf("EXEC", "bootinfo->drives_length:      %10i\n",		bootinfo->drives_length);
	kprintf("EXEC", "bootinfo->drives_addr:        %#010P\n",	bootinfo->drives_addr);
	kprintf("EXEC", "bootinfo->config_table:       %10i\n",		bootinfo->config_table);
	kprintf("EXEC", "bootinfo->bootloader_name:    \"%s\"\n",	bootinfo->boot_loader_name);
	kprintf("EXEC", "bootinfo->apm_table:          %#010X\n",	bootinfo->apm_table);
	kprintf("EXEC", "bootinfo->vbe_control_info:   %#010P\n",	bootinfo->vbe_control_info);
	kprintf("EXEC", "bootinfo->vbe_mode_info:      %#010P\n",	bootinfo->vbe_mode_info);
	kprintf("EXEC", "bootinfo->vbe_mode:           %#010P\n",	bootinfo->vbe_mode);
	kprintf("EXEC", "bootinfo->vbe_interface_seg:  %#010P\n",	bootinfo->vbe_interface_seg);
	kprintf("EXEC", "bootinfo->vbe_interface_off:  %#010P\n",	bootinfo->vbe_interface_off);
	kprintf("EXEC", "bootinfo->vbe_interface_len:  %10i\n",		bootinfo->vbe_interface_len);

	// Interrupt handler
	Enable();


	if (OpenLibrary(EXPANSIONNAME, 0) == NULL)
		kprintf("EXE", "Faild to load %s !\n", EXPANSIONNAME);
	expansion_init();


	// keyboard.device
	// install the keyboard to IR 33
	kkybrd_init(33);



	//
	// trackdisk.device
	//
	// set drive 0 as current drive
	flpydsk_set_working_drive(0);

	// install floppy disk to IR 38, uses IRQ 6
	flpydsk_init(38);


	// serial test
	memset(&SerialDevice, 0, sizeof(struct Device));
	SerialDevice.Library.IdString = "serial.device";
	SerialDevice.Library.Version = 1;
	SerialDevice.Library.Revision = 0;
	AddDevice(&SerialDevice);


	// Try to read the hardware clock...
	UBYTE hour, min, sec;
	outportb(0x70, 0x4);
	hour = inportb(0x71);
	outportb(0x70, 0x2);
	min = inportb(0x71);
	outportb(0x70, 0x0);
	sec = inportb(0x71);
	kprintf("EXEC", "hh:mm:ss => %i:%i:%i\n", hour, min, sec);


	/*
		MsgPort * msg = CreateMsgPort();
		IORequest * io = CreateIORequest(msg, sizeof(MsgPort));
		OpenDevice("serial.device", 0, io, 0);
		io->io_Command = SDCMD_QUERY;
		DoIO(io);
		CloseDevice(io);
		DeleteIORequest(io);
		DeleteMsgPort(msg);
		*/


	/*
		IORequest io_serial;
		io_serial.io_Command = CMD_WRITE;

		BYTE err = OpenDevice("serial.device", 0, &io_serial, 0);
		DoIO(&io_serial);
		CloseDevice(&io_serial);
		*/


	// Symmetric multiprocessing init
	//smp_init();

	/*
		// Test some physical memory allocation / deallocation
		APTR p = (APTR)pmm_alloc_page();
		console_printf("p allocated at %X\n", p);
		APTR p2 = (APTR)pmm_alloc_pages(200);
		console_printf("allocated 200 blocks for p2 at %X\n", p2);
		pmm_free_page(p);
		p = (APTR)pmm_alloc_page();
		console_printf("Unallocated p to free block 1. p is reallocated to %x\n", p);
		APTR p3 = (APTR)pmm_alloc_pages(30);
		console_printf("allocated 30 blocks for p3 at %X\n", p3);
		pmm_free_pages(p3, 30);
		pmm_free_page(p);
		pmm_free_pages(p2, 200);

		p3 = (APTR)pmm_alloc_pages(30);
		console_printf("allocated 30 blocks for p3 at %X\n", p3);
		pmm_free_pages(p3, 30);
		*/



	// Syscall init
	//	setvect(0x80, syscall_dispatcher, I86_IDT_DESC_RING3);
	//	install_tss(5, 0x10, 0);

	if (OpenLibrary(GRAPHICSNAME, 0) == NULL)
		kprintf("EXEC", "Failed to load %s !\n", GRAPHICSNAME);
	graphics_init();


	if (OpenLibrary(INTUITIONNAME, 0) == NULL)
		kprintf("EXEC", "Failed to load %s !\n", INTUITIONNAME);
	//intuition_init();


	return TRUE;
}


struct x86_reg regs;

//
// kernel entry point is called by boot loader
//
void kernel_entry()
{
	ULONG magic = 0;
	struct multiboot_info *mbi = NULL;
	__asm
	{
		mov		magic, eax
		mov		mbi, ebx
	}


	// Retrieve the kernel size passed by the bootloader
	//ULONG ksize = 0;
	//_asm
	//{
	//	mov	dword ptr[ksize], edx
	//}
	SysBase.KernelSize = 150 * 512;		// HACK: As I do not find how to do, I put the hard value (150)
	kprintf(NULL, "\n\n\n\n\n\n\n\n\n\n");
	kserial_init();


#ifdef ARCH_X86
	// Set registers for protected mode
	_asm
	{
		mov ax, 10h
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
	}
#endif



	kernel_init(mbi);

	kernel_loop();

	console_printf("\nExit command recieved; kernel halted\n");
	kprintf("EXEC", "\nExit command recieved; kernel halted...\n");

	exit(0);

	kprintf("EXEC", "\nshutdown complete. Halting system\n");

	Disable();
	Halt();
}


//
// Multiboot header
//
//! loading address
#define MULTIBOOT_HEADER_FLAGS			0x00010007 // MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE | MULTIBOOT_AOUT_KLUDGE
#define	MULTIBOOT_HEADER_CHECKSUM		-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)


#define	LOADBASE						KERNEL_PBASE
#define	ALIGN							0x00000400
#define	HEADER_ADDRESS					LOADBASE + ALIGN


#pragma code_seg(".a$0")
__declspec(allocate(".a$0"))
struct multiboot_header _MultibootInfo =
{
	MULTIBOOT_HEADER_MAGIC,
	MULTIBOOT_HEADER_FLAGS,
	MULTIBOOT_HEADER_CHECKSUM,
	HEADER_ADDRESS,				// 
	LOADBASE,					// Where to load the kernel in physical memory
	0,							// load end address
	0,							// bss end address
	kernel_entry,				// Entry point
	0, 1024, 768, 32			// Video mode
};
#pragma comment(linker, "/merge:.text=.a")
