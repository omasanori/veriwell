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

/* PASS3.C - Snap forward pointers, resolve hierarchical names, expr lengths */

#define PASS3_C

#include <stdlib.h>
#include <string.h>
#include "glue.h"
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "obstack.h"
#include "acc_user.h"
#include "pli.h"
#include "systask.h"
#include "decl.h"
#include "multdiv.h"
#include "pass3.h"
#include "scope.h"
#include "specify.h"
#include "gates.h"
#include "eval.h"
#include "pass2.h"
#include "timescal.h"
#include "veriwell.h"
#if VDEBUG != 0
#include <stdio.h>
#include "flags.h"
#endif

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

extern tree current_scope;

tree *debug_;
tree debug_tree;

static int in_lval = 0;		/* set if processing lvalue (used in concat) */
static int in_connect_ports = 0;	/* set if building port connections */
#define INSERT_PTR(NODE) obstack_ptr_grow (&inst_obstack, (tree) NODE)

/* This obstack contains pointers to expression/ref/decl nodes in the
   order that they should be evaluated (postfix). */
struct obstack inst_obstack;
struct obstack alt_inst_obstack;

/* This is some global information that needs to be kept around for
   when markers need to be generated for references inside event
   expressions.  Perhaps this is a grungy way of doing it.  */

struct Marker_info marker_info;

/* save the number of instructions for the last expression
   processed in case bit_ref needs it */
static int last_inst_length;

/* this is used as a temp stack fro saving stack size */
extern struct obstack linkage_obstack;

/* In fixup_nbits, while the tree is being walked, use the following
   variables to keep track of the space required to execute the
   expression. */
ngroups_t have_for_push, have_for_pad, stack_size = 0, stack_extension = 0;
int stack_lineno;
char *stack_filename;

tree debug_node;

/* Lets NET lvals point to its source */
tree global_cont_assign;

/* Keep track of the largest number of regsisters required thus far. */
int max_label = 0;

/* The instruction obstack may be interrupted so that part-select references
   and other constants can be evaluated.  When something needs to be
   evaluated, push the current state of the obstack, and start a new one.
   When the evaluation is complete, pop the obstack and restore the state
   so that new stuff can be added to the old stuff. */

struct inst_level {
    struct inst_level *prev;	/* where to pop to */
    void *base;
    void *finish;
};

static struct inst_level *inst_stack;


void push_inst()
{
    void *finish = obstack_next_free(&inst_obstack);	/* mark temporary end */
    void *base = obstack_finish(&inst_obstack);
    struct inst_level *tem =
	(struct inst_level *) obstack_alloc(&inst_obstack,
					    sizeof(struct inst_level));
    tem->prev = inst_stack;
    tem->base = base;		/* save the base */
    tem->finish = finish;	/* save the current end */
    inst_stack = tem;
}

void pop_inst()
{
    struct inst_level *tem = inst_stack;
    void *base;

    inst_stack = tem->prev;
    base = tem->base;
    obstack_free(&inst_obstack, tem->finish);	/* unwind to end of prev stack */
    obstack_set_base(&inst_obstack, base);	/* restore prev stack base */
    return;
}

/* Certain instructions need to keep track of the size of subexpressions.
   In doing so, it will push the current stack size settings and reset
   them.  It will pop the saved setting at the end of its operation. */

struct stack_size_level {
    struct stack_size_level *prev;	/* where to pop to */
    ngroups_t have_for_push;
    ngroups_t have_for_pad;
};

static struct stack_size_level *stack_size_stack;

void push_stack_size()
{
    struct stack_size_level *tem
	= (struct stack_size_level *) obstack_alloc(&linkage_obstack,
						    sizeof(struct
							   stack_size_level));
    tem->prev = stack_size_stack;
    tem->have_for_push = have_for_push;
    tem->have_for_pad = have_for_pad;
    stack_size_stack = tem;
    have_for_pad = have_for_push = 0;
}

void pop_stack_size()
{
    struct stack_size_level *tem = stack_size_stack;

    stack_size_stack = tem->prev;
    have_for_push = tem->have_for_push;
    have_for_pad = tem->have_for_pad;
    obstack_free(&linkage_obstack, (char *) tem);
    return;
}

/* At the end of pass 3, stack_allocate is called to allocate space
   for the stack */

void stack_allocate()
{
    if (!R_alloc(max_label, stack_size)) {
	lineno = stack_lineno;
	input_filename = stack_filename;
	fatal("Out of memory", NULL);
    }
}

/* Make sure there is enough space on the stack for an expression that
   will be evaluated in the middle of another expression.  Eg shift count
   and conditional operator. */
/* Note that this code is very similar to pass3_expr() and perhaps should
   be merged someday. */

static nbits_t ensure_stack_space(tree node)
{
    nbits_t nbits;
    ngroups_t tmp_pad;
    char *type = tree_code_type[TREE_CODE(node)];

    push_stack_size();		/* suspend current stack count */
    nbits = fixup_nbits(node);	/* generate instructions & shift cnt */
    adjust_nbits(nbits, &node, (tree *) obstack_base(&inst_obstack));
    if (*type != 'e' || (*++type != '1' && *type != 'r'))
	reserve_stack_space(TREE_SUB_LABEL(node), nbits, nbits);
    tmp_pad = have_for_push + have_for_pad;	/* treat all as padding */
    pop_stack_size();		/* get back current count */
    have_for_pad = max(tmp_pad, have_for_pad);	/* account for the extra padding */
    return nbits;
}

/* reserve space on the expression stack.  Most of the space in needed to
   push datum, but some is for the result which would be included
   in the calculation for the parent expression, if any, so it is
   treated as temporary padding. */

static void
reserve_stack_space(int number, nbits_t nbits_push, nbits_t nbits_pad)
{
    ngroups_t need_for_push = number * (bits_to_groups(nbits_push) + 1);
    ngroups_t need_for_pad = bits_to_groups(nbits_pad) + 1;

    have_for_push += need_for_push;	/* add to the stack */
    if (need_for_push >= have_for_pad)	/* take away from the padding */
	have_for_pad = 0;
    else
	have_for_pad -= need_for_push;
    have_for_pad = max(have_for_pad, need_for_pad);	/* incread pad if nec. */
}

/* Return the module containing the given scope */
tree module_of(tree scope)
{
    if (TREE_CODE(scope) == MODULE_BLOCK)
	return scope;
    else
	return (module_of(BLOCK_UP(scope)));
}

tree lookdown_scope(char *ident, tree scope)
{
    tree t;

    // ??? mdh - also look in parent scopes
    do {
	for (t = BLOCK_DOWN(scope); t; t = TREE_CHAIN(t)) {
	    if (!strcmp(IDENT(BLOCK_NAME(t)), ident))
		return t;
	}
	scope = BLOCK_UP(scope);
    }
    while (scope);
    return NULL_TREE;
}

/* Build a marker if the reference is inside an even expression */

void maybe_build_marker(tree decl)
{
    if (decl == error_mark_node) {
	return;
    }
    if (marker_info.in_event)
	BuildMarker(decl, &marker_info);
}

void adjust_nbits(nbits_t nbits, tree * tree_addr, tree * code_base)
{
    tree node = *tree_addr;
    ngroups_t ngroups;
    char *tree_type_ptr = tree_code_type[TREE_CODE(node)];

    /* Always make sure there is space on the stack */
    if ((ngroups = bits_to_groups(nbits)) >= stack_size)
	stack_size = ngroups + 1;
//    if (stack_size > 50)
//      fatal ("Big stack size at line %lu\n", lineno);

    if (TREE_NBITS(node) >= nbits)
	return;
    switch (*tree_type_ptr) {
    case ('e'):
	switch (*++tree_type_ptr) {
	case ('1'):		/* binary condition */
	    TREE_NBITS(node) = nbits;
	    return;

	case ('x'):		/* binary context-dependent */
	    TREE_NBITS(node) = nbits;
	    adjust_nbits(nbits, &TREE_OPERAND(node, 0), code_base);
	    adjust_nbits(nbits, &TREE_OPERAND(node, 1), code_base);
	    return;

	case ('u'):		/* unary context-dependent */
	    TREE_NBITS(node) = nbits;
	    adjust_nbits(nbits, &TREE_OPERAND(node, 0), code_base);
	    return;

	case ('r'):		/* unary reduction */
	case ('c'):		/* unary type conversion */
	    TREE_NBITS(node) = nbits;
	    return;

	case ('s'):		/* shift */
	    TREE_NBITS(node) = nbits;
	    /* pass on to operand, not to shift count */
	    adjust_nbits(nbits, &TREE_OPERAND(node, 0), code_base);
	    return;

	case ('3'):		/* conditional */
	    TREE_NBITS(node) = nbits;
	    adjust_nbits(nbits, &COND_TRUE(node), COND_TRUE_CODE(node));
	    adjust_nbits(nbits, &COND_FALSE(node), COND_FALSE_CODE(node));
	    return;
	}

    case ('r'):
    case ('c'):
	TREE_NBITS(node) = nbits;
	return;
    case ('d'):
	{
	    tree t = make_node(SHADOW_REF);
	    TREE_CHAIN(t) = node;
	    TREE_REAL_ATTR(t) = TREE_REAL_ATTR(node);
	    TREE_INTEGER_ATTR(t) = TREE_INTEGER_ATTR(node);
	    *tree_addr = t;
	    /* go back through code pointers and change decl pointer to
	       ref pointer */
	    {
		tree *p;
		for (p = code_base; *p != node; p++);
		*p = t;
	    }
	    TREE_NBITS(t) = nbits;
	    return;
	}
    }
}

