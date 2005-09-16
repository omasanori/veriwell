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
/**********************************************************************
 *
 *      specify.c
 *              - specify block code
 *
 **********************************************************************
 */

#define SPECIFY_C

#include <stdio.h>
#include <limits.h>
#include "vtypes.h"
#include "tree.h"
#include "specify.h"
#include "decl.h"
#include "schedule.h"
#include "parse.h"
#include "acc_user.h"
#include "pli.h"
#include "systask.h"
#include "strobe.h"
#include "gates.h"
#include "io.h"
#include "eval.h"
#include "pass3.h"
#include "store.h"
#include "veriwell.h"
#include "glue.h"
#include "print.h"
#include "copy.h"
#include "timescal.h"

#define edgeMask(a,b) (edge_mask[(a)][(b)])
#if 0				// obsolate
unsigned int edge_mask[4][4] = {
/*      0       1       Z       X       */
/* 0 */ 0x0000, 0x0002, 0x0008, 0x0008,
/* 1 */ 0x0010, 0x0000, 0x0080, 0x0080,
/* Z */ 0x1000, 0x2000, 0x0000, 0x0000,
/* X */ 0x1000, 0x2000, 0x0000, 0x0000
};

unsigned int posedgeMask = 0x8 | 0x2 | 0x2000;
unsigned int alledgeMask = 0x309a;
unsigned int negedgeMask = 0x80 | 0x10 | 0x1000;
#endif				// 0

unsigned int edge_mask[4][4] = {
/*      0       	1       	Z       	X       */
/* 0 */ accNoedge, accEdge01, accEdge0x, accEdge0x,
/* 1 */ accEdge10, accNoedge, accEdge1x, accEdge1x,
/* Z */ accEdgex0, accEdgex1, accNoedge, accNoedge,
/* X */ accEdgex0, accEdgex1, accNoedge, accNoedge
};

unsigned int posedgeMask = accPosedge;
unsigned int alledgeMask = accPosedge + accNegedge;
unsigned int negedgeMask = accNegedge;



extern tree current_scope;
extern struct Marker_info marker_info;
extern Group **R;
extern lineno_t stmt_lineno;
extern lineno_t lineno;
extern char *input_filename;
extern int in_initial;


#define minimum(a,b) ((a)>(b)?(b):(a))
#define maximum(a,b) ((a)<(b)?(b):(a))


/**********************************************************************
 *
 *      check_input_port
 *              - verify the given identifier is currently defined
 *                as an input or inout port 
 *
 **********************************************************************
 */
tree check_input_port(tree identifier)
{
    tree t = IDENT_CURRENT_DECL(identifier);

    /* validate port ref */
    if (!t || DECL_CONTEXT(t) != current_scope	/* ||
						   root_port_decl(t) != identifier */ ) {
	error("'%s' is not in the port list", IDENT(identifier),
	      NULL_CHAR);
	return (error_mark_node);
    }
    if (!PORT_INPUT_ATTR(t)) {
	error("'%s' is not an input or inout port", IDENT(identifier),
	      NULL_CHAR);
	return (error_mark_node);
    }
    return (identifier);
}

/**********************************************************************
 *
 *      check_output_port
 *              - verify the given identifier is currently defined
 *                as an output or inout port 
 *
 **********************************************************************
 */
tree check_output_port(tree identifier)
{
    tree t = IDENT_CURRENT_DECL(identifier);

    /* validate port ref */
    if (!t || DECL_CONTEXT(t) != current_scope	/* ||
						   root_port_decl(t) != identifier */ ) {
	error("'%s' is not in the port list", IDENT(identifier),
	      NULL_CHAR);
	return (error_mark_node);
    }
    if (!PORT_OUTPUT_ATTR(t)) {
	error("'%s' is not an output or inout port", IDENT(identifier),
	      NULL_CHAR);
	return (error_mark_node);
    }
    return (identifier);
}

/**********************************************************************
 *
 *      check_path
 *              validate path arguments
 *
 **********************************************************************
 */
tree check_path(tree path)
{
    runtime_error(path);

    if (PATH_SOURCE(path) != NULL_TREE) {
	warning("Edge sensitive paths are not supported in this version",
		NULL_CHAR, NULL_CHAR);
	if (TREE_CHAIN(PATH_INPUTS(path))) {
	    error
		("Multiple inputs are not allowed for edge sensitive paths",
		 NULL_CHAR, NULL_CHAR);
	}
    } else if (PATH_CONDITION(path) != NULL_TREE) {
	warning("Level sensitive paths are not supported in this version",
		NULL_CHAR, NULL_CHAR);
    } else if (PATH_POLARITY(path) != 0) {
	warning("Polarized paths are not supported in this version",
		NULL_CHAR, NULL_CHAR);
    } else if (PATH_PARALLEL(path)) {
	warning("Parallel paths are not supported in this version",
		NULL_CHAR, NULL_CHAR);
    }
// If you follow the cadence bnf this is correct but.....
//        if( PATH_PARALLEL(path) && (TREE_CHAIN(PATH_OUTPUTS(path)) ||
//                        TREE_CHAIN(PATH_INPUTS(path)) ) ) {
//                error( "Multiple inputs/outputs are not allowed for with => paths",
//                                        NULL_CHAR, NULL_CHAR );
//        }

    return (path);
}

/**********************************************************************
 *
 *      specify_save_param_list
 *              - save specparam list in module 
 *
 **********************************************************************
 */

void specify_save_param_list(tree currentModule, tree list)
{

    tree t;

    /*
       convert all PARAM_DECL's to SPECPARAM_DECL's 
       validate param list:
       should not contain any parameter refs in expressions ??????
     */

    for (t = list; t; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == PARAM_DECL);
	TREE_SET_CODE(t, SPECPARAM_DECL);
    }

    /*      append list to spec params of current scope
     */

    MODULE_SPECDEFS(currentModule) =
	chainon(list, MODULE_SPECDEFS(currentModule));

}


/**********************************************************************
 *
 *      build_path( tree inputs, tree outputs, int parallelFlag )
 *              - create a PATH_NODE  
 *
 **********************************************************************
 */

tree
build_path(tree inputs, tree outputs, tree source, tree delays,
	   tree condition, int parallelFlag, int edge, int polarity)
{
    tree t;

    ASSERT(parallelFlag == TRUE || parallelFlag == FALSE);
    ASSERT(outputs != NULL_TREE);
    ASSERT(inputs != NULL_TREE);
    ASSERT(edge >= -1 && edge <= 1);
    ASSERT(polarity >= -1 && polarity <= 1);

    t = make_node(PATH_NODE);
    PATH_SOURCE_LINE(t) = lineno;
    PATH_SOURCE_FILE(t) = input_filename;
    PATH_SOURCE(t) = source;
    PATH_DELAYS(t) = delays;
    PATH_INPUTS(t) = inputs;
    PATH_OUTPUTS(t) = outputs;
    PATH_PARALLEL(t) = parallelFlag;
    PATH_EDGE(t) = edge;
    PATH_CONDITION(t) = condition;
    PATH_POLARITY(t) = polarity;
    return (t);
}

