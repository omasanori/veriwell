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

/* EVAL.C - Interpretively evaluate operands and operators */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "vtypes.h"
#include "tree.h"
#include "acc_user.h"
#include "pli.h"
#include "schedule.h"
#include "runtime.h"
#include "systask.h"
#include "flags.h"
#include "io.h"
#include "eval.h"
#include "glue.h"
#include "multdiv.h"
#include "store.h"
#include "eval.h"
#include "exec.h"
#include "veriwell.h"

static int xor_reduce(Bit a, int l);
static void copy_left(nbits_t size, nbits_t multiplier, Group * g);
static void load_fill(Group * g, int ngroups, Bit aval, Bit bval);
static void load_1bit(Group * g, int ngroups, int bit);
static void eval_and_retain_flags(tree * pc);

/* Array of virtual accumulator registers.  "r" is the register at the
   top of the stack, that is, the register to be used next.  The size
   of the register is dynamically set, depending on the operation. */
Group **R_base;
Group **R;
int current_area = 0;
int current_regs = 0;
nbits_t R_nbits = 0;
ngroups_t R_ngroups = 0;
Bit R_mask;
tree global_shadow;		/* Used for eval_nbits */
double x32;			/* This is set to 2^32 */

/* Convert a reg type to a double */
double reg_to_real(Group * g, nbits_t nbits)
{
    double x = 0;
    ngroups_t ngroups = bits_to_groups(nbits);
    Group *g1 = g + ngroups;

    x = AVAL(g1) & mask_right1[R_nbits & NBITS_MASK];
    for (int i = ngroups; i > 0; i--) {
	g1--;
	x = ldexp(x, sizeof(Bit) * 8);
	x += AVAL(g1);
    }
    return x;
}

void real_to_reg(double x, Group * g, ngroups_t ngroups)
{
    x += 0.5;			/* round up */
    for (int i = 0; i <= ngroups; i++, g++) {
	AVAL(g) = (Bit) fmod(x, x32);
	BVAL(g) = 0;
	x = ldexp(x, -(sizeof(Bit) * 8));
    }
}

static int xor_reduce(Bit a, int l)
{
    int result = 0;

    for (int i = 0; i < l; i++) {
	if (((a >> i) & 1) == 1) {
	    result = !result;
	}
    }
    return result;
}

int part_lref(Group * g1, Group * g2, struct part_info *info)
{
    Bit lshift = PART_SHIFT(info);
    Bit lmask1 = PART_LMASK1(info);
    Bit aval;
    Bit bval;
    ngroups_t ngroups = PART_NGROUPS(info);
    ngroups_t i;
    int changed = 0;

    if (!ngroups) {
	if (PART_ALIGNED(info)) {
	    /* case 1: reference within one group without crossing a boundary */
	    aval = AVAL(g1);
	    bval = BVAL(g1);
	    AVAL(g1) = ((AVAL(g2) << lshift) & ~lmask1) | (aval & lmask1);
	    BVAL(g1) = ((BVAL(g2) << lshift) & ~lmask1) | (bval & lmask1);
	    changed = aval != AVAL(g1) || bval != BVAL(g1);
	} else {
	    /* case 2: reference within one group on stack, 
	     * but crosses boundary in dest 
	     */
	    Bit lmask2 = PART_LMASK2(info);
	    Bit rshift = sizeof(Bit) * 8 - lshift;

	    aval = AVAL(g1);
	    bval = BVAL(g1);
	    AVAL(g1) = (AVAL(g2) << lshift) | (aval & lmask1);
	    BVAL(g1) = (BVAL(g2) << lshift) | (bval & lmask1);
	    changed = aval != AVAL(g1) || bval != BVAL(g1);
	    g1++;
	    /* if lshift is 0, then rshift is 32 which is an undefined
	       shift count for most compilers */
	    if (lshift) {
		aval = AVAL(g1);
		bval = BVAL(g1);
		AVAL(g1) =
		    ((AVAL(g2) >> rshift) & ~lmask2) | (aval & lmask2);
		BVAL(g1) =
		    ((BVAL(g2) >> rshift) & ~lmask2) | (bval & lmask2);
		changed = changed || aval != AVAL(g1) || bval != BVAL(g1);
	    }
	}
    } else {
	/* case 3 & 4: reference more than one group */
	Bit rshift = sizeof(Bit) * 8 - lshift;
	Bit lmask2 = PART_LMASK2(info);
	aval = AVAL(g1);
	bval = BVAL(g1);
	for (i = 0; i < ngroups; i++) {
	    AVAL(g1) = ((AVAL(g2) << lshift) & ~lmask1) | (aval & lmask1);
	    BVAL(g1) = ((BVAL(g2) << lshift) & ~lmask1) | (bval & lmask1);
	    changed = changed || aval != AVAL(g1) || bval != BVAL(g1);
	    g1++;
	    aval = AVAL(g1);
	    bval = BVAL(g1);
	    if (lshift) {
		AVAL(g1) =
		    ((AVAL(g2) >> rshift) & lmask1) | (aval & ~lmask1);
		BVAL(g1) =
		    ((BVAL(g2) >> rshift) & lmask1) | (bval & ~lmask1);
	    }
	    aval = AVAL(g1);
	    bval = BVAL(g1);
	    g2++;
	}

	if (lmask2 || !lshift) {	/* make sure that there is more stuff to copy */
	    if (PART_ALIGNED(info)) {
		AVAL(g1) =
		    ((AVAL(g2) << lshift) & ~lmask2) | (aval & lmask2);
		BVAL(g1) =
		    ((BVAL(g2) << lshift) & ~lmask2) | (bval & lmask2);
		changed = changed || aval != AVAL(g1) || bval != BVAL(g1);
	    } else {
		AVAL(g1) = (AVAL(g2) << lshift) | (aval & lmask1);
		BVAL(g1) = (BVAL(g2) << lshift) | (bval & lmask1);
		changed = changed || aval != AVAL(g1) || bval != BVAL(g1);
		g1++;
		aval = AVAL(g1);
		bval = BVAL(g1);
		AVAL(g1) = (AVAL(g2) >> rshift) | (aval & lmask2);
		BVAL(g1) = (BVAL(g2) >> rshift) | (bval & lmask2);
		changed = changed || aval != AVAL(g1) || bval != BVAL(g1);
	    }
	}
    }
    return changed;
}

static void copy_left(nbits_t size, nbits_t multiplier, Group * g)
{
    ngroups_t i;
    ngroups_t ngroups = bits_to_groups(size);
    ngroups_t shift_groups;
    nbits_t shift_bits;
    Group *g1;
    Group *g2;
    Bit mask;

    shift_bits = size * (multiplier - 1);
    shift_groups = bits_to_groups(shift_bits + 1);
    shift_bits %= BITS_IN_GROUP;
    if (shift_groups <= 0) {
	shift_groups = 0;
    }

    if (shift_bits + size > BITS_IN_GROUP) {
	ngroups++;
    }

    g1 = g + ngroups + shift_groups;	/* msb of dest */
    g2 = g1 - shift_groups;	/* msb of source */
    mask = mask_right1[size & NBITS_MASK];

    if (shift_groups <= R_ngroups) {
	AVAL(g1) |= ((AVAL(g2) & mask) << shift_bits);	// | AVAL (g1);
	BVAL(g1) |= ((BVAL(g2) & mask) << shift_bits);	// | BVAL (g1);
	for (i = 0; i < ngroups; i++) {
	    g2--;
	    AVAL(g1) |= AVAL(g2) >> (BITS_IN_GROUP - shift_bits);
	    BVAL(g1) |= BVAL(g2) >> (BITS_IN_GROUP - shift_bits);
	    g1--;
	    AVAL(g1) |= (AVAL(g2) << shift_bits);	// | tmp;
	    BVAL(g1) |= (BVAL(g2) << shift_bits);	// | tmp2b;
	}
	g1--;
    }
}

/* Return 0 if result is zero, else return 1 */
/* TREE_NBITS has the size of the current subexpression, that is, it
   is the size of the accumulator.  This is not to be confused with the
   size of a constant being loaded wiich may have to be zero filled to
   the size of the accumulator.  Note that the size of a constant will
   never be larger than the size of the accumulator since the size of the
   accumulator is determined at parse time to be the max of the sizes of
   the subexpression.  Operators that are context-determined (arithmetic)
   don't change the accumulator size; its size is set at the beginning
   of an expression (when constants, decls, or refs are loaded) and on self-
   determined operators (conditionals and reductions). */

/* fill the datum with all of one value (0, 1, X, Z) */

static void load_fill(Group * g, int ngroups, Bit aval, Bit bval)
{
    int i;

    AVAL(g) = aval;
    BVAL(g) = bval;
    for (i = 1; i <= ngroups; i++) {
	g++;
	AVAL(g) = aval;
	BVAL(g) = bval;
    }
}

