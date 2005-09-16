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

#define TREE_C

#include <stdio.h>
#include <stdlib.h>
//#if !defined(THINK_C) && !defined(__MWERKS__)
//#include <varargs.h>
//#endif // THINK_C
#include <stdarg.h>
#include <string.h>
#include "vtypes.h"
#include "tree.h"
//#ifndef __GNUC__
#include "obstack.h"
//#else
//#include <obstack.h>
//#endif
#include "decl.h"
#include "glue.h"
#include "scope.h"
#include "veriwell.h"
#include "timescal.h"
#if VDEBUG != 0
#include "flags.h"
#endif

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

tree resolve_hierarchical_name(tree);

//extern void * xmalloc ();

/* Tree nodes of permanent duration are allocated in this obstack.
   They are the identifier nodes, declaration nodes, and
   block nodes. */

struct obstack permanent_obstack;

/* Each module has its own stack for its statements so that they
   can be deallocated on a per-module basis after its code has been
   generated.  These are initialized dynamically.  */

struct obstack *module_obstack;

/* Scaffolding (information needed between phases) are put in the linkage
   stack. */

struct obstack linkage_obstack;

/* Low-level code that is generated from syntax trees is put on the
   code stack. */

struct obstack code_obstack;

/* Expressions are put on the temporary stack while they are being
   processed.  They are copied to the permanent stack after being
   folded and converted, where necessary. */

struct obstack temporary_obstack;

/* This usually points to permanent obstack. */

struct obstack *saveable_obstack;

/* This points at either permanent_obstack or temporary_obstack.  */

struct obstack *current_obstack;

/* This points to either permanent, temporary, or momentary obstack. */

struct obstack *expression_obstack;

/* Addresses of first objects in some obstacks.
   This is for freeing their entire contents.  */
char *temporary_firstobj;

/* Table indexed by tree code giving a string containing a character
   classifying the tree code.  Possibilities are
   d, s, c, r and e.  See tree.def for details.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) TYPE,

char *tree_code_type[] = {
#include "tree.def"
};

#undef DEFTREECODE

/* Table indexed by tree code giving number of expression
   operands beyond the fixed part of the node structure.
   Not used for types or decls.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) LENGTH,

int tree_code_length[] = {
#include "tree.def"
};

#undef DEFTREECODE

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) NAME,

char *tree_code_name[] = {
#include "tree.def"
};

#undef DEFTREECODE

/* Hash table for uniquizing IDENTIFIER_NODEs by name.  */

#define MAX_HASH_TABLE 1009
static tree hash_table[MAX_HASH_TABLE];	/* id hash buckets */

/* 0 while creating built-in identifiers.  */
static int do_identifier_warnings;

/* Used to pass lsb and msb expression pointers during parsing. */
tree range_handle;

/* Points to the toplevel of the tree */
tree top_level = NULL_TREE;

/* List of continuous assignments */
tree continuous_assignments = NULL_TREE;

/* Init data for node creation, at the beginning of compilation.  */

tree build_decl(enum tree_code code, tree name, enum tree_type type);

void init_tree()
{
    obstack_init(&permanent_obstack);
    obstack_init(&linkage_obstack);

    obstack_init(&temporary_obstack);
    temporary_firstobj = (char *) obstack_alloc(&temporary_obstack, 0);

    current_obstack = &permanent_obstack;
//  expression_obstack = &momentary_obstack;
    expression_obstack = &permanent_obstack;

    range_handle = make_node(RANGE_HANDLE);
    integer_zero_node = build_int_cst((unsigned_32_t) 0);
    integer_one_node = build_int_cst((unsigned_32_t) 1);
    error_mark_node = make_node(ERROR_MARK);
    TREE_NBITS(error_mark_node) = 0;
}

/* Allocate SIZE bytes in the current obstack
   and return a pointer to them.
   In practice the current obstack is always the temporary one.  */

char *oballoc(long size)
{
    return (char *) obstack_alloc(current_obstack, size);
}

/* Free the object PTR in the current obstack
   as well as everything allocated since PTR.
   In practice the current obstack is always the temporary one.  */

void obfree(char *ptr)
{
    obstack_free(current_obstack, ptr);
}