/**********************************************************************
 *
 *      insert_specify_paths( tree module )     
 *              - validate and insert specify paths  
 *
 **********************************************************************
 */
void insert_specify_paths(tree module)
{
    tree paths;
    tree out_list;
    tree out_decl;
    tree out_ident;
    tree t;
    tree gate;
    int driver_count;
    int msb;
    int lsb;
    int bitNumber;
    tree instance;
    int outputNumber;


    ASSERT(module != NULL_TREE);
    for (paths = MODULE_SPECDEFS(module); paths; paths = TREE_CHAIN(paths)) {
	if (TREE_CODE(paths) != PATH_NODE) {
	    continue;
	}
	runtime_error(paths);

	/* this node is only used by pli to iterate though the modpaths */

	instance = make_node(PATH_INSTANCE);
	PATH_INSTANCE_MODULE(instance) = module;
	PATH_INSTANCE_PATHDESC(instance) = paths;
	TREE_CHAIN(instance) = MODULE_SPECINST(module);
	MODULE_SPECINST(module) = instance;

	outputNumber = 0;
	for (out_list = PATH_OUTPUTS(paths); out_list;
	     out_list = TREE_CHAIN(out_list)) {
	    t = TREE_PURPOSE(out_list);
	    ASSERT(t != NULL_TREE);
	    switch (TREE_CODE(t)) {
	    case PART_REF:
		out_ident = PART_NAME(t);
		out_decl = IDENT_CURRENT_DECL(out_ident);
		msb =
		    get_range(PART_MSB_(t), IDENTIFIER_POINTER(out_ident));
		lsb =
		    get_range(PART_LSB_(t), IDENTIFIER_POINTER(out_ident));
		break;
	    case BIT_REF:
		out_ident = BIT_REF_NAME(t);
		out_decl = IDENT_CURRENT_DECL(out_ident);
		msb =
		    get_range(BIT_EXPR(t), IDENTIFIER_POINTER(out_ident));
		lsb = msb;
		break;


	    default:
		out_ident = t;
		out_decl = IDENT_CURRENT_DECL(out_ident);
		if (TREE_CODE(out_decl) == NET_VECTOR_DECL) {
		    msb = MSB(out_decl);
		    lsb = LSB(out_decl);
		} else {
		    msb = 0;
		    lsb = 0;
		}
		break;
	    }
	    ASSERT(out_decl != NULL_TREE);
	    if (lsb > msb) {
		int temp;
		temp = lsb;
		lsb = msb;
		msb = temp;
	    }
	    for (bitNumber = lsb; bitNumber <= msb; bitNumber++) {
		driver_count = get_driver_count(out_decl, bitNumber);
		if (driver_count == 0) {
		    error
			("specify path for net %s does not connect to a gate primitive",
			 IDENT(out_ident), NULL_CHAR);
		    continue;
		}
		if (driver_count > 1) {
		    error("net %s has more than one driver",
			  IDENT(out_ident), NULL_CHAR);
		    return;
		}
		gate = get_first_driver(out_decl, bitNumber);
		ASSERT(gate != NULL_TREE);
		if (TREE_CODE(gate) != GATE_INSTANCE) {
		    warning
			("specify path for net %s does not connect to a gate primitive",
			 IDENT(out_ident), NULL_CHAR);
		    return;
		}
		switch ((int) TREE_TYPE(gate)) {
		case GATE_AND_TYPE:
		case GATE_NAND_TYPE:
		case GATE_OR_TYPE:
		case GATE_NOR_TYPE:
		case GATE_XOR_TYPE:
		case GATE_XNOR_TYPE:
		case GATE_BUF_TYPE:
		case GATE_NOT_TYPE:
		case GATE_BUFIF0_TYPE:
		case GATE_BUFIF1_TYPE:
		case GATE_NOTIF0_TYPE:
		case GATE_NOTIF1_TYPE:
		case GATE_NMOS_TYPE:
		case GATE_PMOS_TYPE:
		case GATE_RNMOS_TYPE:
		case GATE_RPMOS_TYPE:
		case GATE_CMOS_TYPE:
		case GATE_RCMOS_TYPE:
		case GATE_UDP_TYPE:
		    break;

		case GATE_TRAN_TYPE:
		case GATE_RTRAN_TYPE:
		case GATE_TRANIF0_TYPE:
		case GATE_TRANIF1_TYPE:
		case GATE_RTRANIF0_TYPE:
		case GATE_RTRANIF1_TYPE:
		    error
			("specify paths cannot be assigned to bidirectional switch\n",
			 NULL_CHAR, NULL_CHAR);
		    return;

		case GATE_PULLUP_TYPE:
		case GATE_PULLDN_TYPE:
		    error
			("specify paths cannot be assigned to pullups or pulldowns\n",
			 NULL_CHAR, NULL_CHAR);
		    return;

		default:
		    ASSERT(FALSE);
		}
		if (TREE_CHAIN(GATE_OUTPUT_LIST(gate)) != NULL_TREE) {
		    error
			("specify paths cannot be assigned to buf/not gates driving multiple outputs\n",
			 NULL_CHAR, NULL_CHAR);
		    return;
		}
		add_spec_to_gate(module, gate, paths, outputNumber);
		outputNumber++;
	    }
	}
    }
}

/**********************************************************************
 *
 *      is_driven_by     
 *              - return TRUE if decl[bit] is driven by gate
 *				  bit is ignore if scalar    
 *
 **********************************************************************
 */
int is_driven_by(tree driver, tree decl, int bit)
{
    tree t;
    tree t1;
    int lsb;
    int msb;
    tree surrogate;

    if (TREE_CODE(driver) != GATE_INSTANCE) {
	return (TRUE);
    }
    for (t = GATE_OUTPUT_LIST(driver); t != NULL_TREE; t = TREE_CHAIN(t)) {
	ASSERT(TREE_CODE(t) == TREE_LIST);
	t1 = TREE_PURPOSE(t);
	switch (TREE_CODE(t1)) {
	case BIT_REF:
	    surrogate = BIT_REF_DECL(t1);
	    ASSERT(surrogate != NULL_TREE);
	    if (TREE_CHAIN(surrogate) != decl) {
		return (FALSE);
	    }
	    msb = get_range(BIT_EXPR(t1),
			    IDENTIFIER_POINTER(BIT_REF_NAME(t1)));
	    if (msb == bit) {
		return (TRUE);
	    }
	    break;
	case PART_REF:
	    surrogate = PART_DECL(t1);
	    ASSERT(surrogate != NULL_TREE);
	    if (TREE_CHAIN(surrogate) != decl) {
		return (FALSE);
	    }
	    msb = get_range(PART_MSB_(t1),
			    IDENTIFIER_POINTER(PART_NAME(t1)));
	    lsb = get_range(PART_LSB_(t1),
			    IDENTIFIER_POINTER(PART_NAME(t1)));
	    if (bit >= lsb && bit <= msb) {
		return (TRUE);
	    }
	    break;
	default:
	    surrogate = t1;
	    ASSERT(surrogate != NULL_TREE);
	    if (TREE_CHAIN(surrogate) == decl) {
		return (TRUE);
	    }
	    break;
	}
    }
    return (FALSE);
}

