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

/* DUMPVAR.C - Value change dump file routines ($dumpvars, etc.) */

#include <stdio.h>
#include <time.h>
#include "glue.h"
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "print.h"
#include "dumpvar.h"
#include "veriwell.h"
#include "timescal.h"

struct Dump dumpvar;
static FILE *dumpfile;


static void dumpvars_scope(tree scope);
static char *dumpvars_xlate(int index);
static void dumpvars_printscope(tree scope);
static void dumpvars_printscopes(tree scope, int level);
static void dumpvars_printvar(Marker * marker, Marker * prev);
static int dumpvars_inside_scopes(tree scope, int scan_only);
static int dumpvars_printvars(tree scope, int level);
static void dumpvars_time(void);
static void dumpvars_print_val(tree decl, int index);
static void dumpvars_dump_all(void);



/* Build a VCD marker on a qualified decl node */

void dumpvars_mark(tree node)
{
    struct Marker_info marker_info;

    marker_info.first = dumpvar.markers_first;
    marker_info.last = dumpvar.markers_last;
    marker_info.delay = NULL;
    marker_info.current_scb = readylist;
    marker_info.flags = M_VCD;
    BuildMarker(node, &marker_info);
    dumpvar.markers_first = marker_info.first;
    dumpvar.markers_last = marker_info.last;
}

/* Mark all of the dump-able vars in a scope */

static void dumpvars_scope(tree scope)
{
    tree decl;
    tree dummy;

    for (decl = BLOCK_PORTS(scope); decl; decl = TREE_CHAIN(decl)) {
	if (!PORT_REDEFINED_ATTR(decl)) {
	    if (is_var(decl, &dummy)) {
		dumpvars_mark(decl);
	    }
	}
    }

    for (decl = BLOCK_DECL(scope); decl; decl = TREE_CHAIN(decl)) {
	if (is_var(decl, &dummy)) {
	    dumpvars_mark(decl);
	}
    }
}

/* recursively traverse hierarchy to "level" levels and marks all the decls
   in each scope */

void dumpvars_scope_down(tree scope, int level)
{
    tree s;

    dumpvars_scope(scope);
    /* Do tasks, functions, and named block w/out regard for level */
    for (s = BLOCK_DOWN(scope); s; s = TREE_CHAIN(s)) {
	if (*tree_code_type[TREE_CODE(s)] == 'b') {
	    if (TREE_CODE(s) != MODULE_BLOCK) {
		dumpvars_scope_down(s, level);
	    }
	}
    }

    if (!--level) {
	return;
    }

    /* Go down to instantiated blocks */
    for (s = BLOCK_DOWN(scope); s; s = TREE_CHAIN(s)) {
	if (*tree_code_type[TREE_CODE(s)] == 'b') {
	    if (TREE_CODE(s) == MODULE_BLOCK) {
		dumpvars_scope_down(s, level);
	    }
	}
    }
}


void dumpvars_all(void)
{
    tree s;

    for (s = top_level; s; s = TREE_CHAIN(s)) {
	dumpvars_scope_down(s, 4000);
    }
}

void dumpvars_enable(void)
{
    Marker *marker;

    dumpvar.enabled = 1;
    for (marker = dumpvar.printed; marker; marker = marker->link) {
	thread_marker(marker);
    }
}

void dumpvars_disable(void)
{
    Marker *marker;

    dumpvar.enabled = 0;
    for (marker = dumpvar.printed; marker; marker = marker->link) {
	REMOVE_LIST(marker);
    }
}

/* translate the index into ascii characters from the legal char set
   (33-126) */

#define XLATE_START 33
#define XLATE_END 126

static char *dumpvars_xlate(int index)
{
    int num_ascii = XLATE_END - XLATE_START + 1;
    int i = 0;
    int accum = index;
    int offset;
    static char buffer[5];

    do {
	offset = accum % num_ascii;
	buffer[i++] = offset + XLATE_START;
	accum /= num_ascii;
    }
    while (accum > 0);
    buffer[i] = '\0';
    return buffer;
}

/* Print a $scope line */

