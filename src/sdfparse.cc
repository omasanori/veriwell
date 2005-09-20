/* A Bison parser, made from /home/mdh/sourceforge/veriwell/src/sdfparse.yy
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
# define	BLOB	281
# define	POSEDGE	282
# define	NEGEDGE	283
# define	DELAYFILE	284
# define	SDFVERSION	285
# define	DESIGN	286
# define	DATE	287
# define	VENDOR	288
# define	PROGRAM	289
# define	VERSION_	290
# define	DIVIDER	291
# define	VOLTAGE	292
# define	PROCESS	293
# define	TEMPERATURE	294
# define	TIMESCALE	295
# define	CELL	296
# define	CELLTYPE	297
# define	INSTANCE	298
# define	DELAY	299
# define	TIMINGCHECK	300
# define	INCLUDE	301
# define	ABSOLUTE	302
# define	INCREMENT	303
# define	USERDEF	304
# define	PATHPULSE	305
# define	GLOBALPATHPULSE	306
# define	IOPATH	307
# define	COND	308
# define	PORT	309
# define	INTERCONNECT	310
# define	NETDELAY	311
# define	DEVICE	312
# define	SETUP	313
# define	HOLD	314
# define	SETUPHOLD	315
# define	RECOVERY	316
# define	SKEW	317
# define	WIDTH	318
# define	PERIOD	319
# define	NOCHANGE	320
# define	PATHCONSTRAINT	321
# define	SUM	322
# define	DIFF	323
# define	SKEWCONSTRAINT	324
# define	MICROSECOND	325
# define	NANOSECOND	326
# define	PICOSECOND	327
# define	EDGE_01	328
# define	EDGE_10	329
# define	EDGE_0z	330
# define	EDGE_1z	331
# define	EDGE_z0	332
# define	EDGE_z1	333
# define	EOF_	334

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


#line 93 "sdfparse.yy"
#ifndef YYSTYPE
typedef union {
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
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		376
#define	YYFLAG		-32768
#define	YYNTBASE	97

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 334 ? yytranslate[x] : 160)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     7,
       8,     9,    12,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     3,    19,    24,    29,    34,    39,    44,    49,
      50,    56,    61,    65,    70,    75,    79,    85,    91,    97,
     101,   103,   106,   108,   109,   110,   119,   124,   128,   131,
     135,   139,   143,   145,   149,   150,   154,   155,   160,   161,
     166,   169,   171,   177,   179,   180,   187,   194,   201,   208,
     215,   221,   227,   234,   237,   239,   240,   246,   247,   253,
     256,   258,   261,   263,   265,   267,   269,   272,   274,   276,
     279,   286,   297,   303,   310,   316,   321,   327,   328,   335,
     346,   352,   359,   365,   370,   376,   378,   379,   383,   385,
     387,   392,   399,   403,   407,   411,   415,   419,   423,   427,
     431,   435,   437,   440,   444,   448,   452,   454,   458,   465,
     475,   488,   504,   523,   527,   534,   544,   557,   573,   592,
     598,   604,   610,   611,   617,   623,   629,   630,   632,   635,
     638,   640,   642,   644,   645,   647,   648,   650,   651,   653,
     655
};
static const short yyrhs[] =
{
      98,    94,     0,    29,    44,    99,   100,   101,   102,   103,
     104,   105,   107,   108,   109,   110,   112,   158,     0,    29,
      45,   157,   158,     0,    29,    46,   157,   158,     0,    29,
      47,   157,   158,     0,    29,    48,   157,   158,     0,    29,
      49,   157,   158,     0,    29,    50,   157,   158,     0,     0,
      29,    51,   106,    41,   158,     0,    29,    52,   151,   158,
       0,    29,    52,   158,     0,    29,    53,   157,   158,     0,
      29,    54,   151,   158,     0,    29,    54,   158,     0,    29,
      55,   111,    86,   158,     0,    29,    55,   111,    85,   158,
       0,    29,    55,   111,    87,   158,     0,    29,    55,   158,
       0,    36,     0,   113,   112,     0,   113,     0,     0,     0,
      29,    56,   114,   115,    29,   116,   119,   158,     0,    29,
      57,    39,   158,     0,   117,    29,   116,     0,   117,    29,
       0,    58,    40,   158,     0,    58,    26,   158,     0,    58,
      40,   158,     0,   120,     0,   120,    29,   119,     0,     0,
      59,   127,   158,     0,     0,    60,   121,   123,   158,     0,
       0,    61,   122,    41,   158,     0,   124,   123,     0,   124,
       0,    29,    68,   125,   126,   158,     0,   126,     0,     0,
      29,    73,   142,   140,   147,   158,     0,    29,    74,   142,
     140,   147,   158,     0,    29,    75,   142,   140,   146,   158,
       0,    29,    76,   140,   140,   147,   158,     0,    29,    77,
     140,   140,   147,   158,     0,    29,    78,   140,   147,   158,
       0,    29,    79,   140,   147,   158,     0,    29,    80,   140,
     142,   146,   158,     0,   128,   127,     0,   128,     0,     0,
      29,    62,   129,   131,   158,     0,     0,    29,    63,   130,
     132,   158,     0,   137,   131,     0,   137,     0,   139,   139,
       0,   139,     0,   136,     0,   134,     0,   135,     0,   118,
     135,     0,    40,     0,   142,     0,   118,   142,     0,    29,
      67,   140,   142,   145,   158,     0,    29,    68,   138,    29,
      67,   140,   142,   145,   158,   158,     0,    29,    69,   142,
     145,   158,     0,    29,    70,   136,   136,   145,   158,     0,
      29,    71,   133,   145,   158,     0,    29,    72,   145,   158,
       0,    29,    72,   136,   145,   158,     0,     0,    29,    67,
     140,   142,   148,   158,     0,    29,    68,   138,    29,    67,
     140,   142,   148,   158,   158,     0,    29,    69,   142,   148,
     158,     0,    29,    70,   136,   136,   148,   158,     0,    29,
      71,   133,   148,   158,     0,    29,    72,   145,   158,     0,
      29,    72,   136,   148,   158,     0,   142,     0,     0,    29,
     141,   144,     0,   143,     0,    40,     0,    40,    30,    36,
     159,     0,    40,    30,    36,     6,    36,   159,     0,    42,
     142,   158,     0,    43,   142,   158,     0,    88,   142,   158,
       0,    89,   142,   158,     0,    93,   142,   158,     0,    92,
     142,   158,     0,    91,   142,   158,     0,    90,   142,   158,
       0,    29,   154,   158,     0,   149,     0,   147,   147,     0,
      29,   153,   158,     0,    29,   151,   158,     0,    29,   153,
     158,     0,   150,     0,    29,   152,   158,     0,    29,   152,
     158,    29,   152,   158,     0,    29,   152,   158,    29,   152,
     158,    29,   152,   158,     0,    29,   152,   158,    29,   152,
     158,    29,   152,   158,    29,   152,   158,     0,    29,   152,
     158,    29,   152,   158,    29,   152,   158,    29,   152,   158,
      29,   152,   158,     0,    29,   152,   158,    29,   152,   158,
      29,   152,   158,    29,   152,   158,    29,   152,   158,    29,
     152,   158,     0,    29,   151,   158,     0,    29,   151,   158,
      29,   151,   158,     0,    29,   151,   158,    29,   151,   158,
      29,   151,   158,     0,    29,   151,   158,    29,   151,   158,
      29,   151,   158,    29,   151,   158,     0,    29,   151,   158,
      29,   151,   158,    29,   151,   158,    29,   151,   158,    29,
     151,   158,     0,    29,   151,   158,    29,   151,   158,    29,
     151,   158,    29,   151,   158,    29,   151,   158,    29,   151,
     158,     0,   153,     6,   155,     6,   155,     0,   155,     6,
     153,     6,   155,     0,   155,     6,   155,     6,   153,     0,
       0,   154,     6,   156,     6,   156,     0,   156,     6,   154,
       6,   156,     0,   156,     6,   156,     6,   154,     0,     0,
     154,     0,    24,   154,     0,    25,   154,     0,    38,     0,
      36,     0,   153,     0,     0,   154,     0,     0,    39,     0,
       0,    95,     0,    96,     0,     1,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   222,   226,   233,   238,   243,   248,   253,   258,   263,
     263,   268,   270,   275,   280,   282,   287,   292,   297,   302,
     310,   329,   330,   331,   334,   334,   345,   353,   354,   358,
     363,   371,   378,   379,   382,   383,   384,   384,   385,   385,
     392,   393,   397,   400,   405,   409,   413,   417,   421,   425,
     429,   433,   437,   444,   445,   449,   449,   455,   455,   464,
     465,   469,   470,   474,   475,   479,   480,   488,   498,   499,
     508,   512,   517,   521,   525,   529,   533,   540,   544,   548,
     553,   557,   561,   565,   569,   575,   576,   576,   583,   587,
     594,   603,   615,   620,   625,   630,   635,   640,   645,   650,
     659,   670,   674,   683,   694,   702,   713,   717,   722,   728,
     735,   744,   754,   769,   774,   780,   787,   796,   806,   820,
     827,   834,   841,   850,   857,   864,   871,   880,   881,   883,
     887,   888,   893,   898,   903,   908,   913,   914,   923,   928,
     930
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
  "IDENTIFIER", "BLOB", "POSEDGE", "NEGEDGE", "DELAYFILE", "SDFVERSION", 
  "DESIGN", "DATE", "VENDOR", "PROGRAM", "VERSION_", "DIVIDER", "VOLTAGE", 
  "PROCESS", "TEMPERATURE", "TIMESCALE", "CELL", "CELLTYPE", "INSTANCE", 
  "DELAY", "TIMINGCHECK", "INCLUDE", "ABSOLUTE", "INCREMENT", "USERDEF", 
  "PATHPULSE", "GLOBALPATHPULSE", "IOPATH", "COND", "PORT", 
  "INTERCONNECT", "NETDELAY", "DEVICE", "SETUP", "HOLD", "SETUPHOLD", 
  "RECOVERY", "SKEW", "WIDTH", "PERIOD", "NOCHANGE", "PATHCONSTRAINT", 
  "SUM", "DIFF", "SKEWCONSTRAINT", "MICROSECOND", "NANOSECOND", 
  "PICOSECOND", "EDGE_01", "EDGE_10", "EDGE_0z", "EDGE_1z", "EDGE_z0", 
  "EDGE_z1", "EOF_", "')'", "']'", "sdf", "delay_file", "sdf_version", 
  "design_name", "date", "vendor", "program_name", "program_version", 
  "hierarchy_divider", "@1", "voltage", "process", "temperature", 
  "time_scale", "ts", "cells", "cell", "@2", "celltype", "instances", 
  "instance", "string_instance", "timing_specs", "timing_spec", "@3", 
  "@4", "tcdefs", "tcdef", "timing_check_condition", "tcheck", "deltypes", 
  "deltype", "@5", "@6", "absvals", "incvals", "name", "net_instance", 
  "net", "port_instance", "absval", "conditional_port_expr", "incval", 
  "port_spec", "@7", "port_path", "port", "port_edge", "value", 
  "double_tc_rvalue", "tc_rvalue", "rvalue", "exp_list", "rexp_list", 
  "rexpression", "expression", "real", "preal", "creal", "cpreal", 
  "nstring", "rp", "rb", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    97,    98,    99,   100,   101,   102,   103,   104,   106,
     105,   107,   107,   108,   109,   109,   110,   110,   110,   110,
     111,   112,   112,   112,   114,   113,   115,   116,   116,   117,
     117,   118,   119,   119,   120,   120,   121,   120,   122,   120,
     123,   123,   124,   124,   125,   126,   126,   126,   126,   126,
     126,   126,   126,   127,   127,   129,   128,   130,   128,   131,
     131,   132,   132,   133,   133,   134,   134,   135,   136,   136,
     137,   137,   137,   137,   137,   137,   137,   138,   139,   139,
     139,   139,   139,   139,   139,   140,   141,   140,   142,   143,
     143,   143,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   145,   146,   147,   147,   148,   148,   149,   149,   149,
     149,   149,   149,   150,   150,   150,   150,   150,   150,   151,
     151,   151,   151,   152,   152,   152,   152,   153,   153,   153,
     154,   154,   155,   155,   156,   156,   157,   157,   158,   159,
     159
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     2,    15,     4,     4,     4,     4,     4,     4,     0,
       5,     4,     3,     4,     4,     3,     5,     5,     5,     3,
       1,     2,     1,     0,     0,     8,     4,     3,     2,     3,
       3,     3,     1,     3,     0,     3,     0,     4,     0,     4,
       2,     1,     5,     1,     0,     6,     6,     6,     6,     6,
       5,     5,     6,     2,     1,     0,     5,     0,     5,     2,
       1,     2,     1,     1,     1,     1,     2,     1,     1,     2,
       6,    10,     5,     6,     5,     4,     5,     0,     6,    10,
       5,     6,     5,     4,     5,     1,     0,     3,     1,     1,
       4,     6,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     3,     3,     3,     1,     3,     6,     9,
      12,    15,    18,     3,     6,     9,    12,    15,    18,     5,
       5,     5,     0,     5,     5,     5,     0,     1,     2,     2,
       1,     1,     1,     0,     1,     0,     1,     0,     1,     1,
       1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,   137,     0,     0,
     136,     0,   137,     0,     0,   138,     3,     0,   137,     0,
       0,     4,     0,   137,     0,     0,     5,     0,   137,     0,
       0,     6,     0,   137,     0,     0,     7,     0,     9,     0,
       0,     8,     0,   133,     0,     0,     0,     0,     0,   131,
     130,     0,     0,   127,     0,    12,   137,     0,     0,    10,
     128,   129,    11,   133,   133,     0,   133,     0,    23,   132,
       0,     0,     0,    13,     0,    15,     0,     0,     0,    22,
     133,   133,     0,    14,    20,     0,    19,    24,     2,    21,
     119,   120,   121,     0,     0,     0,     0,    17,    16,    18,
       0,     0,     0,     0,     0,     0,    34,     0,    26,     0,
       0,     0,    36,    38,     0,    32,    28,    30,    29,     0,
       0,    54,     0,     0,    25,    34,    27,    55,    57,    35,
      53,     0,     0,    41,    43,     0,    33,     0,     0,    44,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    40,
      39,     0,     0,    60,     0,     0,    62,     0,    89,     0,
      88,     0,     0,    86,     0,    85,     0,     0,     0,     0,
       0,    77,     0,     0,     0,     0,    56,    59,     0,    77,
       0,     0,     0,     0,    58,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,    67,     0,     0,    64,
      65,    63,   126,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,    69,
       0,    66,     0,     0,     0,     0,     0,    75,     0,     0,
     122,     0,   106,     0,     0,     0,    83,   140,     0,   139,
      90,    45,    46,    47,   102,     0,     0,     0,     0,     0,
       0,     0,     0,    48,    49,   104,   103,    52,     0,     0,
      72,    31,     0,    74,   107,   135,   100,   135,    76,     0,
       0,     0,     0,    80,     0,    82,    84,     0,    92,    93,
      94,    95,    99,    98,    97,    96,    70,     0,    73,   126,
     134,     0,     0,     0,    78,     0,   113,   105,    81,    91,
       0,     0,     0,   135,   135,     0,     0,   122,     0,   108,
     123,   124,   125,     0,     0,     0,   126,     0,   114,    71,
       0,    79,   122,   109,     0,   126,   115,     0,   122,   110,
       0,   126,   116,     0,   122,   111,     0,   126,   117,     0,
     122,   112,     0,   118,     0,     0,     0
};

static const short yydefgoto[] =
{
     374,     2,     6,     9,    14,    20,    25,    30,    35,    42,
      40,    45,    58,    68,    85,    78,    79,    96,   101,   106,
     107,   203,   114,   115,   122,   123,   132,   133,   157,   134,
     120,   121,   137,   138,   152,   155,   208,   209,   210,   211,
     153,   200,   156,   164,   192,   165,   160,   237,   214,   227,
     228,   261,   215,   262,    51,   253,    52,    53,    54,   255,
      11,    16,   270
};

static const short yypact[] =
{
      -2,   -13,   -58,    23,-32768,     8,    35,    30,    39,    62,
  -32768,    19,    30,    63,    87,-32768,-32768,    19,    30,    69,
      90,-32768,    19,    30,    71,    94,-32768,    19,    30,    76,
     102,-32768,    19,    30,    81,   105,-32768,    19,-32768,    85,
     111,-32768,   113,    -6,    93,   123,    19,    52,    52,-32768,
  -32768,    19,   157,-32768,   159,-32768,    30,   112,   140,-32768,
  -32768,-32768,-32768,   103,   103,    19,    -6,   116,   143,-32768,
     171,   172,   175,-32768,    19,-32768,   -32,   133,    19,   143,
     103,   103,   103,-32768,-32768,    64,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    19,    19,    19,   163,-32768,-32768,-32768,
     146,   177,   174,   156,    19,    54,   124,   186,-32768,    19,
      19,   187,-32768,-32768,    19,   188,   156,-32768,-32768,   -51,
      19,   187,   190,   179,-32768,   124,-32768,-32768,-32768,-32768,
  -32768,   283,    19,   190,-32768,    19,-32768,   197,   203,-32768,
     193,   193,   193,     4,     4,     4,     4,     4,-32768,-32768,
  -32768,   305,    19,   197,   311,    19,   203,   205,   208,     4,
  -32768,     4,     4,-32768,     4,-32768,     4,   212,   212,   193,
       4,-32768,   193,    -3,    46,    25,-32768,-32768,     4,-32768,
     193,    -3,    46,    25,-32768,-32768,   291,    19,   206,   212,
     212,   212,   -18,   212,   212,   162,    19,    19,   212,   193,
     216,   217,   209,   193,    -3,-32768,   208,   213,   217,-32768,
  -32768,-32768,    22,   217,    19,-32768,   193,   225,   226,    -3,
     226,   226,    19,-32768,     0,    19,    19,    19,   212,   193,
     193,   193,   193,   193,   193,   193,   193,-32768,    19,    19,
      19,     7,-32768,-32768,    19,   217,   189,    19,    19,-32768,
     217,-32768,    19,    19,    17,   251,    19,-32768,   226,   199,
     162,    19,-32768,   226,    19,    19,-32768,-32768,   231,-32768,
  -32768,-32768,-32768,-32768,-32768,    19,    19,    19,    19,    19,
      19,    19,    19,-32768,-32768,-32768,-32768,-32768,    19,     4,
  -32768,-32768,    19,-32768,   239,    52,-32768,    52,-32768,    19,
       4,    19,     7,-32768,    19,-32768,-32768,    15,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   193,-32768,    22,
  -32768,   263,   264,   266,-32768,   193,   244,-32768,-32768,-32768,
     217,    19,   270,    52,    52,    52,   226,   162,    19,   248,
  -32768,-32768,-32768,    19,    19,    19,    22,    19,   250,-32768,
      19,-32768,   162,   252,    19,    22,   254,    19,   162,   257,
      19,    22,   265,    19,   162,   267,    19,    22,   269,    19,
     162,-32768,    19,-32768,   280,   292,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   221,-32768,-32768,-32768,   192,
  -32768,   -69,   170,-32768,-32768,-32768,   173,-32768,-32768,   147,
     182,-32768,-32768,-32768,   160,-32768,   127,-32768,   108,    21,
  -32768,   138,   164,    29,-32768,   -74,-32768,-32768,  -166,   121,
     -46,  -212,-32768,-32768,   -59,  -305,   -42,   -45,   148,  -252,
     279,   -17,    16
};


#define	YYLAST		383


static const short yytable[] =
{
      21,   267,    60,    61,    84,    26,   268,    74,   264,   265,
      31,   127,   128,    63,   331,    36,   267,   222,    47,    48,
      41,    69,    71,   295,   229,   230,    55,     1,  -135,    59,
      49,     3,    50,   163,    62,   247,     4,   158,    69,    69,
      92,   350,   252,   321,   158,   323,   299,   256,    73,    75,
     357,   304,     5,     7,   212,   202,   363,    83,    49,    86,
      50,    88,   369,    15,     8,   158,   159,   161,   162,    10,
     231,   232,   233,   234,   235,   236,    97,    98,    99,   288,
     109,   340,   341,   202,   292,    12,   206,   108,    49,    15,
      50,    13,   117,   118,   110,   198,   269,   124,   201,   205,
     205,   205,    15,   129,   202,   207,   218,   205,   205,   205,
      18,   269,    15,   207,    15,   148,    19,    23,   150,    24,
      28,   196,   197,    29,   343,   245,    33,    47,    48,   249,
     205,    34,    38,   249,    39,   176,   240,    43,   184,    49,
      44,    50,   258,   225,   226,   205,    56,   238,   239,    93,
      94,    95,    57,   241,    46,   275,   276,   277,   278,   279,
     280,   281,   282,    63,   338,    64,    66,   254,  -133,    67,
     223,    76,    77,   166,   167,   168,   169,    80,    81,   242,
     243,    82,   274,   111,   112,   113,    47,    48,   189,    87,
     190,   191,   100,   193,   204,   194,   213,   257,    49,   199,
      50,   301,   219,   102,   221,   266,   103,   216,   271,   272,
     273,    70,    72,   104,   105,   116,   119,   125,   302,   131,
     135,   283,   284,   285,   286,   250,   151,   287,    90,    91,
     290,   291,   154,   158,   186,   293,   294,   296,   188,   298,
     263,   195,   224,   330,   303,   246,   212,   305,   306,   248,
     320,   336,   322,   206,   259,   260,   289,   297,   308,   309,
     310,   311,   312,   313,   314,   315,   300,   307,   319,   333,
     334,   316,   335,   337,   332,   318,   295,   346,   344,   352,
     375,   355,   324,   358,   326,   327,   361,   328,   320,   320,
     342,    17,   376,   354,   364,   136,   367,    22,   370,   360,
      89,   332,    27,   130,   187,   366,   149,    32,   126,   220,
     332,   372,    37,   177,   339,   251,   332,   217,   317,   244,
     185,   345,   332,   329,     0,     0,   347,   348,   349,   325,
     351,     0,     0,   353,     0,    65,     0,   356,     0,     0,
     359,     0,     0,   362,     0,     0,   365,     0,     0,   368,
       0,   139,   371,     0,     0,   373,   140,   141,   142,   143,
     144,   145,   146,   147,   140,   141,   142,   143,   144,   145,
     146,   147,   170,   171,   172,   173,   174,   175,   178,   179,
     180,   181,   182,   183
};

static const short yycheck[] =
{
      17,     1,    47,    48,    36,    22,     6,    66,   220,   221,
      27,    62,    63,     6,   319,    32,     1,   183,    24,    25,
      37,    63,    64,     6,    42,    43,    43,    29,     6,    46,
      36,    44,    38,    29,    51,   201,    94,    40,    80,    81,
      82,   346,   208,   295,    40,   297,   258,   213,    65,    66,
     355,   263,    29,    45,    29,    58,   361,    74,    36,    76,
      38,    78,   367,    95,    29,    40,   140,   141,   142,    39,
      88,    89,    90,    91,    92,    93,    93,    94,    95,   245,
      26,   333,   334,    58,   250,    46,    40,   104,    36,    95,
      38,    29,   109,   110,    40,   169,    96,   114,   172,   173,
     174,   175,    95,   120,    58,   174,   180,   181,   182,   183,
      47,    96,    95,   182,    95,   132,    29,    48,   135,    29,
      49,   167,   168,    29,   336,   199,    50,    24,    25,   203,
     204,    29,    51,   207,    29,   152,   195,    52,   155,    36,
      29,    38,   216,   189,   190,   219,    53,   193,   194,    85,
      86,    87,    29,   195,    41,   229,   230,   231,   232,   233,
     234,   235,   236,     6,   330,     6,    54,   212,     6,    29,
     187,    55,    29,   144,   145,   146,   147,     6,     6,   196,
     197,     6,   228,    59,    60,    61,    24,    25,   159,    56,
     161,   162,    29,   164,   173,   166,   175,   214,    36,   170,
      38,   260,   181,    57,   183,   222,    29,   178,   225,   226,
     227,    63,    64,    39,    58,    29,    29,    29,   260,    29,
      41,   238,   239,   240,   241,   204,    29,   244,    80,    81,
     247,   248,    29,    40,    29,   252,   253,   254,    30,   256,
     219,    29,    36,   317,   261,    29,    29,   264,   265,    40,
     295,   325,   297,    40,    29,    29,    67,     6,   275,   276,
     277,   278,   279,   280,   281,   282,    67,    36,    29,     6,
       6,   288,     6,    29,   319,   292,     6,    29,   337,    29,
       0,    29,   299,    29,   301,   302,    29,   304,   333,   334,
     335,    12,     0,   352,    29,   125,    29,    18,    29,   358,
      79,   346,    23,   121,   157,   364,   133,    28,   116,   182,
     355,   370,    33,   153,   331,   207,   361,   179,   289,   198,
     156,   338,   367,   307,    -1,    -1,   343,   344,   345,   300,
     347,    -1,    -1,   350,    -1,    56,    -1,   354,    -1,    -1,
     357,    -1,    -1,   360,    -1,    -1,   363,    -1,    -1,   366,
      -1,    68,   369,    -1,    -1,   372,    73,    74,    75,    76,
      77,    78,    79,    80,    73,    74,    75,    76,    77,    78,
      79,    80,    67,    68,    69,    70,    71,    72,    67,    68,
      69,    70,    71,    72
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

case 2:
#line 229 "sdfparse.yy"
{}
    break;
case 3:
#line 234 "sdfparse.yy"
{}
    break;
case 4:
#line 239 "sdfparse.yy"
{}
    break;
case 5:
#line 244 "sdfparse.yy"
{}
    break;
case 6:
#line 249 "sdfparse.yy"
{}
    break;
case 7:
#line 254 "sdfparse.yy"
{}
    break;
case 8:
#line 259 "sdfparse.yy"
{}
    break;
case 9:
#line 263 "sdfparse.yy"
{ setParseBlob(); }
    break;
case 10:
#line 264 "sdfparse.yy"
{}
    break;
case 11:
#line 269 "sdfparse.yy"
{}
    break;
case 12:
#line 271 "sdfparse.yy"
{}
    break;
case 13:
#line 276 "sdfparse.yy"
{}
    break;
case 14:
#line 281 "sdfparse.yy"
{}
    break;
case 15:
#line 283 "sdfparse.yy"
{}
    break;
case 16:
#line 288 "sdfparse.yy"
{
			/* set time scale for back annotation */
			setTimeScale( yyvsp[-2].integer - 9 );
		}
    break;
