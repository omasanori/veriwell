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

/* LEX.C - Read tokens for Verilog grammer */

#define LEX_C

#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "vtypes.h"
#include "tree.h"

#include "parse.h"
#include "macro.h"
#include "glue.h"
#include "lex.h"
#include "veriwell.h"
#include "timescal.h"
#include "exec.h"
#include "io.h"

#define follow(EXPECT, YES, NO)  \
  (nextchar == EXPECT) ? \
    (token_buffer[2] = '\0', nextchar = -1, YES) : \
    (token_buffer[1] = '\0', NO)

#define eatline(c) while (c != '\n' && c != EOF) c = fin->fgetc()

//extern  void * xmalloc();
//extern  void * xrealloc();

void yyerror();
void parse_base_const1(enum radii, char *, nbits_t, nbits_t, tree);
int yylex(YYSTYPE *);
lineno_t lineno;
lineno_t stmt_lineno;
lineno_t keywd_lineno;
static int maxtoken;		/* the current length of token buffer */
char *token_buffer;		/* the actual token buffer */
int enable_edge_digit;
int enable_udp_parse;


static int nextchar = -1;
int is_interactive = 0;

int second_eof = 0;		/* Send EOF twice: 1st as token, 2nd as 0 */

//  Bit     value, *v = &value;
int i;

File *fin__;			/* temp for debugging */


#include "v.h"

/* let syntax coloring tap into lexer */
int IsReservedWord(char *string, int len)
{
    struct keywords *ptr;
    char buffer[32];

    if (len > sizeof(buffer) - 1) {
	return (FALSE);
    }
    strncpy(buffer, string, len);
    buffer[len] = 0;
    ptr = (struct keywords *) Perfect_Hash::is_reserved_word(buffer, len);
    return (ptr != NULL);
}

/* Make the token buffer longer, preserving the data in it.
   P should point to just beyond the last valid character in the old buffer.
   The value we return is a pointer to the new buffer
   at a place corresponding to P.  */

void init_lex()
{
    lineno = 1;
    nextchar = -1;
    maxtoken = 40;
    token_buffer = (char *) xmalloc(maxtoken + 2);
    enable_edge_digit = FALSE;
    enable_udp_parse = FALSE;
}

void reinit_lex()
{
    nextchar = -1;
}

int convert_edge_digit(int c)
{
    ASSERT(X >= 0 && Z >= 0 && ZERO >= 0 && ONE >= 0);
    switch (c) {
    case 'x':
    case 'X':
	return (X);
    case '0':
	return (ZERO);
    case '1':
	return (ONE);
    }
    return (-1);
}

static char *extend_token_buffer(char *p)
{
    int offset = p - token_buffer;

    maxtoken = maxtoken * 2 + 10;
    token_buffer = (char *) xrealloc(token_buffer, maxtoken + 2);

    return token_buffer + offset;
}

void eatnum(void)
{
    char c;
    for (c = fin->fgetc(); isalnum(c); c = fin->fgetc());
    fin->fungetc(c);
}

static char *p;

/* Read a macro name for `define and `ifdef; report error if necessary */

static void read_macro_name()
{
    int c;
    c = fin->fgetc();
    p = token_buffer;
//    c = skip_white_space (getc (fin));

/* Read the rest of the line */

    for (; c == ' ' || c == '\t'; c = fin->fgetc());

    if (c == '\n') {
	error("Missing macro name in compiler directive", NULL_CHAR,
	      NULL_CHAR);
	nextchar = c;
    }

    if (isalnum(c) || c == '_' || c == '$') {
	do {
	    if (p >= token_buffer + maxtoken - 1)
		p = extend_token_buffer(p);
	    *p++ = c;
	    c = fin->fgetc();
	}
	while (isalnum(c) || c == '_' || c == '$');

	*p = 0;
	fin->fungetc(c);
    } else {
	yyerror("Illegal macro name");
	eatline(c);
	nextchar = c;
	return;
    }
}

static int if_nesting_level = 0;
//static int if_enable_level = 0;
static int if_disable_level = 0;
//static int if_enable = 1;

/* Return zero if lex should just return after processing macro
   (in most cases, lex shoudl read another token before going back to
    the parser)
*/