static struct part_info *cook_part_ref(nbits_t offset_high,
				       nbits_t offset_low, int concat)
{
    struct part_info *info =
	(struct part_info *) xmalloc(sizeof(struct part_info));
    int rel_offset_low = offset_low % BITS_IN_GROUP;
    int rel_offset_high = offset_high % BITS_IN_GROUP;
    int length = offset_high - offset_low + 1;

    /* Case 1: within one group */
    if (offset_high / BITS_IN_GROUP == offset_low / BITS_IN_GROUP) {
	PART_ALIGNED(info) = 1;

	PART_LMASK1(info) =
	    ~(mask_right[rel_offset_low] ^
	      mask_right[rel_offset_high + 1]);
	PART_RMASK(info) =
	    mask_right[rel_offset_high - rel_offset_low + 1];
	PART_NGROUPS(info) = 0;
	PART_SHIFT(info) = rel_offset_low;
	if (rel_offset_high == BITS_IN_GROUP - 1)
	    PART_NEXT(info) = 1;	/* next avail group in concat */
	else
	    PART_NEXT(info) = 0;	/* still room in this group */
    } else {
	PART_NGROUPS(info) = bits_to_groups(length);
	PART_ALIGNED(info) = rel_offset_high >= rel_offset_low;
	PART_RMASK(info) =
	    mask_right[rel_offset_high - rel_offset_low + 1];
	PART_LMASK1(info) = mask_right[rel_offset_low];
	if (!PART_ALIGNED(info))
	    PART_LMASK2(info) = ~mask_right[rel_offset_high + 1];
	else
	    PART_LMASK2(info) =
		~(mask_right[rel_offset_low] ^
		  mask_right[rel_offset_high + 1]);
	PART_SHIFT(info) = rel_offset_low;
	PART_NEXT(info) = bits_to_groups(length + rel_offset_low + 1);
    }
    return info;
}

/* Evaluate LSB or MSB of DECL or part-select reference.  Must scan
   expression tree (see fixup_nbits) and generate code.  Then evaluate
   expression checking for legal range limits. */

nbits_t get_range(tree node, char *ident)
{
    tree *expr_code;
    nbits_t index;

    push_inst();		/* Push the code obstack since we're prob in the middle of something */
    expr_code = pass3_expr(node);
    /*
     * allow things to continue even if errors exist
     */
//  if( max_label == 0 ) {
//    return 0;
//  }
    if (TREE_NBITS(node) > MAX_BITS_IN_NUMBER)
	error("Range expression must be within 32 bits", NULL_CHAR,
	      NULL_CHAR);
    if (!R_alloc(max_label, stack_size))
	fatal("Not enough memory to evaluate range expression of %s",
	      IDENTIFIER_POINTER(DECL_NAME(node)));
    index = eval_range(expr_code, ident);
    pop_inst();
    return index;
}

/* Evaluate 32-bit constant */

Group *get_const(tree node, nbits_t * nbits_ptr)
{
    tree *expr_code;
    Group *val;

    push_inst();		/* Push the code obstack since we're prob in the middle of something */
    expr_code = pass3_expr(node);
    if (!R_alloc(max_label, stack_size))
	fatal("Not enough memory to evaluate constant expression",
	      NULL_CHAR);
    val = eval_(expr_code, nbits_ptr);
//  if (TREE_NBITS (node) > MAX_BITS_IN_NUMBER)
//    error ("Constant expression must be within 32 bits", NULL_CHAR, NULL_CHAR);
    pop_inst();
    return val;
}

/* Evaluate 32-bit scaled delay */

Bit get_delay(tree node)
{
    tree *expr_code;
    Group *val;
    nbits_t nbits_ptr;

    push_inst();		/* Push the code obstack since we're prob in the middle of something */
    expr_code = timescale_scale(node);
    if (!R_alloc(max_label, stack_size))
	fatal("Not enough memory to evaluate constant expression",
	      NULL_CHAR);
    val = eval_(expr_code, &nbits_ptr);
//  if (TREE_NBITS (node) > MAX_BITS_IN_NUMBER)
//    error ("Constant expression must be within 32 bits", NULL_CHAR, NULL_CHAR);
//  ASSERT( nbits_ptr <= 32 );
    pop_inst();
    return AVAL(val);
}

/* resolve the hierarchical name and build a shadow for the decl node */

static nbits_t pass3_hierarchy(tree node)
{
    tree t1 = resolve_hierarchical_name(node);
    tree t = make_node(SHADOW_REF);

    REF_NAME(t) = node;
    TREE_CHAIN(t) = t1;
    TREE_NBITS(t) = TREE_NBITS(t1);
    HIERARCHICAL_ATTR(t) = 1;
    INSERT_PTR(t);
    maybe_build_marker(t1);	/* Marker to original decl */
    return TREE_NBITS(t);
}