/**********************************************************************
 *
 *      get_driver_count(tree decl, int bit)     
 *              - count the number of drivers on net given its decl 
 *
 **********************************************************************
 */

int get_driver_count(tree decl, int bit)
{
    int count;
    tree t;
    tree driver;

    ASSERT(decl != NULL_TREE);
    count = 0;
    for (t = NET_SOURCE(decl); t; t = NET_SOURCE(t)) {
	driver = NET_ASSIGNMENT(t);
	if (driver != NULL && TREE_CODE(driver) == TREE_LIST &&
	    GATE_TERMINAL_GATE(driver) != NULL &&
	    TREE_CODE(GATE_TERMINAL_GATE(driver)) == GATE_INSTANCE) {
	    driver = GATE_TERMINAL_GATE(driver);
	}
	if (driver == NULL || is_driven_by(driver, decl, bit)) {
	    count++;
	}
    }
    return (count);
}

/**********************************************************************
 *
 *      get_first_driver(tree decl, bit)     
 *              - return the first driver of the net given its decl
 *
 **********************************************************************
 */

tree get_first_driver(tree decl, int bit)
{
    int count;
    tree t;
    tree driver;

    ASSERT(decl != NULL_TREE);
    for (t = NET_SOURCE(decl); t; t = NET_SOURCE(t)) {
	driver = NET_ASSIGNMENT(t);
	if (driver != NULL && TREE_CODE(driver) == TREE_LIST &&
	    GATE_TERMINAL_GATE(driver) != NULL &&
	    TREE_CODE(GATE_TERMINAL_GATE(driver)) == GATE_INSTANCE) {
	    driver = GATE_TERMINAL_GATE(driver);
	}
	if (driver == NULL || is_driven_by(driver, decl, bit)) {
	    return (driver);
	}
    }
    return (NULL_TREE);
}

/**********************************************************************
 *
 *      add_spec_to_gate( tree module, tree driver, tree pathdesc )     
 *              -  add path info to gate
 *                
 *
 **********************************************************************
 */

void
add_spec_to_gate(tree module, tree gate, tree pathdesc, int outputNumber)
{
    tree t;
    tree spec;

    ASSERT(pathdesc != NULL_TREE);
    ASSERT(gate != NULL_TREE);

    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    spec = GATE_PATH_OUTPUT(gate);
    if (spec == NULL_TREE) {
	spec = make_node(PATH_OUTPUT);
	PATH_OUTPUT_GATE(spec) = gate;
	GATE_PATH_OUTPUT(gate) = spec;
	PATH_OUTPUT_VALUE(spec) = X;
	PATH_OUTPUT_CURRENT_VALUE(spec) = X;
	PATH_OUTPUT_PATHDESC(spec) = NULL_TREE;
	PATH_OUTPUT_CONSTRAINTS(spec) = NULL_TREE;
	PATH_OUTPUT_SCB(spec) = BuildSCB(spec, NOLIST);
	TREE_CHAIN(spec) = MODULE_SPECINST(module);
	MODULE_SPECINST(module) = spec;
    }

    t = make_node(TREE_LIST);
    t->common.sub_label = outputNumber;
    TREE_PURPOSE(t) = pathdesc;
    TREE_CHAIN(t) = PATH_OUTPUT_PATHDESC(spec);
    PATH_OUTPUT_PATHDESC(spec) = t;
//      TREE_CHAIN(spec) = MODULE_SPECINST(module);
//      MODULE_SPECINST(module) = spec;



}

/**********************************************************************
 *
 *      pass3_specify( tree spec )      
 *              - perform pass3 setup on specify path. 
 *
 **********************************************************************
 */

void pass3_specify(tree gate)
{
    tree in_list;
    tree t;
    tree pathdesc;
    tree pathdescList;
    tree decl;
    tree constraint;
    tree spec;
    tree ref;
    tree t1;
    tree instance;

    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    spec = GATE_PATH_OUTPUT(gate);
    if (spec == NULL_TREE) {
	return;
    }
    pathdescList = PATH_OUTPUT_PATHDESC(spec);
    if (pathdescList == NULL_TREE) {
	return;
    }

    for (; pathdescList; pathdescList = TREE_CHAIN(pathdescList)) {
	ASSERT(TREE_CODE(pathdescList) == TREE_LIST);
	pathdesc = TREE_PURPOSE(pathdescList);
	ASSERT(pathdesc != NULL_TREE);

	/* find instance */
	for (instance = MODULE_SPECINST(GATE_SCOPE(gate));
	     instance; instance = TREE_CHAIN(instance)) {

	    if (TREE_CODE(instance) != PATH_INSTANCE) {
		continue;
	    }
	    if (PATH_INSTANCE_PATHDESC(instance) == pathdesc) {
		break;
	    }
	}
	ASSERT(instance != NULL);
	constraint = make_node(PATH_CONSTRAINT);
	PATH_CONSTRAINT_OUTPUT_NUMBER(constraint) =
	    pathdescList->common.sub_label;
	PATH_CONSTRAINT_DEPENDENCIES(constraint) = NULL_TREE;
	PATH_CONSTRAINT_INSTANCE(constraint) = instance;
	PATH_CONSTRAINT_PATHDESC(constraint) = pathdesc;
	TREE_CHAIN(constraint) = PATH_OUTPUT_CONSTRAINTS(spec);
	PATH_OUTPUT_CONSTRAINTS(spec) = constraint;
	runtime_error(pathdesc);
	calculate_delays(constraint);
	for (in_list = PATH_INPUTS(pathdesc); in_list;
	     in_list = TREE_CHAIN(in_list)) {
	    t = make_node(TREE_LIST);
	    MODPATH_INPUT_TERMINAL_PARENT(t) = constraint;
	    if (PATH_CONSTRAINT_DEPENDENCIES(constraint) == NULL) {
		PATH_CONSTRAINT_DEPENDENCIES(constraint) = t;
	    } else {
		for (t1 = PATH_CONSTRAINT_DEPENDENCIES(constraint);
		     TREE_CHAIN(t1); t1 = TREE_CHAIN(t1));
		TREE_CHAIN(t1) = t;
	    }
	    ref = TREE_PURPOSE(in_list);
	    ASSERT(ref);
	    switch (TREE_CODE(ref)) {
	    case BIT_REF:
		TREE_PURPOSE(t) =
		    build_bit_ref(BIT_REF_NAME(ref), BIT_EXPR(ref));
		ref = TREE_PURPOSE(t);
		decl = IDENT_CURRENT_DECL(BIT_REF_NAME(ref));
		BIT_REF_DECL(ref) = decl;
		ASSERT(decl != NULL_TREE);
		break;

	    case PART_REF:
		TREE_PURPOSE(t) =
		    build_part_ref(PART_NAME(ref), PART_MSB_(ref),
				   PART_LSB_(ref));
		ref = TREE_PURPOSE(t);
		decl = IDENT_CURRENT_DECL(PART_NAME(ref));
		PART_DECL(ref) = decl;
		ASSERT(decl != NULL_TREE);
		break;

	    default:
		decl = IDENT_CURRENT_DECL(ref);
		ASSERT(decl != NULL_TREE);
		TREE_PURPOSE(t) = decl;
		break;
	    }
	}
    }
}

