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

/* TRACE.C - Print statements as they are being executed */

#define TRACE_C

#include <stdio.h>
#include "vtypes.h"
#include "tree.h"
#include "runtime.h"
#include "schedule.h"
#include "io.h"
#include "print.h"
#include "trace.h"

void
trace_result(Group * val, nbits_t print_nbits, nbits_t nbits,
	     int integer_attr)
{
    print_datum(val, print_nbits, nbits, HEX, 0, 0);
    printf_V(", ");
    print_bcd((handle_t) 1, val, nbits, integer_attr, 0);
//  printf_V("\n");
    HEAPTEST("trace_result 2");
}

static void trace_lval(tree lval)
{
    enum tree_code code = TREE_CODE(lval);
    Group g;

    switch (code) {
    case PART_REF:
	printf_V("%s", IDENTIFIER_POINTER(DECL_NAME(PART_DECL(lval))));
	printf_V("[");
	g.bit.aval = PART_MSB(lval);
	g.bit.bval = 0;
	/* Since we don't keep around the code for the index epression,
	   we don't have the actual nbits if the expression is a since
	   hierarchical name.  Use 32 bits in that case (the default size
	   for hierarchical names). */
	trace_result(&g, TREE_NBITS(PART_MSB_(lval)),
		     TREE_NBITS(PART_MSB_(lval)),
		     TREE_INTEGER_ATTR(PART_MSB_(lval)));
	printf_V(": ");
	g.bit.aval = PART_LSB(lval);
	trace_result(&g, TREE_NBITS(PART_LSB_(lval)),
		     TREE_NBITS(PART_LSB_(lval)),
		     TREE_INTEGER_ATTR(PART_LSB_(lval)));
	printf_V("]");
	break;

    default:
	print_expr(lval);
    }
}

static void
trace_assign_result(tree lval, Group * val, nbits_t nbits,
		    int integer_attr)
{
    trace_lval(lval);
    printf_V(" = ");
//  trace_result (val, R_nbits, nbits, integer_attr);
    if (TREE_REAL_ATTR(lval))
	printf_V("%lg", REAL_(val));
    else
	trace_result(val, nbits, nbits, integer_attr);
    printf_V(";\n");
}

void trace_net_type(enum tree_type type)
{
    switch (type) {
    case NET_WIRE_TYPE:
	printf_V("WIRE ");
	break;
    case NET_TRI_TYPE:
	printf_V("TRI ");
	break;
    case NET_WAND_TYPE:
	printf_V("WAND ");
	break;
    case NET_TRIAND_TYPE:
	printf_V("TRIAND ");
	break;
    case NET_WOR_TYPE:
	printf_V("WOR ");
	break;
    case NET_TRIOR_TYPE:
	printf_V("TRIOR ");
	break;
    case NET_TRI0_TYPE:
	printf_V("TRI0 ");
	break;
    case NET_TRI1_TYPE:
	printf_V("TRI1 ");
	break;
    case (int) NULL:
	printf_V("ASSIGN ");
	break;
    default:
	printf_V("(NOT KNOWN) ");
	break;
    }
}

static tree current_trace_scope = NULL_TREE;

