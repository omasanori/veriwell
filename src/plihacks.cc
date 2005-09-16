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

/******************************************************************************
 *
 *	plihacks.c
 *		this is a set of useful pli calls 	
 *
 ******************************************************************************/
#define PLIHACKS_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "veriuser.h"
#include "acc_user.h"
#include "glue.h"
#include "plihacks.h"
#include "vtypes.h"


#define HASH_SIZE	(4096)
#define	HASH(h)	((((long)(h)>>16) ^ ((long)(h)&0xffff))%HASH_SIZE);

typedef struct LogEntry_tag {
    handle object;
    struct LogEntry_tag *next;
} LogEntry_t;

static int depth;
static int declList[] =
    { accNamedEvent, accTimeVar, accIntegerVar, accRealVar, accRegister,
0 };
static int enableReason = FALSE;
static LogEntry_t *hashTable[HASH_SIZE];

/*************************************************************************

	$$abort
		-	this routine will execute an exit(2) system call

**************************************************************************/

int abort_call(int data, int reason)
{

    acc_initialize();

    shell_exit(2);

    acc_close();
    return (0);

}

/*************************************************************************

	push	
		- increase the current depth	

**************************************************************************/
static void push()
{
    ASSERT(depth >= 0);
    depth++;
}

/*************************************************************************

	pop	
		- decrease the current depth	

**************************************************************************/
static void pop()
{
    ASSERT(depth > 0);
    depth--;
}

/*************************************************************************

	indent	
		- indent the correct number of spaces for the given level	

**************************************************************************/
static void indent()
{
    int count;

    for (count = 0; count < depth; count++) {
	io_printf("    ");
    }
}

/*************************************************************************

	DumpPath	
		- output all information about the given module	path

**************************************************************************/
static void DumpPath(handle object)
{
    double d1;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    handle terminal;
    handle parent;
    handle in1;
    handle out1;


    push();

    indent();
    parent = acc_handle_parent(object);
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    indent();
    if (acc_handle_condition(object)) {
	io_printf("- condition is present\n");
    } else {
	io_printf("- condition is not present\n");
    }

    indent();
    io_printf("- polarity is ");
    switch (acc_fetch_polarity(object)) {
    case accPositive:
	io_printf("accPositive");
	break;
    case accNegative:
	io_printf("accNegative");
	break;
    case accUnknown:
	io_printf("accUnknown");
	break;
    default:
	io_printf("unknown");
    }
    io_printf("\n");

    acc_fetch_delays(object, &d1, &d2, &d3, &d4, &d5, &d6);
    indent();
    io_printf("- delay1 = %g\n", d1);
    indent();
    io_printf("- delay2 = %g\n", d2);
    indent();
    io_printf("- delay3 = %g\n", d3);
    indent();
    io_printf("- delay4 = %g\n", d4);
    indent();
    io_printf("- delay5 = %g\n", d5);
    indent();
    io_printf("- delay6 = %g\n", d6);

    indent();
    io_printf("=> acc_handle_pathin()\n");
    in1 = acc_handle_pathin(object);
    DumpObject(in1);

    indent();
    io_printf("=> acc_handle_pathout()\n");
    out1 = acc_handle_pathout(object);
    DumpObject(out1);

    // test acc_handle_modpath
    if (acc_handle_modpath
	(parent, acc_fetch_name(in1), acc_fetch_name(out1))
	!= object) {
	tf_error("acc_handle_modpath() test failure");
    }

    indent();
    io_printf("=> acc_next_output()\n");
    terminal = null;
    while (TRUE) {
	terminal = acc_next_output(object, terminal);
	if (terminal == null) {
	    break;
	}
	DumpObject(terminal);
    }
    indent();
    io_printf("=> acc_next_input()\n");
    terminal = null;
    while (TRUE) {
	terminal = acc_next_input(object, terminal);
	if (terminal == null) {
	    break;
	}
	DumpObject(terminal);
    }
    pop();
}