/**********************************************************************
 *
 *      handle_specify_path     
 *              - check specify path and schedule if needed 
 *                return TRUE if propagation will be handled by
 *                specify_block node
 *
 **********************************************************************
 */

int handle_specify_path(tree gate)
{
    enum logical_value oldValue;
    enum logical_value newValue;
    delay_t delay;
    tree dependency;
    tree constraint;
    tree t;
    time64 latestTime;
    delay_t currentDelay;
    tree spec;
    BOOL result;
    BOOL compareFlag;

    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    spec = GATE_PATH_OUTPUT(gate);
    ASSERT(spec != NULL_TREE);
    ASSERT(!in_initial);


    latestTime.timeh = 0;
    latestTime.timel = 0;

    result = FALSE;
    delay = INT_MAX;
    newValue = GATE_OUTPUT(gate);
    oldValue = PATH_OUTPUT_CURRENT_VALUE(spec);
    ASSERT(PATH_OUTPUT_CONSTRAINTS(spec) != NULL_TREE);
    for (constraint = PATH_OUTPUT_CONSTRAINTS(spec); constraint;
	 constraint = TREE_CHAIN(constraint)) {
	ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
	ASSERT(PATH_CONSTRAINT_DEPENDENCIES(constraint) != NULL_TREE);
	for (t = PATH_CONSTRAINT_DEPENDENCIES(constraint); t;
	     t = TREE_CHAIN(t)) {
	    ASSERT(TREE_CODE(t) == TREE_LIST);
	    dependency = TREE_PURPOSE(t);
	    ASSERT(dependency != NULL_TREE);

	    // this is a hack for now - any bit will activate
	    switch (TREE_CODE(dependency)) {
	    case BIT_REF:
		dependency = BIT_REF_DECL(dependency);
		ASSERT(dependency != NULL_TREE);
		break;
	    case PART_REF:
		dependency = PART_DECL(dependency);
		ASSERT(dependency != NULL_TREE);
		break;
	    }
	    if (CMPTIME64(&latestTime, &DECL_UPDATE_TIME(dependency)) < 0) {
		ASSIGNTIME64(&latestTime, &DECL_UPDATE_TIME(dependency));
		delay =
		    PATH_CONSTRAINT_DELAY(constraint, oldValue, newValue);
	    } else
		if (CMPTIME64(&latestTime, &DECL_UPDATE_TIME(dependency))
		    == 0) {
		currentDelay =
		    PATH_CONSTRAINT_DELAY(constraint, oldValue, newValue);
		if (currentDelay < delay) {
		    delay = currentDelay;
		}
	    }

	}
    }
    ADDTIME6432(&latestTime, &latestTime, delay);
    compareFlag = CMPTIME64(&latestTime, &CurrentTime);
    if (compareFlag >= 0) {

	/* this should work for the wrap around case */

	delay = latestTime.timel - CurrentTime.timel;
	// this is a test kludge
//              if( PATH_OUTPUT_VALUE(spec)==PATH_OUTPUT_CURRENT_VALUE(spec) ) {
	Schedule(delay, PATH_OUTPUT_SCB(spec), 0);
//              } 
	PATH_OUTPUT_VALUE(spec) = newValue;
	result = TRUE;
    } else {
	PATH_OUTPUT_VALUE(spec) = newValue;
	PATH_OUTPUT_CURRENT_VALUE(spec) = newValue;
    }
    return (result);
}

/**********************************************************************
 *
 *      propagate_specify_path( tree pc )       
 *              - pass on specify path output
 *
 **********************************************************************
 */
void propagate_specify_path(tree pc)
{
    tree gate;

    ASSERT(pc != NULL_TREE);
    ASSERT(TREE_CODE(pc) == PATH_OUTPUT);
    gate = PATH_OUTPUT_GATE(pc);
    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    propagate_specify_output(gate, PATH_OUTPUT_VALUE(pc));
    PATH_OUTPUT_CURRENT_VALUE(pc) = PATH_OUTPUT_VALUE(pc);
}

/**********************************************************************
 *
 *      calculate_delays( tree constraint )     
 *              - recalculate the delays for path. 
 *
 **********************************************************************
 */

void calculate_delays(tree constraint)
{
    int delays[6];
    tree t;
    int count;
    tree pathdesc;
    nbits_t *dummy;

    ASSERT(X >= 0 && X <= 3);
    ASSERT(Z >= 0 && Z <= 3);
    ASSERT(ONE >= 0 && ONE <= 3);
    ASSERT(ZERO >= 0 && ZERO <= 3);
    ASSERT(constraint != NULL);
    ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
    pathdesc = PATH_CONSTRAINT_PATHDESC(constraint);
    ASSERT(pathdesc != NULL_TREE);
    ASSERT(TREE_CODE(pathdesc) == PATH_NODE);
    for (t = PATH_DELAYS(pathdesc), count = 0; t;
	 t = TREE_CHAIN(t), count++) {
	ASSERT(count < 6);
	ASSERT(TREE_PURPOSE(t) != NULL_TREE);
	delays[count] = get_delay(TREE_PURPOSE(t));
    }
    switch (count) {
    case 1:
	PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, Z, ONE) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, ONE, Z) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) = delays[0];
	break;
    case 2:
	PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, Z, ONE) = delays[0];

	PATH_CONSTRAINT_DELAY(constraint, ONE, Z) = delays[1];
	PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) = delays[1];
	PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) = delays[1];
	break;
    case 3:
	PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, Z, ONE) = delays[0];

	PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) = delays[1];
	PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) = delays[1];

	PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) = delays[2];
	PATH_CONSTRAINT_DELAY(constraint, ONE, Z) = delays[2];
	break;
    case 6:
	PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) = delays[0];
	PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) = delays[1];
	PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) = delays[2];
	PATH_CONSTRAINT_DELAY(constraint, Z, ONE) = delays[3];
	PATH_CONSTRAINT_DELAY(constraint, ONE, Z) = delays[4];
	PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) = delays[5];
	break;
    default:
	ASSERT(FALSE);
    }
    setXPathConstraints(constraint);

}

