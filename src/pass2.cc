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

/* PASS2.C - Generate hierarchy, allocate variables, propigate parameters */

#define PASS2_C

#include "glue.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "obstack.h"
#include "runtime.h"
#include "decl.h"
#include "scope.h"
#include "macro.h"
#include "io.h"
#include "store.h"
#include "udp.h"
#include "pass3.h"
#include "eval.h"
#include "copy.h"
#include "check.h"
#include "pass2.h"
#include "veriwell.h"
#include "flags.h"
#include "lex.h"

static void parse_at_top_scope(tree scope);


#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free


/* This obstack contains pointers to expression/ref/decl nodes in the
   order that they should be evaluated (postfix). */
extern struct obstack inst_obstack;
extern struct obstack alt_inst_obstack;

/* This is some global information that needs to be kept around for
   when markers need to be generated for references inside event
   expressions.  Perhaps this is a grungy way of doing it.  */

extern struct Marker_info marker_info;
extern LibPath_t *ypathList;

/* TREE_LIST of continuous assignments that are port connections */
tree port_connections = NULL_TREE;


/* Memory allocation routines for pass3_decl.  Allocate the memory and
   initialize to all Xs. */

Group *malloc_X(ngroups_t ngroups)
{
    ngroups_t i;
    Group *tmp, *tmp1;

    tmp1 = tmp = (Group *) xmalloc(ngroups * sizeof(Group));
    if (tmp)
	for (i = 0; i < ngroups; i++, tmp++)
	    BVAL(tmp) = AVAL(tmp) = (Bit) - 1;

    return tmp1;
}

Group *realloc_X(Group * g, ngroups_t ngroups)
{
    ngroups_t i;
    Group *tmp, *tmp1;

    tmp1 = tmp = (Group *) xrealloc((char*)g, ngroups * sizeof(Group));
    if (tmp)
	for (i = 0; i < ngroups; i++, tmp++)
	    BVAL(tmp) = AVAL(tmp) = (Bit) - 1;

    return tmp1;
}

/* Memory allocation routines for pass3_decl.  Allocate the memory and
   initialize to all Zs for Nets. */

Group *malloc_Z(ngroups_t ngroups)
{
    ngroups_t i;
    Group *tmp, *tmp1;

    tmp1 = tmp = (Group *) xmalloc(ngroups * sizeof(Group));
    if (tmp)
	for (i = 0; i < ngroups; i++, tmp++) {
	    BVAL(tmp) = (Bit) - 1;
	    AVAL(tmp) = 0;
	}

    return tmp1;
}

Group *realloc_Z(Group * g, ngroups_t ngroups)
{
    ngroups_t i;
    Group *tmp, *tmp1;

    tmp1 = tmp = (Group *) xrealloc((char*)g, ngroups * sizeof(Group));
    if (tmp)
	for (i = 0; i < ngroups; i++) {
	    BVAL(tmp) = (Bit) - 1;
	    AVAL(tmp) = 0;
	}

    return tmp1;
}

static ngroups_t do_vector_stuff(tree decl)
{
    nbits_t nbits;
    ngroups_t ngroups;

    MSB(decl) =
	get_range(DECL_MSB(decl), IDENTIFIER_POINTER(DECL_NAME(decl)));
    LSB(decl) =
	get_range(DECL_LSB(decl), IDENTIFIER_POINTER(DECL_NAME(decl)));
    nbits = ABS(MSB(decl) - LSB(decl)) + 1;
    VECTOR_DIRECTION_ATTR(decl) = (MSB(decl) >= LSB(decl));
    ngroups = bits_to_groups(nbits);
    if (ngroups > MAX_GROUPS)
	sorry1("Maximum vector size is %d", (char *) MAX_BITS);
    TREE_NBITS(decl) = nbits;
    return ngroups;
}

/* Allocate storage for declaration block after evaluating and checking
   LSB and MSBs. */

