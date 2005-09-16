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

/* PRINT.C - Print numbers, text, statements to screen, files, log */
#define PRINT_C

#include <stdio.h>
#include <stdlib.h>
#include "vtypes.h"
#include "tree.h"
#include "io.h"
#include "print.h"
#include "glue.h"
#include "runtime.h"
#include "veriwell.h"

static int print_indent;
char indent_spaces[100] = "";
static char *doubler = NULL;
static unsigned int dd = 0;
static char *acc = NULL;
static unsigned int aa = 0;

static char *print_buf;
static int print_buf_size;

char *set_print_buf(unsigned_32_t size)
{
    if (size > print_buf_size) {
	if (!print_buf)
	    print_buf = (char *) xmalloc(size);
	else
	    print_buf = (char *) xrealloc(print_buf, size);
	print_buf_size = size;
	if (!print_buf) {
	    error("Out of memory while displaying a constant", NULL, NULL);
	}
    }
    return print_buf;
}

void
print_bcd(handle_t handle, Group * val, nbits_t nbits, int integer_attr,
	  int pad)
{
    print_bcd_(val, nbits, integer_attr, pad);
    fprintf_V(handle, "%s", print_buf);
}

static void print_char(handle_t handle, int c, int fill)
{
    if (!c) {
	if (fill)
	    fprintf_V(handle, " ");
    } else if (c == '\n')
	fprintf_V(handle, "\\n");
    else if (c == ('\"'))
	fprintf_V(handle, "\\\"");
    else if (c == '\\')
	fprintf_V(handle, "\\\\");
    else if (c == '\t')
	fprintf_V(handle, "\\t");
    else if (c < ' ' || c >= 127)	/* 177 octal */
	fprintf_V(handle, "\\%.3o", c);
    else
	fprintf_V(handle, "%c", c);
}

static void
print_string(handle_t handle, Group * g_base, nbits_t nbits, int fill)
{
    ngroups_t i, ngroups = bits_to_groups(nbits);
    int num_chars, starting_char_offset, j, c;
    Group *g;

//  fprintf_V (handle, "\"");
    num_chars = nbits / 8;
    starting_char_offset = (num_chars - 1) % sizeof(Bit) + 1;
    g = g_base + ngroups;
    for (j = starting_char_offset - 1; j >= 0; j--) {
	c = (AVAL(g) >> j * 8) & 255;
	print_char(handle, c, fill);
    }
    --g;
    for (i = 1; i <= ngroups; i++, --g)
	for (j = sizeof(Bit) - 1; j >= 0; j--) {
	    c = (AVAL(g) >> j * 8) & 255;
	    print_char(handle, c, fill);
	}
//  fprintf_V (handle, "\"");
}

/* Print a decimal number.  If all bits are X, print "x".  If some bits
   are X, print "X".  If all bits are Z, print "z".  If some bits are
   Z, print "Z".  Else print the number.
*/

void
print_dec(handle_t handle, Group * g, nbits_t nbits, int int_attr, int pad)
{
    Group *g1 = g;
    ngroups_t i, ngroups = bits_to_groups(nbits);
    Bit tmp, mask = mask_right1[nbits & NBITS_MASK];
    enum { START, NO_XZ, SOME_Z, ALL_Z, SOME_X, ALL_X } state;

    state = START;
    for (i = 0; i < ngroups; i++, g1++) {
	tmp = AVAL(g1) & BVAL(g1);	/* Look for Xs */
	if (tmp == -1)		/* All Xs */
	    state = (state == START || state == ALL_X) ? ALL_X : SOME_X;
	else if (tmp || state == ALL_X) {	/* Some Xs */
	    state = SOME_X;
	    break;		/* SOME_X has highest precedence */
	} else {		/* No Xs, look for Zs */

	    tmp = ~AVAL(g1) & BVAL(g1);	/* Look for Zs */
	    if (tmp == -1)	/* All Zs */
		state = (state == START
			 || state == ALL_Z) ? ALL_Z : SOME_Z;
	    else if (tmp || state == ALL_Z)	/* Some Zs */
		state = SOME_Z;
	    else
		state = NO_XZ;
	}
    }

    switch (state) {
    case START:
	tmp = AVAL(g1) & BVAL(g1) & mask;	/* mask, fill w/1s */
	if (tmp == mask)
	    fprintf_V(handle, "x");
	else if (tmp)
	    fprintf_V(handle, "X");
	else if ((tmp = ~AVAL(g1) & BVAL(g1) & mask) == mask)
	    fprintf_V(handle, "z");
	else if (tmp)
	    fprintf_V(handle, "Z");
	else
	    print_bcd(handle, g, nbits, int_attr, pad);
	break;
    case NO_XZ:
	print_bcd(handle, g, nbits, int_attr, pad);
	break;
    case SOME_X:
	fprintf_V(handle, "X");
	break;
    case SOME_Z:
	fprintf_V(handle, "Z");
	break;
    case ALL_X:
	if ((AVAL(g1) & BVAL(g1) & mask) == mask)
	    fprintf_V(handle, "x");
	else
	    fprintf_V(handle, "X");
	break;
    case ALL_Z:
	if ((~AVAL(g1) & BVAL(g1) & mask) == mask)
	    fprintf_V(handle, "z");
	else if (AVAL(g1) & BVAL(g1) & mask)
	    fprintf_V(handle, "X");
	else
	    fprintf_V(handle, "Z");
	break;
    }
}

