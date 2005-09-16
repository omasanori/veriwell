/*****************************************************************************
 * Copyright 1994-2005, Elliot Mednick and Mark Hummel
 * This file is part of Veriwell.
 *
 * Veriwell is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Veriwell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *****************************************************************************/
#define OBSTACK_C

#include "glue.h"
#include "obstack.h"
#include "vtypes.h"
#include "tree.h"
#include "veriwell.h"
#include <string.h>
/**************************************************************************************
	obstack_init
		- initialize obstack
**************************************************************************************/
void obstack_init(obstack_t * ostack)
{
    chunk_t *chunk;
    ASSERT(ostack != NULL);
    chunk = (chunk_t *) xmalloc(OBSTACK_DEFAULT_BLOCK_SIZE);
    chunk->next = NULL;
    chunk->freeOffset = 0;
    chunk->objectOffset = 0;
    chunk->size = OBSTACK_DEFAULT_BLOCK_SIZE - (sizeof(chunk_t) - 4);
    ostack->defaultSize = chunk->size;
    ostack->alignment = 8;	/* Sparcs like 8 */
    ostack->next = chunk;
}

/**************************************************************************************
	obstack_next_free
		- return next free byte
**************************************************************************************/
void *obstack_next_free(obstack_t * ostack)
{
    chunk_t *chunk;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);

    chunk = ostack->next;
    ASSERT(chunk->freeOffset <= chunk->size);
    ASSERT(chunk->objectOffset <= chunk->size);
    ASSERT(chunk->freeOffset >= 0);
    ASSERT(chunk->objectOffset >= 0);
    return (&chunk->data[chunk->freeOffset]);
}

/**************************************************************************************
	obstack_base
		- return base of current object
**************************************************************************************/
void *obstack_base(obstack_t * ostack)
{
    chunk_t *chunk;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);
    chunk = ostack->next;
    ASSERT(chunk->freeOffset <= chunk->size);
    ASSERT(chunk->objectOffset <= chunk->size);
    ASSERT(chunk->freeOffset >= 0);
    ASSERT(chunk->objectOffset >= 0);
    return (&chunk->data[chunk->objectOffset]);
}

/**************************************************************************************
	obstack_set_base
		- force current object base
**************************************************************************************/
void obstack_set_base(obstack_t * ostack, void *object)
{
    chunk_t *chunk;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);

    chunk = ostack->next;
    ASSERT(chunk->freeOffset <= chunk->size);
    ASSERT(chunk->objectOffset <= chunk->size);
    ASSERT(chunk->freeOffset >= 0);
    ASSERT(chunk->objectOffset >= 0);
    ASSERT(&chunk->data[0] <= (char *) object);
    ASSERT(&chunk->data[chunk->size] >= (char *) object);

    chunk->objectOffset = ((char *) object) - &chunk->data[0];
}

/**************************************************************************************
	obstack_ptr_grow
		- grow obstack by one pointer and copy object ptr into it
**************************************************************************************/
void obstack_ptr_grow(obstack_t * ostack, void *object)
{
    obstack_grow(ostack, &object, sizeof(object));
}

/**************************************************************************************
	obstack_object_size
		- return size of current object
**************************************************************************************/
int obstack_object_size(obstack_t * ostack)
{
    chunk_t *chunk;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);

    chunk = ostack->next;
    ASSERT(chunk->freeOffset <= chunk->size);
    ASSERT(chunk->objectOffset <= chunk->size);
    ASSERT(chunk->freeOffset >= 0);
    ASSERT(chunk->objectOffset >= 0);
    return (chunk->freeOffset - chunk->objectOffset);
}

/**************************************************************************************
	obstack_copy
		- copy object into obstack, returns ptr to data in obstack
**************************************************************************************/
void *obstack_copy(obstack_t * ostack, void *object, int size)
{
    void *ptr;
    ptr = obstack_alloc(ostack, size);
    memcpy(ptr, object, size);
    return (ptr);
}

/**************************************************************************************
	obstack_copy0
		- copy object into obstack with null terminator, returns ptr to data in obstack
**************************************************************************************/
void *obstack_copy0(obstack_t * ostack, void *object, int size)
{
    void *ptr;
    ptr = obstack_alloc(ostack, size + 1);
    memcpy(ptr, object, size);
    ((char *) ptr)[size] = 0;
    return (ptr);
}