void pass3_decl(tree decl)
{
    enum tree_type code = (enum tree_type) TREE_CODE(decl);
    nbits_t nbits;
    ngroups_t ngroups;
    ngroups_t array_size;

    lineno = DECL_SOURCE_LINE(decl);
    input_filename = DECL_SOURCE_FILE(decl);
    INITIALIZED_ATTR(decl) = 1;
    SET_DECL_STATE(decl, X);	/* By default; nest will change this to Z */
    switch (code) {

    case NET_SCALAR_DECL:
	SET_DECL_STATE(decl, Z);
	pass3_delay(NET_DELAY(decl));

	if (STMT_SURROGATE_ATTR(decl)) {
	    /* copy stuff from original; TREE_CHAIN points to original */
	    TREE_NBITS(decl) = 1;
	    DECL_STORAGE(decl) = (Group *) malloc_Z((ngroups_t) 1);
	    marker_info.first = NULL;
	    marker_info.last = NULL;
	    marker_info.flags = (enum marker_flags) (M_NET + M_FIXED);
//  if (/*PORT_INPUT_ATTR (decl) ||*/ PORT_OUTPUT_ATTR (decl))
	    if (PORT_IMMEDIATE_ATTR(decl))	// || PORT_COLLAPSED_ATTR (decl))
		marker_info.flags =
		    (enum marker_flags) (M_PORT + marker_info.flags);
	    marker_info.delay = NET_DELAY(decl);
	    if (!NET_ASSIGN_ATTR(decl)
		|| PORT_COLLAPSED_ATTR(TREE_CHAIN(decl))
		|| NET_DELAY(TREE_CHAIN(decl)))
		BuildMarker(decl, &marker_info);
	    break;
	}			/* if not surrogate, do the rest the same as for regs */
	DECL_STORAGE(decl) = (Group *) malloc_Z((ngroups_t) 1);
	TREE_NBITS(decl) = 1;
	break;

    case REG_SCALAR_DECL:
	DECL_STORAGE(decl) = (Group *) malloc_X((ngroups_t) 1);
	TREE_NBITS(decl) = 1;
	break;

    case INTEGER_DECL:
	MSB(decl) = 31;
	LSB(decl) = 0;
	VECTOR_DIRECTION_ATTR(decl) = 1;
	DECL_STORAGE(decl) = (Group *) malloc_X((ngroups_t) 1);
	TREE_NBITS(decl) = 32;
	break;

    case REAL_DECL:
	MSB(decl) = 31;
	LSB(decl) = 0;
	VECTOR_DIRECTION_ATTR(decl) = 1;
	DECL_STORAGE(decl) = (Group *) malloc_X((ngroups_t) 1);
	REAL_(DECL_STORAGE(decl)) = 0;
	TREE_NBITS(decl) = 32;
	break;

    case ARRAY_DECL:
	ARRAY_HI(decl) = get_range(ARRAY_HI_EXPR(decl),
				   IDENT(DECL_NAME(decl)));
	ARRAY_LO(decl) = get_range(ARRAY_LO_EXPR(decl),
				   IDENT(DECL_NAME(decl)));

	switch (ARRAY_CODE(decl)) {
	case REG_VECTOR_DECL:
	    ngroups = do_vector_stuff(decl);
	    break;

	case INTEGER_DECL:
	    TREE_NBITS(decl) = 32;
	    ngroups = 0;
	    break;

	case TIME_DECL:
	    TREE_NBITS(decl) = 64;
	    ngroups = 1;
	    break;

	case REG_SCALAR_DECL:
	    TREE_NBITS(decl) = 1;
	    ngroups = 0;
	    break;

	default:
	    fatal("Internal error: unknown array type", NULL);
	}
	{
	    nbits_t tmp_array_size;
	    tmp_array_size = ABS(ARRAY_HI(decl) - ARRAY_LO(decl)) + 1;
	    if (tmp_array_size > MAX_ARRAY
		|| tmp_array_size * (ngroups + 1) > MAX_ARRAY) {
		error("Maximum array size exceeded in array '%s'",
		      IDENT(DECL_NAME(decl)), NULL_CHAR);
		array_size = 1;
	    } else
		array_size = tmp_array_size;
	}			/* Allocate pointers to array elements */
	DECL_STORAGE(decl) =
	    (Group *) malloc_X(array_size * (ngroups + 1));

	VECTOR_DIRECTION_ATTR(decl) = (ARRAY_HI(decl) >= ARRAY_LO(decl));
	break;

    case NET_VECTOR_DECL:
	SET_DECL_STATE(decl, Z);
	pass3_delay(NET_DELAY(decl));

	if (STMT_SURROGATE_ATTR(decl)) {
	    if (TREE_CODE(TREE_CHAIN(decl)) == NET_SCALAR_DECL)
		fatal("Scalar converted to vector", NULL);
	    /* copy stuff from original; TREE_CHAIN points to original */
	    MSB(decl) = MSB(TREE_CHAIN(decl));
	    LSB(decl) = LSB(TREE_CHAIN(decl));
	    VECTOR_DIRECTION_ATTR(decl) =
		VECTOR_DIRECTION_ATTR(TREE_CHAIN(decl));
	    TREE_NBITS(decl) = TREE_NBITS(TREE_CHAIN(decl));
	    ngroups = bits_to_groups(TREE_NBITS(decl));
	    DECL_STORAGE(decl) = (Group *) malloc_Z(ngroups + 1);
	    marker_info.first = NULL;
	    marker_info.last = NULL;
	    marker_info.flags = (enum marker_flags) (M_NET + M_FIXED);
//  if (/*PORT_INPUT_ATTR (decl) ||*/ PORT_OUTPUT_ATTR (decl))
	    if (PORT_IMMEDIATE_ATTR(decl))	// || NET_ASSIGN_ATTR (decl))
		marker_info.flags =
		    (enum marker_flags) (M_PORT + marker_info.flags);
	    marker_info.delay = NET_DELAY(decl);
	    if (!NET_ASSIGN_ATTR(decl)
		|| PORT_COLLAPSED_ATTR(TREE_CHAIN(decl)))
		BuildMarker(decl, &marker_info);
	    break;
	}
	/* if not surrogate, do the rest the same as for regs */
	ngroups = do_vector_stuff(decl);
	DECL_STORAGE(decl) = (Group *) malloc_Z(ngroups + 1);
	break;

    case TIME_DECL:
	MSB(decl) = 0;
	LSB(decl) = 0;
	TREE_NBITS(decl) = 64;
	DECL_STORAGE(decl) = (Group *) malloc_X((ngroups_t) 2);
	break;

    case REG_VECTOR_DECL:
	ngroups = do_vector_stuff(decl);
	DECL_STORAGE(decl) = (Group *) malloc_X(ngroups + 1);
	break;

    case TMP_DECL:
	TREE_NBITS(decl) = TREE_NBITS(TREE_CHAIN(decl));	/* inherit bits */
	ngroups = bits_to_groups(TREE_NBITS(decl));
	DECL_STORAGE(decl) = (Group *) malloc_Z(ngroups + 1);
	break;

    case EVENT_DECL:
    case BLOCK_DECL:
	TREE_NBITS(decl) = 1;
	break;

    case SPECPARAM_DECL:
    case PARAM_DECL:
	if (!DECL_PARAM_REDIRECT(decl)) {
	    DECL_PARAM_RVAL_CODE(decl) = pass3_expr(DECL_PARAM_RVAL(decl));
	    nbits = TREE_NBITS(DECL_PARAM_RVAL(decl));
	    TREE_REAL_ATTR(decl) = TREE_REAL_ATTR(DECL_PARAM_RVAL(decl));
	    TREE_INTEGER_ATTR(decl) =
		TREE_INTEGER_ATTR(DECL_PARAM_RVAL(decl));
	} else {
	    DECL_PARAM_RVAL_CODE(decl) =
		pass3_expr(DECL_PARAM_REDIRECT(decl));
	    nbits = TREE_NBITS(DECL_PARAM_REDIRECT(decl));
	    TREE_REAL_ATTR(decl) =
		TREE_REAL_ATTR(DECL_PARAM_REDIRECT(decl));
	    TREE_INTEGER_ATTR(decl) =
		TREE_INTEGER_ATTR(DECL_PARAM_REDIRECT(decl));
	}
	ngroups = bits_to_groups(nbits);
	if (ngroups > MAX_GROUPS)
	    sorry1("Maximum vector size is %d", (char *) MAX_BITS);
	if (DECL_MSB(decl)) {
	    do_vector_stuff(decl);
//        MSB (decl) = get_range (DECL_MSB (decl), IDENTIFIER_POINTER (DECL_NAME (decl)));
//        LSB (decl) = get_range (DECL_LSB (decl), IDENTIFIER_POINTER (DECL_NAME (decl)));
//        VECTOR_DIRECTION_ATTR (decl) = (MSB (decl) >= LSB (decl));
	} else {
	    VECTOR_DIRECTION_ATTR(decl) = 1;	/* default to msb>lsb */
	    LSB(decl) = 0;
	    MSB(decl) = nbits - 1;
	}

	TREE_NBITS(decl) = nbits;
	if (bits_to_groups(nbits) > MAX_GROUPS)
	    sorry1("Maximum vector size is %d", (char *) MAX_BITS);
	DECL_STORAGE(decl) = (Group *) malloc_X(ngroups + 1);
	INITIALIZED_ATTR(decl) = 1;
	{
	    /* need to allocate space for expressions; these are defined in
	       pass3.c */
	    extern ngroups_t stack_size;
	    extern int max_label;

	    if (!R_alloc(max_label, stack_size))
		fatal
		    ("Not enough memory to evaluate parameter expression of '%s'",
		     IDENT(DECL_NAME(decl)));
	}
	eval(DECL_PARAM_RVAL_CODE(decl));
	store(decl, decl);
	break;

    default:
	error("Don't know how to allocate for %s (%s)",
	      tree_code_name[code], IDENT(DECL_NAME(decl)));
    }

    if ((enum tree_code)code != EVENT_DECL && (enum tree_code)code != BLOCK_DECL)
	if (DECL_STORAGE(decl) == 0)
//    if ((code != ARRAY_DECL && DECL_STORAGE (decl) == 0)
//  || (code == ARRAY_DECL && DECL_STORAGE (decl) == 0))
	    fatal("Not enough memory to allocate storage for '%s'",
		  IDENTIFIER_POINTER(DECL_NAME(decl)));

}


