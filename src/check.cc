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

/* CHECK.C - Compile-time semantic checking */

#include <stdio.h>
#include "vtypes.h"
#include "tree.h"
#include "decl.h"
#include "check.h"
#include "veriwell.h"

extern tree current_scope;
extern int in_tf;
extern int in_systask;		/* defined in parse.y */
extern int in_event;

unsigned int big_label = 0;


static void fixup_labels(tree t, tree op1, tree op2)
{
    unsigned int label1, label2, sub_label1, sub_label2;
    enum tree_code code_op1 = TREE_CODE(op1);
    enum tree_code code_op2 = TREE_CODE(op2);
    char *type_ptr = tree_code_type[code_op1];

    if (*type_ptr == 'e' ||
	code_op1 == BIT_REF || code_op1 == ARRAY_REF
	|| code_op1 == CONCAT_REF) {
	label1 = TREE_LABEL(op1);
	/* cond or reduction */
	if (*++type_ptr == '1' || *type_ptr == 'r') {
	    sub_label1 = 1;
	} else {
	    sub_label1 = TREE_SUB_LABEL(op1);
	}
    } else {
	label1 = 1;
	sub_label1 = 1;
    }

    type_ptr = tree_code_type[TREE_CODE(op2)];
    if (*type_ptr == 'e'
	|| code_op2 == BIT_REF
	|| code_op2 == ARRAY_REF || code_op2 == CONCAT_REF) {
	label2 = TREE_LABEL(op2);
	/* cond or reduction */
	if (*++type_ptr == '1' || *type_ptr == 'r') {
	    sub_label2 = 0;
	} else {
	    sub_label2 = TREE_SUB_LABEL(op2);
	}
    } else {
	label2 = 0;
	sub_label2 = 0;
    }

    if (label1 == label2) {
	TREE_LABEL(t) = label1 + 1;
    } else {
	TREE_LABEL(t) = max(label1, label2);
    }
    if (sub_label1 == sub_label2) {
	TREE_SUB_LABEL(t) = sub_label1 + 1;
    } else {
	TREE_SUB_LABEL(t) = max(sub_label1, sub_label2);
    }

    if (TREE_LABEL(t) > big_label) {
	big_label = TREE_LABEL(t);
    }
    if (TREE_LABEL(t) == 0) {
	error("Expression has too many subexpressions", NULL_CHAR,
	      NULL_CHAR);
    }
}

/* Build a binary operation.  Also determine if this operation will
   require another stack entry.  Later, we'll try to be smart about
   applying associativity and stack swapping to reduce the number of
   stack levels required (and speed up operation).  Also determine
   depth of stack for a subexpression using the same expression
   bit lengths (self-determining operators, like conditionals will
   cause a possible bit-length change).
*/

tree build_binary_op(enum tree_code code, tree op1, tree op2)
{
    tree t = build_nt(code, op1, op2);

    fixup_labels(t, op1, op2);

    /* Handle default conversion when going from REAL to any other type;
       that is, convert to real type if another real is there */
    if (TREE_REAL_ATTR(op1) ^ TREE_REAL_ATTR(op2)) {
	if (TREE_REAL_ATTR(op1)) {
	    TREE_OPERAND(t, 1) = build_unary_op(REAL_CONV_EXPR, op2);
	} else {
	    TREE_OPERAND(t, 0) = build_unary_op(REAL_CONV_EXPR, op1);
	}
    }

    TREE_CONSTANT_ATTR(t) = TREE_CONSTANT_ATTR(op1) &
	TREE_CONSTANT_ATTR(op2);
    TREE_INTEGER_ATTR(t) = TREE_INTEGER_ATTR(op1) & TREE_INTEGER_ATTR(op2);
    TREE_REAL_ATTR(t) = TREE_REAL_ATTR(op1) | TREE_REAL_ATTR(op2);
    if (tree_code_type[code][0] == 'e' && tree_code_type[code][1] == '1') {
	TREE_INTEGER_ATTR(t) = 0;
	TREE_REAL_ATTR(t) = 0;
    }

    return t;
}

