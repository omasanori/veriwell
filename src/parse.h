/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LOWEST = 258,
     IF = 259,
     ELSE = 260,
     OROR = 261,
     ANDAND = 262,
     ANDANDAND = 263,
     XNOR = 264,
     CASE_INEQUALITY = 265,
     CASE_EQUALITY = 266,
     LOGICAL_INEQUALITY = 267,
     LOGICAL_EQUALITY = 268,
     GE = 269,
     GT = 270,
     LE = 271,
     LT = 272,
     RIGHT_SHIFT = 273,
     LEFT_SHIFT = 274,
     REDUCTION_NOR = 275,
     REDUCTION_NAND = 276,
     UNARY = 277,
     IDENTIFIER = 278,
     HIERARCHY_IDENTIFIER = 279,
     STARARROW = 280,
     EQUALARROW = 281,
     SPECPARAM = 282,
     SETUP = 283,
     HOLD = 284,
     PERIOD = 285,
     WIDTH = 286,
     SKEW = 287,
     RECOVERY = 288,
     SETUPHOLD = 289,
     NOCHANGE = 290,
     MODULE = 291,
     MACROMODULE = 292,
     ENDMODULE = 293,
     SPECIFY = 294,
     ENDSPECIFY = 295,
     PRIMITIVE = 296,
     ENDPRIMITIVE = 297,
     TABLE = 298,
     ENDTABLE = 299,
     TASK = 300,
     ENDTASK = 301,
     FUNCTION = 302,
     ENDFUNCTION = 303,
     PARAMETER = 304,
     INPUT = 305,
     OUTPUT = 306,
     INOUT = 307,
     NETTYPE = 308,
     TRIREG = 309,
     SCALARED = 310,
     VECTORED = 311,
     REG = 312,
     TIME = 313,
     INTEGER = 314,
     REAL = 315,
     EVENT = 316,
     ASSIGN = 317,
     DEFPARAM = 318,
     STRENGTH0 = 319,
     STRENGTH1 = 320,
     SUPPLY0 = 321,
     SUPPLY1 = 322,
     LOGIC = 323,
     BUFFER = 324,
     BUFIF = 325,
     CMOS = 326,
     PULL = 327,
     TRAN = 328,
     INITIAL = 329,
     ALWAYS = 330,
     CASE = 331,
     CASEZ = 332,
     CASEX = 333,
     ENDCASE = 334,
     FOREVER = 335,
     REPEAT = 336,
     WHILE = 337,
     FOR = 338,
     WAIT_ = 339,
     ARROW_ = 340,
     DISABLE = 341,
     DEASSIGN = 342,
     FORCE = 343,
     RELEASE = 344,
     DEFAULT = 345,
     BEGIN = 346,
     END = 347,
     FORK = 348,
     JOIN = 349,
     SYSTEM_IDENTIFIER = 350,
     STRING = 351,
     NUMBER = 352,
     BASE_NUMBER = 353,
     POSEDGE = 354,
     NEGEDGE = 355,
     OR = 356,
     SMALL = 357,
     MEDIUM = 358,
     LARGE = 359,
     EDGE = 360,
     DOUBLE_DIGIT = 361,
     UDP_CHAR = 362,
     UDP_ONE = 363,
     UDP_ZERO = 364,
     UDP_X = 365,
     UDP_B = 366,
     UDP_HOOK = 367,
     EOF_ = 368
   };
#endif
#define LOWEST 258
#define IF 259
#define ELSE 260
#define OROR 261
#define ANDAND 262
#define ANDANDAND 263
#define XNOR 264
#define CASE_INEQUALITY 265
#define CASE_EQUALITY 266
#define LOGICAL_INEQUALITY 267
#define LOGICAL_EQUALITY 268
#define GE 269
#define GT 270
#define LE 271
#define LT 272
#define RIGHT_SHIFT 273
#define LEFT_SHIFT 274
#define REDUCTION_NOR 275
#define REDUCTION_NAND 276
#define UNARY 277
#define IDENTIFIER 278
#define HIERARCHY_IDENTIFIER 279
#define STARARROW 280
#define EQUALARROW 281
#define SPECPARAM 282
#define SETUP 283
#define HOLD 284
#define PERIOD 285
#define WIDTH 286
#define SKEW 287
#define RECOVERY 288
#define SETUPHOLD 289
#define NOCHANGE 290
#define MODULE 291
#define MACROMODULE 292
#define ENDMODULE 293
#define SPECIFY 294
#define ENDSPECIFY 295
#define PRIMITIVE 296
#define ENDPRIMITIVE 297
#define TABLE 298
#define ENDTABLE 299
#define TASK 300
#define ENDTASK 301
#define FUNCTION 302
#define ENDFUNCTION 303
#define PARAMETER 304
#define INPUT 305
#define OUTPUT 306
#define INOUT 307
#define NETTYPE 308
#define TRIREG 309
#define SCALARED 310
#define VECTORED 311
#define REG 312
#define TIME 313
#define INTEGER 314
#define REAL 315
#define EVENT 316
#define ASSIGN 317
#define DEFPARAM 318
#define STRENGTH0 319
#define STRENGTH1 320
#define SUPPLY0 321
#define SUPPLY1 322
#define LOGIC 323
#define BUFFER 324
#define BUFIF 325
#define CMOS 326
#define PULL 327
#define TRAN 328
#define INITIAL 329
#define ALWAYS 330
#define CASE 331
#define CASEZ 332
#define CASEX 333
#define ENDCASE 334
#define FOREVER 335
#define REPEAT 336
#define WHILE 337
#define FOR 338
#define WAIT_ 339
#define ARROW_ 340
#define DISABLE 341
#define DEASSIGN 342
#define FORCE 343
#define RELEASE 344
#define DEFAULT 345
#define BEGIN 346
#define END 347
#define FORK 348
#define JOIN 349
#define SYSTEM_IDENTIFIER 350
#define STRING 351
#define NUMBER 352
#define BASE_NUMBER 353
#define POSEDGE 354
#define NEGEDGE 355
#define OR 356
#define SMALL 357
#define MEDIUM 358
#define LARGE 359
#define EDGE 360
#define DOUBLE_DIGIT 361
#define UDP_CHAR 362
#define UDP_ONE 363
#define UDP_ZERO 364
#define UDP_X 365
#define UDP_B 366
#define UDP_HOOK 367
#define EOF_ 368




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 133 "parse.yy"
typedef union YYSTYPE {
	tree	ttype;	/* pointer to a tree node */
	int	integer;	/* used for integers */
//	inst	ctype;	/* pointer to an instruction node (code) */
//	Datum	*dtype; /* pointer to a bit string for returning NUMBERs */
	lineno_t ltype;	/* for passing line numbers */
	enum tree_code	code;	/* for operators */
	enum tree_type	gtype;	/* for gates */
	enum tree_type	ntype;	/* for nets (same as gtype, buy more readable) */
	char	double_digit[2]; /* structure for parsing edges */
	char	c;		/* char used in udp parsing */
} YYSTYPE;
/* Line 1318 of yacc.c.  */
#line 276 "parse.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