/* Allocate SIZE bytes in the permanent obstack
   and return a pointer to them.  */

char *permalloc(long size)
{
    char *tmp = (char *) obstack_alloc(&permanent_obstack, size);
    return tmp;
}

/* Allocate SIZE bytes in the linkage obstack
   and return a pointer to them.  */

char *linkalloc(long size)
{
    return (char *) obstack_alloc(&linkage_obstack, size);
}

/* clear everything out of the temporary obstack */

void clear_temporary()
{
    obstack_free(&temporary_obstack, temporary_firstobj);
}

tree build_int_cst(signed_32_t i)
{
    tree t = make_node(INTEGER_CST);

//  INT_CST_HIGH (t) = HIGH_INT (i);
//  INT_CST_LOW (t) = LOW_INT (i);
    INT_CST_DATA(t) = i;
    TREE_CONSTANT_ATTR(t) = 1;
    TREE_INTEGER_ATTR(t) = 1;
    TREE_NBITS(t) = 32;
    return t;
}

tree build_real_cst(char *real_string)
{
    tree t = make_node(REAL_CST);
    REAL_CST_DATA(t) = atof(real_string);
    TREE_CONSTANT_ATTR(t) = 1;
    TREE_REAL_ATTR(t) = 1;
    TREE_NBITS(t) = 32;
    return t;
}

void switch_to_linkage_obstack()
{
    current_obstack = &linkage_obstack;
}

void switch_to_permanent_obstack()
{
    current_obstack = &permanent_obstack;
}


/* Return a newly allocated node of code CODE.
   Initialize the node's unique id and its TREE_PERMANENT flag.
   For decl and type nodes, some other fields are initialized.
   The rest of the node is initialized to zero.

   Achoo!  I got a code in the node.  */

int num_nodes = 0;
int num_nodes_g = 0;
int num_nodes_e = 0;
int num_nodes_b = 0;
int num_nodes_s = 0;
int num_nodes_r = 0;
int num_nodes_c = 0;
int num_nodes_d = 0;
int num_nodes_x = 0;

tree make_node(enum tree_code code)
{
    register tree t;
    register int type = *tree_code_type[(int) code];
    register int length;
    register struct obstack *obstack = current_obstack;

    num_nodes++;
//HEAPTEST ("Heap corrupted (in make_node)");

    switch (type) {
    case 'b':			/* A block node */
	num_nodes_b++;
	length = sizeof(struct tree_block_common)
	    + tree_code_length[(int) code] * sizeof(char *);
	obstack = &permanent_obstack;
	break;

    case 'd':			/* A decl node */
	num_nodes_d++;
	length = sizeof(struct tree_decl_common)
	    + tree_code_length[(int) code] * sizeof(char *);
	obstack = &permanent_obstack;
	break;

    case 'g':			/* a gate node */
	num_nodes_g++;
	length = sizeof(struct tree_gate_instance)
	    + (tree_code_length[(int) code]) * sizeof(char *);
	/* All gates are put where we can preserve them if nec.  */
	if (obstack != &permanent_obstack)
	    obstack = module_obstack;
	break;

    case 's':			/* a stmt node */
	num_nodes_s++;
	length = sizeof(struct tree_stmt)
	    + (tree_code_length[(int) code] - 1) * sizeof(char *);
	/* All stmts are put where we can preserve them if nec.  */
	if (obstack != &permanent_obstack)
	    obstack = module_obstack;
	break;

    case 'r':			/* a reference */
	num_nodes_r++;
	goto e;
    case 'e':			/* an expression */
	num_nodes_e++;
      e:
	obstack = expression_obstack;
	length = sizeof(struct tree_exp)
	    + (tree_code_length[(int) code] - 1) * sizeof(char *);
	break;

    case 'c':			/* a constant */
	num_nodes_c++;
	obstack = expression_obstack;
	goto x;

    case 'x':			/* something random, like an identifier.  */
	num_nodes_x++;
      x:length = sizeof(struct tree_common)
	    + tree_code_length[(int) code] * sizeof(char *);
	/* Identifier nodes are always permanent since they are
	   unique in a compiler run.  */
	if (code == IDENTIFIER_NODE)
	    obstack = &permanent_obstack;
    }

//HEAPTEST ("In make_node, before alloc");
    t = (tree) obstack_alloc(obstack, length);

    memset(t, 0, length);
    TREE_SET_TYPE(t, 0);
    TREE_CHAIN(t) = 0;

    TREE_SET_CODE(t, code);
    TREE_LABEL(t) = 0;
    TREE_SUB_LABEL(t) = 0;

    if (type == 's' || type == 'd' || type == 'b') {
	STMT_SOURCE_LINE(t) = (type == 's'
			       || type == 'b') ? stmt_lineno : lineno;
#if VDEBUG != 0
	if (vdebug)
	    printf_V("Line %d\n", STMT_SOURCE_LINE(t));
#endif
	STMT_SOURCE_FILE(t) = input_filename;
	if (type == 'd')
	    DECL_EVENT_NOCAST(t) = NULL;
	else if (type == 's')
	    STMT_SCOPE(t) = current_scope;
    }
    if (code == REAL_DECL)
	TREE_REAL_ATTR(t) = 1;

#if VDEBUG != 0
    if (vdebug) {
	extern unsigned long mem;
	unsigned long mem_left = mem_avail();

	printf_V("Made node '%s', at %p, size %d\n", tree_code_name[code],
		 t, length);
	printf_V("Memory used: %lu\n", mem - mem_left);
	printf_V("Memory left: %lu\n", mem_left);

    }
#endif
//HEAPTEST ("Heap corrupted (make_node)");

    return t;
}