static nbits_t fixup_reference(tree node)
{
    enum tree_code code = TREE_CODE(node);
    nbits_t msb, lsb, msb_decl, lsb_decl;
    nbits_t nbits, nbits1, nbits_decl, prev_nbits;
    Group *val;
    tree t;

    switch (code) {
    case BIT_REF:
	BIT_EXPR_CODE(node) = pass3_expr_intrude(BIT_EXPR(node), 1);
	TREE_LABEL(node) = TREE_LABEL(BIT_EXPR(node));
	TREE_SUB_LABEL(node) = TREE_SUB_LABEL(BIT_EXPR(node));
	if (TREE_NBITS(*BIT_EXPR_CODE(node)) > MAX_BITS_IN_NUMBER)
	    error("Bit-select index expression must be within 32 bits",
		  NULL_CHAR, NULL_CHAR);

/* If this is a hierarchical reference, then there was no way to
   determine if this was a bit reference or an array reference.
   Once we know, make sure the reference is set correctly.  Since
   bit and array references look identical, then all that need to
   be dome is to set the code correctly so that eval() will
   do the right thing. */

	if (HIERARCHICAL_ATTR(node)) {
	    BIT_REF_DECL(node) =
		resolve_hierarchical_name(BIT_REF_NAME(node));
	    if (TREE_CODE(BIT_REF_DECL(node)) == ARRAY_DECL)
		TREE_SET_CODE(node, ARRAY_REF);
	}
	maybe_build_marker(BIT_REF_DECL(node));
	if (TREE_CODE(node) == ARRAY_REF)
	    TREE_NBITS(node) = TREE_NBITS(BIT_REF_DECL(node));
	else
	    TREE_NBITS(node) = 1;
	break;

    case ARRAY_REF:
	ARRAY_EXPR_CODE(node) = pass3_expr_intrude(ARRAY_EXPR(node), 1);
	if (TREE_NBITS(*ARRAY_EXPR_CODE(node)) > MAX_BITS_IN_NUMBER)
	    error("Array index expression must be within 32 bits",
		  NULL_CHAR, NULL_CHAR);

	maybe_build_marker(ARRAY_REF_DECL(node));
	TREE_NBITS(node) = TREE_NBITS(ARRAY_REF_DECL(node));
	break;

    case CONCAT_REP_REF:
	val = get_const(CONCAT_EXPR(node), &nbits);
	if (!BVAL(val))
	    CONCAT_COUNT(node) = AVAL(val);
	else
	    CONCAT_COUNT(node) = 0;	/* X or Z in expression */
	if (nbits > MAX_BITS_IN_NUMBER)
	    warning
		("Repeat count expression is larger than 32 bits; will be truncated",
		 NULL, NULL);

    case CONCAT_REF:
	nbits = prev_nbits = 0;
//      push_inst ();

/* TREE_PURPOSE (t) is the member expression.  TREE_VALUE (t) points
   to another tree_list node.  TREE_PURPOSE of this 2nd list node is
   the expression code; TREE_VALUE of the 2nd list node points to a
   part_info structure.
   */

	for (t = CONCAT_LIST(node); t; t = TREE_CHAIN(t)) {
	    tree tmp_tree;

	    TREE_VALUE(t) = make_node(TREE_LIST);

	    TREE_PURPOSE(TREE_VALUE(t)) =
		(tree) pass3_expr_intrude(TREE_PURPOSE(t), 1);
	    TREE_LABEL(node) =
		max(TREE_LABEL(node), TREE_LABEL(TREE_PURPOSE(t)));
	    TREE_SUB_LABEL(node) =
		max(TREE_SUB_LABEL(node), TREE_SUB_LABEL(TREE_PURPOSE(t)));

/* To get the right NBITS, use TREE_NBITS from the original expression
   if it is not a hierarchical reference; get TREE_NBITS from the
   dereferenced expression code if it is a hierarchical reference.
   */

	    tmp_tree =
		H_(*(tree *) TREE_PURPOSE(TREE_VALUE(t)), TREE_PURPOSE(t));
	    nbits1 = TREE_NBITS(tmp_tree);

/* lval case shifts over previous concat entry; rval case shifts
   from absolute location */

	    TREE_VALUE(TREE_VALUE(t)) =
		(tree) cook_part_ref(nbits + nbits1 - 1, nbits, 1);

	    /* nbits is the total number of bits in the concat ref */
	    nbits += nbits1;
	    if (bits_to_groups(nbits) > bits_to_groups(nbits1))
		prev_nbits = nbits % BITS_IN_GROUP;
	    else
		prev_nbits = nbits1;
	}
	TREE_LABEL(node) += 1;
	TREE_SUB_LABEL(node) += 1;
	CONCAT_NBITS(node) = nbits;
	if (code == CONCAT_REF)
	    TREE_NBITS(node) = nbits;
	else
	    TREE_NBITS(node) = nbits * CONCAT_COUNT(node);
	break;

    case PART_REF:
	if (HIERARCHICAL_ATTR(node)) {
	    PART_DECL(node) = resolve_hierarchical_name(PART_NAME(node));
	    if (PART_DECL(node) == error_mark_node) {
		TREE_NBITS(node) = 1;
		break;
	    }
	}

/* If the decl that node is pointing to is a redeclared port, move the pointer
   to the newer decl */
/*

      if ((PORT_OUTPUT_ATTR (PART_DECL (node)) ||
	   PORT_INPUT_ATTR (PART_DECL (node)))
	  && DECL_THREAD (PART_DECL (node)))
	PART_DECL (node) = DECL_THREAD (PART_DECL (node));
	*/
	PART_MSB(node) = msb =
	    get_range(PART_MSB_(node),
		      IDENTIFIER_POINTER(DECL_NAME(PART_DECL(node))));
	PART_LSB(node) = lsb =
	    get_range(PART_LSB_(node),
		      IDENTIFIER_POINTER(DECL_NAME(PART_DECL(node))));

	nbits = msb - lsb;
	if (ABS(nbits) + 1 > MAX_BITS)
	    error("Part-select for '%s' encompases too many bits",
		  IDENT(DECL_NAME(PART_DECL(node))), NULL_CHAR);
	msb_decl = MSB(PART_DECL(node));
	lsb_decl = LSB(PART_DECL(node));
	nbits_decl = msb_decl - lsb_decl;
	if (nbits < 0) {
	    if (nbits_decl > 0)
		error("MSB and LSB of '%s' are reversed",
		      IDENT(DECL_NAME(PART_DECL(node))), NULL_CHAR);
	    else if (msb < msb_decl || lsb > lsb_decl)
		error
		    ("Part-select is outside limits set by declaration of '%s'",
		     IDENT(DECL_NAME(PART_DECL(node))), NULL_CHAR);
	} else {
	    if (nbits_decl < 0)
		error("MSB and LSB of '%s' are reversed",
		      IDENT(DECL_NAME(PART_DECL(node))), NULL_CHAR);
	    else if (msb > msb_decl || lsb < lsb_decl)
		error
		    ("Part-select is outside limits set by declaration of '%s'",
		     IDENT(DECL_NAME(PART_DECL(node))), NULL_CHAR);
	}
	PART_STORAGE(node) = DECL_STORAGE(PART_DECL(node))
	    + (ABS(lsb - lsb_decl)) / BITS_IN_GROUP;
	PART_INFO(node) =
	    cook_part_ref(ABS(msb - lsb_decl), ABS(lsb - lsb_decl), 0);
	TREE_NBITS(node) = ABS(nbits) + 1;
	maybe_build_marker(PART_DECL(node));
	break;

    case FUNCTION_REF:
	if (HIERARCHICAL_ATTR(node))
	    t = search_scope(FUNC_REF_NAME(node), 0);
	else
//    t = lookdown_scope (FUNC_REF_NAME (node), current_pass3_module);
	    t = lookdown_scope(FUNC_REF_NAME(node),
			       module_of(current_scope));
	if (!t) {
	    error("Function '%s' not defined", FUNC_REF_NAME(node),
		  NULL_CHAR);
	    break;
	}
	FUNC_REF_INASSIGN(node) = NULL_TREE;
	FUNC_REF_FUNC(node) = t;
	FUNC_REF_CONTEXT(node) = current_scope;
	if (FUNC_REF_ARGS(node)) {
	    pass3_args(FUNC_REF_ARGS(node), FUNC_REF_NAME(node),
		       &FUNC_REF_INASSIGN(node), (tree *) NULL,
		       BLOCK_PORTS(t), 1);
	}
	TREE_NBITS(node) = TREE_NBITS(FUNCT_DECL(t));
	TREE_REAL_ATTR(node) = TREE_REAL_ATTR(FUNCT_DECL(t));
	TREE_INTEGER_ATTR(node) = TREE_INTEGER_ATTR(FUNCT_DECL(t));
	break;

    case SYSFUNCTION_REF:
	{
	    p_tfcell p_tf;
	    enum sysfunction_type sysfunction =
		lookup_sysfunction(FUNC_REF_NAME(node), &p_tf);
	    if (sysfunction == -1) {
		if (lookup_systask(FUNC_REF_NAME(node), &p_tf) != -1)
		    error
			("'%s' is a system task; it cannot be called as a function",
			 FUNC_REF_NAME(node), NULL_CHAR);
		else
		    error("Unknown system function '%s'",
			  FUNC_REF_NAME(node), NULL_CHAR);
		TREE_NBITS(node) = 32;
		break;
	    }
	    FUNC_REF_CONTEXT(node) = current_scope;
	    SET_FUNC_REF_SYSTYPE(node, sysfunction);
	    FUNC_REF_USERTF(node) = p_tf;
	    init_sysfunction(node);
	}
	break;

    }				/* switch */
    return TREE_NBITS(node);
}

/* Convert op1 to real if necessary */

int pass3_early_conversion(tree * op1_ptr, tree op2)
{
    tree op1 = *op1_ptr;

    /* if op1 is not real but op2 is, insert a real conversion operator */
    if (!TREE_REAL_ATTR(op1) && TREE_REAL_ATTR(op2)) {
	*op1_ptr = build_unary_op(REAL_CONV_EXPR, op1);
	TREE_REAL_ATTR(*op1_ptr) = 1;
	TREE_INTEGER_ATTR(*op1_ptr) = 0;
	TREE_NBITS(*op1_ptr) = 32;
	return 1;
    }
    return 0;
}

/* Convert op1 to integer if necessary */

int pass3_early_both_conversion(tree * op1_ptr, tree op2)
{
    tree op1 = *op1_ptr;

    /* if op1 is real, but op2 is not, convert op1 to integer */
    if (TREE_REAL_ATTR(op1) && !TREE_REAL_ATTR(op2)) {
	*op1_ptr = build_unary_op(INT_CONV_EXPR, op1);
	TREE_REAL_ATTR(*op1_ptr) = 0;
	TREE_INTEGER_ATTR(*op1_ptr) = 1;
	TREE_NBITS(*op1_ptr) = 1;
	return 1;
    } else
	return pass3_early_conversion(op1_ptr, op2);
}


/* After it is already too late, add an integer->real conversion block */

int pass3_late_conversion(tree * op1_ptr, tree op2, int code_offset)
{
    tree *p, *p_end;

    if (pass3_early_conversion(op1_ptr, op2)) {
	INSERT_PTR(NULL_TREE);	/* Make room for one more pointer */

	/* Start at current end, shift all instruction pointers down one
	   until we are at the place where the conversion instruction needs
	   to be inserted */
	p = (tree *) obstack_next_free(&inst_obstack);
	p_end =
	    (tree *) ((char *) obstack_base(&inst_obstack) + code_offset);
	while (--p > p_end)
	    *p = *(p - 1);
	*p = *op1_ptr;		/* Insert newly-created conversion block */
	return 1;
    }
    return 0;
}

/* Fixup_nbits performs several tasks: 
   1) sets TREE_NBITS for each node with is the expression length of the
      result of the operation.  It make sure that the Verilog rules for
      context-dependent expression length are followed.  If two operands
      of an operator ar of different lengths, adjust_nbits will be called
      to enlarge the smaller subexpression.  Adjusting does not go below
      conditional and reduction operators since their lengths are not context-
      dependent.
   2) As the expression tree is walked, a list is created that points to
      all of the nodes in the expression in postfix order.  This list will
      later be used to execute/evaluate the expression at runtime.
   3) As part-references are found, the expressions for their msbs and lsbs
      are evaluated and tested for consistancy (same direction as the declared
      var, etc.).  This is done by recursively calling fixup_nbits.
*/

