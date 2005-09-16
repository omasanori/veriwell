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

/* sdflex.c	- lexer for sdf's */

#define	SDFCLEX_C

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include "vtypes.h"
#include "glue.h"
#include "tree.h"
#include "veriwell.h"
#include "acc_user.h"
#include "sdf.h"
#include "sdfcpars.h"
#include "sdfclex.h"
#include "veriuser.h"

typedef struct {
    char *string;
    int token;
} table_t;

static table_t keywordTable[] = {
    "iopath", IOPATH,
    "port", PORT,
    "interconnect", INTERCONNECT,
    "netdelay", NETDELAY,
    "device", DEVICE,
    "setup", SETUP,
    "hold", HOLD,
    "setuphold", SETUPHOLD,
    "recovery", RECOVERY,
    "skew", SKEW,
    "width", WIDTH,
    "period", PERIOD,
    "nochange", NOCHANGE,
    "pathpulse", PATHPULSE,
    "ignore", IGNORE,
    "intermod_path", INTERMOD_PATH,
    "mipd", MIPD,
    "cell", CELL,
    "use", USE,
    "interconnect_mipd", INTERCONNECT_MIPD,
    "minimum", MINIMUM,
    "maximum", MAXIMUM,
    "average", AVERAGE,
    "mtm", MTM,
    "tool_control", TOOL_CONTROL,
    "typical", TYPICAL,
    "scale_factors", SCALE_FACTORS,
    "from_minimum", FROM_MINIMUM,
    "from_typical", FROM_TYPICAL,
    "from_maximum", FROM_MAXIMUM,
    "from_mtm", FROM_MTM,
    "turnoff_delay", TURNOFF_DELAY,
    "turnon_delay", TURNON_DELAY,
    "module", MODULE,
    "map_inner", MAP_INNER,
    "add", ADD,
    "override", OVERRIDE,
    "ignore", IGNORE,
    "from_file", FROM_FILE,
    "scale_type", SCALE_TYPE,
    NULL, 0
};

static int lineNumber;
static FILE *fin;
static char filename[MAX_FILENAME_LENGTH];

static char *tokenBuffer;
static int bufferLength;
static char *bufferPtr;

static int hierarchyChar;

int sdfconfig_lex(YYSTYPE * val);

/*******************************************************************

	appendTokenBuffer
		-	add character to token buffer

*******************************************************************/

static void appendTokenBuffer(int c)
{
    ASSERT(tokenBuffer != NULL);
    if (bufferPtr >= tokenBuffer + bufferLength) {
	bufferLength += 40;
	tokenBuffer = (char *) xrealloc(tokenBuffer, bufferLength);
    }
    *bufferPtr = (char) c;
    bufferPtr += 1;
}

/*******************************************************************

	scanInteger
		- append characters to token buffer if they are part of
			an integer

 *******************************************************************/

static void scanInteger(void)
{
    int c;

    while (TRUE) {
	c = getc(fin);
	if (c < '0' || c > '9') {
	    break;
	}
	appendTokenBuffer(c);
    };
    ungetc(c, fin);
}

/*******************************************************************

	isStringEqual	
		- returns TRUE if strings are equal( case insensitive ) 

 ******************************************************************/

static int isStringEqual(char *s1, char *s2)
{
    while (*s1 != 0 && *s2 != 0) {
	if (toupper(*s1) != toupper(*s2)) {
	    return (FALSE);
	}
	s1++;
	s2++;
    }
    if (*s1 != 0 || *s2 != 0) {
	return (FALSE);
    }
    return (TRUE);
}

/*******************************************************************

	resetTokenBuffer
		- flush token buffer	

*******************************************************************/

static void resetTokenBuffer()
{
    bufferPtr = tokenBuffer;
}

/*******************************************************************

	sdflexPushFile
		- push current file descriptor and position

 *******************************************************************/

static void sdflexPushFile()
{
    if (fin != NULL) {
	ASSERT(FALSE);
    }
}

/*******************************************************************

	sdflexPopFile
		- pop current file descriptor and position

 *******************************************************************/

static void sdflexPopFile(void)
{
    if (fin != NULL) {
	shell_fclose(fin);
	fin = NULL;
    }
}

/*******************************************************************

	sdflexOpenFile
		- open an input file
 *******************************************************************/

FILE *sdfclexOpenFile(char *aFilename)
{
    FILE *newFile;

    sdflexPushFile();
    strncpy(filename, aFilename, sizeof(filename));
    newFile = shell_fopen(filename, "r");
    if (newFile == NULL) {
	tf_error("could not open file '%s'", filename);
	sdflexPopFile();
	return (NULL);
    }
    fin = newFile;
    lineNumber = 1;
    return (fin);
}

/*******************************************************************

	sdfconfig_error
		-	print error messages for sdf config parser

 *******************************************************************/

void sdfconfig_error(char *message)
{
    io_printf("SDF ERROR: %s L%ld, %s\n", filename, lineNumber, message);
}


/*******************************************************************
	match	
		- check for 2 character sequences 

********************************************************************/

static int match(int c, int noToken, char *matchString, int yesToken)
{
    char c1;


    c1 = getc(fin);
    if (c1 == matchString[1]) {
	appendTokenBuffer(c);
	appendTokenBuffer(c1);
	appendTokenBuffer(0);
	return (yesToken);
    } else {
	ungetc(c1, fin);
	if (c == matchString[0]) {
	    appendTokenBuffer(c);
	    appendTokenBuffer(0);
	}
	return (noToken);
    }
}

/*******************************************************************

	parseNumber
		-	parse all forms of numbers

 *******************************************************************/