/* A module wasn't found in the hierarchy; look for a library using the
   -y option and the +libext options if they are supplied */

tree check_library(char *name)
{
    char *ytmp;
    char *p;
    char *pathend;
    char path[256];
    LibPath_t *ypath;
    File *f;
    tree tmp_module_list, tmp_module;
    tree saved_scope;



    for (ypath = ypathList; ypath != NULL; ypath = ypath->next) {
	strcpy(path, ypath->path);	/* copy the library path */
	strcat(path, PATHSEP);	/* Add trailing "/" (or "\" or ":") */
	strcat(path, name);	/* Add name of module */
	pathend = &path[strlen(path)];

	if (ylibext) {		/* +libext option supplied */
	    ytmp = (char *) xmalloc(strlen(ylibext) + 1);
	    strcpy(ytmp, ylibext);

	    /* loop through all the extensions given in the option */
	    for (p = strtok(ytmp, "+"); p; p = strtok(NULL, "+")) {
		strcpy(pathend, p);	/* append various extensions */
//              printf ("Attempting to open %s\n", path);
		f = File::fopen(path, "rt");	/* try it */
		if (f) {
		    goto foundit;
		}
	    }
	}
    }

    return NULL_TREE;

  foundit:

    push_stream(fin, 2);
    fin = f;
    input_filename = xmalloc(strlen(path) + 1);
    lineno = 1;

    strcpy(input_filename, path);
    tmp_module_list = module_list;	/* Start new module list for... */
    module_list = NULL_TREE;	/* ...library file */

/* We have to be at the top module level for an instance to happen.  Pop
   this scope level so that all idents have no current value.  Now, the
   parse can happen in a clean environment.  Else, the parse will pick up
   idents used in the parent module.  */

#if 0
    saved_scope = current_scope;
    pop_scope();		/* free up idents */
    current_scope = NULL_TREE;	/* incase timescale is used */
    prog_parse();		/* Parse the library file */
    module_list = nreverse(module_list);	/* put first mod first */
    tmp_module = TREE_PURPOSE(module_list);	/* get first one */
    module_list = chainon(tmp_module_list, module_list);	/* reassemble */

    fin = pop_stream();
    set_scope(saved_scope);	/* restore parent scope */
#endif

    parse_at_top_scope(current_scope);

    module_list = nreverse(module_list);	/* put first mod first */
    tmp_module = TREE_PURPOSE(module_list);	/* get first one */
    module_list = chainon(tmp_module_list, module_list);	/* reassemble */

    fin = pop_stream();
    LIB_MODULE_ATTR(tmp_module) = 1;
    return tmp_module;
}

