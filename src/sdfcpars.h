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
     IOPATH = 282,
     PORT = 283,
     INTERCONNECT = 284,
     NETDELAY = 285,
     DEVICE = 286,
     SETUP = 287,
     HOLD = 288,
     SETUPHOLD = 289,
     RECOVERY = 290,
     SKEW = 291,
     WIDTH = 292,
     PERIOD = 293,
     NOCHANGE = 294,
     PATHPULSE = 295,
     IGNORE = 296,
     INTERMOD_PATH = 297,
     MIPD = 298,
     CELL = 299,
     USE = 300,
     INTERCONNECT_MIPD = 301,
     MINIMUM = 302,
     MAXIMUM = 303,
     AVERAGE = 304,
     MTM = 305,
     TOOL_CONTROL = 306,
     TYPICAL = 307,
     SCALE_FACTORS = 308,
     SCALE_TYPE = 309,
     FROM_MINIMUM = 310,
     FROM_TYPICAL = 311,
     FROM_MAXIMUM = 312,
     FROM_MTM = 313,
     FROM_FILE = 314,
     TURNOFF_DELAY = 315,
     TURNON_DELAY = 316,
     MODULE = 317,
     MAP_INNER = 318,
     ADD = 319,
     OVERRIDE = 320,
     EOF_ = 321
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
#define IOPATH 282
#define PORT 283
#define INTERCONNECT 284
#define NETDELAY 285
#define DEVICE 286
#define SETUP 287
#define HOLD 288
#define SETUPHOLD 289
#define RECOVERY 290
#define SKEW 291
#define WIDTH 292
#define PERIOD 293
#define NOCHANGE 294
#define PATHPULSE 295
#define IGNORE 296
#define INTERMOD_PATH 297
#define MIPD 298
#define CELL 299
#define USE 300
#define INTERCONNECT_MIPD 301
#define MINIMUM 302
#define MAXIMUM 303
#define AVERAGE 304
#define MTM 305
#define TOOL_CONTROL 306
#define TYPICAL 307
#define SCALE_FACTORS 308
#define SCALE_TYPE 309
#define FROM_MINIMUM 310
#define FROM_TYPICAL 311
#define FROM_MAXIMUM 312
#define FROM_MTM 313
#define FROM_FILE 314
#define TURNOFF_DELAY 315
#define TURNON_DELAY 316
#define MODULE 317
#define MAP_INNER 318
#define ADD 319
#define OVERRIDE 320
#define EOF_ 321




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 96 "sdfcpars.yy"
typedef union YYSTYPE {
	int				integer;
	double			floating;
	char			*ptr;
	eScaleType_t	etype;
	eDelayType_t  	dtype;
	eSDFConstruct_t	stype;
	eVerilogConstruct_t vtype;
} YYSTYPE;
/* Line 1318 of yacc.c.  */
#line 179 "sdfcpars.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





