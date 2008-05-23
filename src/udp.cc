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


/*******************************************************************
 *
 *		upd.c
 *			-	procedures for parsing and executing upd's
 *
 *******************************************************************
 */

#define UDP_C

#include <stdio.h>
#include <string.h>
#include "vtypes.h"
#include "tree.h"
#include "runtime.h"
#include "udp.h"
#include "gates.h"
#include "schedule.h"
#include "eval.h"
#include "glue.h"
#include "copy.h"
#include "veriwell.h"
#include "nsched.h"

extern lineno_t lineno;
extern char *input_filename;
extern tree current_module;

int deltaTable[10][4][4];
static int udp_inited = FALSE;

/*******************************************************************
 *
 *		build_udp	
 *			- creates a udp tree structure	
 *
 *******************************************************************
 */
tree build_udp(tree identifier)
{
    tree t;

    ASSERT(udp_inited == TRUE);
    t = build_module(identifier);
    UDP_ATTR(t) = TRUE;
    UDP_TABLE(t) = make_node(UDP_TABLE_NODE);
    return (t);
}


/*******************************************************************
 *
 *		build_udp_string	
 *			- creates a udp string structure 
 *
 *******************************************************************
 */
tree build_udp_string(void)
{
    tree t;

    t = make_node(UDP_STRING_NODE);
    UDP_STRING_STRING(t)[0] = 0;
    UDP_STRING_LINE(t) = lineno;
    UDP_STRING_FILE(t) = input_filename;
    return (t);
}

/*******************************************************************
 *
 *		end_primitive	
 *			- post process udp definition	
 *
 *******************************************************************
 */
void end_primitive(tree node)
{
    tree t;
    tree t1;
    char *name;
    int numberOfTables;
    int tableSize;
    int i;
    char *table;
    tree udp_table;
    int numberOfEntries;

    ASSERT(node != NULL_TREE);
    ASSERT(TREE_CODE(node) == MODULE_BLOCK);
    ASSERT(UDP_ATTR(node));
    ASSERT(UDP_STRING_LIST(node) != NULL_TREE);

    for (t = UDP_PORT_LIST(node); t; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == TREE_LIST);
	t1 = TREE_PURPOSE(t);
	ASSERT(t1 != NULL_TREE);
	/* port must be defined and an identifier node */
	/* first port must be an output, rest inputs */

	name = IDENT(DECL_NAME(t1));
	if (TREE_CODE(t1) != NET_SCALAR_DECL) {
	    error("Port '%s' is not defined as a scalar net", name,
		  NULL_CHAR);
	    return;
	} else if (t == MODULE_PORT_LIST(node) &&
		   (!PORT_OUTPUT_ATTR(t1) || PORT_INPUT_ATTR(t1))) {
	    error("Port '%s' not declared as OUTPUT", name, NULL_CHAR);
	    return;
	} else if (t != MODULE_PORT_LIST(node) &&
		   (PORT_OUTPUT_ATTR(t1) || !PORT_INPUT_ATTR(t1))) {
	    error("Port '%s' not declared as INPUT", name, NULL_CHAR);
	    return;
	}
    }
    if ((UDP_REG_NAME(node) == NULL_TREE &&
	 list_length(UDP_PORT_LIST(node)) > 11) ||
	(UDP_REG_NAME(node) != NULL_TREE &&
	 list_length(UDP_PORT_LIST(node)) > 10)) {
	error("too many ports for udp primitive '%s'",
	      UDP_NAME(node), NULL_CHAR);
	return;
    }

    /* build state tables */

    if (UDP_REG_NAME(node) == NULL_TREE) {
	numberOfTables = list_length(UDP_PORT_LIST(node)) - 1;
	numberOfEntries = numberOfTables;
    } else {
	numberOfTables = list_length(UDP_PORT_LIST(node)) - 1;
	numberOfEntries = numberOfTables + 1;
    }


    ASSERT(numberOfTables <= 10);
    ASSERT(numberOfEntries <= 10);

    tableSize = int_power(3, numberOfEntries);
    udp_table = UDP_TABLE(node);
    ASSERT(udp_table != NULL_TREE);
    ASSERT(TREE_CODE(udp_table) == UDP_TABLE_NODE);

    for (i = 0; i < numberOfTables; i++) {
	table = xmalloc(tableSize);

	UDP_TABLE_ARRAY(udp_table, i) = table;
	memset(table, 0xff, tableSize);
	fill_udp_table(table, i, numberOfEntries, UDP_STRING_LIST(node),
		       UDP_REG_NAME(node) != NULL_TREE);
    }
}


