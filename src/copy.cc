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

/* COPY.C - Copy structures for multiple module instances */

#include <stdio.h>
#include "vtypes.h"
#include "tree.h"
#include "obstack.h"
#include "decl.h"
#include "schedule.h"
#include "scope.h"
#include "io.h"
#include "copy.h"
#include "veriwell.h"

static void copy_assignment(tree dest_node, tree source_node);
static tree copy_delay(tree node);
static tree copy_cont_assigns(tree node);
static tree copy_instances(tree node);
static tree copy_decl(tree t);
static tree copy_decl_chain(tree node, int parameter);
static void copy_decl_defs(tree dest_scope, tree source_scope);
static tree substitute_new_decl(tree old_decl);
static tree copy_list(tree node);
static tree copy_tree(tree node);


extern tree current_scope;
static int in_lval = 0;
static tree current_gate;
static tree current_treelist;

static void copy_assignment(tree dest_node, tree source_node)
{
    enum tree_code code = TREE_CODE(source_node);

    if (code != ASSIGN_CONT_STMT) {
	STMT_ASSIGN_LVAL(dest_node) =
	    copy_tree(STMT_ASSIGN_LVAL(source_node));
	STMT_ASSIGN_RVAL(dest_node) =
	    copy_tree(STMT_ASSIGN_RVAL(source_node));
    }
}

/* Copy delay expressions */

static tree copy_delay(tree node)
{
    tree new_node;
    unsigned int i;

    if (!node) {
	return NULL_TREE;
    }

    new_node = copy_node(node);
    for (i = 0; i < TREE_LABEL(new_node); i++) {
	if (TREE_OPERAND(node, i)) {
	    TREE_OPERAND(new_node, i) = copy_tree(TREE_OPERAND(node, i));
	}
    }
    return new_node;
}

/* Copy continuous assignments.  NOTE: This does not distiguish between 
   continuous assignments and net assignments. */

static tree copy_cont_assigns(tree node)
{
    tree t;
    tree t1;
    tree new_node;
    tree new_node_tree;
    tree first = NULL_TREE;
    tree prev = NULL_TREE;
    tree new_lval;

    for (t1 = node; t1; t1 = TREE_CHAIN(t1)) {
	t = TREE_PURPOSE(t1);
	in_lval = 1;
	if (TREE_CHAIN(t)) {
	    new_lval = copy_tree(STMT_ASSIGN_LVAL(TREE_CHAIN(t)));
	    NET_ASSIGN_ATTR(new_lval) =
		NET_ASSIGN_ATTR(STMT_ASSIGN_LVAL(TREE_CHAIN(t)));
	    TREE_SET_TYPE(new_lval, TREE_TYPE
			  (STMT_ASSIGN_LVAL(TREE_CHAIN(t))));
	} else {
	    new_lval = copy_tree(STMT_ASSIGN_LVAL(t));
	    NET_ASSIGN_ATTR(new_lval) =
		NET_ASSIGN_ATTR(STMT_ASSIGN_LVAL(t));
	    TREE_SET_TYPE(new_lval, TREE_TYPE(STMT_ASSIGN_LVAL(t)));
	}
	in_lval = 0;
	new_node = build_cont_assign(new_lval,
				     copy_tree(STMT_ASSIGN_RVAL(t)),
				     STMT_SOURCE_LINE(t),
				     copy_delay(STMT_ASSIGN_DELAY(t)),
				     STMT_ASSIGN_DELAY(t) != NULL_TREE);
	new_node_tree = build_tree_list(new_node, NULL_TREE);
	if (!first) {
	    first = new_node_tree;
	} else {
	    TREE_CHAIN(prev) = new_node_tree;
	}
	prev = new_node_tree;
    }
    return first;
}

/* Copy INSTANCE_NODEs */

static tree copy_instances(tree node)
{
    tree t;
    tree new_node;
    tree first = NULL_TREE;
    tree prev = NULL_TREE;

    for (t = node; t; t = TREE_CHAIN(t)) {
	new_node = copy_node(t);
	INSTANCE_PORTS(new_node) = copy_tree(INSTANCE_PORTS(t));
	INSTANCE_PARAMS(new_node) = copy_tree(INSTANCE_PORTS(t));
	if (!first) {
	    first = new_node;
	} else {
	    TREE_CHAIN(prev) = new_node;
	}
	TREE_CHAIN(new_node) = NULL_TREE;
	prev = new_node;
    }
    return first;
}