static int compiler_directive_or_macro(char *token)
{
    char c;
    tree macro_name_node;
    tree macro_string_node;
    YYSTYPE yyval;
    int depth;
    int commentCount;

    if (strcmp(token, "ifdef") == 0) {
	if_nesting_level++;
	if (if_disable_level) {
	    if_disable_level++;
	    return 1;
	}

	read_macro_name();

	if (!lookup_macro(token_buffer)) {
	    if_disable_level = 1;	/* doesn't exist, eat until `else or `endif */
	    for (yylex(&yyval); if_disable_level; yylex(&yyval));
	}
    }

    else if (strcmp(token, "endif") == 0) {
	if (!if_nesting_level) {
	    error("Unbalanced ifdef/endif pairs", NULL_CHAR, NULL_CHAR);
	    return 0;
	}

	--if_nesting_level;
	if (if_disable_level) {
	    --if_disable_level;
	    if (!if_disable_level)
		return 0;	/* Fprce lex to return */
	}
//    eatline(c);
//    nextchar = c;

    }

    else if (strcmp(token, "else") == 0) {
	if (!if_nesting_level)
	    error("`else not paired with `ifdef", NULL_CHAR, NULL_CHAR);
	/* currently enabled, now disable */
	else if (!if_disable_level) {
	    if_disable_level = 1;
	    for (yylex(&yyval); if_disable_level; yylex(&yyval));
	}
	/* currently disabled at one level; enable */
	else if (if_disable_level == 1) {
	    if_disable_level = 0;
	    return 0;
	}
//    eatline(c);
//    nextchar = c;
    } else if (if_disable_level)
	return 1;

/* `define: Process the macro directive */
    else if (strcmp(token, "undef") == 0) {
	read_macro_name();
	delete_macro(strlen(token_buffer), token_buffer);
    } else if (strcmp(token, "define") == 0) {
	read_macro_name();

	macro_name_node = build_string(strlen(token_buffer), token_buffer);

	p = token_buffer;
//      c = skip_white_space (c);

/* Read the rest of the line */

	for (c = fin->fgetc(); c == ' ' || c == '\t'; c = fin->fgetc());

//  elliot: defines containing other definitions don't work
//          this is my attempt to fix
//    for (*p = c; *p != '\n' && *p != EOF; *p = getc (fin))
//      if (p++ >= token_buffer + maxtoken - 1)
//          p = extend_token_buffer (p);

	depth = 0;
	commentCount = 0;
	for (*p = c; *p != '\n' && (depth != 0 || *p != EOF);
	     *p = fin->fgetc()) {
	    if (*p == EOF) {
		depth--;
		fin = pop_stream();
		continue;
	    } else if (commentCount == 2) {
		continue;
	    } else if (*p == '/') {
		commentCount++;
		if (commentCount == 2) {
		    p -= 2;
		}
	    } else if (commentCount == 1) {
		commentCount = 0;
	    }
	    if (*p == '`') {
		int definitionOffset = p - token_buffer; // amitrupu_fix: nested `define
		for (*p = fin->fgetc();
		     isalnum(*p) || *p == '_' || *p == '$';
		     *p = fin->fgetc()) {
		    if (p++ >= token_buffer + maxtoken - 1) {
			p = extend_token_buffer(p);
		    }
		}
		fin->fungetc(*p);
		*p = 0;
		char *definition = token_buffer + definitionOffset;
		expand_macro(definition);
/*fin__  = fin; */
		depth++;
		p = definition;
		continue;
	    }
	    if (p++ >= token_buffer + maxtoken - 1) {
		p = extend_token_buffer(p);
	    }
	}

	nextchar = *p;
	*p = 0;

	macro_string_node =
	    build_string(strlen(token_buffer), token_buffer);
	define_macro(macro_name_node, macro_string_node);


    }
    /* end if "define" */
    else if (strcmp(token, "include") == 0) {
	c = fin->fgetc();
	p = token_buffer;
	c = skip_white_space(fin->fgetc());

/* Read the rest of the line */

	if (c != '"') {
	    error("String expected after `include", NULL_CHAR, NULL_CHAR);
	    return 1;
	} else {
	    do {
		if (p >= token_buffer + maxtoken - 1)
		    p = extend_token_buffer(p);
		*p++ = c;
		c = fin->fgetc();
	    }
	    while (c != EOF && c != '\n' && c != '"');

	    *p = 0;
	    eatline(c);
	    lineno++;

	    open_include(token_buffer);

	}
    } /* end if "include" */
    else if (strcmp(token, "timescale") == 0) {
	if (!process_timescale())
	    eatline(c);
    }

    else if (strcmp(token, "accelerate") == 0
	     || strcmp(token, "expand_vectornets") == 0
	     || strcmp(token, "noremove_netnames") == 0
	     || strcmp(token, "resetall") == 0
	     || strcmp(token, "unconnected_drive") == 0
	     || strcmp(token, "celldefine") == 0
	     || strcmp(token, "endcelldefine") == 0
	     || strcmp(token, "noaccelerate") == 0
	     || strcmp(token, "nounconnected_drive") == 0
	     || strcmp(token, "protect") == 0
	     || strcmp(token, "default_nettype") == 0
	     || strcmp(token, "endprotect") == 0
	     || strcmp(token, "noexpand_vectornets") == 0
	     || strcmp(token, "remove_netnames") == 0
	     || strcmp(token, "timescale") == 0) {
	c = fin->fgetc();
	eatline(c);
	nextchar = c;
    }
/* It is not a compiler directive, must be a macro */
    else {
	expand_macro(token);
/*fin__ = fin;*/
    }


    return 1;
}