nbits_t fixup_nbits(tree node)
{
    nbits_t nbits_left, nbits_right, nbits;
    enum tree_code code = TREE_CODE(node);
    char *tree_type_ptr = tree_code_type[code];
    tree *finish;
    int finish_size, op0_offset;


    switch (*tree_type_ptr) {
    case 'x':
	if (HIERARCHICAL_ATTR(node))
	    return pass3_hierarchy(node);
	else if (node == error_mark_node)
	    return 1;
#if VDEBUG != 0
	else if (vdebug) {
	    if (code == ERROR_MARK) {
		printf_error_V("Pass3 over error_mark");
		return 0;
	    }
	}
#endif
	else
	    fatal("Internal: Attempt to reparse identifier", NULL_CHAR);
	break;

    case ('r'):
	INSERT_PTR(node);
	TREE_NBITS(node) = fixup_reference(node);
	return TREE_NBITS(node);

    case ('d'):		/* decl nodes will have had TREE_NBITS set */
	maybe_build_marker(node);
//      if (!INITIALIZED_ATTR (node))
//  fatal ("Uninitialized decl: %s\n", IDENTIFIER_POINTER (DECL_NAME (node)));
	if (in_lval)
	    if (code == NET_VECTOR_DECL || code == NET_SCALAR_DECL)
		NET_ASSIGNMENT(node) = global_cont_assign;

    case ('c'):		/*  as will constant nodes */
	INSERT_PTR(node);
	return TREE_NBITS(node);

    case ('e'):
	switch (*++tree_type_ptr) {

	    /* The condition operator (?:) is treated as a binary op for the
	       purpose of allocating stack space since in the worst case
	       (condition = X), both true and false expressions need to be
	       evaluated.  But, make sure there is enough stack space for
	       the condition expression */

	case ('3'):
	    COND_EXPR_CODE(node) = pass3_expr_intrude(COND_EXPR(node), 0);

	    push_inst();
	    nbits_left = fixup_nbits(COND_TRUE(node));
	    INSERT_PTR(NULL_TREE);	/* Make room for possible conversion */
	    op0_offset = obstack_object_size(&inst_obstack);
	    INSERT_PTR(NULL_TREE);
	    finish_size = obstack_object_size(&inst_obstack);
	    finish = (tree *) obstack_finish(&inst_obstack);
	    COND_TRUE_CODE(node) =
		(tree *) obstack_copy(&alt_inst_obstack, finish,
				      finish_size);

	    if (pass3_early_conversion(&COND_FALSE(node), COND_TRUE(node)))
		TREE_REAL_ATTR(node) = 1;
	    nbits_right = fixup_nbits(COND_FALSE(node));
	    INSERT_PTR(NULL_TREE);
	    finish_size = obstack_object_size(&inst_obstack);
	    finish = (tree *) obstack_finish(&inst_obstack);
	    COND_FALSE_CODE(node) =
		(tree *) obstack_copy(&alt_inst_obstack, finish,
				      finish_size);
	    pop_inst();		/* go back to current expression */

	    /* go back and add real conversion if necessary */
	    if (pass3_early_conversion(&COND_TRUE(node), COND_FALSE(node))) {
		tree *p = COND_TRUE_CODE(node) + op0_offset;
		*p = COND_TRUE(node);
		nbits_left = 32;
		TREE_REAL_ATTR(node) = 1;
	    }

	    if (nbits_left < nbits_right) {
		adjust_nbits(nbits_right, &COND_TRUE(node),
			     COND_TRUE_CODE(node));
		nbits = nbits_right;
	    } else if (nbits_left > nbits_right) {
		adjust_nbits(nbits_left, &COND_FALSE(node),
			     COND_FALSE_CODE(node));
		nbits = nbits_left;
	    } else
		nbits = nbits_left;	/* nbits_left equals nbits_right */

	    INSERT_PTR(node);
	    TREE_NBITS(node) = (*tree_type_ptr == '1') ? 1 : nbits_left;
	    break;

/* For a binary context-dependent operations, return the largest of the
   left and right operands and make the smaller side the same size. */

	case ('x'):
	case ('1'):
	    nbits_left = fixup_nbits(TREE_OPERAND(node, 0));
	    op0_offset = obstack_object_size(&inst_obstack);

	    /* && and || are conditional, the operator terminate once the
	       truth can be deterimined.  Thus, the right op is recuresively
	       evaluated at runtime only if necessary; a seperate set of
	       instructions is generated for this. */

	    if (code == TRUTH_ANDIF_EXPR || code == TRUTH_ORIF_EXPR) {
		push_inst();
		if (pass3_early_conversion(&TREE_OPERAND(node, 1),
					   TREE_OPERAND(node, 0))) {
		    TREE_REAL_ATTR(node) = 1;
		    TREE_INTEGER_ATTR(node) = 0;
		}
		nbits_right = fixup_nbits(TREE_OPERAND(node, 1));
		INSERT_PTR(NULL_TREE);
		finish_size = obstack_object_size(&inst_obstack);
		finish = (tree *) obstack_finish(&inst_obstack);
		TRUTH_EXPR_CODE(node) =
		    (tree *) obstack_copy(&alt_inst_obstack, finish,
					  finish_size);
		pop_inst();
		if (pass3_late_conversion(&TREE_OPERAND(node, 1),
					  TREE_OPERAND(node, 0),
					  op0_offset)) {
		    TREE_REAL_ATTR(node) = 1;
		    TREE_INTEGER_ATTR(node) = 0;
		}
	    } else {
		if (pass3_early_conversion(&TREE_OPERAND(node, 1),
					   TREE_OPERAND(node, 0))) {
		    TREE_REAL_ATTR(node) = 1;
		    TREE_INTEGER_ATTR(node) = 0;
		}
		nbits_right = fixup_nbits(TREE_OPERAND(node, 1));
		if (pass3_late_conversion(&TREE_OPERAND(node, 0),
					  TREE_OPERAND(node, 1),
					  op0_offset)) {
		    TREE_REAL_ATTR(node) = 1;
		    TREE_INTEGER_ATTR(node) = 0;
		}

		if (TREE_REAL_ATTR(TREE_OPERAND(node, 0))
		    && TREE_REAL_ATTR(TREE_OPERAND(node, 1))) {
		    TREE_REAL_ATTR(node) = 1;
		    TREE_INTEGER_ATTR(node) = 0;
		}
	    }

	    if (nbits_left < nbits_right) {
		adjust_nbits(nbits_right, &TREE_OPERAND(node, 0),
			     (tree *) obstack_base(&inst_obstack));
		nbits = nbits_right;
	    } else if (nbits_left > nbits_right) {
		if (code == TRUTH_ANDIF_EXPR || code == TRUTH_ORIF_EXPR)
		    adjust_nbits(nbits_left, &TREE_OPERAND(node, 1),
				 TRUTH_EXPR_CODE(node));
		else
		    adjust_nbits(nbits_left, &TREE_OPERAND(node, 1),
				 (tree *) obstack_base(&inst_obstack));
		nbits = nbits_left;
	    } else
		nbits = nbits_left;	/* nbits_left equals nbits_right */

	    /* reserve enough stack space for the subexpression */
	    if (*tree_type_ptr == '1')
		reserve_stack_space(TREE_SUB_LABEL(node), nbits,
				    (nbits_t) 1);

	    INSERT_PTR(node);
	    TREE_NBITS(node) = nbits = (*tree_type_ptr == '1') ? 1 : nbits;

	    break;

	case 'r':
	    if (code == POSEDGE_EXPR || code == NEGEDGE_EXPR)
		TREE_OPERAND(node, 1) = (tree) X;
	    reserve_stack_space(TREE_SUB_LABEL(node),
				fixup_nbits(TREE_OPERAND(node, 0)),
				(nbits_t) 1);
	    if (TREE_REAL_ATTR(TREE_OPERAND(node, 0)))
		error("Reduction not allowed on real numbers", NULL, NULL);
	    INSERT_PTR(node);
	    TREE_NBITS(node) = nbits = 1;
	    break;

	case 's':		/* shift operations */
	    TREE_NBITS(node) = nbits = fixup_nbits(SHIFT_OPERAND(node));
	    SHIFT_NBITS(node) = ensure_stack_space(SHIFT_COUNT(node));
	    if (SHIFT_NBITS(node) > MAX_BITS_IN_NUMBER)
		error("Shift count must be within 32 bits", NULL_CHAR,
		      NULL_CHAR);

	    if (TREE_REAL_ATTR(SHIFT_OPERAND(node))
		|| TREE_REAL_ATTR(SHIFT_COUNT(node)))
		error("Shift not allowed on real numbers", NULL, NULL);

	    INSERT_PTR(node);
	    break;

	case 'u':
	    TREE_NBITS(node) = nbits = fixup_nbits(TREE_OPERAND(node, 0));
	    TREE_REAL_ATTR(node) = TREE_REAL_ATTR(TREE_OPERAND(node, 0));
	    TREE_INTEGER_ATTR(node) =
		TREE_INTEGER_ATTR(TREE_OPERAND(node, 0));
	    INSERT_PTR(node);
	    break;

	case 'c':
	    nbits = fixup_nbits(TREE_OPERAND(node, 0));

	    if (code == REAL_CONV_EXPR) {
		/* It is possible that the operand is already real (function or
		   hierarchical name).  If so, make it a nop. */
		if (TREE_REAL_ATTR(TREE_OPERAND(node, 0))) {
		    TREE_SET_CODE(node, NOP_EXPR);
		    TREE_NBITS(node) = nbits;
		    break;
		}
		TREE_NBITS(node) = nbits = 32;
	    } else
		/* When converting to integer, let adjust_nbits fill in the correct
		   size */
		TREE_NBITS(node) = nbits = 1;
	    INSERT_PTR(node);
	    break;

	default:
	    TREE_NBITS(node) = nbits = fixup_nbits(TREE_OPERAND(node, 0));
	    TREE_REAL_ATTR(node) = TREE_REAL_ATTR(TREE_OPERAND(node, 0));
	    TREE_INTEGER_ATTR(node) =
		TREE_INTEGER_ATTR(TREE_OPERAND(node, 0));
	    INSERT_PTR(node);
	    break;
	}			/* switch 'e' */
    }
    return nbits;
}

/* Process lval; discard any generated code; set lval flag so the proper
   concat code can be generated. */

void pass3_lval(tree * node)
{
    push_inst();
    /* Get here if lval is a hierarchical reference */
    if (TREE_CODE(*node) == IDENTIFIER_NODE && HIERARCHICAL_ATTR(*node)) {
	tree t, t1;

	t = make_node(SHADOW_LVAL_REF);
	t1 = resolve_hierarchical_name(*node);
	REF_NAME(t) = *node;
	TREE_CHAIN(t) = t1;
	TREE_NBITS(t) = fixup_nbits(t1);
	HIERARCHICAL_ATTR(t) = 1;
	*node = t;
    } else {
	in_lval = 1;
	pass3_node(*node);
	in_lval = 0;
    }
    pop_inst();
}