static enum print_state
print_group(Bit aval, Bit bval, int size, enum print_state state, int last,
	    enum radii radix, int *octal_shift,
	    Bit * residue_aval, Bit * residue_bval)
{
    int i;
    Bit temp1, temp2;
    Bit digit_mask, size_mask, radix_mask;
    int decrement;
    char *pb = print_buf;
    int any_xs;

    switch (radix) {
    case (HEX):
	radix_mask = 15;
	size_mask = (Bit) - 4;
	decrement = 4;
	break;

    case (OCT):
	radix_mask = 7;
	size_mask = (Bit) - 1;
	decrement = 3;
	break;

    case (BIN):
	radix_mask = 1;
	size_mask = (Bit) - 1;
	decrement = 1;
	break;
    }

    aval = aval & mask_right[size];
    bval = bval & mask_right[size];

    /* if radix is octal, digits may be unaligned.  Print residue from previous
       group, if applicable, and start from the right place.
     */
    if (radix == OCT) {
	/* Note: this formula works for 32-bit groups only!! */
#if NBITS_MASK != 0x1f
#error Octal only supports 32 bits
#endif
	int unaligned = size % 3 - *octal_shift;

	/* Carry forward bits from prev group */
	if (size == BITS_IN_GROUP && unaligned > 0) {
	    i = 32 - unaligned;
	    temp1 = (aval >> i) | (*residue_aval << unaligned);
	    temp2 = (bval >> i) | (*residue_bval << unaligned);
	    goto oct_continue;	/* yeah, yeah; clean this up later */
	} else {
	    i = (size - 1) - ((size - 1) % 3) + *octal_shift;
	    if (i == 32)
		i -= 3;
	}
    } else
	i = (size - 1) & size_mask;

    for (; i >= 0; i = i - decrement) {
	temp1 = (aval >> i) & radix_mask;
	temp2 = (bval >> i) & radix_mask;
      oct_continue:
	digit_mask = (mask_right[size] >> i) & radix_mask;

	if (!temp2)		/* Zero or one */
	    switch (state) {
	    case START:
	    case EXTEND_0:
		if (temp1) {	/* not zero */
		    state = EXTEND_DONE;
		    sprintf(pb++, "%1x", temp1);
		} else {	/* zero */

		    state = EXTEND_0;
		    *pb = '\0';
		}
		break;

	    case EXTEND_X:
		sprintf(pb, "x%1x", temp1);
		pb += 2;
		state = EXTEND_DONE;
		break;

	    case EXTEND_Z:
		sprintf(pb, "z%1x", temp1);
		pb += 2;
		state = EXTEND_DONE;
		break;

	    case EXTEND_DONE:
		sprintf(pb++, "%1x", temp1);
		break;
	    }

	else if ((any_xs = temp1 & temp2) != 0)	/* One or more X's */
	    switch (state) {
	    case START:
		state = EXTEND_X;
	    case EXTEND_X:
		*pb = '\0';
		break;

	    case EXTEND_Z:
		if (any_xs == digit_mask)
		    sprintf(pb, "zx");
		else
		    sprintf(pb, "zX");
		pb += 2;
		state = EXTEND_DONE;
		break;

	    case EXTEND_0:
		if (any_xs == digit_mask)
		    sprintf(pb, "0x");
		else
		    sprintf(pb, "0X");
		pb += 2;
		state = EXTEND_DONE;
		break;

	    case EXTEND_DONE:
		if ((any_xs & digit_mask) == digit_mask)
		    sprintf(pb++, "x");
		else
		    sprintf(pb++, "X");
		break;
	    }

	else			/* All Z's */
	    switch (state) {
	    case START:
		state = EXTEND_Z;
	    case EXTEND_Z:
		*pb = '\0';
		break;

	    case EXTEND_X:
		if (temp1)
		    sprintf(pb, "xZ");
		else
		    sprintf(pb, "xz");
		pb += 2;
		state = EXTEND_DONE;
		break;

	    case EXTEND_0:
		if (temp1)
		    sprintf(pb, "0Z");
		else
		    sprintf(pb, "0z");
		pb += 2;
		state = EXTEND_DONE;
		break;

	    case EXTEND_DONE:
		if (temp1)
		    sprintf(pb++, "Z");
		else
		    sprintf(pb++, "z");
		break;
	    }

    }

    /* Nothing was printed? */
    if (last && state != EXTEND_DONE) {
	if (state == EXTEND_0)
	    sprintf(pb++, "0");
	else if (state == EXTEND_X)
	    sprintf(pb++, "x");
	else
	    sprintf(pb++, "z");
    }

    if (radix == OCT) {
	if (i < 0) {
	    *residue_aval = aval & mask_right[*octal_shift];
	    *residue_bval = bval & mask_right[*octal_shift];
	    *octal_shift = (*octal_shift + 2) % 3;
	} else {
	    *octal_shift = 0;
	    *residue_aval = 0;
	    *residue_bval = 0;
	}
    }
    return state;
}

/* Print a binary number to a "c"-style FILE handle (e.g. for dump files) */

