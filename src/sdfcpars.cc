/* A Bison parser, made from /home/mdh/sourceforge/veriwell/src/sdfcpars.yy
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	IF	257
# define	ELSE	258
# define	OROR	259
# define	ANDAND	260
# define	ANDANDAND	261
# define	XNOR	262
# define	LOGICAL_EQUALITY	263
# define	LOGICAL_INEQUALITY	264
# define	CASE_EQUALITY	265
# define	CASE_INEQUALITY	266
# define	LT	267
# define	LE	268
# define	GT	269
# define	GE	270
# define	LEFT_SHIFT	271
# define	RIGHT_SHIFT	272
# define	REDUCTION_NOR	273
# define	REDUCTION_NAND	274
# define	UNARY	275
# define	INTEGER	276
# define	SCALAR_CONSTANT	277
# define	FLOATING	278
# define	QSTRING	279
# define	IDENTIFIER	280
# define	IOPATH	281
# define	PORT	282
# define	INTERCONNECT	283
# define	NETDELAY	284
# define	DEVICE	285
# define	SETUP	286
# define	HOLD	287
# define	SETUPHOLD	288
# define	RECOVERY	289
# define	SKEW	290
# define	WIDTH	291
# define	PERIOD	292
# define	NOCHANGE	293
# define	PATHPULSE	294
# define	IGNORE	295
# define	INTERMOD_PATH	296
# define	MIPD	297
# define	CELL	298
# define	USE	299
# define	INTERCONNECT_MIPD	300
# define	MINIMUM	301
# define	MAXIMUM	302
# define	AVERAGE	303
# define	MTM	304
# define	TOOL_CONTROL	305
# define	TYPICAL	306
# define	SCALE_FACTORS	307
# define	SCALE_TYPE	308
# define	FROM_MINIMUM	309
# define	FROM_TYPICAL	310
# define	FROM_MAXIMUM	311
# define	FROM_MTM	312
# define	FROM_FILE	313
# define	TURNOFF_DELAY	314
# define	TURNON_DELAY	315
# define	MODULE	316
# define	MAP_INNER	317
# define	ADD	318
# define	OVERRIDE	319
# define	EOF_	320

#line 22 "sdfcpars.yy"

#define YYERROR_VERBOSE

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "glue.h"
#include "acc_user.h"
#include "sdflex.h"
#include "sdf.h"
#include "sdfclex.h"

#include "sdfcpars.h"
#ifdef __cplusplus
    int	sdfconfig_lex( YYSTYPE* );
#endif // __cplusplus

/* Used in BISON.SIM */
extern char *token_buffer;


#define yymaxdepth sdfconfig_maxdepth
#define yyparse sdfconfig_parse
#define yylex sdfconfig_lex
#define yyerror sdfconfig_error
#define yylval sdfconfig_lval
#define yychar sdfconfig_char
#define yydebug sdfconfig_debug
#define yypact sdfconfig_pact
#define yyr1 sdfconfig_r1
#define yyr2 sdfconfig_r2
#define yydef sdfconfig_def
#define yychk sdfconfig_chk
#define yypgo sdfconfig_pgo
#define yyact sdfconfig_act
#define yyexca sdfconfig_exca
#define yyerrflag sdfconfig_errflag
#define yynerrs sdfconfig_nerrs
#define yyps sdfconfig_ps
#define yypv sdfconfig_pv
#define yys sdfconfig_s
#define yy_yys sdfconfig_yys
#define yystate sdfconfig_state
#define yytmp sdfconfig_tmp
#define yyv sdfconfig_v
#define yy_yyv sdfconfig_yyv
#define yyval sdfconfig_val
#define yylloc sdfconfig_lloc
#define yyreds sdfconfig_reds
#define yytoks sdfconfig_toks
#define yylhs sdfconfig_yylhs
#define yylen sdfconfig_yylen
#define yydefred sdfconfig_yydefred
#define yydgoto sdfconfig_yydgoto
#define yysindex sdfconfig_yysindex
#define yyrindex sdfconfig_yyrindex
#define yygindex sdfconfig_yygindex
#define yytable sdfconfig_yytable
#define yycheck sdfconfig_yycheck
#define yyname sdfconfig_yyname
#define yyrule sdfconfig_yyrule



