#ifndef	EXEC_TYPES_H
#define	EXEC_TYPES_H


#define GLOBAL		extern							// the declaratory use of an external 
//#define IMPORT		extern							// reference to an external 
#define STATIC		static							// a local static variable 
#define REGISTER	register						// a (hopefully) register variable 
#define EXTERN		extern "C"						//
#define EXPORT		EXTERN __declspec(dllexport)    // exported from DLL
#define IMPORT		EXTERN __declspec(dllimport)    // imported from DLL
#define NAKED		__declspec(naked)				// no prolog or epilog code added
#define NORETURN	__declspec(noreturn)

#ifndef VOID
#define VOID void
#endif

#ifndef CONST
#define CONST const
#endif


typedef void *				APTR;			// 32-bit untyped pointer 

typedef long long			QUAD;			// signed 64-bit quantity					int64_t
typedef unsigned long long	UQUAD;			// unsigned 64-bit quantity					uint64_t
typedef long				LONG;			// signed 32-bit quantity					int32_t
typedef unsigned long		ULONG;			// unsigned 32-bit quantity					uint32_t
typedef short				WORD;			// signed 16-bit quantity					int16_t
typedef unsigned short		UWORD;			// unsigned 16-bit quantity					uint16_t
typedef char				BYTE;			// signed 8-bit quantity					int8_t
typedef unsigned char		UBYTE;			// unsigned 8-bit quantity					uint8_t

typedef const char *		STRPTR;			// string pointer (NULL terminated) 


typedef unsigned long		LONGBITS;		// 32 bits manipulated individually 
typedef unsigned short		WORDBITS;		// 16 bits manipulated individually			
typedef unsigned char		BYTEBITS;		// 8 bits manipulated individually 

// Types with specific semantics 
typedef float				FLOAT;
typedef double				DOUBLE;
typedef short				BOOL;
typedef unsigned char		TEXT;

#ifndef TRUE
#define TRUE				1
#endif
#ifndef FALSE
#define FALSE				0
#endif
#ifndef NULL
#define NULL				0L
#endif


#define BYTEMASK			0xFF




//
// Set bit
__inline UBYTE SetBit(UBYTE word, UBYTE mask)
{
	return word | mask;
}

// Clears bit
__inline UBYTE ClrBit(UBYTE word, UBYTE mask) 
{
	return word & (~mask);
}

// Assigns the bit defined in mask to a value
__inline UBYTE AssignBit(UBYTE word, UBYTE mask, BOOL value) 
{
	return value ? SetBit(word, mask) : ClrBit(word, mask);
}







//
// Sizes of integral types
// 
#define	CHAR_BIT	8					// max # of bits in a "char" 
#define	SCHAR_MIN	(-128)				// min value of a "signed char" 
#define	SCHAR_MAX	127					// max value of a "signed char" 
#define	UCHAR_MAX	255					// max value of an "unsigned char" 

#define	MB_LEN_MAX	5

#define	CHAR_MIN	SCHAR_MIN			// min value of a "char" 
#define	CHAR_MAX	SCHAR_MAX			// max value of a "char" 

#define	SHRT_MIN	(-32768)			// min value of a "short int" 
#define	SHRT_MAX	(32767)				// max value of a "short int" 
#define	USHRT_MAX	(65535)				// max value of "unsigned short int" 
#define	INT_MIN		(-2147483647-1)		// min value of an "int" 
#define	INT_MAX		(2147483647)		// max value of an "int" 
#define	UINT_MAX	(4294967295U)		// max value of an "unsigned int" 
#define	LONG_MIN	(-2147483647L-1L)	// min value of a "long int" 
#define	LONG_MAX	(2147483647L)		// max value of a "long int" 
#define	ULONG_MAX	(4294967295UL) 		// max value of "unsigned long int" 




#endif