static void dumpvars_printscope(tree scope)
{
    enum tree_code code = TREE_CODE(scope);

    fprintf(dumpfile, "\n$scope ");
    switch (code) {
    case MODULE_BLOCK:
	fprintf(dumpfile, "module");
	break;
    case NAMED_BLOCK:
	fprintf(dumpfile, "begin");
	break;
    case TASK_BLOCK:
	fprintf(dumpfile, "task");
	break;
    case FUNCTION_BLOCK:
	fprintf(dumpfile, "function");
	break;
    }
    fprintf(dumpfile, " %s $end\n", IDENT(BLOCK_NAME(scope)));
}

/* Print one or more $scope lines.  Go up "level" levels
   and start from there. */

static void dumpvars_printscopes(tree scope, int level)
{
    int i;
    int j;
    tree s;

    for (i = level; i; i--) {
	s = scope;
	for (j = i; j; j--) {
	    s = BLOCK_UP(s);
	}
	dumpvars_printscope(s);
    }
    dumpvars_printscope(scope);
}

static void dumpvars_printvar(Marker * marker, Marker * prev)
{
    tree decl = marker->decl;
    int index = marker->expr.vcd_id;
    enum tree_code code = TREE_CODE(decl);
    Marker *tmp;

    fprintf(dumpfile, "$var %-5s %5ld %-4s %s ",
	    decl_type(decl), TREE_NBITS(decl), dumpvars_xlate(index),
	    IDENT(DECL_NAME(decl)));
    if (code == REG_VECTOR_DECL || code == NET_VECTOR_DECL) {
	fprintf(dumpfile, "[%d:%d]", MSB(decl), LSB(decl));
    }
    fprintf(dumpfile, " $end\n");

    /* Thread onto "printed" list */
    tmp = marker->link;
    marker->link = dumpvar.printed;
    dumpvar.printed = marker;
    /* Take off of unprinted list */
    if (prev) {
	prev->link = tmp;
    } else {
	dumpvar.unprinted = tmp;	/* head of the list case */
    }
}

static int dumpvars_inside_scopes(tree scope, int scan_only)
{
    Marker *marker;
    Marker *prev;
    Marker *next;
    tree s;
    int printed_something = 0;
    int printed_scope = 0;

    for (s = BLOCK_DOWN(scope); s; s = TREE_CHAIN(s)) {
	if (TREE_CODE(s) != MODULE_BLOCK) {
	    if (!scan_only) {
		dumpvars_printscope(s);
		printed_scope = 1;
	    }

	    prev = NULL;
	    for (marker = dumpvar.unprinted; marker; marker = next) {
		next = marker->link;	/* do now,since printvar will change link */
		if (DECL_CONTEXT(marker->decl) == s) {
		    printed_something = 1;
		    if (!scan_only) {
			dumpvars_printvar(marker, prev);
		    }
		} else {
		    prev = marker;	/* previous marker not printed out */
		}
	    }

	    printed_something |= dumpvars_inside_scopes(s, scan_only);
	    if (printed_scope) {
		fprintf(dumpfile, "$upscope $end\n\n");
	    }
	}
    }
    return printed_something;
}

/* Recursively print the scopes and vars to "level" levels  */

static int dumpvars_printvars(tree scope, int level)
{
    Marker *marker;
    Marker *prev = NULL;
    Marker *next;
    tree s;
    int printed_something = 0;

    for (marker = dumpvar.unprinted; marker; marker = next) {
	next = marker->link;	/* Do now; printvar will change link */
	if (DECL_CONTEXT(marker->decl) == scope) {
	    if (!printed_something) {
		dumpvars_printscopes(scope, level);
		printed_something = 1;
	    }
	    dumpvars_printvar(marker, prev);
	} else {
	    prev = marker;
	}
    }
    /* This keeps track of what scopes have been printed */
    if (printed_something) {
	level = 0;
    } else {
	level++;
    }

    /* Go down to child modules; do lower scopes later */
    for (s = BLOCK_DOWN(scope); s; s = TREE_CHAIN(s)) {
	if (TREE_CODE(s) == MODULE_BLOCK) {
	    printed_something |= dumpvars_printvars(s, level);
	}
    }

    /* If something in this module or a lower has been printed (dumped)
       then go ahead and print all of the lower scopes, 
       regardless of whether there is something to be printed */

    if (printed_something) {
	dumpvars_inside_scopes(scope, 0);
    } else {
	if (dumpvars_inside_scopes(scope, 1)) {
	    printed_something = 1;
	    dumpvars_inside_scopes(scope, 0);
	}
    }
    if (printed_something) {
	fprintf(dumpfile, "$upscope $end\n\n");
    }
    return printed_something;
}

