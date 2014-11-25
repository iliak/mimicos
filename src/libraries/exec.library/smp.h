#ifndef SMP_H
#define SMP_H


//
//  Symmetric Multiprocessing(SMP)
//


#include <exec/types.h>



//
//
struct mps_fps_hdr					// MP Floating Pointer Structure	
{									//
	ULONG	fps_sig;				// _MP_(0x5F4D505F)			
	ULONG	fps_mpct_paddr;			// paddr of MP Configuration Table	
	UBYTE	fps_len;				// in paragraph(16-bytes units)	
	UBYTE	fps_spec_rev;			// MP Spec. version no.			
	UBYTE	fps_cksum;				// checksum of complete structure	
	UBYTE	fps_featinfo1;			// mp feature info byte 1 		
	UBYTE	fps_featinfo2;			// mp feature info byte 2		
	UBYTE	fps_featinfo3;			// mp feature info byte 3		
	UBYTE	fps_featinfo4;			// mp feature info byte 4		
	UBYTE	fps_featinfo5;			// mp feature info byte 5		
};

//
//
struct mps_ct_hdr					// MP Configuration Table Header	
{									//
	ULONG	ct_sig;					// "PCMP"				
	UWORD	ct_len;					// base configuration in bytes 		
	UBYTE	ct_spec_rev;			// MP Spec. version no.			
	UBYTE	ct_cksum;				// base configuration table checksum	
	char	ct_oem_id[8];			// string identifies the manufacturer	
	char	ct_prod_id[12];			// string identifies the product	
	ULONG	ct_oem_ptr;				// paddr to an OEM-defined table	
	UWORD	ct_oem_tbl_len;			// size of base OEM table in bytes	
	UWORD	ct_entry_cnt;			// no. of entries in the base table	
	ULONG	ct_local_apic;			// paddr of local APIC			
	UWORD	ct_ext_tbl_len;			// extended table in bytes 		
	UBYTE	ct_ext_cksum;			// checksum for the extended table	
};

// Base MP Configuration Table entry type definitions 
#define	CPU_TYPE		0
#define	BUS_TYPE		1
#define	IO_APIC_TYPE	2
#define	IO_INTR_TYPE	3
#define	LOCAL_INTR_TYPE	4

// Base MP Configuration Table entry size definitions 
#define	CPU_SIZE		20
#define	BUS_SIZE		8
#define	IO_APIC_SIZE	8
#define	IO_INTR_SIZE	8
#define	LOCAL_INTR_SIZE	8

// Extended MP Configuration Table entry type definitions 
#define	SYS_AS_MAPPING			128
#define	BUS_HIERARCHY_DESC		129
#define	COMP_BUS_AS_MODIFIER	130

// Extended MP Configuration Table entry size definitions 
#define	SYS_AS_MAPPING_SIZE			20
#define	BUS_HIERARCHY_DESC_SIZE		8
#define	COMP_BUS_AS_MODIFIER_SIZE	8

struct sasm						// System Address Space Mapping Entry	
{
	UBYTE sasm_type;			// type 128				
	UBYTE sasm_len;				// entry length in bytes(20)		
	UBYTE sasm_bus_id;			// bus id where this is mapped		
	UBYTE sasm_as_type;			// system address type			

	// system address type definitions 
#define	IO_TYPE		0
#define	MEM_TYPE	1
#define	PREFETCH_TYPE	2
#define	BUSRANGE_TYPE	3
	ULONG sasm_as_base;			// starting address			
	ULONG sasm_as_base_hi;
	ULONG sasm_as_len;			// no. of addresses visiblie to the bus	
	ULONG sasm_as_len_hi;
};

struct bhd {					// Bus Hierarchy Descriptor Entry	
	UBYTE bhd_type;				// type 129				
	UBYTE bhd_len;				// entry length in bytes(8)		
	UBYTE bhd_bus_id;			// bus id of this bus			
	UBYTE bhd_bus_info;			// bus information			
// Bus Information bit definition 
#define	BHD_BUS_INFO_SD	1		// Subtractive Decode Bus		
	UBYTE bhd_parent;
};

struct cbasm					// Compatibility Bus Address Space Modifier Entry 
{
	UBYTE cbasm_type;			// type 130				
	UBYTE cbasm_len;			// entry length in bytes(8)		
	UBYTE cbasm_bus_id;			// bus to be modified			
	UBYTE cbasm_addr_mod;		// address modifier			
// Address Modifier bit definiton 
#define	CBASM_ADDR_MOD_PR	1	// 1 = subtracted, 0 = added 
	ULONG cbasm_pr_list;		// identify list of predefined address ranges 
};



//
// Protos
//

void smp_init(void);


#endif