/* Return a new node with the same contents as NODE
   except that its TREE_CHAIN is zero and it has a fresh uid.  */

tree copy_node(tree node)
{
    register tree t;
    register enum tree_code code = TREE_CODE(node);
    register int length;

    HEAPTEST("In copy_node");
    switch (*tree_code_type[(int) code]) {
    case 'b':			/* A block node */
	length = sizeof(struct tree_block_common)
	    + tree_code_length[(int) code] * sizeof(char *);
	break;

    case 'd':			/* A decl node */
	length = sizeof(struct tree_decl_common)
	    + tree_code_length[(int) code] * sizeof(char *);
	break;

    case 's':			/* a stmt node */
	length = sizeof(struct tree_stmt)
	    + (tree_code_length[(int) code] - 1) * sizeof(char *);
	break;


    case 'g':			/* a gate node */
	length = sizeof(struct tree_gate_instance)
	    + (tree_code_length[(int) code]) * sizeof(char *);
	break;

    case 'r':			/* a reference */
    case 'e':			/* an expression */
	length = sizeof(struct tree_exp)
	    + (tree_code_length[(int) code] - 1) * sizeof(char *);
	break;

    case 'c':			/* a constant */
    case 'x':			/* something random, like an identifier.  */
	length = sizeof(struct tree_common)
	    + tree_code_length[(int) code] * sizeof(char *);
    }

    HEAPTEST("In copy_node, before alloc");
//printf ("next free = %p, limit = %p\n", current_obstack->next_free, current_obstack->chunk_limit);
//printf ("length is %d\n", length);
//  t = (tree) obstack_alloc (current_obstack, length);
    t = (tree) obstack_copy(current_obstack, node, length);
    HEAPTEST("In copy_node, after alloc");
//printf ("Allocated node %p, length %d\n", t, length);
//printf ("alignment %d, base %p, limit %p\n",
//current_obstack->alignment_mask, current_obstack->object_base, current_obstack->chunk_limit);

//  for (i = ((length + sizeof (int) - 1) / sizeof (int)) - 1;
//       i >= 0;
//       i--)
//    ((int *) t)[i] = ((int *) node)[i];
    HEAPTEST("In copy_node, after alloc and init");

    TREE_CHAIN(t) = 0;

    return t;
}


#define HASHBITS 30

/* Return an IDENTIFIER_NODE whose name is TEXT (a null-terminated string).
   If an identifier with that name has previously been referred to,
   the same node is returned this time.  */