void print_binary_file(FILE * file, Group * g, nbits_t nbits)
{
    ngroups_t i, ngroups = bits_to_groups(nbits);
    int octal_shift;		/* These aren't relavent */
    Bit r_aval, r_bval;		/* ... */
    enum print_state state;

    set_print_buf(sizeof(Bit) * 8 + 2);	/* One for \0 and one for just in case */

    if (!ngroups) {		/* sizeof (Bit) is in bytes */
	print_group(AVAL(g), BVAL(g), (int) nbits, START, 1, BIN,
		    &octal_shift, &r_aval, &r_bval);
	fprintf(file, "%s", print_buf);
    } else {
	g = g + ngroups;
	state = print_group(AVAL(g), BVAL(g),
			    (int) ((nbits - 1) % (sizeof(Bit) * 8) + 1),
			    START, 0, BIN, &octal_shift, &r_aval, &r_bval);
	fprintf(file, "%s", print_buf);
	g--;
	for (i = ngroups - 1; i > 0; i--, g--) {
	    state = print_group(AVAL(g), BVAL(g), sizeof(Bit) * 8,
				state, 0, BIN, &octal_shift, &r_aval,
				&r_bval);
	    fprintf(file, "%s", print_buf);
	}
	state = print_group(AVAL(g), BVAL(g), sizeof(Bit) * 8,
			    state, 1, BIN, &octal_shift, &r_aval, &r_bval);
	fprintf(file, "%s", print_buf);
    }
}

/* print number pointed to by 'g' in given radix.  If "print_nbits" is
   supplied, print in the form of "32h'xxxx", else leave out radix
   info and pad with spaces.
*/

void
print_datum_file(handle_t handle, Group * g, nbits_t print_nbits,
		 nbits_t nbits, enum radii radix, int is_integer,
		 int fill_space, char *spec)
{
    ngroups_t i;
    ngroups_t ngroups = bits_to_groups(nbits);
    int octal_shift;
    Bit r_aval = 0, r_bval = 0;
    enum print_state state;

    set_print_buf(sizeof(Bit) * 8 + 3);

    switch (radix) {
    case (HEX):
	if (print_nbits)
	    printf_V("%d'h", print_nbits);
	break;
    case (OCT):
	if (print_nbits)
	    printf_V("%d'o", print_nbits);
	octal_shift = ngroups % 3;
//    residue = residue ? 3 - residue : 0;
	r_aval = r_bval = 0;
	break;
    case (BIN):
	if (print_nbits)
	    printf_V("%d'b", print_nbits);
	break;
    case (DEC):
	print_bcd(handle, g, nbits, is_integer, fill_space);
	return;
//    print_dec (handle, g, nbits, is_integer, fill_space);
//    return;
    case STRING_:
	print_string(handle, g, nbits, fill_space);
	return;
    case CHAR_:
	printf_V("%c", AVAL(g));
	return;
    case EXP_:
    case FLOAT_:
    case EXPFLOAT_:
	printf_V(spec, REAL_(g));
	return;
    case TIME_:
//  timeformatprint (REAL_ (g));    /* "%t", this is a timeformat */
	return;
    }

//      if (fill_space)
//        fill = ' ';
//      else
//      {
    if (print_nbits || !fill_space)
	state = START;		/* space fill */
    else
	state = EXTEND_DONE;	/* zero fill */
//      }

    if (!ngroups) {		/* sizeof (Bit) is in bytes */
	print_group(AVAL(g), BVAL(g), (int) nbits, state, 1, radix,
		    &octal_shift, &r_aval, &r_bval);
	fprintf_V(handle, "%s", print_buf);
    } else {
	g = g + ngroups;
	state = print_group(AVAL(g), BVAL(g),
			    (int) ((nbits - 1) % (sizeof(Bit) * 8) + 1),
			    state, 0, radix, &octal_shift, &r_aval,
			    &r_bval);
	fprintf_V(handle, "%s", print_buf);
	g--;
	for (i = ngroups - 1; i > 0; i--, g--) {
	    state = print_group(AVAL(g), BVAL(g), sizeof(Bit) * 8,
				state, 0, radix, &octal_shift, &r_aval,
				&r_bval);
	    fprintf_V(handle, "%s", print_buf);
	}
	state = print_group(AVAL(g), BVAL(g), sizeof(Bit) * 8,
			    state, 1, radix, &octal_shift, &r_aval,
			    &r_bval);
	fprintf_V(handle, "%s", print_buf);
    }
}

void
print_datum(Group * g, nbits_t print_nbits, nbits_t nbits,
	    enum radii radix, int is_integer, int fill_space)
{
    print_datum_file((handle_t) 1, g, print_nbits, nbits, radix,
		     is_integer, fill_space, NULL);
}

void print_common(tree node)
{
    printf_error_V("%s%s @ %p; chain: %p; len: %i; type: %u\n",
		   indent_spaces, tree_code_name[TREE_CODE(node)], node,
		   TREE_CHAIN(node), tree_code_length[TREE_CODE(node)],
		   TREE_TYPE(node));
}

static void print_block_common(tree node)
{
    printf("%sdecl: %p; ports: %p; params: %p; up: %p; down: %p\n",
	   indent_spaces, BLOCK_DECL(node), BLOCK_PORTS(node),
	   BLOCK_PARAM(node), BLOCK_UP(node), BLOCK_DOWN(node));
}

static void print_decl_common(tree node)
{
    printf("%s%s[]; context: %p\n", indent_spaces,
	   IDENTIFIER_POINTER(DECL_NAME(node)), DECL_CONTEXT(node));
}