int dumpvars_header(tree node)
{
    tree scope;
    Marker *marker;
    int id = 0;
    time_t t = time(NULL);
    int units;
    int prec;			/* for timescale for header */
    char timebuf[100];

    for (marker = dumpvar.unprinted; marker; marker = marker->link) {
	marker->expr.vcd_id = id++;
    }
    dumpvar.total = id;

    dumpfile = shell_fopen(dumpvar.file_name, "w");
    if (!dumpfile) {
	runtime_error(node);
	warning("Cannot open dump file '%s'; skipping $dumpvars",
		dumpvar.file_name, NULL);
	return 0;
    }
    fprintf(dumpfile, "$date\n");
    fprintf(dumpfile, "      %s\n", ctime(&t));
    fprintf(dumpfile, "$end\n");
    fprintf(dumpfile, "$version\n");
    fprintf(dumpfile, "      %s %s\n", VERIWELL, VERSION);
    fprintf(dumpfile, "$end\n");

    timescale_get(STMT_SCOPE(node), &units, &prec);
    fprintf(dumpfile, "$timescale\n      %s\n$end\n\n",
	    timescale_string(prec, timebuf));
    for (scope = top_level; scope; scope = TREE_CHAIN(scope)) {
	dumpvars_printvars(scope, 0);
    }
    fprintf(dumpfile, "$enddefinitions      $end\n");
    return 1;
}

static void dumpvars_time(void)
{
    if (dumpvar.new_time) {
	dumpvar.new_time = 0;
	fprintf(dumpfile, "#%s\n", time_string(&CurrentTime));
    }
}

/* Print a single value */

static void dumpvars_print_val(tree decl, int index)
{
    if (TREE_NBITS(decl) == 1) {
	switch (DECL_STATE(decl)) {
	case ZERO:
	    fprintf(dumpfile, "0");
	    break;
	case ONE:
	    fprintf(dumpfile, "1");
	    break;
	case X:
	    fprintf(dumpfile, "x");
	    break;
	case Z:
	    fprintf(dumpfile, "z");
	    break;
	}
    } else {
	fprintf(dumpfile, "b");
	print_binary_file(dumpfile, DECL_STORAGE(decl), TREE_NBITS(decl));
	fprintf(dumpfile, " ");
    }
    fprintf(dumpfile, "%s", dumpvars_xlate(index));
}

/* Print the current values of all dumped vars */

static void dumpvars_dump_all(void)
{
    Marker *marker;

    for (marker = dumpvar.printed; marker; marker = marker->link) {
	dumpvars_print_val(marker->decl, marker->expr.vcd_id);
	fprintf(dumpfile, "\n");
    }
}

/* Print a variable change; print the time first if needed;
   Called from sched.c */

void dumpvars_dump(tree decl, int index)
{
    dumpvars_time();
    dumpvars_print_val(decl, index);
    fprintf(dumpfile, "\n");
}

/* Print first line of dump */

void dumpvars_checkpoint(char *which)
{
    dumpvars_time();
    fprintf(dumpfile, "%s\n", which);
    dumpvars_dump_all();
    fprintf(dumpfile, "$end\n");
}

void dumpvars_x(char *which)
{
    Marker *marker;

    dumpvars_time();
    fprintf(dumpfile, "%s ", which);

    for (marker = dumpvar.printed; marker; marker = marker->link) {
	tree decl = marker->decl;
	int index = marker->expr.vcd_id;

	if (TREE_NBITS(decl) == 1) {
	    fprintf(dumpfile, "x");
	} else {
	    fprintf(dumpfile, "bx");
	}
	fprintf(dumpfile, " ");

	fprintf(dumpfile, "%s\n", dumpvars_xlate(index));
    }
    fprintf(dumpfile, "$end\n\n");
}

/* Flush the dumpfile */

void dumpvars_flush()
{
    if (dumpfile) {
	fflush(dumpfile);
    }
}

void init_dumpvar(void)
{
    dumpvar.file_name = "veriwell.dmp";
    dumpfile = NULL;
}