case 17:
#line 293 "sdfparse.yy"
{
			/* set time scale for back annotation */
			setTimeScale( yyvsp[-2].integer - 6 );
		}
    break;
case 18:
#line 298 "sdfparse.yy"
{
			/* set time scale for back annotation */
			setTimeScale( yyvsp[-2].integer - 12 );
		}
    break;
case 19:
#line 303 "sdfparse.yy"
{
			/* set default (1ns) time scale for back annotation */
			setTimeScale( -9 );
		}
    break;
case 20:
#line 311 "sdfparse.yy"
{
			switch(	yyvsp[0].integer ) {
				case 1:
					yyval.integer = 0;
					break;
				case 10:
					yyval.integer = 1;
					break;
				case 100:
					yyval.integer = 2;
					break;
				default:
					sdf_error( "Timescale multiplier must be either 1,10, or 100" );
					yyval.integer = 0;
			} 
		}
    break;
case 24:
#line 335 "sdfparse.yy"
{
			/* reset current instance */
			resetInstance();
			/* free all allocated strings */
			purgeStrings();
		}
    break;
case 26:
#line 346 "sdfparse.yy"
{	
			/* set current cell type */
			setCellType( yyvsp[-1].ptr );
		}
    break;
case 29:
#line 359 "sdfparse.yy"
{
			/* set down thru the hierarchy */
			findInstance( yyvsp[-1].ptr );
		}
    break;
