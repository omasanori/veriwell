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
     IF = 258,
     ELSE = 259,
     OROR = 260,
     ANDAND = 261,
     ANDANDAND = 262,
     XNOR = 263,
     CASE_INEQUALITY = 264,
     CASE_EQUALITY = 265,
     LOGICAL_INEQUALITY = 266,
     LOGICAL_EQUALITY = 267,
     GE = 268,
     GT = 269,
     LE = 270,
     LT = 271,
     RIGHT_SHIFT = 272,
     LEFT_SHIFT = 273,
     REDUCTION_NOR = 274,
     REDUCTION_NAND = 275,
     UNARY = 276,
     INTEGER = 277,
     SCALAR_CONSTANT = 278,
     FLOATING = 279,
     QSTRING = 280,
     IDENTIFIER = 281,
     BLOB = 282,
     POSEDGE = 283,
     NEGEDGE = 284,
     DELAYFILE = 285,
     SDFVERSION = 286,
     DESIGN = 287,
     DATE = 288,
     VENDOR = 289,
     PROGRAM = 290,
     VERSION_ = 291,
     DIVIDER = 292,
     VOLTAGE = 293,
     PROCESS = 294,
     TEMPERATURE = 295,
     TIMESCALE = 296,
     CELL = 297,
     CELLTYPE = 298,
     INSTANCE = 299,
     DELAY = 300,
     TIMINGCHECK = 301,
     INCLUDE = 302,
     ABSOLUTE = 303,
     INCREMENT = 304,
     USERDEF = 305,
     PATHPULSE = 306,
     GLOBALPATHPULSE = 307,
     IOPATH = 308,
     COND = 309,
     PORT = 310,
     INTERCONNECT = 311,
     NETDELAY = 312,
     DEVICE = 313,
     SETUP = 314,
     HOLD = 315,
     SETUPHOLD = 316,
     RECOVERY = 317,
     SKEW = 318,
     WIDTH = 319,
     PERIOD = 320,
     NOCHANGE = 321,
     PATHCONSTRAINT = 322,
     SUM = 323,
     DIFF = 324,
     SKEWCONSTRAINT = 325,
     MICROSECOND = 326,
     NANOSECOND = 327,
     PICOSECOND = 328,
     EDGE_01 = 329,
     EDGE_10 = 330,
     EDGE_0z = 331,
     EDGE_1z = 332,
     EDGE_z0 = 333,
     EDGE_z1 = 334,
     EOF_ = 335
   };
#endif
#define IF 258
#define ELSE 259
#define OROR 260
#define ANDAND 261
#define ANDANDAND 262
#define XNOR 263
#define CASE_INEQUALITY 264
#define CASE_EQUALITY 265
#define LOGICAL_INEQUALITY 266
#define LOGICAL_EQUALITY 267
#define GE 268
#define GT 269
#define LE 270
#define LT 271
#define RIGHT_SHIFT 272
#define LEFT_SHIFT 273
#define REDUCTION_NOR 274
#define REDUCTION_NAND 275
#define UNARY 276
#define INTEGER 277
#define SCALAR_CONSTANT 278
#define FLOATING 279
#define QSTRING 280
#define IDENTIFIER 281
#define BLOB 282
#define POSEDGE 283
#define NEGEDGE 284
#define DELAYFILE 285
#define SDFVERSION 286
#define DESIGN 287
#define DATE 288
#define VENDOR 289
#define PROGRAM 290
#define VERSION_ 291
#define DIVIDER 292
#define VOLTAGE 293
#define PROCESS 294
#define TEMPERATURE 295
#define TIMESCALE 296
#define CELL 297
#define CELLTYPE 298
#define INSTANCE 299
#define DELAY 300
#define TIMINGCHECK 301
#define INCLUDE 302
#define ABSOLUTE 303
#define INCREMENT 304
#define USERDEF 305
#define PATHPULSE 306
#define GLOBALPATHPULSE 307
#define IOPATH 308
#define COND 309
#define PORT 310
#define INTERCONNECT 311
#define NETDELAY 312
#define DEVICE 313
#define SETUP 314
#define HOLD 315
#define SETUPHOLD 316
#define RECOVERY 317
#define SKEW 318
#define WIDTH 319
#define PERIOD 320
#define NOCHANGE 321
#define PATHCONSTRAINT 322
#define SUM 323
#define DIFF 324
#define SKEWCONSTRAINT 325
#define MICROSECOND 326
#define NANOSECOND 327
#define PICOSECOND 328
#define EDGE_01 329
#define EDGE_10 330
#define EDGE_0z 331
#define EDGE_1z 332
#define EDGE_z0 333
#define EDGE_z1 334
#define EOF_ 335




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 93 "sdfparse.yy"
typedef union YYSTYPE {
	int				integer;
	double			floating;
	char			*ptr;
	eScaleType_t	etype;
	eDelayType_t  	dtype;
	eSDFConstruct_t	stype;
	eVerilogConstruct_t vtype;
	char			character;
	creal_t			creal;
	triple_t		triple;
	elist_t			elist;
	port_t			port;
} YYSTYPE;
/* Line 1318 of yacc.c.  */
#line 212 "sdfparse.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