static void load_1bit(Group * g, int ngroups, int bit)
{
    int i;
    Bit aval;
    Bit bval;

    switch (bit) {
    case (0):
	aval = 0;
	bval = 0;
	break;
    case (1):
	aval = 1;
	bval = 0;
	break;
    case ('x'):
	aval = 1;
	bval = 1;
	break;
    case ('z'):
	aval = 0;
	bval = 1;
	break;
    }
    AVAL(g) = aval;
    BVAL(g) = bval;
    for (i = 1; i <= ngroups; i++) {
	g++;
	AVAL(g) = 0;
	BVAL(g) = 0;
    }
}

ngroups_t part_rref(Group * g1, Group * g2, struct part_info *info)
{
    Bit mask1 = ~PART_LMASK1(info);
    Bit mask2 = ~PART_LMASK2(info);
    int rshift = PART_SHIFT(info);
    Bit rmask = PART_RMASK(info);
    ngroups_t ngroups = PART_NGROUPS(info);
    ngroups_t i;

    if (!rshift) {		/* Aligned */
	if (R_ngroups) {
	    for (i = 0; i < ngroups; i++, g1++, g2++) {
		AVAL(g1) = AVAL(g2);
		BVAL(g1) = BVAL(g2);
	    }
	}
	AVAL(g1) = AVAL(g2) & rmask;	//mask1;//rmask;
	BVAL(g1) = BVAL(g2) & rmask;	//mask1;//rmask;
    } else {			/* there is an offset */
	/* Unaligned, but can be moved with one copy, i.e. no boundary cross */
	if (!ngroups && PART_ALIGNED(info)) {
	    AVAL(g1) = ((AVAL(g2) /*& mask1 */ ) >> rshift) & rmask;
	    BVAL(g1) = ((BVAL(g2) /*& mask1 */ ) >> rshift) & rmask;
	    /* worst case: unaligned and crosses boundary */
	} else {
	    int lshift = sizeof(Bit) * 8 - rshift;
	    /* it takes two copies to move one unaligned group.  Do the
	       copies in pairs so that each pair will form one aligned
	       group on the stack.  Note that we may copy too much, but this
	       will be masked. */
	    for (i = 0; i <= ngroups; i++) {
		AVAL(g1) = AVAL(g2) >> rshift;
		BVAL(g1) = BVAL(g2) >> rshift;
		g2++;
		/* if rshift is 0, then lshift is 32 which is undefined */
		if (rshift) {
		    AVAL(g1) |= AVAL(g2) << lshift;
		    BVAL(g1) |= BVAL(g2) << lshift;
		} else {
		    AVAL(g1) = AVAL(g2);
		    BVAL(g1) = BVAL(g2);
		}
		g1++;
	    }

	    /* Did we copy too much?  If so mask off the stuff we don't want */
	    if (PART_ALIGNED(info)) {
		g1--;
		AVAL(g1) &= rmask;
		BVAL(g1) &= rmask;
		/* we have to make one more copy to the stack */
	    } else {
		AVAL(g1) = (AVAL(g2) >> rshift) /* & mask2 */ ;
		BVAL(g1) = (BVAL(g2) >> rshift) /* & mask2 */ ;
	    }
	}			/* else (crosses boundary) */
    }				/* else (offset) */
    return PART_NEXT(info);
}

int in_cond = 0;
int is_integer = 0;
int is_real = 0;

Group *eval_(tree * pc, nbits_t * nbits)
{
    eval(pc);
    *nbits = R_nbits;
    return *--R;
}

/* eval a single decl node */

void eval_1(tree decl)
{
    tree pc[2];

    pc[0] = decl;
    pc[1] = NULL_TREE;
    eval(pc);
}

static void eval_and_retain_flags(tree * pc)
{
    int saved_integer_flag = is_integer;
    int saved_real_flag = is_real;

    eval(pc);
    is_integer = saved_integer_flag;
    is_real = saved_real_flag;
}