tree build_unary_op(enum tree_code code, tree op)
{
    tree t = build_nt(code, op);

    TREE_LABEL(t) = TREE_LABEL(op);
    TREE_SUB_LABEL(t) = TREE_SUB_LABEL(op);
    TREE_CONSTANT_ATTR(t) = TREE_CONSTANT_ATTR(op);
    TREE_INTEGER_ATTR(t) = TREE_INTEGER_ATTR(op);
    TREE_REAL_ATTR(t) = TREE_REAL_ATTR(op);

    /* force type changes for conversion */
    if (TREE_CODE(t) == INT_CONV_EXPR) {
	TREE_REAL_ATTR(t) = 0;
	TREE_INTEGER_ATTR(t) = 1;
    } else if (TREE_CODE(t) == REAL_CONV_EXPR) {
	TREE_REAL_ATTR(t) = 1;
	TREE_INTEGER_ATTR(t) = 0;
    }
    return t;
}

tree build_cond_expr(tree op1, tree op2, tree op3)
{
    tree t = make_node(COND_EXPR);

    COND_EXPR(t) = op1;
    COND_TRUE(t) = op2;
    COND_FALSE(t) = op3;
    TREE_CONSTANT_ATTR(t) = TREE_CONSTANT_ATTR(op1) &
	TREE_CONSTANT_ATTR(op2) & TREE_CONSTANT_ATTR(op3);
    TREE_INTEGER_ATTR(t) = TREE_INTEGER_ATTR(op2) & TREE_INTEGER_ATTR(op3);
    TREE_REAL_ATTR(t) = TREE_REAL_ATTR(op2) | TREE_REAL_ATTR(op3);

    /* Handle default conversion when going from REAL to any other type */
    if (TREE_REAL_ATTR(op2) ^ TREE_REAL_ATTR(op3)) {
	if (TREE_REAL_ATTR(op2)) {
	    op3 = build_unary_op(REAL_CONV_EXPR, op3);
	} else {
	    op2 = build_unary_op(REAL_CONV_EXPR, op2);
	}
    }
    TREE_LABEL(t) = 1 + max(TREE_LABEL(op2), TREE_LABEL(op3));
    TREE_SUB_LABEL(t) = 1 + max(TREE_SUB_LABEL(op2), TREE_SUB_LABEL(op3));
    if (TREE_LABEL(t) > big_label) {
	big_label = TREE_LABEL(t);
    }
    if (TREE_LABEL(t) == 0) {
	error("Expression has too many subexpressions", NULL_CHAR,
	      NULL_CHAR);
    }
    return t;
}

/* Make the label of a concat ref the highest of he labels of its
   components.  Also, if all of its components are constant, then make
   the reference constant. */

void concat_labels(tree node)
{
    tree t;
    tree t1;

    TREE_CONSTANT_ATTR(node) = 1;
    for (t = CONCAT_LIST(node); t; t = TREE_CHAIN(t)) {
	t1 = TREE_PURPOSE(t);
	if (TREE_LABEL(node) < TREE_LABEL(t1)) {
	    TREE_LABEL(node) = TREE_LABEL(t1);
	}
	if (TREE_SUB_LABEL(node) < TREE_SUB_LABEL(t1)) {
	    TREE_SUB_LABEL(node) = TREE_SUB_LABEL(t1);
	}
	if (!TREE_CONSTANT_ATTR(t1)) {
	    TREE_CONSTANT_ATTR(t) = 0;
	}
    }
}

/* Determine if this is a legal lval, return decl node */

tree check_lval(tree node, enum lval_type lval_type, tree spec)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (node) {
	if ((t = IDENT_CURRENT_DECL(node)) != 0) {
	    if (TREE_CODE(t) == ARRAY_DECL) {
		error("Illegal reference to array '%s'",
		      IDENT(node), NULL_CHAR);
		return error_mark_node;
	    }
	}
    }
    return check_lval_nocheck(node, lval_type, spec);
}