tree get_identifier(char *text)
{
    register unsigned hi;
    register int i;
    register tree idp;
    register int len, hash_len;

    /* Compute length of text in len.  */
    for (len = 0; text[len]; len++);

    /* Decide how much of that length to hash on */
    hash_len = len;
/*  if (warn_id_clash && len > id_clash_len)
 *    hash_len = id_clash_len;
 */

    /* Compute hash code */
    hi = hash_len;
    for (i = 0; i < hash_len; i++)
	hi = ((hi * 613) + (unsigned) (text[i]));

    hi &= (1 << HASHBITS) - 1;
    hi %= MAX_HASH_TABLE;

    /* Search table for identifier */
    for (idp = hash_table[hi]; idp; idp = TREE_CHAIN(idp))
	if (IDENTIFIER_LENGTH(idp) == len
	    && !strcmp(IDENTIFIER_POINTER(idp), text))
	    return idp;		/* <-- return if found */

    /* Not found; optionally warn about a similar identifier */
/*
  if (warn_id_clash && do_identifier_warnings && len >= id_clash_len)
    for (idp = hash_table[hi]; idp; idp = TREE_CHAIN (idp))
      if (!strncmp (IDENTIFIER_POINTER (idp), text, id_clash_len))
	{
	  warning ("`%s' and `%s' identical in first n characters",
		   IDENTIFIER_POINTER (idp), text);
	  break;
	}
*/
    /* Not found, create one, add to chain */
    idp = make_node(IDENTIFIER_NODE);
    IDENTIFIER_LENGTH(idp) = len;
    IDENTIFIER_POINTER(idp) =
	(char *) obstack_copy0(&permanent_obstack, text, len);
    TREE_CHAIN(idp) = hash_table[hi];
    hash_table[hi] = idp;
    TREE_NBITS(idp) = 32;	/* A default number for hierarchical names */
    return idp;			/* <-- return if created */
}

/* Enable warnings on similar identifiers (if requested).
   Done after the built-in identifiers are created.  */

void start_identifier_warnings()
{
    do_identifier_warnings = 1;
}


/* Return a node that points to a bit constant. */

tree build_bit_cst(nbits_t length, enum radii radix)
{
    register tree t = make_node(BIT_CST);
    Group *g =
	(Group *) permalloc((bits_to_groups(length) + 1) * sizeof(Group));


    BIT_CST_GROUP(t) = g;
    BIT_CST_NBITS(t) = length;
    BIT_CST_SET_RADIX(t, radix);
    TREE_TYPE(t) = REG_TYPE;
    TREE_CONSTANT_ATTR(t) = 1;
    TREE_NBITS(t) = length;	/* default to this; this may be changed at pass 2 */

    return t;
}

/* Return a newly constructed STRING_CST node whose value is
   the LEN characters at STR.
   The TREE_TYPE is not initialized.  */

tree build_string(int len, char *str)
{
    register tree s = make_node(STRING_CST);
    TREE_STRING_LENGTH(s) = len;
    TREE_STRING_POINTER(s) =
	(char *) obstack_copy0(current_obstack, str, len);
    return s;
}



/* Return the length of a chain of nodes chained through TREE_CHAIN.
   We expect a null pointer to mark the end of the chain.
   This is the Lisp primitive `length'.  */

int list_length(tree t)
{
    register tree tail;
    register int len = 0;

    for (tail = t; tail; tail = TREE_CHAIN(tail))
	len++;

    return len;
}

/* Concatenate two chains of nodes (chained through TREE_CHAIN)
   by modifying the last node in chain 1 to point to chain 2.
   This is the Lisp primitive `nconc'.  */

tree chainon(tree op1, tree op2)
{
    tree t;

    if (op1) {
	for (t = op1; TREE_CHAIN(t); t = TREE_CHAIN(t))
	    if (t == op2) {
		printf("Fatal: Circularity during compilation\n");
//    printf ("%lu\n", coreleft ());
		shell_abort();	/* Circularity being created */
	    }
	TREE_CHAIN(t) = op2;
	return op1;
    } else
	return op2;
}

/* Return a newly created TREE_LIST node whose
   purpose and value fields are PARM and VALUE.  */

tree build_tree_list(tree parm, tree value)
{
    register tree t = make_node(TREE_LIST);
    TREE_PURPOSE(t) = parm;
    TREE_VALUE(t) = value;
    TREE_3RD(t) = NULL_TREE;
    return t;
}

