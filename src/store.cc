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

/* STORE.C - Routines for storing results of expressions */

#define STORE_C

#include <stdio.h>
#include <stddef.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "eval.h"
#include "runtime.h"
#include "flags.h"
#include "store.h"
#include "veriwell.h"


int store_computeOnly = 0;
int store_useCached = 0;
SCB *store_scb = 0;

void store_array(tree decl, Bit index)
{
//  tree decl = ARRAY_REF_DECL (lval);
    ngroups_t array_hi = ARRAY_HI(decl);
    ngroups_t array_lo = ARRAY_LO(decl);
//  Bit aval = AVAL (g1);
    Group *g1, *g2;
    ngroups_t ngroups = 1 + bits_to_groups(TREE_NBITS(decl));
    ngroups_t offset, i;
    nbits_t nbits;
    int cond = 0;

    if (VECTOR_DIRECTION_ATTR(decl)) {
	if (index < array_lo || index > array_hi)
	    cond = X;
	else {
	    offset = (index - array_lo) * ngroups;
	}
    } else {
	if (index < array_hi || index > array_lo)
	    cond = X;
	else {
	    offset = (index - array_hi) * ngroups;
	}
    }

    g2 = *--R;
    if (!cond)
	g1 = DECL_STORAGE(decl) + offset;
    else
	return;

//  nbits = TREE_NBITS (lval);
    nbits = TREE_NBITS(decl);
    ngroups = bits_to_groups(nbits);

    for (i = 1; i <= ngroups; i++, g1++, g2++) {
	if (!cond) {
	    AVAL(g1) = AVAL(g2);
	    BVAL(g1) = BVAL(g2);
	} else {
	    AVAL(g1) = (Bit) - 1;
	    BVAL(g1) = (Bit) - 1;
	}
    }
    if (!cond) {
	AVAL(g1) = AVAL(g2) & mask_right1[nbits & NBITS_MASK];
	BVAL(g1) = BVAL(g2) & mask_right1[nbits & NBITS_MASK];
    } else {
	AVAL(g1) = (Bit) - 1 & mask_right1[nbits & NBITS_MASK];
	BVAL(g1) = (Bit) - 1 & mask_right1[nbits & NBITS_MASK];
    }
}

static int deferred = 0;	/* set during the storing of concats */

void notify(tree LVAL, enum logical_value STATE, int CHANGED, tree pc)
{
/* Currently, pc is null only on PLI calls to here */
    if (pc) {
	DECL_UPDATE_FILE(LVAL) = STMT_SOURCE_FILE(pc);
	DECL_UPDATE_LINE(LVAL) = STMT_SOURCE_LINE(pc);
    }
    ASSIGNTIME64(&DECL_UPDATE_TIME(LVAL), &CurrentTime);
    if (DECL_EVENT_CHAIN(LVAL) && (CHANGED))
	NotifyEvent(DECL_EVENT_CHAIN(LVAL), STATE, deferred);
}