void trace_stmt(tree node, Group * g, int skip, int val)
{
    tree lval, tmp_tree;
    extern int in_simulation;
    enum tree_code code = TREE_CODE(node);
    tree scope;

    if (!in_simulation)
	return;

    if (code == FUNCTION_REF) {
	printf_V("L%d ", FUNC_REF_LINE(node));
	printf_V("\"%s\" ", FUNC_REF_FILE(node));
    } else {
	printf_V("L%d ", STMT_SOURCE_LINE(node));
	printf_V("\"%s\" ", STMT_SOURCE_FILE(node));
    }

    if (*tree_code_type[code] == 's') {
	scope = STMT_SCOPE(node);
	printf_V("(");
	print_scope((handle_t) 1, scope);
	printf_V(")");
    } else if (*tree_code_type[code] == 'g') {
	scope = GATE_SCOPE(node);
	printf_V("(");
	print_scope((handle_t) 1, scope);
	printf_V(")");
    } else {
	printf_V("(");
	if (*tree_code_type[code] == 'd') {
	    scope = DECL_CONTEXT(node);
	    print_scope((handle_t) 1, DECL_CONTEXT(node));
	} else if (code == FUNCTION_REF) {
	    scope = FUNC_REF_CONTEXT(node);
	    print_scope((handle_t) 1, FUNC_REF_CONTEXT(node));
	} else {		/* scopying blocks get here (eg. task_block upon return from task) */

	    scope = node;
	    print_scope((handle_t) 1, node);
	}
	printf_V(")");
    }

/*  Work on this later...
  if (scope != current_trace_scope)
  {
    printf_V ("*** New Scope: ");
    print_scope ((handle_t) 1, scope);
    printf_V ("\n");
    current_trace_scope = scope;
  }
*/
//  if (BLOCK_UP (readylist -> current_scope)) {
    printf_V(": ");
    {
	HEAPTEST("Heap Corrupted (Detected in Trace)");
    }
    switch (code) {
    case ALWAYS_BLOCK:
    case INITIAL_BLOCK:
    case BEGIN_STMT:
    case END_STMT:
    case BEGIN_NAMED_STMT:
    case END_NAMED_STMT:
    case FORK_STMT:
    case JOIN_STMT:
    case FOREVER_STMT:
    case ARROW_STMT:
    case TASK_STMT:
    case SYSTASK_STMT:
    case FUNCTION_REF:
    case DISABLE_STMT:
    case DEASSIGN_STMT:
    case RELEASE_STMT:
    case NULL_STMT:
	print_node(node);
	printf_V("\n");
	break;

    case WHILE_STMT:
	print_node(node);
	printf_V(" >> ");
	if (skip)
	    printf_V("FALSE\n");
	else
	    printf_V("TRUE\n");
	break;

    case (IF_STMT):
	print_node(node);
	printf_V(" >>> ");
	if (skip)
	    printf_V("skipping\n");
	else
	    printf_V("continue\n");
	break;

    case REPEAT_STMT:
    case REPEAT_INIT_STMT:
	print_node(node);
	printf_V(" >>> ");
	tmp_tree =
	    H_(*STMT_REPEAT_EXPR_CODE(node), STMT_REPEAT_EXPR(node));
	trace_result(g, TREE_NBITS(tmp_tree), TREE_NBITS(tmp_tree),
		     TREE_INTEGER_ATTR(tmp_tree));
	printf_V("\n");
	break;

    case CASE_STMT:
    case CASEX_STMT:
    case CASEZ_STMT:
	print_node(node);
	printf_V(" >>> case (");
	trace_result(g, R_nbits, R_nbits,
		     TREE_INTEGER_ATTR(STMT_CASE_EXPR(node)));
	printf_V(")\n");
	break;

    case ASSIGN_STMT:
    case ASSIGN_PROC_STMT:
    case FORCE_STMT:
	print_node(node);
	if (!TREE_INTEGER_ATTR(STMT_ASSIGN_RVAL(node))
	    || *tree_code_type[TREE_CODE(STMT_ASSIGN_RVAL(node))] != 'c') {
	    printf_V(" >>> ");
	    tmp_tree =
		H_(*STMT_ASSIGN_RVAL_CODE(node), STMT_ASSIGN_RVAL(node));

	    trace_assign_result(STMT_ASSIGN_LVAL(node), g,
				TREE_NBITS(tmp_tree),
				TREE_INTEGER_ATTR(tmp_tree));
	} else
	    printf_V("\n");
	break;

    case ASSIGN_DELAY_STMT:
    case ASSIGN_EVENT_STMT:
	print_node(node);
	printf_V(" >>> ");

	if (!val) {		/* after the event is triggered */
	    if (!TREE_INTEGER_ATTR(STMT_ASSIGN_RVAL(node))
		|| *tree_code_type[TREE_CODE(STMT_ASSIGN_RVAL(node))] !=
		'c') {


		tmp_tree =
		    H_(*STMT_ASSIGN_RVAL_CODE(node),
		       STMT_ASSIGN_RVAL(node));
		trace_assign_result(STMT_ASSIGN_LVAL(node), g,
				    TREE_NBITS(tmp_tree),
				    TREE_INTEGER_ATTR(tmp_tree));
	    } else
		printf_V("\n");
	} else {
	    if (code == ASSIGN_DELAY_STMT) {
		printf_V(" #(");
		trace_result(g, R_nbits, R_nbits,
			     TREE_INTEGER_ATTR(STMT_DELAY_EXPR(node)));
		printf_V(") ");
	    } else {
		printf_V(" @() ");
	    }
	    tmp_tree =
		H_(*STMT_ASSIGN_RVAL_CODE(node), STMT_ASSIGN_RVAL(node));
	    trace_assign_result(STMT_ASSIGN_LVAL(node),
				DECL_STORAGE(STMT_ASSIGN_TMP(node)),
				TREE_NBITS(tmp_tree),
				TREE_INTEGER_ATTR(tmp_tree));
	}
	break;

    case ASSIGN_NONBLK_DELAY_STMT:
    case ASSIGN_NONBLK_EVENT_STMT:
	print_node(node);

	if (!val) {		/* after the event is triggered */
	    if (!TREE_INTEGER_ATTR(STMT_ASSIGN_RVAL(node))
		|| *tree_code_type[TREE_CODE(STMT_ASSIGN_RVAL(node))] !=
		'c') {

		printf_V(" >>> ");

		tmp_tree =
		    H_(*STMT_ASSIGN_RVAL_CODE(node),
		       STMT_ASSIGN_RVAL(node));
		trace_assign_result(STMT_ASSIGN_LVAL(node), g,
				    TREE_NBITS(tmp_tree),
				    TREE_INTEGER_ATTR(tmp_tree));
	    } else
		printf_V("\n");
	} else {
	    printf_V(" >>> ");

	    tmp_tree =
		H_(*STMT_ASSIGN_RVAL_CODE(node), STMT_ASSIGN_RVAL(node));
	    if (code == ASSIGN_NONBLK_DELAY_STMT) {
		printf_V(" #(");
		trace_result(g, R_nbits, R_nbits,
			     TREE_INTEGER_ATTR(STMT_DELAY_EXPR
					       (STMT_ASSIGN_DELAY(node))));
		printf_V(") ");
		trace_assign_result(STMT_ASSIGN_LVAL(node),
				    DECL_STORAGE(((SCB *)
						  STMT_ASSIGN_TMP(node))->
						 here.decl),
//      DECL_STORAGE ((tree) ((SCB *) STMT_ASSIGN_TMP (node))->fork),
				    TREE_NBITS(tmp_tree),
				    TREE_INTEGER_ATTR(tmp_tree));
	    } else {
		printf_V(" @() ");
		trace_assign_result(STMT_ASSIGN_LVAL(node),
				    DECL_STORAGE(STMT_ASSIGN_TMP(node)),
//      DECL_STORAGE ((tree) ((SCB *) STMT_ASSIGN_TMP (node))->fork),
				    TREE_NBITS(tmp_tree),
				    TREE_INTEGER_ATTR(tmp_tree));
	    }
	}
	break;

    case NET_VECTOR_DECL:
    case NET_SCALAR_DECL:
	if (!val) {
//  printf_V ("(port propagation (down))\n");
//  return;
	}

	trace_net_type((enum tree_type) TREE_TYPE(node));
	print_expr(node);
	printf_V(" >>> NET = ");
	trace_result(g, TREE_NBITS(node), TREE_NBITS(node), 0);
//      trace_result (g, TREE_NBITS (node), R_nbits, 0);
//      trace_assign_result (lval, g, R_nbits, 0);
	printf_V("\n");
	break;

    case GATE_INSTANCE:
	printf_V("GATE >>> 1'b");
	switch (GATE_OUTPUT(node)) {
	case ONE:
	    printf_V("1");
	    break;
	case ZERO:
	    printf_V("0");
	    break;
	case X:
	    printf_V("x");
	    break;
	case Z:
	    printf_V("z");
	    break;
	}

	printf_V("\n");
	break;

    case ASSIGN_CONT_STMT:
	if (TREE_CHAIN(node) &&
	    TREE_CODE(TREE_CHAIN(node)) == ASSIGN_CONT_STMT)
	    lval = STMT_ASSIGN_LVAL(TREE_CHAIN(node));
	else
	    lval = STMT_ASSIGN_LVAL(node);
	/* first part of statement (with most info) is pointed to by TREE_CHAIN */

	trace_net_type((enum tree_type) TREE_TYPE(lval));
	print_delay(STMT_ASSIGN_DELAY(node));
//      printf_V (" ");
	print_expr(lval);
//      printf_V (" = ");
//      if (STMT_ASSIGN_RVAL (node))
//      trace_expr (STMT_ASSIGN_RVAL (node));
	printf_V(" >>> ");
	trace_assign_result(lval, g, R_nbits,
			    TREE_INTEGER_ATTR(STMT_ASSIGN_RVAL(node)));
	break;

    case (FOR_STMT):
	print_node(node);
	printf_V(") >>> ");
	if (val) {
	    trace_assign_result(STMT_ASSIGN_LVAL(STMT_FOR_ASSIGN(node)), g,
				R_nbits,
				TREE_INTEGER_ATTR(STMT_ASSIGN_RVAL
						  (STMT_FOR_ASSIGN
						   (node))));
	} else if (skip)
	    printf_V("TRUE\n");
	else
	    printf_V("FALSE\n");
	break;

    case DELAY_STMT:
	print_node(node);
	if (val) {
	    if (!TREE_INTEGER_ATTR(STMT_DELAY_EXPR(node))) {
		printf_V(" >>> #");
		trace_result(g, R_nbits, R_nbits,
			     TREE_INTEGER_ATTR(STMT_DELAY_EXPR(node)));
	    }
	} else
	    printf_V(" >>> CONTINUE");
	printf_V("\n");
	break;

    case WAIT_STMT:
	print_node(node);
	printf_V(" >>> ");
	if (skip)
	    printf_V("CONTINUE\n");
	else
	    printf_V("WAIT\n");
	break;

    case EVENT_STMT:
	print_node(node);
	if (skip)
	    printf_V(" >>> CONTINUE\n");
	else
	    printf_V("\n");
	break;

    case TASK_BLOCK:
    case FUNCTION_BLOCK:
//      print_node (node);
	printf_V("%s; >>> RETURNING\n", IDENT(BLOCK_NAME(node)));
	break;

    default:
	printf_V("Statment not known to trace: %s\n",
		 tree_code_name[code]);
	break;
    }
    return;
}

/**********************************************************************
 *
 *	init_trace
 *		- initializes global and static data for trace.c
 *
 **********************************************************************
 */
void init_trace()
{
//  currently no globals to initialize
}