/* Return a newly created TREE_LIST node whose
   purpose and value fields are PARM and VALUE
   and whose TREE_CHAIN is CHAIN.  */

tree tree_cons(tree purpose, tree value, tree chain)
{
    register tree node = make_node(TREE_LIST);
    TREE_CHAIN(node) = chain;
    TREE_PURPOSE(node) = purpose;
    TREE_VALUE(node) = value;
    TREE_3RD(node) = NULL_TREE;
    return node;
}

/* Same as `tree_cons' but make a permanent object.  */

tree perm_tree_cons(tree purpose, tree value, tree chain)
{
    register tree node;
    register struct obstack *ambient_obstack = current_obstack;
    current_obstack = &permanent_obstack;

    node = make_node(TREE_LIST);
    TREE_CHAIN(node) = chain;
    TREE_PURPOSE(node) = purpose;
    TREE_VALUE(node) = value;
    TREE_3RD(node) = NULL_TREE;

    current_obstack = ambient_obstack;
    return node;
}

tree free_link = NULL_TREE;

/* Same as `tree_cons' but make a linkage object.  */

tree link_tree_cons(tree purpose, tree value, tree chain)
{
    register tree node;
    register struct obstack *ambient_obstack = current_obstack;
    current_obstack = &permanent_obstack;

    if (!free_link)
	node = make_node(TREE_LIST);
    else {
	node = free_link;
	free_link = TREE_CHAIN(free_link);
    }
    TREE_CHAIN(node) = chain;
    TREE_PURPOSE(node) = purpose;
    TREE_VALUE(node) = value;
    TREE_3RD(node) = NULL_TREE;

    current_obstack = ambient_obstack;
    return node;
}

void free_link_list(tree node)
{
    TREE_CHAIN(node) = free_link;
    free_link = node;
}

/* Reverse the order of elements in the chain T,
   and return the new head of the chain (old last element).  */

tree nreverse(tree t)
{
    register tree prev = 0, decl, next;
    for (decl = t; decl; decl = next) {
	next = TREE_CHAIN(decl);
	TREE_CHAIN(decl) = prev;
	prev = decl;
    }
    return prev;
}

void set_all_list_values(tree list, tree value)
{
    tree t;

    for (t = list; t; t = TREE_CHAIN(t))
	TREE_VALUE(t) = value;
}

/* Return non-zero if node is a vector net or scalar net */

int is_net_code(enum tree_code code)
{

/* Return nonzero if node is a net or a reference to a net or any entry
   of a concat is a net. */

    if (code == NET_VECTOR_DECL || code == NET_SCALAR_DECL)
	return 1;
    else
	return 0;
}

int is_all_net(tree node)
{
    enum tree_code code = TREE_CODE(node);
    tree t;

    if (code == BIT_REF)
	code = TREE_CODE(BIT_REF_DECL(node));
    else if (code == PART_REF)
	code = TREE_CODE(PART_DECL(node));

    if (is_net_code(code))
	return 1;
    if (code == CONCAT_REF) {
	for (t = CONCAT_LIST(node); t; t = TREE_CHAIN(t))
	    if (!is_all_net(TREE_EXPR(t)))
		return 0;
	return 1;
    }

    return 0;
}

/* Return nonzero if REF is a reference to an array */

int is_array(tree ref)
{
    if (ref)
	return (TREE_CODE(ref) == ARRAY_REF);
    else
	return 0;
}

/* Return nonzero if REF is a string */

int is_string(tree ref)
{
    if (ref)
	if (TREE_CODE(ref) == BIT_CST)
	    return (BIT_CST_RADIX(ref) == STRING_);
    return 0;
}

/* Return non-zero if pure decl (used for $dumpvar, currently) as well
   as the actual decl node. */

int is_var(tree node, tree * decl)
{
    enum tree_code code = TREE_CODE(node);

    /* Hierarchical name; get decl node then check again */
    if (code == IDENTIFIER_NODE && HIERARCHICAL_ATTR(node)) {
	*decl = resolve_hierarchical_name(node);
	return is_var(*decl, decl);
    }

    /* Must be a DECL and not array nor parameter (for dumpvars) */
    if (*tree_code_type[code] == 'd')
	if (code != ARRAY_DECL && code != PARAM_DECL && code != BLOCK_DECL) {
	    *decl = node;
	    return 1;
	}

    return 0;
}