/**********************************************************************
 *
 *      setXPathConstraints( tree constraint )   
 *              - calculate the x delays for paths 
 *
 **********************************************************************
 */
void setXPathConstraints(tree constraint)
{
    ASSERT(constraint != NULL);
    ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);

    PATH_CONSTRAINT_DELAY(constraint, ZERO, X) =
	minimum(PATH_CONSTRAINT_DELAY(constraint, ZERO, Z),
		PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE));
    PATH_CONSTRAINT_DELAY(constraint, ONE, X) =
	minimum(PATH_CONSTRAINT_DELAY(constraint, ONE, Z),
		PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO));
    PATH_CONSTRAINT_DELAY(constraint, Z, X) =
	minimum(PATH_CONSTRAINT_DELAY(constraint, Z, ONE),
		PATH_CONSTRAINT_DELAY(constraint, Z, ZERO));
    PATH_CONSTRAINT_DELAY(constraint, X, ZERO) =
	maximum(PATH_CONSTRAINT_DELAY(constraint, Z, ZERO),
		PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO));
    PATH_CONSTRAINT_DELAY(constraint, X, ONE) =
	maximum(PATH_CONSTRAINT_DELAY(constraint, Z, ONE),
		PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE));
    PATH_CONSTRAINT_DELAY(constraint, X, Z) =
	maximum(PATH_CONSTRAINT_DELAY(constraint, ONE, Z),
		PATH_CONSTRAINT_DELAY(constraint, ZERO, Z));

}

/**********************************************************************
 *
 *      specify_update_timings( tree module )   
 *              - recalculate the delays for paths in module and 
 *                      timing checks. 
 *
 **********************************************************************
 */

void specify_update_timings(tree module)
{
    tree t;
    tree constraint;

    ASSERT(module != NULL_TREE);
    ASSERT(TREE_CODE(module) == MODULE_BLOCK);

    for (t = MODULE_SPECINST(module); t; t = TREE_CHAIN(t)) {
	if (TREE_CODE(t) == PATH_OUTPUT) {
	    ASSERT(PATH_OUTPUT_CONSTRAINTS(t) != NULL_TREE);
	    for (constraint = PATH_OUTPUT_CONSTRAINTS(t); constraint;
		 constraint = TREE_CHAIN(constraint)) {
		ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
		calculate_delays(constraint);
	    }
	} else if (TREE_CODE(t) == TIMING_CHECK) {
	    calculate_params(t);
	}
    }

}

/**********************************************************************
 *
 *      build_timing_event      
 *              - build parse node for an event related to a timing check. 
 *
 **********************************************************************
 */

tree build_timing_event(int edge, tree expression, tree condition)
{
    tree t;

    ASSERT(expression != NULL_TREE);
    t = make_node(TIMING_EVENT);
    TIMING_EVENT_EXPRESSION(t) = expression;
    TIMING_EVENT_CONDITION(t) = condition;
    TIMING_EVENT_EDGESPEC(t) = edge;
    return (t);
}

/**********************************************************************
 *
 *      build_timing_statement  
 *              - build parse node for a timing check. 
 *
 **********************************************************************
 */

tree
build_timing_statement(int checkType, tree event1, tree event2,
		       tree param1, tree param2, tree notifier)
{
    tree t;

    ASSERT(checkType == SETUP ||
	   checkType == HOLD ||
	   checkType == WIDTH ||
	   checkType == PERIOD ||
	   checkType == SKEW ||
	   checkType == RECOVERY || checkType == SETUPHOLD);
    t = make_node(CHECK_SPEC);
    CHECK_SPEC_EVENT1(t) = event1;
    CHECK_SPEC_EVENT2(t) = event2;
    CHECK_SPEC_PARAM1(t) = param1;
    CHECK_SPEC_PARAM2(t) = param2;
    CHECK_SPEC_NOTIFIER(t) = notifier;
    CHECK_SPEC_CHECKTYPE(t) = checkType;
    return (t);

}

/**********************************************************************
 *
 *      pass3_timing_check( tree check )        
 *              - perform pass3 setup on timing checks. 
 *
 **********************************************************************
 */

void pass3_timing_check(tree check, tree module)
{
    tree event1;
    tree event2;
    tree condition1;
    tree condition2;
    unsigned int edgeSpec1;
    unsigned int edgeSpec2;
    tree expression;
    tree timing_check;
    unsigned int msb1;
    unsigned int msb2;
    unsigned int lsb1;
    unsigned int lsb2;
    tree decl1;
    tree decl2;
    tree expr1;
    tree expr2;
    tree notifier;
    unsigned int i;
    unsigned int j;
    unsigned int temp;

    ASSERT(check != NULL_TREE);
    ASSERT(TREE_CODE(check) == CHECK_SPEC);
    event1 = CHECK_SPEC_EVENT1(check);
    ASSERT(event1 != NULL_TREE);
    event2 = CHECK_SPEC_EVENT2(check);	// note event2 may be null
    runtime_error(check);

    if (CHECK_SPEC_NOTIFIER(check)) {
//                CHECK_SPEC_NOTIFIER(check) = 
	notifier = make_notifier(CHECK_SPEC_NOTIFIER(check));
    } else
	notifier = NULL_TREE;

    condition1 = TIMING_EVENT_CONDITION(event1);
    edgeSpec1 = TIMING_EVENT_EDGESPEC(event1);
    if (CHECK_SPEC_CHECKTYPE(check) == WIDTH) {
	condition2 = condition1;
	edgeSpec2 = edgeSpec1 ^ 0xffff;
    } else if (event2) {
	condition2 = TIMING_EVENT_CONDITION(event2);
	edgeSpec2 = TIMING_EVENT_EDGESPEC(event2);
    } else {
	condition2 = NULL_TREE;
	edgeSpec2 = 0;
    }

    expression = TIMING_EVENT_EXPRESSION(event1);
    ASSERT(expression != NULL_TREE);
    switch (TREE_CODE(expression)) {
    case BIT_REF:
	decl1 = BIT_REF_DECL(expression);
	msb1 = 0;
	lsb1 = 0;
	break;
    case NET_SCALAR_DECL:
	msb1 = 0;
	lsb1 = 0;
	decl1 = expression;
	break;
    case PART_REF:
	decl1 = PART_DECL(expression);
	msb1 = get_range(PART_MSB_(expression),
			 IDENTIFIER_POINTER(DECL_NAME(decl1)));
	lsb1 = get_range(PART_LSB_(expression),
			 IDENTIFIER_POINTER(DECL_NAME(decl1)));
	break;
    case NET_VECTOR_DECL:
	decl1 = expression;
	msb1 = MSB(decl1);
	lsb1 = LSB(decl1);
	break;
    default:
	ASSERT(FALSE);
    }

    decl2 = NULL_TREE;
    msb2 = 0;
    lsb2 = 0;
    if (event2) {
	expression = TIMING_EVENT_EXPRESSION(event2);
	ASSERT(expression != NULL_TREE);
	switch (TREE_CODE(expression)) {
	case BIT_REF:
	    decl2 = BIT_REF_DECL(expression);
	    msb2 = 0;
	    lsb2 = 0;
	    break;
	case NET_SCALAR_DECL:
	    msb2 = 0;
	    lsb2 = 0;
	    decl2 = expression;
	    break;
	case PART_REF:
	    decl2 = PART_DECL(expression);
	    msb2 = get_range(PART_MSB_(expression),
			     IDENTIFIER_POINTER(DECL_NAME(decl2)));
	    lsb2 = get_range(PART_LSB_(expression),
			     IDENTIFIER_POINTER(DECL_NAME(decl2)));
	    break;
	case NET_VECTOR_DECL:
	    decl2 = expression;
	    msb2 = MSB(decl2);
	    lsb2 = LSB(decl2);
	    break;
	default:
	    ASSERT(FALSE);
	}
    }

    if (lsb1 > msb1) {
	temp = msb1;
	msb1 = lsb1;
	lsb1 = temp;
    }
    if (lsb2 > msb2) {
	temp = msb2;
	msb2 = lsb2;
	lsb2 = temp;
    }

    for (i = lsb1; i <= msb1; i++) {
	if (lsb1 == msb1) {
	    expr1 = decl1;
	} else {
	    expr1 = build_bit_ref(decl1, build_int_cst(i));
	}
	for (j = lsb2; j <= msb2; j++) {
	    if (lsb2 == msb2) {
		expr2 = decl2;
	    } else {
		expr2 = build_bit_ref(decl2, build_int_cst(j));
	    }
	    timing_check = make_timing_check(module, check, edgeSpec1,
					     expr1, condition1, edgeSpec2,
					     expr2, condition2, notifier);
	}
    }




}