/*******************************************************************
 *
 *		instantiate_udp	
 *			- creates an instance of a udp gate	
 *
 *******************************************************************
 */
void instantiate_udp(tree module, tree caller, tree udp_def)
{
    tree gate;
    tree t;
    tree delay;
    tree delay1;
    tree delay2;
    tree delay3;
    int label;


    ASSERT(caller != NULL_TREE);
    ASSERT(TREE_CODE(caller) == INSTANCE_NODE);
    ASSERT(udp_def != NULL_TREE);
    ASSERT(TREE_CODE(udp_def) == MODULE_BLOCK);
    ASSERT(UDP_ATTR(udp_def));

    t = INSTANCE_PARAMS(caller);
    if (t != NULL_TREE) {
	ASSERT(TREE_CODE(t) == TREE_LIST);
	delay1 = TREE_PURPOSE(t);
	delay2 = delay1;
	delay3 = NULL_TREE;
	t = TREE_CHAIN(t);
	label = 1;
	if (t) {
	    ASSERT(TREE_CODE(t) == TREE_LIST);
	    delay2 = TREE_PURPOSE(t);
	    t = TREE_CHAIN(t);
	    label = 2;
	}
	if (t) {
	    ASSERT(TREE_CODE(t) == TREE_LIST);
	    delay3 = TREE_PURPOSE(t);
	    t = TREE_CHAIN(t);
	    label = 3;
	}
	delay = build_nt(DELAY_EXPR, delay1, delay2, delay3);
	TREE_LABEL(delay) = label;
    } else {
	delay = NULL_TREE;
    }

    gate = build_gate_instance(GATE_UDP_TYPE, INSTANCE_FILE(caller),
			       INSTANCE_LINE(caller),
			       INSTANCE_NAME(caller),
			       copy_tree_with_stuff(INSTANCE_PORTS(caller),
						    NULL_TREE), delay,
			       udp_def);

    BLOCK_BODY(module) = chainon(gate, BLOCK_BODY(module));
//  MODULE_INSTANCES(module) = chainon( gate, 
//                              MODULE_INSTANCES(module) );
}

/*******************************************************************
 *
 *		append_udp_digits	
 *			- append 2 digits to udp string. allocate new node if needed 
 *
 *******************************************************************
 */
void append_udp_digits(tree * ptr, char c1, char c2)
{
    tree t;
    int length;

    if (*ptr == NULL_TREE) {
	*ptr = build_udp_string();
    }
    t = *ptr;
    /* note the size of the string must be at least one position
       greater than the maximum allowable size. This fact will be 
       used to do later error checking */

    length = strlen(UDP_STRING_STRING(t));
    if (length >= sizeof(UDP_STRING_STRING(t)) + 2) {
	return;
    }

    UDP_STRING_STRING(t)[length++] = c1;
    UDP_STRING_STRING(t)[length++] = c2;
}

/*******************************************************************
 *
 *		validate_udp_string	
 *			- validate the contents of the udp string.
 *			  prune out colons. 
 *
 *******************************************************************
 */