/* Return nonzero if var or hierarchical ref is a block as well as actual
   BLOCK node. */

int is_block(tree node, tree * scope)
{
    enum tree_code code = TREE_CODE(node);

    /* Hierarchical scope, get actual BLOCK node */
    if (code == IDENTIFIER_NODE && HIERARCHICAL_ATTR(node)) {
	*scope = search_scope(IDENT(node), 0);
	return is_block(*scope, scope);
    }

    /* This could be either a pointer to a block (if called recursively)... */
    if (*tree_code_type[code] == 'b') {
	*scope = node;
	return 1;
    }
    /* ...or a pointer to a BLOCK_DECL */
    else if (code == BLOCK_DECL) {
	*scope = DECL_CONTEXT(node);
	return 1;
    }

    return 0;
}

/* return non-zero if node is a legal procedural lvalue. */

int is_proc_lval(tree node)
{
    enum tree_code code = TREE_CODE(node);

    if (*tree_code_type[code] == 'c' || *tree_code_type[code] == 'e')
	return 0;
    if (is_all_net(node))
	return 0;
    if (code == FUNCTION_REF)
	return 0;
    return 1;
}

/* Return name of identifier, be it hierarchical or otherwise */

char *ident_name(tree node)
{
    enum tree_code code = TREE_CODE(node);

    if (code == IDENTIFIER_NODE)
	return IDENT(node);
    else
	return IDENT(DECL_NAME(node));
}

/* Given an lval, check if it is a "SHADOW_LVAL_REF".  If so, return the
   lval that it is shadowing (a hierarchical reference). */

tree the_lval(tree lval_arg)
{
    tree lval;

    if (TREE_CODE(lval_arg) != SHADOW_LVAL_REF)
	return lval_arg;
    lval = TREE_CHAIN(lval_arg);	/* This points to the actual decl */
    return lval;
}

/* Return the string of the type, suitable for printing */

char *decl_type(tree node)
{
    enum tree_code code = TREE_CODE(node);

    switch (code) {
    case EVENT_DECL:
	return "event";
    case INTEGER_DECL:
	return "integer";
    case REG_SCALAR_DECL:
	return "reg";
    case REG_VECTOR_DECL:
	return "reg";
    case TIME_DECL:
	return "time";
    case REAL_DECL:
	return "real";
    case NET_SCALAR_DECL:
    case NET_VECTOR_DECL:
	switch (TREE_TYPE(node)) {
	case NET_WIRE_TYPE:
	    return "wire";
	case NET_TRI_TYPE:
	    return "tri";
	case NET_WAND_TYPE:
	    return "wand";
	case NET_TRIAND_TYPE:
	    return "triand";
	case NET_WOR_TYPE:
	    return "wor";
	case NET_TRIOR_TYPE:
	    return "trior";
	case NET_TRI0_TYPE:
	    return "tri0";
	case NET_TRI1_TYPE:
	    return "tri1";
	case NET_SUPPLY0_TYPE:
	    return "supply0";
	case NET_SUPPLY1_TYPE:
	    return "supply1";
	}
    }
    return "";
}


/* Low-level constructors for expressions.  */

/* Build an expression of code CODE, data type TYPE,
   and operands as specified by the arguments ARG1 and following arguments.
   Expressions and reference nodes can be created this way.
   Constants, decls, types and misc nodes cannot be.  */

tree build(enum tree_code code, enum tree_type type, ...)
{
    /*register */ va_list p;
    register tree t;
    register int length;
    register int i;

    va_start(p, type);

//  code = va_arg (p, enum tree_code);
    t = make_node(code);
    length = tree_code_length[(int) code];
    TREE_TYPE(t) = type;
//  TREE_TYPE (t) = va_arg (p, enum tree_type);

    if (length == 2) {
	/* This is equivalent to the loop below, but faster.  */
	register tree arg0 = va_arg(p, tree);
	register tree arg1 = va_arg(p, tree);
	TREE_OPERAND(t, 0) = arg0;
	TREE_OPERAND(t, 1) = arg1;
    } else {
	for (i = 0; i < length; i++) {
	    register tree operand = va_arg(p, tree);
	    TREE_OPERAND(t, i) = operand;
	}
    }
    va_end(p);
    return t;
}