static tree copy_decl(tree t)
{
    tree new_decl;
    enum tree_code new_code;

    new_decl = copy_node(t);
    new_code = TREE_CODE(new_decl);
    if (TREE_CODE(t) == ARRAY_DECL) {
	ARRAY_HI_EXPR(new_decl) = copy_tree(ARRAY_HI_EXPR(t));
	ARRAY_LO_EXPR(new_decl) = copy_tree(ARRAY_LO_EXPR(t));
	if (ARRAY_CODE(new_decl) == REG_VECTOR_DECL) {
	    DECL_MSB(new_decl) = copy_tree(DECL_MSB(t));
	    DECL_LSB(new_decl) = copy_tree(DECL_LSB(t));
	}
    }
    DECL_CONTEXT(new_decl) = current_scope;
    DECL_THREAD(new_decl) = DECL_THREAD(t);
    if (new_code == NET_VECTOR_DECL || new_code == REG_VECTOR_DECL) {
	DECL_MSB(new_decl) = copy_tree(DECL_MSB(t));
	DECL_LSB(new_decl) = copy_tree(DECL_LSB(t));
    }

    if (new_code == NET_VECTOR_DECL || new_code == NET_SCALAR_DECL) {
	NET_SOURCE(new_decl) = NULL_TREE;
	NET_DELAY(new_decl) = copy_tree(NET_DELAY(t));
    } else if (new_code == PARAM_DECL) {
	DECL_PARAM_REDIRECT(new_decl) = NULL_TREE;
	DECL_PARAM_RVAL(new_decl) = copy_tree(DECL_PARAM_RVAL(t));
    }

    set_decl(DECL_NAME(new_decl), new_decl);
    return new_decl;
}

/* scan a chain of decl nodes, make a copy, and set the ident node
   to the new current decl */

static tree copy_decl_chain(tree node, int parameter)
{
    tree t;
    tree new_decl;
    tree prev_decl = NULL_TREE;
    tree first_decl = NULL_TREE;

    for (t = node; t; t = TREE_CHAIN(t)) {
	if (TREE_CODE(t) == BLOCK_DECL) {	/* Don't copy block_decls */
	    continue;
	}
	/* Either copy only parameters or no parameters */
	if (parameter) {
	    if (TREE_CODE(t) != PARAM_DECL) {
		continue;
	    }

	} else {
	    if (TREE_CODE(t) == PARAM_DECL) {
		continue;
	    }
	}
	new_decl = copy_decl(t);
	if (!first_decl) {
	    first_decl = new_decl;
	} else {
	    TREE_CHAIN(prev_decl) = new_decl;
	}
	TREE_CHAIN(new_decl) = NULL_TREE;
	prev_decl = new_decl;
    }
    return first_decl;
}

/* For the given block, copy the port list and the decl list.  Then
   go back and update the threads, since they are forward pointers
   and must be updated in a second pass. */

static void copy_decl_defs(tree dest_scope, tree source_scope)
{
    tree t;

    /* First copy parameters */
    BLOCK_DECL(dest_scope) = NULL_TREE;
    BLOCK_DECL(dest_scope) = copy_decl_chain(BLOCK_DECL(source_scope), 1);

    /* Copy ports first, since some ports are redefined as regs; we want
       IDENT_CURRENT_DECL to reflect this */
    BLOCK_PORTS(dest_scope) =
	copy_decl_chain(BLOCK_PORTS(source_scope), 0);
    BLOCK_DECL(dest_scope) =
	chainon(BLOCK_DECL(dest_scope),
		copy_decl_chain(BLOCK_DECL(source_scope), 0));
    for (t = BLOCK_PORTS(dest_scope); t; t = TREE_CHAIN(t)) {
	if (DECL_THREAD(t)) {
	    DECL_THREAD(t) = substitute_new_decl(DECL_THREAD(t));
	}
    }
    for (t = BLOCK_DECL(dest_scope); t; t = TREE_CHAIN(t)) {
	if (DECL_THREAD(t)) {
	    DECL_THREAD(t) = substitute_new_decl(DECL_THREAD(t));
	}
    }
}