/*************************************************************************

	DumpTimingCheck	
		- output all information about the given timing check 

**************************************************************************/
static void DumpTimingCheck(handle object)
{
    double d1;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    handle parent;
    handle arg1;
    handle arg2;
    int edge1;
    int edge2;

    push();

    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));


    acc_fetch_delays(object, &d1, &d2, &d3, &d4, &d5, &d6);
    indent();
    io_printf("- delay1 = %g\n", d1);

    arg1 = acc_handle_tchkarg1(object);
    if (arg1 != null) {
	edge1 = acc_fetch_edge(arg1);
    }

    arg2 = acc_handle_tchkarg2(object);
    if (arg2 != null) {
	edge2 = acc_fetch_edge(arg2);
    }

    if (object != acc_handle_tchk(parent, acc_fetch_fulltype(object),
				  acc_fetch_name(arg1), edge1,
				  acc_fetch_name(arg2), edge2)) {
	tf_error("ERROR: acc_handle_tchk() test failure");
    }

    indent();
    arg1 = acc_handle_tchkarg1(object);
    io_printf("- arg 1 is %s\n", acc_fetch_name(arg1));
    if (arg1) {
	DumpObject(arg1);
    }

    indent();
    arg2 = acc_handle_tchkarg2(object);
    io_printf("- arg 2 is %s\n", acc_fetch_name(arg2));
    if (arg2) {
	DumpObject(arg2);
    }

    pop();
}

/*************************************************************************

	DumpParam	
		- output all information about the given param 

**************************************************************************/
static void DumpParam(handle object)
{
    handle parent;
    handle newObject;
    int type;

    push();

    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    type = acc_fetch_paramtype(object);
    indent();
    io_printf("- is type ");
    switch (type) {
    case accIntegerParam:
	io_printf("accIntegerParam");
	break;
    case accRealParam:
	io_printf("accRealParam");
	break;
    case accStringParam:
	io_printf("accStringParam");
	break;
    default:
	io_printf("unknown");
    }
    io_printf("\n");

    indent();
    switch (type) {
    case accIntegerParam:
	io_printf("- value is %d\n", (int) acc_fetch_paramval(object));
	break;
    case accRealParam:
	io_printf("- value is %f\n", acc_fetch_paramval(object));
	break;
    case accStringParam:
	io_printf("- value is %s\n",
		  (char *) (int) acc_fetch_paramval(object));
	break;
    }

    pop();
}

/*************************************************************************

	DumpDecl	
		- output all information about the given Decl 

**************************************************************************/
static void DumpDecl(handle object)
{
    handle parent;
    handle newObject;

    push();

    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));


    if (acc_fetch_type(object) != accNamedEvent) {
	indent();
	io_printf("value(b) = %s\n", acc_fetch_value(object, "%b"));
	indent();
	io_printf("value(o) = %s\n", acc_fetch_value(object, "%o"));
	indent();
	io_printf("value(h) = %s\n", acc_fetch_value(object, "%h"));
	indent();
	io_printf("value(d) = %s\n", acc_fetch_value(object, "%d"));
    }

    pop();
}

