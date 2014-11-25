#ifndef	GRAPHICS_GFX_H
#define	GRAPHICS_GFX_H


#include <exec/types.h>

#define BITSET	0x8000
#define BITCLR	0



struct Rectangle
{
    WORD   MinX,MinY;
    WORD   MaxX,MaxY;
};

struct Rect32
{
    LONG    MinX,MinY;
    LONG    MaxX,MaxY;
};

typedef struct tPoint
{
    WORD	x,y;
} Point;

typedef UBYTE *PLANEPTR;

struct BitMap
{
    UWORD		BytesPerRow;
    UWORD		Rows;
    UBYTE		Flags;
    UBYTE		Depth;
    UWORD		pad;
    PLANEPTR	Planes[8];
};


// flags for AllocBitMap, etc. 
#define BMB_CLEAR 0
#define BMB_DISPLAYABLE 1
#define BMB_INTERLEAVED 2
#define BMB_STANDARD 3
#define BMB_MINPLANES 4

#define BMF_CLEAR (1l<<BMB_CLEAR)
#define BMF_DISPLAYABLE (1l<<BMB_DISPLAYABLE)
#define BMF_INTERLEAVED (1l<<BMB_INTERLEAVED)
#define BMF_STANDARD (1l<<BMB_STANDARD)
#define BMF_MINPLANES (1l<<BMB_MINPLANES)

// the following are for GetBitMapAttr() 
#define BMA_HEIGHT 0
#define BMA_DEPTH 4
#define BMA_WIDTH 8
#define BMA_FLAGS 12

#endif	
