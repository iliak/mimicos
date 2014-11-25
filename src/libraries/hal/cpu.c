//****************************************************************************
//**
//**    cpu.cpp
//**
//**	This is the processor interface. Everything outside of this module
//**	must use this interface when working on processor data.
//**
//**	A processor is a module that manages the very basic data structures
//**	and data within the system. The processor interface provides the interface
//**	for managing processors, processor cores, accessing processor structures,
//**	and more
//**
//****************************************************************************

#include <exec/cpu.h>
#include <exec/macros.h>

#include "cpu.h"
#include "gdt.h"
#include "idt.h"


VOID dump_regs(struct x86_reg *regs)
{
	if (regs == NULL)
		return;

	kprintf("", "CONTROL REGISTERS\n");
	kprintf("", "	CR0: 0x%08X		%s%s%s%s%s%s%s%s%s%s%s\n", regs->cr0,
		CHECK_BIT(regs->cr0, CR0_PROTECTED_MODE_BIT) ? "[PE]" : "",
		CHECK_BIT(regs->cr0, CR0_MONITOR_COPROCESSOR_BIT) ? "[MP]" : "",
		CHECK_BIT(regs->cr0, CR0_EMULATION_BIT) ? "[EM]" : "",
		CHECK_BIT(regs->cr0, CR0_TASK_SWITCHED_BIT) ? "[TS]" : "",
		CHECK_BIT(regs->cr0, CR0_EXTENSION_TYPE_BIT) ? "[ET]" : "",
		CHECK_BIT(regs->cr0, CR0_NUMERIC_ERROR_BIT) ? "[NE]" : "",
		CHECK_BIT(regs->cr0, CR0_WRITE_PROTECT_BIT) ? "[WP]" : "",
		CHECK_BIT(regs->cr0, CR0_ALIGNMENT_MASK_BIT) ? "[AM]" : "",
		CHECK_BIT(regs->cr0, CR0_NOT_WRITE_THROUGH_BIT) ? "[NW]" : "",
		CHECK_BIT(regs->cr0, CR0_CACHE_DISABLE_BIT) ? "[CD]" : "",
		CHECK_BIT(regs->cr0, CR0_PAGING_BIT) ? "[PG]" : "");
	kprintf("", "	CR2: 0x%08X		%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n", regs->cr2,
		CHECK_BIT(regs->cr2, CR3_VIRTUAL_MODE_EXT_BIT) ? "[VME]" : "",
		CHECK_BIT(regs->cr2, CR3_PMODE_VINTERRUPTS_BIT) ? "[PVI]" : "",
		CHECK_BIT(regs->cr2, CR3_TIME_STAMP_DISABLE_BIT) ? "[TSD]" : "",
		CHECK_BIT(regs->cr2, CR3_DEBUGGING_EXT_BIT) ? "[DE]" : "",
		CHECK_BIT(regs->cr2, CR3_PAGE_SIZE_EXT_BIT) ? "[PSE]" : "",
		CHECK_BIT(regs->cr2, CR3_PHYSICAL_ADDRESS_EXT_BIT) ? "[PAE]" : "",
		CHECK_BIT(regs->cr2, CR3_MACHINE_CHECK_EXT_BIT) ? "[MCE]" : "",
		CHECK_BIT(regs->cr2, CR3_PAGE_GLOBAL_ENABLE_BIT) ? "[PGE]" : "",
		CHECK_BIT(regs->cr2, CR3_PERFORMANCE_MONITORING_COUNTER_ENABLE_BIT) ? "[PCE]" : "",
		CHECK_BIT(regs->cr2, CR3_OS_FX_SUPPORT_BIT) ? "[OSFXSR]" : "",
		CHECK_BIT(regs->cr2, CR3_OS_UNMASKED_SIMD_BIT) ? "[OSXMMEXCPT]" : "",
		CHECK_BIT(regs->cr2, CR3_VM_EXT_ENABLE_BIT) ? "[VMXE]" : "",
		CHECK_BIT(regs->cr2, CR3_SAFER_MODE_EXT_ENABLE_BIT) ? "[SMXE]" : "",
		CHECK_BIT(regs->cr2, CR3_PCID_ENABLE_BIT) ? "[PCIDE]" : "",
		CHECK_BIT(regs->cr2, CR3_OS_PROC_EXT_ENABLE_BIT) ? "[OSXSAVE]" : "",
		CHECK_BIT(regs->cr2, CR3_SUPERVISOR_MODE_EXECUTION_BIT) ? "[SMEP]" : "",
		CHECK_BIT(regs->cr2, CR3_SUPERVISOR_MODE_ACCESS_BIT) ? "[SMAP]" : "");
	kprintf("", "	CR3: 0x%08X	\n", regs->cr3);
	//kprintf("", "	CR4: 0x%08X	\n", regs->cr4);

	kprintf("", "GENERAL REGISTERS\n");
	kprintf("", "	EAX: 0x%08X		EBX: 0x%08X		ECX: 0x%08X		EDX: 0x%08X\n", regs->eax, regs->ebx, regs->ecx, regs->edx);
	kprintf("", "	ESI: 0x%08X		EDI: 0x%08X		EBP: 0x%08X		ESP: 0x%08X\n", regs->esi, regs->edi, regs->ebp, regs->esp);
	kprintf("", "	EIP: 0x%08X\n", regs->eip);

	kprintf("", "SEGMENT REGISTERS\n");
	kprintf("", "	CS: 0x%08X		DS: 0x%08X		ES: 0x%08X\n", regs->cs, regs->ds, regs->es);
	kprintf("", "	GS: 0x%08X		SS: 0x%08X		FS: 0x%08X\n", regs->gs, regs->ss, regs->fs);

	kprintf("", "STATUT REGISTERS\n");
	kprintf("", "	EFLAGS: 0x%08X\n", regs->eflags);

}

// Initializes cpu resources
int i86_cpu_initialize()
{
	kprintf("CPU", "init()\n");

	// initialize processor tables
	i86_gdt_init();
	i86_idt_init(0x8);		// 0x8 : offset within the GDT

	return 0;
}

// shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown()
{


}

// returns vender name of cpu
char* i86_cpu_get_vender()
{
	static char	vender[32] = {0};

#ifdef _MSC_VER
	_asm
	{
		mov		eax, 0
		cpuid
		mov		dword ptr [vender], ebx
		mov		dword ptr [vender+4], edx
		mov		dword ptr [vender+8], ecx
	}
#endif

	return vender;
}

// flush all internal and external processor caches
void i86_cpu_flush_caches()
{

#ifdef _MSC_VER
	_asm
	{
		cli
		invd
		sti
	}
#endif
}

// same as above but writes the data back into memory first
void i86_cpu_flush_caches_write()
{

#ifdef _MSC_VER
	_asm 
	{
		cli
		wbinvd
		sti
	}
#endif
}

// flushes TLB entry
void i86_cpu_flush_tlb_entry(ULONG addr)
{

#ifdef _MSC_VER
	_asm
	{
		cli
		invlpg	addr
		sti
	}
#endif
}