tree check_lval_nocheck(tree node, enum lval_type lval_type, tree spec)
{
    tree t = IDENT_CURRENT_DECL(node);
    enum tree_code code;
    char *type;
    tree tmp;

    if (lval_type == LVAL_GATE) {	// for gates 
	t = node;
    } else {
	t = IDENT_CURRENT_DECL(node);
    }

    if (!t && lval_type != LVAL_NEW_NET && lval_type != LVAL_GATE) {
	if (HIERARCHICAL_ATTR(node)) {
	    return node;
	} else {
	    error("'%s' not declared", IDENT(node), NULL_CHAR);
	    return error_mark_node;
	}
    } else if (t == error_mark_node) {
	return error_mark_node;
	/* Continuous assignment inside net declaration (ex: "wire foo = bar;") */
    } else if (lval_type == LVAL_NEW_NET) {	/* net decl with con't assignment */
	if (check_net(node) == error_mark_node) {
	    error("'%s' previously declared", IDENT(node), NULL_CHAR);
	} else {		/* make net */
	    t = make_decl(node, spec, NULL_TREE, NULL_TREE);
	    BLOCK_DECL(current_scope) =
		chainon(t, BLOCK_DECL(current_scope));
	    NET_DELAY(t) = NULL_TREE;
	    if (TREE_CODE(t) == NET_SCALAR_DECL) {
		tmp = make_node(NET_SCALAR_DECL);
	    } else {
		tmp = make_node(NET_VECTOR_DECL);
	    }
	    DECL_NAME(tmp) = DECL_NAME(t);	/* Do this for trace */
	    TREE_TYPE(tmp) = TREE_TYPE(t);
	    STMT_SURROGATE_ATTR(tmp) = 1;
	    NET_ASSIGN_ATTR(tmp) = 1;
	    DECL_SOURCE_LINE(tmp) = lineno;
	    DECL_SOURCE_FILE(tmp) = input_filename;
	    PORT_OUTPUT_ATTR(tmp) = 0;
	    PORT_INPUT_ATTR(tmp) = 0;
	    PORT_COLLAPSED_ATTR(tmp) = 0;

	    NET_SOURCE(tmp) = NET_SOURCE(t);	/* thread me into source list */
	    NET_SOURCE(t) = tmp;
	    TREE_CHAIN(tmp) = t;	/* point back to original net */
	    return tmp;
	}
    }

    code = TREE_CODE(t);
    type = tree_code_type[code];

    if (*type == 'e' || *type == 'c' || *type == 'b') {
	error("Illegal Lvalue", NULL_CHAR, NULL_CHAR);
	return error_mark_node;
	/* This is an OK non-net, non-continuous assignment */
    } else if (lval_type == LVAL_REG &&
	       code != NET_VECTOR_DECL && code != NET_SCALAR_DECL) {
	return t;
    } else if (lval_type == LVAL_REG_NET) {	/* All types allowed (FORCE stmt) */
	return t;
	/* Net, but not continuous */
    } else if (lval_type == LVAL_REG) {
	error("Lval '%s' cannot be a net", IDENT(node), NULL_CHAR);
	return error_mark_node;
	/* Continuous assignment (ex: "assign foo = bar;") */
    } else if (lval_type == LVAL_NET ||	/* continuous assignment */
	       lval_type == LVAL_GATE) {	/* gates assignment */
	if (code == NET_VECTOR_DECL) {
	    tmp = make_node(NET_VECTOR_DECL);
	} else if (code == NET_SCALAR_DECL) {
	    tmp = make_node(NET_SCALAR_DECL);
	} else {
	    error("Continuous assignment lval '%s' not a net",
		  IDENT(node), NULL_CHAR);
	    return error_mark_node;
	}
	DECL_NAME(tmp) = DECL_NAME(t);
	TREE_TYPE(tmp) = 0;
	STMT_SURROGATE_ATTR(tmp) = 1;
	DECL_SOURCE_LINE(tmp) = lineno;
	DECL_SOURCE_FILE(tmp) = input_filename;
	PORT_OUTPUT_ATTR(tmp) = 0;
	PORT_INPUT_ATTR(tmp) = 0;
	PORT_COLLAPSED_ATTR(tmp) = 0;
	NET_SOURCE(tmp) = NET_SOURCE(t);	/* thread me into source list */
	NET_SOURCE(t) = tmp;
	TREE_CHAIN(tmp) = t;	/* point back to original net */
	REFERENCED_ATTR(t) = 1;
	return tmp;
    } else {			/* Port connections */
	fatal("Shouldn't here for port connections", NULL);
	tmp = copy_node(spec);
	DECL_NAME(tmp) = DECL_NAME(spec);
	TREE_TYPE(tmp) = 0;
	STMT_SURROGATE_ATTR(tmp) = 1;
	DECL_SOURCE_LINE(tmp) = lineno;
	DECL_SOURCE_FILE(tmp) = input_filename;
	PORT_OUTPUT_ATTR(tmp) = 0;
	PORT_INPUT_ATTR(tmp) = 0;
	PORT_COLLAPSED_ATTR(tmp) = 0;
	NET_SOURCE(tmp) = NET_SOURCE(spec);	/* thread me into source list */
	NET_SOURCE(spec) = tmp;
	TREE_CHAIN(tmp) = spec;	/* point back to original net */
	return tmp;
    }
}