int read_num_token(char *p)
{
    int numchars;
    int c;

    c = skip_white_space(fin->fgetc());

    for (numchars = 0; isalnum(c) || c == '?' || c == '_';
	 c = fin->fgetc())
	if (c != '_') {
	    if (p >= token_buffer + maxtoken - 3)
		p = extend_token_buffer(p);
	    *p++ = c;
	    numchars++;
	}
//  --numchars;
    *p = '\0';
//  nextchar = c;
    fin->fungetc(c);
    nextchar = -1;

    return numchars;
}

static tree parse_base_const(nbits_t length)
{
    int c;
    char *p1;
    int offset;
    tree t;
    nbits_t numchars;
    enum radii base;

    HEAPTEST("Heap Corrupted (in Lex 2)");
    *(p + 1) = '\0';
    switch (c = *p++ = fin->fgetc()) {
    case 'b':
    case 'B':
	base = BIN;
	break;
    case 'h':
    case 'H':
	base = HEX;
	break;
    case 'o':
    case 'O':
	base = OCT;
	break;
    case 'd':
    case 'D':
	base = DEC;
	break;
    default:
	{
	    char c1[2];
	    c1[0] = c;
	    c1[1] = '\0';
	    error("Illegal radix ", (char *) c1, NULL_CHAR);
	    base = HEX;		/* read the number anyway */
	}
    }
    offset = p - token_buffer;	/* token_buffer may move if number is large */
    numchars = read_num_token(p);

//  p1 = p - 1;
    p1 = token_buffer + offset + numchars - 1;

    t = build_bit_cst(length, base);

    parse_base_const1(base, p1, length, numchars, t);
    HEAPTEST("Heap Corrupted (in Lex 1)");
    return t;
}

void
parse_base_const1(enum radii base, char *p1, nbits_t length,
		  nbits_t numchars, tree t)
{
    parse_base_const1g(base, p1, length, numchars, BIT_CST_GROUP(t));
}

