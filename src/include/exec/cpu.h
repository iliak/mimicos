#ifndef CPU_H
#define CPU_H

#include <exec\types.h>

/* Vendor-strings. */
#define CPUID_VENDOR_OLDAMD       "AMDisbetter!" //early engineering samples of AMD K5 processor
#define CPUID_VENDOR_AMD          "AuthenticAMD"
#define CPUID_VENDOR_INTEL        "GenuineIntel"
#define CPUID_VENDOR_VIA          "CentaurHauls"
#define CPUID_VENDOR_OLDTRANSMETA "TransmetaCPU"
#define CPUID_VENDOR_TRANSMETA    "GenuineTMx86"
#define CPUID_VENDOR_CYRIX        "CyrixInstead"
#define CPUID_VENDOR_CENTAUR      "CentaurHauls"
#define CPUID_VENDOR_NEXGEN       "NexGenDriven"
#define CPUID_VENDOR_UMC          "UMC UMC UMC "
#define CPUID_VENDOR_SIS          "SiS SiS SiS "
#define CPUID_VENDOR_NSC          "Geode by NSC"
#define CPUID_VENDOR_RISE         "RiseRiseRise"




enum
{
	CPUID_FEAT_ECX_SSE3 = 1 << 0,
	CPUID_FEAT_ECX_PCLMUL = 1 << 1,
	CPUID_FEAT_ECX_DTES64 = 1 << 2,
	CPUID_FEAT_ECX_MONITOR = 1 << 3,
	CPUID_FEAT_ECX_DS_CPL = 1 << 4,
	CPUID_FEAT_ECX_VMX = 1 << 5,
	CPUID_FEAT_ECX_SMX = 1 << 6,
	CPUID_FEAT_ECX_EST = 1 << 7,
	CPUID_FEAT_ECX_TM2 = 1 << 8,
	CPUID_FEAT_ECX_SSSE3 = 1 << 9,
	CPUID_FEAT_ECX_CID = 1 << 10,
	CPUID_FEAT_ECX_FMA = 1 << 12,
	CPUID_FEAT_ECX_CX16 = 1 << 13,
	CPUID_FEAT_ECX_ETPRD = 1 << 14,
	CPUID_FEAT_ECX_PDCM = 1 << 15,
	CPUID_FEAT_ECX_DCA = 1 << 18,
	CPUID_FEAT_ECX_SSE4_1 = 1 << 19,
	CPUID_FEAT_ECX_SSE4_2 = 1 << 20,
	CPUID_FEAT_ECX_x2APIC = 1 << 21,
	CPUID_FEAT_ECX_MOVBE = 1 << 22,
	CPUID_FEAT_ECX_POPCNT = 1 << 23,
	CPUID_FEAT_ECX_AES = 1 << 25,
	CPUID_FEAT_ECX_XSAVE = 1 << 26,
	CPUID_FEAT_ECX_OSXSAVE = 1 << 27,
	CPUID_FEAT_ECX_AVX = 1 << 28,

	CPUID_FEAT_EDX_FPU = 1 << 0,
	CPUID_FEAT_EDX_VME = 1 << 1,
	CPUID_FEAT_EDX_DE = 1 << 2,
	CPUID_FEAT_EDX_PSE = 1 << 3,
	CPUID_FEAT_EDX_TSC = 1 << 4,
	CPUID_FEAT_EDX_MSR = 1 << 5,
	CPUID_FEAT_EDX_PAE = 1 << 6,
	CPUID_FEAT_EDX_MCE = 1 << 7,
	CPUID_FEAT_EDX_CX8 = 1 << 8,
	CPUID_FEAT_EDX_APIC = 1 << 9,
	CPUID_FEAT_EDX_SEP = 1 << 11,
	CPUID_FEAT_EDX_MTRR = 1 << 12,
	CPUID_FEAT_EDX_PGE = 1 << 13,
	CPUID_FEAT_EDX_MCA = 1 << 14,
	CPUID_FEAT_EDX_CMOV = 1 << 15,
	CPUID_FEAT_EDX_PAT = 1 << 16,
	CPUID_FEAT_EDX_PSE36 = 1 << 17,
	CPUID_FEAT_EDX_PSN = 1 << 18,
	CPUID_FEAT_EDX_CLF = 1 << 19,
	CPUID_FEAT_EDX_DTES = 1 << 21,
	CPUID_FEAT_EDX_ACPI = 1 << 22,
	CPUID_FEAT_EDX_MMX = 1 << 23,
	CPUID_FEAT_EDX_FXSR = 1 << 24,
	CPUID_FEAT_EDX_SSE = 1 << 25,
	CPUID_FEAT_EDX_SSE2 = 1 << 26,
	CPUID_FEAT_EDX_SS = 1 << 27,
	CPUID_FEAT_EDX_HTT = 1 << 28,
	CPUID_FEAT_EDX_TM1 = 1 << 29,
	CPUID_FEAT_EDX_IA64 = 1 << 30,
	CPUID_FEAT_EDX_PBE = 1 << 31
};