/* Add a source to a net.  It should be already qualified as a decl node. */

tree make_net_source(tree decl)
{
    tree tmp = copy_node(decl);
    DECL_NAME(tmp) = DECL_NAME(decl);
    TREE_TYPE(tmp) = 0;
    STMT_SURROGATE_ATTR(tmp) = 1;
    DECL_SOURCE_LINE(tmp) = lineno;
    DECL_SOURCE_FILE(tmp) = input_filename;
    PORT_OUTPUT_ATTR(tmp) = 0;
    PORT_INPUT_ATTR(tmp) = 0;
    PORT_COLLAPSED_ATTR(tmp) = 0;
    NET_SOURCE(tmp) = NET_SOURCE(decl);	/* thread me into source list */
    NET_SOURCE(decl) = tmp;
    TREE_CHAIN(tmp) = decl;	/* point back to original net */
    return tmp;
}

/* Port might be a bit or part select.  Since this is lval, it must be
   a net, so it can't be an array ref.  It is assumed that the port
   is already tested for net-ness. */

tree check_lval_port(tree port)
{
    tree new_t;
    tree t;
    tree t1;
    tree prev;
    tree first;

    switch (TREE_CODE(port)) {
    case BIT_REF:
	new_t = copy_node(port);
	BIT_REF_DECL(new_t) = make_net_source(BIT_REF_DECL(port));
	break;

    case PART_REF:
	new_t = copy_node(port);
	PART_DECL(new_t) = make_net_source(PART_DECL(port));
	break;

	/* If this is a concat, then each component needs to be copied */
    case CONCAT_REF:
	new_t = copy_node(port);
	prev = NULL_TREE;
	first = NULL_TREE;
	for (t = CONCAT_LIST(port); t; t = TREE_CHAIN(t)) {
	    t1 = build_tree_list(check_lval_port
				 (TREE_PURPOSE(t)), NULL_TREE);
	    if (first) {
		TREE_CHAIN(prev) = t1;
	    } else {
		first = t1;
	    }
	    prev = t1;
	}
	CONCAT_LIST(new_t) = first;
	break;

    default:
	new_t = make_net_source(port);
	break;
    }
    return new_t;
}

/* This is to support pass2; set the immediate attr in the right place */

void set_immediate_attr(tree port, int attr)
{
    tree t;

    switch (TREE_CODE(port)) {
    case BIT_REF:
	PORT_IMMEDIATE_ATTR(BIT_REF_DECL(port)) = attr;
	break;

    case PART_REF:
	PORT_IMMEDIATE_ATTR(PART_DECL(port)) = attr;
	break;

	/* If this is a concat, then each component needs to be copied */
    case CONCAT_REF:
	for (t = CONCAT_LIST(port); t; t = TREE_CHAIN(t)) {
	    set_immediate_attr(TREE_PURPOSE(t), attr);
	}
	break;

    default:
	PORT_IMMEDIATE_ATTR(port) = attr;
	break;
    }
}