/**********************************************************************
 *
 *      make_timing_check
 *              - build a timing check node in pass3. 
 *
 **********************************************************************
 */
tree
make_timing_check(tree module, tree check, unsigned int edgespec1,
		  tree expression1, tree condition1,
		  unsigned int edgespec2, tree expression2,
		  tree condition2, tree notifier)
{
    tree t;
    time64 zero_time = { 0, 0 };
    Marker *markerList;

    markerList = NULL;

    ASSERT(check != NULL_TREE);
    ASSERT(TREE_CODE(check) == CHECK_SPEC);
    ASSERT(expression1 != NULL);

    t = make_node(TIMING_CHECK);

    TIMING_CHECK_CONDITION(t, 0) = condition1;
    TIMING_CHECK_CONDITION(t, 1) = condition2;
    TIMING_CHECK_CURRENTVALUE(t, 0) = X;
    TIMING_CHECK_CURRENTVALUE(t, 1) = X;
    TIMING_CHECK_CHECKSPEC(t) = check;
    TIMING_CHECK_EXPRESSION(t, 0) = build_tree_list(expression1, t);
    if (expression2 != NULL) {
	TIMING_CHECK_EXPRESSION(t, 1) = build_tree_list(expression2, t);
    } else {
	TIMING_CHECK_EXPRESSION(t, 1) = NULL;
    }

    ASSIGNTIME64(&TIMING_CHECK_CHANGETIME(t, 0), &zero_time);
    ASSIGNTIME64(&TIMING_CHECK_CHANGETIME(t, 1), &zero_time);


    TIMING_CHECK_EDGESPEC(t, 0) = edgespec1;
    TIMING_CHECK_EDGESPEC(t, 1) = edgespec2;
//        TIMING_CHECK_NOTIFIER(t) = CHECK_SPEC_NOTIFIER(check);
    TIMING_CHECK_NOTIFIER(t) = notifier;
    TIMING_CHECK_EXPRESSIONCODE(t, 0) = pass3_expr_marker
	(expression1, &markerList, (enum marker_flags) (M_FIXED + M_IDLE),
	 t, NULL_TREE);
    if (expression2 != NULL_TREE) {
	TIMING_CHECK_EXPRESSIONCODE(t, 1) = pass3_expr_marker
	    (expression2, &markerList,
	     (enum marker_flags) (M_FIXED + M_IDLE), t, NULL_TREE);
    } else {
	TIMING_CHECK_EXPRESSIONCODE(t, 1) = NULL;
    }
    if (condition1) {
	TIMING_CHECK_CONDITIONCODE(t, 0) = pass3_expr(condition1);
	if (!check_scalar(condition1)) {
	    error("the condition clause must be scalar", NULL_CHAR,
		  NULL_CHAR);
	}
    } else {
	TIMING_CHECK_CONDITIONCODE(t, 0) = NULL;
    }
    if (condition2) {
	TIMING_CHECK_CONDITIONCODE(t, 1) = pass3_expr(condition2);
	if (!check_scalar(condition2)) {
	    error("the condition clause must be scalar", NULL_CHAR,
		  NULL_CHAR);
	}
    } else {
	TIMING_CHECK_CONDITIONCODE(t, 1) = NULL;
    }
    calculate_params(t);


    ASSERT(module != NULL_TREE);
    ASSERT(TREE_CODE(module) == MODULE_BLOCK);
    TREE_CHAIN(t) = MODULE_SPECINST(module);
    MODULE_SPECINST(module) = t;
    TIMING_CHECK_MODULE(t) = module;
    return (t);

}

/**********************************************************************
 *
 *      make_notifier   
 *              - create notifier node. 
 *
 **********************************************************************
 */
tree make_notifier(tree ident)
{
    tree decl;
    tree notifier;

    ASSERT(ident != NULL_TREE);
    ASSERT(TREE_CODE(ident) == IDENTIFIER_NODE);
    decl = IDENT_CURRENT_DECL(ident);
    ASSERT(decl != NULL_TREE);
    if (TREE_CODE(decl) != REG_SCALAR_DECL) {
	error("%s must be a single bit register", IDENT(ident), NULL_CHAR);
	return (error_mark_node);
    }
    notifier = make_node(NOTIFIER_NODE);
    NOTIFIER_DECL(notifier) = decl;
    NOTIFIER_TIMESTAMP(notifier).timel = 0;
    NOTIFIER_TIMESTAMP(notifier).timeh = 0;
    return (notifier);
}

/**********************************************************************
 *
 *      calculate_params( tree timing_check )   
 *              - recalculate timing check parameters. 
 *
 **********************************************************************
 */
void calculate_params(tree t)
{
    tree check;
    nbits_t *dummy;

    ASSERT(t != NULL_TREE);
    ASSERT(TREE_CODE(t) == TIMING_CHECK);
    check = TIMING_CHECK_CHECKSPEC(t);
    ASSERT(check != NULL_TREE);
    ASSERT(TREE_CODE(check) == CHECK_SPEC);
    TIMING_CHECK_PARAM1(t) = get_delay(CHECK_SPEC_PARAM1(check));
    if (TIMING_CHECK_PARAM1(t) < 0) {
	TIMING_CHECK_PARAM1(t) = 0;
    }
    if (CHECK_SPEC_PARAM2(check)) {
	TIMING_CHECK_PARAM2(t) = get_delay(CHECK_SPEC_PARAM2(check));
	if (TIMING_CHECK_PARAM2(t) < 0) {
	    TIMING_CHECK_PARAM2(t) = 0;
	}
    }
}

