#ifndef UTILITY_TAGITEM_H
#define UTILITY_TAGITEM_H
//
//
//	Extended specification mechanism
//


//***************************************************************************


#include <exec/types.h>


//***************************************************************************
// Tags are a general mechanism of extensible data arrays for parameter
// specification and property inquiry. In practice, tags are used in arrays,
// or chain of arrays.
//
 

typedef ULONG Tag;

struct TagItem
{
    Tag	  ti_Tag;	// identifies the type of data 
    ULONG ti_Data;	// type-specific data	       
};

// constants for Tag.ti_Tag, control tag values 
#define TAG_DONE   (0UL)		// terminates array of TagItems. ti_Data unused 
#define TAG_END	   (0UL)		// synonym for TAG_DONE			  
#define	TAG_IGNORE (1UL)		// ignore this item, not end of array		  
#define	TAG_MORE   (2UL)		// ti_Data is pointer to another array of TagItems
								// note that this tag terminates the current array 
#define	TAG_SKIP   (3UL)		// skip this and the next ti_Data items	  

// differentiates user tags from control tags 
#define TAG_USER   ((ULONG)(1UL<<31))

// If the TAG_USER bit is set in a tag number, it tells utility.library that
// the tag is not a control tag (like TAG_DONE, TAG_IGNORE, TAG_MORE) and is
// instead an application tag.
 


//***************************************************************************


// Tag filter logic specifiers for use with FilterTagItems() 
#define TAGFILTER_AND 0		// exclude everything but filter hits	
#define TAGFILTER_NOT 1		// exclude only filter hits		


//***************************************************************************


// Mapping types for use with MapTags() 
#define MAP_REMOVE_NOT_FOUND 0	// remove tags that aren't in mapList 
#define MAP_KEEP_NOT_FOUND   1	// keep tags that aren't in mapList   


//***************************************************************************


#endif // UTILITY_TAGITEM_H 