tree check_rval(tree ident)
{
    extern int in_instantiation;	/* defined in parse.y */
    tree decl = IDENT_CURRENT_DECL(ident);
    enum tree_code code;
    char *type;


    if (!decl) {
	/* There is no decl node if this is hierachical; also, don't complain
	   for systasks -- let them test it themselves in case of forward decls
	   that will not be resolved 'till pass3, like module names */
	if (HIERARCHICAL_ATTR(ident) || in_systask) {
	    return ident;
	} else if (in_instantiation) {
	    return make_default_net(ident);
	} else {
	    error("'%s' not declared", IDENT(ident), NULL_CHAR);
	    return error_mark_node;
	}
    } else if (decl == error_mark_node) {
	return error_mark_node;
    } else if (*tree_code_type[TREE_CODE(decl)] == 'b' && !in_systask) {	/* module block */
	error("'%s' is a module type and is being usd illegally",
	      IDENT(ident), NULL_CHAR);
	return error_mark_node;
    } else if (TREE_CODE(decl) == EVENT_DECL && !in_event) {
	error("'%s' is an EVENT type and is being used illegally",
	      IDENT(ident), NULL_CHAR);
	return error_mark_node;
    } else if (TREE_CODE(decl) == ARRAY_DECL && !in_systask) {
	error("Illegal array reference: '%s'", IDENT(ident), NULL_CHAR);

    } else {
	REFERENCED_ATTR(decl) = 1;
	return decl;
    }

    return error_mark_node;
}

tree check_rval_nocheck(tree ident)
{
    tree decl = IDENT_CURRENT_DECL(ident);

    if (!decl) {
	if (HIERARCHICAL_ATTR(ident)) {
	    return ident;
	} else {
	    error("'%s' not declared", IDENT(ident), NULL_CHAR);
	    return error_mark_node;
	}
    } else if (decl == error_mark_node) {
	return error_mark_node;
    } else if (TREE_CODE(decl) == EVENT_DECL && !in_event) {
	error("'%s' is an EVENT type and is being used illegally",
	      IDENT(ident), NULL_CHAR);
	return error_mark_node;
    } else {
	REFERENCED_ATTR(decl) = 1;
	return decl;
    }
}

tree check_block(tree ident)
{
    tree t = IDENT_CURRENT_DECL(ident);

    if (t) {
	error("Block '%s' previously declared", IDENT(ident), NULL_CHAR);
	return error_mark_node;
    }
    return ident;
}

/* Bit reference (foo [i]) may be a bit select of a reg or net vector or
    may be an array reference.  */

tree build_bit_ref(tree decl, tree index)
{
    tree ident = DECL_NAME(decl);
    tree node;

    if (decl == error_mark_node) {
	return error_mark_node;
    }
    switch (TREE_CODE(decl)) {
    case REG_SCALAR_DECL:
    case NET_SCALAR_DECL:
    case REG_VECTOR_DECL:
    case NET_VECTOR_DECL:
    case INTEGER_DECL:
    case PARAM_DECL:
	node = build_nt(BIT_REF, decl, index, NULL_TREE, ident);
	TREE_LABEL(node) = TREE_LABEL(index);
	TREE_SUB_LABEL(node) = TREE_SUB_LABEL(index);
	return node;
    case (ARRAY_DECL):
	node = build_nt(ARRAY_REF, decl, index, NULL_TREE, ident);
	TREE_LABEL(node) = TREE_LABEL(index);
	TREE_SUB_LABEL(node) = TREE_SUB_LABEL(index);
	TREE_INTEGER_ATTR(node) = TREE_INTEGER_ATTR(decl);
	TREE_REAL_ATTR(node) = TREE_REAL_ATTR(decl);
	return node;
    case IDENTIFIER_NODE:	/* for hierarchical reference. */
	node = build_nt(BIT_REF, decl, index, NULL_TREE, decl);
	HIERARCHICAL_ATTR(node) = 1;
	TREE_LABEL(node) = TREE_LABEL(index);
	TREE_SUB_LABEL(node) = TREE_SUB_LABEL(index);
	return node;
    default:
	error("'%s' is not of a type that supports bit indexing",
	      IDENT(ident), NULL_CHAR);
    }
    return error_mark_node;
}