case 30:
#line 364 "sdfparse.yy"
{
			/* this is a wild card  */
			findInstance( "*" );
		}
    break;
case 31:
#line 372 "sdfparse.yy"
{
			yyval.ptr = yyvsp[-1].ptr;
		}
    break;
case 36:
#line 384 "sdfparse.yy"
{setAbsoluteDelay(TRUE);}
    break;
case 38:
#line 385 "sdfparse.yy"
{ setParseBlob(); }
    break;
case 39:
#line 386 "sdfparse.yy"
{
			sdflexOpenFile( yyvsp[-1].ptr );
		}
    break;
case 42:
#line 399 "sdfparse.yy"
{}
    break;
case 43:
#line 401 "sdfparse.yy"
{}
    break;
case 45:
#line 410 "sdfparse.yy"
{
			setConstraint( eSETUP, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 46:
#line 414 "sdfparse.yy"
{
			setConstraint( eHOLD, &yyvsp[-2].port, &yyvsp[-3].port, yyvsp[-1].elist );
		}
    break;
case 47:
#line 418 "sdfparse.yy"
{
			setConstraint( eSETUPHOLD, &yyvsp[-2].port, &yyvsp[-3].port, yyvsp[-1].elist );
		}
    break;
case 48:
#line 422 "sdfparse.yy"
{
			setConstraint( eRECOVERY, &yyvsp[-2].port, &yyvsp[-3].port, yyvsp[-1].elist );
		}
    break;
case 49:
#line 426 "sdfparse.yy"
{
			setConstraint( eSKEW, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 50:
#line 430 "sdfparse.yy"
{
			setConstraint( eWIDTH, &yyvsp[-2].port, NULL, yyvsp[-1].elist );
		}
    break;
case 51:
#line 434 "sdfparse.yy"
{
			setConstraint( ePERIOD, &yyvsp[-2].port, NULL, yyvsp[-1].elist );
		}
    break;
case 52:
#line 438 "sdfparse.yy"
{
			setConstraint( eNOCHANGE, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 55:
#line 450 "sdfparse.yy"
{
			/* mark delay type as absolute */
			setAbsoluteDelay( TRUE );
		}
    break;
case 57:
#line 456 "sdfparse.yy"
{
			/* mark delay type as incremental */
			setAbsoluteDelay( FALSE );
		}
    break;
case 66:
#line 481 "sdfparse.yy"
{
			yyval.port = yyvsp[0].port;
			yyval.port.path = yyvsp[-1].ptr;
		}
    break;
case 67:
#line 489 "sdfparse.yy"
{
			yyval.port.path = NULL;
			yyval.port.name = yyvsp[0].ptr;
			yyval.port.scalar = TRUE;
			yyval.port.edge = accNoEdge;
		}
    break;
case 69:
#line 500 "sdfparse.yy"
{
			yyval.port = yyvsp[0].port;
			yyval.port.path = yyvsp[-1].ptr;
		}
    break;
case 70:
#line 509 "sdfparse.yy"
{
			setConstraint( eIOPATH, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 71:
#line 513 "sdfparse.yy"
{
			/* -------- support for COND needs to be added ----------- */
			setConstraint( eIOPATH, &yyvsp[-4].port, &yyvsp[-3].port, yyvsp[-2].elist );
		}
    break;
case 72:
#line 518 "sdfparse.yy"
{
			setConstraint( ePORT, &yyvsp[-2].port, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 73:
#line 522 "sdfparse.yy"
{
			setConstraint( eINTERCONNECT, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 74:
#line 526 "sdfparse.yy"
{
			setConstraint( eNETDELAY, &yyvsp[-2].port, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 75:
#line 530 "sdfparse.yy"
{
			setConstraint( eDEVICE, (port_t*)NULL, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 76:
#line 534 "sdfparse.yy"
{
			setConstraint( eDEVICE, &yyvsp[-2].port, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 78:
#line 545 "sdfparse.yy"
{
			setConstraint( eIOPATH, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 79:
#line 549 "sdfparse.yy"
{
			/* -------- support for COND needs to be added ----------- */
			setConstraint( eIOPATH, &yyvsp[-4].port, &yyvsp[-3].port, yyvsp[-2].elist );
		}
    break;
case 80:
#line 554 "sdfparse.yy"
{
			setConstraint( ePORT, &yyvsp[-2].port, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 81:
#line 558 "sdfparse.yy"
{
			setConstraint( eINTERCONNECT, &yyvsp[-3].port, &yyvsp[-2].port, yyvsp[-1].elist );
		}
    break;
case 82:
#line 562 "sdfparse.yy"
{
			setConstraint( eNETDELAY, &yyvsp[-2].port, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 83:
#line 566 "sdfparse.yy"
{
			setConstraint( eDEVICE, (port_t*)NULL, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 84:
#line 570 "sdfparse.yy"
{
			setConstraint( eDEVICE, &yyvsp[-2].port, (port_t*)NULL, yyvsp[-1].elist );
		}
    break;
case 86:
#line 576 "sdfparse.yy"
{sdfEnableEdges( TRUE ); }
    break;
case 87:
#line 577 "sdfparse.yy"
{ 
			sdfEnableEdges( FALSE );
			yyval.port = yyvsp[0].port;
		}
    break;
case 89:
#line 588 "sdfparse.yy"
{
			yyval.port.path = NULL;
			yyval.port.name = yyvsp[0].ptr;
			yyval.port.scalar = TRUE;
			yyval.port.edge = accNoEdge;
		}
    break;
case 90:
#line 595 "sdfparse.yy"
{
			yyval.port.path = NULL;
			yyval.port.name = yyvsp[-3].ptr;
			yyval.port.scalar = FALSE;
			yyval.port.msb = yyvsp[-1].integer;
			yyval.port.lsb = yyvsp[-1].integer;
			yyval.port.edge = accNoEdge;
		}
    break;
case 91:
#line 604 "sdfparse.yy"
{
			yyval.port.path = NULL;
			yyval.port.name = yyvsp[-5].ptr;
			yyval.port.scalar = FALSE;
			yyval.port.msb = yyvsp[-3].integer;
			yyval.port.lsb = yyvsp[-1].integer;
			yyval.port.edge = accNoedge;
		}
    break;
case 92:
#line 616 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accPosedge;
		}
    break;
case 93:
#line 621 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accNegedge;
		}
    break;
case 94:
#line 626 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accEdge01;
		}
    break;
case 95:
#line 631 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accEdge10;
		}
    break;
case 96:
#line 636 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accEdgex1;
		}
    break;
case 97:
#line 641 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accEdgex0;
		}
    break;
case 98:
#line 646 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accEdge1x;
		}
    break;
case 99:
#line 651 "sdfparse.yy"
{
			yyval.port = yyvsp[-1].port;
			yyval.port.edge = accEdge0x;
		}
    break;
case 100:
#line 660 "sdfparse.yy"
{
			memset( &yyval.elist, 0, sizeof(yyval.elist) );
			yyval.elist.values[0].triple[0].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[0].valid = TRUE;
			yyval.elist.values[0].triple[1].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[1].valid = TRUE;
			yyval.elist.values[0].triple[2].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[2].valid = TRUE;
			yyval.elist.argCount = 1;
		}
    break;
case 102:
#line 675 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-1].elist.values[0];
			yyval.elist.values[1] = yyvsp[0].elist.values[0];
			yyval.elist.argCount = 2;
		}
    break;
case 103:
#line 684 "sdfparse.yy"
{
			memset( &yyval.elist, 0, sizeof(yyval.elist) );
			yyval.elist.values[0].triple[0].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[0].valid = TRUE;
			yyval.elist.values[0].triple[1].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[1].valid = TRUE;
			yyval.elist.values[0].triple[2].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[2].valid = TRUE;
			yyval.elist.argCount = 1;
		}
    break;
case 104:
#line 695 "sdfparse.yy"
{ 
			yyval.elist.values[0] = yyvsp[-1].triple;
			yyval.elist.argCount = 1;
		}
    break;
case 105:
#line 703 "sdfparse.yy"
{
			memset( &yyval.elist, 0, sizeof(yyval.elist) );
			yyval.elist.values[0].triple[0].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[0].valid = TRUE;
			yyval.elist.values[0].triple[1].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[1].valid = TRUE;
			yyval.elist.values[0].triple[2].value = yyvsp[-1].floating;
			yyval.elist.values[0].triple[2].valid = TRUE;
			yyval.elist.argCount = 1;
		}
    break;
case 107:
#line 718 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-1].triple;
			yyval.elist.argCount = 1;
		}
    break;
case 108:
#line 723 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-4].triple;
			yyval.elist.values[1] = yyvsp[-1].triple;
			yyval.elist.argCount = 2;
		}
    break;
case 109:
#line 729 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-7].triple;
			yyval.elist.values[1] = yyvsp[-4].triple;
			yyval.elist.values[2] = yyvsp[-1].triple;
			yyval.elist.argCount = 3;
		}
    break;
case 110:
#line 737 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-10].triple;
			yyval.elist.values[1] = yyvsp[-7].triple;
			yyval.elist.values[2] = yyvsp[-4].triple;
			yyval.elist.values[3] = yyvsp[-1].triple;
			yyval.elist.argCount = 4;
		}
    break;
case 111:
#line 746 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-13].triple;
			yyval.elist.values[1] = yyvsp[-10].triple;
			yyval.elist.values[2] = yyvsp[-7].triple;
			yyval.elist.values[3] = yyvsp[-4].triple;
			yyval.elist.values[4] = yyvsp[-1].triple;
			yyval.elist.argCount = 5;
		}
    break;
case 112:
#line 756 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-16].triple;
			yyval.elist.values[1] = yyvsp[-13].triple;
			yyval.elist.values[2] = yyvsp[-10].triple;
			yyval.elist.values[3] = yyvsp[-7].triple;
			yyval.elist.values[4] = yyvsp[-4].triple;
			yyval.elist.values[5] = yyvsp[-1].triple;
			yyval.elist.argCount = 6;
		}
    break;
case 113:
#line 770 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-1].triple;
			yyval.elist.argCount = 1;
		}
    break;
case 114:
#line 775 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-4].triple;
			yyval.elist.values[1] = yyvsp[-1].triple;
			yyval.elist.argCount = 2;
		}
    break;
case 115:
#line 781 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-7].triple;
			yyval.elist.values[1] = yyvsp[-4].triple;
			yyval.elist.values[2] = yyvsp[-1].triple;
			yyval.elist.argCount = 3;
		}
    break;
case 116:
#line 789 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-10].triple;
			yyval.elist.values[1] = yyvsp[-7].triple;
			yyval.elist.values[2] = yyvsp[-4].triple;
			yyval.elist.values[3] = yyvsp[-1].triple;
			yyval.elist.argCount = 4;
		}
    break;
case 117:
#line 798 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-13].triple;
			yyval.elist.values[1] = yyvsp[-10].triple;
			yyval.elist.values[2] = yyvsp[-7].triple;
			yyval.elist.values[3] = yyvsp[-4].triple;
			yyval.elist.values[4] = yyvsp[-1].triple;
			yyval.elist.argCount = 5;
		}
    break;
case 118:
#line 808 "sdfparse.yy"
{
			yyval.elist.values[0] = yyvsp[-16].triple;
			yyval.elist.values[1] = yyvsp[-13].triple;
			yyval.elist.values[2] = yyvsp[-10].triple;
			yyval.elist.values[3] = yyvsp[-7].triple;
			yyval.elist.values[4] = yyvsp[-4].triple;
			yyval.elist.values[5] = yyvsp[-1].triple;
			yyval.elist.argCount = 6;
		}
    break;
case 119:
#line 821 "sdfparse.yy"
{
			yyval.triple.triple[0].value = yyvsp[-4].floating;
			yyval.triple.triple[0].valid = TRUE;
			yyval.triple.triple[1] = yyvsp[-2].creal;
			yyval.triple.triple[2] = yyvsp[0].creal;
		}
    break;
case 120:
#line 828 "sdfparse.yy"
{
			yyval.triple.triple[0] = yyvsp[-4].creal;
			yyval.triple.triple[1].value = yyvsp[-2].floating;
			yyval.triple.triple[1].valid = TRUE; 
			yyval.triple.triple[2] = yyvsp[0].creal;
		}
    break;
case 121:
#line 835 "sdfparse.yy"
{
			yyval.triple.triple[0] = yyvsp[-4].creal;
			yyval.triple.triple[1] = yyvsp[-2].creal;
			yyval.triple.triple[2].value = yyvsp[0].floating;
			yyval.triple.triple[2].valid = TRUE;
		}
    break;
case 122:
#line 842 "sdfparse.yy"
{
			yyval.triple.triple[0].valid = FALSE;
			yyval.triple.triple[1].valid = FALSE;
			yyval.triple.triple[2].valid = FALSE;
		}
    break;
case 123:
#line 851 "sdfparse.yy"
{
			yyval.triple.triple[0].value = yyvsp[-4].floating;
			yyval.triple.triple[0].valid = TRUE;
			yyval.triple.triple[1] = yyvsp[-2].creal;
			yyval.triple.triple[2] = yyvsp[0].creal;
		}
    break;
case 124:
#line 858 "sdfparse.yy"
{
			yyval.triple.triple[0] = yyvsp[-4].creal;
			yyval.triple.triple[1].value = yyvsp[-2].floating;
			yyval.triple.triple[1].valid = TRUE; 
			yyval.triple.triple[2] = yyvsp[0].creal;
		}
    break;
case 125:
#line 865 "sdfparse.yy"
{
			yyval.triple.triple[0] = yyvsp[-4].creal;
			yyval.triple.triple[1] = yyvsp[-2].creal;
			yyval.triple.triple[2].value = yyvsp[0].floating;
			yyval.triple.triple[2].valid = TRUE;
		}
    break;
case 126:
#line 872 "sdfparse.yy"
{
			yyval.triple.triple[0].valid = FALSE;
			yyval.triple.triple[1].valid = FALSE;
			yyval.triple.triple[2].valid = FALSE;
		}
    break;
case 128:
#line 882 "sdfparse.yy"
{ yyval.floating = yyvsp[0].floating; }
    break;
case 129:
#line 884 "sdfparse.yy"
{ yyval.floating = -yyvsp[0].floating; }
    break;
case 131:
#line 889 "sdfparse.yy"
{ yyval.floating = (double) yyvsp[0].integer; }
    break;
case 132:
#line 894 "sdfparse.yy"
{ 	
			yyval.creal.value = yyvsp[0].floating; 
			yyval.creal.valid = TRUE;
		}
    break;
case 133:
#line 899 "sdfparse.yy"
{ yyval.creal.valid = FALSE; }
    break;
case 134:
#line 904 "sdfparse.yy"
{ 	
			yyval.creal.value = yyvsp[0].floating; 
			yyval.creal.valid = TRUE;
		}
    break;
case 135:
#line 909 "sdfparse.yy"
{ yyval.creal.valid = FALSE; }
    break;
case 137:
#line 915 "sdfparse.yy"
{ yyval.ptr = ""; }
    break;
case 138:
#line 924 "sdfparse.yy"
{ yyerrok; }
    break;
case 139:
#line 929 "sdfparse.yy"
{ yyerrok; }
    break;
case 140:
#line 931 "sdfparse.yy"
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
#line 934 "sdfparse.yy"