/*************************************************************************

	DumpReg	
		- output all information about the given Reg 

**************************************************************************/
static void DumpReg(handle object)
{
    int msb;
    int lsb;
    handle parent;
    handle newObject;

    push();

    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    indent();
    io_printf("- is ");
    if (acc_object_of_type(object, accScalar)) {
	io_printf("accScalar, ");
    }
    if (acc_object_of_type(object, accVector)) {
	io_printf("accVector, ");
    }
    io_printf("\n");

    if (acc_fetch_size(object) > 1) {
	indent();
	acc_fetch_range(object, &msb, &lsb);
	io_printf("- range is [%d:%d]\n", msb, lsb);
    }

    indent();
    io_printf("- size is %d\n", acc_fetch_size(object));

    indent();
    io_printf("value(b) = %s\n", acc_fetch_value(object, "%b"));
    indent();
    io_printf("value(o) = %s\n", acc_fetch_value(object, "%o"));
    indent();
    io_printf("value(h) = %s\n", acc_fetch_value(object, "%h"));
    indent();
    io_printf("value(d) = %s\n", acc_fetch_value(object, "%d"));

    newObject = null;
    while (TRUE) {
	newObject = acc_next_load(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    pop();
}

/*************************************************************************

	DumpNet	
		- output all information about the given Net 

**************************************************************************/
static void DumpNet(handle object)
{
    int msb;
    int lsb;
    handle parent;
    handle newObject;

    push();

    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    indent();
    io_printf("- is ");
    if (acc_object_of_type(object, accScalar)) {
	io_printf("accScalar, ");
    }
    if (acc_object_of_type(object, accVector)) {
	io_printf("accVector, ");
    }
    if (acc_object_of_type(object, accCollapsedNet)) {
	io_printf("accCollapsedNet, ");
    }
    if (acc_object_of_type(object, accExpandedVector)) {
	io_printf("accExpandedVector, ");
    }
    if (acc_object_of_type(object, accUnexpandedVector)) {
	io_printf("accUnexpandedVector, ");
    }
    if (acc_object_of_type(object, accScope)) {
	io_printf("accScope, ");
    }
    if (acc_object_of_type(object, accModPathHasIfnone)) {
	io_printf("accModPathHasIfnone, ");
    }
    io_printf("\n");

    if (acc_fetch_size(object) > 1) {
	indent();
	acc_fetch_range(object, &msb, &lsb);
	io_printf("- range is [%d:%d]\n", msb, lsb);
    }

    indent();
    io_printf("- size is %d\n", acc_fetch_size(object));

    indent();
    io_printf("value(b) = %s\n", acc_fetch_value(object, "%b"));
    indent();
    io_printf("value(o) = %s\n", acc_fetch_value(object, "%o"));
    indent();
    io_printf("value(h) = %s\n", acc_fetch_value(object, "%h"));
    indent();
    io_printf("value(d) = %s\n", acc_fetch_value(object, "%d"));

    newObject = acc_handle_simulated_net(object);
    indent();
    io_printf("=> acc_handle_simulated_net()\n");
    DumpObject(newObject);

    indent();
    io_printf("=> acc_next_driver()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_driver(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    indent();
    io_printf("=> acc_next_load()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_load(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    indent();
    io_printf("=> acc_next_cell_load()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_cell_load(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    pop();
}

/*************************************************************************

	DumpPrimitive	
		- output all information about the given primitive 

**************************************************************************/
static void DumpPrimitive(handle object)
{
    double d1;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    handle terminal;
    char *type;
    handle parent;

    push();

    indent();
    io_printf("- defname %s\n", acc_fetch_defname(object));

    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    indent();
    io_printf("=> acc_next_terminal()\n");
    terminal = null;
    while (true) {
	terminal = acc_next_terminal(object, terminal);
	if (terminal == null) {
	    break;
	}
	DumpObject(terminal);
    }

    acc_fetch_delays(object, &d1, &d2, &d3, &d4, &d5, &d6);
    indent();
    io_printf("- rising = %g\n", d1);
    indent();
    io_printf("- falling = %g\n", d2);
    indent();
    io_printf("- z = %g\n", d3);
    pop();
}

/*************************************************************************

	lineTrace
		trace execution

**************************************************************************/

void lineTrace(char *file, unsigned int line, char *udata)
{
    io_printf("Executing: %s[%d]\n", file, line);
}

/*************************************************************************

	TimescaleToString	
		- convert time unit to a string 

**************************************************************************/

static char *TimescaleToString(int t)
{
    switch (t) {
    case 2:
	return "100 s";
    case 1:
	return "10 s";
    case 0:
	return "1 s";
    case -1:
	return "100 ms";
    case -2:
	return "10 ms";
    case -3:
	return "1 ms";
    case -4:
	return "100 us";
    case -5:
	return "10 us";
    case -6:
	return "1 us";
    case -7:
	return "100 ns";
    case -8:
	return "10 ns";
    case -9:
	return "1 ns";
    case -10:
	return "100 ps";
    case -11:
	return "10 ps";
    case -12:
	return "1 ps";
    case -13:
	return "100 fs";
    case -14:
	return "10 fs";
    case -15:
	return "1 fs";
    default:
	return "unknown";
    }
}

/*************************************************************************

	DumpTerminal	
		- output all information about the given primitive terminal	

**************************************************************************/

static void DumpTerminal(handle object)
{
    handle parent;
    handle newObject;
    int index;

    push();
    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    index = acc_fetch_index(object);
    indent();
    io_printf("- index is %d\n", index);
    if (acc_handle_terminal(parent, index) != object) {
	tf_error("acc_handle_terminal() test failure\n");
    }

    indent();
    io_printf("- direction is ");
    switch (acc_fetch_direction(object)) {
    case accInput:
	io_printf("accInput");
	break;
    case accOutput:
	io_printf("accOutput");
	break;
    case accInout:
	io_printf("accInout");
	break;
    case accMixedIo:
	io_printf("accMixedIo");
	break;
    default:
	io_printf("unknown");
    }
    io_printf("\n");


    indent();
    io_printf("=> acc_handle_conn()\n");
    newObject = acc_handle_conn(object);
    DumpObject(newObject);

    pop();

}

/*************************************************************************

	DumpTchkTerminal	
		- output all information about the given timing check terminal	

**************************************************************************/

static void DumpTchkTerminal(handle object)
{
    handle parent;
    handle newObject;
    int index;
    int edge;

    push();
    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    indent();
    io_printf("- edge is ");
    edge = acc_fetch_edge(object);
    switch (edge) {
    case accNoedge:
	io_printf("accNoedge");
	break;
    case accPosedge:
	io_printf("accPosedge");
	break;
    case accNegedge:
	io_printf("accNegedge");
	break;
    default:
	io_printf("[");
	if (edge & accEdge01) {
	    io_printf("accEdge01,");
	}
	if (edge & accEdge10) {
	    io_printf("accEdge10,");
	}
	if (edge & accEdge0x) {
	    io_printf("accEdge0x,");
	}
	if (edge & accEdgex1) {
	    io_printf("accEdgex0,");
	}
	if (edge & accEdgex1) {
	    io_printf("accEdgex1,");
	}
	if (edge & accEdge1x) {
	    io_printf("accEdge1x,");
	}
	io_printf("]");
    }
    io_printf("\n");


    if (acc_handle_condition(object)) {
	indent();
	io_printf("- has condition\n");
    }

    indent();
    io_printf("=> acc_handle_conn()\n");
    newObject = acc_handle_conn(object);
    DumpObject(newObject);

    pop();

}

/*************************************************************************

	DumpPathTerminal	
		- output all information about the given path terminal	

**************************************************************************/

static void DumpPathTerminal(handle object)
{
    handle parent;
    handle newObject;
    int index;
    int edge;

    push();
    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    indent();
    io_printf("- edge is ");
    edge = acc_fetch_edge(object);
    switch (edge) {
    case accNoedge:
	io_printf("accNoedge");
	break;
    case accPosedge:
	io_printf("accPosedge");
	break;
    case accNegedge:
	io_printf("accNegedge");
	break;
    default:
	io_printf("[");
	if (edge & accEdge01) {
	    io_printf("accEdge01,");
	}
	if (edge & accEdge10) {
	    io_printf("accEdge10,");
	}
	if (edge & accEdge0x) {
	    io_printf("accEdge0x,");
	}
	if (edge & accEdgex1) {
	    io_printf("accEdgex0,");
	}
	if (edge & accEdgex1) {
	    io_printf("accEdgex1,");
	}
	if (edge & accEdge1x) {
	    io_printf("accEdge1x,");
	}
	io_printf("]");
    }
    io_printf("\n");

    indent();
    io_printf("=> acc_handle_conn()\n");
    newObject = acc_handle_conn(object);
    DumpObject(newObject);

    pop();

}

/*************************************************************************

	DumpPort	
		- output all information about the given port	

**************************************************************************/

static void DumpPort(handle object)
{
    handle parent;
    handle newObject;
    int index;

    push();
    parent = acc_handle_parent(object);
    indent();
    io_printf("- is child of %s\n", acc_fetch_fullname(parent));

    index = acc_fetch_index(object);
    indent();
    io_printf("- index is %d\n", index);
    if (acc_handle_port(parent, index) != object) {
	tf_error("acc_handle_port() test failure\n");
    }

    indent();
    io_printf("- direction is ");
    switch (acc_fetch_direction(object)) {
    case accInput:
	io_printf("accInput");
	break;
    case accOutput:
	io_printf("accOutput");
	break;
    case accInout:
	io_printf("accInout");
	break;
    case accMixedIo:
	io_printf("accMixedIo");
	break;
    default:
	io_printf("unknown");
    }
    io_printf("\n");

    indent();
    io_printf("=> acc_handle_loconn()\n");
    newObject = acc_handle_loconn(object);
    DumpObject(newObject);

/*
	indent();
	io_printf( "=> acc_handle_hiconn()\n" );
	newObject = acc_handle_hiconn( object );
	DumpObject( newObject );
*/
    pop();

}

/*************************************************************************

	DumpFunction	
		- output all information about the given function	

**************************************************************************/

static void DumpFunction(handle object)
{
    handle child;
    handle path;
    handle gate;
    handle port;
    char *type;
    handle driver;
    handle check;
    handle net;
    handle firstNet;
    s_location loc;
    handle newObject;
    int delayMode;
    s_timescale_info timescale_info;
    handle parent;


    push();

    acc_mod_lcb_add(object, lineTrace, NULL);
    indent();
    io_printf("- defname is %s\n", acc_fetch_defname(object));

    indent();
    acc_fetch_location(&loc, object);
    io_printf("- location is %s[%d]\n", loc.filename, loc.line_no);

    indent();
    io_printf("- is of type %s[%s]\n",
	      acc_fetch_type_str(acc_fetch_type(object)),
	      acc_fetch_type_str(acc_fetch_fulltype(object)));

    parent = acc_handle_parent(object);
    indent();
    if (parent != null) {
	io_printf("- is child of %s\n", acc_fetch_fullname(parent));
    } else {
	io_printf("- is child of %s\n", "<null>");
    }





    // test acc_next_child
    indent();
    io_printf("=> acc_next_child()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_child(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_cell
    indent();
    io_printf("=> acc_next_cell()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_cell(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }



    // test acc_next_net
    indent();
    io_printf("=> acc_next_net()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_net(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_parameter
    indent();
    io_printf("=> acc_next_parameter()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_parameter(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_port
    indent();
    io_printf("=> acc_next_port()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_port(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_portout
    indent();
    io_printf("=> acc_next_portout()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_portout(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_primitive
    indent();
    io_printf("=> acc_next_primitive()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_primitive(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_scope
    indent();
    io_printf("=> acc_next_scope()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_scope(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }



    // test acc_next
    indent();
    io_printf("=> acc_next()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next(declList, object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }
    pop();
}

/*************************************************************************

	DumpTask	
		- output all information about the given task	

**************************************************************************/

static void DumpTask(handle object)
{
    handle child;
    handle path;
    handle gate;
    handle port;
    char *type;
    handle driver;
    handle check;
    handle net;
    handle firstNet;
    s_location loc;
    handle newObject;
    int delayMode;
    s_timescale_info timescale_info;
    handle parent;


    push();

    acc_mod_lcb_add(object, lineTrace, NULL);
    indent();
    io_printf("- defname is %s\n", acc_fetch_defname(object));

    indent();
    acc_fetch_location(&loc, object);
    io_printf("- location is %s[%d]\n", loc.filename, loc.line_no);

    indent();
    io_printf("- is of type %s[%s]\n",
	      acc_fetch_type_str(acc_fetch_type(object)),
	      acc_fetch_type_str(acc_fetch_fulltype(object)));

    parent = acc_handle_parent(object);
    indent();
    if (parent != null) {
	io_printf("- is child of %s\n", acc_fetch_fullname(parent));
    } else {
	io_printf("- is child of %s\n", "<null>");
    }



    // test acc_next_child
    indent();
    io_printf("=> acc_next_child()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_child(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_cell
    indent();
    io_printf("=> acc_next_cell()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_cell(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_net
    indent();
    io_printf("=> acc_next_net()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_net(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_parameter
    indent();
    io_printf("=> acc_next_parameter()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_parameter(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_port
    indent();
    io_printf("=> acc_next_port()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_port(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_portout
    indent();
    io_printf("=> acc_next_portout()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_portout(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_primitive
    indent();
    io_printf("=> acc_next_primitive()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_primitive(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_scope
    indent();
    io_printf("=> acc_next_scope()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_scope(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next
    indent();
    io_printf("=> acc_next()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next(declList, object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }
    pop();
}

/*************************************************************************

	DumpModule	
		- output all information about the given module	

**************************************************************************/

static void DumpModule(handle object)
{
    handle child;
    handle path;
    handle gate;
    handle port;
    char *type;
    handle driver;
    handle check;
    handle net;
    handle firstNet;
    s_location loc;
    handle newObject;
    int delayMode;
    s_timescale_info timescale_info;
    handle parent;


    push();

    acc_mod_lcb_add(object, lineTrace, NULL);
    indent();
    io_printf("- defname is %s\n", acc_fetch_defname(object));

    indent();
    acc_fetch_location(&loc, object);
    io_printf("- location is %s[%d]\n", loc.filename, loc.line_no);

    indent();
    io_printf("- is of type %s[%s]\n",
	      acc_fetch_type_str(acc_fetch_type(object)),
	      acc_fetch_type_str(acc_fetch_fulltype(object)));

    parent = acc_handle_parent(object);
    indent();
    if (parent != null) {
	io_printf("- is child of %s\n", acc_fetch_fullname(parent));
    } else {
	io_printf("- is child of %s\n", "<null>");
    }

    indent();
    io_printf("- delay mode is ");
    delayMode = acc_fetch_delay_mode(object);
    switch (delayMode) {
    case accDelayModeNone:
	io_printf("accDelayModeNone");
	break;
    case accDelayModeZero:
	io_printf("accDelayModeZero");
	break;
    case accDelayModeUnit:
	io_printf("accDelayModeUnit");
	break;
    case accDelayModeDistrib:
	io_printf("accDelayModeDistrib");
	break;
    case accDelayModePath:
	io_printf("accDelayModePath");
	break;
    case accDelayModeMTM:
	io_printf("accDelayModeMTM");
	break;
    default:
	io_printf("unknown");

    }
    io_printf("\n");

    acc_fetch_timescale_info(object, &timescale_info);
    indent();
    io_printf("- timescale is %s/%s\n",
	      TimescaleToString(timescale_info.unit),
	      TimescaleToString(timescale_info.precision));




    // test acc_next_child
    indent();
    io_printf("=> acc_next_child()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_child(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_cell
    indent();
    io_printf("=> acc_next_cell()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_cell(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_modpath
    indent();
    io_printf("=> acc_next_modpath()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_modpath(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_net
    indent();
    io_printf("=> acc_next_net()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_net(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_parameter
    indent();
    io_printf("=> acc_next_parameter()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_parameter(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_port
    indent();
    io_printf("=> acc_next_port()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_port(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_portout
    indent();
    io_printf("=> acc_next_portout()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_portout(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_primitive
    indent();
    io_printf("=> acc_next_primitive()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_primitive(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_scope
    indent();
    io_printf("=> acc_next_scope()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_scope(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next_specparam
    indent();
    io_printf("=> acc_next_specparam()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_specparam(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }


    // test acc_next_tchk
    indent();
    io_printf("=> acc_next_tchk()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next_tchk(object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }

    // test acc_next
    indent();
    io_printf("=> acc_next()\n");
    newObject = null;
    while (TRUE) {
	newObject = acc_next(declList, object, newObject);
	if (newObject == null) {
	    break;
	}
	DumpObject(newObject);
    }
    pop();
}

/*************************************************************************

	DumpObject	
		- log object and dispatch if not yet visited 

**************************************************************************/

static void DumpObject(handle object)
{
    int index;
    int type;
    LogEntry_t *logEntry;

    index = HASH(object);

    for (logEntry = hashTable[index];
	 logEntry && logEntry->object != object;
	 logEntry = logEntry->next);

    indent();
    io_printf("%s[%s] is %s<%s>\n", acc_fetch_name(object),
	      acc_fetch_fullname(object),
	      acc_fetch_type_str(acc_fetch_type(object)),
	      acc_fetch_type_str(acc_fetch_fulltype(object)));

    if (logEntry && logEntry->object == object) {
	return;
    }

    logEntry = (LogEntry_t *) xmalloc(sizeof(LogEntry_t));

    logEntry->object = object;
    logEntry->next = hashTable[index];
    hashTable[index] = logEntry;

    type = acc_fetch_type(object);
    switch (type) {
    case accTchkTerminal:
	DumpTchkTerminal(object);
	break;
    case accTchk:
	DumpTimingCheck(object);
	break;
    case accParameter:
    case accSpecparam:
	DumpParam(object);
	break;
    case accIntegerVar:
    case accRealVar:
    case accTimeVar:
    case accNamedEvent:
	DumpDecl(object);
	break;
    case accReg:
	DumpReg(object);
	break;
    case accNet:
	DumpNet(object);
	break;
    case accTerminal:
	DumpTerminal(object);
	break;
    case accPathTerminal:
	DumpPathTerminal(object);
	break;
    case accPath:
	DumpPath(object);
	break;
    case accPort:
	DumpPort(object);
	break;
    case accFunction:
	DumpFunction(object);
	break;
    case accTask:
	DumpTask(object);
	break;
    case accModule:
	DumpModule(object);
	break;
    case accPrimitive:
	DumpPrimitive(object);
	break;
    default:
	indent();
	io_printf("Object of type %s, not yet supported\n",
		  acc_fetch_type_str(type));
    }
}


/*************************************************************************

	$$dumpstructure
		-	this routine will dump out all information for the entire model 

**************************************************************************/

int dumpstructure_call(int data, int reason)
{
    handle scope;
    int i;

    acc_initialize();

    switch (reason) {
    case reason_calltf:
	io_printf("reason type %s\n", "calltf");
	break;
    case reason_checktf:
	enableReason = TRUE;
	io_printf("reason type %s\n", "checktf");
	return 0;
    case reason_sizetf:
	io_printf("reason type %s\n", "sizetf");
	return 0;
    case reason_disable:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "disable");
	return 0;
    case reason_paramvc:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "paramvc");
	return 0;
    case reason_synch:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "synch");
	return 0;
    case reason_finish:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "finish");
	return 0;
    case reason_reactivate:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "reactivate");
	return 0;
    case reason_rosynch:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "rosynch");
	return 0;
    case reason_paramdrc:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "paramdrc");
	return 0;
    case reason_interactive:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "interactive");
	return 0;
    case reason_endofcompile:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "endofcompile");
	for (i = 0; i < HASH_SIZE; i++) {
	    hashTable[i] = NULL;
	}
	return 0;
    case reason_force:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "force");
	return 0;
    case reason_release:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "release");
	return 0;
    case reason_scope:
	if (!enableReason) {
	    return 0;
	}
	io_printf("reason type %s\n", "scope");
	return 0;
    default:
	if (!enableReason) {
	    return 0;
	}
	io_printf("unknown reason type %d\n", reason);
	return 0;
    }

    depth = 0;

    tf_asynchon();
    scope = null;
    while (true) {
	scope = acc_next_topmod(scope);
	if (scope == null) {
	    break;
	}
	DumpObject(scope);
    }


    acc_close();
    return (0);
}

/*************************************************************************

	$$setvalue
		- this will allow set value to be tested
		$$setvalue( mode, decl, value, radix,delay )
		mode is string eg "accForce:
		decl is net or reg
		value is string or parameter value
		radix is string eg "accStringVal"
		delay is a time variable

**************************************************************************/

int setvalue_call(int data, int reason)
{
    handle decl;
    s_acc_vecval vecval;
    s_acc_value value;
    s_setval_delay delay;
    char *ptr;

    if (reason != reason_calltf) {
	return 0;
    }

    acc_initialize();

    if (tf_nump() != 5) {
	tf_error
	    (" Error: $$setvalue( \"mode\", decl, \"value\", \"format\", delay );");
	return 0;
    }


    ptr = acc_fetch_tfarg_str(1);
    if (!strcmp(ptr, "accNoDelay")) {
	delay.model = accNoDelay;
    } else if (!strcmp(ptr, "accInertialDelay")) {
	delay.model = accInertialDelay;
    } else if (!strcmp(ptr, "accTransportDelay")) {
	delay.model = accTransportDelay;
    } else if (!strcmp(ptr, "accPureTransportDelay")) {
	delay.model = accPureTransportDelay;
    } else if (!strcmp(ptr, "accAssignFlag")) {
	delay.model = accAssignFlag;
    } else if (!strcmp(ptr, "accDeassignFlag")) {
	delay.model = accDeassignFlag;
    } else if (!strcmp(ptr, "accForceFlag")) {
	delay.model = accForceFlag;
    } else if (!strcmp(ptr, "accReleaseFlag")) {
	delay.model = accReleaseFlag;
    }

    decl = acc_handle_tfarg(2);

    ptr = acc_fetch_tfarg_str(4);
    if (!strcmp(ptr, "accScalarVal")) {
	ptr = acc_fetch_tfarg_str(3);
	value.format = accScalarVal;
	if (!strcmp(ptr, "acc0")) {
	    value.value.scalar = acc0;
	} else if (!strcmp(ptr, "acc1")) {
	    value.value.scalar = acc1;
	} else if (!strcmp(ptr, "accX")) {
	    value.value.scalar = accX;
	} else {
	    value.value.scalar = accZ;
	}
    } else if (!strcmp(ptr, "accVectorVal")) {
	s_tfexprinfo info;
	tf_exprinfo(3, &info);
	value.format = accVectorVal;
	value.value.vector = (p_acc_vecval) info.expr_value_p;
    } else if (!strcmp(ptr, "accIntVal")) {
	value.value.integer = acc_fetch_tfarg_int(3);
	value.format = accIntVal;
    } else if (!strcmp(ptr, "accRealVal")) {
	value.value.real = acc_fetch_tfarg(3);
	value.format = accRealVal;
    } else if (!strcmp(ptr, "accStringVal")) {
	value.value.str = acc_fetch_tfarg_str(3);
	value.format = accStringVal;
    } else if (!strcmp(ptr, "accBinStrVal")) {
	value.value.str = acc_fetch_tfarg_str(3);
	value.format = accBinStrVal;
    } else if (!strcmp(ptr, "accOctStrVal")) {
	value.value.str = acc_fetch_tfarg_str(3);
	value.format = accOctStrVal;
    } else if (!strcmp(ptr, "accDecStrVal")) {
	value.value.str = acc_fetch_tfarg_str(3);
	value.format = accDecStrVal;
    } else if (!strcmp(ptr, "accHexStrVal")) {
	value.value.str = acc_fetch_tfarg_str(3);
	value.format = accHexStrVal;
    }

    delay.time.low = tf_getlongp(&delay.time.high, 5);
    delay.time.type = accSimTime;

    acc_set_value(decl, &value, &delay);

    acc_close();

    return 0;
}
