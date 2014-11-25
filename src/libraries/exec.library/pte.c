
//****************************************************************************
//**
//**    paging_pte.cpp
//**		-Page Table Entries(PTE). This provides an abstract interface
//**	to aid in management of PTEs.
//**
//****************************************************************************

#include "pte.h"

void pte_set_attrib(pt_entry* e, ULONG attrib)
{
	*e |= attrib;
}

void pte_del_attrib(pt_entry* e, ULONG attrib)
{
	*e &= ~attrib;
}

void pte_set_frame(pt_entry* e, phy_addr addr)
{
	*e =(*e & ~I86_PTE_FRAME) | addr;
}

BOOL pte_is_present(pt_entry e)
{
	return e & I86_PTE_PRESENT;
}

BOOL pte_is_writable(pt_entry e)
{
	return e & I86_PTE_WRITABLE;
}

phy_addr pte_pfn(pt_entry e)
{
	return e & I86_PTE_FRAME;
}

