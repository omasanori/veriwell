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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 22 "sdfparse.yy"

#define YYERROR_VERBOSE

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "glue.h"
#include "sdflex.h"
#include "acc_user.h"
#include "veriuser.h"
#include "sdf.h"

#include "sdfparse.h"
#ifdef __cplusplus
    int	sdf_lex( YYSTYPE* );
#endif // __cplusplus

#define yymaxdepth sdf_maxdepth
#define yyparse sdf_parse
#define yylex sdf_lex
#define yyerror sdf_error
#define yylval sdf_lval
#define yychar sdf_char
#define yydebug sdf_debug
#define yypact sdf_pact
#define yyr1 sdf_r1
#define yyr2 sdf_r2
#define yydef sdf_def
#define yychk sdf_chk
#define yypgo sdf_pgo
#define yyact sdf_act
#define yyexca sdf_exca
#define yyerrflag sdf_errflag
#define yynerrs sdf_nerrs
#define yyps sdf_ps
#define yypv sdf_pv
#define yys sdf_s
#define yy_yys sdf_yys
#define yystate sdf_state
#define yytmp sdf_tmp
#define yyv sdf_v
#define yy_yyv sdf_yyv
#define yyval sdf_val
#define yylloc sdf_lloc
#define yyreds sdf_reds
#define yytoks sdf_toks
#define yylhs sdf_yylhs
#define yylen sdf_yylen
#define yydefred sdf_yydefred
#define yydgoto sdf_yydgoto
#define yysindex sdf_yysindex
#define yyrindex sdf_yyrindex
#define yygindex sdf_yygindex
#define yytable sdf_yytable
#define yycheck sdf_yycheck
#define yyname sdf_yyname
#define yyrule sdf_yyrule

/* Used in BISON.SIM */
extern char *token_buffer;