/* Similar except don't specify the TREE_TYPE
   and leave the TREE_VOLATILE as 0.
   It is permissible for arguments to be null,
   or even garbage if their values do not matter.  */

tree build_nt(enum tree_code code, ...)
{
    /*register */ va_list p;
    register tree t;
    register int length;
    register int i;

    va_start(p, code);

    t = make_node(code);
    length = tree_code_length[(int) code];

    for (i = 0; i < length; i++)
	TREE_OPERAND(t, i) = va_arg(p, tree);

    va_end(p);
    return t;
}

/* Create a DECL_... node of code CODE, name NAME and data type TYPE.
   We do NOT enter this node in any sort of symbol table.

   layout_decl is used to set up the decl's storage layout.
   Other slots are initialized to 0 or null pointers.  */

tree build_decl(enum tree_code code, tree name, enum tree_type type)
{
    register tree t;

    t = make_node(code);

/*  if (type == error_mark_node)
    type = integer_type_node; */
/* That is not done, deliberately, so that having error_mark_node
   as the type can suppress useless errors in the use of this variable.  */

    DECL_NAME(t) = name;
    TREE_TYPE(t) = type;

    return t;
}

/* Build an array node */
tree build_array(tree ident, tree decl, tree lobounds, tree hibounds)
{
    tree node = make_node(ARRAY_DECL);

//  ARRAY_DECL (node) = decl;
    ARRAY_CODE(node) = TREE_CODE(decl);
    ARRAY_HI_EXPR(node) = hibounds;
    ARRAY_LO_EXPR(node) = lobounds;
    DECL_NAME(node) = ident;
    DECL_MSB(node) = DECL_MSB(decl);
    DECL_LSB(node) = DECL_LSB(decl);
    if (TREE_CODE(decl) == INTEGER_DECL)
	TREE_INTEGER_ATTR(node) = 1;
    if (TREE_CODE(decl) == REAL_DECL)
	TREE_REAL_ATTR(node) = 1;
    return node;
}

/* Build a module block */
tree build_module(tree ident)
{
    tree tmp = make_node(MODULE_BLOCK);
#if VDEBUG != 0
    if (vdebug)
	printf_error_V("Building module %s\n", IDENTIFIER_POINTER(ident));
#endif
    BLOCK_NAME(tmp) = ident;
    MODULE_NAME(tmp) = IDENTIFIER_POINTER(ident);
    BLOCK_UP(tmp) = BLOCK_DECL(tmp) = BLOCK_PORTS(tmp) = NULL_TREE;
    BLOCK_DOWN(tmp) = BLOCK_ARGS(tmp) = NULL_TREE;
    MODULE_DEFPARAMS(tmp) = NULL_TREE;
    MODULE_SPECDEFS(tmp) = NULL_TREE;
    MODULE_SPECINST(tmp) = NULL_TREE;
    /* Module names have their own namespace, so the following remains comment out */
    /*  IDENT_CURRENT_DECL (ident) = tmp; *//* make ident point to mod */
    set_decl(ident, tmp);
    module_timescale(tmp);
    return tmp;
}

/* Clean up at end of module parse */

void end_module(tree node)
{
    tree t, t1, t2;
    char *name;

    for (t = MODULE_PORT_LIST(node); t; t = TREE_CHAIN(t)) {
	t1 = TREE_PURPOSE(t);
	t2 = root_port_decl(t1);
	if (!t2 || TREE_CODE(t2) == IDENTIFIER_NODE)
	    error("Port '%s' not declared", IDENT(t1), NULL_CHAR);
	else if (!PORT_INPUT_ATTR(t1) && !PORT_OUTPUT_ATTR(t1)) {
	    if (TREE_CODE(t1) == IDENTIFIER_NODE)
		name = IDENT(t1);
	    else
		name = IDENT(DECL_NAME(t1));
	    error("Port '%s' not declared as INPUT, OUTPUT, or INOUT",
		  name, NULL_CHAR);
	}
    }
}

extern tree current_scope;