/* Part reference (foo [2:5]) must be a reg or a net vector.  May also work
    with integers, reals, and parameter, but this needs further investigation.
    msb and lsb trees will already have been tested for constant expressions. */

tree build_part_ref(tree decl, tree msb, tree lsb)
{
    tree tmp;
    tree ident = DECL_NAME(decl);
    if (decl == error_mark_node) {
	return error_mark_node;
    }

    switch (TREE_CODE(decl)) {
    case (REG_VECTOR_DECL):
    case (NET_VECTOR_DECL):
    case INTEGER_DECL:
    case PARAM_DECL:
	tmp = make_node(PART_REF);
	PART_MSB_(tmp) = msb;
	PART_LSB_(tmp) = lsb;
	PART_NAME(tmp) = PART_DECL(tmp) = decl;
	return tmp;
    case (REG_SCALAR_DECL):
    case (NET_SCALAR_DECL):
	error("'%s' is not a vector", IDENT(ident), NULL_CHAR);
	return error_mark_node;
    case (ARRAY_DECL):
	error
	    ("'%s' is an array; it cannot be be referenced as a part-select",
	     IDENT(ident), NULL_CHAR);
	break;
    case IDENTIFIER_NODE:	/* for hierarchical reference */
	tmp = make_node(PART_REF);
	PART_MSB_(tmp) = msb;
	PART_LSB_(tmp) = lsb;
	PART_NAME(tmp) = PART_DECL(tmp) = decl;
	HIERARCHICAL_ATTR(tmp) = 1;
	return tmp;
    default:
	error("'%s' is not of a type that supports part-selects",
	      IDENT(ident), NULL_CHAR);
    }
    return error_mark_node;
}

tree build_function_call(tree ident, tree args)
{
    tree t = IDENT_CURRENT_DECL(ident), tmp;

    if (t) {
	if (DECL_CONTEXT(t) == current_scope &&
	    !(TREE_CODE(t) == BLOCK_DECL &&
	      TREE_CODE(DECL_THREAD(t)) == FUNCTION_BLOCK)) {
	    error("The name '%s' is already used", IDENT(ident),
		  NULL_CHAR);
	    return error_mark_node;
	} else if (t == error_mark_node) {
	    return error_mark_node;
	}
    }
    tmp = build_nt(FUNCTION_REF, IDENTIFIER_POINTER(ident), args,
		   NULL_TREE, NULL_TREE, lineno, input_filename,
		   current_scope);
    TREE_NBITS(tmp) = 1;	/* default; if function not defined */
    return tmp;
}

tree check_task(tree node)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (t && DECL_CONTEXT(t) == current_scope) {
	error("The name '%s' is previously declared", IDENT(node),
	      NULL_CHAR);
	return error_mark_node;
    } else if (t == error_mark_node) {
	return error_mark_node;
    } else {
	return node;
    }
}

tree check_function(tree node)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (t && DECL_CONTEXT(t) == current_scope) {
	error("The name '%s' is already used", IDENT(node), NULL_CHAR);
	return error_mark_node;
    } else if (t == error_mark_node) {
	return error_mark_node;
    } else {
	return node;
    }
}

tree check_reg(tree node)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (t && (DECL_CONTEXT(t) == current_scope)) {
	if (!PORT_INPUT_ATTR(t) && !PORT_OUTPUT_ATTR(t)) {
	    error("The name '%s' has already been declared",
		  IDENT(node), NULL_CHAR);
	    return error_mark_node;
	} else if (PORT_REDEFINED_ATTR(t)) {
	    error("The port '%s' has already been redefined",
		  IDENT(node), NULL_CHAR);
	    return error_mark_node;
	} else if (PORT_INPUT_ATTR(t) &&
		   TREE_CODE(current_scope) == MODULE_BLOCK) {
	    error("Input port '%s' cannot be redefined as a REG",
		  IDENT(node), NULL_CHAR);
	    return error_mark_node;
	} else if (t == error_mark_node) {
	    return error_mark_node;
	}
    }
    return node;
}