/**************************************************************************************
	obstack_finish
		- finalize object by advance object base to next free spot(aligned)
**************************************************************************************/
void *obstack_finish(obstack_t * ostack)
{
    chunk_t *chunk;
    int offset;
    int startOffset;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);

    chunk = ostack->next;
    ASSERT(chunk->freeOffset <= chunk->size);
    ASSERT(chunk->objectOffset <= chunk->size);
    ASSERT(chunk->freeOffset >= 0);
    ASSERT(chunk->objectOffset >= 0);
    offset = chunk->freeOffset;
    startOffset = chunk->objectOffset;
    offset =
	((offset + ostack->alignment -
	  1) / ostack->alignment) * ostack->alignment;
    if (offset > chunk->size) {
	offset = chunk->size;
    }
    chunk->freeOffset = offset;
    chunk->objectOffset = offset;
    return (&chunk->data[startOffset]);
}

/**************************************************************************************
	obstack_alloc
		- allocate space and advance object. return start of object
**************************************************************************************/
void *obstack_alloc(obstack_t * ostack, int size)
{
    void *ptr;
    chunk_t *chunk;

    ptr = grow_chunk(ostack, size);
    ASSERT(ptr == obstack_base(ostack));

    return (obstack_finish(ostack));
}

/**************************************************************************************
	obstack_grow
		- append data to current object
**************************************************************************************/
void obstack_grow(obstack_t * ostack, void *data, int size)
{
    void *ptr;

    ptr = grow_chunk(ostack, size);
    memcpy(ptr, data, size);
}

/**************************************************************************************
	obstack_free
		- free all storage after object
			if object is 0, free all
**************************************************************************************/
void obstack_free(obstack_t * ostack, void *object)
{
    chunk_t *chunk;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);

    chunk = ostack->next;
    while (chunk && chunk->next) {
	ASSERT(chunk->freeOffset >= 0);
	ASSERT(chunk->objectOffset >= 0);
	ASSERT(chunk->freeOffset <= chunk->size);
	ASSERT(chunk->objectOffset <= chunk->size);
	if ((char *) object >= &chunk->data[0]
	    && ((char *) object <= (&chunk->data[0] + chunk->size))) {
	    break;
	}
	ostack->next = chunk->next;
	free(chunk);
	chunk = ostack->next;
    }
    if (object == 0) {
	chunk->freeOffset = 0;
	chunk->objectOffset = 0;
    } else {
	ASSERT((char *) object >= &chunk->data[0]
	       && ((char *) object <= (&chunk->data[0] + chunk->size)));
	chunk->objectOffset = (char *) object - &chunk->data[0];
	chunk->freeOffset = chunk->objectOffset;
    }

}

/**************************************************************************************
	grow_chunk
		- make room for data, allocate new hunk if needed
**************************************************************************************/
static void *grow_chunk(obstack_t * ostack, int size)
{
    chunk_t *chunk;
    chunk_t *newChunk;
    int currentSize;
    int newSize;
    void *ptr;
    ASSERT(ostack != NULL);
    ASSERT(ostack->next != NULL);
    chunk = ostack->next;
    ASSERT(chunk->freeOffset <= chunk->size);
    ASSERT(chunk->objectOffset <= chunk->size);
    ASSERT(chunk->freeOffset >= 0);
    ASSERT(chunk->objectOffset >= 0);
    if (chunk->size - chunk->freeOffset < size) {
	newSize = OBSTACK_DEFAULT_BLOCK_SIZE - (sizeof(chunk_t) - 4);
	if (newSize < size) {
	    newSize = size;
	}
	currentSize = chunk->freeOffset - chunk->objectOffset;
	newChunk = (chunk_t *) xmalloc(newSize + (sizeof(chunk_t) - 4));
	newChunk->next = chunk;
	newChunk->freeOffset = currentSize;
	newChunk->objectOffset = 0;
	newChunk->size = newSize;
	memcpy(&newChunk->data[0], &chunk->data[chunk->objectOffset],
	       currentSize);
	ostack->next = newChunk;
	chunk = newChunk;
    }

    ptr = &chunk->data[chunk->freeOffset];
    chunk->freeOffset += size;
    return (ptr);

}