void validate_udp_string(tree udp, tree udp_string)
{
    int combFlag;
    int numberOfPorts;
    int length;
    int i;
    int edgeCount;
    char *ptr;

    ASSERT(udp != NULL_TREE);
    ASSERT(TREE_CODE(udp) == MODULE_BLOCK);
    ASSERT(UDP_ATTR(udp));
    ASSERT(udp_string != NULL_TREE);
    ASSERT(TREE_CODE(udp_string) == UDP_STRING_NODE);

    ptr = UDP_STRING_STRING(udp_string);
    combFlag = (UDP_REG_NAME(udp) == NULL_TREE);
    numberOfPorts = list_length(UDP_PORT_LIST(udp));
    length = strlen(ptr);

    if ((combFlag && (numberOfPorts + 1) * 2 != length) ||
	(!combFlag && (numberOfPorts + 3) * 2 != length)) {
	error("Bad table entry", NULL_CHAR, NULL_CHAR);
	return;
    }
    if (combFlag) {
	if (ptr[(numberOfPorts - 1) * 2] != ':' ||
	    ptr[(numberOfPorts - 1) * 2 + 1] != ':') {
	    error("Missing ':' in table entry", NULL_CHAR, NULL_CHAR);
	    return;
	}
	memmove(&ptr[(numberOfPorts - 1) * 2], &ptr[numberOfPorts * 2], 3);
    } else {
	if (ptr[(numberOfPorts - 1) * 2] != ':' ||
	    ptr[(numberOfPorts - 1) * 2 + 1] != ':' ||
	    ptr[(numberOfPorts + 1) * 2] != ':' ||
	    ptr[(numberOfPorts + 1) * 2 + 1] != ':') {
	    error("Missing ':' in table entry", NULL_CHAR, NULL_CHAR);
	    return;
	}
	memmove(&ptr[(numberOfPorts - 1) * 2], &ptr[(numberOfPorts) * 2],
		2);
	memmove(&ptr[(numberOfPorts) * 2], &ptr[(numberOfPorts + 2) * 2],
		3);
    }

    /*  validate number of edges */

    edgeCount = 0;
    length = strlen(ptr);
    for (i = 0; i < length; i += 2) {
	if (is_edge(&ptr[i])) {
	    edgeCount++;
	}
    }
    if (combFlag && edgeCount != 0) {
	error("Edge specifications are not allowed in sequential upd's",
	      NULL_CHAR, NULL_CHAR);
	return;
    } else if (!combFlag && edgeCount > 1) {
	error("Only one edge specification is allowed per table entry",
	      NULL_CHAR, NULL_CHAR);
	return;
    }

    /* validate inputs */
    for (i = 0; i < (numberOfPorts - 1) * 2; i += 2) {
	if (!is_valid_input(&ptr[i], combFlag)) {
	    error("illegal character in input portion of table",
		  NULL_CHAR, NULL_CHAR);
	    return;
	}
    }

    /* validate previous output for seq. udp */

    if (!combFlag && !is_valid_input(&ptr[length - 4], TRUE)) {
	error("illegal character in current value portion of table",
	      NULL_CHAR, NULL_CHAR);
	return;
    }

    /* validate output */

    if (!is_valid_output(&ptr[length - 2], combFlag)) {
	error("illegal character in output portion of table",
	      NULL_CHAR, NULL_CHAR);
	return;
    }


}

/*******************************************************************
 *
 *		is_edge	
 *			- returns TRUE if two bytes represent and edge 
 *
 *******************************************************************
 */
int is_edge(char *ptr)
{
    if (ptr[0] != ptr[1]) {
	return (TRUE);
    }
    switch (*ptr) {
    case '*':
    case 'r':
    case 'f':
    case 'p':
    case 'n':
	return (TRUE);
    }
    return (FALSE);
}

/*******************************************************************
 *
 *		is_valid_input	
 *			- returns TRUE if two bytes represent valid input 
 *
 *******************************************************************
 */
int is_valid_input(char *ptr, int combFlag)
{
    if (combFlag && is_edge(ptr)) {
	return (FALSE);
    }
    if (*ptr == '-') {
	return (FALSE);
    }
    return (TRUE);
}

/*******************************************************************
 *
 *		is_valid_output	
 *			- returns TRUE if two bytes represent valid output 
 *
 *******************************************************************
 */