/* This routine starts the expression re-parsing and returns a pointer
   to the postfix sequence pointers. It also performs final type conversion
   if requested and if necessary. (This used to be pass3_expra until
   conversion was added.  Now there is a wrapper for pass3_expr that calls
   this).
*/

tree *pass3_expr_convert(tree node, enum convert_to convert_to)
{
    nbits_t nbits;
    char *type = tree_code_type[TREE_CODE(node)];

    /* initialize stack size for this expression */
    have_for_pad = 0;
    have_for_push = stack_extension;

    nbits = fixup_nbits(node);
    if (convert_to != eNONE) {
	if (convert_to == eREAL && !TREE_REAL_ATTR(node)) {
	    node = build_unary_op(REAL_CONV_EXPR, node);
	    TREE_NBITS(node) = 32;
	    INSERT_PTR(node);
	}

	else if (convert_to == eINT && TREE_REAL_ATTR(node)) {
	    node = build_unary_op(INT_CONV_EXPR, node);
	    TREE_NBITS(node) = 1;	/* adjust_bits will change this */
	    INSERT_PTR(node);
	}
    }
    adjust_nbits(nbits, &node, (tree *) obstack_base(&inst_obstack));

    if (*type != 'e' || (*++type != '1' && *type != 'r'))
	reserve_stack_space(TREE_SUB_LABEL(node), nbits, nbits);

    /* If this is the largest stack size seen so far, make it so */
    if (have_for_pad + have_for_push > stack_size) {
	stack_size = have_for_pad + have_for_push;
	stack_lineno = lineno;	/* save linenumber and filename of largest expr */
	stack_filename = input_filename;
    }
    stack_size = max(stack_size, have_for_pad + have_for_push);
#if VDEGUB != 0
    if (stack_size > 50)
	fatal("Too big stack size at line %lu\n", (char *) lineno);
#endif

    /* save largest label encountered thus far */
    if (TREE_LABEL(node) > max_label)
	max_label = TREE_LABEL(node);

    INSERT_PTR(NULL_TREE);	/* zero for stop */

    /* Save this in case the insts for this expression need to be
       copied to alt_inst_obstack (i.e. for bit_refs) */
    last_inst_length = obstack_object_size(&inst_obstack);
    return (tree *) obstack_finish(&inst_obstack);
}

/* This is a wrapper for pass3_expr_convert.  pass3_expr was changed to
   pass3_expr_convert when the conversion aspect was added.  Existing
   calls to pass3_expr did not have to be changed to add arguments.
*/

tree *pass3_expr(tree node)
{
    return pass3_expr_convert(node, eNONE);
}

/* This is like pass3_expr_convert, except convert op1 to be the same
   type as op2.  op2 is assumed to have already been through pass3_expr
   so its type is known.
*/

tree *pass3_expr_match_convert(tree op1, tree op2)
{
    if (TREE_REAL_ATTR(op2))
	return (pass3_expr_convert(op1, eREAL));
    else
	return (pass3_expr_convert(op1, eINT));
}

/* This is like pass3_expr, except that it is used to generate instructions
   for an expression while processing another expression.  For example,
   bit-select and array index expressions need to be evaluated during the
   of the expression the variable are used in.  Also, conditionals (?:),
   &&/||, concatentations, and function arguments call this.  Note that
   there are two flavors that have a subtile diffenece.  One case will
   restore the current expression's stack size at the end of the re-parse --
   this is the most common usage; the other does not -- this is used for
   (?:) expressions where the size of evaluating the result needs to
   be factored into the rest of the expression. */

tree *pass3_expr_intrude(tree node, int restore_stack)
{
    tree *finish, *tmp;
    int finish_size;

    push_inst();		/* start another sequence of insts */
    if (restore_stack)
	ensure_stack_space(node);
    else
	fixup_nbits(node);
    INSERT_PTR(NULL_TREE);	/* generate stop */
    finish_size = obstack_object_size(&inst_obstack);
    finish = (tree *) obstack_finish(&inst_obstack);

    obstack_grow(&alt_inst_obstack, finish, finish_size);
    tmp = (tree *) obstack_finish(&alt_inst_obstack);
    pop_inst();
    return tmp;
}

/* While expanding expression, create markers for each reference */

tree *pass3_expr_marker(tree node, Marker ** marker_ptr,
			enum marker_flags flags, tree info,
			tree convert_to)
{
    tree *expr;
    Marker *marker;

    marker_info.in_event = 1;
    marker_info.first = NULL;
    marker_info.last = NULL;
    marker_info.flags = flags;
    marker_info.delay = NULL;

    if (TREE_CODE(node) != EVENT_DECL) {
	if (convert_to)
	    expr = pass3_expr_match_convert(node, convert_to);
	else
	    expr = pass3_expr(node);
    } else {
	marker_info.flags =
	    (enum marker_flags) (~M_TEST & marker_info.flags);
	BuildMarker(node, &marker_info);
	expr = NULL;
    }

    marker_info.in_event = 0;

    /* now that expression is re-parsed, scan all of the generated markers
       and add the pointer to the expression and make the list of markers
       circular */
    if (marker_info.first) {	/* NULL if constant expression */
	for (marker = marker_info.first; marker->link;
	     marker = marker->link) {
	    if (info == NULL)
		marker->expr.expr = expr;	/* In most cases, put expr code in marker */
	    else
		marker->expr.tree = info;	/* But, somtimes use it for monitor, gates */
	}
	if (info == NULL)
	    marker->expr.expr = expr;
	else
	    marker->expr.tree = info;

	/* if other markers there, append these news ones */
	if (*marker_ptr) {
	    for (marker = *marker_ptr; marker->link != *marker_ptr; marker = marker->link);	/* gets to end of list */
	    marker->link = marker_info.first;	/* append */
	    marker_info.last->link = *marker_ptr;	/* make circular */
	} else {
	    marker->link = marker_info.first;	/* Make list circular */
	    *marker_ptr = marker_info.first;
	}
    }

    return expr;
}


void pass3_delay(tree node)
{
    unsigned int i;

    if (!node)
	return;

    for (i = 0; i < TREE_LABEL(node); i++) {
	if (TREE_OPERAND(node, i)) {
//      TREE_OPERAND (node, i + 3) = (tree) pass3_expr (TREE_OPERAND (node, i));
	    TREE_OPERAND(node, i + 3) =
		(tree) timescale_scale(TREE_OPERAND(node, i));
	}
    }
}

/* There are several cases for assignments.  The simple case is a simple
   a = b assignment.  The second case is a continuous assignment.  The
   third case is a continuous assignment with a delay.  The fourth case
   is a procedural assignment with an event (not yet implemented).
   Return true if the lval and the rval are the same size before adjusting
   (needed for port discrepancy warning) */