void print_stats(void)
{
    extern unsigned long mem;
    extern int num_nodes;
    extern int num_nodes_g;
    extern int num_nodes_e;
    extern int num_nodes_b;
    extern int num_nodes_s;
    extern int num_nodes_r;
    extern int num_nodes_c;
    extern int num_nodes_d;
    extern int num_nodes_x;
    extern int big_label;
    extern int max_label;
    extern ngroups_t stack_size;


    extern int serial;



    printf_V("%s\nVersion %s\n\n", VERIWELL, VERSION);
    printf_V("\n");
    printf_V("Number of objects: %u\n", num_nodes);
    printf_V("  Primitive objects: %u\n", num_nodes_g);
    printf_V("  Expression objects: %u\n", num_nodes_e);
    printf_V("  Block objects: %u\n", num_nodes_b);
    printf_V("  Statement objects: %u\n", num_nodes_s);
    printf_V("  Reference objects: %u\n", num_nodes_r);
    printf_V("  Constant objects: %u\n", num_nodes_c);
    printf_V("  Decl objects: %u\n", num_nodes_d);
    printf_V("  Misc objects: %u\n", num_nodes_x);
    printf_V("Largest Expression: %u\n", big_label);
    printf_V("Stack size: %u\n", stack_size);
    printf_V("Stack objects: %u\n", max_label);
}

static void print_args(tree node)
{
    tree t;

    for (t = node; t; t = TREE_CHAIN(t)) {
	if (TREE_EXPR(t))
	    print_expr(TREE_EXPR(t));
	if (TREE_CHAIN(t))
	    printf_V(", ");
    }
}

static void print_ref(tree node)
{
    enum tree_code code = TREE_CODE(node);
    tree t;

    switch (code) {
    case (IDENTIFIER_NODE):
	printf_V("%s", IDENTIFIER_POINTER(node));
	break;
    case (TREE_LIST):
	printf_V("{");
	for (t = TREE_VALUE(node); t; t = TREE_CHAIN(t))
	    print_ref(t);
	printf_V("}");
	break;
    case ARRAY_REF:
	if (HIERARCHICAL_ATTR(node))
	    printf_V("%s", IDENT(ARRAY_REF_NAME(node)));
	else
	    printf_V("%s", IDENT(DECL_NAME(ARRAY_REF_DECL(node))));
	printf_V("[");
	print_expr(TREE_OPERAND(node, 1));
	printf_V("]");
	break;
    case (BIT_REF):
	if (HIERARCHICAL_ATTR(node))
	    printf_V("%s", IDENT(BIT_REF_NAME(node)));
	else
	    printf_V("%s", IDENT(DECL_NAME(BIT_REF_DECL(node))));
	printf_V("[");
	print_expr(BIT_EXPR(node));
	printf_V("]");
	break;
    case CONCAT_REP_REF:
	printf_V("{");
	print_expr(CONCAT_EXPR(node));
	printf_V(" ");
    case CONCAT_REF:
	printf_V("{");
	CONCAT_LIST(node) = nreverse(CONCAT_LIST(node));
	for (t = CONCAT_LIST(node); t; t = TREE_CHAIN(t)) {
	    print_expr(TREE_PURPOSE(t));
	    if (TREE_CHAIN(t))
		printf_V(", ");
	}
	CONCAT_LIST(node) = nreverse(CONCAT_LIST(node));
	printf_V("}");
	if (code == CONCAT_REP_REF)
	    printf_V("}");
	break;
    case (PART_REF):
	if (HIERARCHICAL_ATTR(node))
	    printf_V("%s", IDENT(PART_NAME(node)));
	else
	    printf_V("%s", IDENT(DECL_NAME(PART_DECL(node))));
	printf_V("[");
	print_expr(PART_MSB_(node));
	printf_V(": ");
	print_expr(PART_LSB_(node));
	printf_V("]");
	break;
    case SHADOW_REF:
	print_expr(TREE_CHAIN(node));
	break;
    case SHADOW_LVAL_REF:
	printf_V("%s", IDENT(REF_NAME(node)));
	break;
    case SYSFUNCTION_REF:
    case FUNCTION_REF:
	printf_V("%s", FUNC_REF_NAME(node));
	printf_V(" (");
	print_args(FUNC_REF_ARGS(node));
	printf_V(")");
	break;

    default:
	printf_V("Unimplemeted reference in trace: %s",
		 tree_code_name[TREE_CODE(node)]);
	break;
    }
}

void print_delay(tree node)
{
    int i;

    if (!node)
	return;

    printf_V("#");
    if (TREE_LABEL(node) != 1 ||
	*tree_code_type[TREE_CODE(TREE_OPERAND(node, 0))] != 'd' &&
	*tree_code_type[TREE_CODE(TREE_OPERAND(node, 0))] != 'c')
	printf_V("(");

    for (i = 0;;) {
	print_expr(TREE_OPERAND(node, i));
	if (++i < TREE_LABEL(node))
	    printf_V(", ");
	else
	    break;
    }

    if (TREE_LABEL(node) != 1 ||
	*tree_code_type[TREE_CODE(TREE_OPERAND(node, 0))] != 'd' &&
	*tree_code_type[TREE_CODE(TREE_OPERAND(node, 0))] != 'c')
	printf_V(")");
    printf_V(" ");
}

static void print_delay_stmt(tree node)
{
    printf_V("#");
    if (*tree_code_type[TREE_CODE(STMT_DELAY_EXPR(node))] == 'e')
	printf_V("(");
    print_expr(STMT_DELAY_EXPR(node));
    if (*tree_code_type[TREE_CODE(STMT_DELAY_EXPR(node))] == 'e')
	printf_V(")");
}

