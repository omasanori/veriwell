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

/* MACRO.C - Macro definitions and expansion; push/pop streams */
#define MACRO_C

#include <stdio.h>
#include <string.h>
#include "vtypes.h"
#include "tree.h"
#include "obstack.h"
#include "io.h"
#include "macro.h"
#include "glue.h"
#include "lex.h"
#include "veriwell.h"

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free


/* Macros are stored (for now) as a chained tree list of string nodes */
static tree macro_chain = NULL_TREE;

void define_macro(tree name, tree rest)
{
    tree new_t;
/* look for same name */

    if (rest && ((new_t = lookup_macro(IDENTIFIER_POINTER(name))) != 0)) {

//  Elliot: verilog does not warn about this
//    warning ("Macro %s previously defined, overriding\n", IDENT (name), NULL_CHAR);
	TREE_VALUE(new_t) = rest;	/* replace old definition */
    } else
	macro_chain = tree_cons(name, rest, macro_chain);
}

tree lookup_macro(char *name)
{
    tree p;

//printf_V ("Macro name: '%s':\n", name);
    for (p = macro_chain;
	 p && strcmp(name, IDENTIFIER_POINTER(TREE_PURPOSE(p))) != 0;
	 p = TREE_CHAIN(p));
//    printf_V ("    '%s'\n", IDENT (TREE_PURPOSE (p)));
// printf_V ("Returning %p\n\n", p);
    return p;
}

void delete_macro(int len, char *name)
{
    tree p;
    tree *parent;
    parent = &macro_chain;
    for (p = *parent;
	 p && strncmp(name, IDENTIFIER_POINTER(TREE_PURPOSE(p)), len) != 0
	 && strlen(IDENTIFIER_POINTER(TREE_PURPOSE(p))) == len;
	 parent = &p, p = TREE_CHAIN(p));

    // ??? mdh - note this is wasteful, object is left dangling
    //           on obstack.
    if (p) {
	*parent = TREE_CHAIN(p);
    } else {
	*parent = 0;
    }

}

void expand_macro(char *name)
{
    tree found = lookup_macro(name);

    // IEEE spec say expandition of undefined macros is NULL
    if (found) {
	push_stream(fin, 0);
	fin = File::fopenbuf(TREE_STRING_POINTER(TREE_VALUE(found)),
			     TREE_STRING_LENGTH(TREE_VALUE(found)));
    }
}

void open_include(char *filename)
{
    File *include;
    char *include_name;

    if (incdir && *incdir) { // amitrupu_fix: when no +incdir given
	char *p;
	char *tmpdir;

	tmpdir = (char *) xmalloc(strlen(incdir) + 1);
	strcpy(tmpdir, incdir);
	for (p = strtok(tmpdir, "+"); p; p = strtok(NULL, "+")) {
	    include_name =
		(char *) xmalloc(strlen(filename) + strlen(p) + 1);
	    strcpy(include_name, p);
	    strcat(include_name, PATHSEP);
	    strcat(include_name, &filename[1]);

	    include = File::fopen(include_name, "rt");
	    if (include)
		break;
	    free(include_name);
	}
    } else {
	include_name = (char *) xmalloc(strlen(filename) + 1);
	strcpy(include_name, &filename[1]);	/* strip leading " */

	include = File::fopen(include_name, "rt");
    }
    if (!include)
	error("Could not open include file '%s'", &filename[1], NULL_CHAR);
    else {
	printf_V("Compiling included source file '%s'\n", include_name);
	push_stream(fin, 1);
	lineno = 1;
	fin = include;
	input_filename = include_name;
    }
}

static struct obstack macro_obstack;

struct macro_level {
    struct macro_level *prev;	/* where to pop to */
    File *file;			/* FILE structure within stack */
    char *filename;		/* filename (if pushing `include file) */
    lineno_t lineno;		/* Linenumber to pop to */
    int include;		/* Non-zero if this is include file */
};

static struct macro_level *macro_stack;

void init_macro()
{
    tree name, rest;

    obstack_init(&macro_obstack);
    macro_stack = 0;

    name = build_string(12, "__VERIWELL__");
    rest = build_string(0, "");
    define_macro(name, rest);
}

/* include == 0: Macro
   include == 1: included file (`include)
   include == 2: library or pushed file that needs to be closed
*/

void push_stream(File * file, int include)
{
    struct macro_level *tem
	= (struct macro_level *) obstack_alloc(&macro_obstack,
					       sizeof(struct macro_level));
    tem->prev = macro_stack;
    tem->file = file;		/* Copy entire FILE structure onto stack */
    tem->filename = input_filename;
    tem->lineno = lineno;
    tem->include = include;
    macro_stack = tem;
}

File *pop_stream(void)
{
    struct macro_level *tem = macro_stack;
    File *tempfile;

    if (macro_stack == 0) {
	if (fin)
	    fin->fclose();
	return 0;
    }

    input_filename = tem->filename;
    if (tem->include) {
	if (tem->include == 1)
	    printf_V("Continuing compilation of source file '%s'\n",
		     input_filename);
	fin->fclose();
    }
    macro_stack = tem->prev;
    tempfile = tem->file;
    lineno = tem->lineno;
    obstack_free(&macro_obstack, (POINTER) tem);
    return tempfile;
}

static struct obstack input_obstack;

struct input_level {
    struct input_level *prev;	/* where to pop to */
    FILE *file;			/* FILE structure within stack */
};

static struct input_level *input_stack;

void init_input()
{
    obstack_init(&input_obstack);
    input_stack = 0;
}

/*****************************************************************
 *
 *		init_macro_1
 *			- initializes global and static variables for macro.c
 *
 *****************************************************************
 */

void init_macro_1()
{
    macro_chain = NULL_TREE;
    memset(&macro_obstack, 0, sizeof(macro_obstack));
    memset(&macro_stack, 0, sizeof(macro_stack));
    memset(&input_obstack, 0, sizeof(input_obstack));
    memset(&input_stack, 0, sizeof(input_stack));
}