/**********************************************************************
 *
 *      check_scalar
 *              - verify the given node  is scalar  
 *
 **********************************************************************
 */
int check_scalar(tree node)
{
    tree t;

    t = node;
    ASSERT(node != NULL_TREE);
    if (TREE_CODE(t) == IDENTIFIER) {
	t = IDENT_CURRENT_DECL(node);
	ASSERT(t != NULL_TREE);
    }

    if (TREE_CODE(t) != NET_SCALAR_DECL && TREE_NBITS(t) != 1) {
	return (FALSE);
    }
    return (TRUE);
}


/**********************************************************************
 *
 *      check_one_bit_constant
 *              - verify the given node is a one bit constant
 *
 **********************************************************************
 */
tree check_one_bit_constant(tree constant)
{
    ASSERT(constant != NULL_TREE);
    ASSERT(TREE_CODE(constant) == INTEGER_CST
	   || TREE_CODE(constant) == BIT_CST);
    if (TREE_CODE(constant) == INTEGER_CST) {
	if (INT_CST_DATA(constant) != 0 && INT_CST_DATA(constant) != 1) {
	    error("expecting a single bit constant", NULL_CHAR, NULL_CHAR);
	    return (error_mark_node);
	}
    } else {
	if (TREE_NBITS(constant) != 1 || !TREE_CONSTANT_ATTR(constant)) {
	    error("expecting a single bit constant", NULL_CHAR, NULL_CHAR);
	    return (error_mark_node);
	}
    }
    return (constant);
}

/**********************************************************************
 *
 *      timingCheck
 *              - perform timing check
 *
 **********************************************************************
 */


void timingCheck(tree node)
{
    enum logical_value bit1;
    enum logical_value bit2;
    enum logical_value was_bit1;
    enum logical_value was_bit2;
    int event1;
    int event2;
    int result;
    tree check;
    char *name;

    ASSERT(TREE_CODE(node) == TIMING_CHECK);
    check = TIMING_CHECK_CHECKSPEC(node);
    ASSERT(check != NULL_TREE);
    was_bit1 = (enum logical_value) TIMING_CHECK_CURRENTVALUE(node, 0);
    was_bit2 = (enum logical_value) TIMING_CHECK_CURRENTVALUE(node, 1);
    bit1 = eval_bit(TIMING_CHECK_EXPRESSIONCODE(node, 0));
    if (TIMING_CHECK_EXPRESSIONCODE(node, 1)) {
	bit2 = eval_bit(TIMING_CHECK_EXPRESSIONCODE(node, 1));
    } else {
	bit2 = bit1;
    }

    if (was_bit1 != bit1 &&
	(edgeMask(was_bit1, bit1) & TIMING_CHECK_EDGESPEC(node, 0)) &&
	(TIMING_CHECK_CONDITIONCODE(node, 0) == NULL ||
	 test(TIMING_CHECK_CONDITIONCODE(node, 0)))) {
	event1 = TRUE;
    } else {
	event1 = FALSE;
    }

    if (was_bit2 != bit2 &&
	(edgeMask(was_bit2, bit2) & TIMING_CHECK_EDGESPEC(node, 1)) &&
	(TIMING_CHECK_CONDITIONCODE(node, 1) == NULL ||
	 test(TIMING_CHECK_CONDITIONCODE(node, 1)))) {
	event2 = TRUE;
    } else {
	event2 = FALSE;
    }

    switch (CHECK_SPEC_CHECKTYPE(check)) {
    case SETUP:
	name = "setup";
	result = setupCheck(node, event1, event2);
	break;
    case HOLD:
	name = "hold";
	result = holdCheck(node, event1, event2);
	break;
    case WIDTH:
	name = "width";
	result = widthCheck(node, event1, event2);
	break;
    case PERIOD:
	name = "period";
	result = periodCheck(node, event1, event2);
	break;
    case SKEW:
	name = "skew";
	result = skewCheck(node, event1, event2);
	break;
    case RECOVERY:
	name = "recovery";
	result = recoveryCheck(node, event1, event2);
	break;
    case SETUPHOLD:
	name = "setuphold";
	result = setupholdCheck(node, event1, event2);
	break;
    default:
	ASSERT(FALSE);
	return;
    }

    if (!result) {
	printf_V("  \"%s\", %d: Timing violation in ",
		 STMT_SOURCE_FILE(check), STMT_SOURCE_LINE(check));
	print_scope((handle_t) 1, TIMING_CHECK_MODULE(node));
	printf_V("\n$%s( ", name);
	trace_timing_event(TIMING_EVENT_EXPRESSION
			   (CHECK_SPEC_EVENT1(check)),
			   TIMING_CHECK_EDGESPEC(node, 0),
			   TIMING_EVENT_CONDITION(CHECK_SPEC_EVENT1
						  (check)));
	printf_V(":");
	if (CHECK_SPEC_CHECKTYPE(check) == SETUPHOLD && event1) {
	    print_time(&CurrentTime);
	} else if (CHECK_SPEC_CHECKTYPE(check) == HOLD && event1) {
	    print_time(&CurrentTime);
	} else {
	    print_time(&TIMING_CHECK_CHANGETIME(node, 0));
	}

	printf_V(", ");
	if (TIMING_CHECK_EXPRESSIONCODE(node, 1)) {
	    trace_timing_event(TIMING_EVENT_EXPRESSION
			       (CHECK_SPEC_EVENT2(check)),
			       TIMING_CHECK_EDGESPEC(node, 1),
			       TIMING_EVENT_CONDITION(CHECK_SPEC_EVENT2
						      (check)));
	}
	printf_V(":");
	if (CHECK_SPEC_CHECKTYPE(check) == SETUPHOLD && !event2) {
	    print_time(&TIMING_CHECK_CHANGETIME(node, 1));
	} else {
	    print_time(&CurrentTime);
	}

	if (CHECK_SPEC_PARAM1(check)) {
	    printf_V(", ");
	    print_expr(CHECK_SPEC_PARAM1(check));
	    printf_V(":%d", TIMING_CHECK_PARAM1(node));
	}
	if (CHECK_SPEC_PARAM2(check)) {
	    printf_V(", ");
	    print_expr(CHECK_SPEC_PARAM2(check));
	    printf_V(":%d", TIMING_CHECK_PARAM2(node));
	}

	printf_V(" );\n");
	if (TIMING_CHECK_NOTIFIER(node)) {
	    toggle_notifier(TIMING_CHECK_NOTIFIER(node));
	}
    }

    if (event1) {
	ASSIGNTIME64(&TIMING_CHECK_CHANGETIME(node, 0), &CurrentTime);
    }
    if (event2) {
	ASSIGNTIME64(&TIMING_CHECK_CHANGETIME(node, 1), &CurrentTime);
    }
    TIMING_CHECK_CURRENTVALUE(node, 0) = bit1;
    TIMING_CHECK_CURRENTVALUE(node, 1) = bit2;
}

