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

/***************************************************************************
 *
 * gates.c
 *   - gate building and execution code
 *
 *	Things not implemented: 
 *	1) bit selects on pullup,pulldown, tran, rtran gates
 *	2) pullup, pulldown, tran, and rtran need strengths to work
 *	3) trireg not supported
 **************************************************************************
 */

#include "glue.h"
#include <stddef.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "runtime.h"
#include "gates.h"
#include "specify.h"
#include "udp.h"
#include "store.h"
#include "pass3.h"
#include "veriwell.h"
#include "eval.h"
#include "scope.h"
#include "nsched.h"



void and_exec(struct Marker *marker);
void nand_exec(struct Marker *marker);
void or_exec(struct Marker *marker);
void nor_exec(struct Marker *marker);
void xor_exec(struct Marker *marker);
void xnor_exec(struct Marker *marker);
void buf_exec(struct Marker *marker);
void not_exec(struct Marker *marker);
void bufif0_exec(struct Marker *marker);
void bufif1_exec(struct Marker *marker);
void notif0_exec(struct Marker *marker);
void notif1_exec(struct Marker *marker);
void nmos_exec(struct Marker *marker);
void pmos_exec(struct Marker *marker);
void rnmos_exec(struct Marker *marker);
void rpmos_exec(struct Marker *marker);
void cmos_exec(struct Marker *marker);
void rcmos_exec(struct Marker *marker);
void tranif0_exec(struct Marker *marker);
void rtranif0_exec(struct Marker *marker);
void tranif1_exec(struct Marker *marker);
void rtranif1_exec(struct Marker *marker);
void tran_exec(struct Marker *marker);
void rtran_exec(struct Marker *marker);
void pull_exec(struct Marker *marker);



/************************************************************************
 *
 *      build_gate_instance
 *              - create and initialize a gate structure 
 *
 ************************************************************************
 */