void initSDFParse()
{
	sdflexInit();
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

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
/* Line 190 of yacc.c.  */
#line 321 "sdfparse.cc"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 333 "sdfparse.cc"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   383

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  64
/* YYNRULES -- Number of rules. */
#define YYNRULES  141
/* YYNRULES -- Number of states. */
#define YYNSTATES  376

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   335

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,    28,    13,     2,
      29,    95,    26,    24,     2,    25,     2,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     2,
       2,     2,     2,     5,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    30,     2,    96,    11,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    10,     2,    32,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       7,     8,     9,    12,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     6,    22,    27,    32,    37,    42,    47,
      52,    53,    59,    64,    68,    73,    78,    82,    88,    94,
     100,   104,   106,   109,   111,   112,   113,   122,   127,   131,
     134,   138,   142,   146,   148,   152,   153,   157,   158,   163,
     164,   169,   172,   174,   180,   182,   183,   190,   197,   204,
     211,   218,   224,   230,   237,   240,   242,   243,   249,   250,
     256,   259,   261,   264,   266,   268,   270,   272,   275,   277,
     279,   282,   289,   300,   306,   313,   319,   324,   330,   331,
     338,   349,   355,   362,   368,   373,   379,   381,   382,   386,
     388,   390,   395,   402,   406,   410,   414,   418,   422,   426,
     430,   434,   438,   440,   443,   447,   451,   455,   457,   461,
     468,   478,   491,   507,   526,   530,   537,   547,   560,   576,
     595,   601,   607,   613,   614,   620,   626,   632,   633,   635,
     638,   641,   643,   645,   647,   648,   650,   651,   653,   654,
     656,   658
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
      98,     0,    -1,    99,    94,    -1,    29,    44,   100,   101,
     102,   103,   104,   105,   106,   108,   109,   110,   111,   113,
     159,    -1,    29,    45,   158,   159,    -1,    29,    46,   158,
     159,    -1,    29,    47,   158,   159,    -1,    29,    48,   158,
     159,    -1,    29,    49,   158,   159,    -1,    29,    50,   158,
     159,    -1,    -1,    29,    51,   107,    41,   159,    -1,    29,
      52,   152,   159,    -1,    29,    52,   159,    -1,    29,    53,
     158,   159,    -1,    29,    54,   152,   159,    -1,    29,    54,
     159,    -1,    29,    55,   112,    86,   159,    -1,    29,    55,
     112,    85,   159,    -1,    29,    55,   112,    87,   159,    -1,
      29,    55,   159,    -1,    36,    -1,   114,   113,    -1,   114,
      -1,    -1,    -1,    29,    56,   115,   116,    29,   117,   120,
     159,    -1,    29,    57,    39,   159,    -1,   118,    29,   117,
      -1,   118,    29,    -1,    58,    40,   159,    -1,    58,    26,
     159,    -1,    58,    40,   159,    -1,   121,    -1,   121,    29,
     120,    -1,    -1,    59,   128,   159,    -1,    -1,    60,   122,
     124,   159,    -1,    -1,    61,   123,    41,   159,    -1,   125,
     124,    -1,   125,    -1,    29,    68,   126,   127,   159,    -1,
     127,    -1,    -1,    29,    73,   143,   141,   148,   159,    -1,
      29,    74,   143,   141,   148,   159,    -1,    29,    75,   143,
     141,   147,   159,    -1,    29,    76,   141,   141,   148,   159,
      -1,    29,    77,   141,   141,   148,   159,    -1,    29,    78,
     141,   148,   159,    -1,    29,    79,   141,   148,   159,    -1,
      29,    80,   141,   143,   147,   159,    -1,   129,   128,    -1,
     129,    -1,    -1,    29,    62,   130,   132,   159,    -1,    -1,
      29,    63,   131,   133,   159,    -1,   138,   132,    -1,   138,
      -1,   140,   140,    -1,   140,    -1,   137,    -1,   135,    -1,
     136,    -1,   119,   136,    -1,    40,    -1,   143,    -1,   119,
     143,    -1,    29,    67,   141,   143,   146,   159,    -1,    29,
      68,   139,    29,    67,   141,   143,   146,   159,   159,    -1,
      29,    69,   143,   146,   159,    -1,    29,    70,   137,   137,
     146,   159,    -1,    29,    71,   134,   146,   159,    -1,    29,
      72,   146,   159,    -1,    29,    72,   137,   146,   159,    -1,
      -1,    29,    67,   141,   143,   149,   159,    -1,    29,    68,
     139,    29,    67,   141,   143,   149,   159,   159,    -1,    29,
      69,   143,   149,   159,    -1,    29,    70,   137,   137,   149,
     159,    -1,    29,    71,   134,   149,   159,    -1,    29,    72,
     146,   159,    -1,    29,    72,   137,   149,   159,    -1,   143,
      -1,    -1,    29,   142,   145,    -1,   144,    -1,    40,    -1,
      40,    30,    36,   160,    -1,    40,    30,    36,     6,    36,
     160,    -1,    42,   143,   159,    -1,    43,   143,   159,    -1,
      88,   143,   159,    -1,    89,   143,   159,    -1,    93,   143,
     159,    -1,    92,   143,   159,    -1,    91,   143,   159,    -1,
      90,   143,   159,    -1,    29,   155,   159,    -1,   150,    -1,
     148,   148,    -1,    29,   154,   159,    -1,    29,   152,   159,
      -1,    29,   154,   159,    -1,   151,    -1,    29,   153,   159,
      -1,    29,   153,   159,    29,   153,   159,    -1,    29,   153,
     159,    29,   153,   159,    29,   153,   159,    -1,    29,   153,
     159,    29,   153,   159,    29,   153,   159,    29,   153,   159,
      -1,    29,   153,   159,    29,   153,   159,    29,   153,   159,
      29,   153,   159,    29,   153,   159,    -1,    29,   153,   159,
      29,   153,   159,    29,   153,   159,    29,   153,   159,    29,
     153,   159,    29,   153,   159,    -1,    29,   152,   159,    -1,
      29,   152,   159,    29,   152,   159,    -1,    29,   152,   159,
      29,   152,   159,    29,   152,   159,    -1,    29,   152,   159,
      29,   152,   159,    29,   152,   159,    29,   152,   159,    -1,
      29,   152,   159,    29,   152,   159,    29,   152,   159,    29,
     152,   159,    29,   152,   159,    -1,    29,   152,   159,    29,
     152,   159,    29,   152,   159,    29,   152,   159,    29,   152,
     159,    29,   152,   159,    -1,   154,     6,   156,     6,   156,
      -1,   156,     6,   154,     6,   156,    -1,   156,     6,   156,
       6,   154,    -1,    -1,   155,     6,   157,     6,   157,    -1,
     157,     6,   155,     6,   157,    -1,   157,     6,   157,     6,
     155,    -1,    -1,   155,    -1,    24,   155,    -1,    25,   155,
      -1,    38,    -1,    36,    -1,   154,    -1,    -1,   155,    -1,
      -1,    39,    -1,    -1,    95,    -1,    96,    -1,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   222,   222,   226,   233,   238,   243,   248,   253,   258,
     263,   263,   268,   270,   275,   280,   282,   287,   292,   297,
     302,   310,   329,   330,   331,   335,   334,   345,   353,   354,
     358,   363,   371,   378,   379,   381,   383,   384,   384,   385,
     385,   392,   393,   398,   400,   404,   409,   413,   417,   421,
     425,   429,   433,   437,   444,   445,   450,   449,   456,   455,
     464,   465,   469,   470,   474,   475,   479,   480,   488,   498,
     499,   508,   512,   517,   521,   525,   529,   533,   539,   544,
     548,   553,   557,   561,   565,   569,   575,   576,   576,   583,
     587,   594,   603,   615,   620,   625,   630,   635,   640,   645,
     650,   659,   670,   674,   683,   694,   702,   713,   717,   722,
     728,   735,   744,   754,   769,   774,   780,   787,   796,   806,
     820,   827,   834,   842,   850,   857,   864,   872,   880,   881,
     883,   887,   888,   893,   899,   903,   909,   913,   915,   923,
     928,   930
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "'?'", "':'", "OROR",
  "ANDAND", "ANDANDAND", "'|'", "'^'", "XNOR", "'&'", "CASE_INEQUALITY",
  "CASE_EQUALITY", "LOGICAL_INEQUALITY", "LOGICAL_EQUALITY", "GE", "GT",
  "LE", "LT", "RIGHT_SHIFT", "LEFT_SHIFT", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'('", "'['", "'!'", "'~'", "REDUCTION_NOR", "REDUCTION_NAND",
  "UNARY", "INTEGER", "SCALAR_CONSTANT", "FLOATING", "QSTRING",
  "IDENTIFIER", "BLOB", "POSEDGE", "NEGEDGE", "DELAYFILE", "SDFVERSION",
  "DESIGN", "DATE", "VENDOR", "PROGRAM", "VERSION_", "DIVIDER", "VOLTAGE",
  "PROCESS", "TEMPERATURE", "TIMESCALE", "CELL", "CELLTYPE", "INSTANCE",
  "DELAY", "TIMINGCHECK", "INCLUDE", "ABSOLUTE", "INCREMENT", "USERDEF",
  "PATHPULSE", "GLOBALPATHPULSE", "IOPATH", "COND", "PORT", "INTERCONNECT",
  "NETDELAY", "DEVICE", "SETUP", "HOLD", "SETUPHOLD", "RECOVERY", "SKEW",
  "WIDTH", "PERIOD", "NOCHANGE", "PATHCONSTRAINT", "SUM", "DIFF",
  "SKEWCONSTRAINT", "MICROSECOND", "NANOSECOND", "PICOSECOND", "EDGE_01",
  "EDGE_10", "EDGE_0z", "EDGE_1z", "EDGE_z0", "EDGE_z1", "EOF_", "')'",
  "']'", "$accept", "sdf", "delay_file", "sdf_version", "design_name",
  "date", "vendor", "program_name", "program_version", "hierarchy_divider",
  "@1", "voltage", "process", "temperature", "time_scale", "ts", "cells",
  "cell", "@2", "celltype", "instances", "instance", "string_instance",
  "timing_specs", "timing_spec", "@3", "@4", "tcdefs", "tcdef",
  "timing_check_condition", "tcheck", "deltypes", "deltype", "@5", "@6",
  "absvals", "incvals", "name", "net_instance", "net", "port_instance",
  "absval", "conditional_port_expr", "incval", "port_spec", "@7",
  "port_path", "port", "port_edge", "value", "double_tc_rvalue",
  "tc_rvalue", "rvalue", "exp_list", "rexp_list", "rexpression",
  "expression", "real", "preal", "creal", "cpreal", "nstring", "rp", "rb", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,    63,    58,   260,   261,   262,
     124,    94,   263,    38,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,    43,    45,    42,    47,    37,    40,
      91,    33,   126,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,    41,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     107,   106,   108,   108,   109,   110,   110,   111,   111,   111,
     111,   112,   113,   113,   113,   115,   114,   116,   117,   117,
     118,   118,   119,   120,   120,   121,   121,   122,   121,   123,
     121,   124,   124,   125,   125,   126,   127,   127,   127,   127,
     127,   127,   127,   127,   128,   128,   130,   129,   131,   129,
     132,   132,   133,   133,   134,   134,   135,   135,   136,   137,
     137,   138,   138,   138,   138,   138,   138,   138,   139,   140,
     140,   140,   140,   140,   140,   140,   141,   142,   141,   143,
     144,   144,   144,   145,   145,   145,   145,   145,   145,   145,
     145,   146,   146,   147,   148,   148,   149,   149,   150,   150,
     150,   150,   150,   150,   151,   151,   151,   151,   151,   151,
     152,   152,   152,   152,   153,   153,   153,   153,   154,   154,
     154,   155,   155,   156,   156,   157,   157,   158,   158,   159,
     160,   160
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,    15,     4,     4,     4,     4,     4,     4,
       0,     5,     4,     3,     4,     4,     3,     5,     5,     5,
       3,     1,     2,     1,     0,     0,     8,     4,     3,     2,
       3,     3,     3,     1,     3,     0,     3,     0,     4,     0,
       4,     2,     1,     5,     1,     0,     6,     6,     6,     6,
       6,     5,     5,     6,     2,     1,     0,     5,     0,     5,
       2,     1,     2,     1,     1,     1,     1,     2,     1,     1,
       2,     6,    10,     5,     6,     5,     4,     5,     0,     6,
      10,     5,     6,     5,     4,     5,     1,     0,     3,     1,
       1,     4,     6,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     2,     3,     3,     3,     1,     3,     6,
       9,    12,    15,    18,     3,     6,     9,    12,    15,    18,
       5,     5,     5,     0,     5,     5,     5,     0,     1,     2,
       2,     1,     1,     1,     0,     1,     0,     1,     0,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,     0,     1,     2,     0,     0,   138,
       0,     0,   137,     0,   138,     0,     0,   139,     4,     0,
     138,     0,     0,     5,     0,   138,     0,     0,     6,     0,
     138,     0,     0,     7,     0,   138,     0,     0,     8,     0,
      10,     0,     0,     9,     0,   134,     0,     0,     0,     0,
       0,   132,   131,     0,     0,   128,     0,    13,   138,     0,
       0,    11,   129,   130,    12,   134,   134,     0,   134,     0,
      24,   133,     0,     0,     0,    14,     0,    16,     0,     0,
       0,    23,   134,   134,     0,    15,    21,     0,    20,    25,
       3,    22,   120,   121,   122,     0,     0,     0,     0,    18,
      17,    19,     0,     0,     0,     0,     0,     0,    35,     0,
      27,     0,     0,     0,    37,    39,     0,    33,    29,    31,
      30,     0,     0,    55,     0,     0,    26,    35,    28,    56,
      58,    36,    54,     0,     0,    42,    44,     0,    34,     0,
       0,    45,     0,     0,     0,     0,     0,     0,     0,     0,
      38,    41,    40,     0,     0,    61,     0,     0,    63,     0,
      90,     0,    89,     0,     0,    87,     0,    86,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,    57,    60,
       0,    78,     0,     0,     0,     0,    59,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    68,     0,
       0,    65,    66,    64,   127,     0,     0,   102,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
       0,    70,     0,    67,     0,     0,     0,     0,     0,    76,
       0,     0,   123,     0,   107,     0,     0,     0,    84,   141,
       0,   140,    91,    46,    47,    48,   103,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,   105,   104,    53,
       0,     0,    73,    32,     0,    75,   108,   136,   101,   136,
      77,     0,     0,     0,     0,    81,     0,    83,    85,     0,
      93,    94,    95,    96,   100,    99,    98,    97,    71,     0,
      74,   127,   135,     0,     0,     0,    79,     0,   114,   106,
      82,    92,     0,     0,     0,   136,   136,     0,     0,   123,
       0,   109,   124,   125,   126,     0,     0,     0,   127,     0,
     115,    72,     0,    80,   123,   110,     0,   127,   116,     0,
     123,   111,     0,   127,   117,     0,   123,   112,     0,   127,
     118,     0,   123,   113,     0,   119
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     2,     3,     8,    11,    16,    22,    27,    32,    37,
      44,    42,    47,    60,    70,    87,    80,    81,    98,   103,
     108,   109,   205,   116,   117,   124,   125,   134,   135,   159,
     136,   122,   123,   139,   140,   154,   157,   210,   211,   212,
     213,   155,   202,   158,   166,   194,   167,   162,   239,   216,
     229,   230,   263,   217,   264,    53,   255,    54,    55,    56,
     257,    13,    18,   272
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -308
static const short int yypact[] =
{
      -2,   -13,    36,   -42,    24,  -308,  -308,    40,    35,    30,
      45,    81,  -308,    21,    30,    67,    88,  -308,  -308,    21,
      30,    71,    91,  -308,    21,    30,    74,    97,  -308,    21,
      30,    82,   102,  -308,    21,    30,    83,   108,  -308,    21,
    -308,    94,   111,  -308,   113,    -6,    99,   134,    21,    52,
      52,  -308,  -308,    21,   159,  -308,   160,  -308,    30,   115,
     142,  -308,  -308,  -308,  -308,   103,   103,    21,    -6,   117,
     148,  -308,   172,   175,   183,  -308,    21,  -308,   -32,   136,
      21,   148,   103,   103,   103,  -308,  -308,    64,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,    21,    21,    21,   174,  -308,
    -308,  -308,   149,   184,   176,   156,    21,    54,   124,   187,
    -308,    21,    21,   188,  -308,  -308,    21,   190,   156,  -308,
    -308,   -51,    21,   188,   191,   185,  -308,   124,  -308,  -308,
    -308,  -308,  -308,   283,    21,   191,  -308,    21,  -308,   203,
     204,  -308,   194,   194,   194,     4,     4,     4,     4,     4,
    -308,  -308,  -308,   305,    21,   203,   311,    21,   204,   209,
     211,     4,  -308,     4,     4,  -308,     4,  -308,     4,   213,
     213,   194,     4,  -308,   194,    -3,    46,    25,  -308,  -308,
       4,  -308,   194,    -3,    46,    25,  -308,  -308,   291,    21,
     210,   213,   213,   213,   -18,   213,   213,   162,    21,    21,
     213,   194,   216,   220,   214,   194,    -3,  -308,   211,   215,
     220,  -308,  -308,  -308,    22,   220,    21,  -308,   194,   224,
     227,    -3,   227,   227,    21,  -308,     0,    21,    21,    21,
     213,   194,   194,   194,   194,   194,   194,   194,   194,  -308,
      21,    21,    21,     7,  -308,  -308,    21,   220,   199,    21,
      21,  -308,   220,  -308,    21,    21,    17,   251,    21,  -308,
     227,   200,   162,    21,  -308,   227,    21,    21,  -308,  -308,
     232,  -308,  -308,  -308,  -308,  -308,  -308,    21,    21,    21,
      21,    21,    21,    21,    21,  -308,  -308,  -308,  -308,  -308,
      21,     4,  -308,  -308,    21,  -308,   240,    52,  -308,    52,
    -308,    21,     4,    21,     7,  -308,    21,  -308,  -308,    15,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,   194,
    -308,    22,  -308,   264,   266,   267,  -308,   194,   247,  -308,
    -308,  -308,   220,    21,   271,    52,    52,    52,   227,   162,
      21,   250,  -308,  -308,  -308,    21,    21,    21,    22,    21,
     252,  -308,    21,  -308,   162,   254,    21,    22,   257,    21,
     162,   263,    21,    22,   265,    21,   162,   269,    21,    22,
     274,    21,   162,  -308,    21,  -308
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,   219,  -308,  -308,  -308,
     177,  -308,   -71,   153,  -308,  -308,  -308,   161,  -308,  -308,
     145,   186,  -308,  -308,  -308,   151,  -308,   129,  -308,   106,
      19,  -308,   127,   165,    27,  -308,   -76,  -308,  -308,  -168,
     119,   -48,  -214,  -308,  -308,   -61,  -307,   -44,   -47,   146,
    -254,   277,   -19,     8
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -137
static const short int yytable[] =
{
      23,   269,    62,    63,    86,    28,   270,    76,   266,   267,
      33,   129,   130,    65,   333,    38,   269,   224,    49,    50,
      43,    71,    73,   297,   231,   232,    57,     1,  -136,    61,
      51,     4,    52,   165,    64,   249,     5,   160,    71,    71,
      94,   352,   254,   323,   160,   325,   301,   258,    75,    77,
     359,   306,     6,     7,   214,   204,   365,    85,    51,    88,
      52,    90,   371,    17,    10,   160,   161,   163,   164,    12,
     233,   234,   235,   236,   237,   238,    99,   100,   101,   290,
     111,   342,   343,   204,   294,     9,   208,   110,    51,    17,
      52,    14,   119,   120,   112,   200,   271,   126,   203,   207,
     207,   207,    17,   131,   204,   209,   220,   207,   207,   207,
      15,   271,    17,   209,    20,   150,    17,    21,   152,    25,
      26,   198,   199,    30,   345,   247,    31,    49,    50,   251,
     207,    36,    35,   251,    40,   178,   242,    41,   186,    51,
      46,    52,   260,   227,   228,   207,    45,   240,   241,    95,
      96,    97,    58,   243,    48,   277,   278,   279,   280,   281,
     282,   283,   284,    59,   340,    65,    66,   256,  -134,    68,
     225,    69,    78,   168,   169,   170,   171,    79,    82,   244,
     245,    83,   276,   113,   114,   115,    49,    50,   191,    84,
     192,   193,    89,   195,   206,   196,   215,   259,    51,   201,
      52,   303,   221,   102,   223,   268,   104,   218,   273,   274,
     275,    72,    74,   105,   107,   106,   118,   121,   304,   127,
     133,   285,   286,   287,   288,   252,   137,   289,    92,    93,
     292,   293,   153,   156,   160,   295,   296,   298,   188,   300,
     265,   190,   197,   332,   305,   248,   226,   307,   308,   214,
     322,   338,   324,   261,   250,   208,   262,   299,   310,   311,
     312,   313,   314,   315,   316,   317,   291,   302,   309,   321,
     335,   318,   336,   337,   334,   320,   339,   297,   346,   348,
     138,   354,   326,   357,   328,   329,   360,   330,   322,   322,
     344,    19,   363,   356,   366,   128,   151,    24,   369,   362,
      91,   334,    29,   372,   189,   368,   179,    34,   219,   132,
     334,   374,    39,   222,   341,   253,   334,   331,   319,   246,
       0,   347,   334,   187,     0,     0,   349,   350,   351,   327,
     353,     0,     0,   355,     0,    67,     0,   358,     0,     0,
     361,     0,     0,   364,     0,     0,   367,     0,     0,   370,
       0,   141,   373,     0,     0,   375,   142,   143,   144,   145,
     146,   147,   148,   149,   142,   143,   144,   145,   146,   147,
     148,   149,   172,   173,   174,   175,   176,   177,   180,   181,
     182,   183,   184,   185
};

static const short int yycheck[] =
{
      19,     1,    49,    50,    36,    24,     6,    68,   222,   223,
      29,    62,    63,     6,   321,    34,     1,   185,    24,    25,
      39,    65,    66,     6,    42,    43,    45,    29,     6,    48,
      36,    44,    38,    29,    53,   203,     0,    40,    82,    83,
      84,   348,   210,   297,    40,   299,   260,   215,    67,    68,
     357,   265,    94,    29,    29,    58,   363,    76,    36,    78,
      38,    80,   369,    95,    29,    40,   142,   143,   144,    39,
      88,    89,    90,    91,    92,    93,    95,    96,    97,   247,
      26,   335,   336,    58,   252,    45,    40,   106,    36,    95,
      38,    46,   111,   112,    40,   171,    96,   116,   174,   175,
     176,   177,    95,   122,    58,   176,   182,   183,   184,   185,
      29,    96,    95,   184,    47,   134,    95,    29,   137,    48,
      29,   169,   170,    49,   338,   201,    29,    24,    25,   205,
     206,    29,    50,   209,    51,   154,   197,    29,   157,    36,
      29,    38,   218,   191,   192,   221,    52,   195,   196,    85,
      86,    87,    53,   197,    41,   231,   232,   233,   234,   235,
     236,   237,   238,    29,   332,     6,     6,   214,     6,    54,
     189,    29,    55,   146,   147,   148,   149,    29,     6,   198,
     199,     6,   230,    59,    60,    61,    24,    25,   161,     6,
     163,   164,    56,   166,   175,   168,   177,   216,    36,   172,
      38,   262,   183,    29,   185,   224,    57,   180,   227,   228,
     229,    65,    66,    29,    58,    39,    29,    29,   262,    29,
      29,   240,   241,   242,   243,   206,    41,   246,    82,    83,
     249,   250,    29,    29,    40,   254,   255,   256,    29,   258,
     221,    30,    29,   319,   263,    29,    36,   266,   267,    29,
     297,   327,   299,    29,    40,    40,    29,     6,   277,   278,
     279,   280,   281,   282,   283,   284,    67,    67,    36,    29,
       6,   290,     6,     6,   321,   294,    29,     6,   339,    29,
     127,    29,   301,    29,   303,   304,    29,   306,   335,   336,
     337,    14,    29,   354,    29,   118,   135,    20,    29,   360,
      81,   348,    25,    29,   159,   366,   155,    30,   181,   123,
     357,   372,    35,   184,   333,   209,   363,   309,   291,   200,
      -1,   340,   369,   158,    -1,    -1,   345,   346,   347,   302,
     349,    -1,    -1,   352,    -1,    58,    -1,   356,    -1,    -1,
     359,    -1,    -1,   362,    -1,    -1,   365,    -1,    -1,   368,
      -1,    68,   371,    -1,    -1,   374,    73,    74,    75,    76,
      77,    78,    79,    80,    73,    74,    75,    76,    77,    78,
      79,    80,    67,    68,    69,    70,    71,    72,    67,    68,
      69,    70,    71,    72
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    29,    98,    99,    44,     0,    94,    29,   100,    45,
      29,   101,    39,   158,    46,    29,   102,    95,   159,   158,
      47,    29,   103,   159,   158,    48,    29,   104,   159,   158,
      49,    29,   105,   159,   158,    50,    29,   106,   159,   158,
      51,    29,   108,   159,   107,    52,    29,   109,    41,    24,
      25,    36,    38,   152,   154,   155,   156,   159,    53,    29,
     110,   159,   155,   155,   159,     6,     6,   158,    54,    29,
     111,   154,   156,   154,   156,   159,   152,   159,    55,    29,
     113,   114,     6,     6,     6,   159,    36,   112,   159,    56,
     159,   113,   156,   156,   154,    85,    86,    87,   115,   159,
     159,   159,    29,   116,    57,    29,    39,    58,   117,   118,
     159,    26,    40,    59,    60,    61,   120,   121,    29,   159,
     159,    29,   128,   129,   122,   123,   159,    29,   117,    62,
      63,   159,   128,    29,   124,   125,   127,    41,   120,   130,
     131,    68,    73,    74,    75,    76,    77,    78,    79,    80,
     159,   124,   159,    29,   132,   138,    29,   133,   140,   126,
      40,   143,   144,   143,   143,    29,   141,   143,   141,   141,
     141,   141,    67,    68,    69,    70,    71,    72,   159,   132,
      67,    68,    69,    70,    71,    72,   159,   140,    29,   127,
      30,   141,   141,   141,   142,   141,   141,    29,   148,   148,
     143,   141,   139,   143,    58,   119,   137,   143,    40,   119,
     134,   135,   136,   137,    29,   137,   146,   150,   141,   139,
     143,   137,   134,   137,   146,   159,    36,   148,   148,   147,
     148,    42,    43,    88,    89,    90,    91,    92,    93,   145,
     148,   148,   152,   154,   159,   159,   147,   143,    29,   146,
      40,   143,   137,   136,   146,   153,   155,   157,   146,   159,
     143,    29,    29,   149,   151,   137,   149,   149,   159,     1,
       6,    96,   160,   159,   159,   159,   148,   143,   143,   143,
     143,   143,   143,   143,   143,   159,   159,   159,   159,   159,
     146,    67,   159,   159,   146,   159,   159,     6,   159,     6,
     159,   149,    67,   152,   154,   159,   149,   159,   159,    36,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   141,
     159,    29,   155,   157,   155,   157,   159,   141,   159,   159,
     159,   160,   143,   153,   155,     6,     6,     6,   143,    29,
     146,   159,   157,   157,   155,   149,   152,   159,    29,   159,
     159,   159,   153,   159,    29,   159,   152,    29,   159,   153,
      29,   159,   152,    29,   159,   153,    29,   159,   152,    29,
     159,   153,    29,   159,   152,   159
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  yyvsp[0] = yylval;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 229 "sdfparse.yy"
    {}
    break;

  case 4:
#line 234 "sdfparse.yy"
    {}
    break;

  case 5:
#line 239 "sdfparse.yy"
    {}
    break;

  case 6:
#line 244 "sdfparse.yy"
    {}
    break;

  case 7:
#line 249 "sdfparse.yy"
    {}
    break;

  case 8:
#line 254 "sdfparse.yy"
    {}
    break;

  case 9:
#line 259 "sdfparse.yy"
    {}
    break;

  case 10:
#line 263 "sdfparse.yy"
    { setParseBlob(); }
    break;

  case 11:
#line 264 "sdfparse.yy"
    {}
    break;

  case 12:
#line 269 "sdfparse.yy"
    {}
    break;

  case 13:
#line 271 "sdfparse.yy"
    {}
    break;

  case 14:
#line 276 "sdfparse.yy"
    {}
    break;

  case 15:
#line 281 "sdfparse.yy"
    {}
    break;

  case 16:
#line 283 "sdfparse.yy"
    {}
    break;

  case 17:
#line 288 "sdfparse.yy"
    {
			/* set time scale for back annotation */
			setTimeScale( (yyvsp[-2].integer) - 9 );
		}
    break;

  case 18:
#line 293 "sdfparse.yy"
    {
			/* set time scale for back annotation */
			setTimeScale( (yyvsp[-2].integer) - 6 );
		}
    break;

  case 19:
#line 298 "sdfparse.yy"
    {
			/* set time scale for back annotation */
			setTimeScale( (yyvsp[-2].integer) - 12 );
		}
    break;

  case 20:
#line 303 "sdfparse.yy"
    {
			/* set default (1ns) time scale for back annotation */
			setTimeScale( -9 );
		}
    break;

  case 21:
#line 311 "sdfparse.yy"
    {
			switch(	(yyvsp[0].integer) ) {
				case 1:
					(yyval.integer) = 0;
					break;
				case 10:
					(yyval.integer) = 1;
					break;
				case 100:
					(yyval.integer) = 2;
					break;
				default:
					sdf_error( "Timescale multiplier must be either 1,10, or 100" );
					(yyval.integer) = 0;
			} 
		}
    break;

  case 25:
#line 335 "sdfparse.yy"
    {
			/* reset current instance */
			resetInstance();
			/* free all allocated strings */
			purgeStrings();
		}
    break;

  case 27:
#line 346 "sdfparse.yy"
    {	
			/* set current cell type */
			setCellType( (yyvsp[-1].ptr) );
		}
    break;

  case 30:
#line 359 "sdfparse.yy"
    {
			/* set down thru the hierarchy */
			findInstance( (yyvsp[-1].ptr) );
		}
    break;

  case 31:
#line 364 "sdfparse.yy"
    {
			/* this is a wild card  */
			findInstance( "*" );
		}
    break;

  case 32:
#line 372 "sdfparse.yy"
    {
			(yyval.ptr) = (yyvsp[-1].ptr);
		}
    break;

  case 37:
#line 384 "sdfparse.yy"
    {setAbsoluteDelay(TRUE);}
    break;

  case 39:
#line 385 "sdfparse.yy"
    { setParseBlob(); }
    break;

  case 40:
#line 386 "sdfparse.yy"
    {
			sdflexOpenFile( (yyvsp[-1].ptr) );
		}
    break;

  case 43:
#line 399 "sdfparse.yy"
    {}
    break;

  case 44:
#line 401 "sdfparse.yy"
    {}
    break;

  case 46:
#line 410 "sdfparse.yy"
    {
			setConstraint( eSETUP, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 47:
#line 414 "sdfparse.yy"
    {
			setConstraint( eHOLD, &(yyvsp[-2].port), &(yyvsp[-3].port), (yyvsp[-1].elist) );
		}
    break;

  case 48:
#line 418 "sdfparse.yy"
    {
			setConstraint( eSETUPHOLD, &(yyvsp[-2].port), &(yyvsp[-3].port), (yyvsp[-1].elist) );
		}
    break;

  case 49:
#line 422 "sdfparse.yy"
    {
			setConstraint( eRECOVERY, &(yyvsp[-2].port), &(yyvsp[-3].port), (yyvsp[-1].elist) );
		}
    break;

  case 50:
#line 426 "sdfparse.yy"
    {
			setConstraint( eSKEW, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 51:
#line 430 "sdfparse.yy"
    {
			setConstraint( eWIDTH, &(yyvsp[-2].port), NULL, (yyvsp[-1].elist) );
		}
    break;

  case 52:
#line 434 "sdfparse.yy"
    {
			setConstraint( ePERIOD, &(yyvsp[-2].port), NULL, (yyvsp[-1].elist) );
		}
    break;

  case 53:
#line 438 "sdfparse.yy"
    {
			setConstraint( eNOCHANGE, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 56:
#line 450 "sdfparse.yy"
    {
			/* mark delay type as absolute */
			setAbsoluteDelay( TRUE );
		}
    break;

  case 58:
#line 456 "sdfparse.yy"
    {
			/* mark delay type as incremental */
			setAbsoluteDelay( FALSE );
		}
    break;

  case 67:
#line 481 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[0].port);
			(yyval.port).path = (yyvsp[-1].ptr);
		}
    break;

  case 68:
#line 489 "sdfparse.yy"
    {
			(yyval.port).path = NULL;
			(yyval.port).name = (yyvsp[0].ptr);
			(yyval.port).scalar = TRUE;
			(yyval.port).edge = accNoEdge;
		}
    break;

  case 70:
#line 500 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[0].port);
			(yyval.port).path = (yyvsp[-1].ptr);
		}
    break;

  case 71:
#line 509 "sdfparse.yy"
    {
			setConstraint( eIOPATH, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 72:
#line 513 "sdfparse.yy"
    {
			/* -------- support for COND needs to be added ----------- */
			setConstraint( eIOPATH, &(yyvsp[-4].port), &(yyvsp[-3].port), (yyvsp[-2].elist) );
		}
    break;

  case 73:
#line 518 "sdfparse.yy"
    {
			setConstraint( ePORT, &(yyvsp[-2].port), (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 74:
#line 522 "sdfparse.yy"
    {
			setConstraint( eINTERCONNECT, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 75:
#line 526 "sdfparse.yy"
    {
			setConstraint( eNETDELAY, &(yyvsp[-2].port), (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 76:
#line 530 "sdfparse.yy"
    {
			setConstraint( eDEVICE, (port_t*)NULL, (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 77:
#line 534 "sdfparse.yy"
    {
			setConstraint( eDEVICE, &(yyvsp[-2].port), (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 79:
#line 545 "sdfparse.yy"
    {
			setConstraint( eIOPATH, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 80:
#line 549 "sdfparse.yy"
    {
			/* -------- support for COND needs to be added ----------- */
			setConstraint( eIOPATH, &(yyvsp[-4].port), &(yyvsp[-3].port), (yyvsp[-2].elist) );
		}
    break;

  case 81:
#line 554 "sdfparse.yy"
    {
			setConstraint( ePORT, &(yyvsp[-2].port), (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 82:
#line 558 "sdfparse.yy"
    {
			setConstraint( eINTERCONNECT, &(yyvsp[-3].port), &(yyvsp[-2].port), (yyvsp[-1].elist) );
		}
    break;

  case 83:
#line 562 "sdfparse.yy"
    {
			setConstraint( eNETDELAY, &(yyvsp[-2].port), (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 84:
#line 566 "sdfparse.yy"
    {
			setConstraint( eDEVICE, (port_t*)NULL, (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 85:
#line 570 "sdfparse.yy"
    {
			setConstraint( eDEVICE, &(yyvsp[-2].port), (port_t*)NULL, (yyvsp[-1].elist) );
		}
    break;

  case 87:
#line 576 "sdfparse.yy"
    {sdfEnableEdges( TRUE ); }
    break;

  case 88:
#line 577 "sdfparse.yy"
    { 
			sdfEnableEdges( FALSE );
			(yyval.port) = (yyvsp[0].port);
		}
    break;

  case 90:
#line 588 "sdfparse.yy"
    {
			(yyval.port).path = NULL;
			(yyval.port).name = (yyvsp[0].ptr);
			(yyval.port).scalar = TRUE;
			(yyval.port).edge = accNoEdge;
		}
    break;

  case 91:
#line 595 "sdfparse.yy"
    {
			(yyval.port).path = NULL;
			(yyval.port).name = (yyvsp[-3].ptr);
			(yyval.port).scalar = FALSE;
			(yyval.port).msb = (yyvsp[-1].integer);
			(yyval.port).lsb = (yyvsp[-1].integer);
			(yyval.port).edge = accNoEdge;
		}
    break;

  case 92:
#line 604 "sdfparse.yy"
    {
			(yyval.port).path = NULL;
			(yyval.port).name = (yyvsp[-5].ptr);
			(yyval.port).scalar = FALSE;
			(yyval.port).msb = (yyvsp[-3].integer);
			(yyval.port).lsb = (yyvsp[-1].integer);
			(yyval.port).edge = accNoedge;
		}
    break;

  case 93:
#line 616 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accPosedge;
		}
    break;

  case 94:
#line 621 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accNegedge;
		}
    break;

  case 95:
#line 626 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accEdge01;
		}
    break;

  case 96:
#line 631 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accEdge10;
		}
    break;

  case 97:
#line 636 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accEdgex1;
		}
    break;

  case 98:
#line 641 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accEdgex0;
		}
    break;

  case 99:
#line 646 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accEdge1x;
		}
    break;

  case 100:
#line 651 "sdfparse.yy"
    {
			(yyval.port) = (yyvsp[-1].port);
			(yyval.port).edge = accEdge0x;
		}
    break;

  case 101:
#line 660 "sdfparse.yy"
    {
			memset( &(yyval.elist), 0, sizeof((yyval.elist)) );
			(yyval.elist).values[0].triple[0].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[0].valid = TRUE;
			(yyval.elist).values[0].triple[1].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[1].valid = TRUE;
			(yyval.elist).values[0].triple[2].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[2].valid = TRUE;
			(yyval.elist).argCount = 1;
		}
    break;

  case 103:
#line 675 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-1].elist).values[0];
			(yyval.elist).values[1] = (yyvsp[0].elist).values[0];
			(yyval.elist).argCount = 2;
		}
    break;

  case 104:
#line 684 "sdfparse.yy"
    {
			memset( &(yyval.elist), 0, sizeof((yyval.elist)) );
			(yyval.elist).values[0].triple[0].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[0].valid = TRUE;
			(yyval.elist).values[0].triple[1].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[1].valid = TRUE;
			(yyval.elist).values[0].triple[2].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[2].valid = TRUE;
			(yyval.elist).argCount = 1;
		}
    break;

  case 105:
#line 695 "sdfparse.yy"
    { 
			(yyval.elist).values[0] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 1;
		}
    break;

  case 106:
#line 703 "sdfparse.yy"
    {
			memset( &(yyval.elist), 0, sizeof((yyval.elist)) );
			(yyval.elist).values[0].triple[0].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[0].valid = TRUE;
			(yyval.elist).values[0].triple[1].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[1].valid = TRUE;
			(yyval.elist).values[0].triple[2].value = (yyvsp[-1].floating);
			(yyval.elist).values[0].triple[2].valid = TRUE;
			(yyval.elist).argCount = 1;
		}
    break;

  case 108:
#line 718 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 1;
		}
    break;

  case 109:
#line 723 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-4].triple);
			(yyval.elist).values[1] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 2;
		}
    break;

  case 110:
#line 729 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-7].triple);
			(yyval.elist).values[1] = (yyvsp[-4].triple);
			(yyval.elist).values[2] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 3;
		}
    break;

  case 111:
#line 737 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-10].triple);
			(yyval.elist).values[1] = (yyvsp[-7].triple);
			(yyval.elist).values[2] = (yyvsp[-4].triple);
			(yyval.elist).values[3] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 4;
		}
    break;

  case 112:
#line 746 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-13].triple);
			(yyval.elist).values[1] = (yyvsp[-10].triple);
			(yyval.elist).values[2] = (yyvsp[-7].triple);
			(yyval.elist).values[3] = (yyvsp[-4].triple);
			(yyval.elist).values[4] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 5;
		}
    break;

  case 113:
#line 756 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-16].triple);
			(yyval.elist).values[1] = (yyvsp[-13].triple);
			(yyval.elist).values[2] = (yyvsp[-10].triple);
			(yyval.elist).values[3] = (yyvsp[-7].triple);
			(yyval.elist).values[4] = (yyvsp[-4].triple);
			(yyval.elist).values[5] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 6;
		}
    break;

  case 114:
#line 770 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 1;
		}
    break;

  case 115:
#line 775 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-4].triple);
			(yyval.elist).values[1] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 2;
		}
    break;

  case 116:
#line 781 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-7].triple);
			(yyval.elist).values[1] = (yyvsp[-4].triple);
			(yyval.elist).values[2] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 3;
		}
    break;

  case 117:
#line 789 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-10].triple);
			(yyval.elist).values[1] = (yyvsp[-7].triple);
			(yyval.elist).values[2] = (yyvsp[-4].triple);
			(yyval.elist).values[3] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 4;
		}
    break;

  case 118:
#line 798 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-13].triple);
			(yyval.elist).values[1] = (yyvsp[-10].triple);
			(yyval.elist).values[2] = (yyvsp[-7].triple);
			(yyval.elist).values[3] = (yyvsp[-4].triple);
			(yyval.elist).values[4] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 5;
		}
    break;

  case 119:
#line 808 "sdfparse.yy"
    {
			(yyval.elist).values[0] = (yyvsp[-16].triple);
			(yyval.elist).values[1] = (yyvsp[-13].triple);
			(yyval.elist).values[2] = (yyvsp[-10].triple);
			(yyval.elist).values[3] = (yyvsp[-7].triple);
			(yyval.elist).values[4] = (yyvsp[-4].triple);
			(yyval.elist).values[5] = (yyvsp[-1].triple);
			(yyval.elist).argCount = 6;
		}
    break;

  case 120:
#line 821 "sdfparse.yy"
    {
			(yyval.triple).triple[0].value = (yyvsp[-4].floating);
			(yyval.triple).triple[0].valid = TRUE;
			(yyval.triple).triple[1] = (yyvsp[-2].creal);
			(yyval.triple).triple[2] = (yyvsp[0].creal);
		}
    break;

  case 121:
#line 828 "sdfparse.yy"
    {
			(yyval.triple).triple[0] = (yyvsp[-4].creal);
			(yyval.triple).triple[1].value = (yyvsp[-2].floating);
			(yyval.triple).triple[1].valid = TRUE; 
			(yyval.triple).triple[2] = (yyvsp[0].creal);
		}
    break;

  case 122:
#line 835 "sdfparse.yy"
    {
			(yyval.triple).triple[0] = (yyvsp[-4].creal);
			(yyval.triple).triple[1] = (yyvsp[-2].creal);
			(yyval.triple).triple[2].value = (yyvsp[0].floating);
			(yyval.triple).triple[2].valid = TRUE;
		}
    break;

  case 123:
#line 842 "sdfparse.yy"
    {
			(yyval.triple).triple[0].valid = FALSE;
			(yyval.triple).triple[1].valid = FALSE;
			(yyval.triple).triple[2].valid = FALSE;
		}
    break;

  case 124:
#line 851 "sdfparse.yy"
    {
			(yyval.triple).triple[0].value = (yyvsp[-4].floating);
			(yyval.triple).triple[0].valid = TRUE;
			(yyval.triple).triple[1] = (yyvsp[-2].creal);
			(yyval.triple).triple[2] = (yyvsp[0].creal);
		}
    break;

  case 125:
#line 858 "sdfparse.yy"
    {
			(yyval.triple).triple[0] = (yyvsp[-4].creal);
			(yyval.triple).triple[1].value = (yyvsp[-2].floating);
			(yyval.triple).triple[1].valid = TRUE; 
			(yyval.triple).triple[2] = (yyvsp[0].creal);
		}
    break;

  case 126:
#line 865 "sdfparse.yy"
    {
			(yyval.triple).triple[0] = (yyvsp[-4].creal);
			(yyval.triple).triple[1] = (yyvsp[-2].creal);
			(yyval.triple).triple[2].value = (yyvsp[0].floating);
			(yyval.triple).triple[2].valid = TRUE;
		}
    break;

  case 127:
#line 872 "sdfparse.yy"
    {
			(yyval.triple).triple[0].valid = FALSE;
			(yyval.triple).triple[1].valid = FALSE;
			(yyval.triple).triple[2].valid = FALSE;
		}
    break;

  case 129:
#line 882 "sdfparse.yy"
    { (yyval.floating) = (yyvsp[0].floating); }
    break;

  case 130:
#line 884 "sdfparse.yy"
    { (yyval.floating) = -(yyvsp[0].floating); }
    break;

  case 132:
#line 889 "sdfparse.yy"
    { (yyval.floating) = (double) (yyvsp[0].integer); }
    break;

  case 133:
#line 894 "sdfparse.yy"
    { 	
			(yyval.creal).value = (yyvsp[0].floating); 
			(yyval.creal).valid = TRUE;
		}
    break;

  case 134:
#line 899 "sdfparse.yy"
    { (yyval.creal).valid = FALSE; }
    break;

  case 135:
#line 904 "sdfparse.yy"
    { 	
			(yyval.creal).value = (yyvsp[0].floating); 
			(yyval.creal).valid = TRUE;
		}
    break;

  case 136:
#line 909 "sdfparse.yy"
    { (yyval.creal).valid = FALSE; }
    break;

  case 138:
#line 915 "sdfparse.yy"
    { (yyval.ptr) = ""; }
    break;

  case 139:
#line 924 "sdfparse.yy"
    { yyerrok; }
    break;

  case 140:
#line 929 "sdfparse.yy"
    { yyerrok; }
    break;

  case 141:
#line 931 "sdfparse.yy"
    { yyerrok; }
    break;


    }

/* Line 1037 of yacc.c.  */
#line 2411 "sdfparse.cc"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 yydestruct ("Error: popping",
                             yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yydestruct ("Error: discarding lookahead",
              yytoken, &yylval);
  yychar = YYEMPTY;
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 934 "sdfparse.yy"