tree check_non_reg(tree node)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (t && (DECL_CONTEXT(t) == current_scope)) {
	error("The name '%s' has already been declared",
	      IDENT(node), NULL_CHAR);
	return error_mark_node;
    } else if (t == error_mark_node) {
	return error_mark_node;
    } else {
	return node;
    }
}

tree check_net(tree node)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (t && !PORT_INPUT_ATTR(t) && !PORT_OUTPUT_ATTR(t)) {
	error("The name '%s' has already been declared",
	      IDENT(node), NULL_CHAR);
	return error_mark_node;
    } else if (t && PORT_REDEFINED_ATTR(t)) {
	error("The port '%s' has already been redefined",
	      IDENT(node), NULL_CHAR);
	return error_mark_node;
    } else if (t == error_mark_node) {
	return error_mark_node;
    } else {
	return node;
    }
}

/* Make sure INPUT/OUTPUT/INOUT identifier is legal.  The critera is slighty
   different for modules than for tasks and functions. */

tree check_port(tree node)
{
    tree t = IDENT_CURRENT_DECL(node);

    if (in_tf) {
	if (t && (DECL_CONTEXT(t) == current_scope)) {
	    error("Port '%s' has already been declared", IDENT(node),
		  NULL_CHAR);
	    return error_mark_node;
	}
	/* If this is a module port, search the port 
	 * list to make sure its in there. 
	 */
    } else {
	/* If the ident is listed in the port list, 
	 * then it will either point
	 * to itself or point to a ref to itself. 
	 */
	if (!t) {
	    error("'%s' is not in the port list", IDENT(node), NULL_CHAR);
	    return error_mark_node;
	} else if (root_port_decl(t) != node) {
	    error("'%s' is not in the port list", IDENT(node), NULL_CHAR);
	    return error_mark_node;
	}
	return t;
    }

    return node;
}

tree check_named_event(tree ident)
{
    tree decl = IDENT_CURRENT_DECL(ident);

    if (HIERARCHICAL_ATTR(ident)) {
	return ident;
    } else if (!decl) {
	error("Event '%s' is not defined\n", IDENT(ident), NULL_CHAR);
	return error_mark_node;
    } else if (TREE_CODE(decl) != EVENT_DECL) {
	error("'%s' is not an EVENT type.\n", IDENT(ident), NULL_CHAR);
	return error_mark_node;
    } else {
	return decl;
    }
}

tree double_check_named_event(tree decl)
{
    tree ident = DECL_NAME(decl);

    if (decl == error_mark_node) {
	return decl;
    }

    if (TREE_CODE(decl) != EVENT_DECL) {
	error("'%s' is not an EVENT type", IDENT(ident), NULL_CHAR);
	return error_mark_node;
    } else {
	return decl;
    }
}

/* In pass3, make sure that the referenced ident node points to a block_decl.
   This cannot be checked at parse time because it may be a forward reference.
   Return the pointed-to block if ok, else return error (note that
   if NULL were returned on an error, a second error would be printed
   about the scope not being found.) */

tree double_check_block(tree ident)
{
    tree decl = IDENT_CURRENT_DECL(ident);

    if (TREE_CODE(decl) != BLOCK_DECL) {
	error("Illegal block reference '%s'", IDENT(ident), NULL_CHAR);
	return error_mark_node;
    }
    return DECL_THREAD(decl);
}

/* Insert type conversion instructions if necessary, return new rval tree */

tree implicit_conversion(tree lval, tree rval)
{
    /* test if rval is real but lval is not; convert to integer (bits) */
    if (!TREE_REAL_ATTR(lval) && TREE_REAL_ATTR(rval)) {
	return build_unary_op(INT_CONV_EXPR, rval);
	/* test if rval is not real, but lval is; convert to real */
    } else if (TREE_REAL_ATTR(lval) && !TREE_REAL_ATTR(rval)) {
	return build_unary_op(REAL_CONV_EXPR, rval);
    } else {
	return rval;
    }
}

/*************************************************************
 *	
 * init_check
 *	- initializes global and static variables for check.c
 *
 *************************************************************
 */
void init_check()
{
    big_label = 0;
}