tree
build_gate_instance(enum tree_type type, char *filename, lineno_t lineno,
		    tree ident, tree arg_list, tree delay, tree udp_def)
{
    tree node;
    int i;
    tree t;
    tree last;
    int numberOfArgs;
    int numberOfOutputs;


    node = make_node(GATE_INSTANCE);
    TREE_TYPE(node) = type;
    GATE_FILE(node) = filename;
    GATE_LINE(node) = lineno;
    if (ident == NULL_TREE) {
	GATE_NAME(node) = NULL;
    } else {
	GATE_NAME(node) = IDENT(ident);
    }
    GATE_DELAY(node) = delay;

    GATE_OUTPUT(node) = X;
    GATE_FIRED_INPUTS(node) = 0;
    GATE_PATH_OUTPUT(node) = NULL_TREE;

    numberOfArgs = list_length(arg_list);
    switch ((int) type) {
    case GATE_UDP_TYPE:
	ASSERT(udp_def != NULL_TREE);
	ASSERT(TREE_CODE(udp_def) == MODULE_BLOCK);
	ASSERT(UDP_ATTR(udp_def));
	GATE_UDP_DEF(node) = udp_def;
	if (list_length(UDP_PORT_LIST(udp_def)) != list_length(arg_list)) {
	    error("Incorrect number of arguments specified for primitive",
		  NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 1;
	break;
    case GATE_AND_TYPE:
    case GATE_NAND_TYPE:
    case GATE_OR_TYPE:
    case GATE_NOR_TYPE:
    case GATE_XOR_TYPE:
    case GATE_XNOR_TYPE:
	if (numberOfArgs < 2) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 64) {
	    error("Too many arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 1;
	break;
    case GATE_BUFIF0_TYPE:
    case GATE_BUFIF1_TYPE:
    case GATE_NOTIF0_TYPE:
    case GATE_NOTIF1_TYPE:
    case GATE_NMOS_TYPE:
    case GATE_PMOS_TYPE:
    case GATE_RNMOS_TYPE:
    case GATE_RPMOS_TYPE:
	if (numberOfArgs < 3) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 3) {
	    error("To many arguments specified for primitive", NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 1;
	break;
    case GATE_BUF_TYPE:
    case GATE_NOT_TYPE:
	if (numberOfArgs < 2) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 9) {
	    error("To many arguments specified for primitive", NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = numberOfArgs - 1;
	break;
    case GATE_CMOS_TYPE:
    case GATE_RCMOS_TYPE:
	if (numberOfArgs < 4) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 4) {
	    error("To many arguments specified for primitive", NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 1;
	break;
    case GATE_PULLUP_TYPE:
    case GATE_PULLDN_TYPE:
	if (numberOfArgs < 1) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 1) {
	    error("To many arguments specified for primitive", NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 1;
	break;
    case GATE_TRAN_TYPE:
    case GATE_RTRAN_TYPE:
	if (numberOfArgs < 2) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 2) {
	    error("To many arguments specified for primitive", NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 2;
	t = arg_list;
	arg_list = tree_cons(TREE_PURPOSE(t), NULL_TREE, arg_list);
	t = TREE_CHAIN(t);
	arg_list = tree_cons(TREE_PURPOSE(t), NULL_TREE, arg_list);
	break;
    case GATE_TRANIF0_TYPE:
    case GATE_TRANIF1_TYPE:
    case GATE_RTRANIF0_TYPE:
    case GATE_RTRANIF1_TYPE:
	if (numberOfArgs < 3) {
	    error("Not enough arguments specified for primitive", NULL,
		  NULL);
	    return (error_mark_node);
	} else if (numberOfArgs > 3) {
	    error("To many arguments specified for primitive", NULL, NULL);
	    return (error_mark_node);
	}
	numberOfOutputs = 2;
	t = arg_list;
	arg_list = tree_cons(TREE_PURPOSE(t), NULL_TREE, arg_list);
	t = TREE_CHAIN(t);
	arg_list = tree_cons(TREE_PURPOSE(t), NULL_TREE, arg_list);
	break;
    default:
	ASSERT(FALSE);
    }

    last = NULL_TREE;
    for (t = arg_list, i = numberOfOutputs; i; i--, t = TREE_CHAIN(t)) {
	TREE_EXPR(t) = check_lval_port(TREE_EXPR(t));
	GATE_TERMINAL_GATE(t) = node;
	switch (TREE_CODE(TREE_EXPR(t))) {
	case NET_SCALAR_DECL:
	    NET_ASSIGNMENT(TREE_EXPR(t)) = t;
	    break;
	case BIT_REF:
	    NET_ASSIGNMENT(BIT_REF_DECL(TREE_EXPR(t))) = t;
	    break;
	}

	last = t;
    }
    ASSERT(last != NULL_TREE);
    TREE_CHAIN(last) = NULL_TREE;

    GATE_OUTPUT_LIST(node) = arg_list;
    GATE_INPUT_LIST(node) = t;
    GATE_INPUTS(node) = list_length(GATE_INPUT_LIST(node));
    GATE_SCOPE(node) = current_scope;

    for (t = GATE_INPUT_LIST(node); t; t = TREE_CHAIN(t)) {
	GATE_TERMINAL_GATE(t) = node;
    }
    return node;
}



/************************************************************************
 *
 *      initializeGates
 *              - propagate gate outputs at initialization
 *
 ************************************************************************
 */
void initialize_gates(void)
{
    tree gate;
    enum logical_value was_output;

    for (gate = PeekGate(); gate; gate = PeekGate()) {
	ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
	RemoveGate(gate);
	was_output = GATE_OUTPUT(gate);
	GATE_OUTPUT(gate) = X;
	(*GATE_PROPAGATE_ROUTINE(gate)) (gate);
	GATE_OUTPUT(gate) = was_output;
	if (was_output != X) {
	    handle_gate(gate);
	}
        /*
         * Create an scb and schedule to run at time 0.
         * Link all inputs on a marker chain to cause
         * all input to reevaluated. Don't do for
         * upd's
         */
    	if( TREE_TYPE(gate) == GATE_UDP_TYPE ) {
	    continue;
	}
	SCB* scb = BuildSCB(gate, NOLIST);
	scb->here.marker = NULL;
        for (tree t = GATE_INPUT_LIST(gate); t; t = TREE_CHAIN(t)) {
 	    Marker* marker = (Marker *) xmalloc(sizeof(Marker));
	    marker->next = scb->here.marker;
	    scb->here.marker = marker;
	    marker->scb = (SCB*)gate;
	    marker->flags = (enum marker_flags) (M_PRIM|M_FIXED);
	    marker->expr.arg = t;
	}
	Schedule(0,scb,0);
    }
}

/************************************************************************
 *
 *      gate_check_delay        
 *              - validate gate delays 
 *
 ************************************************************************
 */

void gate_check_delay(tree gate, unsigned int max_delays)
{
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    ASSERT(max_delays >= 0);

    if (!GATE_DELAY(gate)) {
	return;
    }
    if (!max_delays) {
	error("Delays are not allowed in this type of primitive", NULL,
	      NULL);
    }

    if (TREE_LABEL(GATE_DELAY(gate)) > max_delays) {
	error("Too many delays specified for this type of primitive",
	      NULL, NULL);
    }
    pass3_delay(GATE_DELAY(gate));
}

/************************************************************************
 *
 *      gate_ok_input   
 *              - validate gate input 
 *
 ************************************************************************
 */

int gate_ok_input(tree port)
{
    ASSERT(port != NULL_TREE);

    /* Any port is an ok input */
    return TRUE;
}


/************************************************************************
 *
 *      gate_ok_output  
 *              - validate gate output 
 *
 ************************************************************************
 */


int gate_ok_output(tree port)
{
    ASSERT(port != NULL_TREE);

    /* Gate outputs must be a net and one bit */

    if (is_all_net(port)) {
	if (TREE_NBITS(port) != 1 && TREE_CODE(port) != BIT_REF) {
	    error("Primitive output must be scalar", NULL, NULL);
	    return (FALSE);
	}
	return TRUE;
    }
    error("Primitive output must be a net", NULL, NULL);
    return FALSE;
}

/************************************************************************
 *
 *      gate_check_ports        
 *              - validate gate ports 
 *
 ************************************************************************
 */

void gate_check_ports(tree gate, enum gate_style style)
{
    tree port;
    tree inputPortList = NULL;
    tree outputPortList = NULL;
    tree t;
    int port_count = 0;
    int terminalNumber = 0;
    extern struct Marker_info marker_info;	/* in pass3.c */
    struct Marker *marker = NULL, *mark;
    int first;

    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    inputPortList = GATE_INPUT_LIST(gate);
    outputPortList = GATE_OUTPUT_LIST(gate);
    ASSERT(outputPortList);
    runtime_error(gate);

    for (t = outputPortList; t; t = TREE_CHAIN(t)) {
	port = TREE_EXPR(t);
	ASSERT(port != NULL_TREE);
	gate_ok_output(port);
	TREE_EXPR_CODE(t) = pass3_expr(port);
	GATE_TERMINAL_GATE(t) = gate;
	PORT_OUTPUT_ATTR(t) = 1;
	if (style == PRIM_TRAN) {
	    PORT_INPUT_ATTR(t) = 1;
	}
	GATE_TERMINAL_NUMBER(t) = terminalNumber++;
    }

    for (t = GATE_INPUT_LIST(gate); t; t = TREE_CHAIN(t)) {
	GATE_TERMINAL_GATE(t) = gate;
	if (style != PRIM_TRAN) {
	    PORT_INPUT_ATTR(t) = 1;
	}
	GATE_TERMINAL_NUMBER(t) = terminalNumber++;
	port = TREE_EXPR(t);
	// this is for pli only
	PORT_INPUT_ATTR(t) = 1;
	if (gate_ok_input(port)) {
	    extern struct Marker_info marker_info;
	    SCB *save_scb = marker_info.current_scb;
	    marker_info.current_scb = (SCB *) gate;
	    /* Scan port and generate markers; 
	       make sure makers point back to ports */
	    marker = NULL;
	    GATE_INPUT_NUMBER(t) = port_count;
	    TREE_EXPR_CODE(t) = pass3_expr_marker(port,
						  &marker,
						  (enum
						   marker_flags) (M_PRIM +
								  M_FIXED),
						  NULL_TREE, NULL_TREE);
	    SET_GATE_IN(t, X);
	    marker_info.current_scb = save_scb;

	    /* This is a circular list, so make sure we only go 
	       around once; constants have no markers */
	    for (mark = marker, first = 1;
		 mark && (mark != marker || first == 1);
		 mark = mark->link, first = 0) {
		mark->expr.arg = t;
		/* if single var, no expr, then this can be fast */
		if (port == mark->decl
		    && TREE_CODE(port) == NET_SCALAR_DECL) {
		    mark->flags =
			(enum marker_flags) (M_PRIM_FAST | mark->flags);
		}
	    }
	}
	port_count++;
    }
    GATE_INPUTS(gate) = port_count;
}

/************************************************************************
 *
 *      pass3_gate      
 *              - link and initialize gate 
 *
 ************************************************************************
 */

void pass3_gate(tree gate)
{
    Group *g;
    tree t, arg;
    nbits_t nbits;		/* Used only for _eval call */

    switch (TREE_TYPE(gate)) {
    case GATE_UDP_TYPE:
	GATE_ROUTINE(gate) = &udp_exec;
	gate_check_delay(gate, 2);
	gate_check_ports(gate, PRIM_LOGIC);
	ASSERT(GATE_UDP_DEF(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_UDP_DEF(gate)) == MODULE_BLOCK);
	ASSERT(UDP_ATTR(GATE_UDP_DEF(gate)));
	t = UDP_INITIAL_VALUE(GATE_UDP_DEF(gate));
	if (t && TREE_CODE(t) == INTEGER_CST) {
	    ASSERT((INT_CST_DATA(t) & ~1) == 0);
	    GATE_OUTPUT(gate) = (enum logical_value) (INT_CST_DATA(t) & 1);
	} else if (t && TREE_CODE(t) == BIT_CST) {
	    g = BIT_CST_GROUP(t);
	    GATE_OUTPUT(gate) = VAL_TO_STATE(g);
	} else if (t == NULL_TREE) {
	    GATE_OUTPUT(gate) = X;
	} else {
	    ASSERT(FALSE);
	}
	GATE_PROPAGATE_ROUTINE(gate) = &propagate_unigate_output;
	{
	    int i;
	    int count;
	    int state;
    	    tree udp_def = GATE_UDP_DEF(gate);
    	    ASSERT(udp_def);
    	    ASSERT(TREE_CODE(udp_def) == MODULE_BLOCK);
    	    ASSERT(UDP_ATTR(udp_def));
    	    tree udp_table = UDP_TABLE(udp_def);
    	    ASSERT(udp_table != NULL_TREE);
    	    ASSERT(TREE_CODE(udp_table) == UDP_TABLE_NODE);

	    state = 0;
	    if (UDP_REG_NAME(GATE_UDP_DEF(gate)) != NULL_TREE) {
		switch (GATE_OUTPUT(gate)) {
		case ZERO:
		    state = int_power(3, GATE_INPUTS(gate)) * 0;
		    break;
		case ONE:
		    state = int_power(3, GATE_INPUTS(gate)) * 1;
		    break;
		case X:
		case Z:
		    state = int_power(3, GATE_INPUTS(gate)) * 2;
		    break;
		}
	    }

	    /* Evaluate each argument for inital condition; 
	       there may be a constant in
	       one or more of the inputs, so don't assume X. */
	    stack_allocate();	/* Let eval_ work */
	    for (arg = GATE_INPUT_LIST(gate); arg; arg = TREE_CHAIN(arg)) {
		    state += int_power(3, GATE_INPUT_NUMBER(arg)) * 2;
	    }
	    GATE_STATE(gate) = state;
	    for (arg = GATE_INPUT_LIST(gate); arg; arg = TREE_CHAIN(arg)) {
    		int arg_number = GATE_INPUT_NUMBER(arg);
    	    	char* table = UDP_TABLE_ARRAY(udp_table, arg_number);
    	    	ASSERT(table != NULL);
		g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
		enum logical_value in_new = VAL_TO_STATE(g);
		enum logical_value in_old = GATE_IN(arg);
		enum logical_value out_old = GATE_OUTPUT(gate);
		enum logical_value out_new;
		ASSERT( in_old == X );
		if( in_new == Z ) {
		    in_new = X;
		}
	        if( in_new == in_old ) {
		    continue;
		}
    		/* calculate new output value */
    		state += deltaTable[arg_number][in_old][in_new];
    		SET_GATE_IN(arg, in_new);
    		switch (out_old) {
    		case ZERO:
		        out_new = (enum logical_value) 
						(table[state] & 0x3);
			break;
    		case ONE:
			out_new = (enum logical_value) 
						((table[state] >> 2) & 0x3);
			break;
    		case X:
			out_new = (enum logical_value) 
					((table[state] >> 4) & 0x3);
			break;
    		case Z:
    		default:
			ASSERT(FALSE);
    		}
		if( out_old != out_new &&
		    UDP_REG_NAME(udp_def) != NULL_TREE) {
	        } else {
		    GATE_OUTPUT(gate) = out_new;
		}
	        GATE_STATE(gate) = state;
	    }


	}
	break;

    case GATE_AND_TYPE:
	GATE_ROUTINE(gate) = &and_exec;
	goto logic_common;

    case GATE_NAND_TYPE:
	GATE_ROUTINE(gate) = &nand_exec;
	goto logic_common;

    case GATE_OR_TYPE:
	GATE_ROUTINE(gate) = &or_exec;
	goto logic_common;

    case GATE_NOR_TYPE:
	GATE_ROUTINE(gate) = &nor_exec;
	goto logic_common;

    case GATE_XOR_TYPE:
	GATE_ROUTINE(gate) = &xor_exec;
	goto logic_common;

    case GATE_XNOR_TYPE:
	GATE_ROUTINE(gate) = &xnor_exec;
	goto logic_common;


      logic_common:
	gate_check_delay(gate, 2);
	gate_check_ports(gate, PRIM_LOGIC);
	GATE_X(gate) = GATE_INPUTS(gate);
	GATE_FIRED_INPUTS(gate) = 0;
	GATE_PROPAGATE_ROUTINE(gate) = &propagate_unigate_output;
	break;

    case GATE_BUF_TYPE:
	GATE_ROUTINE(gate) = &buf_exec;
	goto buf_common;

    case GATE_NOT_TYPE:
	GATE_ROUTINE(gate) = &not_exec;
	goto buf_common;

      buf_common:
	gate_check_delay(gate, 2);
	gate_check_ports(gate, PRIM_BUF);
	GATE_PROPAGATE_ROUTINE(gate) = &propagate_unigate_output;
	break;

    case GATE_BUFIF0_TYPE:
	GATE_ROUTINE(gate) = &bufif0_exec;
	goto tri_common;

    case GATE_BUFIF1_TYPE:
	GATE_ROUTINE(gate) = &bufif1_exec;
	goto tri_common;

    case GATE_NOTIF0_TYPE:
	GATE_ROUTINE(gate) = &notif0_exec;
	goto tri_common;

    case GATE_NOTIF1_TYPE:
	GATE_ROUTINE(gate) = &notif1_exec;
	goto tri_common;

    case GATE_NMOS_TYPE:
	GATE_ROUTINE(gate) = &nmos_exec;
	goto tri_common;

    case GATE_PMOS_TYPE:
	GATE_ROUTINE(gate) = &pmos_exec;
	goto tri_common;

    case GATE_RNMOS_TYPE:
	GATE_ROUTINE(gate) = &rnmos_exec;
	goto tri_common;

    case GATE_RPMOS_TYPE:
	GATE_ROUTINE(gate) = &rpmos_exec;
	goto tri_common;

      tri_common:
	gate_check_delay(gate, 3);
	gate_check_ports(gate, PRIM_TRI);
	GATE_PROPAGATE_ROUTINE(gate) = &propagate_unigate_output;
	break;

    case GATE_CMOS_TYPE:
	GATE_ROUTINE(gate) = &cmos_exec;
	goto cmos_common;

    case GATE_RCMOS_TYPE:
	GATE_ROUTINE(gate) = &rcmos_exec;
	goto cmos_common;

      cmos_common:
	gate_check_delay(gate, 3);
	gate_check_ports(gate, PRIM_CMOS);
	GATE_PROPAGATE_ROUTINE(gate) = &propagate_unigate_output;
	break;

    case GATE_TRAN_TYPE:
	GATE_ROUTINE(gate) = &tran_exec;
	gate_check_delay(gate, 0);
	GATE_CONTROL(gate) = ONE;
	goto tran_common;

    case GATE_RTRAN_TYPE:
	GATE_ROUTINE(gate) = &rtran_exec;
	gate_check_delay(gate, 0);
	GATE_CONTROL(gate) = ONE;
	goto tran_common;

    case GATE_TRANIF0_TYPE:
	gate_check_delay(gate, 2);
	GATE_ROUTINE(gate) = &tranif0_exec;
	GATE_CONTROL(gate) = ZERO;
	goto tran_common;

    case GATE_TRANIF1_TYPE:
	gate_check_delay(gate, 2);
	GATE_ROUTINE(gate) = &tranif1_exec;
	GATE_CONTROL(gate) = ZERO;
	goto tran_common;

    case GATE_RTRANIF0_TYPE:
	gate_check_delay(gate, 2);
	GATE_ROUTINE(gate) = &rtranif0_exec;
	GATE_CONTROL(gate) = ZERO;
	goto tran_common;

    case GATE_RTRANIF1_TYPE:
	gate_check_delay(gate, 2);
	GATE_ROUTINE(gate) = &rtranif1_exec;
	GATE_CONTROL(gate) = ZERO;
	goto tran_common;


      tran_common:
	gate_check_ports(gate, PRIM_TRAN);
	GATE_PROPAGATE_ROUTINE(gate) = &propagate_bigate_output;
	break;

    case GATE_PULLUP_TYPE:
	GATE_INPUTS(gate) = 0;
	GATE_ROUTINE(gate) = NULL;
	GATE_OUTPUT(gate) = ONE;
	goto pull_common;

    case GATE_PULLDN_TYPE:
	GATE_INPUTS(gate) = 0;
	GATE_ROUTINE(gate) = NULL;
	GATE_OUTPUT(gate) = ZERO;
	goto pull_common;

      pull_common:
	gate_check_delay(gate, 0);
	gate_check_ports(gate, PRIM_PULL);
	GATE_PROPAGATE_ROUTINE(gate) = propagate_unigate_output;
	break;

    default:
	ASSERT(FALSE);
    }


    /* build bit ref lval if needed */

    for (t = GATE_OUTPUT_LIST(gate); t != NULL_TREE; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == TREE_LIST);
	if (TREE_CODE(TREE_PURPOSE(t)) == BIT_REF) {
	    pass3_lval(&TREE_PURPOSE(t));
	}
    }

    /* propagate the gate output at beginning of simulation */

    ScheduleGate(gate, 0);
}

/* Evaluate the gates in the following manner:
     1) Determine the value of the input and whether it changed...
        case 1: input is a net; new value is propigated through marker
        case 2: input is expression; evaluate expression and determine if
                  input changed.
     2) Update ONE(ZERO) count and X count 
     3) Determine new output
     4) If input has not changed or output has not changed, exit.
*/

/***********************************************************************
 *
 *      and_exec
 *              - execution routine for and gates
 *
 ***********************************************************************
 */

void and_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;
    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

    /* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

    /* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

    /* First, evaluate the active input, if necessary.  If the following is
       true, then decl is the only argument and no need
       reevaluate arg expression.  If is isn't true, then decl is only
       one component of the argument expression, so arg needs to be
       evaluated.
     */

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* update value counts */

    switch (in_old) {
    case X:
    case Z:
	GATE_X(gate)--;
	break;
    case ZERO:
	GATE_ZEROS(gate)--;
	break;
    default:
	break;
    }

    switch (in_new) {
    case X:
    case Z:
	GATE_X(gate)++;
	break;
    case ZERO:
	GATE_ZEROS(gate)++;
	break;
    default:
	break;
    }

    if (GATE_ZEROS(gate) == 0) {
	if (GATE_X(gate) == 0) {
	    out_new = ONE;
	} else {
	    out_new = X;
	}
    } else {
	out_new = ZERO;
    }

    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      nand_exec
 *              - execution routine for nand gates
 *
 ***********************************************************************
 */

void nand_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;
    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* update value counts */

    switch (in_old) {
    case X:
    case Z:
	GATE_X(gate)--;
	break;
    case ZERO:
	GATE_ZEROS(gate)--;
	break;
    default:
	break;
    }

    switch (in_new) {
    case X:
    case Z:
	GATE_X(gate)++;
	break;
    case ZERO:
	GATE_ZEROS(gate)++;
	break;
    default:
	break;
    }

    if (GATE_ZEROS(gate) == 0) {
	if (GATE_X(gate) == 0) {
	    out_new = ZERO;
	} else {
	    out_new = X;
	}
    } else {
	out_new = ONE;
    }

    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      or_exec
 *              - execution routine for or gates
 *
 ***********************************************************************
 */

void or_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* update value counts */

    switch (in_old) {
    case X:
    case Z:
	GATE_X(gate)--;
	break;
    case ONE:
	GATE_ONES(gate)--;
	break;
    default:
	break;
    }

    switch (in_new) {
    case X:
    case Z:
	GATE_X(gate)++;
	break;
    case ONE:
	GATE_ONES(gate)++;
	break;
    default:
	break;
    }

    if (GATE_ONES(gate) == 0) {
	if (GATE_X(gate) == 0) {
	    out_new = ZERO;
	} else {
	    out_new = X;
	}
    } else {
	out_new = ONE;
    }

    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      nor_exec
 *              - execution routine for nor gates
 *
 ***********************************************************************
 */

void nor_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* update value counts */

    switch (in_old) {
    case X:
    case Z:
	GATE_X(gate)--;
	break;
    case ONE:
	GATE_ONES(gate)--;
	break;
    default:
	break;
    }

    switch (in_new) {
    case X:
    case Z:
	GATE_X(gate)++;
	break;
    case ONE:
	GATE_ONES(gate)++;
	break;
    default:
	break;
    }

    if (GATE_ONES(gate) == 0) {
	if (GATE_X(gate) == 0) {
	    out_new = ONE;
	} else {
	    out_new = X;
	}
    } else {
	out_new = ZERO;
    }

    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      xor_exec
 *              - execution routine for xor gates
 *
 ***********************************************************************
 */

void xor_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* update value counts */

    switch (in_old) {
    case X:
    case Z:
	GATE_X(gate)--;
	break;
    case ONE:
	GATE_ONES(gate)--;
	break;
    default:
	break;
    }

    switch (in_new) {
    case X:
    case Z:
	GATE_X(gate)++;
	break;
    case ONE:
	GATE_ONES(gate)++;
	break;
    default:
	break;
    }

    if (GATE_X(gate) == 0) {
	if (GATE_ONES(gate) & 1) {
	    out_new = ONE;
	} else {
	    out_new = ZERO;
	}
    } else {
	out_new = X;
    }

    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      xnor_exec
 *              - execution routine for xnor gates
 *
 ***********************************************************************
 */

void xnor_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* update value counts */

    switch (in_old) {
    case X:
    case Z:
	GATE_X(gate)--;
	break;
    case ONE:
	GATE_ONES(gate)--;
	break;
    default:
	break;
    }

    switch (in_new) {
    case X:
    case Z:
	GATE_X(gate)++;
	break;
    case ONE:
	GATE_ONES(gate)++;
	break;
    default:
	break;
    }

    if (GATE_X(gate) == 0) {
	if (GATE_ONES(gate) & 1) {
	    out_new = ZERO;
	} else {
	    out_new = ONE;
	}
    } else {
	out_new = X;
    }

    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      buf_exec
 *              - execution routine for buf gates
 *
 ***********************************************************************
 */

void buf_exec(struct Marker *marker)
{
    ASSERT(marker != NULL);
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;

    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */


    switch (in_new) {
    case X:
    case Z:
	out_new = X;
	break;
    case ZERO:
	out_new = ZERO;
	break;
    case ONE:
	out_new = ONE;
	break;
    default:
	break;
    }


    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      not_exec
 *              - execution routine for not gates
 *
 ***********************************************************************
 */

void not_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */


    switch (in_new) {
    case X:
    case Z:
	out_new = X;
	break;
    case ZERO:
	out_new = ONE;
	break;
    case ONE:
	out_new = ZERO;
	break;
    default:
	break;
    }


    if (out_new != out_old) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      bufif0_exec
 *              - execution routine for bufif0 gates
 *
 ***********************************************************************
 */

void bufif0_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    int strengthChange = FALSE;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));

	/* this is a kludge until strengths are supported */
	if ((in_new == ONE || in_new == ZERO || in_old == ONE
	     || in_old == ZERO)
	    && (control_value == X || control_value == Z)) {
	    strengthChange = TRUE;
	}
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	out_new = X;
	break;
    case ONE:
	out_new = Z;
	break;
    case ZERO:
	if (input_value == Z) {
	    out_new = X;
	} else {
	    out_new = input_value;
	}
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old || strengthChange) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      bufif1_exec
 *              - execution routine for bufif1 gates
 *
 ***********************************************************************
 */

void bufif1_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    int strengthChange = FALSE;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));

	/* this is a kludge until strengths are supported */
	if ((in_new == ONE || in_new == ZERO || in_old == ONE
	     || in_old == ZERO)
	    && (control_value == X || control_value == Z)) {
	    strengthChange = TRUE;
	}
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	out_new = X;
	break;
    case ZERO:
	out_new = Z;
	break;
    case ONE:
	if (input_value == Z) {
	    out_new = X;
	} else {
	    out_new = input_value;
	}
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old || strengthChange) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      notif0_exec
 *              - execution routine for notif0 gates
 *
 ***********************************************************************
 */

void notif0_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    int strengthChange = FALSE;
    nbits_t nbits;
    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));

	/* this is a kludge until strengths are supported */
	if ((in_new == ONE || in_new == ZERO || in_old == ONE
	     || in_old == ZERO)
	    && (control_value == X || control_value == Z)) {
	    strengthChange = TRUE;
	}
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	out_new = X;
	break;
    case ONE:
	out_new = Z;
	break;
    case ZERO:
	switch (input_value) {
	case X:
	case Z:
	    out_new = X;
	    break;
	case ONE:
	    out_new = ZERO;
	    break;
	case ZERO:
	    out_new = ONE;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old || strengthChange) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      notif1_exec
 *              - execution routine for notif1 gates
 *
 ***********************************************************************
 */