/* unwind the stack and then parse the library at the top level scope;
   restore the state of the scope stack after */

void parse_at_top_scope(tree scope)
{
    tree new_scope;

    new_scope = pop_scope();
    if (new_scope)
	parse_at_top_scope(new_scope);	/* recursively unwind */

    else {			/* at top, now parse */

	current_scope = NULL_TREE;	/* incase timescale is used */
	prog_parse();		/* Parse the library file */
    }
    set_scope(scope);		/* unwind recursion */
}



static tree copy_module(tree node)
{
    return copy_block(node);
}

/* Lookup the module being instantiated.  If the module found is a library
   module, compare number of ports to make sure that another library module
   isn't needed.
   Later, this could be converted to use hashing, esp after gates are
   supported */

static tree module_lookup(tree instance)
{
    tree t;
    char *name = IDENT(INSTANCE_MODULE_NAME(instance));

    for (t = module_list; t; t = TREE_CHAIN(t)) {
	if (!strcmp(name, (MODULE_NAME(TREE_PURPOSE(t))))) {
	    if (UDP_ATTR(TREE_PURPOSE(t))) {	/* this is a udp instantiation */
		return (TREE_PURPOSE(t));
	    }


	    if (LIB_MODULE_ATTR(TREE_PURPOSE(t))) {
		if (list_length(INSTANCE_PORTS(instance)) !=
		    list_length(BLOCK_PORTS(TREE_PURPOSE(t))))
		    continue;
	    }
	    return (TREE_PURPOSE(t));
	}
    }
    return check_library(name);	
}