static void print_event_stmt(tree node)
{
    tree t;

    printf_V("@");

    /* @ identifier */
//  if (TREE_CODE (TREE_EXPR (STMT_EVENT_EXPR_LIST (node))) == ANYEDGE_EXPR)
    if (!TREE_CHAIN(STMT_EVENT_EXPR_LIST(node)) &&
	TREE_CODE(TREE_EXPR(STMT_EVENT_EXPR_LIST(node))) == ANYEDGE_EXPR) {
	tree op = TREE_OPERAND(TREE_EXPR(STMT_EVENT_EXPR_LIST(node)), 0);
	if (*tree_code_type[TREE_CODE(op)]
	    == 'd' && !TREE_CHAIN(STMT_EVENT_EXPR_LIST(node)))
	    print_expr(TREE_EXPR(STMT_EVENT_EXPR_LIST(node)));
    }
    /* @(expression) */
    else {
	printf_V("(");
	for (t = STMT_EVENT_EXPR_LIST(node); t; t = TREE_CHAIN(t)) {
	    print_expr(TREE_EXPR(t));
	    if (TREE_CHAIN(t))
		printf_V(" OR ");
	}
	printf_V(")");
    }
}

void print_assignment(tree node)
{
    print_expr(STMT_ASSIGN_LVAL(node));
    printf_V(" = ");

//  if (TREE_CODE (node) != ASSIGN_STMT)
//    print_delay (STMT_ASSIGN_CONTROL (node));
    print_expr(STMT_ASSIGN_RVAL(node));
}

void print_expr(tree node)
{
    enum tree_code code = TREE_CODE(node);
    tree right_op;
    char *type = tree_code_type[TREE_CODE(node)];

    switch (*type) {
    case ('d'):
	printf_V("%s", IDENTIFIER_POINTER(DECL_NAME(node)));
	break;

    case ('c'):
	if (code == INTEGER_CST) {
//        signed_32_t i;
//        HIGH_INT (i) = INT_CST_HIGH (node);
//        LOW_INT (i) = INT_CST_LOW (node);
	    printf_V("%ld", INT_CST_DATA(node));
	} else if (code == REAL_CST) {
//        signed_32_t i;
//        HIGH_INT (i) = INT_CST_HIGH (node);
//        LOW_INT (i) = INT_CST_LOW (node);
	    printf_V("%lf", REAL_CST_DATA(node));
	} else if (BIT_CST_RADIX(node) == STRING_) {
	    fprintf_V((handle_t) 1, "\"");
	    print_string((handle_t) 1, BIT_CST_GROUP(node),
			 BIT_CST_NBITS(node), 0);
	    fprintf_V((handle_t) 1, "\"");
//        print_datum (BIT_CST_GROUP (node), R_nbits, BIT_CST_NBITS (node), BIT_CST_RADIX (node));
	} else
	    print_datum(BIT_CST_GROUP(node), BIT_CST_NBITS(node),
			BIT_CST_NBITS(node), BIT_CST_RADIX(node), 0, 0);
	break;

    case ('e'):
	switch (*++type) {
	case 'x':
	case '1':
	    print_expr(TREE_OPERAND(node, 0));
	    right_op = TREE_OPERAND(node, 1);
	    printf_V(" %s ", tree_code_name[code]);
	    break;
	case 'u':
	case 'r':
	    right_op = TREE_OPERAND(node, 0);
	    if (code != ANYEDGE_EXPR)
		printf_V("%s", tree_code_name[code]);
	    break;
	case 's':
	    print_expr(SHIFT_OPERAND(node));
	    right_op = NULL_TREE;
	    printf_V(" %s ", tree_code_name[code]);
	    print_expr(SHIFT_COUNT(node));
	    break;
	case '3':
	    printf_V("(");
	    print_expr(COND_EXPR(node));
	    printf_V(" ? ");
	    print_expr(COND_TRUE(node));
	    printf_V(" : ");
	    print_expr(COND_FALSE(node));
	    printf_V(")");
	    break;
	case 'c':
	    right_op = NULL_TREE;
	    if (code == NOP_EXPR) {
		print_expr(TREE_OPERAND(node, 0));
		break;
	    }
	    printf_V(" %s ", tree_code_name[code]);
	    printf_V("(");
	    print_expr(TREE_OPERAND(node, 0));
	    printf_V(")");
	    break;
	default:
	    printf_V("Expression type not implemented in trace\n");
	}
	if ((*type != '3') && right_op) {
	    if (*tree_code_type[TREE_CODE(right_op)] == 'e'
		&& tree_code_length[TREE_CODE(right_op)] == 2) {
		printf_V("(");
		print_expr(right_op);
		printf_V(")");
	    } else
		print_expr(right_op);
	}
	break;

    case ('r'):
	print_ref(node);
	break;

    case 'x':
	if (code == IDENTIFIER_NODE)
	    printf_V("%s", IDENTIFIER_POINTER(node));
	else
	    printf_V("Unimplemeted reference in Trace: %s",
		     tree_code_name[TREE_CODE(node)]);
	break;


    default:
	printf_V("Unimplemeted reference in Trace: %s",
		 tree_code_name[TREE_CODE(node)]);
	break;
    }
}