void notif1_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    int strengthChange = FALSE;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));

	/* this is a kludge until strengths are supported */
	if ((in_new == ONE || in_new == ZERO || in_old == ONE
	     || in_old == ZERO)
	    && (control_value == X || control_value == Z)) {
	    strengthChange = TRUE;
	}
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	out_new = X;
	break;
    case ZERO:
	out_new = Z;
	break;
    case ONE:
	switch (input_value) {
	case X:
	case Z:
	    out_new = X;
	    break;
	case ONE:
	    out_new = ZERO;
	    break;
	case ZERO:
	    out_new = ONE;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old || strengthChange) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      pmos_exec
 *              - execution routine for pmos gates
 *
 ***********************************************************************
 */

void pmos_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	switch (input_value) {
	case X:
	case ONE:
	case ZERO:
	    out_new = X;
	    break;
	case Z:
	    out_new = Z;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    case ONE:
	out_new = Z;
	break;
    case ZERO:
	out_new = input_value;
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old
	|| (out_new == X && arg == GATE_INPUT_LIST(gate))) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      nmos_exec
 *              - execution routine for nmos gates
 *
 ***********************************************************************
 */

void nmos_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	switch (input_value) {
	case X:
	case ONE:
	case ZERO:
	    out_new = X;
	    break;
	case Z:
	    out_new = Z;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    case ZERO:
	out_new = Z;
	break;
    case ONE:
	out_new = input_value;
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old
	|| (out_new == X && arg == GATE_INPUT_LIST(gate))) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      rpmos_exec
 *              - execution routine for rpmos gates
 *
 ***********************************************************************
 */