/* Given a reference to a decl of the module to be copied, return the
   pointer of the newly copied decl.  This is accomplished by looking at
   the name of the decl and seeing what the new current decl is, since
   the new current decl is set when the decl was copied.
*/

static tree substitute_new_decl(tree old_decl)
{
    if (!old_decl) {
	return NULL_TREE;
    }
    if (IDENT_CURRENT_DECL(DECL_NAME(old_decl))) {
	return (IDENT_CURRENT_DECL(DECL_NAME(old_decl)));
    } else {
	fatal
	    ("Attempting to copy a decl reference that does not exist: %s",
	     IDENT(DECL_NAME(old_decl)));
    }
    return NULL_TREE;		/* won't get here (put here to avoid warnings */
}

static tree copy_list(tree node)
{
    tree t;
    tree new_t;
    tree first = NULL_TREE;
    tree prev = NULL_TREE;

    for (t = node; t; t = TREE_CHAIN(t)) {
	if (TREE_CODE(t) == TREE_LIST) {
	    new_t = build_tree_list(copy_tree
				    (TREE_PURPOSE(t)),
				    copy_tree(TREE_VALUE(t)));
	} else {
	    new_t = copy_tree(t);
	}
	if (!first) {
	    first = new_t;
	} else {
	    TREE_CHAIN(prev) = new_t;
	}
	prev = new_t;
    }
    return first;
}

tree copy_block(tree node)
{
    enum tree_code code = TREE_CODE(node);
    tree new_node;
    tree up_scope;

    new_node = copy_node(node);

    /* Copy common block stuff */
    BLOCK_UP(new_node) = current_scope;
    BLOCK_NAME(new_node) = BLOCK_NAME(node);
    BLOCK_SOURCE_LINE(new_node) = BLOCK_SOURCE_LINE(node);
    BLOCK_SOURCE_FILE(new_node) = BLOCK_SOURCE_FILE(node);
    make_block_decl(BLOCK_NAME(new_node), current_scope, new_node);
    up_scope = current_scope;
    current_scope = new_node;

    /* Scan Module */

    if (code == MODULE_BLOCK) {
	INITIALIZED_ATTR(new_node) = 0;	/* Allow pass2 to scan this */
	push_scope();
	MODULE_SPECINST(new_node) = NULL_TREE;
	BLOCK_PARAM(new_node) = copy_tree(BLOCK_PARAM(node));
	copy_decl_defs(new_node, node);
	BLOCK_DOWN(new_node) = NULL_TREE;
	BLOCK_BODY(new_node) = copy_tree(BLOCK_BODY(node));
	BLOCK_DOWN(new_node) = chainon(BLOCK_DOWN(new_node),
				       copy_tree(BLOCK_DOWN(node)));
	MODULE_ASSIGNMENTS(new_node) = copy_cont_assigns
	    (MODULE_ASSIGNMENTS(node));
	MODULE_PORT_LIST(new_node) = copy_tree(MODULE_PORT_LIST(node));
	BLOCK_UP(new_node) = NULL_TREE;
    } else if (code == TASK_BLOCK || code == FUNCTION_BLOCK) {
	/* Thread new sub block in */
	push_scope();
	copy_decl_defs(new_node, node);
	DECL_THREAD(IDENT_CURRENT_DECL(BLOCK_NAME(new_node))) = new_node;

	if (code == FUNCTION_BLOCK)
	    FUNCT_DECL(current_scope) = copy_decl(FUNCT_DECL(node));

	BLOCK_BODY(new_node) = copy_tree(BLOCK_BODY(node));
	BLOCK_DOWN(new_node) = copy_tree(BLOCK_DOWN(node));
	BLOCK_PARAM(new_node) = copy_tree(BLOCK_PARAM(node));
    } else if (code == NAMED_BLOCK) {
	tree ident = BLOCK_NAME(new_node);

	/* Thread new sub block in */
	TREE_CHAIN(new_node) = BLOCK_DOWN(up_scope);
	BLOCK_DOWN(up_scope) = new_node;
	push_scope();
	/* Points BLOCK_DECL to block */
	copy_decl_defs(new_node, node);
	DECL_THREAD(IDENT_CURRENT_DECL(ident)) = new_node;
	BLOCK_BODY(new_node) = copy_tree(BLOCK_BODY(node));
	BLOCK_DOWN(new_node) = copy_tree(BLOCK_DOWN(node));
	BLOCK_PARAM(new_node) = copy_tree(BLOCK_PARAM(node));
    }

    current_scope = pop_scope();
    return new_node;
}