int print_node(tree node)
{
    enum tree_code code = TREE_CODE(node);

    switch (code) {
    case ALWAYS_BLOCK:
	printf_V("ALWAYS");
	break;

    case INITIAL_BLOCK:
	printf_V("INITIAL");
	break;

    case BEGIN_STMT:
	printf_V("BEGIN");
	break;

    case END_STMT:
	printf_V("END");
	break;

    case BEGIN_NAMED_STMT:
	printf_V("BEGIN : %s", IDENT(BLOCK_NAME(STMT_BLOCK(node))));
	break;

    case END_NAMED_STMT:
	printf_V("END : %s", IDENT(BLOCK_NAME(STMT_BLOCK(node))));
	break;

    case FORK_STMT:
	printf_V("FORK");
	break;

    case JOIN_STMT:
	printf_V("JOIN");
	break;

    case FOREVER_STMT:
	printf_V("FOREVER");
	break;

    case WHILE_STMT:
	printf_V("WHILE (");
	print_expr(STMT_WHILE_COND(node));
	printf_V(")");
	break;

    case (IF_STMT):
	printf_V("IF (");
	print_expr(STMT_COND(node));
	printf_V(")");
	break;

    case REPEAT_STMT:
    case REPEAT_INIT_STMT:
	printf_V("REPEAT (");
	print_expr(STMT_REPEAT_EXPR(node));
	printf_V(")");
	break;

    case CASE_STMT:
	printf_V("CASE (");
	goto case_common;
    case CASEX_STMT:
	printf_V("CASEX (");
	goto case_common;
    case CASEZ_STMT:
	printf_V("CASEZ (");
      case_common:
	print_expr(STMT_CASE_EXPR(node));
	printf_V(")");
	break;

    case ASSIGN_PROC_STMT:
	printf_V("ASSIGN ");
	goto assign_common;

    case FORCE_STMT:
	printf_V("FORCE ");
	goto assign_common;

    case ASSIGN_STMT:
      assign_common:
	print_assignment(node);
	printf_V(";");
	break;

    case ASSIGN_DELAY_STMT:
    case ASSIGN_EVENT_STMT:
	print_expr(STMT_ASSIGN_LVAL(node));
	printf_V(" = ");
	if (code == ASSIGN_DELAY_STMT)
	    print_delay_stmt(STMT_ASSIGN_DELAY(node));
	else if (code == ASSIGN_EVENT_STMT)
	    print_event_stmt(STMT_ASSIGN_EVENT(node));
	printf_V(" ");
	print_expr(STMT_ASSIGN_RVAL(node));
	printf_V(";");
	break;

    case ASSIGN_NONBLK_DELAY_STMT:
    case ASSIGN_NONBLK_EVENT_STMT:
	print_expr(STMT_ASSIGN_LVAL(node));
	printf_V(" <= ");
	if (code == ASSIGN_NONBLK_DELAY_STMT)
	    print_delay_stmt(STMT_ASSIGN_DELAY(node));
	else if (code == ASSIGN_NONBLK_EVENT_STMT)
	    print_event_stmt(STMT_ASSIGN_EVENT(node));
	printf_V(" ");
	print_expr(STMT_ASSIGN_RVAL(node));
	printf_V(";");
	break;

    case (FOR_STMT):
	printf_V("For (");
	if (!STMT_SURROGATE_ATTR(node))
	    print_assignment(STMT_FOR_ASSIGN(node));
	printf_V("; ");
	print_expr(STMT_FOR_COND(node));
	printf_V(";");
	if (STMT_SURROGATE_ATTR(node)) {
	    printf_V(" ");
	    print_assignment(STMT_FOR_ASSIGN(node));
	}
	break;

    case DELAY_STMT:
	print_delay_stmt(node);
	break;

    case WAIT_STMT:
	printf_V("WAIT ");
	print_expr(STMT_WAIT_EXPR(node));
	break;

    case EVENT_STMT:
	print_event_stmt(node);
	break;

    case ARROW_STMT:
	printf_V("-> ");
	print_expr(STMT_BODY(node));
	break;

    case TASK_STMT:
    case SYSTASK_STMT:
	printf_V("%s", STMT_TASK_NAME(node));
	if (STMT_TASK_ARGS(node)) {
	    printf_V(" (");
	    print_args(STMT_TASK_ARGS(node));
	    printf_V(")");
	}
	break;

/*
    case TASK_BLOCK:
    case FUNCTION_BLOCK:
      printf_V ("%s; >>> RETURNING\n", IDENT (BLOCK_NAME (node)));
      break;
*/

    case FUNCTION_REF:
	printf_V("%s", FUNC_REF_NAME(node));
	printf_V(" (");
	print_args(FUNC_REF_ARGS(node));
	printf_V(")");
	break;

    case DISABLE_STMT:
	printf_V("DISABLE %s",
		 IDENTIFIER_POINTER(STMT_DISABLE_NAME(node)));
	break;

    case DEASSIGN_STMT:
	printf_V("DEASSIGN %s", IDENT(DECL_NAME(STMT_BODY(node))));
	break;

    case RELEASE_STMT:
	printf_V("RELEASE %s;", IDENT(DECL_NAME(STMT_BODY(node))));
	break;

    case NULL_STMT:
	printf_V(";");
	break;

    default:
	printf_V("Statement not known to print_node: %s\n",
		 tree_code_name[code]);
	break;
    }
    return (0);
}

static unsigned int bcd_add(char a[], char b[], unsigned int ndigits)
{
    unsigned i;
    char aa, carry = 0;

    HEAPTEST("bcd_add 1");
    for (i = 0; i < ndigits; i++) {
	aa = a[i] + b[i] + carry;
	if (aa > 9) {
	    carry = 1;
	    aa -= 10;
	} else
	    carry = 0;

	a[i] = aa;
    }
    HEAPTEST("bcd_add 2");
    if (carry) {
	a[i] = 1;
	HEAPTEST("bcd_add 3");
	return ndigits + 1;
    } else
	return ndigits;
}