static int parseNumber(int c, YYSTYPE * val)
{
    int type;

    if (c != '.') {
	appendTokenBuffer(c);
	if (c == '1') {
	    c = getc(fin);
	    if (c == '\'') {
		c = getc(fin);
		if (c == '0' || c == '1') {
		    appendTokenBuffer(c);
		    appendTokenBuffer(0);
		    val->integer = atoi(tokenBuffer);
		    return (SCALAR_CONSTANT);
		} else {
		    ungetc(c, fin);
		    sdfconfig_error("Illegal scalar constant");
		    val->integer = 0;
		    return (INTEGER);
		}
	    } else {
		ungetc(c, fin);
	    }
	}

	scanInteger();
	type = INTEGER;
	c = getc(fin);
    }

    if (c == '.') {
	appendTokenBuffer(c);
	scanInteger();
	type = FLOATING;
	c = getc(fin);
    }
    if (c == 'e' || c == 'E') {
	appendTokenBuffer(c);
	c = getc(fin);
	if (c == '-' || c == '+') {
	    appendTokenBuffer(c);
	} else {
	    ungetc(c, fin);
	}
	scanInteger();
	type = FLOATING;
    } else {
	ungetc(c, fin);
    }

    appendTokenBuffer(0);

    if (type == FLOATING) {
	val->floating = atof(tokenBuffer);
    } else {
	val->integer = atoi(tokenBuffer);
    }

    return (type);
}

/*******************************************************************

	parseIdentifier
		-	parse identifers and keywords 

 *******************************************************************/

static int parseIdentifier(int firstChar)
{
    int c;
    int escape;
    int i;


    c = firstChar;
    escape = FALSE;
    do {
	if (c != '\\') {
	    if (!escape && c == hierarchyChar) {
		appendTokenBuffer('.');
	    } else {
		appendTokenBuffer(c);
	    }
	    escape = FALSE;
	} else {
	    escape = TRUE;
	}
	c = getc(fin);
    }
    while (c == '_' ||
	   c == '\\' ||
	   (c >= 'a' && c <= 'z') ||
	   (c >= 'A' && c <= 'Z') ||
	   (c >= '0' && c <= '9') || c == hierarchyChar || escape);

    ungetc(c, fin);

    appendTokenBuffer(0);
    ASSERT(tokenBuffer != NULL);
    for (i = 0; keywordTable[i].string != NULL; i++) {
	if (isStringEqual(keywordTable[i].string, tokenBuffer)) {
	    return (keywordTable[i].token);
	}
    }
    return (IDENTIFIER);
}


/*******************************************************************

	sdfconfig_lex
		-	lexer for config parser

 *******************************************************************/

int sdfconfig_lex(YYSTYPE * val)
{
    int c;

    ASSERT(tokenBuffer != NULL);

    resetTokenBuffer();

    if (fin == NULL) {
	free(tokenBuffer);
	tokenBuffer = NULL;
	return (0);
    }

    while (TRUE) {
	c = getc(fin);
	switch (c) {
	case EOF:
	    sdflexPopFile();
	    if (fin == NULL) {
		return (EOF_);
	    }
	    break;
	case ' ':
	case '\r':
	case '\t':
	case '\b':
	case '\f':
	    break;
	case '\n':
	    lineNumber++;
	    break;
	case ')':
	case '(':
	case '+':
	case '-':
	case '|':
	case '*':
	case '/':
	case '%':
	case '[':
	case ']':
	case ':':
	case ';':
	case '{':
	case '}':
	case '\'':
	    appendTokenBuffer(c);
	    appendTokenBuffer(0);
	    return (c);
	case '"':
	    do {
		c = getc(fin);
		appendTokenBuffer(c);
	    }
	    while (c != '"' && c != '\n' && c != EOF);

	    if (c != '"') {
		sdfconfig_error("unterminated string constant");
		resetTokenBuffer();
		break;
	    }
	    return (QSTRING);

	case '&':
	    return match(c, c, "&&", ANDAND);
	case '<':
	    return match(c, match(0, LT, "<=", LE), "<<", LEFT_SHIFT);
	case '>':
	    return match(c, match(0, GT, ">=", GE), ">>", RIGHT_SHIFT);
	case '~':
	    return match(c, match(0, match(0, c,
					   "~&", REDUCTION_NAND),
				  "~|", REDUCTION_NOR), "~^", XNOR);
	case '^':
	    return match(c, c, "^~", XNOR);
	case '=':
	    c = getc(fin);
	    if (c != '=') {
		ungetc(c, fin);
		appendTokenBuffer('=');
		appendTokenBuffer(0);
		return ('=');
	    } else {
		return match(c, LOGICAL_EQUALITY, "==", CASE_EQUALITY);
	    }
	case '!':
	    c = getc(fin);
	    if (c != '=') {
		ungetc(c, fin);
		appendTokenBuffer('!');
		appendTokenBuffer(0);
		return ('!');
	    } else {
		return match(c, LOGICAL_INEQUALITY, "==", CASE_INEQUALITY);
	    }
	default:
	    if ((c >= '0' && c <= '9') || c == '.') {
		return (parseNumber(c, val));
	    } else if (c == '_' ||
		       c == '\\' ||
		       (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		return (parseIdentifier(c));
	    }
	    sdfconfig_error("illegal character");
	    resetTokenBuffer();
	    break;


	}
    }
}



/*******************************************************************

	sdfclexInit
		-	initialize lexer

 *******************************************************************/

void sdfclexInit()
{
    fin = NULL;
    bufferLength = 40;
    tokenBuffer = (char *) xmalloc(bufferLength);
    bufferPtr = tokenBuffer;
    hierarchyChar = (int) '.';
}