// this struct defines the way the registers are stored on the stack during a system call.
struct x86_reg
{
	ULONG	ebx;			 // Base Register
	ULONG	ecx;			 // Counter Register
	ULONG	edx;			 // Data Register
	ULONG	esi;			 // Source Index
	ULONG	edi;			 // Destination Index
	ULONG	ebp;			 // Base Pointer
	ULONG	eax;			 // Accumulator Register
	ULONG	esp;			 // Stack Pointer Register
	ULONG	eflags;			 // Status Register
	ULONG	ds;				 //
	ULONG	es;				 //
	ULONG	fs;				 //
	ULONG	gs;				 //
	//ULONG orig_eax;		 //
	ULONG	eip;			 //
	ULONG	cs;				 //
	ULONG	ss;				 //
	ULONG	cr0, cr2, cr3, cr4;
	ULONG	gtdr, idtr, ldtr, tr;

};

//
//
//
#define save_regs(regs)	_asm				\
{											\
	_asm mov dword ptr[regs].ebx, ebx		\
	_asm mov dword ptr[regs].ecx, ecx		\
	_asm mov dword ptr[regs].edx, edx		\
	_asm mov dword ptr[regs].esi, esi		\
	_asm mov dword ptr[regs].edi, edi		\
	_asm mov dword ptr[regs].ebp, ebp		\
	_asm mov dword ptr[regs].eax, eax		\
	_asm mov dword ptr[regs].esp, esp		\
	_asm mov word ptr[regs].ss, ss			\
	_asm mov word ptr[regs].cs, cs			\
	_asm mov word ptr[regs].ds, ds			\
	_asm mov word ptr[regs].es, es			\
	_asm pushfd								\
	_asm pop dword ptr[regs].eflags			\
	_asm push eax							\
	_asm mov eax, cr0						\
	_asm mov dword ptr[regs].cr0, eax		\
	_asm mov eax, cr2						\
	_asm mov dword ptr[regs].cr2, eax		\
	_asm mov eax, cr3						\
	_asm mov dword ptr[regs].cr3, eax		\
	_asm pop eax							\
}





#define	CR0_PROTECTED_MODE_BIT			0
#define	CR0_MONITOR_COPROCESSOR_BIT		1
#define	CR0_EMULATION_BIT				2
#define	CR0_TASK_SWITCHED_BIT			3
#define	CR0_EXTENSION_TYPE_BIT			4
#define	CR0_NUMERIC_ERROR_BIT			5
#define	CR0_WRITE_PROTECT_BIT			16
#define	CR0_ALIGNMENT_MASK_BIT			18
#define	CR0_NOT_WRITE_THROUGH_BIT		29
#define	CR0_CACHE_DISABLE_BIT			30
#define CR0_PAGING_BIT					31

#define	CR3_VIRTUAL_MODE_EXT_BIT						0
#define	CR3_PMODE_VINTERRUPTS_BIT						1
#define	CR3_TIME_STAMP_DISABLE_BIT						2
#define	CR3_DEBUGGING_EXT_BIT							3
#define	CR3_PAGE_SIZE_EXT_BIT							4
#define	CR3_PHYSICAL_ADDRESS_EXT_BIT					5
#define	CR3_MACHINE_CHECK_EXT_BIT						6
#define	CR3_PAGE_GLOBAL_ENABLE_BIT						7
#define	CR3_PERFORMANCE_MONITORING_COUNTER_ENABLE_BIT	8
#define	CR3_OS_FX_SUPPORT_BIT							9
#define	CR3_OS_UNMASKED_SIMD_BIT						10
#define	CR3_VM_EXT_ENABLE_BIT							13
#define	CR3_SAFER_MODE_EXT_ENABLE_BIT					14
#define	CR3_PCID_ENABLE_BIT								17
#define	CR3_OS_PROC_EXT_ENABLE_BIT						18
#define	CR3_SUPERVISOR_MODE_EXECUTION_BIT				20
#define	CR3_SUPERVISOR_MODE_ACCESS_BIT					21


#define	EFLAGS_CF_BIT			0
#define	EFLAGS_PF_BIT			2
#define	EFLAGS_AF_BIT			4
#define	EFLAGS_ZF_BIT			6
#define	EFLAGS_SF_BIT			7
#define	EFLAGS_TF_BIT			8
#define	EFLAGS_IF_BIT			9
#define	EFLAGS_DF_BIT			10
#define	EFLAGS_OF_BIT			11
#define	EFLAGS_NT_BIT			14
#define	EFLAGS_RF_BIT			16
#define	EFLAGS_VM_BIT			17
#define	EFLAGS_AC_BIT			18
#define	EFLAGS_VIF_BIT			19
#define	EFLAGS_VIP_BIT			20
#define	EFLAGS_ID_BIT			21




#endif // !CPU_H