void
parse_base_const1g(enum radii base, char *p1, nbits_t length,
		   nbits_t numchars, Group * g)
{
    int c, i, numerror, fillchar;
    Bit aval, bval, bit_mask;
    nbits_t j, numbits;
    int start_bit = 0;
    int numgroups = bits_to_groups(length) + 1;
    int overflow;
    int carry;

    numerror = 0;
    switch (base) {
    case BIN:
	if (numchars > length)
	    warning("Extra digits in binary number", NULL_CHAR, NULL_CHAR);
	numbits = min(numchars, length);	/* prevents reading too many chars */
	/* i + j is bit offset; j increments by 8, i wraps at 7 */
	for (j = 0; !numerror && j < length; j += sizeof(Bit) * 8) {
	    aval = 0;
	    bval = 0;
	    for (i = 0; !numerror && i < sizeof(Bit) * 8; i++, p1--) {
		if (i + j < numbits)	/* normal case; read each char in the buffer */
		    c = *p1;
		else if (i + j < length)	/* all chars read, fill to length */
		    c = fillchar;
		else		/* past length, stop doing */
		    break;

		switch (c) {
		case '0':
		    fillchar = '0';
		    break;
		case '1':
		    aval = aval + ((Bit) 1 << i);
		    fillchar = '0';
		    break;
		case 'x':
		case 'X':
		    aval = aval + ((Bit) 1 << i);
		    bval = bval + ((Bit) 1 << i);
		    fillchar = 'x';
		    break;
		case 'z':
		case 'Z':
		case '?':
		    bval = bval + ((Bit) 1 << i);
		    fillchar = 'z';
		    break;
		default:
		    yyerror("Illegal digit in binary number");
		    numerror = 1;
		}
	    }
	    HEAPTEST("Heap Corrupted (in Lex 1)");
	    AVAL(g) = aval;
	    BVAL(g) = bval;
	    HEAPTEST("Heap Corrupted (in Lex 1)");
	    g++;
	}
	break;

    case HEX:
	if ((numchars - 1) * 4 >= length)
	    warning("Extra digits in hexidecimal number", NULL_CHAR,
		    NULL_CHAR);
	numbits = min(numchars * 4, length);	/* prevents reading too many chars */
	/* i + j is bit offset; j increments by 8, i wraps at 7 */
	for (j = 0; !numerror && j < length; j += sizeof(Bit) * 8) {
	    if ((j + (sizeof(Bit) * 8)) > length)
		bit_mask = mask_right[length % (sizeof(Bit) * 8)];
	    else
		bit_mask = (Bit) - 1;
	    aval = 0;
	    bval = 0;
	    for (i = 0; !numerror && i < sizeof(Bit) * 8; i = i + 4, p1--) {
		if (i + j < numbits)	/* normal case; read each char in the buffer */
		    c = *p1;
		else if (i + j < length)	/* all chars read, fill to length */
		    c = fillchar;
		else		/* past length, stop doing */
		    break;

		switch (c) {
		case '0':
		    fillchar = '0';
		    break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		    aval = aval + ((c - '0' + (Bit) 0) << i);
		    fillchar = '0';
		    break;
		case 'a':
		case 'A':
		case 'b':
		case 'B':
		case 'c':
		case 'C':
		case 'd':
		case 'D':
		case 'e':
		case 'E':
		case 'f':
		case 'F':
		    aval = aval + ((toupper(c) - 'A' + (Bit) 10) << i);
		    fillchar = '0';
		    break;

		case 'x':
		case 'X':
		    aval = aval + ((Bit) 15 << i);
		    bval = bval + ((Bit) 15 << i);
		    fillchar = 'x';
		    break;
		case 'z':
		case 'Z':
		case '?':
		    bval = bval + ((Bit) 15 << i);
		    fillchar = 'z';
		    break;
		default:
		    yyerror("Illegal digit in hexidecimal number");
		    numerror = 1;
		}
	    }
	    AVAL(g) = aval & bit_mask;
	    BVAL(g) = bval & bit_mask;
	    g++;
	}
	break;

    case OCT:
	if ((numchars - 1) * 3 > length)
	    warning("Extra digits in octal number", NULL_CHAR, NULL_CHAR);
	numbits = min(numchars * 3, length);	/* prevents reading too many chars */
	start_bit = 0;
	/* i + j is bit offset; j increments by 8, i wraps at 7 */
	for (j = 0; !numerror && j < length; j += sizeof(Bit) * 8) {
	    aval = 0;
	    bval = 0;
	    for (i = start_bit; !numerror && i < (signed) sizeof(Bit) * 8;
		 i = i + 3, p1--) {
		if (i + j < numbits)	/* normal case; read each char in the buffer */
		    c = *p1;
		else if (i + j < length)	/* all chars read, fill to length */
		    c = fillchar;
		else		/* past length, stop doing */
		    break;

		switch (c) {
		case '0':
		    fillchar = '0';
		    break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		    if (i >= 0)
			aval |= (((Bit) c - '0' + (Bit) 0) << i);
		    else
			aval |= (((Bit) c - '0' + (Bit) 0) >> -i);
		    fillchar = '0';
		    break;

		case 'x':
		case 'X':
		    aval = aval + ((Bit) 7 << i);
		    bval = bval + ((Bit) 7 << i);
		    fillchar = 'x';
		    break;
		case 'z':
		case 'Z':
		case '?':
		    bval = bval + ((Bit) 7 << i);
		    fillchar = 'z';
		    break;
		default:
		    error("Illegal digit in octal number: '%c'",
			  (char *) c, NULL_CHAR);
		    numerror = 1;
		}
	    }
	    AVAL(g) = aval;
	    BVAL(g) = bval;
	    start_bit = i - BITS_IN_GROUP - 3;
	    if (start_bit > -3)
		p1++;
	    else
		start_bit = 0;
	    g++;
	}
	break;

    case DEC:
	/* 
	 * clear aval and bval
	 */
	for (i = 0; i < numgroups; ++i) {
	    AVAL(g + i) = 0;
	    BVAL(g + i) = 0;
	}
	overflow = 0;
	carry = 0;
	for (i = 0; i < numchars; ++i) {
	    char c = *p++;
	    unsigned carry = 0;
	    unsigned x2l;
	    unsigned x4l;
	    unsigned x2h = 0;
	    unsigned x4h = 0;
	    if (!isdigit(c)) {
		error("Illegal digit in decimal number", NULL_CHAR,
		      NULL_CHAR);
	    }
	    /*
	     * multiply aval by 10
	     */
	    for (j = 0; j < numgroups; ++j) {
		x2l = (AVAL(g + j) << 1) + x2h;
		x4l = (AVAL(g + j) << 3) + x4h;
		x2h = AVAL(g + j) >> (32 - 1);
		x4h = AVAL(g + j) >> (32 - 3);
		AVAL(g + j) = x2l + carry;
		if (AVAL(g + j) < carry) {
		    AVAL(g + j) = x4l;
		    continue;
		}
		AVAL(g + j) += x4l;
		carry = (AVAL(g + j) < x4l);
	    }
	    overflow = (carry != 0) || (x2h != 0) || (x4h != 0);
	    AVAL(g) += c - '0' + (Bit) 0;
	}
	if (length % (sizeof(Bit) * 8) == 0) {
	    bit_mask = ~0;
	} else {
	    bit_mask = mask_right[length % (sizeof(Bit) * 8)];
	}
	if (AVAL(g + numgroups - 1) & ~bit_mask) {
	    overflow = 1;
	}
	AVAL(g + numgroups - 1) = AVAL(g + numgroups - 1) & bit_mask;
	if (overflow) {
	    warning("Extra digits in decimal number", NULL_CHAR,
		    NULL_CHAR);
	}
	break;
    }
}