int is_valid_output(char *ptr, int combFlag)
{
    if (is_edge(ptr)) {
	return (FALSE);
    }
    if (combFlag && *ptr == '-') {
	return (FALSE);
    }
    return (TRUE);
}

/*******************************************************************
 *
 *		fill_udp_table	
 *			- converts string list to execution table for input i
 *
 *******************************************************************
 */
void
fill_udp_table(char *table, int inputNumber, int inputCount,
	       tree string_list, int seqFlag)
{
    tree t;
    int position;
    int i;
    int tableSize;
    char *save_input_filename;
    int save_lineno;
    int report_error;
    char *string;


    ASSERT(table != NULL);
    ASSERT(inputCount < 11 && inputCount > 0);
    ASSERT(inputNumber < inputCount && inputNumber >= 0);
    ASSERT(string_list != NULL);


    /* do level pass first */

    for (t = string_list; t; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == UDP_STRING_NODE);
	string = UDP_STRING_STRING(t);
	if (!is_edge_string(t, &position)) {
	    save_input_filename = input_filename;
	    save_lineno = lineno;
	    runtime_error(t);
	    report_error = (inputNumber == 0);
	    set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				NULL, string, &string[2], 0xff,
				&report_error);
	    input_filename = save_input_filename;
	    lineno = save_lineno;
	}
    }

    /* do edge pass next */

    for (t = string_list; t; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == UDP_STRING_NODE);
	string = UDP_STRING_STRING(t);
	if (is_edge_string(t, &position) && position == inputNumber) {
	    save_input_filename = input_filename;
	    save_lineno = lineno;
	    runtime_error(t);
	    report_error = TRUE;
	    switch (string[inputNumber * 2]) {
	    case '0':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x03,
				    &report_error);
		break;
	    case '1':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x0c,
				    &report_error);
		break;
	    case 'x':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x30,
				    &report_error);
		break;
	    case '!':
	    case '?':
	    case '*':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x3f,
				    &report_error);
		break;
	    case '%':
	    case 'b':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x0f,
				    &report_error);
		break;
	    case 'r':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x03,
				    &report_error);
		break;
	    case 'f':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x0c,
				    &report_error);
		break;
	    case 'p':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x33,
				    &report_error);
		break;
	    case 'n':
		set_udp_table_entry(table, int_power(3, inputCount - 1), 0,
				    NULL, string, &string[2], 0x3c,
				    &report_error);
		break;
	    default:
		ASSERT(FALSE);
	    }
	    input_filename = save_input_filename;
	    lineno = save_lineno;

	}
    }

    /* x out uninitialized areas */

    tableSize = int_power(3, inputCount);
    for (i = 0; i < tableSize; i++) {
	int temp;

	temp = table[i];
	temp = 0x15 & (~temp) & (temp >> 1);
	table[i] ^= temp;
    }


}

/*******************************************************************
 *
 *		set_udp_table_entry	
 *			- recursive function to set a udp table entry 
 *
 *******************************************************************
 */
