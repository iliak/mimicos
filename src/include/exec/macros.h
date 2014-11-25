#ifndef	MACROS_H
#define	MACROS_H

#include <exec/types.h>


// Align on a boundary (MUST be a power of 2), so that return value <= val
#define ALIGN_INF(val, boundary)			(((ULONG)(val)) & (~((boundary)-1)))

// Align on a boundary (MUST be a power of 2), so that return value >= val
//#define ALIGN_SUP(val, boundary)			({ ULONG __bnd=(boundary); (((((ULONG)(val))-1) & (~(__bnd - 1))) + __bnd); })
#define ALIGN_SUP(val, boundary)			(((((ULONG)(val))-1) & (~(boundary - 1))) + boundary)

// Check whether val is aligned on a boundary (MUST be a power of 2)
#define IS_ALIGNED(val, boundary)		( 0 == (((ULONG)(val)) & ((boundary)-1)) )

// 
// @return TRUE if val is a power of 2.
// @note val is evaluated multiple times
#define IS_POWER_OF_2(val)			((((val) - 1) & (val)) == 0)



// Round down <x> to a multiple of <r>. <r> must be a power of two
#define ROUNDDOWN(x, r) ((x) & ~((r) - 1))

// Round up <x> to a multiple of <r>. <r> must be a power of two
#define ROUNDUP(x, r) (((x) + ((r) - 1)) &  ~((r) - 1))


// Check if a bit is set
#define CHECK_BIT(var, n) ((var) & (1 << (n)))


#endif
