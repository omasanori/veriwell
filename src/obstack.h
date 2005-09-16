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

/* obstack.h - obstack memory management routines */

#ifndef OBSTACK_H
#define OBSTACK_H
// public definitions
// generic pointer definition
typedef void *POINTER;
// obstack chunk definition
typedef struct obstackChunk_tag {
    struct obstackChunk_tag *next;	// pointer to next allocated chunk
    int    size;			// data size of this chunk
    int    objectOffset;		// offset of current object
    int    freeOffset;			// offset of next free byte
    char   data[4];			// start of data area
} chunk_t;
// obstack header definition
typedef struct obstack {
    chunk_t *next;		// pointer to next allocated chunk
    int      alignment;		// number of byte to align to
    int      defaultSize;	// number of data byte to allocate by default
} obstack_t;
// prototypes
void obstack_init(obstack_t *);
void *obstack_alloc(obstack_t *, int);
void obstack_free(obstack_t *, void *);
void obstack_set_base(obstack_t *, void *);
void *obstack_base(obstack_t *);
void *obstack_next_free(obstack_t *);
void *obstack_finish(obstack_t *);
void *obstack_copy(obstack_t *, void *, int);
void *obstack_copy0(obstack_t *, void *, int);
int obstack_object_size(obstack_t *);
void obstack_grow(obstack_t *, void *, int);
void obstack_ptr_grow(obstack_t *, void *);
#ifdef OBSTACK_C
// private definitions
#define OBSTACK_DEFAULT_BLOCK_SIZE	(4096)
static void *grow_chunk(obstack_t *, int);
#endif				// OBSTACK_C
#endif				// OBSTACK_H