int pass3_assignment(tree node)
{
    nbits_t nbits;
    enum tree_code code = TREE_CODE(node);

/* First, process lval */
    /* If this is a cont assignment w/delay, then there is two assignments
       with a temp var in the middle.  Note that net assignments w/delay
       only have one assignment.  Allocate storage for it and make code for
       its rval
       in the second assignment (pointed to by TREE_CHAIN) */

    if (code == ASSIGN_CONT_STMT) {
	pass3_delay(STMT_ASSIGN_DELAY(node));
	if (TREE_CHAIN(node)
	    && TREE_CODE(TREE_CHAIN(node)) == ASSIGN_CONT_STMT) {
	    pass3_lval(&STMT_ASSIGN_LVAL(TREE_CHAIN(node)));
	    TREE_NBITS(STMT_ASSIGN_LVAL(node)) =
		TREE_NBITS(STMT_ASSIGN_LVAL(TREE_CHAIN(node)));
	    pass3_decl(STMT_ASSIGN_LVAL(node));
	    STMT_ASSIGN_RVAL_CODE(TREE_CHAIN(node)) =
		pass3_expr(STMT_ASSIGN_LVAL(node));
	    marker_info.current_scb = BuildSCB(TREE_CHAIN(node), NOLIST);
	    marker_info.last = NULL;
	    marker_info.first = NULL;
	    marker_info.delay = STMT_ASSIGN_DELAY(node);
	    marker_info.flags = (enum marker_flags) (M_FIXED + M_CONT);
	    BuildMarker(STMT_ASSIGN_LVAL(node), &marker_info);
	}
    }

    global_cont_assign = node;
    pass3_lval(&STMT_ASSIGN_LVAL(node));
    nbits = TREE_NBITS(STMT_ASSIGN_LVAL(node));

//  pass3_early_both_conversion (&STMT_ASSIGN_RVAL (node), STMT_ASSIGN_LVAL (node));
    if (code == ASSIGN_STMT
	|| code == ASSIGN_EVENT_STMT || code == ASSIGN_DELAY_STMT
	|| code == ASSIGN_NONBLK_EVENT_STMT
	|| code == ASSIGN_NONBLK_DELAY_STMT)
	STMT_ASSIGN_RVAL_CODE(node) =
	    pass3_expr_match_convert(STMT_ASSIGN_RVAL(node),
				     STMT_ASSIGN_LVAL(node));

/* if continuous assignment, generate markers with the rval. */

    else if (code == ASSIGN_CONT_STMT) {
	STMT_ASSIGN_RVAL_CODE(node) =
	    pass3_expr_marker(STMT_ASSIGN_RVAL(node),
			      &STMT_ASSIGN_EVENT_LIST(node),
			      (enum marker_flags) (M_FIXED + M_CONT), node,
			      STMT_ASSIGN_LVAL(node));
    }

    else if (code == ASSIGN_PROC_STMT || FORCE_STMT) {	/* Procedural cont assign */
	/* if lval is hierarchical, use the actual decl */
	tree lval = the_lval(STMT_ASSIGN_LVAL(node));

	STMT_ASSIGN_RVAL_CODE(node) =
	    pass3_expr_marker(STMT_ASSIGN_RVAL(node),
			      &STMT_ASSIGN_EVENT_LIST(node),
			      M_CONT, node, lval);

	/* create temporary storage for force value */
	tree tmp = make_node(TMP_DECL);
	STMT_FORCE_TMP_DECL(node) = tmp;
	TREE_NBITS(tmp) = TREE_NBITS(STMT_ASSIGN_RVAL(node));
	TREE_CHAIN(tmp) = tmp;
	pass3_decl(tmp);

	/* Generate a marker for the lval, thread it in at the beginning 
	   and make the last market point to the new first marker.
	   Note that all the markers point back to the continuous assignment.
	 */

	{
	    Marker *last = marker_info.last;

	    marker_info.first = marker_info.last = NULL;
	    /* if lval is a shadow, then it was a hierarchical reference;
	       point to the original node. */
	    BuildMarker(lval, &marker_info);
	    marker_info.first->expr.tree = node;
	    marker_info.first->flags = (marker_flags)(M_FORCE | marker_info.first->flags);
	    marker_info.first->link = STMT_ASSIGN_EVENT_LIST(node);
	    STMT_ASSIGN_EVENT_LIST(node) = marker_info.first;
	    if (last)
		last->link = marker_info.first;
	    else
		STMT_ASSIGN_EVENT_LIST(node)->link =
		    STMT_ASSIGN_EVENT_LIST(node);
	}
    }
//  if (nbits == TREE_NBITS (STMT_ASSIGN_RVAL (node)))
    if (nbits == TREE_NBITS(*STMT_ASSIGN_RVAL_CODE(node)))
	return 1;
    else
//    if (TREE_CHAIN (node) && TREE_CODE (TREE_CHAIN (node)) == ASSIGN_CONT_STMT)
//      adjust_nbits (nbits, &STMT_ASSIGN_RVAL (TREE_CHAIN (node)));
//    else

	adjust_nbits(nbits, &STMT_ASSIGN_RVAL(node),
		     STMT_ASSIGN_RVAL_CODE(node));
    return 0;
}

/* arg is a tree_list of arguments; port is a chain of decl_nodes */

void
pass3_args(tree arg_list, char *name, tree * inassign, tree * outassign,
	   tree port_list, int stack_args)
{
    tree arg, arg_tmp, port, *assign;

    port = port_list;
    for (arg_tmp = arg_list; arg_tmp;
	 arg_tmp = TREE_CHAIN(arg_tmp), port = TREE_CHAIN(port)) {
	arg = TREE_PURPOSE(arg_tmp);
	if (!port) {
	    error
		("More TASK/FUNCTION arguments than in TASK/FUNCTION '%s'",
		 name, NULL_CHAR);
	    return;
	}
	if (PORT_INPUT_ATTR(port)) {
	    if (stack_args)
		assign = pass3_expr_intrude(arg, 1);
	    else
		assign = pass3_expr(arg);

	    /* DECL_THREAD is the redefined version of the port */
	    if (DECL_THREAD(port))
		adjust_nbits(TREE_NBITS(DECL_THREAD(port)), &arg, assign);
	    else
		adjust_nbits(TREE_NBITS(port), &arg, assign);
	    *inassign = tree_cons((tree) assign, NULL_TREE, *inassign);
	}
	if (PORT_OUTPUT_ATTR(port)) {
	    if (!is_proc_lval(arg))
		error("Illegal expression connected to output port",
		      NULL_CHAR, NULL_CHAR);
	    else
		pass3_lval(&arg);
	    *outassign = tree_cons(arg, NULL_TREE, *outassign);
	}
    }
    if (port) {
	error("Fewer TASK/FUNCTION arguments than in TASK/FUNCTION '%s'",
	      name, NULL_CHAR);
	return;
    }
    if (inassign)
	*inassign = nreverse(*inassign);
    if (outassign)
	*outassign = nreverse(*outassign);
}

void pass3_delay_stmt(tree node)
{
    tree tmp_tree;

    STMT_DELAY_EXPR_CODE(node) = timescale_scale(STMT_DELAY_EXPR(node));
    tmp_tree = H_(*STMT_DELAY_EXPR_CODE(node), STMT_DELAY_EXPR(node));
}

void pass3_event_stmt(tree node)
{
    tree t;

    STMT_EVENT_MARKER(node) = NULL;
    for (t = STMT_EVENT_EXPR_LIST(node); t; t = TREE_CHAIN(t))
	TREE_EXPR_CODE(t) = pass3_expr_marker(TREE_EXPR(t),
					      &STMT_EVENT_MARKER(node),
					      M_TEST, NULL_TREE,
					      NULL_TREE);
}

/* Reparse a chain of statements, adding a pointer on the last statement's
   TREE_CHAIN */

tree dump;			/* initialized in pass3_tree to point to dump instruction */
tree _return;			/* initialized in pass3_node to point to return instruction */
tree dummy_return;		/* initialized in pass3_tree for disable stmt support */
SCB *finish_scb;


void pass3_node(tree node)
{
    pass3_node_tail(node, NULL_TREE);
}