/* Actually connect the ports; emulate collapsing if necessary.  Pass in
   the module port, the instance port argument, and the instance node for
   line number info */

static void do_connect_ports(tree terminal, tree arg_tree, tree instance)
{
    enum tree_code port_code;
    tree arg = TREE_PURPOSE(arg_tree);
    enum tree_code arg_code = TREE_CODE(arg);
    int collapse;
    tree assign;
    tree port;

    port = TREE_PURPOSE(terminal);
    port_code = TREE_CODE(port);
    //PORT_TERMINAL_HICONN( terminal ) = arg;

    if (PORT_REDEFINED_ATTR(port)) {
	port = DECL_THREAD(port);
	port_code = TREE_CODE(port);
    }

/* Loosely emulate port collapsing by making it so that "collapsed ports"
   propigate immediately.  This will happen is both ports are scalar nets
   or both are vector nets.  (The legality of the ports has already been
   determined and one must already be a net, so it is sufficient to test
   only for type equality.) Note that collapsed port forces it to be
   an INOUT type, but that fact won't be explicitly mentioned in traces. */

/* [Some explaination is necessary: is both arg and port are net vectors
    or both are net scalars, then collapse, regardless of whether the
    port is input, output, or inout.  This is because some models
    (particularly sio85) rely on the fact that collapsed ports propigate
    immediately.  However, don't collapse when the arg is a part-select,
    bit-select, or concat.  Unless the port is an INOUT.  This is because
    we didn't want to slow down models that were already running reasonably
    fast before this change (collapsing part/bit/concats on INOUT) went in.
    If the part-selects, etc were collapsed, some models slowed 100%.]
    */

//  if (arg_code == port_code)
    if ((is_net_code(arg_code) && arg_code == port_code)
	|| PORT_INPUT_ATTR(port) && PORT_OUTPUT_ATTR(port))
//  if (is_all_net (arg) && is_all_net (port))// && PORT_INPUT_ATTR (port) && PORT_OUTPUT_ATTR (port))
	collapse = 1;
    else
	collapse = 0;

/* If port was copied (multiple module instantiation), then this
   would be non-zero which would screw up output ports. So, make sure
   it is NULL.  */

    TREE_3RD(arg_tree) = NULL_TREE;

    if (PORT_INPUT_ATTR(port))	// || collapse)
    {
	assign = build_cont_assign(check_lval_port(port),
				   arg, DECL_SOURCE_LINE(port), NULL_TREE,
				   0);
//    assign = build_cont_assign (check_lval (DECL_NAME (port), 4, port),
//      arg, DECL_SOURCE_LINE (port), NULL_TREE, 0);
	PORT_INPUT_ATTR(STMT_ASSIGN_LVAL(assign)) = PORT_INPUT_ATTR(port);
	PORT_OUTPUT_ATTR(STMT_ASSIGN_LVAL(assign)) =
	    PORT_OUTPUT_ATTR(port);
	PORT_COLLAPSED_ATTR(STMT_ASSIGN_LVAL(assign)) = collapse;
	PORT_IMMEDIATE_ATTR(STMT_ASSIGN_LVAL(assign)) = collapse;
	/* save port connection in 3rd node og arg's tree_list */
	TREE_3RD(arg_tree) = build_tree_list(assign, NULL_TREE);
    }
    if (PORT_OUTPUT_ATTR(port))	// || collapse)
    {
	if (!is_all_net(arg)) {
	    error("Illegal output port specification: '%s'",
		  IDENT(DECL_NAME(arg)), NULL_CHAR);
	    return;
	} else if (port_code == NET_VECTOR_DECL
		   || port_code == NET_SCALAR_DECL) {
	    tree tmp_decl = copy_node(port);
//      tree tmp_decl = make_net_source (port);

	    DECL_THREAD(port) = tmp_decl;
	    DECL_THREAD(tmp_decl) = port;
	    DECL_NAME(tmp_decl) = DECL_NAME(port);

	    assign = build_cont_assign(check_lval_port(arg),
				       tmp_decl, INSTANCE_LINE(instance),
				       NULL_TREE, 0);
	} else {
	    assign = build_cont_assign(check_lval_port(arg),
				       port, INSTANCE_LINE(instance),
				       NULL_TREE, 0);
	}
//    PORT_INPUT_ATTR (STMT_ASSIGN_LVAL (assign)) = PORT_INPUT_ATTR (port);
//    PORT_OUTPUT_ATTR (STMT_ASSIGN_LVAL (assign)) = PORT_OUTPUT_ATTR (port);
//    PORT_COLLAPSED_ATTR (STMT_ASSIGN_LVAL (assign)) = collapse;

//  PORT_IMMEDIATE_ATTR (STMT_ASSIGN_LVAL (assign)) = collapse;// | PORT_OUTPUT_ATTR (port);
	set_immediate_attr(STMT_ASSIGN_LVAL(assign), collapse);
	PORT_COLLAPSED_ATTR(port) = collapse;
	/* If this is bi-directional, then TREE_LIST will already have been
	   created */
	if (TREE_3RD(arg_tree))
	    TREE_VALUE(TREE_3RD(arg_tree)) = assign;
	else
	    TREE_3RD(arg_tree) = build_tree_list(NULL_TREE, assign);
    }
}

