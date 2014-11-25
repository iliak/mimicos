#ifndef _EXCEPTION_H
#define _EXCEPTION_H
//****************************************************************************
//**
//**    exception.h
//**		system exception handlers. These are registered during system
//**		initialization and called automatically when they are encountered
//**
//****************************************************************************



#include <exec/types.h>


extern void _cdecl kernel_panic(const char* fmt, ...);


#define intstart()				\
	_asm	cli					\
	_asm	sub		ebp, 4


// exception handlers

// divide by 0
extern void _cdecl divide_by_zero_fault(ULONG eflags, ULONG cs, ULONG eip, ULONG other);

// single step
extern void _cdecl single_step_trap(ULONG cs, ULONG eip, ULONG eflags);

// non maskable interrupt trap
extern void _cdecl nmi_trap(ULONG cs, ULONG eip, ULONG eflags);

// breakpoint hit
extern void _cdecl breakpoint_trap(ULONG cs, ULONG eip, ULONG eflags);

// overflow
extern void _cdecl overflow_trap(ULONG cs, ULONG eip, ULONG eflags);

// bounds check
extern void _cdecl bounds_check_fault(ULONG cs, ULONG eip, ULONG eflags);

// invalid opcode / instruction
extern void _cdecl invalid_opcode_fault(ULONG cs, ULONG eip, ULONG eflags);

// device not available
extern void _cdecl no_device_fault(ULONG cs, ULONG eip, ULONG eflags);

// double fault
extern void _cdecl double_fault_abort(ULONG cs, ULONG err, ULONG eip, ULONG eflags);

// invalid Task State Segment(TSS)
extern void _cdecl invalid_tss_fault(ULONG cs, ULONG err, ULONG eip, ULONG eflags);

// segment not present
extern void _cdecl no_segment_fault(ULONG cs, ULONG err, ULONG eip, ULONG eflags);

// stack fault
extern void _cdecl stack_fault(ULONG cs, ULONG err, ULONG eip, ULONG eflags);

// general protection fault
extern void _cdecl general_protection_fault(ULONG cs, ULONG err, ULONG eip, ULONG eflags);

// page fault
extern void _cdecl page_fault(ULONG cs, ULONG err, ULONG eip, ULONG eflags);

// Floating Point Unit(FPU) error
extern void _cdecl fpu_fault(ULONG cs, ULONG eip, ULONG eflags);

// alignment check
extern void _cdecl alignment_check_fault(ULONG cs, ULONG err,ULONG eip, ULONG eflags);

// machine check
extern void _cdecl machine_check_abort(ULONG cs, ULONG eip, ULONG eflags);

// Floating Point Unit(FPU) Single Instruction Multiple Data(SIMD) error
extern void _cdecl simd_fpu_fault(ULONG cs, ULONG eip, ULONG eflags);


#endif