void
set_udp_table_entry(char *table, int position, int state, char *last,
		    char *next, char *rest, int from_mask, int *reportFlag)
{
    char c;
    int from;
    int temp;


    /* check for termination condition */

    if (rest[0] == 0) {
	if (next[0] == '-') {
	    c = last[0];
	} else {
	    c = next[0];
	}
	switch (c) {
	case '0':
	    from = 0x00;
	    break;
	case '1':
	    from = 0x15;
	    break;
	case 'x':
	    from = 0x2a;
	    break;
	default:
	    ASSERT(FALSE);
	}
	if ((from_mask & table[state]) != from_mask) {
	    temp = table[state] & from_mask;
	    temp = temp & (temp << 1);
	    temp = temp | (temp >> 1);
	    temp = temp ^ from_mask;
	    if (*reportFlag && ((table[state] ^ from) & temp) == 0) {
//                  warning( "table entry overlap previous entry", NULL_CHAR, NULL_CHAR );
	    } else if (*reportFlag) {
		if (from_mask == 0xff) {	// level<->level conflict 
		    error("level vs level table entry conflict", NULL_CHAR,
			  NULL_CHAR);
		    *reportFlag = FALSE;
		} else {	// edge sensitive entry
		    if ((table[state] & 0xc0) == 0) {	// edge<->level conflict
//  I commented this out because I believe this is a common occurance
//                      warning( "level vs edge table entry conflict", NULL_CHAR, NULL_CHAR );
		    } else {	// edge<->edge conflict
			error("edge vs edge table entry conflict",
			      NULL_CHAR, NULL_CHAR);
			*reportFlag = FALSE;
		    }
		}
	    }
	} else {
	    table[state] =
		(table[state] & ~from_mask) | (from & from_mask);
	}
	return;
    }


    switch (next[1]) {
    case '0':
    case 'f':
	state = state / 3;
	set_udp_table_entry(table, position, state, next, rest, rest + 2,
			    from_mask, reportFlag);
	break;
    case '1':
    case 'r':
	state = state / 3 + position;
	set_udp_table_entry(table, position, state, next, rest, rest + 2,
			    from_mask, reportFlag);
	break;
    case 'x':
	state = state / 3 + 2 * position;
	set_udp_table_entry(table, position, state, next, rest, rest + 2,
			    from_mask, reportFlag);
	break;
    case 'b':
	set_udp_table_entry(table, position, state, last, "00", rest,
			    from_mask, reportFlag);
	set_udp_table_entry(table, position, state, last, "11", rest,
			    from_mask, reportFlag);
	break;
    case '?':
    case '*':
	set_udp_table_entry(table, position, state, last, "00", rest,
			    from_mask, reportFlag);
	set_udp_table_entry(table, position, state, last, "11", rest,
			    from_mask, reportFlag);
	set_udp_table_entry(table, position, state, last, "xx", rest,
			    from_mask, reportFlag);
	break;
    case 'p':
	set_udp_table_entry(table, position, state, last, "xx", rest,
			    from_mask, reportFlag);
	set_udp_table_entry(table, position, state, last, "11", rest,
			    from_mask, reportFlag);
	break;
    case 'n':
	set_udp_table_entry(table, position, state, last, "xx", rest,
			    from_mask, reportFlag);
	set_udp_table_entry(table, position, state, last, "00", rest,
			    from_mask, reportFlag);
	break;
    case '-':			// only an error case can take us here - just return
	return;
    default:
	ASSERT(FALSE);
    }
}


/*******************************************************************
 *
 *		is_edge_string	
 *			- returns TRUE if UDP_STRING is an edge specification 
 *
 *******************************************************************
 */

int is_edge_string(tree t, int *inputNumber)
{
    char *ptr;
    int count;

    ASSERT(t != NULL_TREE);
    ASSERT(TREE_CODE(t) == UDP_STRING_NODE);
    count = 0;
    for (ptr = UDP_STRING_STRING(t); *ptr; ptr += 2) {
	if (is_edge(ptr)) {
	    *inputNumber = count;
	    return (TRUE);
	}
	count++;
    }

    return (FALSE);
}

/*******************************************************************
 *
 *		int_power	
 *			- returns  'b' raised to the 'p' power
 *
 *******************************************************************
 */
int int_power(int b, int p)
{
    int i;
    int result;

    result = 1;
    for (i = 0; i < p; i++) {
	result *= b;
    }
    return (result);
}

/*******************************************************************
 *
 *		udp_exec	
 *			- execute a udp gate	
 *
 *******************************************************************
 */