// ??? mdh - delay part selects and arrays need to be fixed
void store(tree lval, tree pc)
{
    enum tree_code lval_code;
    Group *g1, *g2;
    Bit a, b, current_a, current_b, save_R_mask;
    int cond;
    int changed;
    ngroups_t i, ngroups, save_R_ngroups;
    nbits_t nbits, save_R_nbits;
    tree t;
    enum logical_value old_state, new_state;
    struct part_info *p;
//  tree lval;
    int offset;
    int saveUseCached = store_useCached;
    store_useCached = 0;

//          eval (STMT_ASSIGN_RVAL_CODE (pc));
//  lval = STMT_ASSIGN_LVAL (pc);
    lval_code = TREE_CODE(lval);

    switch (lval_code) {
/***************************************************************************/
/* Integer DECL                                                            */
/***************************************************************************/
/*
	  case INTEGER_DECL:
	    if( store_computeOnly ) {
		break;
	    }
	    g1 = DECL_STORAGE (lval);
	    g2 = *--R;
	    SET_DECL_STATE (lval, ZERO);
	    changed = 0;
	    if (AVAL (g1) != AVAL (g2)) {
	      if (AVAL (g1) != AVAL (g2)) {
		AVAL (g1) = AVAL (g2);
		changed = 1;
	      }
	      if (BVAL (g1) != BVAL (g2)) {
		BVAL (g1) = BVAL (g2);
		changed = 1;
	      }
	    }
//	    g1++; g2++;
//	    AVAL (g1) = AVAL (g2);
//	    BVAL (g1) = BVAL (g2);
	    SET_DECL_STATE (lval, ZERO);
	    SET_STATE (g1);
	    notify (lval, DECL_STATE (lval), changed, pc);
	    break;
*/
/***************************************************************************/
/* REG scalar DECL                                                         */
/***************************************************************************/

    case REG_SCALAR_DECL:
    case NET_SCALAR_DECL:
	if (store_computeOnly) {
	    break;
	}
	g1 = DECL_STORAGE(lval);
	g2 = *--R;
	old_state = DECL_STATE(lval);
	AVAL(g1) = AVAL(g2) & 1;
	BVAL(g1) = BVAL(g2) & 1;
	SET_DECL_STATE(lval, ZERO);
	SET_STATE(g1);
	changed = old_state != DECL_STATE(lval);
	notify(lval, DECL_STATE(lval), changed, pc);
	break;

/***************************************************************************/
/* REG vector DECL                                                         */
/***************************************************************************/

    case INTEGER_DECL:
    case REG_VECTOR_DECL:
    case NET_VECTOR_DECL:
    case TMP_DECL:
    case TIME_DECL:
    case PARAM_DECL:
    case SPECPARAM_DECL:
	if (store_computeOnly) {
	    break;
	}
	nbits = TREE_NBITS(lval);
	ngroups = bits_to_groups(nbits);
	g1 = DECL_STORAGE(lval);
	g2 = *--R;
	old_state = DECL_STATE(lval);
/* MAKE THIS changed = 0 AFTER DEBUGGING!!! */
	changed = 0;
	SET_DECL_STATE(lval, ZERO);
	if (ngroups > 0)
	    for (i = 1; i <= ngroups; i++, g1++, g2++) {
		if (AVAL(g1) != AVAL(g2)) {
		    AVAL(g1) = AVAL(g2);
		    changed = 1;
		}
		if (BVAL(g1) != BVAL(g2)) {
		    BVAL(g1) = BVAL(g2);
		    changed = 1;
		}
		SET_STATE(g1);
	    }
	if (((AVAL(g1) ^ AVAL(g2)) & mask_right1[nbits & NBITS_MASK])
	    || ((BVAL(g1) ^ BVAL(g2)) & mask_right1[nbits & NBITS_MASK])) {
	    AVAL(g1) = AVAL(g2) & mask_right1[nbits & NBITS_MASK];
	    BVAL(g1) = BVAL(g2) & mask_right1[nbits & NBITS_MASK];
	    changed = 1;
	}
	SET_STATE(g1);
	notify(lval, DECL_STATE(lval), changed, pc);
	break;

/***************************************************************************/
/* Part reference                                                          */
/***************************************************************************/

    case PART_REF:
	if (store_computeOnly) {
	    break;
	}
	g1 = PART_STORAGE(lval);
	g2 = *--R;

	changed = part_lref(g1, g2, PART_INFO(lval));
	notify(PART_DECL(lval), ZERO, changed, pc);
	break;

/***************************************************************************/
/* Bit reference                                                           */
/***************************************************************************/

    case BIT_REF:
	/* evaluate the index */
	nbits = R_nbits;	/* save these */
	ngroups = R_ngroups;
	eval(BIT_EXPR_CODE(lval));
	g2 = DECL_STORAGE(BIT_REF_DECL(lval));
//      g1 = *--R; /* points to index */

	cond = 0;
	changed = 0;
	new_state = ZERO;
	/* if index is a big number, make sure all high bits are zero */
	if (R_ngroups)		/* R_ngroups is from the index eval */
	    for (i = 1, g1 = *(R - 1) + 1; i < R_ngroups; i++, g1++) {
		if (BVAL(g1) || AVAL(g1)) {
		    cond = X;
		    break;
		}
	    }
	g1 = *--R;		/* Points to index */
	if (BVAL(g1) || cond)
	    cond = X;
	{
	    tree decl = BIT_REF_DECL(lval);
	    nbits_t decl_lsb = LSB(decl);
	    nbits_t decl_msb = MSB(decl);
	    Bit aval = AVAL(g1);
	    nbits_t bit_offset;
	    ngroups_t group_offset;

	    /* Make sure index is within bounds, else ignore */
	    if (VECTOR_DIRECTION_ATTR(decl)) {
		if (aval < decl_lsb || aval > decl_msb)
		    cond = Z;	/* will make store not happen */
		else {
		    bit_offset = (aval - decl_lsb) % BITS_IN_GROUP;
		    group_offset = bits_to_groups(aval - decl_lsb + 1);
		}
	    } else {
		if (aval < decl_msb || aval > decl_lsb)
		    cond = Z;	/* Will make store not happen */
		else {
//    bit_offset = (aval - decl_msb) % BITS_IN_GROUP;
//    group_offset = bits_to_groups (aval - decl_msb + 1);
		    bit_offset = (decl_lsb - aval) % BITS_IN_GROUP;
		    group_offset = bits_to_groups(decl_lsb - aval + 1);
		}
	    }

	    if (store_computeOnly) {
		// handle cond==Z case
		store_scb->bit_offset = bit_offset;
		if (cond == Z) {
		    store_scb->group_offset = 0xffffffff;
		} else {
		    store_scb->group_offset = group_offset;
		}
		R_nbits = nbits;	/* restore original values in case trace */
		R_ngroups = ngroups;	/*  need them. */
		break;
	    }
	    if (saveUseCached) {
		bit_offset = readylist->bit_offset;
		group_offset = readylist->group_offset;
		cond = 0;
		if (group_offset == 0xffffffff) {
		    cond = Z;
		}
	    }

	    /* take out the least significant bit and pop the stack */
	    a = AVAL(*--R) & 1;
	    b = BVAL(*R) & 1;
	    if (cond == Z)
		goto bit_ref_end;	/* Don't do store, but clean up */


	    current_a = AVAL(g2 + group_offset) & ((Bit) 1 << bit_offset);
	    current_b = BVAL(g2 + group_offset) & ((Bit) 1 << bit_offset);

	    if (a || cond == X) {	/* set bit */
		if (!current_a) {
		    AVAL(g2 + group_offset) |= (Bit) 1 << bit_offset;
		    changed = 1;
		    new_state = ONE;
		}
	    } else {		/* clear bit */

		if (current_a) {
		    AVAL(g2 + group_offset) &= ~((Bit) 1 << bit_offset);
		    changed = 1;
		} else
		    new_state = ONE;	/* never changed; reflects current val */
	    }

	    if (b || cond == X) {	/* set bit */
		if (!current_b) {
		    BVAL(g2 + group_offset) |= (Bit) 1 << bit_offset;
		    changed = 1;
		    new_state = (new_state == ZERO) ? Z : X;
		}
	    } else {		/* clear bit */

		if (current_b) {
		    Bit bval = BVAL(g2 + group_offset);
		    bval &= ~((Bit) 1 << bit_offset);
		    BVAL(g2 + group_offset) = bval;
//    BVAL (g2 + group_offset) &= ~((Bit) 1 << bit_offset);
		    changed = 1;
		} else
		    new_state = (new_state == ZERO) ? Z : X;
	    }
	}

      bit_ref_end:
	R_nbits = nbits;	/* restore original values in case trace */
	R_ngroups = ngroups;	/*  need them. */
	notify(BIT_REF_DECL(lval), new_state, changed, pc);
	break;

/****************************************************************************/
/* Array reference                                                          */
/****************************************************************************/

    case ARRAY_REF:
//{ int test = farheapcheck ();
//  if (test < 0)
//    fatal ("Corrupted before array store");
//}
	/* evaluate the index */
	save_R_nbits = R_nbits;	/* save these */
	save_R_ngroups = R_ngroups;
	save_R_mask = R_mask;

	eval(ARRAY_EXPR_CODE(lval));


	R_nbits = save_R_nbits;	/* save these */
	R_ngroups = save_R_ngroups;
	R_mask = save_R_mask;
	cond = 0;

	/* if index is a big number, make sure all high bits are zero */
//      if (R_ngroups)
//  for (i = 1, g1 = *(R - 1); i < R_ngroups; i++, g1++) {
//    if (BVAL (g1) || AVAL (g1)) {
//      cond = 'X';
//      break;
//    }
//  }
	g1 = *--R;		/* Points to index */
	if (BVAL(g1)) {
	    cond = Z;
	}

	offset = AVAL(g1);
	if (store_computeOnly) {
	    // handle cond==Z case
	    if (cond == Z) {
		store_scb->group_offset = 0xffffffff;
	    } else {
		store_scb->group_offset = offset;
	    }
	    break;;
	}
	if (saveUseCached) {
	    offset = readylist->group_offset;
	    cond = 0;
	    if (offset == 0xffffffff) {
		cond = Z;
	    }
	}
	if (cond) {
	    --R;
	    break;
	}

	store_array(ARRAY_REF_DECL(lval), offset);

	//     R_nbits = nbits;        /* restore original values in case trace */
	//     R_ngroups = ngroups;    /*  need them. */
	notify(ARRAY_REF_DECL(lval), ZERO, 1, pc);
	break;

/***************************************************************************/
/* Concatenation                                                           */
/***************************************************************************/

    case CONCAT_REF:
	store_useCached = saveUseCached;
	if (store_computeOnly) {
	    for (t = CONCAT_LIST(lval); t; t = TREE_CHAIN(t)) {
		p = (struct part_info *) TREE_VALUE(TREE_VALUE(t));
		store(*(tree *) TREE_PURPOSE(TREE_VALUE(t)), pc);
	    }
	    break;
	}
	g1 = *--R;
	g2 = g1;
	nbits = R_nbits;	/* save these */
	ngroups = R_ngroups;
	i = 0;
	deferred++;		/* Don't trigger con't assgn yet */
	/* assume TREE_LIST is in reverse order (lsb first) */
	for (t = CONCAT_LIST(lval); t; t = TREE_CHAIN(t)) {
	    p = (struct part_info *) TREE_VALUE(TREE_VALUE(t));
	    i += PART_NEXT(p);
	    current_a = AVAL(g1 + i);
	    current_b = BVAL(g1 + i);
//        g2 += part_rref (g1, g2, (struct part_info *) TREE_VALUE (TREE_VALUE (t)));
	    g2 += part_rref(g1, g2, p);
	    R++;
	    store(*(tree *) TREE_PURPOSE(TREE_VALUE(t)), pc);
	    AVAL(g1 + i) = current_a;
	    BVAL(g1 + i) = current_b;
	}
	{			/* Save the global deferred_marker and clear.  This is done
				   because NotifyDeferred is not automic.  The case in
				   that excersizes this is where two module have
				   concatenated argument ports which connect to
				   each other and are defined as INOUTs:
				   foo f0 ({a0, a1});
				   bar b0 ({a0, a1}); */
	    extern tree deferred_markers;
	    tree local_deferred = deferred_markers;
	    deferred_markers = NULL;
	    if (!--deferred)
		NotifyDeferred(local_deferred);
	}
	R_nbits = nbits;	/* restore original values in case trace */
	R_ngroups = ngroups;	/*  need them. */
	saveUseCached = store_useCached;
	break;

/***************************************************************************/
/* Shadow lval reference (from hierarchical ref)                                */
/***************************************************************************/

    case SHADOW_LVAL_REF:
	if (store_computeOnly) {
	    break;
	}
	store(TREE_CHAIN(lval), pc);
	break;

/***************************************************************************/
/* Real DECL                                                               */
/***************************************************************************/

    case REAL_DECL:
	if (store_computeOnly) {
	    break;
	}
	g1 = DECL_STORAGE(lval);
	g2 = *--R;
	SET_DECL_STATE(lval, ZERO);
	changed = 0;
	if (AVAL(g1) != AVAL(g2)) {
	    AVAL(g1) = AVAL(g2);
	    changed = 1;
	}
	if (BVAL(g1) != BVAL(g2)) {
	    BVAL(g1) = BVAL(g2);
	    changed = 1;
	}
//      g1++; g2++;
//      AVAL (g1) = AVAL (g2);
//      BVAL (g1) = BVAL (g2);
	SET_DECL_STATE(lval, ZERO);
	SET_STATE(g1);
	notify(lval, DECL_STATE(lval), changed, pc);
	break;

    default:
//    printf ("Line: %ld, File %s: \n", STMT_SOURCE_LINE (pc), STMT_SOURCE_FILE (pc));
	fatal("Internal error: Unimplemented lval reference: %s",
	      tree_code_name[lval_code]);
//          abort ();
    }				/* switch */
    store_useCached = saveUseCached;
}

void init_store()
{
    deferred = 0;
}