static unsigned int bcd_inc(char a[], unsigned int ndigits)
{
    unsigned i;
    char aa, carry = 1;

    for (i = 0; i < ndigits; i++) {
	aa = a[i] + carry;
	if (aa > 9) {
	    carry = 1;
	    aa -= 10;
	} else
	    carry = 0;

	a[i] = aa;
    }
    if (carry) {
	a[i] = 1;
	HEAPTEST("bcd_inc 3");
	return ndigits + 1;
    } else
	return ndigits;
}

#define ACCINC 5

static void
print_bcd_(Group * val, nbits_t nbits, int integer_attr, int pad)
{
    Group *g;
    ngroups_t ngroups = bits_to_groups(nbits), i;
    int j;
//  int xflag = 0, zflag = 0;

    char *acc_tmp;
    char *doubler_tmp;
    char *pb;			/* print buffer pointer */
    unsigned est_digits, res_digits;
    Bit aval, bval, negative, mask, x = 0, z = 0;
    int lastbit, xx = 0, XX = 1, zz = 0, ZZ = 1;

/* guestimate the number of digits needed to support nbits and make sure
   there is enough buffer space to handle the conversion */

    est_digits = (nbits / 3) + 1;	/* Make sure there is at least one */
    if (est_digits > dd) {
	if (!doubler)
	    doubler_tmp = (char *) xmalloc(100);
	else
	    doubler_tmp = (char *) xrealloc(doubler, 1000);
	if (!doubler_tmp) {
	    printf_V("*No memory for decimal conversion*");
	    return;
	} else {
	    doubler = doubler_tmp;
	    dd = est_digits;
	}
    }

/* initialize */

    for (i = 0; i < aa; i++)
	acc[i] = 0;
    for (i = 0; i < dd; i++)
	doubler[i] = 0;

    HEAPTEST("print_bcd 1");
    doubler[0] = 1;
    res_digits = 1;
    if (integer_attr
	&& ((AVAL(val + ngroups) >> ((nbits - 1) % BITS_IN_GROUP)) & 1) ==
	1)
	negative = (Bit) - 1;
    else
	negative = (Bit) 0;

    for (i = 0, g = val; i <= ngroups; i++, g++) {	/* # groups */
	if (i == ngroups) {
	    lastbit = (nbits - 1) % BITS_IN_GROUP + 1;
	    mask = mask_right[lastbit];
	} else {
	    lastbit = sizeof(Bit) * 8;
	    mask = (Bit) - 1;
	}
	aval = (AVAL(g) ^ negative) & mask;
	bval = BVAL(g) & mask;
	x |= aval & bval;
	z |= ~aval & bval;
//    x |= AVAL (g) & BVAL (g);
//    z |= ~AVAL (g) & BVAL (g);
	XX &= !~z;
	ZZ &= !~z;
	xx |= x;
	zz |= z;
	for (j = 0; j < lastbit; j++) {	/* bits/group */
	    if ((aval >> j) & 1) {
		if (aa <= res_digits) {
		    HEAPTEST("print_bcd 1.5");
		    if (acc)
			acc_tmp =
			    (char *) xrealloc(acc,
						   (size_t) (res_digits +
							     ACCINC));
		    else
			acc_tmp =
			    (char *)
			    xmalloc((size_t) (res_digits + ACCINC));
		    HEAPTEST("print_bcd 1.6");
		    if (!acc_tmp) {
			printf_V
			    ("*Not enough memory for decimal conversion*");
			return;
		    } else {
			acc = acc_tmp;
			{
			    int i;
			    HEAPTEST("print_bcd 1.7");
			    for (i = aa; i < res_digits + ACCINC; i++)
				acc_tmp[i] = 0;
			    HEAPTEST("print_bcd 2");
			}
			aa = res_digits + ACCINC;
		    }
		}
		bcd_add(acc, doubler, res_digits);
		HEAPTEST("print_bcd 3");
	    }
	    res_digits = bcd_add(doubler, doubler, res_digits);
	    HEAPTEST("print_bcd 4");
	}
    }
    /* print number first skipping leading zeros */
    if (!aa) {
	acc_tmp = (char *) xmalloc(1);
	if (!acc_tmp) {
	    printf_V("*Not enough memory for decimal conversion*");
	    return;
	} else {
	    acc = acc_tmp;
	    aa = 1;
	    acc[0] = 0;
	}
    }
    HEAPTEST("print_bcd 5");
    if (negative) {
//    fprintf_V (handle, "-");
//    bcd_inc (acc, res_digits);
	bcd_inc(acc, aa);
    }
    HEAPTEST("print_bcd 6");
    pb = set_print_buf(res_digits + 1);

    if (xx || zz) {
	for (i = 0; i < aa; i++)
	    acc[i] = 0;
	negative = 0;
    }


    if (pad) {			/* print spaces */
	/* Fill with spaces up to the size of the accumulator (aa), then start
	   looking at the digits.  If the number is negative, then the "-" needs
	   to be printed before the first non-zero digit, so there needs to be a
	   look ahead.
	 */

//    int still_zero = 1;

	i = res_digits - 1;
	/* aa (size of the accumulator) may be smaller than the reserved size
	   (the maximum size for the given number of bits).  Fill with spaces
	   until the space before acc starts, in case a negative sign is needed.
	 */

	for (; i >= aa; i--)
	    sprintf(pb++, " ");

	/* Now scan acc until a non-zero is found */
	for (; i > 0; i--) {
	    if (acc[i] == 0)
		sprintf(pb++, " ");
	    else
//      {
//  still_zero = 0;
		break;
//      }
	}

	/* Either we are just before the last digit, or we found a non-zero,
	   or both.
	 */

//    if (still_zero)   /* Implies 0 */
//      fprintf_V (handle, "%d", acc[i]);

//    else      /* Found a non-zero */
	{
	    if (integer_attr) {
		if (negative)
		    sprintf(pb++, "-");	/* last leading zero */
		else
		    sprintf(pb++, " ");
	    }
	    /* Print the res of the digits */
	    if (XX)
		sprintf(pb++, "X");
	    else if (ZZ)
		sprintf(pb++, "Z");
	    else if (xx)
		sprintf(pb++, "x");
	    else if (zz)
		sprintf(pb++, "z");
	    else
		for (; i >= 0; i--)
		    sprintf(pb++, "%d", acc[i]);
	}
    } else {
	i = aa;
	do {
	    if (acc[--i] != 0)
		break;
	}
	while (i > 0);
//  if (i)
	if (negative)
	    sprintf(pb++, "-");
	for (; i >= 0; i--)
	    sprintf(pb++, "%d", acc[i]);
    }

//else
//  printf ("0");
}