void udp_exec(struct Marker *marker)
{
    tree gate = (tree) marker->scb;
    tree arg;
    enum logical_value in_new, in_old, out_new, out_old;
    char *table;
    tree udp_def;
    tree udp_table;
    int arg_number;
    int state;
    nbits_t nbit;

    ASSERT(marker != NULL);
    ASSERT(gate != NULL);
    udp_def = GATE_UDP_DEF(gate);
    ASSERT(udp_def);
    ASSERT(TREE_CODE(udp_def) == MODULE_BLOCK);
    ASSERT(UDP_ATTR(udp_def));
    udp_table = UDP_TABLE(udp_def);
    ASSERT(udp_table != NULL_TREE);
    ASSERT(TREE_CODE(udp_table) == UDP_TABLE_NODE);



/* This is the arg expression, code, and last value */

    arg = marker->expr.arg;
    ASSERT(arg != NULL_TREE);

/* save old input and output values */

    in_old = GATE_IN(arg);
    out_old = GATE_OUTPUT(gate);
    arg_number = GATE_INPUT_NUMBER(arg);
    ASSERT(arg_number >= 0);
    ASSERT(UDP_REG_NAME(udp_def) != NULL_TREE
	   || arg_number < GATE_INPUTS(gate));
    ASSERT(UDP_REG_NAME(udp_def) == NULL_TREE
	   || arg_number < GATE_INPUTS(gate) + 1);
    table = UDP_TABLE_ARRAY(udp_table, arg_number);
    ASSERT(table != NULL);

/* First, evaluate the active input, if necessary.  If the following is
   true, then decl is the only argument and no need
   reevaluate arg expression.  If is isn't true, then decl is only
   one component of the argument expression, so arg needs to be
   evaluated.
*/

    if (marker->flags & M_PRIM_FAST) {
	in_new = DECL_STATE(marker->decl);	/* Value is xfer'd in marker */
    } else {
	Group *g = eval_(TREE_EXPR_CODE(arg), &nbit);	/* need to re-eval */
	in_new = VAL_TO_STATE(g);
    }
    if (in_new == Z) {		// Z should always be treated as X
	in_new = X;
    }

    if (in_old == in_new)	/* if input didn't change, */
	return;			/* ...return. */

    SET_GATE_IN(arg, in_new);	/* Input changed; remember */

    /* calculate new state value */

    state = GATE_STATE(gate);
    state += deltaTable[arg_number][in_old][in_new];
    GATE_STATE(gate) = state;

    /* calculate new output value */

    switch (in_old) {
    case ZERO:
	out_new = (enum logical_value) (table[state] & 0x3);
	break;
    case ONE:
	out_new = (enum logical_value) ((table[state] >> 2) & 0x3);
	break;
    case X:
	out_new = (enum logical_value) ((table[state] >> 4) & 0x3);
	break;
    case Z:
    default:
	ASSERT(FALSE);
    }

    /* seq. udps require the output value to be written into the state vector */
//printf( "MDH: s=%d a=%d in_o=%d in_n=%d out_o=%d in_o=%d\n",
//	state, arg_number, in_old, in_new, out_old, out_new);

    if (out_new != out_old) {
	delay_t delay;

	/* seq. udps require the output value to be written into the state vector */

	if (UDP_REG_NAME(udp_def) != NULL_TREE) {
	    state += deltaTable[GATE_INPUTS(gate)][out_old][out_new];
	    GATE_STATE(gate) = state;
	}

	GATE_OUTPUT(gate) = out_new;
	if (GATE_DELAY(gate) && !in_initial) {
	    delay = eval_delay(GATE_DELAY(gate), GATE_OUTPUT(gate));
	} else {
	    delay = 0;
	}

	ScheduleGate(gate, delay);
    }
}

/*******************************************************************
 *
 *		init_udp	
 *			- initialize udp structures	
 *
 *******************************************************************
 */
void init_udp(void)
{

    int i;
    int new_t;
    int old;
    int arg_new;
    int arg_old;

    udp_inited = TRUE;

/* load delta table */

    for (i = 0; i < 10; i++) {
	for (new_t = 0; new_t < 4; new_t++) {
	    if (new_t == 3) {
		arg_new = 2;
	    } else {
		arg_new = new_t;
	    }
	    for (old = 0; old < 4; old++) {
		if (old == 3) {
		    arg_old = 2;
		} else {
		    arg_old = old;
		}
		deltaTable[i][old][new_t] =
		    (arg_new * int_power(3, i)) -
		    (arg_old * int_power(3, i));
	    }
	}
    }

}