int yylex(YYSTYPE * lvalp)
{
    int c;
    int value;

  start_lex:
    HEAPTEST("Heap Corrupted (in Lex)");

    fin__ = fin;


    if (nextchar >= 0)
	c = nextchar, nextchar = -1;
    else {
	if (second_eof) {
	    second_eof = 0;
	    return 0;
	} else if (!fin)
	    c = EOF;

	else
	    c = fin->fgetc();
    }

    if (!c)
	c = EOF;		/* I hate to do this, but CodeWarrior returns
				   a 0 at the end of a macro forsome reason. */

    while (1) {
//    if (!fin)
//      c = EOF;    /* gets here if last line doesn't have newline */

	switch (c) {
	case ' ':		/* whitespace */
	case '\t':
	case '\f':
	case '\r':
	case '\v':
/*      case '\\': */
	case '\b':
	    c = fin->fgetc();
	    break;

	case '\n':		/* special whitespace */
	    c = skip_white_space(c);
	    break;

	case '/':		/* comments */
	    c = skip_white_space(c);

	default:
	    goto found_nonwhite;
	}
    }

  found_nonwhite:

    if (enable_edge_digit) {
	if (convert_edge_digit(c) >= 0) {
	    int c1;

	    c1 = fin->fgetc();
	    if (convert_edge_digit(c1) >= 0) {
		lvalp->double_digit[0] = convert_edge_digit(c);
		lvalp->double_digit[1] = convert_edge_digit(c1);
		return (DOUBLE_DIGIT);
	    }
	    fin->fungetc(c1);
	}
    }
    if (enable_udp_parse) {
	switch (c) {
	case '0':
	    lvalp->c = c;
	    return (UDP_ZERO);
	case '1':
	    lvalp->c = c;
	    return (UDP_ONE);
	case 'x':
	case 'X':
	    lvalp->c = 'x';
	    return (UDP_X);
	case '?':
	    lvalp->c = '?';
	    return (UDP_HOOK);
	case '-':
	case '*':
	case ':':
	    lvalp->c = c;
	    return (UDP_CHAR);
	case 'b':
	case 'B':
	    lvalp->c = 'b';
	    return (UDP_B);
	case 'r':
	case 'R':
	    lvalp->c = 'r';
	    return (UDP_CHAR);
	case 'f':
	case 'F':
	    lvalp->c = 'f';
	    return (UDP_CHAR);
	case 'p':
	case 'P':
	    lvalp->c = 'p';
	    return (UDP_CHAR);
	case 'n':
	case 'N':
	    lvalp->c = 'n';
	    return (UDP_CHAR);
	case 'E':
	case 'e':
	    enable_udp_parse = FALSE;
	    break;
	}
    }


    if (isdigit(c)) {		/* number */
	unsigned_32_t d = 0;
	short overflow = 0;
	short realnum = 0;
	short after_decimal = 0;

	p = token_buffer;
	realnum = 0;

	do {
	    if (p >= token_buffer + maxtoken - 3)
		p = extend_token_buffer(p);
	    *p++ = c;
	    if (c == '_');

	    else if (c == '.') {
		realnum = 1;
		if (after_decimal)
		    error
			("Too many decimal points in floating point constant",
			 NULL, NULL);
		else
		    after_decimal = 1;
	    } else {
		c = c - '0';
		if (d > d * 10)
		    overflow = 1;
		d *= 10;
		d += c;
//    if (d < 0)
//      overflow = 1;
	    }
	    c = fin->fgetc();
	}
	while (c >= '0' && c <= '9' || c == '.' || c == '_');

	if (c == 'e' || c == 'E') {
	    realnum = 1;
	    *p++ = c;
	    for (c = fin->fgetc(); isdigit(c) || c == '-';
		 c = fin->fgetc()) {
		if (p >= token_buffer + maxtoken - 3)
		    p = extend_token_buffer(p);
		*p++ = c;
	    }

	}

	/* See if this is a bit-constant (5'b1001...) */
	if (!realnum && (c = skip_white_space(c)) == '\'') {	/* single quote? */
	    if (d > MAX_BITS || overflow || d <= 0) {
		yyerror("Illegal length in constant");
		d = 0L;		/* but continue parsing */
	    }
	    *p++ = '\'';
	    lvalp->ttype = parse_base_const(d);
/*
	if (d == 1 && v->bval == 0)
	  if (v->aval == 0)
	    lvalp->ttype = bit_zero_node;
	  else
	    lvalp->ttype = bit_one_node;
	else
	  lvalp->ttype = build_nt (BIT_CST, d, v);
*/
	    return BASE_NUMBER;
	} else {		/* Must be an integer constant */
	    nextchar = c;
	    *p = 0;
	    if (overflow)
		yyerror("Decimal constant larger than 32 bits");

	    if (!realnum)
		lvalp->ttype = build_int_cst(d);
	    else
		lvalp->ttype = build_real_cst(token_buffer);

//      switch (d)
//  {
//  case 0: lvalp->ttype = integer_zero_node; break;
//  case 1: lvalp->ttype = integer_one_node; break;
//  default:
//     lvalp->ttype = build_int_cst (d);
//  }
	    return NUMBER;
	}
    }

    /* Bit-constant without length (make assumptions from value) */
    if (c == '\'') {
	unsigned_32_t d;

	p = token_buffer;
	*p++ = '\'';

	lvalp->ttype =
	    parse_base_const((unsigned_32_t) sizeof(unsigned_32_t) * 8);
/*      if (d == 1 && v->bval == 0)
        if (v->aval == 0)
          lvalp->ttype = bit_zero_node;
        else
          lvalp->ttype = bit_zero_node;
      else
        lvalp->ttype = build_nt (BIT_CST, d, v);
*/
	return BASE_NUMBER;
    }

    /* Identifier or keyword */
    if (isalpha(c) || c == '_' || c == '$' || c == '`') {

	p = token_buffer;
	value = IDENTIFIER;

	do {
	    if (p >= token_buffer + maxtoken - 1)
		p = extend_token_buffer(p);

	    if (c == '.')
		if (token_buffer[0] != '$' && token_buffer[0] != '`')
		    value = HIERARCHY_IDENTIFIER;
		else
		    break;	/* System tasks or compiler directive can't have periods */

	    *p++ = c;
	    c = fin->fgetc();


	    /* if we were in a macro, EOF is also end of macro.
	       pop the stream stack (i.e. the macro stack) and
	       read another character.  If this is truly an EOF,
	       the another will be read and the right thing will happen */

	    if (c == EOF) {
		fin = pop_stream();
//    c = getc (fin);
//    c = '\n';
//    c = ' ';  /* Any harmless seperator */
		if (fin)
		    c = fin->fgetc();
		else
		    c = EOF;	/* this is a real EOF */
	    }
	}
	while (isalnum(c) || c == '_' || c == '$' || c == '.');

	*p = 0;

	if (token_buffer[0] == '$')
	    value = SYSTEM_IDENTIFIER;

	else if (token_buffer[0] == '`') {
	    fin->fungetc(c);	/* don't save latest character */
	    nextchar = -1;
	    if (compiler_directive_or_macro(&token_buffer[1]))
		goto start_lex;	/* Read another token */
	    else
		return value;
	}
	nextchar = c;
	lvalp->ttype = NULL_TREE;

/* Is it a keyword? */

	{
	    register const struct keywords *ptr;

	    if ((ptr =
		 Perfect_Hash::is_reserved_word(token_buffer,
						p - token_buffer)) != 0) {
		lvalp->gtype = (enum tree_type) ptr->type;
		value = ptr->kval;
		stmt_lineno = lineno;	/* save line number for trace info */
	    }

	    /* Did not find a keyword, treat it as an identifier */
	    else
		lvalp->ttype = get_identifier(token_buffer);

	    return value;
	}
    }

    if (c == '\\') {		/* escaped identifier */
	p = token_buffer;
	for (;; c = fin->fgetc())
	    switch (c) {
	    case '\t':
	    case '\f':
	    case '\r':
	    case '\v':
	    case '\b':
	    case '\n':
	    case ' ':
		*p = 0;
		nextchar = c;
		lvalp->ttype = get_identifier(token_buffer);
		return IDENTIFIER;
	    default:
		if (p >= token_buffer + maxtoken - 1)
		    p = extend_token_buffer(p);
		*p++ = c;
	    }
    }

    if (c == '"') {		/* quoted string */
	char *p1;
	int len;
	tree t;
	ngroups_t ngroups;
	Group *g;
	int ii;

	for (p = token_buffer; (c = fin->fgetc()) != '"';) {

	    if (c == '\n' || c == EOF) {
		yyerror("Missing quote");
		nextchar = c;
		break;
	    }

	    if (c == '\\')
		switch (c = fin->fgetc()) {
		case 'n':
		    c = '\n';
		    break;
		case 't':
		    c = '\t';
		    break;
		case '\\':
		    c = '\\';
		    break;
		case '"':
		    c = '"';
		    break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		    {
			int c2, c3;
			c2 = fin->fgetc();
			if (c2 < '0' || c2 > '7') {
			    c = c - '0';
			    fin->fungetc(c2);
			    break;
			} else {
			    c3 = fin->fgetc();
			    if (c3 < '0' || c3 > '7') {
				c = (c - '0') * 8 + (c2 - '0');
				fin->fungetc(c3);
				break;
			    }
			}
			c = (c - '0') * 64 + (c2 - '0') * 8 + (c3 - '0');
			break;
		    }
		}

	    if (p >= token_buffer + maxtoken - 1)
		p = extend_token_buffer(p);

	    *p++ = c;
	}
	*p = 0;
	p1 = token_buffer;
	len = p - p1;
	if (len == 0)
	    len = 1;
//    lvalp->ttype = build_string (p - token_buffer + 1, token_buffer);

	t = build_bit_cst((nbits_t) (len * 8), STRING_);
	ngroups = chars_to_groups(len);
	g = BIT_CST_GROUP(t) + ngroups;

	i = ((len - 1) % sizeof(Bit)) + 1;	/* num chars at beginning */
	AVAL(g) = 0;
	for (ii = i - 1; ii >= 0; ii--)
#ifdef WORDS_BIGENDIAN		//Endianness problem
	    *(((char *) (&AVAL(g))) + sizeof(Bit) - 1 - ii) = *p1++;
#else
	    *(((char *) (&AVAL(g))) + ii) = *p1++;
#endif
	BVAL(g) = 0;
	--g;
	for (i = 1; i <= ngroups; i++, g--) {
	    for (ii = sizeof(Bit) - 1; ii >= 0; ii--)
#ifdef WORDS_BIGENDIAN		//Endianness problem
		*(((char *) (&AVAL(g))) + sizeof(Bit) - 1 - ii) = *p1++;
#else
		*(((char *) (&AVAL(g))) + ii) = *p1++;
#endif

//      memcpy (((char *)(&AVAL (g))), p1, sizeof (Bit));
	    BVAL(g) = 0;
	}
	lvalp->ttype = t;

/*    lvalp->ttype = build_string (/* p - * /token_buffer, token_buffer + 1); */
	return STRING;
    }
    token_buffer[0] = c;
    if (c != EOF)
	token_buffer[1] = nextchar = fin->fgetc();
    else
	nextchar = -1;

    switch (c) {
    case '*':
	return follow('>', STARARROW, '*');
    case '>':
	return follow('=', GE, follow('>', RIGHT_SHIFT, GT));
    case '<':
	return follow('=', LE, follow('<', LEFT_SHIFT, LT));
    case '=':
	if (nextchar == '=') {
	    nextchar = fin->fgetc();
	    return follow('=', CASE_EQUALITY, LOGICAL_EQUALITY);
	} else if (nextchar == '>') {
	    nextchar = -1;
	    token_buffer[2] = 0;
	    return EQUALARROW;
	} else {
	    token_buffer[1] = 0;
	    return '=';
	}
    case '!':
	if (nextchar == '=') {
	    nextchar = fin->fgetc();
	    return follow('=', CASE_INEQUALITY, LOGICAL_INEQUALITY);
	} else {
	    token_buffer[1] = 0;
	    return '!';
	}
    case '|':
	return follow('|', OROR, '|');
    case '&':
	if (nextchar == '&') {
	    nextchar = fin->fgetc();
	    return follow('&', ANDANDAND, ANDAND);
	} else {
	    token_buffer[1] = 0;
	    return '&';
	}

    case '-':
	return follow('>', ARROW_, '-');
    case '~':
	return follow('^', XNOR,
		      follow('&', REDUCTION_NAND,
			     follow('|', REDUCTION_NOR, '~')));
    case '^':
	return follow('~', XNOR, '^');

/* There are two EOFs so that "EOF" shows up in the list
   of possible tokens when there is a syntax error */

    case EOF:
	if (second_eof) {
	    second_eof = 0;
	    return 0;		/* send EOF as known by BISON */
	} else {

/* EOFs are generated at the end of a macro substitution.
   Pop the stream pointer, an continue.  If the stream stack
   is empty, then it was a real EOF */

	    if ((fin = pop_stream()) != 0)
		goto start_lex;	/* End of macro, read another char */

	    second_eof = 1;	/* next time around, do above */
	    return EOF_;	/* return tokenized version */
	}
    default:
	token_buffer[1] = '\0';
	if (isprint(c)) {
	    return c;
	} else {
	    warning("Unprintable character ignored: 0x%03x", (char *) c,
		    NULL_CHAR);
	    goto start_lex;
	}
    }
}

