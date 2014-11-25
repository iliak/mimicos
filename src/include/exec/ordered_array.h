#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

// ordered_array.h -- Interface for creating, inserting and deleting
//                    from ordered arrays.
//                    Written for JamesM's kernel development tutorials.


#include <exec\types.h>




// A predicate should return nonzero if the first argument is less than the second. Else 
// it should return zero.
typedef UBYTE (*lessthan_predicate_t)(APTR, APTR);
typedef struct
{
    APTR *					array;
    ULONG					size;
    ULONG					max_size;
    lessthan_predicate_t	less_than;
} ordered_array;

//	A standard less than predicate.
UBYTE standard_lessthan_predicate(APTR a, APTR b);

//	Create an ordered array.
ordered_array create_ordered_array(ULONG max_size, lessthan_predicate_t less_than);
ordered_array place_ordered_array(void *addr, ULONG max_size, lessthan_predicate_t less_than);

//   Destroy an ordered array.
void destroy_ordered_array(ordered_array *array);

//   Add an item into the array.
void insert_ordered_array(APTR item, ordered_array *array);

//   Lookup the item at index i.
APTR lookup_ordered_array(ULONG i, ordered_array *array);

// Deletes the item at location i from the array.
void remove_ordered_array(ULONG i, ordered_array *array);

#endif