void rpmos_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	switch (input_value) {
	case X:
	case ONE:
	case ZERO:
	    out_new = X;
	    break;
	case Z:
	    out_new = Z;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    case ONE:
	out_new = Z;
	break;
    case ZERO:
	out_new = input_value;
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old
	|| (out_new == X && arg == GATE_INPUT_LIST(gate))) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      rnmos_exec
 *              - execution routine for rnmos gates
 *
 ***********************************************************************
 */

void rnmos_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control_value, input_value;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    if (GATE_INPUT_LIST(gate) == arg) {	// data input has changed
	input_value = in_new;
	ASSERT(TREE_CHAIN(arg) != NULL_TREE);
	ASSERT(TREE_CODE(TREE_CHAIN(arg)) == TREE_LIST);
	control_value = GATE_IN(TREE_CHAIN(arg));
    } else {			// control input has changed
	control_value = in_new;
	ASSERT(GATE_INPUT_LIST(gate) != NULL_TREE);
	ASSERT(TREE_CODE(GATE_INPUT_LIST(gate)) == TREE_LIST);
	input_value = GATE_IN(GATE_INPUT_LIST(gate));
    }


    switch (control_value) {
    case X:
    case Z:
	switch (input_value) {
	case X:
	case ONE:
	case ZERO:
	    out_new = X;
	    break;
	case Z:
	    out_new = Z;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    case ZERO:
	out_new = Z;
	break;
    case ONE:
	out_new = input_value;
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old
	|| (out_new == X && arg == GATE_INPUT_LIST(gate))) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      cmos_exec
 *              - execution routine for cmos gates
 *
 ***********************************************************************
 */

void cmos_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    tree arg1;
    tree arg2;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control1_value, control2_value, input_value;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    ASSERT(GATE_INPUT_LIST(gate));
    input_value = GATE_IN(GATE_INPUT_LIST(gate));
    arg1 = TREE_CHAIN(GATE_INPUT_LIST(gate));
    ASSERT(arg1 != NULL_TREE);
    ASSERT(TREE_CODE(arg1) == TREE_LIST);
    control1_value = GATE_IN(arg1);
    arg2 = TREE_CHAIN(arg1);
    ASSERT(arg2 != NULL_TREE);
    ASSERT(TREE_CODE(arg2) == TREE_LIST);
    control2_value = GATE_IN(arg2);

    switch (input_value) {
    case X:
	if (control1_value == ZERO && control2_value == ONE) {
	    out_new = Z;
	} else {
	    out_new = X;
	}
	break;
    case ZERO:
	if (control1_value != ONE && control2_value != ZERO) {
	    if (control1_value == ZERO && control2_value == ONE) {
		out_new = Z;
	    } else {
		out_new = X;
	    }
	} else {
	    out_new = ZERO;
	}
	break;
    case ONE:
	if (control1_value != ONE && control2_value != ZERO) {
	    if (control1_value == ZERO && control2_value == ONE) {
		out_new = Z;
	    } else {
		out_new = X;
	    }
	} else {
	    out_new = ONE;
	}
	break;
    case Z:
	out_new = Z;
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old
	|| (out_new == X && arg == GATE_INPUT_LIST(gate))) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      rcmos_exec
 *              - execution routine for rcmos gates
 *
 ***********************************************************************
 */

void rcmos_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    tree arg1;
    tree arg2;
    enum logical_value in_new, in_old, out_new, out_old;
    enum logical_value control1_value, control2_value, input_value;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* check if this is a control input */

    ASSERT(GATE_INPUT_LIST(gate));
    input_value = GATE_IN(GATE_INPUT_LIST(gate));
    arg1 = TREE_CHAIN(GATE_INPUT_LIST(gate));
    ASSERT(arg1 != NULL_TREE);
    ASSERT(TREE_CODE(arg1) == TREE_LIST);
    control1_value = GATE_IN(arg1);
    arg2 = TREE_CHAIN(arg1);
    ASSERT(arg2 != NULL_TREE);
    ASSERT(TREE_CODE(arg2) == TREE_LIST);
    control2_value = GATE_IN(arg2);

    switch (input_value) {
    case X:
	if (control1_value == ZERO && control2_value == ONE) {
	    out_new = Z;
	} else {
	    out_new = X;
	}
	break;
    case ZERO:
	if (control1_value != ONE && control2_value != ZERO) {
	    if (control1_value == ZERO && control2_value == ONE) {
		out_new = Z;
	    } else {
		out_new = X;
	    }
	} else {
	    out_new = ZERO;
	}
	break;
    case ONE:
	if (control1_value != ONE && control2_value != ZERO) {
	    if (control1_value == ZERO && control2_value == ONE) {
		out_new = Z;
	    } else {
		out_new = X;
	    }
	} else {
	    out_new = ONE;
	}
	break;
    case Z:
	out_new = Z;
	break;
    default:
	ASSERT(FALSE);
    }

    if (out_new != out_old
	|| (out_new == X && arg == GATE_INPUT_LIST(gate))) {
	delay_t delay;

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      tranif0_exec
 *              - execution routine for tranif0 gates
 *
 ***********************************************************************
 */

void tranif0_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* not a control input, schedule immediately if gate is enabled */

    if (TREE_CHAIN(arg) != NULL_TREE) {
	if (GATE_CONTROL(gate) == ONE) {
	    ScheduleGate(gate, 0);
	}
	return;
    }

    /* a control input has changed nets must be reevaluated */

    if (in_new == ZERO) {
	GATE_CONTROL(gate) = ONE;
    } else {
	GATE_CONTROL(gate) = ZERO;
    }

    {
	delay_t delay;

	// no delay if data input has changed

	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      rtranif0_exec
 *              - execution routine for rtranif0 gates
 *
 ***********************************************************************
 */

void rtranif0_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* not a control input, schedule immediately if gate is enabled */

    if (TREE_CHAIN(arg) != NULL_TREE) {
	if (GATE_CONTROL(gate) == ONE) {
	    ScheduleGate(gate, 0);
	}
	return;
    }

    /* a control input has changed nets must be reevaluated */

    if (in_new == ZERO) {
	GATE_CONTROL(gate) = ONE;
    } else {
	GATE_CONTROL(gate) = ZERO;
    }

    {
	delay_t delay;

	// no delay if data input has changed

	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      tranif1_exec
 *              - execution routine for tranif1 gates
 *
 ***********************************************************************
 */

void tranif1_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* not a control input, schedule immediately if gate is enabled */

    if (TREE_CHAIN(arg) != NULL_TREE) {
	if (GATE_CONTROL(gate) == ONE) {
	    ScheduleGate(gate, 0);
	}
	return;
    }

    /* a control input has changed nets must be reevaluated */

    if (in_new == ONE) {
	GATE_CONTROL(gate) = ONE;
    } else {
	GATE_CONTROL(gate) = ZERO;
    }

    {
	delay_t delay;

	// no delay if data input has changed

	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      rtranif1_exec
 *              - execution routine for rtranif1 gates
 *
 ***********************************************************************
 */

void rtranif1_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* not a control input, schedule immediately if gate is enabled */

    if (TREE_CHAIN(arg) != NULL_TREE) {
	if (GATE_CONTROL(gate) == ONE) {
	    ScheduleGate(gate, 0);
	}
	return;
    }

    /* a control input has changed nets must be reevaluated */

    if (in_new == ONE) {
	GATE_CONTROL(gate) = ONE;
    } else {
	GATE_CONTROL(gate) = ZERO;
    }

    {
	delay_t delay;

	// no delay if data input has changed

	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/***********************************************************************
 *
 *      tran_exec
 *              - execution routine for tran gates
 *
 ***********************************************************************
 */

void tran_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* always schedule immediately */

    ScheduleGate(gate, 0);
}

/***********************************************************************
 *
 *      rtran_exec
 *              - execution routine for rtran gates
 *
 ***********************************************************************
 */

void rtran_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old;
    nbits_t nbits;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);

/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);
    ASSERT(TREE_CODE(arg) == TREE_LIST);

/* save old input and output values */

    in_old = GATE_IN(arg);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbits);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* always schedule immediately */

    ScheduleGate(gate, 0);
}

/***********************************************************************
 *
 *      handle_gate
 *              - gate is ready to execute 
 *
 ***********************************************************************
 */
/* MOVE TO EXEC to handle trace
void    handle_gate(tree gate)
{
                

        ASSERT( gate!=NULL_TREE );
        ASSERT( TREE_CODE(gate)==GATE_INSTANCE );


        if( GATE_PATH_OUTPUT(gate)!=NULL_TREE ) {
                if( handle_specify_path(gate) ) {
                        return;
                }
    }
        (*GATE_PROPAGATE_ROUTINE(gate))( gate );        
}
*/
/***********************************************************************
 *
 *      propagate_specify_output
 *              - pass unidirectional gate output to rest of the world 
 *
 ***********************************************************************
 */

void propagate_specify_output(tree gate, enum logical_value v)
{
    Group *g1;
    tree port;
    tree t;


    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    ASSERT(GATE_OUTPUT_LIST(gate) != NULL_TREE);

    for (t = GATE_OUTPUT_LIST(gate); t; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == TREE_LIST);
	g1 = *R++;
	switch (v) {
	case ONE:
	    AVAL(g1) = 1;
	    BVAL(g1) = 0;
	    break;

	case ZERO:
	    AVAL(g1) = 0;
	    BVAL(g1) = 0;
	    break;

	case X:
	    AVAL(g1) = 1;
	    BVAL(g1) = 1;
	    break;

	case Z:
	    AVAL(g1) = 0;
	    BVAL(g1) = 1;
	    break;

	default:
	    ASSERT(FALSE);
	}

	port = TREE_PURPOSE(t);
	ASSERT(port != NULL_TREE);
//                ASSERT( TREE_CODE(port)==NET_SCALAR_DECL );
	store(port, gate);
    }
}