/*
  SKIP_WHITE_SPACE:
  This routine will help you parse through the more difficult "white space"
  characters including single & multi-line comments. This routine excepts a
  starting character & returns the first non-white_space character.
  */

int skip_white_space(int c)
{
    int c1;

    for (;;)
	switch (c) {
	case ' ':		/* whitespace */
	case '\t':
	case '\f':
	case '\r':
	case '\v':
/*      case '\\': */
	case '\b':
	    c = fin->fgetc();
	    break;

	case '\n':		/* special whitespace */
	    lineno++;
	    if (is_interactive)
		print_prompt();
	    c = fin->fgetc();
	    break;

	case EOF:
	    if ((fin = pop_stream()) == 0)
		return c;	/* bonafide EOF */
	    c = fin->fgetc();	/* Macro or `include EOF */
	    break;

	case '/':		/* comments */
	    c = fin->fgetc();
	    if (c == '/')	/* Handle "//" */
		eatline(c);

	    else if (c == '*') {	/* Handle "/*" */
		c1 = 0;
		do {
		    c = c1;
		    switch (c1 = fin->fgetc()) {
		    case '*':
			if (c == '/')
			    yyerror("/* found in comment");
			break;
		    case '\n':
			lineno++;
			if (is_interactive)
			    print_prompt();
			break;
		    case EOF:
			yyerror("unterminated comment block");
			return (0);
		    }
		}
		while (!(c == '*' && c1 == '/'));
		c = fin->fgetc();
	    } else {
		fin->fungetc(c);
		return '/';
	    }
	    break;

	default:
	    return c;
	}
}