void initSDFCParse()
{
	sdfclexInit();
}


#line 96 "sdfcpars.yy"
#ifndef YYSTYPE
typedef union {
	int				integer;
	double			floating;
	char			*ptr;
	eScaleType_t	etype;
	eDelayType_t  	dtype;
	eSDFConstruct_t	stype;
	eVerilogConstruct_t vtype;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		73
#define	YYFLAG		-32768
#define	YYNTBASE	83

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 320 ? yytranslate[x] : 94)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,    28,    13,     2,
      29,     2,    26,    24,     2,    25,     2,    27,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,    82,
       2,    81,     2,     5,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    30,     2,     2,    11,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     7,
       8,     9,    12,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    17,    22,    27,
      32,    41,    46,    51,    56,    58,    60,    62,    64,    66,
      68,    70,    72,    74,    76,    78,    80,    82,    84,    86,
      88,    90,    92,    94,    96,    98,   100,   102,   104,   106,
     108,   110,   112,   114,   116,   118
};
static const short yyrhs[] =
{
      84,    80,     0,    80,     0,    85,     0,    85,    84,     0,
      86,     0,    89,    81,    90,    93,     0,    60,    81,    91,
      93,     0,    64,    81,    92,    93,     0,    64,    81,    65,
      93,     0,    67,    81,    88,     6,    88,     6,    88,    93,
       0,    68,    81,    87,    93,     0,    74,    81,    91,    93,
       0,    74,    81,    73,    93,     0,    69,     0,    70,     0,
      71,     0,    72,     0,    38,     0,    36,     0,    41,     0,
      42,     0,    43,     0,    44,     0,    45,     0,    46,     0,
      47,     0,    48,     0,    49,     0,    50,     0,    51,     0,
      52,     0,    53,     0,    54,     0,    55,     0,    56,     0,
      57,     0,    58,     0,    59,     0,    61,     0,    62,     0,
      63,     0,    61,     0,    66,     0,    62,     0,    82,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   188,   189,   193,   194,   198,   202,   208,   212,   216,
     220,   224,   228,   232,   239,   241,   243,   245,   250,   251,
     258,   260,   262,   264,   266,   268,   270,   272,   274,   276,
     278,   280,   282,   284,   289,   291,   293,   295,   297,   302,
     304,   306,   311,   313,   315,   325
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "IF", "ELSE", "'?'", "':'", "OROR", "ANDAND", 
  "ANDANDAND", "'|'", "'^'", "XNOR", "'&'", "LOGICAL_EQUALITY", 
  "LOGICAL_INEQUALITY", "CASE_EQUALITY", "CASE_INEQUALITY", "LT", "LE", 
  "GT", "GE", "LEFT_SHIFT", "RIGHT_SHIFT", "'+'", "'-'", "'*'", "'/'", 
  "'%'", "'('", "'['", "'!'", "'~'", "REDUCTION_NOR", "REDUCTION_NAND", 
  "UNARY", "INTEGER", "SCALAR_CONSTANT", "FLOATING", "QSTRING", 
  "IDENTIFIER", "IOPATH", "PORT", "INTERCONNECT", "NETDELAY", "DEVICE", 
  "SETUP", "HOLD", "SETUPHOLD", "RECOVERY", "SKEW", "WIDTH", "PERIOD", 
  "NOCHANGE", "PATHPULSE", "IGNORE", "INTERMOD_PATH", "MIPD", "CELL", 
  "USE", "INTERCONNECT_MIPD", "MINIMUM", "MAXIMUM", "AVERAGE", "MTM", 
  "TOOL_CONTROL", "TYPICAL", "SCALE_FACTORS", "SCALE_TYPE", 
  "FROM_MINIMUM", "FROM_TYPICAL", "FROM_MAXIMUM", "FROM_MTM", "FROM_FILE", 
  "TURNOFF_DELAY", "TURNON_DELAY", "MODULE", "MAP_INNER", "ADD", 
  "OVERRIDE", "EOF_", "'='", "';'", "config", "statements", "statement", 
  "map", "stype", "real", "sdf_construct", "veritool_map", "min_max_avg", 
  "min_typ_max", "sc", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    83,    83,    84,    84,    85,    86,    86,    86,    86,
      86,    86,    86,    86,    87,    87,    87,    87,    88,    88,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    90,    90,    90,    90,    90,    91,
      91,    91,    92,    92,    92,    93
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     2,     1,     1,     2,     1,     4,     4,     4,     4,
       8,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     0,     0,     0,     0,     0,
       2,     0,     3,     5,     0,     0,     0,     0,     0,     0,
       1,     4,     0,    39,    40,    41,     0,    42,    44,     0,
      43,     0,    19,    18,     0,    14,    15,    16,    17,     0,
       0,     0,    34,    35,    36,    37,    38,     0,    45,     7,
       9,     8,     0,    11,    13,    12,     6,     0,     0,     0,
      10,     0,     0,     0
};

