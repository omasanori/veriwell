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

/* vtypes.h - Compiler-independent data types; common prototypes */

#ifndef VTYPES_H
#define VTYPES_H

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
#else /* !__cplusplus */ 
#  define BEGIN_C_DECLS
#  define END_C_DECLS 
#endif

enum logical_value { ZERO = 0, ONE = 1, Z = 2, X = 3 };
//#define H_NBITS(X, Y) HIERARCHICAL_ATTR (node) ? TREE_NBITS (X) : TREE_NBITS (Y);
#define H_(X, Y) HIERARCHICAL_ATTR (Y) ? (X) : (Y)
#define NULL_CHAR ((char *) 0)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define BADPTR ((void*)(0xff))

struct SCB;

struct Time64 {
    unsigned timeh;
    unsigned timel;
};

#define ADDTIME6432(RESULT, TIME, NUM)			\
	{						\
	(RESULT)->timeh = ((TIME)->timel > ~(NUM))	\
		? (TIME)->timeh + 1 : (TIME)->timeh;	\
	(RESULT)->timel = (TIME)->timel + (NUM);	\
	}

#define ADDTIME6464(RESULT, TIME1, TIME2)		\
	{						\
	(RESULT)->timeh = ((TIME1)->timel > ~(TIME2)->timel)	\
		? (TIME1)->timeh + (TIME2)->timeh + 1 : (TIME1)->timeh + (TIME2)->timeh;	\
	(RESULT)->timel = (TIME1)->timel + (TIME2)->timel;	\
	}

/* Return 0 if equal, a negative number is TIME1 < TIME2, else a positive number */

#define CMPTIME64(TIME1, TIME2)			\
	((TIME1)->timeh != (TIME2)->timeh		\
		? (((TIME1)->timeh < (TIME2)->timeh)?-1:1)	\
		: (((TIME1)->timel < (TIME2)->timel))?-1: \
		((TIME1)->timel != (TIME2)->timel?1:0))

#define ASSIGNTIME64(DEST, SRC)			\
	{					\
		(DEST)->timeh = (SRC)->timeh;	\
		(DEST)->timel = (SRC)->timel;	\
	}


#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))
#define ALIGN_32
#define PATHSEP "/"
#define CLOCK_T

#define VERIWELL "Veriwell"

/* Attempt to put all machine dependent types here */

#define MAX_BITS_IN_NUMBER 32
/* It takes 8 bytes to store 32 bits; in one 64k segment, there can be
   8192 groups.  Take two away malloc header overhead. */
#define MAX_GROUPS (8192L-2)
#define MAX_BITS (MAX_GROUPS*32)
#define MAX_ARRAY ((unsigned) -1)


typedef unsigned unsigned_32_t;
typedef signed int signed_32_t;
typedef unsigned short unsigned_16_t;
typedef signed short signed_16_t;

/* For storing a single group; for 32-bit compilers, this should be 32 bits. */
typedef unsigned_32_t Bit;
#define BITS_IN_GROUP (sizeof (Bit) * 8)

/* For storing the number of bits for a datum */
typedef signed_32_t nbits_t;

/* For storing the number of groups for a datum */
typedef int ngroups_t;
typedef unsigned_32_t lineno_t;
typedef unsigned_32_t delay_t;
typedef struct Time64 time64;
typedef unsigned_32_t handle_t;
typedef unsigned int array_t;

#define SIZEOF_INTEGER sizeof (Group)


/* number of bits to shift nbits by to get n groups.  5 -> 32 bits, 4 -> 16 */
//#define GROUP_SHIFT_BITS 5
#define bits_to_groups(BITS) ((BITS==0)?0:((Bit)((BITS) - 1) >> 5))
#define chars_to_groups(CHARS) ((Bit)((CHARS) - 1) >> 2)
#define NBITS_MASK 0x1f

#define ABS(N)  ((N) >= 0 ? (N) : -(N))

typedef union group {
    struct {			/*       0  1  z  x */
	Bit aval;		/* aval: 0  1  0  1 */
	Bit bval;		/* bval: 0  0  1  1 */
    } bit;
    double real;
} Group;

#define NGROUPS(p) (p)->ngroups
#define GROUP(p) (p)->group
#define NBITS(p) (p)->nbits
#define D_INTEGER(p) (p)->integer
#define D_CONSTANT(p) (p)->constant

#define AVAL(G) ((G)->bit.aval)
#define BVAL(G) ((G)->bit.bval)

#define REAL_(G) ((G)->real)

#define FLAGS(p) (unsigned_16_t *)(p)


extern Bit mask_right[];
extern Bit mask_right1[];

extern void error(char *, char *, char *);
extern void warning(char *, char *, char *);
extern void sorry(char *);
extern void sorry1(char *, char *);

union tree_node;
extern void store(union tree_node *, union tree_node *);



#if defined(DEBUG) || defined(ASSERTIONS)
#ifndef ASSERT
void _Assert(char *, unsigned);	/* defined in verilog.c */

#define ASSERT(f)			\
    if (f)				\
      NULL;				\
    else				\
      _Assert(__FILE__, __LINE__)

#endif
#else
#ifdef ASSERT
#undef ASSERT
#endif
#define ASSERT(f)	NULL
#endif

#if defined(DEBUG)
#define DEBUGPRINT(P) printf (P)
#else
#define DEBUGPRINT(P)
#endif

#define HEAPTEST(X)

#endif				// VTYPES_H