void pass3_node_tail(tree node, tree label)
{
    enum tree_code code = TREE_CODE(node);
    tree t, t1, alt_chain, prev, tmp_tree;
    extern void print_common();

#if VDEBUG != 0
    if (vdebug)
	printf("pass3: Node: %s @ %p, chain %p\n",
	       tree_code_name[code], node, TREE_CHAIN(node));
#endif
//HEAPTEST ("Heap corrupted (in beginning of pass3)");

    switch (*tree_code_type[code]) {
    case ('b'):

/* Scan Module */

	if (code == MODULE_BLOCK && !UDP_ATTR(node)) {
//  current_scope = node;
	    set_scope(node);
//  current_pass3_module = node;



/* Scan continuous assignments */

	    for (t = MODULE_ASSIGNMENTS(node); t; t = TREE_CHAIN(t)) {
		lineno = STMT_SOURCE_LINE(TREE_PURPOSE(t));
		input_filename = STMT_SOURCE_FILE(TREE_PURPOSE(t));
		pass3_assignment(TREE_PURPOSE(t));
	    }

/* insert specify paths */

	    insert_specify_paths(node);

/* now actually process specify paths */

	    for (t = BLOCK_BODY(node); t; t = TREE_CHAIN(t))
		if (TREE_CODE(t) == GATE_INSTANCE) {
		    pass3_specify(t);
		}

/* Scan timing checks */

	    for (t = MODULE_SPECDEFS(node); t; t = TREE_CHAIN(t)) {
		if (TREE_CODE(t) == CHECK_SPEC) {
		    pass3_timing_check(t, node);
		}
	    }

/* Scan INITIAL and ALWAYS blocks ignoring gates */

	    for (t = BLOCK_BODY(node); t; t = TREE_CHAIN(t))
		if (TREE_CODE(t) == INSTANCE_NODE && !UDP_ATTR(t))
		    pass3_node(INSTANCE_BLOCK(t));
		else if (TREE_CODE(t) == GATE_INSTANCE)
		    pass3_gate(t);
		else if (TREE_CODE(t) != INSTANCE_NODE)
		    pass3_node(TREE_PURPOSE(t));

/* Scan functions and tasks within current module */

	    for (t = BLOCK_DOWN(node); t; t = TREE_CHAIN(t))
		if (TREE_CODE(t) != MODULE_BLOCK)
		    pass3_node(t);

	    current_scope = pop_scope();
	}
	/* MODULE */
	else if (code == TASK_BLOCK || code == FUNCTION_BLOCK) {
//  current_scope = node;
	    set_scope(node);

	    TASK_OUTPUT(node) = TASK_INPUT(node) = NULL;
	    for (t = BLOCK_PORTS(node); t; t = TREE_CHAIN(t)) {
		/* Input is an lvalue */
		if (PORT_INPUT_ATTR(t)) {
		    push_inst();
		    if (PORT_REDEFINED_ATTR(t))	/* re-decl'ed port */
			TASK_INPUT(node) =
			    tree_cons(DECL_THREAD(t), NULL_TREE,
				      TASK_INPUT(node));
		    else
			TASK_INPUT(node) =
			    tree_cons(t, NULL_TREE, TASK_INPUT(node));
		    pop_inst();
		}
		/* Output is an rvalue */
		if (PORT_OUTPUT_ATTR(t)) {
		    if (PORT_REDEFINED_ATTR(t))	/* re-decl'ed port */
			TASK_OUTPUT(node) =
			    tree_cons((tree) pass3_expr(DECL_THREAD(t)),
				      NULL_TREE, TASK_OUTPUT(node));
		    else
			TASK_OUTPUT(node) =
			    tree_cons((tree) pass3_expr(t), NULL_TREE,
				      TASK_OUTPUT(node));
		}
	    }
	    if (TREE_CODE(node) != TASK_BLOCK
		&& TREE_CODE(node) != FUNCTION_BLOCK)
		fatal("oops", NULL);
	    if (!node)
		fatal("oops!", NULL);
	    TASK_OUTPUT(node) = nreverse(TASK_OUTPUT(node));
	    TASK_INPUT(node) = nreverse(TASK_INPUT(node));

	    /* Scan body of task or function. */
	    if (code == FUNCTION_BLOCK) {
		int max_label_save;
		ngroups_t stack_size_save;

//    FUNCT_DECL_CODE (node) = pass3_expr (FUNCT_DECL (node));
		TREE_NBITS(node) = TREE_NBITS(FUNCT_DECL(node));

		stack_size_save = stack_size;
		max_label_save = max_label;
		max_label = 0;
		stack_size = 0;
		pass3_node_tail(BLOCK_BODY(node), node);
		TREE_LABEL(node) = max_label + 2 + 4;
		FUNCT_AREA(node) = stack_size + 1 + 10;
		stack_size = stack_size_save;
		max_label = max_label_save;
	    } else		/* TASK_BLOCK */
		pass3_node_tail(BLOCK_BODY(node), node);
	    current_scope = pop_scope();
	}
	/* TASK, FUNCTION */
	else if (code == NAMED_BLOCK)
	    break;
	else
	    goto default1;
	break;

    case ('e'):
    case ('r'):
    case ('d'):
	fixup_nbits(node);
	break;

    case ('s'):
	for (; node; node = TREE_CHAIN(node)) {
	    code = TREE_CODE(node);
	    prev = node;
	    lineno = STMT_SOURCE_LINE(node);
	    input_filename = STMT_SOURCE_FILE(node);
	    alt_chain = TREE_CHAIN(node);
	    /* this makes sure that statments that use STMT_BODY and whose
	       statements then merge with TREE_CHAIN will correctly propigate
	       the label if the statement is last in a list (else, the end
	       of STMT_BODY will have NULL instead of the correct label) */
	    if (!alt_chain)
		alt_chain = label;

	    switch (code) {
	    case (ALWAYS_BLOCK):
		marker_info.current_scb = BuildSCB(node, READY_LIST);
		pass3_node_tail(STMT_BODY(node), node);
		break;

	    case (INITIAL_BLOCK):
		marker_info.current_scb = BuildSCB(node, READY_LIST);
		pass3_node_tail(STMT_BODY(node), dump);
		break;

	    case (BEGIN_STMT):
		pass3_node_tail(STMT_BODY(node), alt_chain);
		break;

	    case BEGIN_NAMED_STMT:
		set_scope(STMT_BLOCK(node));
		pass3_node_tail(STMT_BODY(node), alt_chain);
		current_scope = pop_scope();
		break;

		/* Unlike the BEGIN stmt, the body of FORK first has the accompanying
		   JOIN stmt and then a TREE_LIST(!) of statements.  Make each
		   stamtement point to the JOIN and them move the JOIN to the end of
		   FORK.  Note that the statements on the body might be BEGIN/END
		   or nested FORK/JOIN. */
	    case FORK_NAMED_STMT:
		set_scope(STMT_BLOCK(node));
	    case FORK_STMT:
		t1 = STMT_BODY(node);	/* Points to JOIN */
		for (t = TREE_CHAIN(t1); t; t = TREE_CHAIN(t))
		    pass3_node_tail(TREE_PURPOSE(t), t1);

		/* first stmt in chain is always the JOIN stmt. */
		STMT_BODY(node) = TREE_CHAIN(t1);	/* take JOIN out of list */
		TREE_CHAIN(t1) = TREE_CHAIN(node);	/* Thread JOIN in after FORK */
		TREE_CHAIN(node) = t1;
		STMT_JOIN_FORK(t1) = node;	/* Make join point to fork */
		if (code == FORK_NAMED_STMT)
		    current_scope = pop_scope();
		break;

	    case END_STMT:
	    case END_NAMED_STMT:
	    case JOIN_STMT:
	    case JOIN_NAMED_STMT:
//      case DUMP_STMT:     /* doesn't get here often: initial #t stmt; */
		break;

	    case ASSIGN_STMT:
	    case ASSIGN_PROC_STMT:
	    case FORCE_STMT:
		pass3_assignment(node);
		break;

	    case ASSIGN_DELAY_STMT:
	    case ASSIGN_EVENT_STMT:
		pass3_assignment(node);
		STMT_ASSIGN_TMP(node) = make_node(TMP_DECL);
		TREE_CHAIN(STMT_ASSIGN_TMP(node)) = STMT_ASSIGN_LVAL(node);
		pass3_decl(STMT_ASSIGN_TMP(node));
		STMT_ASSIGN_TMPCODE(node) =
		    pass3_expr(STMT_ASSIGN_TMP(node));
		if (code == ASSIGN_DELAY_STMT)
		    pass3_delay_stmt(STMT_ASSIGN_DELAY(node));
		else
		    pass3_event_stmt(STMT_ASSIGN_EVENT(node));
		break;

	    case ASSIGN_NONBLK_DELAY_STMT:
	    case ASSIGN_NONBLK_EVENT_STMT:
		pass3_assignment(node);
		if (code == ASSIGN_NONBLK_DELAY_STMT)
		    pass3_delay_stmt(STMT_ASSIGN_DELAY(node));
		else {
		    SCB *save_scb = marker_info.current_scb;

		    STMT_ASSIGN_TMP(node) = make_node(TMP_DECL);
		    TREE_CHAIN(STMT_ASSIGN_TMP(node)) =
			STMT_ASSIGN_LVAL(node);
		    pass3_decl(STMT_ASSIGN_TMP(node));
		    STMT_ASSIGN_TMPCODE(node) =
			pass3_expr(STMT_ASSIGN_TMP(node));

		    marker_info.current_scb = BuildSCB(node, NOLIST);
		    pass3_event_stmt(STMT_ASSIGN_EVENT(node));
		    STMT_ASSIGN_SCB(node) = marker_info.current_scb;
		    marker_info.current_scb = save_scb;
		}
		break;

	    case (IF_STMT):
//HEAPTEST ("Heap corrupted (in pass3 -- if)");
		STMT_COND_CODE(node) = pass3_expr(STMT_COND(node));
		pass3_node_tail(STMT_THEN(node), alt_chain);
		if (STMT_ELSE(node)) {
		    pass3_node_tail(STMT_ELSE(node), alt_chain);
		}
		break;

	    case (WHILE_STMT):
		STMT_WHILE_COND_CODE(node) =
		    pass3_expr(STMT_WHILE_COND(node));
		pass3_node_tail(STMT_BODY(node), node);
		break;

	    case (REPEAT_INIT_STMT):
		/* stmt_body points to repeat_init node */
		STMT_REPEAT_EXPR_CODE(node) =
		    pass3_expr(STMT_REPEAT_EXPR(node));
		tmp_tree =
		    H_(*STMT_REPEAT_EXPR_CODE(node),
		       STMT_REPEAT_EXPR(node));
		if (TREE_NBITS(tmp_tree) > MAX_BITS_IN_NUMBER)
		    error("REPEAT expression must be within 32 bits",
			  NULL_CHAR, NULL_CHAR);
		STMT_REPEAT_VAR(node) =
		    (Group *) xmalloc(sizeof(Group) *
				      (1 +
				       bits_to_groups(TREE_NBITS
						      (tmp_tree))));

		pass3_node_tail(STMT_BODY(node), alt_chain);
		break;

	    case REPEAT_STMT:
		/* inherit values from REPEAT_INIT */
		STMT_REPEAT_EXPR_CODE(node) =
		    STMT_REPEAT_EXPR_CODE(STMT_BODY(node));
		STMT_REPEAT_VAR(node) = STMT_REPEAT_VAR(STMT_BODY(node));
		STMT_BODY(node) = STMT_BODY(STMT_BODY(node));
		break;

	    case (FOR_STMT):
		pass3_node(STMT_FOR_ASSIGN(node));
		if (!STMT_SURROGATE_ATTR(node)) {	/* just do this once */
		    STMT_FOR_COND_CODE(node) =
			pass3_expr(STMT_FOR_COND(node));
		    pass3_node_tail(STMT_BODY(node), alt_chain);
		}
		/* If this is the iteration part, TREE_CHAIN is pointing to
		   the initial part.  Make TREE_CHAIN point to the same place
		   as the initial part is pointing. */
		else {
		    STMT_FOR_COND_CODE(node) =
			STMT_FOR_COND_CODE(TREE_CHAIN(node));
		    STMT_BODY(node) = STMT_BODY(TREE_CHAIN(node));
		    TREE_CHAIN(node) = NULL_TREE;	/* Let tail fix this */
		}
		break;

	    case FOREVER_STMT:
		pass3_node_tail(STMT_BODY(node), node);
		break;

	    case CASE_STMT:
	    case CASEX_STMT:
	    case CASEZ_STMT:
		STMT_CASE_EXPR_CODE(node) =
		    pass3_expr(STMT_CASE_EXPR(node));
		{
		    tree tmp_tree =
			H_(*STMT_CASE_EXPR_CODE(node),
			   STMT_CASE_EXPR(node));
		    nbits_t nbits = TREE_NBITS(tmp_tree);
		    ngroups_t ngroups = 1 + bits_to_groups(nbits);

		    stack_extension += ngroups;
		    for (t = STMT_CASE_LIST(node); t; t = TREE_CHAIN(t)) {
			/* evaluate the expressions */
			for (t1 = TREE_PURPOSE(t); t1; t1 = TREE_CHAIN(t1)) {
			    TREE_EXPR_CODE(t1) = pass3_expr(TREE_EXPR(t1));
/********** THIS BREAKS IF EXPR IS LARGER THAN CASE_EXPR!!! */
			    adjust_nbits(nbits, &TREE_EXPR(t1),
					 TREE_EXPR_CODE(t1));
			}
			pass3_node_tail(TREE_VALUE(t), alt_chain);
		    }
		    if (STMT_CASE_DEFAULT(node))
			pass3_node_tail(STMT_CASE_DEFAULT(node),
					alt_chain);

		    stack_extension -= ngroups;
		}
		break;

	    case DELAY_STMT:
		pass3_delay_stmt(node);
		pass3_node_tail(STMT_BODY(node), alt_chain);
		break;

	    case (WAIT_STMT):
		STMT_WAIT_MARKER(node) = NULL;
		STMT_WAIT_EXPR_CODE(node) =
		    pass3_expr_marker(STMT_WAIT_EXPR(node),
				      &STMT_WAIT_MARKER(node), M_PLAIN,
				      NULL_TREE, NULL_TREE);
		pass3_node_tail(STMT_BODY(node), alt_chain);
		break;

	    case EVENT_STMT:
		pass3_event_stmt(node);
		pass3_node_tail(STMT_BODY(node), alt_chain);
		break;

	    case ARROW_STMT:
		if (HIERARCHICAL_ATTR(STMT_ARROW_NAME(node)))
		    STMT_ARROW_EVENT(node) = double_check_named_event
			(resolve_hierarchical_name(STMT_ARROW_NAME(node)));
		else
		    STMT_ARROW_EVENT(node) = STMT_ARROW_NAME(node);
		break;

	    case (DISABLE_BLOCK_STMT):
	    case (DISABLE_TASK_STMT):
		pass3_node(STMT_BODY(node));
		break;

	    case DISABLE_STMT:
		if (HIERARCHICAL_ATTR(node))
		    t = search_scope(IDENTIFIER_POINTER
				     (STMT_DISABLE_NAME(node)), 0);
		else {
		    t = double_check_block(STMT_DISABLE_NAME(node));
		}
		if (!t) {
		    error("Undefined scope '%s' cannot be disabled",
			  IDENT(STMT_DISABLE_NAME(node)), NULL_CHAR);
		    break;
		}
		STMT_DISABLE_SCOPE(node) = t;
		break;

	    case (DEASSIGN_STMT):
//    pass3_node (STMT_BODY (node));
//    break;

	    case (RELEASE_STMT):
//    pass3_node (STMT_BODY (node));
		pass3_lval(&STMT_BODY(node));
		break;

	    case TASK_STMT:
		if (HIERARCHICAL_ATTR(node))
		    t = search_scope(STMT_TASK_NAME(node), 0);
		else
		    t = lookdown_scope(STMT_TASK_NAME(node),
				       module_of(current_scope));
		if (!t || TREE_CODE(t) != TASK_BLOCK) {
		    error("Task '%s' not defined", STMT_TASK_NAME(node),
			  NULL_CHAR);
		    break;
		}
		STMT_TASK_INASSIGN(node) = NULL_TREE;
		STMT_TASK_OUTASSIGN(node) = NULL_TREE;
		STMT_TASK_TASK(node) = t;
		if (STMT_TASK_ARGS(node))
		    pass3_args(STMT_TASK_ARGS(node), STMT_TASK_NAME(node),
			       &STMT_TASK_INASSIGN(node),
			       &STMT_TASK_OUTASSIGN(node), BLOCK_PORTS(t),
			       0);
		break;

	    case SYSTASK_STMT:
		{
		    p_tfcell p_tf;
		    enum systask_type systask;
		    systask = lookup_systask(STMT_TASK_NAME(node), &p_tf);
		    if (systask == -1) {
			if (lookup_sysfunction(STMT_TASK_NAME(node), &p_tf)
			    != -1)
			    error
				("'%s' is a function; it cannot be called as a task",
				 STMT_TASK_NAME(node), NULL_CHAR);
			else
			    error("Unknown system task '%s'",
				  STMT_TASK_NAME(node), NULL_CHAR);
			break;
		    }
		    SET_STMT_SYSTASK_TYPE(node, systask);
		    STMT_SYSTASK_USERTF(node) = p_tf;
		    init_systask(node);
		}
		break;

	    case NULL_STMT:
		break;

	    case LOOPBACK_STMT:
		/* if points to CASE, then chance to point to what CASE pointes to */
		if (TREE_CODE(STMT_BODY(node)) == CASE_STMT ||
		    TREE_CODE(STMT_BODY(node)) == CASEX_STMT ||
		    TREE_CODE(STMT_BODY(node)) == CASEZ_STMT)
		    STMT_BODY(node) = TREE_CHAIN(STMT_BODY(node));
		break;

	    default:
#if VDEBUG != 0
		printf_error_V("unimplemented PASS3 instruction:");
		print_common(node);
#else
		fatal
		    ("Internal error: Attempt to reparse unknown structure",
		     NULL_CHAR);
#endif
	    }			/* switch */

//HEAPTEST ("Heap corrupted (in pass3 -- end of stmts)");

	}			/* for (do next statement) */
	TREE_CHAIN(prev) = label;
	break;

    case ('x'):
#if VDEBUG != 0
	if (code == ERROR_MARK)
	    printf_error_V("line: %lu   error_mark\n", lineno);
	else {
	    printf_error_V("Unimplemented PASS3 x\n");
	    print_common(node);
	}
#else
	fatal
	    ("Fatal error: unexpected structure encountered during Phase III",
	     NULL_CHAR);
#endif

	break;

    default:
      default1:
#if VDEBUG != 0
	print_common(node);
	fatal("Fatal: Unexpected structure encountered during Phase III",
	      NULL_CHAR);
#else
	printf("Printing something here...\n");
	printf("node = %p\n", node);
	fatal
	    ("Fatal: ...Unexpected structure encountered during Phase III",
	     NULL_CHAR);
#endif
    }
}