static void connect_ports(tree inside, tree outside)
{
    tree port, arg;

    arg = INSTANCE_PORTS(outside);

    /* named ports match in one of two conditions: if the name matches the
       name of the decl of the port (if it is a decl and not a concat,
       part-select, etc), or if the port is also named and the names match.
       Note that matching is determined if the IDENTIFIER_NODEs are the
       same since there is one hash table for an entire model. */

    if (TREE_VALUE(arg)) {
	for (; arg; arg = TREE_CHAIN(arg)) {
	    for (port = MODULE_PORT_LIST(inside); port;
		 port = TREE_CHAIN(port)) {
		if ((*tree_code_type[TREE_CODE(TREE_PURPOSE(port))] == 'd'
		     && TREE_VALUE(arg) == DECL_NAME(TREE_PURPOSE(port)))
		    || TREE_VALUE(arg) == TREE_VALUE(port)) {
		    if (NAMED_PORT_ATTR(port))
			error("More than one connection to a port",
			      NULL_CHAR, NULL_CHAR);
		    NAMED_PORT_ATTR(port) = 1;
		    do_connect_ports(port, arg, outside);
		    break;
		}
	    }
	    if (!port)
		error("No port named '%s'", IDENT(TREE_VALUE(arg)),
		      NULL_CHAR);
	}

	/* reset named port markers */
	for (port = MODULE_PORT_LIST(inside); port;
	     port = TREE_CHAIN(port))
	    NAMED_PORT_ATTR(port) = 0;
    }

/* Ordered list of ports */
    else
	for (port = MODULE_PORT_LIST(inside); port && arg;
	     port = TREE_CHAIN(port), arg = TREE_CHAIN(arg))
	    if (TREE_PURPOSE(arg))
		do_connect_ports(port, arg, outside);

    if (arg && TREE_PURPOSE(arg))
	warning("Too many instance ports", NULL, NULL);
    else if (port)
	warning("Too few instance ports", NULL, NULL);

}

/* If there are any defparams in the module, they are chained in
   MODULE_DEFPARAMS.  Make sure lval exists, then redirect its parameter
   to our lval.  Only call this for modules. */

static void resolve_defparams(tree scope)
{
    tree t, param, lval;
    tree resolve_hierarchical_name(tree);
    tree search_up_decl(char *, tree);

    for (t = MODULE_DEFPARAMS(scope); t; t = TREE_CHAIN(t)) {
	lval = STMT_ASSIGN_LVAL(t);
	if (HIERARCHICAL_ATTR(lval))
	    param = resolve_hierarchical_name(lval);
	else
	    param = search_up_decl(IDENT(lval), scope);
	if (param == error_mark_node)
	    return;
	if (!param) {
	    error("Parameter '%s' not declared", IDENT(lval), NULL_CHAR);
	    return;
	}
	DECL_PARAM_REDIRECT(param) = STMT_ASSIGN_RVAL(t);
    }
}

/* Initialize all decls in the current scope and all others below.  These
   will be task, functions, and named blocksm and, of course, modules. */