/* Build a task block */
tree build_task(tree ident)
{
    tree tmp = make_node(TASK_BLOCK);

    BLOCK_NAME(tmp) = ident;
    BLOCK_UP(tmp) = current_scope;
//  make_decl (ident, current_scope);
    return tmp;
}

/* Build a function block */
tree build_function(tree ident)
{
    tree tmp = make_node(FUNCTION_BLOCK);

    BLOCK_NAME(tmp) = ident;
    BLOCK_UP(tmp) = current_scope;
//  make_decl (ident, current_scope);
    return tmp;
}

tree build_stmt(enum tree_code code, ...)
{
    /*register */ va_list p;
//  register enum tree_code code;
    register tree t;
    register int length;
    register int i;

    va_start(p, code);

//  code = va_arg (p, enum tree_code);
    t = make_node(code);
    if (*tree_code_type[(int) code] == 's')
	STMT_SOURCE_LINE(t) = va_arg(p, lineno_t);
    length = tree_code_length[(int) code];

    for (i = 0; i < length; i++)
	STMT_OPERAND(t, i) = va_arg(p, tree);

    va_end(p);
    return t;
}

/* add a node to the list of continuous assignments
   (used during intialization) */

void add_cont_assign_list(tree node)
{
    continuous_assignments =
	tree_cons(node, NULL_TREE, continuous_assignments);
}

/* Low-level constructors for statements.
   These constructors all expect source file name and line number
   as arguments, as well as enough arguments to fill in the data
   in the statement node.  */

tree
build_cont_assign(tree lval, tree rval, lineno_t lineno, tree delay,
		  int split)
{
    tree stmt, tmp_var;

    if (split) {
	/* Make two assignments */

	if (TREE_CODE(lval) == NET_SCALAR_DECL)
	    tmp_var = make_node(TMP_DECL);
	else
	    tmp_var = make_node(TMP_DECL);
	TREE_CHAIN(tmp_var) = lval;	/* point to NET for pass3 */

	/* pass3 fills on the rest */
	stmt = build_stmt(ASSIGN_CONT_STMT, lineno, tmp_var, rval,
			  NULL_TREE, NULL_TREE, delay);
	TREE_CHAIN(stmt) = build_stmt(ASSIGN_CONT_STMT, lineno,
				      lval, NULL_TREE, NULL_TREE,
				      NULL_TREE, NULL_TREE);

	if (TREE_CODE(lval) == NET_SCALAR_DECL) {
	    NET_ASSIGNMENT(lval) = tmp_var;
	}

    } else
	stmt = build_stmt(ASSIGN_CONT_STMT, lineno, lval, rval,
			  NULL_TREE, NULL_TREE, delay);

    if (TREE_CODE(lval) == NET_SCALAR_DECL) {
	NET_ASSIGNMENT(lval) = stmt;
    }

    add_cont_assign_list(stmt);
    return stmt;
}



/************************************************************************
 *
 *	init_tree_1
 *		- initializes global and static variables for tree.c
 *
 ************************************************************************
 */

void init_tree_1()
{
    top_level = NULL_TREE;
    continuous_assignments = NULL_TREE;
    num_nodes = 0;
    num_nodes_g = 0;
    num_nodes_e = 0;
    num_nodes_b = 0;
    num_nodes_s = 0;
    num_nodes_r = 0;
    num_nodes_c = 0;
    num_nodes_d = 0;
    num_nodes_x = 0;
    free_link = NULL_TREE;
    memset(&permanent_obstack, 0, sizeof(permanent_obstack));
    memset(&module_obstack, 0, sizeof(module_obstack));
    memset(&linkage_obstack, 0, sizeof(linkage_obstack));
    memset(&code_obstack, 0, sizeof(code_obstack));
    memset(&temporary_obstack, 0, sizeof(temporary_obstack));
    memset(&saveable_obstack, 0, sizeof(saveable_obstack));
    memset(&current_obstack, 0, sizeof(current_obstack));
    memset(&expression_obstack, 0, sizeof(expression_obstack));
    memset(&temporary_firstobj, 0, sizeof(temporary_firstobj));
    memset(&hash_table, 0, sizeof(hash_table));
    do_identifier_warnings = 0;
    range_handle = NULL_TREE;
}