static const short yydefgoto[] =
{
      71,    21,    22,    23,    49,    44,    24,    57,    36,    41,
      59
};

static const short yypact[] =
{
     -41,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   -66,   -60,   -56,   -52,   -49,
  -32768,   -19,    -1,-32768,   -17,   -25,   -31,   -14,   -15,   -45,
  -32768,-32768,    22,-32768,-32768,-32768,   -20,-32768,-32768,   -20,
  -32768,   -20,-32768,-32768,    59,-32768,-32768,-32768,-32768,   -20,
     -20,   -20,-32768,-32768,-32768,-32768,-32768,   -20,-32768,-32768,
  -32768,-32768,   -14,-32768,-32768,-32768,-32768,    65,   -14,   -20,
  -32768,    72,    74,-32768
};

static const short yypgoto[] =
{
  -32768,    53,-32768,-32768,-32768,   -48,-32768,-32768,    54,-32768,
      19
};


#define	YYLAST		88


static const short yytable[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    67,    25,    33,    34,    35,    15,
      69,    26,    42,    16,    43,    27,    17,    18,    50,    28,
      37,    38,    29,    19,    39,    40,    33,    34,    35,    20,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    45,    46,    47,    48,    60,    15,
      61,    30,    58,    16,    32,    62,    17,    18,    63,    64,
      65,    68,    72,    19,    73,    31,    66,    52,    53,    54,
      55,    56,     0,    51,     0,     0,     0,     0,    70
};

