// ordered_array.c -- Implementation for creating, inserting and deleting
//                    from ordered arrays.
//                    Written for JamesM's kernel development tutorials.

#include <exec/ordered_array.h>
#include <string.h>
#include "kheap.h"

BYTE standard_lessthan_predicate(APTR a, APTR b)
{
	return (a < b) ? 1 : 0;
}

ordered_array create_ordered_array(ULONG max_size, lessthan_predicate_t less_than)
{
	ordered_array to_ret;
	to_ret.array = (APTR)kmalloc(max_size * sizeof(APTR));
	memset(to_ret.array, 0, max_size * sizeof(APTR));
	to_ret.size = 0;
	to_ret.max_size = max_size;
	to_ret.less_than = less_than;
	return to_ret;
}

ordered_array place_ordered_array(void *addr, ULONG max_size, lessthan_predicate_t less_than)
{
	ordered_array to_ret;
	to_ret.array = (APTR*)addr;
	memset(to_ret.array, 0, max_size*sizeof(APTR));
	to_ret.size = 0;
	to_ret.max_size = max_size;
	to_ret.less_than = less_than;
	return to_ret;
}

void destroy_ordered_array(ordered_array *array)
{
	//    kfree(array->array);
}

void insert_ordered_array(APTR item, ordered_array *array)
{
	//ASSERT(array->less_than);
	ULONG iterator = 0;
	while (iterator < array->size && array->less_than(array->array[iterator], item))
		iterator++;
	if (iterator == array->size) // just add at the end of the array.
		array->array[array->size++] = item;
	else
	{
		APTR tmp = array->array[iterator];
		array->array[iterator] = item;
		while (iterator < array->size)
		{
			iterator++;
			APTR tmp2 = array->array[iterator];
			array->array[iterator] = tmp;
			tmp = tmp2;
		}
		array->size++;
	}
}

APTR lookup_ordered_array(ULONG i, ordered_array *array)
{
	//ASSERT(i < array->size);
	return array->array[i];
}

void remove_ordered_array(ULONG i, ordered_array *array)
{
	while (i < array->size)
	{
		array->array[i] = array->array[i + 1];
		i++;
	}
	array->size--;
}