/***********************************************************************
 *
 *      propagate_unigate_output
 *              - pass unidirectional gate output to rest of the world 
 *
 ***********************************************************************
 */

void propagate_unigate_output(tree gate)
{
    Group *g1;
    tree port;
    tree t;
    enum logical_value v;


    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    ASSERT(GATE_OUTPUT_LIST(gate) != NULL_TREE);
    v = GATE_OUTPUT(gate);

    for (t = GATE_OUTPUT_LIST(gate); t; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == TREE_LIST);
	g1 = *R++;
	switch (v) {
	case ONE:
	    AVAL(g1) = 1;
	    BVAL(g1) = 0;
	    break;

	case ZERO:
	    AVAL(g1) = 0;
	    BVAL(g1) = 0;
	    break;

	case X:
	    AVAL(g1) = 1;
	    BVAL(g1) = 1;
	    break;

	case Z:
	    AVAL(g1) = 0;
	    BVAL(g1) = 1;
	    break;

	default:
	    ASSERT(FALSE);
	}

	port = TREE_PURPOSE(t);
	ASSERT(port != NULL_TREE);
//                ASSERT( TREE_CODE(port)==NET_SCALAR_DECL );
	store(port, gate);
    }
}

/***********************************************************************
 *
 *      propagate_bigate_output
 *              - pass bidirectional gate output to rest of the world 
 *
 ***********************************************************************
 */