void connect_instances(tree module)
{
    tree t, arg;
    int port_count, even;

    current_scope = module;	/* Instance args could be hierarchical names */
    for (t = MODULE_INSTANCES(module); t; t = TREE_CHAIN(t))
	if (TREE_CODE(t) == INSTANCE_NODE && !UDP_ATTR(t)) {
	    port_count = 1;
	    for (arg = INSTANCE_PORTS(t); arg;
		 arg = TREE_CHAIN(arg), port_count++)
		if (TREE_3RD(arg)) {
		    even = 1;
		    /* input port */
		    if (TREE_PURPOSE(TREE_3RD(arg)))
			even =
			    pass3_assignment(TREE_PURPOSE(TREE_3RD(arg)));
		    /* output port */
		    if (TREE_VALUE(TREE_3RD(arg)))
			even &=
			    pass3_assignment(TREE_VALUE(TREE_3RD(arg)));
		    if (!even) {
			lineno = STMT_SOURCE_LINE(t);
			input_filename = STMT_SOURCE_FILE(t);
			warning("Port sizes don't match in port #%d",
				(char *) port_count, NULL_CHAR);
		    }
		}
	    /* Recursively connect modules below this one */
	    connect_instances(INSTANCE_BLOCK(t));
	}
}

void pass3_tree(tree node)
{
    tree t;

    tree finish_stmt = build_stmt(FINISH_STMT, NULL);

    /* Dummy finish SCB is used so that there will be a command-line
       prompt after the last stmt if the last stmt causes a call to
       dispatch. */
    current_scope = NULL;
    finish_scb = BuildSCB(finish_stmt, NOLIST);
    dump = build_stmt(DUMP_STMT, NULL);
    dummy_return = build_stmt(DUMMY_RETURN_STMT, NULL);

    for (t = node; t; t = TREE_CHAIN(t))
	pass3_node(t);

    for (t = node; t; t = TREE_CHAIN(t))
	connect_instances(t);

}

void pass3_interactive(tree node)
{
    pass3_node_tail(node, dump);
}

/********************************************************************
 *
 *	init_pass3
 *		- initialize global and static variables for pass3.c
 *
 ********************************************************************
 */

void init_pass3()
{
    in_lval = 0;		/* set if processing lvalue (used in concat) */
    in_connect_ports = 0;	/* set if building port connections */
    stack_size = 0;
    stack_extension = 0;
    max_label = 0;
    debug_ = 0;
    debug_tree = NULL_TREE;
    memset(&inst_obstack, 0, sizeof(inst_obstack));
    memset(&alt_inst_obstack, 0, sizeof(alt_inst_obstack));
    memset(&marker_info, 0, sizeof(marker_info));
    last_inst_length = 0;
    have_for_push = 0;
    have_for_pad = 0;
    stack_lineno = 0;
    stack_filename = NULL;
    debug_node = NULL_TREE;
    global_cont_assign = NULL_TREE;
    memset(&inst_stack, 0, sizeof(inst_stack));
    memset(&stack_size_stack, 0, sizeof(stack_size_stack));
    dump = NULL_TREE;
    _return = NULL_TREE;
    dummy_return = NULL_TREE;
    memset(&finish_scb, 0, sizeof(finish_scb));
}