static void initialize_decls(tree scope)
{
    tree t, t1;

    current_scope = scope;

    if (TREE_CODE(scope) == MODULE_BLOCK) {
	resolve_defparams(scope);

	for (t = MODULE_SPECDEFS(scope); t; t = TREE_CHAIN(t)) {
	    if (TREE_CODE(t) == SPECPARAM_DECL && !INITIALIZED_ATTR(t)) {
		pass3_decl(t);
	    }
	}
    }

    for (t = BLOCK_DECL(scope); t; t = TREE_CHAIN(t)) {
	pass3_decl(t);
	/* process other sources to a net */
	if (TREE_CODE(t) == NET_VECTOR_DECL ||
	    TREE_CODE(t) == NET_SCALAR_DECL) {
	    marker_info.current_scb = BuildSCB(t, NOLIST);
	    if (PORT_OUTPUT_ATTR(t) || PORT_COLLAPSED_ATTR(t))
		if (DECL_THREAD(t))
		    pass3_decl(DECL_THREAD(t));

	    for (t1 = NET_SOURCE(t); t1; t1 = NET_SOURCE(t1)) {
		if (strcmp(IDENTIFIER_POINTER(DECL_NAME(t1)),
			   IDENTIFIER_POINTER(DECL_NAME(t))))
		    fatal("Line %lu  Driver names to not agree\n",
			  (char *) lineno);
		pass3_decl(t1);
	    }
	}
    }

    for (t = BLOCK_PORTS(scope); t; t = TREE_CHAIN(t)) {
	if (PORT_REDEFINED_ATTR(t))
	    continue;
	pass3_decl(t);
	/* process other sources to a net */
	if (TREE_CODE(t) == NET_VECTOR_DECL ||
	    TREE_CODE(t) == NET_SCALAR_DECL) {
	    marker_info.current_scb = BuildSCB(t, NOLIST);
	    if (PORT_OUTPUT_ATTR(t) || PORT_COLLAPSED_ATTR(t))
		if (DECL_THREAD(t))
		    pass3_decl(DECL_THREAD(t));

	    for (t1 = NET_SOURCE(t); t1; t1 = NET_SOURCE(t1))
		pass3_decl(t1);
	}
    }

    /* Allocate return variable */
    if (TREE_CODE(scope) == FUNCTION_BLOCK) {
	pass3_decl(FUNCT_DECL(scope));
	TREE_NBITS(scope) = TREE_NBITS(FUNCT_DECL(scope));
    }


    /* do this for everyone below us */
    for (t = BLOCK_DOWN(scope); t; t = TREE_CHAIN(t))
	initialize_decls(t);

}

/* recursively link module together hierarchically */