/**********************************************************************
 *
 *      toggle_notifier 
 *              - toggle notifier expression
 *
 **********************************************************************
 */

void toggle_notifier(tree notifier)
{
    Group *g;
    tree decl;

    ASSERT(notifier != NULL_TREE);
    /* only allow notifier to be toggled once a tick */
    if (CMPTIME64(&NOTIFIER_TIMESTAMP(notifier), &CurrentTime) == 0) {
	return;
    }
    NOTIFIER_TIMESTAMP(notifier) = CurrentTime;
    decl = NOTIFIER_DECL(notifier);
    ASSERT(decl != NULL_TREE);
    eval_1(decl);
    g = *(R - 1);
    if (BVAL(g) & 1) {
	if (AVAL(g) & 1) {
	    AVAL(g) = 0;
	    BVAL(g) = 0;
	}
    } else {
	AVAL(g) = AVAL(g) ^ 1;
    }
    store(decl, notifier);
}

/**********************************************************************
 *
 *      eval_bit
 *              - evaluate a 1 bit expression
 *
 **********************************************************************
 */

static enum logical_value eval_bit(tree * code)
{
    Group *g;
    int result;

    ASSERT(code != NULL);
    eval(code);

    g = *--R;
    result = ((BVAL(g) & 0x1) << 1) | (AVAL(g) & 0x1);
    return ((enum logical_value) result);

}

/**********************************************************************
 *
 *      setupCheck
 *              - perform setup timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int setupCheck(tree node, int event1, int event2)
{
    time64 time;

    if (event2) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    return (TRUE);
	}
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if (CMPTIME64(&time, &CurrentTime) > 0) {
	    return (FALSE);
	}
    }
    return (TRUE);

}


/**********************************************************************
 *
 *      holdCheck
 *              - perform hold timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int holdCheck(tree node, int event1, int event2)
{
    time64 time;
    int zeroHoldCheck;

    if (event2) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    return (TRUE);
	}
	zeroHoldCheck = TIMING_CHECK_PARAM1(node) == 0;
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if ((event1 && !zeroHoldCheck)
	    || (CMPTIME64(&time, &CurrentTime) > 0)) {
	    return (FALSE);
	}
    }
    return (TRUE);

}

/**********************************************************************
 *
 *      widthCheck
 *              - perform width timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int widthCheck(tree node, int event1, int event2)
{
    time64 time;

    if (event2) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    return (TRUE);
	}
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if (CMPTIME64(&time, &CurrentTime) > 0) {
	    return (FALSE);
	}
    }
    return (TRUE);

}

/**********************************************************************
 *
 *      periodCheck
 *              - perform period timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int periodCheck(tree node, int event1, int event2)
{
    time64 time;

    if (event1) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    return (TRUE);
	}
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if (CMPTIME64(&time, &CurrentTime) > 0) {
	    return (FALSE);
	}
    }
    return (TRUE);

}

/**********************************************************************
 *
 *      skewCheck
 *              - perform skew timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int skewCheck(tree node, int event1, int event2)
{
    time64 time;

    if (event2) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    return (TRUE);
	}
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if ((CMPTIME64(&time, &CurrentTime) < 0)) {
	    return (FALSE);
	}
    }
    return (TRUE);

}

/**********************************************************************
 *
 *      recoveryCheck
 *              - perform recovery timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int recoveryCheck(tree node, int event1, int event2)
{
    time64 time;

    if (event2) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    return (TRUE);
	}
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if (CMPTIME64(&time, &CurrentTime) > 0) {
	    return (FALSE);
	}
    }
    return (TRUE);

}

/**********************************************************************
 *
 *      setupholdCheck
 *              - perform setuphold timing check
 *                      returns TRUE on pass.
 **********************************************************************
 */
int setupholdCheck(tree node, int event1, int event2)
{
    time64 time;
    int zeroHoldCheck;

    if (event1) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 1));
	if (time.timel == 0 && time.timeh == 0) {
	    goto skip;
	}
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM1(node));
	if (CMPTIME64(&time, &CurrentTime) > 0) {
	    return (FALSE);
	}
    }
  skip:
    if (event2) {
	ASSIGNTIME64(&time, &TIMING_CHECK_CHANGETIME(node, 0));
	if (time.timel == 0 && time.timeh == 0) {
	    goto skip1;
	}
	zeroHoldCheck = TIMING_CHECK_PARAM2(node) == 0;
	ADDTIME6432(&time, &time, TIMING_CHECK_PARAM2(node));
	if ((event1 && !zeroHoldCheck)
	    || (CMPTIME64(&time, &CurrentTime) > 0)) {
	    return (FALSE);
	}
    }
  skip1:
    return (TRUE);

}

/**********************************************************************
 *
 *      trace_timing_event
 *              - print out trace of timing event
 **********************************************************************
 */
void trace_timing_event(tree event, unsigned int edge, tree condition)
{
    int first;

    ASSERT(event != NULL_TREE);
    if (edge == posedgeMask) {
	printf_V("(posedge ");
	print_expr(event);
	printf_V(")");
    } else if (edge == negedgeMask) {
	printf_V("(negedge ");
	print_expr(event);
	printf_V(")");
    } else if (edge == alledgeMask) {
	print_expr(event);
    } else {
	first = TRUE;
	printf_V("edge[");
	if (edgeMask(0, X) & edge) {
	    if (!first) {
		printf_V(",");
	    }
	    printf_V("0x");
	    first = FALSE;
	}
	if (edgeMask(0, 1) & edge) {
	    if (!first) {
		printf_V(",");
	    }
	    printf_V("01");
	    first = FALSE;
	}
	if (edgeMask(X, 1) & edge) {
	    if (!first) {
		printf_V(",");
	    }
	    printf_V("x1");
	    first = FALSE;
	}
	if (edgeMask(X, 0) & edge) {
	    if (!first) {
		printf_V(",");
	    }
	    printf_V("x0");
	    first = FALSE;
	}
	if (edgeMask(1, 0) & edge) {
	    if (!first) {
		printf_V(",");
	    }
	    printf_V("10");
	    first = FALSE;
	}
	if (edgeMask(1, X) & edge) {
	    if (!first) {
		printf_V(",");
	    }
	    printf_V("1x");
	    first = FALSE;
	}
	printf_V("] ");
	print_expr(event);
    }
    if (condition) {
	printf_V("&&&(");
	print_expr(condition);
	printf_V(")");
    }
}