void print_scope(handle_t handle, tree scope)
{
    if (BLOCK_UP(scope)) {
	print_scope(handle, BLOCK_UP(scope));
	fprintf_V(handle, ".");
    }
    fprintf_V(handle, "%s", IDENT(BLOCK_NAME(scope)));
}

/* These two routines print the raw simulation time.  See timescale.c
   for routines that print times based on the current timescales. */

void print_time(time64 * time_p)
{
    Group v[2];
    AVAL(&v[0]) = time_p->timel;
    AVAL(&v[1]) = time_p->timeh;
    BVAL(&v[0]) = BVAL(&v[1]) = 0;
    print_bcd((handle_t) 1, v, (nbits_t) 64, 0, 0);
}

char *time_string(time64 * time_p)
{
    Group v[2];
    AVAL(&v[0]) = time_p->timel;
    AVAL(&v[1]) = time_p->timeh;
    BVAL(&v[0]) = BVAL(&v[1]) = 0;
    print_bcd_(v, (nbits_t) 64, 0, 0);
    return print_buf;
}

char *sprint_decimal(Group * val, nbits_t nbits)
{
    print_bcd_(val, nbits, 0, 0);
    return (print_buf);
}

char *sprint_hex(Group * val, nbits_t nbits)
{
    int i;
    int j;
    Group *g;
    int bitCount;
    char *result;
    int digit;
    int abit;
    int bbit;
    char *ptr;
    int xCount;
    int zCount;
    int count;
    char c;

    g = val;
    bitCount = 0;

    ptr = set_print_buf((nbits + 3) / 4 + 1);
    ptr += (nbits + 3) / 4;
    *ptr-- = 0;

    for (i = 0; i <= R_ngroups; i++, g++) {
	for (j = 0; j < BITS_IN_GROUP; j++) {
	    if ((bitCount % 4) == 0) {
		digit = 0;
		xCount = 0;
		zCount = 0;
		count = 0;
	    }
	    abit = (AVAL(g) >> j) & 0x1;
	    bbit = (BVAL(g) >> j) & 0x1;
	    digit |= abit << (bitCount % 4);
	    if (bbit == 1) {
		if (abit == 1) {
		    xCount++;
		} else {
		    zCount++;
		}
	    }
	    count++;
	    bitCount++;
	    if (bitCount == nbits || bitCount % 4 == 0) {
		if (xCount == count) {
		    // x
		    c = 'x';
		} else if (zCount == count) {
		    // z 
		    c = 'z';
		} else if (xCount == 0 && zCount == 0) {
		    // number
		    if (digit <= 9) {
			c = '0' + digit;
		    } else {
			c = 'A' + digit - 10;
		    }
		} else if (xCount == 0) {
		    // Z
		    c = 'Z';
		} else {
		    // X
		    c = 'X';
		}
		*ptr-- = c;
	    }
	    if (bitCount == nbits) {
		return (print_buf);
	    }
	}
    }
    while (ptr >= print_buf) {
	*ptr-- = '0';
    }
    return (print_buf);
}


char *sprint_binary(Group * val, nbits_t nbits)
{
    int i;
    int j;
    Group *g;
    int bitCount;
    char *result;
    int abit;
    int bbit;
    char *ptr;
    char c;

    g = val;
    bitCount = 0;

    ptr = set_print_buf(nbits + 1);
    ptr += nbits;
    *ptr-- = 0;

    for (i = 0; i <= R_ngroups; i++, g++) {
	for (j = 0; j < BITS_IN_GROUP; j++) {
	    abit = (AVAL(g) >> j) & 0x1;
	    bbit = (BVAL(g) >> j) & 0x1;
	    if (bbit == 1) {
		if (abit == 1) {
		    *ptr-- = 'x';
		} else {
		    *ptr-- = 'z';
		}
	    } else {
		if (abit == 1) {
		    *ptr-- = '1';
		} else {
		    *ptr-- = '0';
		}
	    }
	    bitCount++;
	    if (bitCount == nbits) {
		return (print_buf);
	    }
	}
    }
    while (ptr >= print_buf) {
	*ptr-- = '0';
    }
    return (print_buf);
}

/**************************************************************
 *	init_print
 *		- initialize global and static variables for print.c
 *
 **************************************************************
 */

void init_print()
{
    indent_spaces[0] = 0;
    print_indent = 0;
    doubler = NULL;
    dd = 0;
    acc = NULL;
    aa = 0;
    print_buf_size = 0;
    print_buf = NULL;
}