static int backslash(int c)
{				/* get next char with /'s interpreted */
//  char *strchr(); /* `strchr()' in some systems */
    static char transtab[] = "b\bf\fn\nr\rt\t";
    if (c != '\\')
	return c;
    c = fin->fgetc();
    if (islower(c) && strchr(transtab, c))
	return strchr(transtab, c)[1];
    return c;
}

/* follow(expect, ifyes, ifno)	/* look ahead for >=, etc. */
/*{
	int c = getc(fin);
	token[1] = c;
	token[2] = '\0';
	if (c == expect)
		return ifyes;
	ungetc(c, fin);
	token[1] = '\0';
	return ifno;
} */

/* Report syntax errors */

void yyerror(char *s)
{
//  char buf[200];

//  strcpy (buf, s);
//  strcat (buf, " before '%s'");
//  error (s, token_buffer);
    error(s, NULL_CHAR, NULL_CHAR);
}

void fix_lastchar(void)
{
    fin->fungetc(nextchar);
    nextchar = -1;
}

/*******************************************************************
 *
 *		init_lex_1
 *			-	initialize global and static variables for lex.c
 *
 *******************************************************************
 */

void init_lex_1()
{
    nextchar = -1;
    is_interactive = 0;
    second_eof = 0;		/* Send EOF twice: 1st as token, 2nd as 0 */
    if_nesting_level = 0;
    if_disable_level = 0;
    memset(&lineno, 0, sizeof(lineno));
    memset(&stmt_lineno, 0, sizeof(stmt_lineno));
    memset(&keywd_lineno, 0, sizeof(keywd_lineno));
    maxtoken = 0;
    token_buffer = NULL;
    i = 0;
}