static const short yycheck[] =
{
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    62,    81,    61,    62,    63,    60,
      68,    81,    36,    64,    38,    81,    67,    68,    73,    81,
      61,    62,    81,    74,    65,    66,    61,    62,    63,    80,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    69,    70,    71,    72,    39,    60,
      41,    80,    82,    64,    81,     6,    67,    68,    49,    50,
      51,     6,     0,    74,     0,    22,    57,    55,    56,    57,
      58,    59,    -1,    29,    -1,    -1,    -1,    -1,    69
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/home/mdh/sourceforge/bison/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
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
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


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
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


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
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
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

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

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
#endif

#line 315 "/home/mdh/sourceforge/bison/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
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

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 6:
#line 203 "sdfcpars.yy"
{
			if( ! setMap( yyvsp[-3].stype, yyvsp[-1].vtype ) ) {
				sdfconfig_error( "illegal construct mapping" );
			}
		}
    break;
case 7:
#line 209 "sdfcpars.yy"
{
			setInterconnectMipd( yyvsp[-1].dtype );
		}
    break;
case 8:
#line 213 "sdfcpars.yy"
{
			setMTMSpec( yyvsp[-1].dtype );
		}
    break;
case 9:
#line 217 "sdfcpars.yy"
{
			setMTMSpec( eTOOL_CONTROL );
		}
    break;
case 10:
#line 221 "sdfcpars.yy"
{
			setScaleFactors( yyvsp[-5].floating, yyvsp[-3].floating, yyvsp[-1].floating );
		}
    break;
case 11:
#line 225 "sdfcpars.yy"
{
			setScaleType( yyvsp[-1].etype );
		}
    break;
case 12:
#line 229 "sdfcpars.yy"
{
			setTurnOffDelay( yyvsp[-1].dtype );
		}
    break;
case 13:
#line 233 "sdfcpars.yy"
{
			setTurnOffDelay( eFROM_FILE );
		}
    break;
case 14:
#line 240 "sdfcpars.yy"
{ yyval.etype = eFROM_MINIMUM; }
    break;
case 15:
#line 242 "sdfcpars.yy"
{ yyval.etype = eFROM_TYPICAL; }
    break;
case 16:
#line 244 "sdfcpars.yy"
{ yyval.etype = eFROM_MAXIMUM; }
    break;
case 17:
#line 246 "sdfcpars.yy"
{ yyval.etype = eFROM_MTM; }
    break;
case 19:
#line 252 "sdfcpars.yy"
{
			yyval.floating = (double) yyvsp[0].integer;
		}
    break;
case 20:
#line 259 "sdfcpars.yy"
{ yyval.stype = eIOPATH; }
    break;
case 21:
#line 261 "sdfcpars.yy"
{ yyval.stype = ePORT; }
    break;
case 22:
#line 263 "sdfcpars.yy"
{ yyval.stype = eINTERCONNECT; }
    break;
case 23:
#line 265 "sdfcpars.yy"
{ yyval.stype = eNETDELAY; }
    break;
case 24:
#line 267 "sdfcpars.yy"
{ yyval.stype = eDEVICE; }
    break;
case 25:
#line 269 "sdfcpars.yy"
{ yyval.stype = eSETUP; }
    break;
case 26:
#line 271 "sdfcpars.yy"
{ yyval.stype = eHOLD; }
    break;
case 27:
#line 273 "sdfcpars.yy"
{ yyval.stype = eSETUPHOLD; }
    break;
case 28:
#line 275 "sdfcpars.yy"
{ yyval.stype = eRECOVERY; }
    break;
case 29:
#line 277 "sdfcpars.yy"
{ yyval.stype = eSKEW; }
    break;
case 30:
#line 279 "sdfcpars.yy"
{ yyval.stype = eWIDTH; }
    break;
case 31:
#line 281 "sdfcpars.yy"
{ yyval.stype = ePERIOD; }
    break;
case 32:
#line 283 "sdfcpars.yy"
{ yyval.stype = eNOCHANGE; }
    break;
case 33:
#line 285 "sdfcpars.yy"
{ yyval.stype = ePATHPULSE; }
    break;
case 34:
#line 290 "sdfcpars.yy"
{ yyval.vtype = eVIGNORE; }
    break;
case 35:
#line 292 "sdfcpars.yy"
{ yyval.vtype = eVINTERMODPATH; }
    break;
case 36:
#line 294 "sdfcpars.yy"
{ yyval.vtype = eVMIPD; }
    break;
case 37:
#line 296 "sdfcpars.yy"
{ yyval.vtype = eVCELL; }
    break;
case 38:
#line 298 "sdfcpars.yy"
{ yyval.vtype = eVUSE; }
    break;
case 39:
#line 303 "sdfcpars.yy"
{ yyval.dtype = eMINIMUM; }
    break;
case 40:
#line 305 "sdfcpars.yy"
{ yyval.dtype = eMAXIMUM; }
    break;
case 41:
#line 307 "sdfcpars.yy"
{ yyval.dtype = eAVERAGE; }
    break;
case 42:
#line 312 "sdfcpars.yy"
{ yyval.dtype = eMINIMUM; }
    break;
case 43:
#line 314 "sdfcpars.yy"
{ yyval.dtype = eTYPICAL; }
    break;
case 44:
#line 316 "sdfcpars.yy"
{ yyval.dtype = eMAXIMUM; }
    break;
case 45:
#line 326 "sdfcpars.yy"
{ yyerrok; }
    break;
}

#line 705 "/home/mdh/sourceforge/bison/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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
  yyresult = 1;
  goto yyreturn;

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 329 "sdfcpars.yy"