/* Make a copy of a TREE LIST and everything under it.  Return a pointer
   to the head of the list.  Note that although a TREE_LIST has three
   operands, the third is always used to store generated code, so it is not
   copied (generated code always occurs in pass3, after the module copy).
*/

static tree copy_tree(tree node)
{
    return copy_tree_with_stuff(node, NULL_TREE);
}

/* recursively copy a node and everything under it.  Return a pointer to
   the head of the newly copied tree.  */

tree copy_tree_with_stuff(tree node, tree stuff)
{
    enum tree_code code;	// = TREE_CODE (node);
    char *tree_type_ptr;	// = tree_code_type [TREE_CODE (node)];
    tree new_node;
    tree prev;
    tree first;
    int lval_save;

    if (!node) {
	return NULL_TREE;
    }
    if ((unsigned long) node == 1) {
	return (tree) 1;
    }
    code = TREE_CODE(node);
    tree_type_ptr = tree_code_type[code];

    switch (*tree_code_type[code]) {
    case 'b':
	/* Only copy one module; don't do sub modules and don't do
	   named blocks (named statement will generate one) */
	prev = first = NULL_TREE;
	for (; node; node = TREE_CHAIN(node)) {
	    code = TREE_CODE(node);
	    if (code != MODULE_BLOCK && code != NAMED_BLOCK) {
		new_node = copy_block(node);
		if (prev) {
		    TREE_CHAIN(prev) = new_node;
		} else {
		    first = new_node;
		}
		prev = new_node;
	    }
	}
	return first;

    case 'e':
	new_node = copy_node(node);
	switch (*++tree_type_ptr) {
	case '3':
	    COND_EXPR(new_node) = copy_tree(COND_EXPR(node));
	    COND_TRUE(new_node) = copy_tree(COND_TRUE(node));
	    COND_FALSE(new_node) = copy_tree(COND_FALSE(node));
	    break;
	case 'x':
	case '1':
	case 's':
	    TREE_OPERAND(new_node, 1) = copy_tree(TREE_OPERAND(node, 1));
	case 'u':
	case 'r':
	default:
	    TREE_OPERAND(new_node, 0) = copy_tree(TREE_OPERAND(node, 0));
	    break;
	}			/* switch 'e' */
	break;

    case 'r':
	new_node = copy_node(node);
	lval_save = in_lval;
	switch (TREE_CODE(new_node)) {
	case BIT_REF:
	    in_lval = 0;
	    BIT_EXPR(new_node) = copy_tree(BIT_EXPR(node));
	    BIT_REF_NAME(new_node) = copy_tree(BIT_REF_NAME(node));
	    in_lval = lval_save;
	    BIT_REF_DECL(new_node) = copy_tree(BIT_REF_DECL(node));
	    break;
	case ARRAY_REF:
	    in_lval = 0;
	    ARRAY_EXPR(new_node) = copy_tree(ARRAY_EXPR(node));
	    ARRAY_REF_NAME(new_node) = copy_tree(ARRAY_REF_NAME(node));
	    in_lval = lval_save;
	    ARRAY_REF_DECL(new_node) = copy_tree(ARRAY_REF_DECL(node));
	    break;
	case CONCAT_REP_REF:
	    CONCAT_EXPR(new_node) = copy_tree(CONCAT_EXPR(node));
	case CONCAT_REF:
	    CONCAT_LIST(new_node) = copy_tree(CONCAT_LIST(node));
	    break;
	case PART_REF:
	    in_lval = 0;
	    PART_MSB_(new_node) = copy_tree(PART_MSB_(node));
	    PART_LSB_(new_node) = copy_tree(PART_LSB_(node));
	    PART_NAME(new_node) = copy_tree(PART_NAME(node));
	    in_lval = lval_save;
	    PART_DECL(new_node) = copy_tree(PART_DECL(node));
	    break;
	case SYSFUNCTION_REF:
	case FUNCTION_REF:
	    FUNC_REF_ARGS(new_node) = copy_tree(FUNC_REF_ARGS(node));
	    FUNC_REF_CONTEXT(new_node) = current_scope;
	    break;
	}			/* switch reference code */
	break;

    case 'd':
	if (code == TMP_DECL) {
	    new_node = copy_node(node);
	} else {
	    new_node = substitute_new_decl(node);
	    if (in_lval) {
		/* Build a new driver for the continuous assignment.  The parent
		   net has already been copied.  Note that net assignments go
		   through here too, but that the NET_ASSIGN_ATTR was already set
		   in substitute_decl_node, as was the type. */
		new_node = check_lval(DECL_NAME(new_node),
				      LVAL_NET, new_node);
		if (current_gate != NULL_TREE) {
//          NET_ASSIGNMENT(  new_node ) = current_gate;
		    NET_ASSIGNMENT(new_node) = current_treelist;
		}
	    }
	}
	break;

    case 'c':
    case 'x':
	if (code == IDENTIFIER_NODE) {
	    new_node = node;
	} else {
	    new_node = copy_node(node);
	}
	if (TREE_CODE(new_node) == TREE_LIST) {
	    current_treelist = new_node;
	    TREE_PURPOSE(new_node) = copy_tree(TREE_PURPOSE(node));
	    if (current_gate != NULL) {
		GATE_TERMINAL_GATE(new_node) = current_gate;
	    } else {
		TREE_VALUE(new_node) = copy_tree(TREE_VALUE(node));
	    }
	    TREE_CHAIN(new_node) = copy_tree(TREE_CHAIN(node));
	}
	break;


    case 'g':
    case 's':
	prev = first = NULL_TREE;
	for (; node; node = TREE_CHAIN(node)) {
	    code = TREE_CODE(node);
	    /* Always and initials are headed by TREE_LISTs; get here if
	       INSTANCE_NODEs are first, otherwise TREE_LIST gets copied 
	       somewhere else. */
	    new_node = copy_node(node);
	    if (prev) {
		TREE_CHAIN(prev) = new_node;
	    }
	    if (!first) {
		first = new_node;
	    }
	    prev = new_node;

	    if (TREE_CODE(node) == TREE_LIST) {
		TREE_PURPOSE(new_node) = copy_tree(TREE_PURPOSE(node));
		TREE_VALUE(new_node) = copy_tree(TREE_VALUE(node));
		continue;
	    } else if (*tree_code_type[TREE_CODE(node)] != 's' &&
		       *tree_code_type[TREE_CODE(node)] != 'g') {
		printf_V("coping non-statement in statement chain");
	    }

	    if (*tree_code_type[TREE_CODE(node)] == 'g') {
		GATE_SCOPE(new_node) = current_scope;
	    } else {
		STMT_SCOPE(new_node) = current_scope;
	    }
	    switch (TREE_CODE(new_node)) {
	    case ALWAYS_BLOCK:
	    case INITIAL_BLOCK:
	    case BEGIN_STMT:
	    case FOREVER_STMT:
	    case DEASSIGN_STMT:
	    case RELEASE_STMT:
	    case ARROW_STMT:
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		break;
	    case BEGIN_NAMED_STMT:
	    case FORK_NAMED_STMT:
		STMT_BLOCK(new_node) = copy_block(STMT_BLOCK(node));
		/* decls have already been copied */
		set_scope(STMT_BLOCK(new_node));
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		current_scope = pop_scope();
		break;
	    case FORK_STMT:
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		break;
	    case END_STMT:
	    case END_NAMED_STMT:
	    case JOIN_STMT:
	    case JOIN_NAMED_STMT:
		break;
	    case ASSIGN_PROC_STMT:
	    case ASSIGN_STMT:
		copy_assignment(new_node, node);
		break;
	    case ASSIGN_DELAY_STMT:
	    case ASSIGN_NONBLK_DELAY_STMT:
		copy_assignment(new_node, node);
		STMT_ASSIGN_DELAY(new_node) =
		    copy_delay(STMT_ASSIGN_DELAY(node));
		break;
	    case ASSIGN_EVENT_STMT:
	    case ASSIGN_NONBLK_EVENT_STMT:
		copy_assignment(new_node, node);
		STMT_ASSIGN_EVENT(new_node) =
		    copy_tree(STMT_ASSIGN_DELAY(node));
		break;
	    case IF_STMT:
		STMT_COND(new_node) = copy_tree(STMT_COND(node));
		STMT_THEN(new_node) = copy_tree(STMT_THEN(node));
		STMT_ELSE(new_node) = copy_tree(STMT_ELSE(node));
		break;
	    case WHILE_STMT:
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		STMT_WHILE_COND(new_node) =
		    copy_tree(STMT_WHILE_COND(node));
		break;
	    case REPEAT_INIT_STMT:
		STMT_REPEAT_EXPR(new_node) =
		    copy_tree(STMT_REPEAT_EXPR(node));
		STMT_BODY(new_node) =
		    copy_tree_with_stuff(STMT_BODY(node), new_node);
		break;
	    case REPEAT_STMT:
		STMT_REPEAT_EXPR(new_node) =
		    copy_tree(STMT_REPEAT_EXPR(node));
		STMT_BODY(new_node) = stuff;
		break;
	    case FOR_STMT:
		if (!STMT_SURROGATE_ATTR(node)) {
		    STMT_FOR_ASSIGN(new_node) =
			copy_tree(STMT_FOR_ASSIGN(node));
		    STMT_FOR_COND(new_node) =
			copy_tree(STMT_FOR_COND(node));
		    STMT_BODY(new_node) =
			copy_tree_with_stuff(STMT_BODY(node), new_node);
		} else {
		    STMT_FOR_ASSIGN(new_node) =
			copy_tree(STMT_FOR_ASSIGN(node));
		    STMT_FOR_COND(new_node) = STMT_FOR_COND(stuff);
		    STMT_BODY(new_node) = STMT_BODY(stuff);
		    TREE_CHAIN(new_node) = stuff;
		    return first;
		}
		break;
	    case CASE_STMT:
	    case CASEX_STMT:
	    case CASEZ_STMT:
		STMT_CASE_EXPR(new_node) = copy_tree(STMT_CASE_EXPR(node));
		STMT_CASE_LIST(new_node) = copy_tree(STMT_CASE_LIST(node));
		STMT_CASE_DEFAULT(new_node) =
		    copy_tree(STMT_CASE_DEFAULT(node));
		break;
	    case DELAY_STMT:
		STMT_DELAY_EXPR(new_node) =
		    copy_tree(STMT_DELAY_EXPR(node));
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		break;
	    case WAIT_STMT:
		STMT_WAIT_EXPR(new_node) = copy_tree(STMT_WAIT_EXPR(node));
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		break;
	    case EVENT_STMT:
		STMT_EVENT_EXPR_LIST(new_node) =
		    copy_tree(STMT_EVENT_EXPR_LIST(node));
		STMT_BODY(new_node) = copy_tree(STMT_BODY(node));
		STMT_EVENT_MARKER(new_node) = NULL;
		break;
	    case FORCE_STMT:
		copy_assignment(new_node, node);
		break;
	    case DISABLE_STMT:
		break;		/* STMT_DISABLE_NAME copied by default */
	    case TASK_STMT:
	    case SYSTASK_STMT:
		STMT_TASK_ARGS(new_node) = copy_tree(STMT_TASK_ARGS(node));
		break;
	    case NULL_STMT:
		break;
	    case INSTANCE_NODE:
		INSTANCE_PORTS(new_node) = copy_tree(INSTANCE_PORTS(node));
		INSTANCE_PARAMS(new_node) =
		    copy_tree(INSTANCE_PARAMS(node));
		break;
	    case GATE_INSTANCE:
		in_lval = 1;
		current_gate = new_node;
		GATE_OUTPUT_LIST(new_node) =
		    copy_tree(GATE_OUTPUT_LIST(node));
		in_lval = 0;
		GATE_INPUT_LIST(new_node) =
		    copy_tree(GATE_INPUT_LIST(node));
		current_gate = NULL;
		GATE_DELAY(new_node) = copy_tree(GATE_DELAY(node));
		break;
	    default:
		fatal("Fatal error: Attempt to copy unknown statement",
		      NULL);
	    }

	}			/* for */
	return first;
	break;

    default:
	if (node == (tree) ALWAYS_CODE) {
	    return (tree) ALWAYS_CODE;
	}
	fatal("Fatal: Unexpected structure encountered"
	      " during module copy", NULL);
    }
    return new_node;
}

/****************************************************************
 *
 *  init_copy
 *	- initialize global and static variables for copy.c
 *
 ****************************************************************
 */

void init_copy()
{
    in_lval = 0;
    current_gate = NULL;
}