void propagate_bigate_output(tree gate)
{
    tree port1_in;
    tree port1_out;
    tree port2_in;
    tree port2_out;
    tree t_in;
    tree t_out;
    Group *g1;


    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);

    t_in = GATE_INPUT_LIST(gate);
    t_out = GATE_OUTPUT_LIST(gate);
    ASSERT(t_in != NULL_TREE);
    ASSERT(t_out != NULL_TREE);
    port2_out = TREE_PURPOSE(t_out);
    port1_in = TREE_PURPOSE(t_in);
    t_in = TREE_CHAIN(t_in);
    t_out = TREE_CHAIN(t_out);
    ASSERT(t_in != NULL_TREE);
    ASSERT(t_out != NULL_TREE);
    port1_out = TREE_PURPOSE(t_out);
    port2_in = TREE_PURPOSE(t_in);
    ASSERT(port1_in != NULL_TREE);
    ASSERT(TREE_CODE(port1_in) == NET_SCALAR_DECL);
    ASSERT(port2_in != NULL_TREE);
    ASSERT(TREE_CODE(port2_in) == NET_SCALAR_DECL);
    ASSERT(port1_out != NULL_TREE);
    ASSERT(TREE_CODE(port1_out) == NET_SCALAR_DECL);
    ASSERT(port2_out != NULL_TREE);
    ASSERT(TREE_CODE(port2_out) == NET_SCALAR_DECL);

    if (GATE_CONTROL(gate) == ONE) {
	do_net_eval(TREE_CHAIN(port1_out), 0, port1_out);
    } else {
	g1 = *R++;
	AVAL(g1) = 0;
	BVAL(g1) = 1;
    }
    store(port2_out, gate);

    if (GATE_CONTROL(gate) == ONE) {
	do_net_eval(TREE_CHAIN(port2_out), 0, port2_out);
    } else {
	g1 = *R++;
	AVAL(g1) = 0;
	BVAL(g1) = 1;
    }
    store(port1_out, gate);
}