void eval(tree * pc)
{
    tree t;
    tree t1;
    tree t2;
    enum tree_code code;
    enum logical_value cond;
    enum logical_value cond1;
    int tmpint;
    Group *g;
    Group *g1;
    Group *g2;
    ngroups_t i;
    ngroups_t ngroups;
    ngroups_t shift_groups;
    nbits_t shift_bits;
    nbits_t nbits;
    Bit tmp1;
    Bit tmp2;

    /* Test if this should use integer operations.  Note that eval can be
       called recursively from the conditional operator (?:), so the
       integer flag must be preserved [pathological case: (1 ? -12/3 : 1'b1)
       is NOT an integer, so the -12/3 must be evaluated as unsigned]. */
    if (!in_cond) {
	is_integer = TREE_INTEGER_ATTR(*pc);
	is_real = TREE_REAL_ATTR(*pc);
    }
    if (!pc) {
	printf_error_V("NULL pc in EVAL, aborting.\n");
	shell_abort();
    }

    while ((t = *pc) != 0) {
	code = TREE_CODE(t);
	switch (code) {

/***************************************************************************/
/* Value loading                                                           */
/***************************************************************************/

	case INTEGER_DECL:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & (nbits_t) NBITS_MASK];
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    AVAL(g1) = AVAL(g2);
	    BVAL(g1) = BVAL(g2);
	    if (R_ngroups > 0) {
		for (i = 1; i <= R_ngroups; i++) {
		    g1++;
		    g2++;
		    AVAL(g1) = 0;
		    BVAL(g1) = 0;
		}
	    }
	    *++R = g1 + 1;
	    break;

	case REG_VECTOR_DECL:
	case NET_VECTOR_DECL:
	case TMP_DECL:
	case TIME_DECL:
	case PARAM_DECL:
	case SPECPARAM_DECL:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    AVAL(g1) = AVAL(g2);
	    BVAL(g1) = BVAL(g2);
	    if (R_ngroups > 0) {
		for (i = 1; i <= R_ngroups; i++) {
		    g1++;
		    g2++;
		    AVAL(g1) = AVAL(g2);
		    BVAL(g1) = BVAL(g2);
		}
	    }
	    *++R = g1 + 1;
	    break;

	case EVENT_DECL:
	    g1 = *R;
	    AVAL(g1) = 1;	/* Stuff "true" for evaluation purposes (ignored) */
	    BVAL(g1) = 0;
	    *++R = g1 + 1;
	    break;

	case REG_SCALAR_DECL:
	case NET_SCALAR_DECL:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    AVAL(g1) = AVAL(g2);
	    BVAL(g1) = BVAL(g2);
	    if (R_ngroups > 0) {
		for (i = 1; i <= R_ngroups; i++) {
		    g1++;
		    g2++;
		    AVAL(g1) = 0;
		    BVAL(g1) = 0;
		}
	    }
	    *++R = g1 + 1;
	    break;

	case REAL_DECL:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & (nbits_t) NBITS_MASK];
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    REAL_(g1) = REAL_(g2);
	    *++R = g1 + 1;
	    break;

	case SHADOW_REF:	/* This works for vect and scalared types */
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    t = TREE_CHAIN(t);	/* point to original decl */
	    g1 = *R;
	    if (TREE_CODE(t) == EVENT_DECL) {
		AVAL(g1) = 1;	/* true */
		BVAL(g1) = 0;
		for (i = 1; i <= R_ngroups; i++) {
		    g1++;
		    AVAL(g1) = 0;
		    BVAL(g1) = 0;
		}
		*++R = g1 + 1;
		break;
	    }

	    g2 = DECL_STORAGE(t);
	    nbits = TREE_NBITS(t);
	    ngroups = bits_to_groups(nbits);

	    /* Load low full groups */
	    for (i = 0; i < ngroups; i++, g1++, g2++) {
		AVAL(g1) = AVAL(g2);
		BVAL(g1) = BVAL(g2);
	    }
	    /* Load highest, possibly partial, group */
	    AVAL(g1) = AVAL(g2) & mask_right1[nbits & NBITS_MASK];
	    BVAL(g1) = BVAL(g2) & mask_right1[nbits & NBITS_MASK];

	    /* Fill remainder with 0's if necessary, since decl could be
	       smaller than stack size */
	    for (i++; i <= R_ngroups; i++) {
		g1++;
		AVAL(g1) = 0;
		BVAL(g1) = 0;
	    }
	    *++R = g1 + 1;
	    break;

	case ARRAY_REF:
	    /* evaluate the index (guarranteed to be >= 32 bits) */
	    eval_and_retain_flags(BIT_EXPR_CODE(t));
	    cond = ZERO;
	    g1 = *--R;
	    if (BVAL(g1) & R_mask) {
		cond = X;
	    }
	    {
		tree decl = ARRAY_REF_DECL(t);
		ngroups_t array_hi = ARRAY_HI(decl);
		ngroups_t array_lo = ARRAY_LO(decl);
		ngroups_t ngroups = 1 + bits_to_groups(TREE_NBITS(decl));
		Bit aval = AVAL(g1) & R_mask;
		ngroups_t offset;

		if (VECTOR_DIRECTION_ATTR(decl)) {
		    if (aval < array_lo || aval > array_hi) {
			cond = X;
		    } else {
			offset = (aval - array_lo) * ngroups;
		    }
		} else {
		    if (aval < array_hi || aval > array_lo) {
			cond = X;
		    } else {
			offset = (aval - array_hi) * ngroups;
		    }
		}

		R_nbits = TREE_NBITS(t);
		R_ngroups = bits_to_groups(R_nbits);
		R_mask = mask_right1[R_nbits & NBITS_MASK];

		g1 = *R;
		if (cond != X) {
		    g2 = DECL_STORAGE(decl) + offset;
		}

		ngroups = bits_to_groups(TREE_NBITS(decl));
		if (cond != X) {
		    AVAL(g1) = AVAL(g2);
		    BVAL(g1) = BVAL(g2);
		} else {
		    AVAL(g1) = (Bit) - 1;
		    BVAL(g1) = (Bit) - 1;
		}
		for (i = 1; i <= ngroups; i++) {
		    g1++;
		    g2++;
		    if (cond != X) {
			AVAL(g1) = AVAL(g2);
			BVAL(g1) = BVAL(g2);
		    } else {
			AVAL(g1) = (Bit) - 1;
			BVAL(g1) = (Bit) - 1;
		    }
		}
		for (; i <= R_ngroups; i++) {
		    g1++;
		    AVAL(g1) = 0;
		    BVAL(g1) = 0;
		}
		*++R = g1 + 1;
	    }
	    break;

	case BIT_REF:
	    /* evaluate the index */
	    eval_and_retain_flags(BIT_EXPR_CODE(t));
	    g2 = DECL_STORAGE(BIT_REF_DECL(t));
	    cond = ZERO;
	    g1 = *--R;
	    if (BVAL(g1)) {
		cond = X;
		goto condition_done;
	    }
	    {
		tree decl = BIT_REF_DECL(t);
		nbits_t decl_lsb = LSB(decl);
		nbits_t decl_msb = MSB(decl);
		Bit aval = AVAL(g1);
		nbits_t bit_offset;
		ngroups_t group_offset;

		if (VECTOR_DIRECTION_ATTR(decl)) {
		    if (aval < decl_lsb || aval > decl_msb) {
			cond = X;
			goto condition_done;
		    } else {
			bit_offset = (aval - decl_lsb) % BITS_IN_GROUP;
			group_offset = bits_to_groups(aval - decl_lsb + 1);
		    }
		} else {
		    if (aval < decl_msb || aval > decl_lsb) {
			cond = X;
			goto condition_done;
		    } else {
			bit_offset = (decl_lsb - aval) % BITS_IN_GROUP;
			group_offset = bits_to_groups(decl_lsb - aval + 1);
		    }
		}
		cond = ((AVAL(g2 + group_offset) &
			 ((Bit) 1 << bit_offset)) != 0) ? ONE : ZERO;
		if (BVAL(g2 + group_offset) & ((Bit) 1 << bit_offset)) {
		    if (cond) {
			cond = X;
		    } else {
			cond = Z;
		    }
		}
	    }
	    goto condition_done;

	case PART_REF:
	    {
		struct part_info *info = PART_INFO(t);
		R_nbits = TREE_NBITS(t);
		R_ngroups = bits_to_groups(R_nbits);
		R_mask = mask_right1[R_nbits & NBITS_MASK];
		g1 = *R;
		g2 = PART_STORAGE(t);
		part_rref(g1, g2, info);

		/* Fill high order with zeros */
		for (i = PART_NGROUPS(info) + 1; i <= R_ngroups; i++) {
		    AVAL(g1 + i) = 0;
		    BVAL(g1 + 1) = 0;
		}
		*++R = g1 + 1 + R_ngroups;
		break;
	    }

	case INTEGER_CST:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g = *R;
	    AVAL(g) = (unsigned_32_t) INT_CST_DATA(t);
	    BVAL(g) = 0;
	    if (R_ngroups > 0) {
		for (i = 1; i <= R_ngroups; i++) {
		    g++;
		    AVAL(g) = 0;
		    BVAL(g) = 0;
		}
	    }
	    *++R = g + 1;
	    break;

	case REAL_CST:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g = *R;
	    REAL_(g) = REAL_CST_DATA(t);
	    *++R = g + 1;
	    break;

	case BIT_CST:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g = *R;
	    g1 = BIT_CST_GROUP(t);
	    /* always copy one group */
	    AVAL(g) = AVAL(g1);
	    BVAL(g) = BVAL(g1);
	    {
		ngroups_t j = bits_to_groups(BIT_CST_NBITS(t));
		i = 1;
		/* If more than one group to copy, copy them all */
		if (j >= 1) {
		    for (i = 1; i <= j; i++) {
			g++;
			g1++;
			AVAL(g) = AVAL(g1);
			BVAL(g) = BVAL(g1);
		    }
		}
		/* If virtual register is larger than constant, zero fill */
		if (R_ngroups > j) {
		    for (; i <= R_ngroups; i++) {
			g++;
			AVAL(g) = 0;
			BVAL(g) = 0;
		    }
		}
	    }
	    *++R = g + 1;
	    break;

	case CONCAT_REF:
	case CONCAT_REP_REF:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g1 = *R;
	    g = *R;
	    ngroups = 0;
	    *++R = g2 = g1 + R_ngroups + 1;
	    for (i = 0; i <= R_ngroups; i++, g1++) {
		AVAL(g1) = 0;
		BVAL(g1) = 0;
	    }
	    g1 = g;
	    for (t1 = CONCAT_LIST(t); t1; t1 = TREE_CHAIN(t1)) {
		eval((tree *) TREE_PURPOSE(TREE_VALUE(t1)));
		R--;
		part_lref(g1, g2, (struct part_info *)
			  TREE_VALUE(TREE_VALUE(t1)));
		g1 += PART_NEXT((struct part_info *)
				TREE_VALUE(TREE_VALUE(t1)));
	    }
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    if (code == CONCAT_REP_REF) {
		ngroups = bits_to_groups(CONCAT_NBITS(t));
		g1 = g + ngroups + 1;
		for (i = ngroups + 1; i <= R_ngroups; i++, g1++) {
		    AVAL(g1) = BVAL(g1) = 0;
		}
		for (i = CONCAT_COUNT(t); i > 1; i--) {
		    copy_left(CONCAT_NBITS(t), i, g);
		}
	    }
	    break;

	case FUNCTION_REF:
	    {
		Group **R_save;
		Group **R_base_save;
		nbits_t R_nbits_save;
		ngroups_t R_ngroups_save;
		Bit R_mask_save;

		R_nbits = TREE_NBITS(t);
		R_ngroups = bits_to_groups(R_nbits);
		R_mask = mask_right1[R_nbits & NBITS_MASK];

		t2 = FUNCT_INPUT(FUNC_REF_FUNC(t));
		for (t1 = FUNC_REF_INASSIGN(t); t1;
		     t1 = TREE_CHAIN(t1), t2 = TREE_CHAIN(t2)) {
		    eval((tree *) TREE_PURPOSE(t1));
		    store(TREE_PURPOSE(t2), FUNC_REF_FUNC(t));
		}
		R_save = R;
		R_base_save = R_base;
		R_nbits_save = R_nbits;
		R_ngroups_save = R_ngroups;
		R_mask_save = R_mask;
		R_base = (union group **) xmalloc
		    (TREE_LABEL(FUNC_REF_FUNC(t)) *
		     sizeof(union group **));
		R = R_base;
		*R = (union group *)
		    xmalloc(FUNCT_AREA(FUNC_REF_FUNC(t))
				 * sizeof(Group));
		if (!R || !*R) {
		    printf_error_V("Out of memory at function call (%s)\n",
				   FUNC_REF_NAME(t));
		    shell_abort();
		}
		exec_(t);
		free(*R_base);
		free(R);
		R = R_save;
		R_base = R_base_save;
		R_nbits = R_nbits_save;
		R_ngroups = R_ngroups_save;
		R_mask = R_mask_save;
		R_nbits = TREE_NBITS(t);
		R_ngroups = bits_to_groups(R_nbits);
		R_mask = mask_right1[R_nbits & NBITS_MASK];
		eval_nbits(FUNCT_DECL(FUNC_REF_FUNC(t)), R_nbits);
	    }
	    break;

	case SYSFUNCTION_REF:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    exec_sysfunc(t, R_nbits);
	    break;


/***************************************************************************/
/* Bitwise operators: ~ & | ^ ~^                                           */
/***************************************************************************/

	case BIT_NOT_EXPR:
	    g1 = *--R;
	    AVAL(g1) = ~AVAL(g1) | BVAL(g1);	/* ~X,~Z -> X */
	    for (i = 1; i <= R_ngroups; i++) {
		g1++;
		AVAL(g1) = ~AVAL(g1) | BVAL(g1);	/* ~X,~Z -> X */
	    }
	    R++;
	    break;

	case BIT_AND_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    if (!BVAL(g1) && !BVAL(g2)) {
		AVAL(g1) &= AVAL(g2);
	    } else {
		tmp1 = AVAL(g1) | BVAL(g1);
		tmp2 = AVAL(g2) | BVAL(g2);
		AVAL(g1) = tmp1 & tmp2;	/* if aval is 1, could be 1 or X */
		BVAL(g1) = AVAL(g1) & (BVAL(g1) | BVAL(g2));
	    }
	    for (i = 1; i <= R_ngroups; i++) {
		g1++;
		g2++;
		if (!BVAL(g1) && !BVAL(g2)) {
		    AVAL(g1) &= AVAL(g2);
		} else {
		    tmp1 = AVAL(g1) | BVAL(g1);
		    tmp2 = AVAL(g2) | BVAL(g2);
		    AVAL(g1) = tmp1 & tmp2;	/* if aval is 1, could be 1 or X */
		    BVAL(g1) = AVAL(g1) & (BVAL(g1) | BVAL(g2));
		}
	    }
	    R++;
	    break;

	case BIT_OR_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    if (!BVAL(g1) && !BVAL(g2)) {
		AVAL(g1) |= AVAL(g2);
	    } else {		/* 1|z->x, 0|anything->0 */
		/* All must be zero */
		tmp1 = AVAL(g1);
		AVAL(g1) |= AVAL(g2) | BVAL(g1) | BVAL(g2);
		BVAL(g1) = (~tmp1 & BVAL(g2))
		    | (~AVAL(g2) & BVAL(g1)) | (BVAL(g1) & BVAL(g2));
	    }
	    for (i = 1; i <= R_ngroups; i++) {
		g1++;
		g2++;
		if (!BVAL(g1) && !BVAL(g2)) {
		    AVAL(g1) |= AVAL(g2);
		} else {
		    tmp1 = AVAL(g1);
		    AVAL(g1) |= AVAL(g2) | BVAL(g1) | BVAL(g2);
		    BVAL(g1) = (~tmp1 & BVAL(g2))
			| (~AVAL(g2) & BVAL(g1)) | (BVAL(g1) & BVAL(g2));
		}
	    }
	    R++;
	    break;

	case BIT_XOR_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    if (!BVAL(g1) && !BVAL(g2)) {
		AVAL(g1) ^= AVAL(g2);
	    } else {
		/* Z or Z XORed with anything is X */
		BVAL(g1) = BVAL(g1) | BVAL(g2);
		AVAL(g1) = (AVAL(g1) ^ AVAL(g2)) | BVAL(g1);
	    }
	    for (i = 1; i <= R_ngroups; i++) {
		g1++;
		g2++;
		if (!BVAL(g1) && !BVAL(g2)) {
		    AVAL(g1) ^= AVAL(g2);
		} else {
		    /* Z or Z XORed with anything is X */
		    BVAL(g1) = BVAL(g1) | BVAL(g2);
		    AVAL(g1) = (AVAL(g1) ^ AVAL(g2)) | BVAL(g1);
		}
	    }
	    R++;
	    break;

	case BIT_XNOR_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    if (!BVAL(g1) && !BVAL(g2)) {
		AVAL(g1) ^= ~AVAL(g2);
	    } else {
		/* Z or Z XORed with anything is X */
		BVAL(g1) = BVAL(g1) | BVAL(g2);
		AVAL(g1) = ~(AVAL(g1) ^ AVAL(g2)) | BVAL(g1);
	    }
	    for (i = 1; i <= R_ngroups; i++) {
		g1++;
		g2++;
		if (!BVAL(g1) && !BVAL(g2)) {
		    AVAL(g1) ^= ~AVAL(g2);
		} else {
		    /* Z or Z XORed with anything is X */
		    BVAL(g1) = BVAL(g1) | BVAL(g2);
		    AVAL(g1) = ~(AVAL(g1) ^ AVAL(g2)) | BVAL(g1);
		}
	    }
	    R++;
	    break;

/***************************************************************************/
/* Event operators: ANYEDGE, POSEDGE, NEGEDGE                              */
/***************************************************************************/

	case ANYEDGE_EXPR:
	    --R;
	    cond = ONE;
	    goto condition_done;

	case POSEDGE_EXPR:
	    --R;
	    cond = (enum logical_value) ((AVAL(*R) & 1) |
					 ((BVAL(*R) & 1) << 1));
	    cond1 =
		(enum logical_value) (unsigned long) TREE_OPERAND(t, 1);
	    tmpint = cond;
	    TREE_OPERAND(t, 1) = (tree) tmpint;

	    if ((cond == ONE || cond1 == ZERO) && cond1 != cond) {
		cond = ONE;
	    } else {
		cond = ZERO;
	    }
	    goto condition_done;

	case NEGEDGE_EXPR:
	    --R;
	    cond = (enum logical_value) ((AVAL(*R) & 1) |
					 ((BVAL(*R) & 1) << 1));
	    cond1 =
		(enum logical_value) (unsigned long) TREE_OPERAND(t, 1);
	    tmpint = cond;
	    TREE_OPERAND(t, 1) = (tree) tmpint;
	    if ((cond == ZERO || cond1 == ONE) && cond1 != cond) {
		cond = ONE;
	    } else {
		cond = ZERO;
	    }
	    goto condition_done;

/***************************************************************************/
/* Reduction operators: & ~& | ~| ^ ~^                                     */
/***************************************************************************/

	case AND_REDUCE_EXPR:
	    g1 = *--R;
	    cond = ONE;
	    for (i = 0; i < R_ngroups; i++, g1++) {
		/* if there are any zeros, we are done */
		if (~AVAL(g1) & ~BVAL(g1)) {
		    cond = ZERO;
		    break;
		    /* (X or Z) and 1 -> X */
		} else if (BVAL(g1)) {
		    cond = X;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (~AVAL(g1) & ~BVAL(g1) & R_mask) {
		cond = ZERO;
		/* (X or Z) and 1 -> X */
	    } else if (BVAL(g1) & R_mask) {
		cond = X;
	    }
	    goto condition_done;

	case NAND_REDUCE_EXPR:
	    g1 = *--R;
	    cond = ZERO;
	    for (i = 0; i < R_ngroups; i++, g1++) {
		/* if there are any zeros, we are done */
		if (~AVAL(g1) & ~BVAL(g1)) {
		    cond = ONE;
		    break;
		    /* (X or Z) and 1 -> X */
		} else if (BVAL(g1)) {
		    cond = X;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (~AVAL(g1) & ~BVAL(g1) & R_mask) {
		cond = ONE;
		/* (X or Z) and 1 -> X */
	    } else if (BVAL(g1) & R_mask) {
		cond = X;
	    }
	    goto condition_done;

	case OR_REDUCE_EXPR:
	    g1 = *--R;
	    cond = ZERO;
	    for (i = 0; i < R_ngroups; i++, g1++) {
		/* if there are any ones, we are done */
		if (AVAL(g1) & ~BVAL(g1)) {
		    cond = ONE;
		    break;
		    /* (X or Z) and 1 -> X */
		} else if (BVAL(g1)) {
		    cond = X;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (AVAL(g1) & ~BVAL(g1) & R_mask) {
		cond = ONE;
		/* (X or Z) and 1 -> X */
	    } else if (BVAL(g1) & R_mask) {
		cond = X;
	    }
	    goto condition_done;

	case NOR_REDUCE_EXPR:
	    g1 = *--R;
	    cond = ONE;
	    for (i = 0; i < R_ngroups; i++, g1++) {
		/* if there are any ones, we are done */
		if (AVAL(g1) & ~BVAL(g1)) {
		    cond = ZERO;
		    break;
		    /* (X or Z) and 1 -> X */
		} else if (BVAL(g1)) {
		    cond = X;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (AVAL(g1) & ~BVAL(g1) & R_mask) {
		cond = ZERO;
		/* (X or Z) and 1 -> X */
	    } else if (BVAL(g1) & R_mask) {
		cond = X;
	    }
	    goto condition_done;

	case TRUTH_NOT_EXPR:
	    DETERMINE_TRUE_COND(g1);
	    if (cond == ONE) {
		cond = ZERO;
	    } else if (cond == ZERO) {
		cond = ONE;
	    }
	    goto condition_done;

	case XOR_REDUCE_EXPR:
	    cond = ZERO;
	    goto xor_common;

	case XNOR_REDUCE_EXPR:
	    cond = ONE;
	  xor_common:
	    g1 = *--R;
	    for (i = 0; i < R_ngroups; i++, g1++) {
		if (BVAL(g1)) {
		    cond = X;
		    break;
		} else if (xor_reduce(AVAL(g1), BITS_IN_GROUP)) {
		    if (cond == ZERO) {
			cond = ONE;
		    } else {
			cond = ZERO;
		    }
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (cond != X) {
		if (BVAL(g1) & R_mask) {
		    cond = X;
		} else if (xor_reduce(AVAL(g1) & R_mask,
				      (R_nbits - 1) % BITS_IN_GROUP + 1)) {
		    if (cond == ZERO) {
			cond = ONE;
		    } else {
			cond = ZERO;
		    }
		}
	    }
	    goto condition_done;

/***************************************************************************/
/* Logical operators: ! && ||                                              */
/***************************************************************************/

/*	case TRUTH_NOT_EXPR:  (see NOR_REDUCE_EXPR)  */
	case TRUTH_ORIF_EXPR:
	    DETERMINE_TRUE_COND(g1);
	    /* if left operand is one, then the right 
	       operand is not evaluated */
	    if (cond != ONE) {
		cond1 = cond;
		TEST(TRUTH_EXPR_CODE(t), g2);
		if (cond == ZERO) {
		    /* if right operand is zero, pass the left op */
		    cond = cond1;
		}
	    }
	    goto condition_done;

	case TRUTH_ANDIF_EXPR:
	    DETERMINE_TRUE_COND(g1);
	    /* if left operand is zero, then the right 
	       operand is not evaluated */
	    if (cond != ZERO) {
		cond1 = cond;
		TEST(TRUTH_EXPR_CODE(t), g2);
		if (cond == ONE) {
		    /* if right operand is one, pass the left op */
		    cond = cond1;
		}
	    }
	    goto condition_done;

/***************************************************************************/
/* Equality operators: == != === !==                                       */
/***************************************************************************/

	case EQ_EXPR:
	    if (is_real) {
		g2 = *--R;
		g1 = *--R;
		AVAL(g1) = (REAL_(g1) == REAL_(g2));
		BVAL(g1) = 0;
		R++;
		break;
	    }
	    g2 = *--R;
	    g1 = *--R;

	    cond = ONE;
	    for (i = 0; i < R_ngroups; i++, g1++, g2++) {
		if (BVAL(g1) || BVAL(g2)) {
		    cond = X;
		} else if (AVAL(g1) != AVAL(g2)) {
		    cond = ZERO;
		    break;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (cond == ONE) {
		if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		    cond = X;
		} else if ((AVAL(g1) ^ AVAL(g2)) & R_mask) {
		    cond = ZERO;
		}
	    }
	    goto condition_done;

	case NE_EXPR:
	    if (is_real) {
		g2 = *--R;
		g1 = *--R;
		AVAL(g1) = (REAL_(g1) != REAL_(g2));
		BVAL(g1) = 0;
		R++;
		break;
	    }
	    {
		/*
		 * x's do not automatically result
		 * in a x result if non-x value prove
		 * inequality
		 */
		int hasX = FALSE;
		g2 = *--R;
		g1 = *--R;

		cond = ZERO;
		for (i = 0; i < R_ngroups; i++, g1++, g2++) {
		    if (BVAL(g1) || BVAL(g2)) {
			hasX = 1;
		    }
		    if ((AVAL(g1) & ~BVAL(g1)) != (AVAL(g2) & ~BVAL(g2))) {
			cond = ONE;
			break;
		    }
		}
		/* last group (most significant) need to be masked */
		if (cond == ZERO) {
		    if ((BVAL(g1) | BVAL(g2)) & R_mask) {
			hasX = X;
		    } else if (((AVAL(g1) & ~BVAL(g1)) ^
				(AVAL(g2) & ~BVAL(g2))) & R_mask) {
			cond = ONE;
		    }
		}
		if (cond == ZERO && hasX) {
		    cond = X;
		}

		goto condition_done;
	    }

	case EQ_CASE_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    cond = ONE;
	    for (i = 0; i < R_ngroups; i++, g1++, g2++) {
		if ((AVAL(g1) != AVAL(g2)) || (BVAL(g1) != BVAL(g2))) {
		    cond = ZERO;
		    break;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (cond == ONE) {
		if (((AVAL(g1) ^ AVAL(g2)) |
		     (BVAL(g1) ^ BVAL(g2))) & R_mask) {
		    cond = ZERO;
		}
	    }
	    goto condition_done;

	case NE_CASE_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    cond = ZERO;
	    for (i = 0; i < R_ngroups; i++, g1++, g2++) {
		if ((AVAL(g1) != AVAL(g2)) || (BVAL(g1) != BVAL(g2))) {
		    cond = ONE;
		    break;
		}
	    }
	    /* last group (most significant) need to be masked */
	    if (cond == ZERO) {
		if (((AVAL(g1) ^ AVAL(g2)) |
		     (BVAL(g1) ^ BVAL(g2))) & R_mask) {
		    cond = ONE;
		}
	    }
	    goto condition_done;

/***************************************************************************/
/* Relational operators: < > <= >=                                         */
/***************************************************************************/

	case LT_EXPR:
	    if (is_integer) {
		g2 = *--R;
		g1 = *--R;
		if (BVAL(g1) || BVAL(g2)) {
		    AVAL(g1) = (Bit) - 1;
		    BVAL(g1) = (Bit) - 1;
		} else {
		    AVAL(g1) = (signed_32_t) AVAL(g1) <
			(signed_32_t) AVAL(g2);
		    BVAL(g1) = 0;
		}
		R++;
		break;
	    } else if (is_real) {
		g2 = *--R;
		g1 = *--R;
		AVAL(g1) = (REAL_(g1) < REAL_(g2));
		BVAL(g1) = 0;
		R++;
		break;
	    }

	    /* evaluate from left-to-right */
	    g2 = (*R--) - 1;
	    g1 = (*R--) - 1;
	    /* first group (most significant) need to be masked */
	    if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		cond = X;
		goto condition_done;
	    } else if ((tmp1 = (AVAL(g1) & R_mask))
		       > (tmp2 = AVAL(g2) & R_mask)) {
		cond = ZERO;
		goto condition_done;
	    } else if (tmp1 < tmp2) {
		cond = ONE;
		goto condition_done;
	    }

	    /* keep doing if equal */
	    for (i = 1, cond = ZERO; i <= R_ngroups; i++) {
		g1--;
		g2--;
		if (BVAL(g1) || BVAL(g2)) {
		    cond = X;
		    break;
		} else if ((tmp1 = AVAL(g1)) > (tmp2 = AVAL(g2))) {
		    cond = ZERO;
		    break;
		} else if (tmp1 < tmp2) {
		    cond = ONE;
		    break;
		}
	    }
	    goto condition_done;

	case LE_EXPR:
	    if (is_integer) {
		g2 = *--R;
		g1 = *--R;
		if (BVAL(g1) || BVAL(g2)) {
		    AVAL(g1) = (Bit) - 1;
		    BVAL(g1) = (Bit) - 1;
		} else {
		    AVAL(g1) = (signed_32_t) AVAL(g1) <=
			(signed_32_t) AVAL(g2);
		    BVAL(g1) = 0;
		}
		R++;
		break;
	    } else if (is_real) {
		g2 = *--R;
		g1 = *--R;
		AVAL(g1) = (REAL_(g1) <= REAL_(g2));
		BVAL(g1) = 0;
		R++;
		break;
	    }

	    /* evaluate from left-to-right */
	    g2 = (*R--) - 1;
	    g1 = (*R--) - 1;
	    /* first group (most significant) need to be masked */
	    if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		cond = X;
		goto condition_done;
	    } else if ((tmp1 = (AVAL(g1) & R_mask))
		       > (tmp2 = AVAL(g2) & R_mask)) {
		cond = ZERO;
		goto condition_done;
	    } else if (tmp1 < tmp2) {
		cond = ONE;
		goto condition_done;
	    }

	    /* keep doing if equal */
	    for (i = 1, cond = ONE; i <= R_ngroups; i++) {
		g1--;
		g2--;
		if (BVAL(g1) || BVAL(g2)) {
		    cond = X;
		    break;
		} else if ((tmp1 = AVAL(g1)) > (tmp2 = AVAL(g2))) {
		    cond = ZERO;
		    break;
		} else if (tmp1 < tmp2) {
		    cond = ONE;
		    break;
		}
	    }
	    goto condition_done;

	case GT_EXPR:
	    if (is_integer) {
		g2 = *--R;
		g1 = *--R;
		if (BVAL(g1) || BVAL(g2)) {
		    AVAL(g1) = (Bit) - 1;
		    BVAL(g1) = (Bit) - 1;
		} else {
		    AVAL(g1) = (signed_32_t) AVAL(g1) >
			(signed_32_t) AVAL(g2);
		    BVAL(g1) = 0;
		}
		R++;
		break;
	    } else if (is_real) {
		g2 = *--R;
		g1 = *--R;
		AVAL(g1) = (REAL_(g1) > REAL_(g2));
		BVAL(g1) = 0;
		R++;
		break;
	    }

	    /* evaluate from left-to-right */
	    g2 = (*R--) - 1;
	    g1 = (*R--) - 1;
	    /* first group (most significant) need to be masked */
	    if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		cond = X;
		goto condition_done;
	    } else if ((tmp1 = (AVAL(g1) & R_mask))
		       < (tmp2 = AVAL(g2) & R_mask)) {
		cond = ZERO;
		goto condition_done;
	    } else if (tmp1 > tmp2) {
		cond = ONE;
		goto condition_done;
	    }

	    /* keep doing if equal */
	    for (i = 1, cond = ZERO; i <= R_ngroups; i++) {
		g1--;
		g2--;
		if (BVAL(g1) || BVAL(g2)) {
		    cond = X;
		    break;
		} else if ((tmp1 = AVAL(g1)) < (tmp2 = AVAL(g2))) {
		    cond = ZERO;
		    break;
		} else if (tmp1 > tmp2) {
		    cond = ONE;
		    break;
		}
	    }
	    goto condition_done;

	case GE_EXPR:
	    if (is_integer) {
		g2 = *--R;
		g1 = *--R;
		if (BVAL(g1) || BVAL(g2)) {
		    AVAL(g1) = (Bit) - 1;
		    BVAL(g1) = (Bit) - 1;
		} else {
		    AVAL(g1) = (signed_32_t) AVAL(g1) >=
			(signed_32_t) AVAL(g2);
		    BVAL(g1) = 0;
		}
		R++;
		break;
	    } else if (is_real) {
		g2 = *--R;
		g1 = *--R;
		AVAL(g1) = (REAL_(g1) >= REAL_(g2));
		BVAL(g1) = 0;
		R++;
		break;
	    }

	    /* evaluate from left-to-right */
	    g2 = (*R--) - 1;
	    g1 = (*R--) - 1;
	    /* first group (most significant) need to be masked */
	    if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		cond = X;
		goto condition_done;
	    } else if ((tmp1 = (AVAL(g1) & R_mask))
		       < (tmp2 = AVAL(g2) & R_mask)) {
		cond = ZERO;
		goto condition_done;
	    } else if (tmp1 > tmp2) {
		cond = ONE;
		goto condition_done;
	    }

	    /* keep doing if equal */
	    for (i = 1, cond = ONE; i <= R_ngroups; i++) {
		g1--;
		g2--;
		if (BVAL(g1) || BVAL(g2)) {
		    cond = X;
		    break;
		} else if ((tmp1 = AVAL(g1)) < (tmp2 = AVAL(g2))) {
		    cond = ZERO;
		    break;
		} else if (tmp1 > tmp2) {
		    cond = ONE;
		    break;
		}
	    }
	    goto condition_done;


	  condition_done:
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    g1 = *R;
	    if (is_real) {
		REAL_(g1) = (cond == ONE);
	    } else {
		AVAL(g1) = (cond == ZERO || cond == Z) ? 0 : 1;
		BVAL(g1) = (cond == X || cond == Z) ? 1 : 0;
	    }
	    if (R_ngroups > 0) {
		for (i = 1; i <= R_ngroups; i++) {
		    g1++;
		    AVAL(g1) = 0;
		    BVAL(g1) = 0;
		}
	    }
	    *++R = g1 + 1;
	    break;

/***************************************************************************/
/* Arithmetic operators: + - * / %                                         */
/***************************************************************************/

	case PLUS_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    {
		register unsigned int carry;
		if (is_real) {
		    REAL_(g1) += REAL_(g2);
		    R++;
		    break;
		}
		cond = ZERO;
		carry = 0;

		for (i = 0; i < R_ngroups; i++, g1++, g2++) {
		    AVAL(g1) += carry;
		    carry = !AVAL(g1) && carry;	/* if wrap around to 0 */
		    carry |= AVAL(g1) > ~AVAL(g2);
		    AVAL(g1) += AVAL(g2);
		    /* If there are any Xs of Zs, make the result unknown */
		    if (BVAL(g1) | BVAL(g2)) {
			cond = X;
		    }
		}

		/* Don't need to prop caary on last group */
		AVAL(g1) += AVAL(g2) + carry;
		if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		    cond = X;
		}

		if (cond == X) {
		    load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
		}
	    }
	    R++;
	    break;

	case MINUS_EXPR:
	    g2 = *--R;
	    g1 = *--R;
	    {
		register unsigned int carry;

		if (is_real) {
		    REAL_(g1) -= REAL_(g2);
		    R++;
		    break;
		}
		cond = ZERO;
		carry = 1;

		for (i = 0; i < R_ngroups; i++, g1++, g2++) {
		    AVAL(g1) += carry;
		    carry = !AVAL(g1) && carry;	/* if wrap around to 0 */
		    carry |= AVAL(g1) > AVAL(g2);
		    AVAL(g1) += ~AVAL(g2);
		    /* If there are any Xs of Zs, make the result unknown */
		    if (BVAL(g1) | BVAL(g2)) {
			cond = X;
		    }
		}

		/* Don't need to prop caary on last group */
		AVAL(g1) += ~AVAL(g2) + carry;
		if ((BVAL(g1) | BVAL(g2)) & R_mask) {
		    cond = X;
		}
		if (cond == X) {
		    load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
		}
	    }
	    R++;
	    break;


#define HIGH16(ptr) (((struct split_32 *)&(ptr))->high)
#define LOW16(ptr) (((struct split_32 *)&(ptr))->low)

	case MULT_EXPR:
	    g2 = *--R;
	    g1 = *--R;


	    if (!R_ngroups) {
		if (is_real) {
		    REAL_(g1) *= REAL_(g2);
		    R++;
		    break;
		}

		if (BVAL(g1) || BVAL(g2)) {
		    AVAL(g1) = (Bit) - 1;
		    BVAL(g1) = (Bit) - 1;
		} else {
		    AVAL(g1) *= AVAL(g2);
		}
		R++;
		break;
	    }

	    cond = ZERO;
	    for (i = 0; i <= R_ngroups; i++) {
		/* If there are any Xs of Zs, make the result unknown */
		if (BVAL(g1 + i) | BVAL(g2 + i)) {
		    cond = X;
		}
	    }
	    if (cond == X) {
		load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
	    } else {
		GroupMult(g1, g1, g2, R_ngroups + 1);
	    }
	    R++;
	    break;

	case DIV_EXPR:
	    {
		int zeroDivisor = 0;
		g2 = *--R;
		g1 = *--R;
		/*
		 * check for zero divisor
		 */
		if (!is_real) {
		    zeroDivisor = 1;
		    for (i = 0; i <= R_ngroups; i++) {
			if (AVAL(g2 + i)) {
			    zeroDivisor = 0;
			}
		    }
		}

		cond = ZERO;
		for (i = 0; i <= R_ngroups; i++) {
		    /* If there are any Xs of Zs, make the result unknown */
		    if (BVAL(g1 + i) | BVAL(g2 + i)) {
			cond = X;
		    }
		}

		if (!is_real && zeroDivisor) {
		    load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
		} else if (is_integer) {
		    AVAL(g1) = (signed) AVAL(g1) / (signed) AVAL(g2);
		} else if (is_real) {
		    REAL_(g1) /= REAL_(g2);
		} else if (!R_ngroups) {
		    AVAL(g1) /= AVAL(g2);
		} else {
		    GroupDiv(g1, g1, g1, g2, R_ngroups + 1, 0);
		}

		if (cond == X && !is_real) {
		    load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
		}
		R++;
	    }
	    break;

	case MOD_EXPR:
	    {
		int zeroDivisor = 0;
		g2 = *--R;
		g1 = *--R;
		/*
		 * check for zero divisor
		 */
		if (!is_real) {
		    zeroDivisor = 1;
		    for (i = 0; i <= R_ngroups; i++) {
			if (AVAL(g2 + i)) {
			    zeroDivisor = 0;
			}
		    }
		}

		cond = ZERO;
		for (i = 0; i <= R_ngroups; i++) {
		    /* If there are any Xs of Zs, make the result unknown */
		    if (BVAL(g1 + i) | BVAL(g2 + i)) {
			cond = X;
		    }
		}

		if (!is_real && zeroDivisor) {
		    load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
		} else if (is_integer) {
		    AVAL(g1) = (signed) AVAL(g1) % (signed) AVAL(g2);
		} else if (!R_ngroups) {
		    AVAL(g1) %= AVAL(g2);
		} else {
		    GroupDiv(g1, g1, g1, g2, R_ngroups + 1, 1);
		}
		if (cond == X && !is_real) {
		    load_fill(*R, R_ngroups, (Bit) - 1, (Bit) - 1);
		}
		R++;
	    }
	    break;

/***************************************************************************/
/* Unary Arithmetic operators: -                                           */
/***************************************************************************/

	case NEGATE_EXPR:
	    g1 = *--R;
	    if (is_real) {
		REAL_(g1) = -REAL_(g1);
		R++;
		break;
	    }
	    if (BVAL(g1)) {
		goto fill_x;
	    }
	    if (!R_ngroups) {
		AVAL(g1) = (~AVAL(g1) + 1) & R_mask;
	    } else {
		tmp1 = AVAL(g1) = ~AVAL(g1) + 1;
		for (i = 1, g1++; i < R_ngroups; i++, g1++) {
		    if (BVAL(g1)) {
			goto fill_x;
		    }
		    tmp1 = AVAL(g1) = ~AVAL(g1) + (tmp1 == 0);
		}
		tmp1 = AVAL(g1) = (~AVAL(g1) + (tmp1 == 0)) & R_mask;
	    }
	    R++;
	    break;

/***************************************************************************/
/* Unary real-to-integer conversion                                        */
/***************************************************************************/

	case INT_CONV_EXPR:
	    g1 = *--R;
	    real_to_reg(REAL_(g1), g1, R_ngroups);
	    is_real = 0;
	    R++;
	    break;

/***************************************************************************/
/* Unary integer-to-real conversion                                        */
/***************************************************************************/

	case REAL_CONV_EXPR:
	    g1 = *--R;
	    REAL_(g1) = reg_to_real(g1, R_nbits);
	    is_real = 1;
	    is_integer = 0;
	    R++;
	    break;

/***************************************************************************/
/* Shift operators: << >>                                                  */
/***************************************************************************/

	case LSHIFT_EXPR:
	    /* evaluate from left-to-right */
	    g = *--R;		/* shift count */
	    g1 = (*R--) - 1;	/* high end of stuff to be shifted */
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    shift_bits = AVAL(g);
	    if (shift_bits < 0) {
		shift_bits = -shift_bits;
		goto right_shift_instead;
	    }
	  left_shift_instead:
	    shift_groups = bits_to_groups(shift_bits + 1);
	    shift_bits %= BITS_IN_GROUP;
	    if (shift_groups <= 0) {
		shift_groups = 0;
	    }

	    /* make sure shift count is ok */
	    if (BVAL(g)) {
		goto fill_x;
	    }
	    g2 = g1 - shift_groups;

	    if (shift_groups <= R_ngroups) {
		AVAL(g1) = (AVAL(g2) << shift_bits);	// | tmp2;
		BVAL(g1) = (BVAL(g2) << shift_bits);	// | tmp2b;
		for (i = 0; i < R_ngroups - shift_groups; i++) {
		    g2--;
		    if (shift_bits) {
			AVAL(g1) |=
			    AVAL(g2) >> (BITS_IN_GROUP - shift_bits);
			BVAL(g1) |=
			    BVAL(g2) >> (BITS_IN_GROUP - shift_bits);
		    }
		    g1--;
		    AVAL(g1) = (AVAL(g2) << shift_bits);	// | tmp2;
		    BVAL(g1) = (BVAL(g2) << shift_bits);	// | tmp2b;

		}
		g1--;
	    }
	    for (i = 0; i < shift_groups; i++, g1--) {
		AVAL(g1) = 0;
		BVAL(g1) = 0;
		tmp2 = 0;
	    }
	    R++;
	    break;

	case RSHIFT_EXPR:
	    /* evaluate from right-to-left */
	    g = *--R;		/* shift count */
	    g1 = *--R;		/* stuff to be shifted */
	    R_nbits = TREE_NBITS(t);
	    R_ngroups = bits_to_groups(R_nbits);
	    R_mask = mask_right1[R_nbits & NBITS_MASK];
	    shift_bits = AVAL(g);
	    if (shift_bits < 0) {
		shift_bits = -shift_bits;
		goto left_shift_instead;
	    }
	  right_shift_instead:
	    shift_groups = bits_to_groups(shift_bits + 1);
	    shift_bits %= BITS_IN_GROUP;
	    if (shift_groups <= 0) {
		shift_groups = 0;
	    }

	    /* make sure shift count is ok */
	    if (BVAL(g)) {
		goto fill_x;
	    }
	    g2 = g1 + shift_groups;
	    for (i = 0; i < R_ngroups - shift_groups; i++, g1++, g2++) {
		if (shift_bits) {
		    AVAL(g1) = (AVAL(g2) >> shift_bits) |
			(AVAL(g2 + 1) << (BITS_IN_GROUP - shift_bits));
		    BVAL(g1) = (BVAL(g2) >> shift_bits) |
			(BVAL(g2 + 1) << (BITS_IN_GROUP - shift_bits));
		} else {
		    AVAL(g1) = AVAL(g2);
		    BVAL(g1) = BVAL(g2);
		}
	    }
	    AVAL(g1) = (AVAL(g2) & R_mask) >> shift_bits;
	    BVAL(g1) = (BVAL(g2) & R_mask) >> shift_bits;
	    for (i = 0; i < shift_groups; i++) {
		g1++;
		AVAL(g1) = 0;
		BVAL(g1) = 0;
		tmp2 = 0;
	    }
	    R++;
	    break;

	  fill_x:
	    tmp1 = (Bit) - 1;
	    tmp2 = (Bit) - 1;
	    g = *R;
	    for (i = 0; i <= R_ngroups; i++, g++) {
		AVAL(g) = tmp1;
		BVAL(g) = tmp2;
	    }
	    R++;
	    break;

/***************************************************************************/
/* Conditional operator: ?:                                                */
/***************************************************************************/

	case COND_EXPR:
	    eval(COND_EXPR_CODE(t));
	    g1 = *--R;
	    cond = ZERO;
	    for (i = 0; i < R_ngroups; i++, g1++) {
		if (BVAL(g1)) {
		    cond = X;
		    goto cond_continue;
		} else if (AVAL(g1)) {
		    cond = ONE;
		    goto cond_continue;
		}
	    }

	    if (BVAL(g1) & R_mask) {
		cond = X;
		/* R_mask is set by 1st call to eval */
	    } else if (AVAL(g1) & R_mask) {
		cond = ONE;
	    }
	  cond_continue:
	    in_cond = ONE;	/* hack to preserve integer bit */
	    if (cond == ONE) {
		eval(COND_TRUE_CODE(t));
	    } else if (cond == ZERO) {
		eval(COND_FALSE_CODE(t));
	    } else {
		eval(COND_TRUE_CODE(t));
		eval(COND_FALSE_CODE(t));
		g2 = *--R;
		g1 = *(R - 1);
		for (i = 0; i <= R_ngroups; i++, g1++, g2++) {
		    tmp1 = AVAL(g1);
		    AVAL(g1) |= AVAL(g2) | BVAL(g1) | BVAL(g2);
		    BVAL(g1) |= BVAL(g2) | (tmp1 ^ AVAL(g2));
		}
	    }
	    in_cond = 0;
	    break;

	default:
	    printf_error_V("Unimplemented expression: %s\n",
			   tree_code_name[TREE_CODE(t)]);
	    shell_abort();
	    break;

	}			/* switch */
	pc++;
    }				/* while */
    return;
}


delay_t eval_delay(tree delay_node, enum logical_value state)
{
    delay_t delay;

    if (!delay_node) {
	return 0;
    }

    /* TREE_LABEL has the number of delays given: 1, 2 or 3 */
    if (TREE_LABEL(delay_node) == 1) {
	eval((tree *) TREE_OPERAND(delay_node, 3));
	ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	delay = AVAL(*--R);
	/* Other cases assume two or tree delays given */
    } else if (state == ONE) {
	eval((tree *) TREE_OPERAND(delay_node, 3));
	ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	delay = AVAL(*--R);
    } else if (state == ZERO) {
	eval((tree *) TREE_OPERAND(delay_node, 4));
	ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	delay = AVAL(*--R);
    } else if (state == Z)
	if (TREE_LABEL(delay_node) == 3) {
	    eval((tree *) TREE_OPERAND(delay_node, 5));
	    ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	    delay = AVAL(*--R);
	} else {		/* 2 values given, get the min */
	    eval((tree *) TREE_OPERAND(delay_node, 3));
	    ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	    delay = AVAL(*--R);
	    eval((tree *) TREE_OPERAND(delay_node, 4));
	    ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	    if (AVAL(*--R) < delay) {
		delay = AVAL(*R);
	    }
	    if (BVAL(*R)) {
		return 0;
	    }
	    /* if X, find MIN of delays */
    } else {
	eval((tree *) TREE_OPERAND(delay_node, 3));
	ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	delay = AVAL(*--R);
	if (BVAL(*R)) {
	    return 0;
	}
	eval((tree *) TREE_OPERAND(delay_node, 4));
	ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	if (AVAL(*--R) < delay) {
	    delay = AVAL(*R);
	}
	if (BVAL(*R)) {
	    return 0;
	}
	if (TREE_LABEL(delay_node) == 3) {
	    eval((tree *) TREE_OPERAND(delay_node, 5));
	    ASSERT(R_ngroups == 0 || AVAL(*(R - 1) + 1) == 0);
	    if (AVAL(*--R) < delay) {
		delay = AVAL(*R);
	    }
	    if (BVAL(*R)) {
		return 0;
	    }
	}
    }
    if (BVAL(*R)) {
	return 0;
    }
    return delay;
}

/* Evaluate nets; if exclude_inout is set, don't use the inout port
     (if there is one) in the evaluation.  This prevents a lockup
     situation in which a net that has drivers on both sides of
     an inout port will lock to X since that X is "driving" both sides
     of the port. */
/* exclude_tree is used to exclude a node from the net calculation for 
	bi-directional gates */


void do_net_eval(tree node, int exclude_inout, tree exclude_tree)
{
    tree t;
    ngroups_t i;
    Group *g1;
    Group *g2;
    Bit z1;
    Bit z2;
    Bit aval1;
    Bit aval2;
    Bit bval1;
    Bit bval2;
    enum tree_type type = (enum tree_type) TREE_TYPE(node);

    R_nbits = TREE_NBITS(node);
    R_ngroups = bits_to_groups(R_nbits);
    R_mask = mask_right1[R_nbits & NBITS_MASK];
    g1 = *R;
    /* First, initialize net to Z */
    for (i = 0; i <= R_ngroups; i++, g1++) {
	AVAL(g1) = 0;
	BVAL(g1) = (unsigned int) -1;
    }

    switch (type) {
	/* Z combined with anything is the anything, 1 combined with 0 is X */
    case NET_WIRE_TYPE:
    case NET_TRI_TYPE:
	for (t = NET_SOURCE(node); t; t = NET_SOURCE(t)) {
	    if (exclude_tree == t) {
		continue;
	    }
	    if (exclude_inout) {
		if (PORT_OUTPUT_ATTR(t) || PORT_COLLAPSED_ATTR(t)) {
		    continue;
		}
	    }
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    for (i = 0; i <= R_ngroups; i++, g1++, g2++) {
		aval1 = AVAL(g1);
		aval2 = AVAL(g2);
		bval1 = BVAL(g1);
		bval2 = BVAL(g2);
		z1 = ~aval1 & bval1;	/* where the Z's are */
		z2 = ~aval2 & bval2;
		BVAL(g1) = ~((z1 & ~bval2) | (z2 & ~bval1) |
			     (~bval1 & ~bval2 & ~(aval1 ^ aval2)));
		AVAL(g1) = aval1 | aval2;
	    }
	}
	break;

	/* These are the same as WIRE/TRI, exept replace Z by 0/1 after eval */
    case NET_TRI0_TYPE:
    case NET_TRI1_TYPE:
	/* Don't eval last source; it's there for initialization only */
	for (t = NET_SOURCE(node); NET_SOURCE(t); t = NET_SOURCE(t)) {
	    if (exclude_tree == t) {
		continue;

	    }
	    if (exclude_inout) {
		if (PORT_OUTPUT_ATTR(t) || PORT_COLLAPSED_ATTR(t)) {
		    continue;
		}
	    }
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    for (i = 0; i <= R_ngroups; i++, g1++, g2++) {
		aval1 = AVAL(g1);
		aval2 = AVAL(g2);
		bval1 = BVAL(g1);
		bval2 = BVAL(g2);
		z1 = ~aval1 & bval1;	/* where the Z's are */
		z2 = ~aval2 & bval2;
		BVAL(g1) = ~((z1 & ~bval2) | (z2 & ~bval1) |
			     (~bval1 & ~bval2 & ~(aval1 ^ aval2)));
		AVAL(g1) = aval1 | aval2;
	    }
	}

	/* Now, replace all Zs by 0 (for TRI0) or 1 (for TRI1) */
	if (type == NET_TRI0_TYPE) {
	    g1 = *R;
	    for (i = 0; i <= R_ngroups; i++, g1++) {
		BVAL(g1) = AVAL(g1) & BVAL(g1);	/* stay set only if X */
	    }
	} else {
	    g1 = *R;
	    for (i = 0; i <= R_ngroups; i++, g1++) {
		aval1 = AVAL(g1);
		bval1 = BVAL(g1);
		BVAL(g1) = aval1 & bval1;
		AVAL(g1) = aval1 | bval1;
	    }
	}
	break;

    case NET_SUPPLY1_TYPE:
	g1 = *R;
	AVAL(g1) = 1;
	BVAL(g1) = 0;
	for (i = 1, g1++; i <= R_ngroups; i++, g1++) {
	    AVAL(g1) = 0;
	    BVAL(g1) = 0;
	}
	break;


    case NET_SUPPLY0_TYPE:
	g1 = *R;
	AVAL(g1) = 0;
	BVAL(g1) = 0;
	for (i = 1, g1++; i <= R_ngroups; i++, g1++) {
	    AVAL(g1) = 0;
	    BVAL(g1) = 0;
	}
	break;

	/* 1 combined with 0 yields 0, Z combined with anything is still anything */
    case NET_WAND_TYPE:
    case NET_TRIAND_TYPE:
	for (t = NET_SOURCE(node); t; t = NET_SOURCE(t)) {
	    if (exclude_tree == t) {
		continue;
	    }

	    if (exclude_inout) {
		if (PORT_OUTPUT_ATTR(t) || PORT_COLLAPSED_ATTR(t)) {
		    continue;
		}
	    }
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    for (i = 0; i <= R_ngroups; i++, g1++, g2++) {
		aval1 = AVAL(g1);
		aval2 = AVAL(g2);
		bval1 = BVAL(g1);
		bval2 = BVAL(g2);
		AVAL(g1) =
		    (aval1 & aval2) | (aval1 & bval2) | (aval2 & bval1);
		BVAL(g1) = (bval1 & bval2) | (aval1 & bval1 & aval2)
		    | (aval2 & bval2 & aval1);
	    }
	}
	break;

	/* 1 combined with 0 yields 1, Z combined with anything is still anything */
    case NET_WOR_TYPE:
    case NET_TRIOR_TYPE:
	for (t = NET_SOURCE(node); t; t = NET_SOURCE(t)) {
	    if (exclude_tree == t) {
		continue;
	    }
	    if (exclude_inout) {
		if (PORT_OUTPUT_ATTR(t) || PORT_COLLAPSED_ATTR(t)) {
		    continue;
		}
	    }
	    g1 = *R;
	    g2 = DECL_STORAGE(t);
	    for (i = 0; i <= R_ngroups; i++, g1++, g2++) {
		aval1 = AVAL(g1);
		aval2 = AVAL(g2);
		bval1 = BVAL(g1);
		bval2 = BVAL(g2);
		BVAL(g1) = (bval1 & bval2) | (aval1 & bval1) &
		    (~aval2 & ~bval2) | (~aval1 & ~bval1 & aval2 & bval2);
		AVAL(g1) = aval1 | aval2;
	    }
	}
	break;

    default:
	fatal("Eval of unknown net type", NULL);
    }				/* end switch (type) */
    *++R = g1;			// + 1;
}

/* Allocate virtual registers */

int R_alloc(int number, int area)
{
    number++;			/* account for the zeroth register */
    number++;			/* account for the zeroth register */
    number++;			/* account for the zeroth register */
    number++;			/* account for the zeroth register */
    if (number > current_regs) {
	if (current_regs == 0) {
	    R_base = (union group **) xmalloc
		(number * sizeof(union group **));
	} else {
	    R_base = (union group **) xrealloc
		((char*)R, number * sizeof(union group **));
	}
	current_regs = number;
    }
    if (!R_base) {
	return 0;
    }
    R = R_base;
    area++;
    area++;
    area++;
    area++;
    if (area > current_area) {
	if (current_area == 0) {
	    *R = (union group *) xmalloc(area * sizeof(Group));
	} else {
	    *R = (union group *) xrealloc((char*)*R, area * sizeof(Group));
	}
	current_area = area;
    }
    if (!*R) {
	return 0;
    }
    return 1;
}

/* Evaluate a decl node but use a different number of nbits.  Do this
   by using a shadow_decl and setting the nbits.  This is used for function
   and sysfunction return values.  Note that REAL and INTEGER attr bits
   are valid in the passed-in decl.
*/

void eval_nbits(tree decl, nbits_t nbits)
{
    if (!global_shadow) {
	global_shadow = make_node(SHADOW_REF);
    }
    TREE_NBITS(global_shadow) = nbits;
    TREE_CHAIN(global_shadow) = decl;
    TREE_REAL_ATTR(global_shadow) = TREE_REAL_ATTR(decl);
    TREE_INTEGER_ATTR(global_shadow) = TREE_INTEGER_ATTR(decl);
    eval_1(global_shadow);
}

nbits_t eval_range(tree * inst, char *ident)
{
    Group *g;
    nbits_t val;

    eval(inst);

    g = *--R;
    if (BVAL(g)) {
	error("Index in '%s' resloves to 'z' or 'x'", ident, NULL_CHAR);
    } else {
	val = AVAL(g);
    }
    return val;
}

/* Evaluate the expression and return the low 32 bits */

Bit eval_const(tree * inst)
{
    Group *g;
    Bit const_;

    eval(inst);

    g = *--R;
    if (BVAL(g)) {
	const_ = 0;
    } else {
	const_ = AVAL(g);
    }
    return const_;
}

/* Evaluate an expression;  return an integer non-zero if the top
   of stack is non-zero.  Then pop the stack. */

enum logical_value test(tree * expr)
{
    enum logical_value cond;
    ngroups_t i;
    Group *g;

    eval(expr);
    DETERMINE_TRUE_COND(g);
    return cond;
}

/***********************************************************************
 *	
 * init_eval
 *	- initializes all global and static variables for eval.c
 *
 ***********************************************************************
 */

void init_eval()
{
    R_nbits = 0;
    R_ngroups = 0;
    in_cond = 0;
    is_integer = 0;
    is_real = 0;
    current_area = 0;
    current_regs = 0;
    x32 = ldexp((double) 1, 32);
    memset(&R_base, 0, sizeof(R_base));
    memset(&R, 0, sizeof(R));
    memset(&R_mask, 0, sizeof(R_mask));
    global_shadow = NULL_TREE;
}