static void do_instantiation(tree node)
{
    tree t, instance;
    int had_to_copy;

/* Allocate space for declared vars */

#if VDEBUG != 0
    if (vdebug)
	printf_V("****** PASS2: Module '%s', instance '%s'\n",
		 MODULE_NAME(node), IDENT(BLOCK_NAME(node)));
#endif

    INITIALIZED_ATTR(node) = 1;

    /* scan all instantiations within this module */
    for (t = MODULE_INSTANCES(node); t; t = TREE_CHAIN(t)) {
	HEAPTEST("In do_instantiation");
	if (TREE_CODE(t) != INSTANCE_NODE)
	    continue;
	/* get line and file info in case of errors */
	lineno = STMT_SOURCE_LINE(t);
	input_filename = STMT_SOURCE_FILE(t);

	/* first, look for module name */
//    instance = module_lookup (IDENTIFIER_POINTER (INSTANCE_MODULE_NAME (t)));
	instance = module_lookup(t);
//printf_V ("   Examining instance '%s', line %lu\n", IDENT (INSTANCE_MODULE_NAME (t)), lineno);
	if (!instance) {
	    error("Module '%s' not defined",
		  IDENT(INSTANCE_MODULE_NAME(t)), NULL_CHAR);
	    continue;
//      break;
	}

	/* if module instance is really a udp, handle here */

	if (UDP_ATTR(instance)) {
	    if (UDP_ATTR(t) == TRUE)	/* already set; must have been copied... */
		continue;	/* ...copy already build the gate instance */
	    UDP_ATTR(t) = TRUE;
	    instantiate_udp(node, t, instance);
	    continue;
	}


	/* Next, if the module is not top-level, the we need to make a copy */
	if (BLOCK_UP(instance)) {
//printf_V ("Starting Copy....");
	    instance = copy_module(instance);
//printf_V ("Ending Copy\n");
	    had_to_copy = 1;
	    if (!instance)
		break;
	} else
	    had_to_copy = 0;

	if (!INSTANCE_NAME(t) && !LIB_MODULE_ATTR(instance)) {
	    error("No module name given in instantiation", NULL_CHAR,
		  NULL_CHAR);
//      return;
	}
/// ehm 3/7/97    else if (!LIB_MODULE_ATTR (instance))
///    {
//      if (!IDENT_CURRENT_DECL (INSTANCE_NAME (t)))
//        fatal ("Instance name isn't in decl list: '%s'", IDENT (INSTANCE_NAME (t)));

//      DECL_THREAD (IDENT_CURRENT_DECL (INSTANCE_NAME (t))) = instance;
//      make_block_decl (INSTANCE_NAME (t), node, instance);
//      if (had_to_copy)
	make_block_decl(INSTANCE_NAME(t), node, instance);
// ehm 3/7/97    }
// "    else
// "    {
// "      INSTANCE_NAME (t) = INSTANCE_MODULE_NAME (t);
//"    }

	/* Thread the module in */
	TREE_CHAIN(instance) = BLOCK_DOWN(node);
	BLOCK_DOWN(node) = instance;
	BLOCK_UP(instance) = node;
	BLOCK_NAME(instance) = INSTANCE_NAME(t);

	INSTANCE_BLOCK(t) = instance;

	/* resolve module parameter overrides #(...) */
	if (INSTANCE_PARAMS(t)) {
	    tree decl, param_list = INSTANCE_PARAMS(t);

	    /* scan decl list for a parameter */
	    for (decl = BLOCK_DECL(instance); decl;
		 decl = TREE_CHAIN(decl)) {
		if (TREE_CODE(decl) != PARAM_DECL)
		    continue;
		DECL_PARAM_REDIRECT(decl) = TREE_PURPOSE(param_list);
		if ((param_list = TREE_CHAIN(param_list)) == 0)
		    break;
	    }
	    if (param_list)
		error("Too many module instance parameter assignments",
		      NULL_CHAR, NULL_CHAR);
	}

	connect_ports(instance, t);

	/* Recursively instantiate modules inside the instance (if not already
	   done). */
	if (!INITIALIZED_ATTR(instance)) {
	    set_scope(instance);
	    do_instantiation(instance);
	    current_scope = pop_scope();
	}
    }
}

/* Main entrypoint */

void build_hierarchy()
{
    tree t, t1;
    extern tree scope0;		/* defined in decl.c */

    obstack_init(&inst_obstack);
    obstack_init(&alt_inst_obstack);

    initialize_scope(NULL_TREE);

    for (t1 = module_list; t1; t1 = TREE_CHAIN(t1)) {
	t = TREE_PURPOSE(t1);
	if (!BLOCK_UP(t) && !UDP_ATTR(t)) {	/* don't do anything if not top_level or udp */
	    set_scope(t);
	    do_instantiation(t);
	    current_scope = pop_scope();
	}
    }
    scope0 = make_node(SUPER_BLOCK);
    BLOCK_DECL(scope0) = NULL;

//printf_V ("Allocating variable storage\n");
    for (t1 = module_list; t1; t1 = TREE_CHAIN(t1)) {
	t = TREE_PURPOSE(t1);
	if (!BLOCK_UP(t) && !UDP_ATTR(t)) {	/* don't do anything if not top_level or udp */
	    TREE_CHAIN(t) = top_level;
	    top_level = t;
	    make_block_decl(BLOCK_NAME(t), scope0, t);
//      initialize_decls (t);
	}
    }
    /* Do this after all top-levels are resolved so that hierarchical defparams
       can be accurately located */
    for (t = top_level; t; t = TREE_CHAIN(t))
	initialize_decls(t);

    BLOCK_DOWN(scope0) = top_level;
    BLOCK_UP(scope0) = NULL;
    BLOCK_NAME(scope0) = NULL;
}


void print_top_modules()
{
    tree t;

    /* for any module that has not moved, thread it onto the top_level list */
    printf_V("Top-level modules:\n");
    for (t = top_level; t; t = TREE_CHAIN(t))
	printf_V("   %s\n", IDENT(BLOCK_NAME(t)));
    printf_V("\n");
}

/**************************************************************
 *
 *		init_pass2
 *			- initializes global and static variables in pass2.c
 *
 **************************************************************
 */

void init_pass2()
{
    port_connections = NULL_TREE;
}
