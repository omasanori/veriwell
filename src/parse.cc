/* A Bison parser, made from /home/mdh/sourceforge/veriwell/src/parse.yy
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	LOWEST	257
# define	IF	258
# define	ELSE	259
# define	OROR	260
# define	ANDAND	261
# define	ANDANDAND	262
# define	XNOR	263
# define	LOGICAL_EQUALITY	264
# define	LOGICAL_INEQUALITY	265
# define	CASE_EQUALITY	266
# define	CASE_INEQUALITY	267
# define	LT	268
# define	LE	269
# define	GT	270
# define	GE	271
# define	LEFT_SHIFT	272
# define	RIGHT_SHIFT	273
# define	REDUCTION_NOR	274
# define	REDUCTION_NAND	275
# define	UNARY	276
# define	IDENTIFIER	277
# define	HIERARCHY_IDENTIFIER	278
# define	STARARROW	279
# define	EQUALARROW	280
# define	SPECPARAM	281
# define	SETUP	282
# define	HOLD	283
# define	PERIOD	284
# define	WIDTH	285
# define	SKEW	286
# define	RECOVERY	287
# define	SETUPHOLD	288
# define	NOCHANGE	289
# define	MODULE	290
# define	MACROMODULE	291
# define	ENDMODULE	292
# define	SPECIFY	293
# define	ENDSPECIFY	294
# define	PRIMITIVE	295
# define	ENDPRIMITIVE	296
# define	TABLE	297
# define	ENDTABLE	298
# define	TASK	299
# define	ENDTASK	300
# define	FUNCTION	301
# define	ENDFUNCTION	302
# define	PARAMETER	303
# define	INPUT	304
# define	OUTPUT	305
# define	INOUT	306
# define	NETTYPE	307
# define	TRIREG	308
# define	SCALARED	309
# define	VECTORED	310
# define	REG	311
# define	TIME	312
# define	INTEGER	313
# define	REAL	314
# define	EVENT	315
# define	ASSIGN	316
# define	DEFPARAM	317
# define	STRENGTH0	318
# define	STRENGTH1	319
# define	SUPPLY0	320
# define	SUPPLY1	321
# define	LOGIC	322
# define	BUFFER	323
# define	BUFIF	324
# define	CMOS	325
# define	PULL	326
# define	TRAN	327
# define	INITIAL	328
# define	ALWAYS	329
# define	CASE	330
# define	CASEZ	331
# define	CASEX	332
# define	ENDCASE	333
# define	FOREVER	334
# define	REPEAT	335
# define	WHILE	336
# define	FOR	337
# define	WAIT_	338
# define	ARROW_	339
# define	DISABLE	340
# define	DEASSIGN	341
# define	FORCE	342
# define	RELEASE	343
# define	DEFAULT	344
# define	BEGIN	345
# define	END	346
# define	FORK	347
# define	JOIN	348
# define	SYSTEM_IDENTIFIER	349
# define	STRING	350
# define	NUMBER	351
# define	BASE_NUMBER	352
# define	POSEDGE	353
# define	NEGEDGE	354
# define	OR	355
# define	SMALL	356
# define	MEDIUM	357
# define	LARGE	358
# define	EDGE	359
# define	DOUBLE_DIGIT	360
# define	UDP_CHAR	361
# define	UDP_ONE	362
# define	UDP_ZERO	363
# define	UDP_X	364
# define	UDP_B	365
# define	UDP_HOOK	366
# define	EOF_	367

#line 22 "parse.yy"

#define yyparse prog_parse
#define YYERROR_VERBOSE
#define YYDEBUG 2

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "glue.h"
#include "vtypes.h"
#include "tree.h"
#include "decl.h"
#include "flags.h"
#include "io.h"
#include "specify.h"
#include "gates.h"
#include "udp.h"
#include "lex.h"
#include "scope.h"
#include "veriwell.h"


#define syn_warning(TEXT) \
	if (synopsys_flag) \
	  synopsys_warn(TEXT);

#define function_error \
	if (in_function) \
	  error ("Statement not allowed in a function", NULL_CHAR, NULL_CHAR)

/* Used in BISON.SIM */
extern char *token_buffer;

void eat_block (short token);

#include "parse.h"
#ifdef __cplusplus
    int	yylex (YYSTYPE *);
#endif // __cplusplus


/* list of parsed modules (exported in tree.h) */
tree module_list = NULL_TREE;

/* tree list of hierarchical references to be resolved at pass3 */
tree hierarchical_list;

/* The spec of the current declaration */
tree current_spec;

tree current_module;
tree current_case;
tree current_delay;
enum tree_type current_gatetype;


tree current_instance_module = NULL_TREE;
tree current_instance_param = NULL_TREE;
tree current_udp_string = NULL_TREE;

tree interactive_statement;

tree tmp_tree;

/* Defined in decl.c */
extern tree current_scope;

/* Defined in verilog.c */
extern int is_interactive;


/* Certain constructs are not allowed while inside a function.  Also, ports
   are treated differently in functions and tasks than for modules. */
int in_function = 0;
int in_tf = 0;
int in_systask = 0;

/* If inside continuous assignment, handle lval differently */
enum lval_type lval_type = LVAL_REG;

/* If inside instantiation port list, allow implicit net declarations. */
int in_instantiation = 0;

/* If inside event statement, accept event types. */
int in_event = 0;

void init_parse()
{
	current_instance_module = NULL_TREE;
	current_instance_param = NULL_TREE;
	current_udp_string = NULL_TREE;
	in_function = 0;
	in_tf = 0;
	in_systask = 0;
	lval_type = LVAL_REG;
	in_instantiation = 0;
	module_list = NULL_TREE;
	hierarchical_list = NULL_TREE;
	current_spec = NULL_TREE;
	current_module = NULL_TREE;
	current_case = NULL_TREE;
	current_delay = NULL_TREE;
	interactive_statement = NULL_TREE;
	tmp_tree = NULL_TREE;
}


#line 133 "parse.yy"
#ifndef YYSTYPE
typedef union {
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
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		998
#define	YYFLAG		-32768
#define	YYNTBASE	138

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 367 ? yytranslate[x] : 349)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,   134,     2,    29,    14,     2,
      30,   135,    27,    25,   128,    26,   129,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     7,   137,
       2,   133,     2,     6,   136,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,   132,    12,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   130,    11,   131,    33,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       8,     9,    10,    13,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,     9,    11,    14,    16,    18,
      19,    20,    21,    31,    32,    35,    38,    42,    43,    45,
      49,    51,    55,    57,    63,    65,    69,    70,    72,    76,
      78,    83,    90,    92,    94,    96,    98,   100,   102,   104,
     106,   108,   110,   112,   114,   116,   117,   118,   119,   131,
     132,   135,   138,   140,   142,   146,   147,   153,   154,   159,
     161,   164,   167,   168,   171,   173,   175,   177,   179,   181,
     183,   188,   190,   192,   194,   196,   198,   199,   207,   208,
     217,   218,   220,   222,   224,   225,   228,   230,   232,   234,
     235,   241,   243,   247,   249,   253,   257,   261,   265,   266,
     271,   276,   281,   286,   289,   292,   295,   297,   301,   303,
     306,   310,   314,   317,   320,   323,   325,   327,   328,   333,
     338,   343,   347,   348,   354,   356,   358,   360,   362,   365,
     368,   369,   370,   376,   380,   382,   386,   388,   391,   395,
     399,   400,   405,   407,   411,   413,   416,   420,   424,   425,
     430,   434,   438,   444,   446,   450,   452,   455,   459,   463,
     465,   472,   474,   478,   480,   484,   485,   489,   493,   497,
     498,   500,   506,   512,   514,   516,   518,   520,   521,   523,
     529,   530,   534,   536,   538,   540,   542,   544,   546,   548,
     549,   553,   554,   559,   561,   565,   570,   574,   575,   581,
     582,   585,   590,   595,   602,   611,   613,   617,   619,   623,
     625,   628,   632,   636,   638,   642,   644,   647,   651,   655,
     660,   664,   666,   668,   670,   674,   676,   680,   682,   683,
     685,   689,   695,   700,   701,   705,   706,   710,   712,   714,
     717,   718,   726,   727,   728,   737,   738,   742,   743,   750,
     751,   758,   759,   770,   771,   775,   776,   780,   781,   788,
     789,   796,   797,   803,   804,   811,   812,   819,   820,   827,
     828,   833,   835,   837,   839,   841,   842,   847,   848,   855,
     856,   861,   862,   869,   870,   875,   876,   879,   881,   883,
     885,   886,   891,   893,   896,   898,   901,   905,   909,   912,
     914,   918,   920,   924,   926,   930,   931,   933,   934,   939,
     940,   948,   949,   954,   955,   963,   964,   967,   970,   971,
     974,   977,   978,   981,   983,   985,   988,   994,   997,   998,
    1005,  1007,  1009,  1011,  1016,  1023,  1024,  1028,  1031,  1032,
    1035,  1038,  1040,  1042,  1044,  1045,  1050,  1056,  1066,  1077,
    1088,  1100,  1106,  1113,  1115,  1119,  1121,  1125,  1129,  1131,
    1135,  1137,  1141,  1145,  1147,  1152,  1159,  1161,  1166,  1173,
    1175,  1181,  1189,  1203,  1205,  1207,  1209,  1211,  1213,  1215,
    1217,  1219,  1221,  1222,  1227,  1238,  1249,  1258,  1266,  1277,
    1288,  1299,  1312,  1324,  1325,  1328,  1330,  1333,  1338,  1341,
    1346,  1347,  1349,  1351,  1352,  1358,  1360,  1362,  1363,  1369,
    1371,  1375,  1377,  1379,  1383,  1386,  1390,  1394,  1398,  1402,
    1404,  1406,  1408,  1410,  1415,  1422,  1426,  1428,  1430,  1434,
    1438,  1440,  1443,  1447,  1451,  1455,  1457,  1459,  1461,  1467,
    1473,  1475,  1478,  1481,  1484,  1487,  1490,  1493,  1496,  1499,
    1502,  1505,  1509,  1513,  1517,  1518,  1523,  1524,  1529,  1533,
    1537,  1538,  1543,  1544,  1549,  1553,  1557,  1561,  1565,  1569,
    1573,  1577,  1581,  1585,  1589,  1593,  1597,  1603,  1605,  1607,
    1609,  1611,  1616,  1623,  1625,  1627,  1631,  1635,  1639,  1646,
    1651,  1656,  1658,  1660,  1662,  1664,  1666,  1667,  1669,  1672,
    1675,  1680,  1682,  1686,  1692,  1695,  1698,  1703,  1704,  1708,
    1709,  1715,  1717,  1720,  1723,  1727,  1729,  1731,  1733
};
static const short yyrhs[] =
{
     241,     0,   139,   127,     0,     1,     0,   127,     0,   140,
       0,   139,   140,     0,   141,     0,   153,     0,     0,     0,
       0,    50,   142,   337,   143,   146,   348,   144,   145,    52,
       0,     0,   145,   152,     0,   145,     1,     0,    30,   147,
     346,     0,     0,   148,     0,   147,   128,   148,     0,     1,
       0,   147,   128,     1,     0,   149,     0,   129,    37,    30,
     149,   346,     0,   151,     0,   130,   150,   131,     0,     0,
     151,     0,   150,   128,   151,     0,    37,     0,    37,    31,
     325,   132,     0,    37,    31,   325,     7,   325,   132,     0,
     175,     0,   181,     0,   188,     0,   180,     0,   216,     0,
     224,     0,   203,     0,   194,     0,   287,     0,   236,     0,
     238,     0,   168,     0,   170,     0,     0,     0,     0,    55,
     154,    37,   155,   146,   348,   156,   157,   160,   161,    56,
       0,     0,   157,   158,     0,   157,     1,     0,   181,     0,
     159,     0,    71,    37,   348,     0,     0,    88,    37,   133,
     320,   348,     0,     0,    57,   162,   163,    58,     0,   164,
       0,   163,   164,     0,   165,   348,     0,     0,   165,   166,
       0,   121,     0,   126,     0,   123,     0,   122,     0,   125,
       0,   124,     0,    30,   167,   167,   346,     0,   124,     0,
     123,     0,   122,     0,   125,     0,   126,     0,     0,    59,
      37,   348,   169,   173,   240,    60,     0,     0,    61,   172,
      37,   348,   171,   173,   240,    62,     0,     0,   215,     0,
      73,     0,    74,     0,     0,   173,   174,     0,   175,     0,
     181,     0,   180,     0,     0,    63,   214,   176,   177,   348,
       0,   178,     0,   177,   128,   178,     0,     1,     0,   177,
       1,   178,     0,   177,   128,     1,     0,    37,   133,   325,
       0,    37,   133,   327,     0,     0,   184,   179,   205,   348,
       0,   186,   179,   207,   348,     0,   185,   179,   208,   348,
       0,   182,   179,   183,   348,     0,    64,   214,     0,    65,
     214,     0,    66,   214,     0,    37,     0,   183,   128,    37,
       0,     1,     0,   183,     1,     0,   183,     1,    37,     0,
     183,   128,     1,     0,    71,   214,     0,    73,   214,     0,
      72,   214,     0,    74,     0,    75,     0,     0,   189,   187,
     208,   348,     0,   190,   179,   208,   348,     0,   189,   187,
     197,   348,     0,   192,   193,   338,     0,     0,    68,   191,
     209,   193,   338,     0,    67,     0,    80,     0,    81,     0,
     215,     0,    69,   215,     0,    70,   215,     0,     0,     0,
     196,   179,   195,   200,   348,     0,    76,   210,   338,     0,
     198,     0,   197,   128,   198,     0,     1,     0,   197,     1,
       0,   197,     1,   198,     0,   197,   128,     1,     0,     0,
      37,   133,   199,   328,     0,   201,     0,   200,   128,   201,
       0,     1,     0,   200,     1,     0,   200,     1,   201,     0,
     200,   128,     1,     0,     0,   285,   133,   202,   328,     0,
      77,   204,   348,     0,   336,   133,   325,     0,   204,   128,
     336,   133,   325,     0,   206,     0,   205,   128,   206,     0,
       1,     0,   205,     1,     0,   205,     1,   206,     0,   205,
     128,     1,     0,    37,     0,    37,    31,   325,     7,   325,
     132,     0,    37,     0,   207,   128,    37,     0,    37,     0,
     208,   128,    37,     0,     0,    30,   116,   346,     0,    30,
     117,   346,     0,    30,   118,   346,     0,     0,   211,     0,
      30,   212,   128,   213,   346,     0,    30,   213,   128,   212,
     346,     0,    78,     0,    80,     0,    79,     0,    81,     0,
       0,   215,     0,    31,   325,     7,   325,   347,     0,     0,
     218,   217,   219,     0,    82,     0,   115,     0,    83,     0,
      84,     0,    85,     0,    86,     0,    87,     0,     0,   220,
     222,   348,     0,     0,   339,   221,   222,   348,     0,   223,
       0,   222,   128,   223,     0,    37,    30,   228,   346,     0,
      30,   228,   346,     0,     0,    37,   226,   225,   229,   348,
       0,     0,   134,   325,     0,   134,    30,   227,   135,     0,
     134,    30,   326,   135,     0,   134,    30,   326,   128,   326,
     135,     0,   134,    30,   326,   128,   326,   128,   326,   135,
       0,   325,     0,   227,   128,   325,     0,   328,     0,   228,
     128,   328,     0,     1,     0,   228,     1,     0,   228,     1,
     328,     0,   228,   128,     1,     0,   230,     0,   229,   128,
     230,     0,     1,     0,   229,     1,     0,   229,     1,   230,
       0,   229,   128,     1,     0,    37,    30,   231,   346,     0,
      30,   231,   346,     0,   232,     0,   234,     0,   233,     0,
     232,   128,   233,     0,     1,     0,   232,   128,     1,     0,
     328,     0,     0,   235,     0,   234,   128,   235,     0,   129,
      37,    30,   328,   346,     0,   129,    37,    30,   346,     0,
       0,    88,   237,   241,     0,     0,    89,   239,   241,     0,
     241,     0,   348,     0,   265,   348,     0,     0,     4,   242,
      30,   328,   346,   240,   263,     0,     0,     0,   243,   264,
     244,    30,   328,   346,   267,    93,     0,     0,    94,   245,
     241,     0,     0,    95,   246,    30,   328,   346,   241,     0,
       0,    96,   247,    30,   328,   346,   241,     0,     0,    97,
     248,    30,   265,   348,   328,   348,   265,   346,   241,     0,
       0,   341,   249,   240,     0,     0,   342,   250,   240,     0,
       0,   285,   133,   251,   341,   328,   348,     0,     0,   285,
     133,   252,   342,   328,   348,     0,     0,   285,    20,   253,
     328,   348,     0,     0,   285,    20,   254,   341,   328,   348,
       0,     0,   285,    20,   255,   342,   328,   348,     0,     0,
      98,   256,    30,   324,   346,   240,     0,     0,    99,   257,
     336,   348,     0,   272,     0,   275,     0,   282,     0,   283,
       0,     0,   100,   258,   336,   348,     0,     0,    76,   259,
     285,   133,   328,   348,     0,     0,   101,   260,   285,   348,
       0,     0,   102,   261,   285,   133,   328,   348,     0,     0,
     103,   262,   285,   348,     0,     0,     5,   240,     0,    90,
       0,    91,     0,    92,     0,     0,   285,   133,   266,   328,
       0,   268,     0,   267,   268,     0,     1,     0,   267,     1,
       0,   269,     7,   240,     0,   104,     7,   240,     0,   104,
     240,     0,   328,     0,   269,   128,   328,     0,     1,     0,
     269,   128,     1,     0,   271,     0,   270,   128,   271,     0,
       0,   328,     0,     0,   105,   273,   278,   106,     0,     0,
     105,     7,    37,   274,   280,   278,   106,     0,     0,   107,
     276,   279,   108,     0,     0,   107,     7,    37,   277,   280,
     279,   108,     0,     0,   278,   241,     0,   278,     1,     0,
       0,   279,   241,     0,   279,     1,     0,     0,   280,   281,
       0,   175,     0,   180,     0,   336,   348,     0,   336,    30,
     269,   346,   348,     0,   109,   348,     0,     0,   109,   284,
      30,   270,   346,   348,     0,   286,     0,   322,     0,   336,
       0,   336,    31,   328,   132,     0,   336,    31,   328,     7,
     328,   132,     0,     0,    53,   288,   289,     0,   290,    54,
       0,     0,   290,   291,     0,   290,     1,     0,   292,     0,
     294,     0,   308,     0,     0,    41,   293,   177,   348,     0,
     307,   295,   133,   300,   348,     0,    30,   296,   303,    30,
     297,     7,   305,   346,   346,     0,    30,   306,   296,   303,
      30,   297,     7,   305,   346,   346,     0,    30,   296,   303,
      30,   297,   302,     7,   305,   346,   346,     0,    30,   306,
     296,   303,    30,   297,   302,     7,   305,   346,   346,     0,
      30,   296,   303,   297,   346,     0,    30,   296,   302,   303,
     297,   346,     0,   298,     0,   296,   128,   298,     0,     1,
       0,   296,     1,   298,     0,   296,   128,     1,     0,   299,
       0,   297,   128,   299,     0,     1,     0,   297,     1,   299,
       0,   297,   128,     1,     0,    37,     0,    37,    31,   325,
     347,     0,    37,    31,   325,     7,   325,   347,     0,    37,
       0,    37,    31,   325,   347,     0,    37,    31,   325,     7,
     325,   347,     0,   301,     0,    30,   301,   128,   301,   346,
       0,    30,   301,   128,   301,   128,   301,   346,     0,    30,
     301,   128,   301,   128,   301,   128,   301,   128,   301,   128,
     301,   346,     0,   326,     0,    25,     0,    26,     0,    39,
       0,    40,     0,   328,     0,   328,     0,   113,     0,   114,
       0,     0,     4,    30,   304,   346,     0,    42,    30,   311,
     128,   311,   128,   328,   309,   346,   348,     0,    43,    30,
     311,   128,   311,   128,   328,   309,   346,   348,     0,    44,
      30,   312,   128,   328,   309,   346,   348,     0,    45,    30,
     312,   128,   328,   346,   348,     0,    45,    30,   312,   128,
     328,   128,   328,   309,   346,   348,     0,    46,    30,   311,
     128,   311,   128,   328,   309,   346,   348,     0,    47,    30,
     312,   128,   311,   128,   328,   309,   346,   348,     0,    48,
      30,   311,   128,   311,   128,   328,   128,   328,   309,   346,
     348,     0,    49,    30,   312,   128,   311,   128,   328,   128,
     328,   346,   348,     0,     0,   128,   310,     0,    37,     0,
     313,   321,     0,   313,   321,    10,   318,     0,   315,   321,
       0,   315,   321,    10,   318,     0,     0,   113,     0,   114,
       0,     0,   119,    31,   314,   317,   347,     0,   113,     0,
     114,     0,     0,   119,    31,   316,   317,   347,     0,   120,
       0,   120,   128,   317,     0,     1,     0,   319,     0,    30,
     318,   135,     0,    33,   319,     0,   319,    15,   320,     0,
     319,    17,   320,     0,   319,    18,   320,     0,   319,    16,
     320,     0,    37,     0,   112,     0,   111,     0,   336,     0,
     336,    31,   328,   132,     0,   336,    31,   325,     7,   325,
     132,     0,   130,   323,   131,     0,   286,     0,   322,     0,
     323,   128,   286,     0,   323,   128,   322,     0,     1,     0,
     323,     1,     0,   323,     1,   286,     0,   323,     1,   322,
       0,   323,   128,     1,     0,   328,     0,   328,     0,   328,
       0,   328,     7,   328,     7,   328,     0,   328,     7,   328,
       7,   328,     0,   333,     0,    25,   333,     0,    26,   333,
       0,    32,   333,     0,    33,   333,     0,    14,   333,     0,
      35,   333,     0,    11,   333,     0,    34,   333,     0,    12,
     333,     0,    13,   333,     0,   328,    25,   328,     0,   328,
      26,   328,     0,   328,    27,   328,     0,     0,   328,    28,
     329,   328,     0,     0,   328,    29,   330,   328,     0,   328,
      15,   328,     0,   328,    16,   328,     0,     0,   328,    17,
     331,   328,     0,     0,   328,    18,   332,   328,     0,   328,
       9,   328,     0,   328,     8,   328,     0,   328,    19,   328,
       0,   328,    20,   328,     0,   328,    21,   328,     0,   328,
      22,   328,     0,   328,    14,   328,     0,   328,    11,   328,
       0,   328,    12,   328,     0,   328,    13,   328,     0,   328,
      23,   328,     0,   328,    24,   328,     0,   328,     6,   328,
       7,   328,     0,   110,     0,   111,     0,   112,     0,   336,
       0,   336,    31,   328,   132,     0,   336,    31,   325,     7,
     325,   132,     0,   334,     0,   335,     0,    30,   326,   346,
       0,    30,     1,   346,     0,   130,   269,   131,     0,   130,
     325,   130,   269,   131,   131,     0,   336,    30,   270,   346,
       0,   109,    30,   270,   346,     0,   109,     0,    38,     0,
      37,     0,    37,     0,   115,     0,     0,   339,     0,   134,
     111,     0,   134,   336,     0,   134,    30,   340,   346,     0,
     326,     0,   326,   128,   326,     0,   326,   128,   326,   128,
     326,     0,   134,   111,     0,   134,   336,     0,   134,    30,
     326,   346,     0,     0,   136,   343,   336,     0,     0,   136,
      30,   344,   345,   346,     0,   328,     0,   113,   328,     0,
     114,   328,     0,   345,   115,   345,     0,   135,     0,   132,
       0,     1,     0,   137,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   426,   435,   436,   445,   454,   455,   467,   468,   472,
     472,   472,   472,   500,   501,   503,   507,   509,   514,   515,
     519,   525,   529,   535,   540,   541,   545,   550,   552,   557,
     559,   564,   572,   574,   576,   578,   580,   581,   583,   585,
     587,   589,   593,   597,   599,   614,   614,   614,   614,   636,
     638,   640,   644,   645,   649,   662,   664,   682,   682,   694,
     695,   701,   709,   711,   715,   720,   725,   730,   735,   740,
     745,   760,   761,   762,   763,   764,   768,   768,   788,   788,
     807,   809,   811,   813,   818,   820,   824,   825,   826,   834,
     834,   843,   844,   848,   849,   851,   855,   858,   864,   869,
     872,   875,   882,   890,   897,   905,   917,   919,   923,   924,
     925,   927,   931,   933,   938,   945,   950,   956,   961,   967,
     968,   977,   984,   984,   993,   994,   996,  1008,  1009,  1014,
    1019,  1024,  1024,  1034,  1041,  1043,  1047,  1048,  1049,  1051,
    1055,  1055,  1065,  1067,  1071,  1072,  1073,  1075,  1079,  1079,
    1085,  1090,  1095,  1104,  1105,  1109,  1110,  1111,  1113,  1117,
    1119,  1126,  1128,  1133,  1135,  1140,  1142,  1144,  1146,  1151,
    1153,  1161,  1163,  1168,  1169,  1173,  1174,  1187,  1189,  1193,
    1209,  1209,  1220,  1222,  1224,  1226,  1228,  1230,  1232,  1246,
    1246,  1251,  1251,  1257,  1258,  1262,  1268,  1279,  1279,  1315,
    1317,  1319,  1321,  1323,  1327,  1335,  1337,  1364,  1366,  1369,
    1370,  1371,  1373,  1383,  1384,  1386,  1387,  1388,  1390,  1394,
    1402,  1412,  1413,  1417,  1418,  1422,  1423,  1427,  1429,  1434,
    1435,  1440,  1443,  1450,  1450,  1458,  1458,  1464,  1465,  1470,
    1473,  1473,  1477,  1477,  1477,  1487,  1487,  1493,  1493,  1502,
    1502,  1508,  1508,  1518,  1518,  1526,  1526,  1535,  1535,  1546,
    1546,  1556,  1556,  1567,  1567,  1578,  1578,  1594,  1594,  1602,
    1602,  1610,  1611,  1612,  1613,  1614,  1614,  1620,  1620,  1626,
    1626,  1631,  1631,  1640,  1640,  1652,  1654,  1659,  1661,  1663,
    1675,  1675,  1682,  1683,  1687,  1688,  1692,  1695,  1700,  1708,
    1710,  1712,  1713,  1717,  1724,  1733,  1735,  1739,  1739,  1746,
    1746,  1770,  1770,  1778,  1778,  1801,  1803,  1807,  1811,  1813,
    1817,  1821,  1823,  1827,  1828,  1832,  1838,  1846,  1848,  1848,
    1857,  1859,  1864,  1866,  1869,  1879,  1879,  1888,  1893,  1895,
    1900,  1903,  1904,  1905,  1923,  1923,  1932,  1943,  1951,  1959,
    1967,  1975,  1981,  1989,  1990,  1994,  1995,  1997,  2000,  2001,
    2005,  2006,  2008,  2011,  2015,  2019,  2025,  2029,  2033,  2039,
    2040,  2044,  2051,  2064,  2070,  2074,  2080,  2084,  2090,  2095,
    2098,  2102,  2108,  2112,  2119,  2124,  2129,  2134,  2139,  2144,
    2149,  2154,  2159,  2166,  2170,  2176,  2179,  2183,  2190,  2194,
    2201,  2206,  2211,  2216,  2216,  2228,  2233,  2238,  2238,  2250,
    2255,  2260,  2266,  2267,  2269,  2271,  2273,  2275,  2277,  2281,
    2285,  2289,  2296,  2301,  2306,  2319,  2327,  2329,  2331,  2333,
    2335,  2336,  2337,  2338,  2339,  2343,  2347,  2358,  2359,  2369,
    2379,  2380,  2382,  2384,  2386,  2388,  2390,  2392,  2394,  2396,
    2398,  2400,  2402,  2404,  2406,  2406,  2410,  2410,  2414,  2416,
    2418,  2418,  2422,  2422,  2426,  2428,  2430,  2432,  2434,  2436,
    2438,  2440,  2442,  2444,  2446,  2448,  2450,  2452,  2472,  2473,
    2474,  2476,  2478,  2480,  2481,  2482,  2484,  2517,  2522,  2531,
    2536,  2540,  2551,  2560,  2564,  2565,  2584,  2586,  2590,  2594,
    2599,  2608,  2612,  2616,  2623,  2625,  2627,  2632,  2632,  2644,
    2644,  2651,  2658,  2661,  2664,  2673,  2678,  2680,  2685
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "LOWEST", "IF", "ELSE", "'?'", "':'", "OROR", 
  "ANDAND", "ANDANDAND", "'|'", "'^'", "XNOR", "'&'", "LOGICAL_EQUALITY", 
  "LOGICAL_INEQUALITY", "CASE_EQUALITY", "CASE_INEQUALITY", "LT", "LE", 
  "GT", "GE", "LEFT_SHIFT", "RIGHT_SHIFT", "'+'", "'-'", "'*'", "'/'", 
  "'%'", "'('", "'['", "'!'", "'~'", "REDUCTION_NOR", "REDUCTION_NAND", 
  "UNARY", "IDENTIFIER", "HIERARCHY_IDENTIFIER", "STARARROW", 
  "EQUALARROW", "SPECPARAM", "SETUP", "HOLD", "PERIOD", "WIDTH", "SKEW", 
  "RECOVERY", "SETUPHOLD", "NOCHANGE", "MODULE", "MACROMODULE", 
  "ENDMODULE", "SPECIFY", "ENDSPECIFY", "PRIMITIVE", "ENDPRIMITIVE", 
  "TABLE", "ENDTABLE", "TASK", "ENDTASK", "FUNCTION", "ENDFUNCTION", 
  "PARAMETER", "INPUT", "OUTPUT", "INOUT", "NETTYPE", "TRIREG", 
  "SCALARED", "VECTORED", "REG", "TIME", "INTEGER", "REAL", "EVENT", 
  "ASSIGN", "DEFPARAM", "STRENGTH0", "STRENGTH1", "SUPPLY0", "SUPPLY1", 
  "LOGIC", "BUFFER", "BUFIF", "CMOS", "PULL", "TRAN", "INITIAL", "ALWAYS", 
  "CASE", "CASEZ", "CASEX", "ENDCASE", "FOREVER", "REPEAT", "WHILE", 
  "FOR", "WAIT_", "ARROW_", "DISABLE", "DEASSIGN", "FORCE", "RELEASE", 
  "DEFAULT", "BEGIN", "END", "FORK", "JOIN", "SYSTEM_IDENTIFIER", 
  "STRING", "NUMBER", "BASE_NUMBER", "POSEDGE", "NEGEDGE", "OR", "SMALL", 
  "MEDIUM", "LARGE", "EDGE", "DOUBLE_DIGIT", "UDP_CHAR", "UDP_ONE", 
  "UDP_ZERO", "UDP_X", "UDP_B", "UDP_HOOK", "EOF_", "','", "'.'", "'{'", 
  "'}'", "']'", "'='", "'#'", "')'", "'@'", "';'", "source_text", 
  "description_list", "description", "module", "@1", "@2", "@3", 
  "module_item_list", "list_of_ports_o", "port_clist", "port", 
  "port_expression_o", "port_reference_clist", "port_reference", 
  "module_item", "UDP", "@4", "@5", "@6", "UDP_declarations", 
  "UDP_declaration", "UDP_reg_statement", "UDP_initial_statement", 
  "UDP_table_definition", "@7", "table_entry_list", "table_entry", 
  "table_digit_list", "table_digit", "table_digit1", "task", "@8", 
  "function", "@9", "function_type", "tf_declaration_list", 
  "tf_declaration", "parameter_declaration", "@10", 
  "list_of_param_assignments", "param_assignment", "setspec", 
  "static_declaration", "port_declaration", "port_spec", 
  "port_decl_identifiers", "reg_spec", "real_spec", "event_spec", 
  "setnetspec", "net_declaration", "net_spec", "trireg_spec", "@11", 
  "nettype", "expandrange_o", "continuous_assign", "@12", "assign_spec", 
  "list_of_net_assignments", "net_assignment", "@13", 
  "list_of_assignments", "cont_assignment", "@14", "parameter_override", 
  "list_of_param_overrides", "reg_decl_identifiers", 
  "reg_decl_identifier", "non_reg_decl_identifiers", "decl_identifiers", 
  "charge_strength_o", "drive_strength_o", "drive_strength", "strength0", 
  "strength1", "xrange", "range", "gate_declaration", "@15", "gatetype", 
  "gate_description", "@16", "@17", "gate_instance_clist", 
  "gate_instance", "UDP_or_module_instantiation", "@18", 
  "xparameter_value_assignment", "constant_expression_clist", 
  "terminal_clist", "module_instance_clist", "module_instance", 
  "list_of_module_connections_o", "module_port_connection_clist", 
  "module_port_connection", "named_port_connection_clist", 
  "named_port_connection", "initial_statement", "@19", "always_statement", 
  "@20", "statement_or_null", "statement", "@21", "@22", "@23", "@24", 
  "@25", "@26", "@27", "@28", "@29", "@30", "@31", "@32", "@33", "@34", 
  "@35", "@36", "@37", "@38", "@39", "@40", "@41", "elsepart", "case", 
  "assignment", "@42", "case_item_plist", "case_expr", "expression_clist", 
  "xexpression_clist", "xexpression", "seq_block", "@43", "@44", 
  "par_block", "@45", "@46", "statement_list", "statement_tree_list", 
  "block_declaration_list", "block_declaration", "task_enable", 
  "system_task_enable", "@47", "lvalue", "lval_normal", "specify_block", 
  "@48", "specify_stuff", "specify_items", "specify_item", 
  "specparam_declaration", "@49", "path_declaration", "path_description", 
  "list_of_path_inputs", "list_of_path_outputs", 
  "specify_input_terminal_descriptor", 
  "specify_output_terminal_descriptor", "path_delay_value", 
  "path_delay_expression", "polarity_operator", "path_operator", 
  "conditional_port_expression", "data_source_expression", 
  "edge_identifier", "specify_if_clause", "system_timing_check", 
  "notify_clause", "notify_register", "timing_check_event", 
  "controlled_timing_check_event", "timing_check_event_control", "@50", 
  "timing_check_event_control_no_empty", "@51", "edge_list", 
  "timing_check_condition", "scalar_expression", "scalar_constant", 
  "timing_input_terminal_descriptor", "lval_concat", "lval_clist", 
  "conditional_expression", "constant_expression", "mintypmax_expression", 
  "mintypmax_expression_triplet", "expression", "@52", "@53", "@54", 
  "@55", "primary", "concatenation", "function_call", "identifier", 
  "IDENTIFIER_or_OR", "delay_o", "delay", "mintypmax_clist", 
  "delay_control", "event_control", "@56", "@57", "event_expression", 
  "rp", "rb", "sc", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   138,   138,   138,   138,   139,   139,   140,   140,   142,
     143,   144,   141,   145,   145,   145,   146,   146,   147,   147,
     147,   147,   148,   148,   149,   149,   149,   150,   150,   151,
     151,   151,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   154,   155,   156,   153,   157,
     157,   157,   158,   158,   159,   160,   160,   162,   161,   163,
     163,   164,   165,   165,   166,   166,   166,   166,   166,   166,
     166,   167,   167,   167,   167,   167,   169,   168,   171,   170,
     172,   172,   172,   172,   173,   173,   174,   174,   174,   176,
     175,   177,   177,   177,   177,   177,   178,   178,   179,   180,
     180,   180,   181,   182,   182,   182,   183,   183,   183,   183,
     183,   183,   184,   184,   184,   185,   186,   187,   188,   188,
     188,   189,   191,   190,   192,   192,   192,   193,   193,   193,
     193,   195,   194,   196,   197,   197,   197,   197,   197,   197,
     199,   198,   200,   200,   200,   200,   200,   200,   202,   201,
     203,   204,   204,   205,   205,   205,   205,   205,   205,   206,
     206,   207,   207,   208,   208,   209,   209,   209,   209,   210,
     210,   211,   211,   212,   212,   213,   213,   214,   214,   215,
     217,   216,   218,   218,   218,   218,   218,   218,   218,   220,
     219,   221,   219,   222,   222,   223,   223,   225,   224,   226,
     226,   226,   226,   226,   226,   227,   227,   228,   228,   228,
     228,   228,   228,   229,   229,   229,   229,   229,   229,   230,
     230,   231,   231,   232,   232,   232,   232,   233,   233,   234,
     234,   235,   235,   237,   236,   239,   238,   240,   240,   241,
     242,   241,   243,   244,   241,   245,   241,   246,   241,   247,
     241,   248,   241,   249,   241,   250,   241,   251,   241,   252,
     241,   253,   241,   254,   241,   255,   241,   256,   241,   257,
     241,   241,   241,   241,   241,   258,   241,   259,   241,   260,
     241,   261,   241,   262,   241,   263,   263,   264,   264,   264,
     266,   265,   267,   267,   267,   267,   268,   268,   268,   269,
     269,   269,   269,   270,   270,   271,   271,   273,   272,   274,
     272,   276,   275,   277,   275,   278,   278,   278,   279,   279,
     279,   280,   280,   281,   281,   282,   282,   283,   284,   283,
     285,   285,   286,   286,   286,   288,   287,   289,   290,   290,
     290,   291,   291,   291,   293,   292,   294,   295,   295,   295,
     295,   295,   295,   296,   296,   296,   296,   296,   297,   297,
     297,   297,   297,   298,   298,   298,   299,   299,   299,   300,
     300,   300,   300,   301,   302,   302,   303,   303,   304,   305,
     306,   306,   307,   307,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   309,   309,   310,   311,   311,   312,   312,
     313,   313,   313,   314,   313,   315,   315,   316,   315,   317,
     317,   317,   318,   318,   318,   318,   318,   318,   318,   319,
     320,   320,   321,   321,   321,   322,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   324,   325,   326,   326,   327,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   329,   328,   330,   328,   328,   328,
     331,   328,   332,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   334,   334,   335,
     335,   335,   336,   336,   337,   337,   338,   338,   339,   339,
     339,   340,   340,   340,   341,   341,   341,   343,   342,   344,
     342,   345,   345,   345,   345,   346,   347,   347,   348
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     1,     1,     2,     1,     1,     0,
       0,     0,     9,     0,     2,     2,     3,     0,     1,     3,
       1,     3,     1,     5,     1,     3,     0,     1,     3,     1,
       4,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     0,     0,    11,     0,
       2,     2,     1,     1,     3,     0,     5,     0,     4,     1,
       2,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       4,     1,     1,     1,     1,     1,     0,     7,     0,     8,
       0,     1,     1,     1,     0,     2,     1,     1,     1,     0,
       5,     1,     3,     1,     3,     3,     3,     3,     0,     4,
       4,     4,     4,     2,     2,     2,     1,     3,     1,     2,
       3,     3,     2,     2,     2,     1,     1,     0,     4,     4,
       4,     3,     0,     5,     1,     1,     1,     1,     2,     2,
       0,     0,     5,     3,     1,     3,     1,     2,     3,     3,
       0,     4,     1,     3,     1,     2,     3,     3,     0,     4,
       3,     3,     5,     1,     3,     1,     2,     3,     3,     1,
       6,     1,     3,     1,     3,     0,     3,     3,     3,     0,
       1,     5,     5,     1,     1,     1,     1,     0,     1,     5,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     0,     4,     1,     3,     4,     3,     0,     5,     0,
       2,     4,     4,     6,     8,     1,     3,     1,     3,     1,
       2,     3,     3,     1,     3,     1,     2,     3,     3,     4,
       3,     1,     1,     1,     3,     1,     3,     1,     0,     1,
       3,     5,     4,     0,     3,     0,     3,     1,     1,     2,
       0,     7,     0,     0,     8,     0,     3,     0,     6,     0,
       6,     0,    10,     0,     3,     0,     3,     0,     6,     0,
       6,     0,     5,     0,     6,     0,     6,     0,     6,     0,
       4,     1,     1,     1,     1,     0,     4,     0,     6,     0,
       4,     0,     6,     0,     4,     0,     2,     1,     1,     1,
       0,     4,     1,     2,     1,     2,     3,     3,     2,     1,
       3,     1,     3,     1,     3,     0,     1,     0,     4,     0,
       7,     0,     4,     0,     7,     0,     2,     2,     0,     2,
       2,     0,     2,     1,     1,     2,     5,     2,     0,     6,
       1,     1,     1,     4,     6,     0,     3,     2,     0,     2,
       2,     1,     1,     1,     0,     4,     5,     9,    10,    10,
      11,     5,     6,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     3,     1,     4,     6,     1,     4,     6,     1,
       5,     7,    13,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     4,    10,    10,     8,     7,    10,    10,
      10,    12,    11,     0,     2,     1,     2,     4,     2,     4,
       0,     1,     1,     0,     5,     1,     1,     0,     5,     1,
       3,     1,     1,     3,     2,     3,     3,     3,     3,     1,
       1,     1,     1,     4,     6,     3,     1,     1,     3,     3,
       1,     2,     3,     3,     3,     1,     1,     1,     5,     5,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     3,     3,     0,     4,     0,     4,     3,     3,
       0,     4,     0,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     1,     1,     1,
       1,     4,     6,     1,     1,     3,     3,     3,     6,     4,
       4,     1,     1,     1,     1,     1,     0,     1,     2,     2,
       4,     1,     3,     5,     2,     2,     4,     0,     3,     0,
       5,     1,     2,     2,     3,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     3,   240,   493,   492,     9,    45,   277,   245,   247,
     249,   251,   267,   269,   275,   279,   281,   283,   307,   311,
     328,     4,     0,     0,   507,     0,     5,     7,     8,     1,
       0,     0,   271,   272,   273,   274,     0,   330,   331,   332,
     253,   255,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,   318,
     518,     0,   327,   430,   426,   427,     0,   332,     0,   504,
     505,   509,     0,     2,     6,   287,   288,   289,   243,   239,
     261,   290,     0,     0,   325,   242,   242,     0,   494,   495,
      10,    46,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,     0,   313,     0,   305,   431,     0,
     425,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   491,   477,   478,   479,     0,     0,   437,   440,
     483,   484,   480,     0,   508,     0,     0,     0,     0,     0,
       0,     0,   301,     0,   299,     0,   254,   237,   238,   256,
       0,    17,    17,     0,     0,     0,     0,     0,     0,   435,
     270,   276,   280,     0,   284,   321,   317,   308,   316,   321,
     320,   312,   319,     0,   303,   306,   432,   433,   434,   428,
     429,   447,   449,   450,   445,   441,   442,     0,     0,   443,
     444,   448,   446,   305,     0,     0,   299,   515,   506,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   460,
     462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   456,   305,     0,     0,     0,   511,     0,     0,     0,
       0,     0,     0,     0,   291,     0,     0,     0,   333,   242,
       0,     0,     0,     0,   242,   242,     0,   290,   242,     0,
     315,   318,   305,     0,   486,   485,     0,   487,     0,     0,
       0,   465,   464,   471,   472,   473,   470,   458,   459,     0,
       0,   466,   467,   468,   469,   474,   475,   451,   452,   453,
       0,     0,     0,     0,   436,   512,   513,     0,   510,     0,
     262,     0,     0,     0,     0,   302,   300,   326,     0,   285,
      20,    29,     0,     0,     0,    18,    22,    24,    11,    47,
     278,   248,   250,     0,   268,   282,   177,   177,   177,   177,
     115,   116,   323,   324,    98,    98,    98,     0,   322,     0,
     304,   329,   490,     0,     0,     0,   461,   463,   455,   457,
     489,     0,   481,   514,     0,   264,   266,   258,   260,   334,
     242,   241,     0,     0,     0,    27,     0,    16,    13,    49,
       0,     0,    89,   178,   112,   114,   113,     0,     0,     0,
     310,   314,     0,   476,   438,     0,   436,   294,   242,     0,
     292,     0,   286,     0,    26,     0,    25,    21,    19,     0,
       0,     0,     0,     0,   155,   159,     0,   153,   163,     0,
     161,     0,   488,   482,   242,   298,   295,   244,   293,   242,
       0,    30,     0,    28,    15,   199,    12,   335,     0,    80,
     177,   177,   177,   124,   122,   169,     0,   125,   126,   182,
     184,   185,   186,   187,   188,   233,   235,   183,    14,    43,
      44,    32,    35,    33,    98,    34,   117,    98,   130,    39,
      98,    38,    36,   180,    37,    41,    42,    40,    51,     0,
       0,    50,    53,     0,    52,   242,     0,    93,     0,     0,
      91,     0,   156,     0,    99,     0,   101,     0,   100,   297,
     296,     0,    23,     0,   197,   338,     0,    82,    83,     0,
      81,   103,   104,   105,   165,     0,   496,   170,     0,     0,
     242,   242,     0,     0,     0,     0,     0,   496,   127,   131,
     189,     0,     0,    57,     0,   252,     0,     0,     0,     0,
      90,     0,   157,   158,   154,   164,   162,    31,     0,   200,
       0,   336,     0,    76,     0,     0,   130,   173,   175,   174,
     176,     0,     0,     0,   133,   497,     0,   150,     0,   234,
     236,   108,   106,     0,   136,   163,     0,   134,     0,     0,
     128,   129,   121,     0,   181,     0,   191,    54,     0,    62,
      48,   517,   516,   179,    96,    97,   436,    94,    95,    92,
       0,     0,   205,     0,   436,   215,     0,     0,     0,   213,
     340,     0,   344,     0,     0,     0,     0,     0,     0,     0,
       0,   337,   339,   341,   342,     0,   343,    84,    78,     0,
       0,     0,   496,     0,     0,     0,   498,   499,     0,   151,
     109,     0,   102,   140,   137,     0,   120,   118,   119,   144,
       0,   142,     0,     0,     0,     0,   193,     0,   421,   420,
       0,    62,    59,     0,     0,     0,     0,   201,     0,   515,
     225,     0,     0,   221,   223,   222,   229,   227,     0,   216,
       0,   198,     0,     0,   400,   400,     0,     0,   400,     0,
     400,     0,     0,     0,   242,    84,   166,   167,   168,   123,
       0,     0,   501,     0,     0,   110,   111,   107,     0,     0,
     138,   139,   135,   145,     0,   132,   148,   209,     0,   207,
       0,     0,   190,     0,    56,    58,    60,     0,    64,    67,
      66,    69,    68,    65,    63,    61,     0,   160,   206,     0,
       0,   220,     0,     0,     0,   217,   218,   214,     0,   378,
       0,   401,   402,     0,     0,     0,     0,   405,   406,     0,
       0,     0,     0,     0,     0,     0,     0,   355,   363,   380,
     381,     0,   353,     0,     0,    85,    86,    88,    87,     0,
     242,   171,   172,     0,   500,   152,   141,   146,   147,   143,
       0,   210,     0,   196,     0,   194,   192,    73,    72,    71,
      74,    75,     0,     0,     0,   203,     0,   226,   224,   230,
     219,   383,   345,   403,   400,   396,   422,   400,   407,     0,
     398,     0,   400,   400,   400,   400,     0,     0,   374,   375,
     376,   377,     0,     0,     0,     0,     0,     0,   369,   373,
      77,     0,   502,   149,   211,   212,   208,   195,     0,   439,
       0,     0,   232,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,   356,   357,   354,
       0,   360,     0,   366,     0,   358,     0,     0,   373,   346,
      79,     0,    70,   204,   231,   411,   409,     0,     0,     0,
       0,   419,   397,   412,     0,   436,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,   364,     0,
       0,     0,     0,     0,   351,     0,     0,   503,     0,   404,
     393,     0,   414,     0,     0,     0,     0,     0,   423,   393,
     408,   395,   394,     0,   393,   387,   393,   393,     0,     0,
       0,   352,     0,     0,     0,   361,   362,   359,     0,     0,
     410,     0,   413,   415,   418,   416,   417,     0,     0,   386,
       0,     0,     0,     0,     0,   365,     0,   379,     0,     0,
     367,     0,     0,     0,   370,     0,   424,     0,     0,     0,
       0,   393,     0,     0,     0,     0,     0,     0,     0,   384,
     385,   388,   389,   390,     0,     0,   347,     0,   368,     0,
       0,     0,   371,     0,   392,   349,   348,     0,     0,   391,
     350,     0,     0,     0,     0,   372,     0,     0,     0
};

static const short yydefgoto[] =
{
     996,    25,    26,    27,    43,   151,   358,   389,   241,   304,
     305,   306,   354,   307,   438,    28,    44,   152,   359,   390,
     461,   462,   463,   514,   569,   641,   642,   643,   714,   782,
     439,   607,   440,   675,   489,   674,   755,   322,   393,   469,
     470,   367,   323,   758,   444,   553,   324,   325,   326,   503,
     445,   446,   447,   494,   448,   507,   449,   563,   450,   556,
     557,   688,   630,   631,   770,   451,   498,   396,   397,   401,
     399,   536,   496,   497,   541,   542,   362,   363,   452,   510,
     453,   564,   565,   637,   635,   636,   454,   530,   484,   581,
     698,   588,   589,   652,   653,   654,   655,   656,   455,   500,
     456,   501,   146,   147,    42,    30,   135,    46,    47,    48,
      49,    85,    86,   139,   140,   136,   137,   138,    50,    51,
      52,    45,    53,    54,    55,   351,    78,    31,   141,   379,
     380,   381,   173,   174,    32,    57,   165,    33,    59,   169,
     104,   106,   250,   328,    34,    35,    61,    36,    37,   457,
     485,   531,   532,   602,   603,   663,   604,   673,   751,   854,
     752,   855,   817,   818,   813,   814,   728,   946,   753,   605,
     606,   879,   912,   734,   740,   735,   833,   741,   838,   867,
     872,   873,   640,   795,    38,    66,   158,   195,   819,   575,
     376,   280,   281,   269,   270,   129,   130,   131,   132,    90,
     544,   545,   683,    40,    41,    72,   133,   227,   255,   573,
     148
};

static const short yypact[] =
{
     685,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    34,    46,
     -52,-32768,    76,   232,    68,    49,-32768,-32768,-32768,-32768,
     500,   -52,-32768,-32768,-32768,-32768,    14,-32768,-32768,     5,
  -32768,-32768,   151,    -8,    37,    90,  1425,   209,   237,   275,
     354,   322,   322,    90,    90,    90,   355,-32768,   363,-32768,
  -32768,   380,-32768,-32768,-32768,-32768,    83,    89,  2201,-32768,
  -32768,-32768,   322,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     326,   433,  1670,  2201,-32768,  1375,  1375,  2201,-32768,-32768,
  -32768,-32768,   273,-32768,  2201,  2201,    90,  2201,   -52,   -52,
     -52,   285,   -52,-32768,   912,-32768,  1049,  2201,    90,    85,
  -32768,   447,   447,   447,   447,   447,   447,  1719,   447,   447,
     447,   447,   482,-32768,-32768,-32768,  1670,   304,  2415,-32768,
  -32768,-32768,   405,  2173,-32768,   487,  2201,   393,   399,   393,
     399,  2201,-32768,   117,  2537,  1883,-32768,-32768,-32768,-32768,
    1602,   510,   510,  2201,  1602,  1602,   -52,   443,   304,  2537,
  -32768,-32768,-32768,  2201,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   150,-32768,  2537,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   304,   304,-32768,
  -32768,-32768,-32768,  2201,   125,   450,  2053,-32768,-32768,  2201,
    2201,  2201,  2201,  2201,  2201,  2201,  2201,  2201,  2201,-32768,
  -32768,  2201,  2201,  2201,  2201,  2201,  2201,  2201,  2201,  2201,
  -32768,-32768,  2201,  2201,  2201,  2201,  2537,   162,  2201,  1554,
    2201,  2201,  2201,  2201,  2537,  1758,   -52,  2201,-32768,  1375,
      69,   -52,   -52,  1554,  1425,  1425,  2201,-32768,  1375,  1554,
     423,   423,  2201,   -52,-32768,-32768,   150,-32768,  1670,  2441,
    2465,  1524,  2556,  1421,  1471,  1471,   863,   905,   905,  2201,
    2201,   782,   782,   782,   782,   579,   579,   573,   573,-32768,
    2201,  2201,   150,   587,  1966,  2537,  2537,  2173,-32768,  1602,
  -32768,  1554,  1554,  1554,  1554,-32768,  2537,-32768,  2005,   593,
  -32768,   590,   605,   617,   194,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1554,-32768,-32768,   598,   598,   598,   598,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1122,-32768,  1169,
  -32768,-32768,-32768,   320,  2201,  2201,   905,   905,-32768,-32768,
  -32768,  2201,-32768,-32768,  1631,-32768,-32768,-32768,-32768,-32768,
    1375,-32768,  2201,   632,   348,-32768,   102,-32768,-32768,-32768,
      90,  2201,-32768,-32768,-32768,-32768,-32768,    38,   641,   643,
  -32768,-32768,   551,  2537,  2537,   553,  2537,    17,  1323,   420,
  -32768,    25,-32768,     0,   -25,   617,-32768,-32768,-32768,  2331,
     404,   304,   686,    39,-32768,   677,     1,-32768,-32768,   190,
  -32768,   191,-32768,-32768,  1375,-32768,    17,-32768,-32768,  1375,
    2201,-32768,   304,-32768,-32768,   577,-32768,-32768,   682,   210,
     598,   598,   598,-32768,-32768,   690,   322,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    40,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   687,
     691,-32768,-32768,   673,-32768,  1425,  2201,-32768,   600,     9,
  -32768,  2201,   694,    50,-32768,   697,-32768,   699,-32768,-32768,
  -32768,   610,-32768,  2231,-32768,-32768,   -52,-32768,-32768,   700,
  -32768,-32768,-32768,-32768,   713,   569,   611,-32768,   257,   616,
    1425,  1425,   211,   293,   641,   598,   598,   611,-32768,-32768,
     611,   -52,   618,-32768,   688,-32768,     8,  2201,   715,   295,
  -32768,   743,-32768,-32768,-32768,-32768,-32768,-32768,  1719,-32768,
      63,-32768,   592,-32768,   -52,   555,    40,-32768,-32768,-32768,
  -32768,   627,   629,   400,-32768,-32768,   322,-32768,  2201,-32768,
  -32768,-32768,-32768,    13,-32768,   625,    47,-32768,   190,   190,
  -32768,-32768,-32768,    88,-32768,   343,-32768,-32768,   392,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  2489,-32768,-32768,-32768,
    2201,   312,-32768,   336,  2415,-32768,   390,   729,    54,-32768,
  -32768,   730,-32768,   732,   733,   734,   735,   736,   737,   739,
     740,-32768,-32768,-32768,-32768,   742,-32768,-32768,-32768,   304,
     304,   304,   611,   464,   517,  2201,-32768,-32768,   640,-32768,
     741,   314,-32768,-32768,   756,   315,-32768,-32768,-32768,-32768,
      66,-32768,   662,  1807,   744,   280,-32768,   343,-32768,-32768,
     -52,   738,-32768,   384,  2201,   667,  2201,-32768,  2201,    65,
  -32768,   763,   304,   674,-32768,   696,-32768,  2537,   390,   444,
     308,-32768,  2201,    39,   452,   452,   459,   459,   452,   459,
     452,   459,   207,   668,  1271,-32768,-32768,-32768,-32768,-32768,
     304,   304,   698,   304,  2201,-32768,-32768,-32768,  2201,   625,
  -32768,-32768,-32768,    90,    96,-32768,-32768,-32768,    67,  2537,
    1807,   343,-32768,   280,-32768,-32768,-32768,   502,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  2513,-32768,-32768,   409,
     783,-32768,  1281,   693,   304,-32768,-32768,-32768,   304,  2537,
       9,-32768,-32768,   794,   701,   322,   702,-32768,-32768,   796,
     704,   322,   705,   706,   709,   714,   716,-32768,   797,-32768,
  -32768,    91,-32768,   345,  2263,-32768,-32768,-32768,-32768,   754,
    1271,-32768,-32768,  2201,-32768,-32768,  2537,-32768,-32768,-32768,
    2201,  2201,  1846,-32768,    67,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   502,  2201,  2201,-32768,   549,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   452,   833,   814,   452,-32768,  2201,
     837,  2201,   452,   452,   452,   452,  2201,   811,-32768,-32768,
  -32768,-32768,   346,   483,   332,   242,  1719,   -52,-32768,-32768,
  -32768,   788,   724,  2537,  2537,-32768,  2537,-32768,   304,  2537,
     718,  1602,-32768,    44,   726,   426,  2201,   727,    44,  2088,
     426,  1578,   728,   731,   745,   746,    19,-32768,-32768,-32768,
     349,-32768,   349,   826,    82,-32768,   828,   747,   304,-32768,
  -32768,  2201,-32768,-32768,-32768,-32768,   748,     8,  2201,   426,
     823,-32768,-32768,   649,   856,  2029,  2201,     8,   827,   304,
  -32768,  2201,   -52,  2201,  2201,  2201,  2201,  2201,-32768,    82,
      21,  2201,   830,   376,-32768,   349,  2201,-32768,    44,-32768,
    2088,   759,-32768,   392,   392,   392,   392,  2201,-32768,  2088,
  -32768,-32768,-32768,   -52,  2088,-32768,  2088,  2088,  2127,  2151,
       8,-32768,  2201,   861,    20,-32768,-32768,-32768,   250,   418,
  -32768,   304,-32768,-32768,-32768,-32768,-32768,   764,   304,-32768,
     304,   304,   304,  2201,  2201,-32768,   304,  2537,  2201,  2201,
  -32768,  2201,   862,  2201,-32768,   -52,-32768,   -52,   -52,   -52,
     -52,  2088,  1602,   304,   304,     8,   304,  2201,   429,-32768,
  -32768,-32768,-32768,-32768,   304,   -52,-32768,   304,-32768,   304,
     304,  2201,-32768,   -52,-32768,-32768,-32768,   304,   767,-32768,
  -32768,  2201,   769,  2201,   304,-32768,   870,   871,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,   847,-32768,-32768,-32768,-32768,-32768,   749,-32768,
     544,   518,-32768,  -270,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   263,-32768,-32768,   123,
  -32768,-32768,-32768,-32768,-32768,   234,-32768,  -388,-32768,   244,
      36,   -79,  -386,   255,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   374,-32768,-32768,-32768,-32768,
      27,-32768,-32768,   -18,-32768,-32768,-32768,-32768,   196,-32768,
     184,-32768,-32768,-32768,   298,   301,    22,  -169,-32768,-32768,
  -32768,-32768,-32768,-32768,   278,   216,-32768,-32768,-32768,-32768,
     218,-32768,    32,   261,-32768,   198,-32768,   219,-32768,-32768,
  -32768,-32768,   -43,    15,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   -92,-32768,-32768,
     542,   -69,  -149,   689,-32768,-32768,-32768,-32768,-32768,-32768,
     695,   692,   775,-32768,-32768,-32768,-32768,   -37,     3,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   199,  -794,
    -319,  -187,-32768,  -753,  -859,  -204,-32768,  -650,-32768,-32768,
  -32768,  -427,-32768,  -407,  -365,-32768,-32768,-32768,-32768,  -808,
    -775,    77,  -190,   212,   205,-32768,-32768,  -217,   -63,-32768,
     -45,-32768,-32768,-32768,-32768,   583,-32768,-32768,   303,-32768,
    -447,   441,-32768,   475,   478,-32768,-32768,   669,   137,  -612,
     -20
};


#define	YYLAST		2585


static const short yytable[] =
{
      62,   441,   472,   442,   156,   127,   283,   410,    92,   571,
     518,    79,   301,   143,   620,    29,   100,   101,   102,    84,
     571,   571,   892,   128,  -301,    64,   887,   949,   922,    88,
     877,   923,   409,   355,    80,    82,    83,   144,   145,   394,
     467,    56,   150,   149,   256,   865,   808,   809,   624,   154,
     155,   523,   159,    58,   188,   659,   889,   194,   890,   157,
     562,    93,   175,   857,   585,   880,  -202,   693,   771,   952,
     300,   361,   128,   282,    91,   395,   468,    63,   160,   161,
     162,   196,   164,   892,   108,    60,   178,   395,   226,   629,
     930,   229,   807,   586,   901,  -202,   234,   768,    71,     5,
     587,   928,  -202,   387,     6,   303,   301,    89,   243,   505,
     506,   176,   179,     3,     4,   413,   808,   809,   249,   168,
      83,   172,     3,     4,   375,     3,     4,     3,     4,   473,
     810,   811,   411,     3,     4,   383,   246,   519,    60,   301,
     572,   621,    60,   929,   392,  -301,    60,    81,   175,   893,
      60,   572,   572,   235,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   866,   679,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   625,    73,   175,   284,   285,
     286,    87,   660,   289,    60,   291,   292,   293,   294,   333,
     296,    60,   298,   481,   694,   772,   299,   -26,   302,   303,
     968,   313,   197,    60,   -26,   314,    22,   175,   747,   290,
     893,   109,   551,   144,   110,    22,   297,   197,    22,   812,
      22,   308,   309,   310,   336,   337,    22,    65,   988,   315,
     -26,   302,   303,   331,   888,   338,   339,   -26,   992,    94,
     994,   361,   226,   807,   748,   235,   368,   369,   552,   516,
     490,   892,   197,   235,   521,   899,   257,   951,   736,   311,
     312,   743,    68,   745,   198,   910,   529,    95,   391,     3,
       4,   345,   346,   347,   348,   808,   809,   287,   252,   508,
     236,   810,   811,   487,   488,   197,   756,   239,   757,   373,
     374,   244,   245,   360,   554,   248,   578,   197,   964,   144,
     574,   966,   742,    39,   744,    96,   746,   382,   945,   726,
     253,   582,   950,   177,   180,   686,   691,   980,   475,   477,
     749,   750,   356,   157,   254,    67,    70,    60,    60,   197,
     555,   619,   468,   851,   144,   405,   560,   561,   586,   364,
     365,   366,   168,    69,   172,   587,   747,   848,    67,    39,
     851,   687,   689,   978,    98,    99,    67,    67,    67,     3,
       4,   479,   852,   645,   288,   502,   480,   508,   504,   853,
     812,   509,   756,   633,   757,   134,   474,   926,   893,   476,
     634,   478,   748,   748,    97,   546,   853,   834,    39,    39,
     837,   650,   103,   332,    60,   842,   843,   844,   845,    67,
     105,   111,   112,   113,   114,   458,   153,    39,   701,    39,
     107,    67,    67,   853,   707,   115,   116,    60,   163,   340,
     117,   406,   118,   119,   120,   121,   344,     3,     4,   718,
     615,   111,   112,   113,   114,   222,   223,     3,     4,   197,
     646,   357,   491,   492,   493,   115,   116,   647,   235,   520,
     117,   372,   118,   119,   120,   121,   869,     3,     4,   870,
    -263,   -55,  -265,   871,   648,   583,   533,   765,   420,   421,
     422,   649,   576,   931,   586,   459,   385,   117,   547,   386,
     515,   587,   938,   584,     3,     4,   316,   940,   847,   941,
     942,   567,   460,   849,   317,   318,   319,   320,   321,   122,
     123,   124,   125,   638,   639,   708,   709,   710,   711,   712,
     713,   616,   193,   407,   608,   549,   550,   228,  -228,   651,
     126,    60,   810,   811,   378,  -228,   632,    23,   465,   122,
     123,   124,   125,   622,   974,    24,   626,   784,   627,   628,
     240,   657,    39,   538,   785,   540,   953,    39,    39,   482,
     126,    39,   682,   197,   577,   579,   122,   981,   124,   125,
     111,   112,   113,   114,   197,   731,   732,  -257,   661,  -259,
     128,   733,   737,   738,   115,   116,   247,   126,   739,   117,
     258,   118,   119,   120,   121,   719,     3,     4,   699,   846,
      75,    76,    77,   590,   341,   537,   591,   539,   350,   716,
     219,   220,   221,   128,   217,   218,   219,   220,   221,   850,
     695,   856,   230,   657,   232,   702,   231,   729,   233,   874,
     704,   352,  -382,   715,   777,   778,   779,   780,   781,   361,
      39,   759,    39,   592,   593,   594,   595,   596,   597,   598,
     599,   600,   353,   766,   443,   464,   601,   537,   538,   539,
     540,   690,   692,    39,   301,   699,   632,   632,   122,   123,
     124,   125,   384,    67,   903,   904,   905,   906,   522,   524,
     920,   609,   610,   611,   924,   767,   769,   657,   398,   126,
     400,    39,   402,   776,   197,   403,     1,   558,   559,     2,
     937,   725,   727,   466,   181,   182,   183,   184,   185,   186,
     822,   189,   190,   191,   192,   925,   927,    39,   471,   128,
     792,   483,    39,   933,   934,   935,   936,   821,   128,   486,
     495,   830,     3,     4,   511,   823,   824,   826,   512,   499,
     513,   395,   965,   517,   525,     5,   526,   534,   829,   128,
       6,   831,   527,   535,   570,   543,   676,   677,   678,   548,
     580,   568,   468,   858,   839,   613,   841,   614,   623,   658,
     662,     7,   664,   665,   666,   667,   668,   669,    39,   670,
     671,   128,   672,   684,   700,  -242,  -242,  -242,   685,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,   721,
      18,   875,    19,   689,    20,   696,   705,   859,   897,   717,
     720,   754,   722,    39,    39,   215,   216,   217,   218,   219,
     220,   221,    21,   786,   820,    22,   128,   761,   762,    23,
     764,    24,   651,   900,   723,   793,   763,   798,   806,   794,
     797,   909,   799,   801,   802,   773,   914,   803,   916,   917,
     918,   919,   804,   835,   805,   836,   617,   840,   748,   618,
     860,   128,   861,   863,   868,   876,   883,   891,   895,   884,
     871,   790,   915,   907,   911,   791,    67,   853,   948,   967,
     997,   998,    74,   885,   886,   896,   898,   947,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   939,   932,   991,   956,   993,   961,   962,
     388,   242,   412,   947,   706,   828,   947,   730,   128,   760,
     612,   827,   681,   166,   680,   703,     2,   775,   774,   724,
     788,   408,   947,   832,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   969,   128,   970,   971,   972,
     973,   330,   789,   329,   251,   327,   128,   902,   128,     3,
       4,   566,   815,   800,     0,   984,   343,     0,     0,     0,
       0,     0,     0,   989,     0,   862,     0,     0,   864,     0,
       0,     0,     0,     0,     0,     0,     0,    39,   882,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,   894,     0,     0,     0,     0,    67,    67,     0,     0,
       0,     0,  -242,  -242,  -242,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,   913,    18,   167,    19,
       0,    20,     0,     0,     0,     0,   921,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   796,     0,
       0,     0,    22,     0,   796,     0,    23,     0,    24,     0,
     170,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,     0,     0,   954,     0,   955,     0,
       0,     0,     0,     0,     0,   957,     0,   958,   959,   960,
       0,     0,     0,   963,     0,     0,     3,     4,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   975,
     976,   977,     0,   979,     0,   982,     0,     0,     0,     0,
       0,   983,     0,     0,   985,     0,   986,   987,     0,     0,
       0,     0,     0,   166,   990,     7,     2,     0,     0,     0,
       0,   995,     0,     0,     0,     0,     0,     0,     0,  -242,
    -242,  -242,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,     0,    18,     0,    19,   171,    20,     3,
       4,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     170,     0,     0,     2,     0,     0,     0,     0,     0,    22,
       0,     0,     0,    23,     0,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     0,     0,
       0,     0,  -242,  -242,  -242,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,     0,    18,   370,    19,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     0,     0,     0,     0,
       0,     0,    22,     0,     0,     0,    23,     0,    24,  -242,
    -242,  -242,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,     0,    18,     2,    19,   371,    20,     0,
       0,     0,   787,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,   112,   113,   114,     0,     0,     0,    22,
       0,     0,     0,    23,     0,    24,   115,   116,     3,     4,
       0,   117,     0,   118,   119,   120,   121,     0,     3,     4,
       0,     0,     0,     0,     0,     0,     0,     2,     0,     0,
     404,     0,     0,     0,   316,   420,   421,   422,     0,     0,
       0,     0,   317,   318,   319,   320,   321,     7,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     0,     0,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    18,     0,    19,     2,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     122,   123,   124,   125,     0,     0,     0,     0,     0,     7,
       0,    22,     0,     0,     0,    23,     0,    24,    60,  -228,
       0,   126,     3,     4,     0,     0,  -228,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,     0,    18,     2,
      19,     0,    20,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,     7,     0,    22,     0,     0,     0,    23,     0,    24,
      60,     0,     3,     4,     0,     0,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,     0,    20,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,     7,     0,     0,     0,    22,     0,     0,     0,    23,
       0,    24,    60,     0,     0,     0,     0,     0,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,   202,    20,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,     0,    22,     0,     0,     0,    23,
     199,    24,   201,   202,     0,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   199,     0,   201,   202,     0,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   199,     0,
     201,   202,     0,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   377,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,   112,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   116,     0,     0,
       0,   117,     0,   118,   119,   120,   121,     0,     3,     4,
       0,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,   112,   113,   114,     0,     0,     0,     0,     0,
       0,    60,     0,     0,     0,   115,   116,     0,     0,     0,
     117,     0,   118,   119,   120,   121,   881,     3,     4,     0,
       0,     0,     0,   197,     0,     0,     0,     0,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   112,   113,   114,     0,   378,     0,   197,     0,     0,
     122,   123,   124,   125,   115,   116,     0,     0,     0,   117,
       0,   118,   119,   120,   121,     0,     3,     4,     0,   295,
       0,   126,     0,     0,     0,     0,     0,     0,     0,   111,
     112,   113,   114,     0,     0,     0,     0,     0,     0,   122,
     123,   124,   125,   115,   116,     0,     0,     0,   117,     0,
     118,   119,   120,   121,     0,     3,     4,     0,     0,     0,
     126,     0,     0,     0,     0,     0,     0,     0,   697,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,   112,
     113,   114,     0,     0,     0,     0,     0,     0,   122,   123,
     124,   125,   115,   116,     0,     0,     0,   117,     0,   118,
     119,   120,   121,     0,     3,     4,     0,   825,     0,   126,
       0,     0,     0,     0,     0,     0,     0,   111,   112,   113,
     114,     0,     0,     0,     0,     0,     0,   122,   123,   124,
     125,   115,   116,     0,     0,     0,   117,     0,   118,   119,
     120,   121,     0,     3,     4,     0,     0,     0,   126,   199,
     237,   201,   202,     0,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,     0,     0,     0,   122,   123,   124,   125,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   126,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,   123,   124,   125,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   199,     0,   201,   202,   126,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   199,     0,   201,   202,   238,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   199,     0,   201,   202,     0,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   199,
       0,   201,   202,     0,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   199,     0,   201,   202,   342,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   199,     0,   201,   202,   349,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   199,     0,   201,
     202,   908,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,     0,     0,  -436,   111,   112,   113,   114,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,   116,
       0,     0,     0,   117,     0,   118,   119,   120,   121,     0,
       3,     4,   111,   112,   113,   114,   878,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,   116,     0,     0,
       0,   117,     0,   118,   119,   120,   121,     0,     3,     4,
       0,     0,   111,   112,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   943,   115,   116,     0,     0,
       0,   528,     0,   118,   119,   120,   121,     0,     3,     4,
       0,     0,     0,     0,   111,   112,   113,   114,     0,   944,
       0,     0,   122,   123,   124,   125,   224,   225,   115,   116,
       0,     0,     0,   816,     0,   118,   119,   120,   121,     0,
       3,     4,     0,   126,     0,     0,     0,     0,     0,     0,
     122,   123,   124,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,   414,     0,     0,     0,     0,     0,     0,     0,
     122,   123,   124,   125,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   126,     0,     0,     0,     0,     0,     0,   415,     0,
       0,     0,   122,   123,   124,   125,     0,     0,     0,     0,
       0,     0,     0,   416,   417,     0,     0,     0,     0,     0,
     418,     0,   419,   126,   316,   420,   421,   422,   423,   424,
       0,     0,   317,   318,   319,   320,   321,   425,   426,     0,
       0,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   199,   200,   201,   202,     0,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,     0,   437,   199,   334,   201,
     202,     0,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   199,   335,   201,   202,     0,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   199,   644,   201,   202,     0,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   199,
     783,   201,   202,     0,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   199,     0,   201,   202,     0,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221
};

static const short yycheck[] =
{
      20,   389,     1,   389,    96,    68,   223,     7,    45,     1,
       1,    31,    37,    82,     1,     0,    53,    54,    55,    39,
       1,     1,     1,    68,     7,    22,     7,     7,     7,    37,
     838,   890,     7,   303,    20,    30,    31,    82,    83,     1,
       1,     7,    87,    86,   193,     1,    25,    26,     1,    94,
      95,     1,    97,     7,   117,     1,   850,   126,   852,    96,
     507,    46,   107,   816,     1,   840,     1,     1,     1,   928,
       1,    31,   117,   222,    37,    37,    37,     1,    98,    99,
     100,   126,   102,     1,     1,   137,     1,    37,   133,     1,
     898,   136,     1,    30,   869,    30,   141,     1,    30,    50,
      37,   895,    37,     1,    55,   130,    37,   115,   153,    69,
      70,   108,   109,    37,    38,   385,    25,    26,   163,   104,
      31,   106,    37,    38,   341,    37,    38,    37,    38,   128,
      39,    40,   132,    37,    38,   352,   156,   128,   137,    37,
     132,   128,   137,   896,   361,   128,   137,   133,   193,   128,
     137,   132,   132,   128,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   120,   612,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   128,   127,   222,   223,   224,
     225,    30,   128,   228,   137,   230,   231,   232,   233,   258,
     235,   137,   237,   410,   128,   128,   239,   128,   129,   130,
     953,   246,   135,   137,   135,   248,   130,   252,     1,   229,
     128,   128,     1,   258,   131,   130,   236,   135,   130,   128,
     130,   241,   242,   243,   269,   270,   130,    22,   981,   249,
     128,   129,   130,   253,   846,   280,   281,   135,   991,    30,
     993,    31,   287,     1,    37,   128,   325,   326,    37,   466,
     419,     1,   135,   128,   471,   867,   131,     7,   665,   244,
     245,   668,    30,   670,   127,   877,   483,    30,   360,    37,
      38,   291,   292,   293,   294,    25,    26,   115,   128,   448,
     143,    39,    40,    73,    74,   135,   674,   150,   674,   334,
     335,   154,   155,   313,     1,   158,     1,   135,   948,   344,
     517,   951,   667,     0,   669,    30,   671,   350,   920,     1,
     173,   528,   924,   108,   109,     1,     1,   967,   128,   128,
     113,   114,   128,   360,   187,    22,    23,   137,   137,   135,
      37,   548,    37,     1,   379,   378,   505,   506,    30,   317,
     318,   319,   327,   111,   329,    37,     1,     1,    45,    46,
       1,    37,    37,   965,    51,    52,    53,    54,    55,    37,
      38,   404,    30,   580,   227,   444,   409,   536,   447,    37,
     128,   450,   760,    30,   760,    72,   396,     1,   128,   399,
      37,   401,    37,    37,    30,   128,    37,   794,    85,    86,
     797,     1,    37,   256,   137,   802,   803,   804,   805,    96,
      37,    11,    12,    13,    14,     1,   133,   104,   128,   106,
      30,   108,   109,    37,    30,    25,    26,   137,   133,   282,
      30,     1,    32,    33,    34,    35,   289,    37,    38,   646,
      30,    11,    12,    13,    14,    30,    31,    37,    38,   135,
     128,   304,   420,   421,   422,    25,    26,   135,   128,   469,
      30,   131,    32,    33,    34,    35,    30,    37,    38,    33,
     134,    57,   136,    37,   128,   528,   486,   684,    64,    65,
      66,   135,   517,   900,    30,    71,   128,    30,   498,   131,
     465,    37,   909,   528,    37,    38,    63,   914,   807,   916,
     917,   511,    88,   812,    71,    72,    73,    74,    75,   109,
     110,   111,   112,   111,   112,   121,   122,   123,   124,   125,
     126,   111,    30,    93,   534,   500,   501,    30,   128,   129,
     130,   137,    39,    40,   104,   135,   563,   134,   391,   109,
     110,   111,   112,   553,   961,   136,   556,   128,   558,   559,
      30,   586,   239,    79,   135,    81,   128,   244,   245,   412,
     130,   248,   615,   135,   518,   519,   109,   128,   111,   112,
      11,    12,    13,    14,   135,   113,   114,   134,   588,   136,
     615,   119,   113,   114,    25,    26,   133,   130,   119,    30,
     130,    32,    33,    34,    35,   648,    37,    38,   633,   806,
      90,    91,    92,     1,     7,    78,     4,    80,     5,   644,
      27,    28,    29,   648,    25,    26,    27,    28,    29,   813,
     630,   815,   137,   658,   139,   635,   138,   662,   140,   836,
     640,    31,    30,   643,   122,   123,   124,   125,   126,    31,
     327,   674,   329,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    37,   688,   389,   390,    54,    78,    79,    80,
      81,   624,   625,   350,    37,   700,   693,   694,   109,   110,
     111,   112,    30,   360,    15,    16,    17,    18,   472,   473,
     887,   116,   117,   118,   891,   693,   694,   722,    37,   130,
      37,   378,   131,   703,   135,   132,     1,   503,   504,     4,
     907,   659,   660,     7,   111,   112,   113,   114,   115,   116,
     763,   118,   119,   120,   121,   892,   893,   404,    31,   754,
     730,   134,   409,   903,   904,   905,   906,   760,   763,    37,
      30,   784,    37,    38,    37,   770,   771,   772,    37,   426,
      57,    37,   949,   133,    37,    50,    37,    37,   783,   784,
      55,   786,   132,    30,    56,   134,   609,   610,   611,   133,
       7,   133,    37,   816,   799,   128,   801,   128,   133,    30,
      30,    76,    30,    30,    30,    30,    30,    30,   465,    30,
      30,   816,    30,   133,    30,    90,    91,    92,    37,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   652,
     105,   836,   107,    37,   109,   133,    58,   817,   861,   132,
      37,   133,   128,   500,   501,    23,    24,    25,    26,    27,
      28,    29,   127,    30,    60,   130,   861,   680,   681,   134,
     683,   136,   129,   868,   128,    31,   128,    31,    31,   128,
     128,   876,   128,   128,   128,   698,   881,   128,   883,   884,
     885,   886,   128,    10,   128,    31,   543,    10,    37,   546,
      62,   896,   128,   135,   128,   128,   128,    31,    30,   128,
      37,   724,   882,     7,    37,   728,   563,    37,     7,     7,
       0,     0,    25,   128,   128,   128,   128,   922,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,   913,   135,   128,   132,   128,   943,   944,
     356,   152,   384,   948,   641,   782,   951,   663,   953,   675,
     536,   774,   614,     1,   613,   637,     4,   701,   700,   658,
     722,   379,   967,   786,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,   955,   981,   957,   958,   959,
     960,   252,   723,   251,   169,   250,   991,   870,   993,    37,
      38,   510,   753,   741,    -1,   975,   287,    -1,    -1,    -1,
      -1,    -1,    -1,   983,    -1,   828,    -1,    -1,   831,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   674,   841,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,   854,    -1,    -1,    -1,    -1,   693,   694,    -1,    -1,
      -1,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   879,   105,   106,   107,
      -1,   109,    -1,    -1,    -1,    -1,   889,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   735,    -1,
      -1,    -1,   130,    -1,   741,    -1,   134,    -1,   136,    -1,
       1,    -1,    -1,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   760,    -1,    -1,   929,    -1,   931,    -1,
      -1,    -1,    -1,    -1,    -1,   938,    -1,   940,   941,   942,
      -1,    -1,    -1,   946,    -1,    -1,    37,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   962,
     963,   964,    -1,   966,    -1,   968,    -1,    -1,    -1,    -1,
      -1,   974,    -1,    -1,   977,    -1,   979,   980,    -1,    -1,
      -1,    -1,    -1,     1,   987,    76,     4,    -1,    -1,    -1,
      -1,   994,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,   107,   108,   109,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,     4,    -1,    -1,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      -1,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,   105,     4,   107,   108,   109,    -1,
      -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,   130,
      -1,    -1,    -1,   134,    -1,   136,    25,    26,    37,    38,
      -1,    30,    -1,    32,    33,    34,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,
       7,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,   107,     4,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,    -1,    76,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,   137,   128,
      -1,   130,    37,    38,    -1,    -1,   135,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,    -1,   105,     4,
     107,    -1,   109,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    76,    -1,   130,    -1,    -1,    -1,   134,    -1,   136,
     137,    -1,    37,    38,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,   107,    -1,   109,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    76,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,   137,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,   107,     9,   109,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,   130,    -1,    -1,    -1,   134,
       6,   136,     8,     9,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,     6,    -1,     8,     9,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     6,    -1,
       8,     9,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    30,    -1,    32,    33,    34,    35,    -1,    37,    38,
      -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,   137,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      30,    -1,    32,    33,    34,    35,   128,    37,    38,    -1,
      -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    12,    13,    14,    -1,   104,    -1,   135,    -1,    -1,
     109,   110,   111,   112,    25,    26,    -1,    -1,    -1,    30,
      -1,    32,    33,    34,    35,    -1,    37,    38,    -1,     1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,    25,    26,    -1,    -1,    -1,    30,    -1,
      32,    33,    34,    35,    -1,    37,    38,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,    25,    26,    -1,    -1,    -1,    30,    -1,    32,
      33,    34,    35,    -1,    37,    38,    -1,     1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    11,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,    25,    26,    -1,    -1,    -1,    30,    -1,    32,    33,
      34,    35,    -1,    37,    38,    -1,    -1,    -1,   130,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    -1,   109,   110,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     6,    -1,     8,     9,   130,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     6,    -1,     8,     9,   132,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     6,    -1,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     6,
      -1,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     6,    -1,     8,     9,   132,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     6,    -1,     8,     9,   132,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,     6,    -1,     8,
       9,   132,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,   130,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    30,    -1,    32,    33,    34,    35,    -1,
      37,    38,    11,    12,    13,    14,   128,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,    -1,
      -1,    30,    -1,    32,    33,    34,    35,    -1,    37,    38,
      -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,    25,    26,    -1,    -1,
      -1,    30,    -1,    32,    33,    34,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    11,    12,    13,    14,    -1,   128,
      -1,    -1,   109,   110,   111,   112,   113,   114,    25,    26,
      -1,    -1,    -1,    30,    -1,    32,    33,    34,    35,    -1,
      37,    38,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    61,   130,    63,    64,    65,    66,    67,    68,
      -1,    -1,    71,    72,    73,    74,    75,    76,    77,    -1,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,     6,     7,     8,     9,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,   115,     6,     7,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,     6,     7,     8,     9,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     6,    -1,     8,     9,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29
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

case 1:
#line 426 "parse.yy"
{ if (!is_interactive)
			    YYFAIL;
#if VDEBUG != 0
			  if (vdebug)
			    printf_error_V ("interactive statement\n");
#endif
			  interactive_statement = yyvsp[0].ttype;
			  fix_lastchar();
			  YYACCEPT;}
    break;
case 3:
#line 437 "parse.yy"
{ 
#if VDEBUG != 0
if (vdebug)
  printf ("source error\n");
#endif
		  if (is_interactive)
		    YYABORT;
		}
    break;
case 4:
#line 445 "parse.yy"
{ 
#if VDEBUG != 0
if (vdebug)
  printf_error_V ("EOF\n");
#endif
		 yyval.ttype = NULL; }
    break;
case 6:
#line 456 "parse.yy"
{ yyerrok;
#if VDEBUG != 0
if (vdebug)
  printf ("description_list\n");
#endif
	  	}
    break;
case 9:
#line 473 "parse.yy"
{ if (is_interactive) {
		    YYFAIL;
                  }
		}
    break;
case 10:
#line 478 "parse.yy"
{
		  current_module = current_scope = build_module (yyvsp[0].ttype);
		  push_scope ();
		  /* list of parse modules */
		  module_list = tree_cons (current_module, NULL_TREE, module_list);
		  /* for now, assume all to be at lop level */
//	       	  top_level = chainon (current_module, top_level);
		}
    break;
case 11:
#line 487 "parse.yy"
{ MODULE_PORT_LIST (current_module) = nreverse (yyvsp[-1].ttype); }
    break;
case 12:
#line 489 "parse.yy"
{ 
		  current_scope = pop_scope (); 
		  BLOCK_BODY (current_module) = nreverse (BLOCK_BODY (current_module));
		  BLOCK_DECL (current_module) = nreverse (BLOCK_DECL (current_module));
		  end_module (current_module);
		}
    break;
case 14:
#line 502 "parse.yy"
{ yyerrok; }
    break;
case 16:
#line 508 "parse.yy"
{ yyval.ttype = yyvsp[-1].ttype; }
    break;
case 17:
#line 510 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 19:
#line 516 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype);
		}
    break;
case 22:
#line 530 "parse.yy"
{ if (yyvsp[0].ttype)
		    yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE);
		  else
		    yyval.ttype = NULL_TREE;
		}
    break;
case 23:
#line 536 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[-1].ttype, yyvsp[-3].ttype); }
    break;
case 25:
#line 542 "parse.yy"
{ yyval.ttype = yyvsp[-1].ttype; 
		  sorry ("Port concatenations not supported in this version");
		}
    break;
case 26:
#line 546 "parse.yy"
{ yyval.ttype = NULL_TREE; }
    break;
case 27:
#line 551 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 28:
#line 553 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); }
    break;
case 29:
#line 558 "parse.yy"
{ set_decl (yyvsp[0].ttype, yyvsp[0].ttype); }
    break;
case 30:
#line 560 "parse.yy"
{ yyval.ttype = build_bit_ref (yyvsp[-3].ttype, yyvsp[-1].ttype);
		  set_decl (yyvsp[-3].ttype, yyval.ttype);
//		  sorry ("Port bit-select not supported in this version");
		}
    break;
case 31:
#line 565 "parse.yy"
{ yyval.ttype = build_part_ref (yyvsp[-5].ttype, yyvsp[-3].ttype, yyvsp[-1].ttype);
		  set_decl (yyvsp[-5].ttype, yyval.ttype);
//		  sorry ("Port part-selects not supported in this version");
		}
    break;
case 32:
#line 573 "parse.yy"
{}
    break;
case 33:
#line 575 "parse.yy"
{}
    break;
case 34:
#line 577 "parse.yy"
{}
    break;
case 35:
#line 579 "parse.yy"
{}
    break;
case 37:
#line 582 "parse.yy"
{}
    break;
case 38:
#line 584 "parse.yy"
{}
    break;
case 39:
#line 586 "parse.yy"
{}
    break;
case 40:
#line 588 "parse.yy"
{}
    break;
case 41:
#line 590 "parse.yy"
{ BLOCK_BODY (current_module) = tree_cons (yyvsp[0].ttype,
			(tree)INITIAL_CODE, BLOCK_BODY (current_module));
		}
    break;
case 42:
#line 594 "parse.yy"
{ BLOCK_BODY (current_module) = tree_cons (yyvsp[0].ttype,
			(tree)ALWAYS_CODE, BLOCK_BODY (current_module));
		}
    break;
case 43:
#line 598 "parse.yy"
{}
    break;
case 44:
#line 600 "parse.yy"
{}
    break;
case 45:
#line 615 "parse.yy"
{ if (is_interactive) {
		    YYFAIL;
                  }
		}
    break;
case 46:
#line 620 "parse.yy"
{
		  current_module = current_scope = build_udp (yyvsp[0].ttype);
		  push_scope ();
		  /* list of parse modules */
		  module_list = tree_cons (current_module, NULL_TREE, module_list);
		}
    break;
case 47:
#line 627 "parse.yy"
{ UDP_PORT_LIST (current_module) = nreverse (yyvsp[-1].ttype); }
    break;
case 48:
#line 629 "parse.yy"
{
		  current_scope = pop_scope (); 
		  UDP_STRING_LIST (current_module) = nreverse (UDP_STRING_LIST (current_module));
		  end_primitive (current_module);
		}
    break;
case 49:
#line 637 "parse.yy"
{ yyval.ttype = NULL_TREE; }
    break;
case 50:
#line 639 "parse.yy"
{ yyerrok; }
    break;
case 54:
#line 650 "parse.yy"
{
		if( strcmp( IDENT(yyvsp[-1].ttype), 
			IDENT(DECL_NAME(TREE_PURPOSE(MODULE_PORT_LIST(current_module)))) )!=0 ) {
			error( "only the first declared port in a udp can be a reg",
									NULL_CHAR, NULL_CHAR );		
		} else {	
			UDP_REG_NAME(current_module) = yyvsp[-1].ttype;
		}
		yyval.ttype = NULL_TREE;
	}
    break;
case 55:
#line 663 "parse.yy"
{ yyval.ttype = NULL_TREE; }
    break;
case 56:
#line 665 "parse.yy"
{
		if( UDP_REG_NAME(current_module)==NULL_TREE ) {
			error( "initial statement is no allowed in combinatorial udp's",
									NULL_CHAR, NULL_CHAR );		
		} else if( strcmp(IDENT(yyvsp[-3].ttype),IDENT(UDP_REG_NAME(current_module)))!=0 ) {
			error( "initial statement does not reference port output",
									NULL_CHAR, NULL_CHAR );		
		} else {
			UDP_INITIAL_VALUE(current_module) = yyvsp[-1].ttype;
		}
		yyval.ttype = NULL_TREE;
	}
    break;
case 57:
#line 683 "parse.yy"
{ 	extern int enable_udp_parse;
		enable_udp_parse = TRUE;
	    current_udp_string = NULL_TREE;
	}
    break;
case 58:
#line 688 "parse.yy"
{ 
		UDP_STRING_LIST(current_module) = yyvsp[-1].ttype;
		yyval.ttype = NULL_TREE;
	}
    break;
case 60:
#line 696 "parse.yy"
{ 
		yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-1].ttype);
	}
    break;
case 61:
#line 702 "parse.yy"
{ 
		validate_udp_string( current_module, current_udp_string );
		yyval.ttype = current_udp_string;
		current_udp_string = NULL_TREE;
	}
    break;
case 62:
#line 710 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 63:
#line 712 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 64:
#line 716 "parse.yy"
{
		append_udp_digits( &current_udp_string, yyvsp[0].c, yyvsp[0].c );
		yyval.ttype = NULL;
	}
    break;
case 65:
#line 721 "parse.yy"
{
		append_udp_digits( &current_udp_string,'?','?' );
		yyval.ttype = NULL;
	}
    break;
case 66:
#line 726 "parse.yy"
{
		append_udp_digits( &current_udp_string,'0','0' );
		yyval.ttype = NULL;
	}
    break;
case 67:
#line 731 "parse.yy"
{
		append_udp_digits( &current_udp_string,'1','1' );
		yyval.ttype = NULL;
	}
    break;
case 68:
#line 736 "parse.yy"
{
		append_udp_digits( &current_udp_string,'b','b' );
		yyval.ttype = NULL;
	}
    break;
case 69:
#line 741 "parse.yy"
{
		append_udp_digits( &current_udp_string,'x','x' );
		yyval.ttype = NULL;
	}
    break;
case 70:
#line 746 "parse.yy"
{ 		
		/* ? and b must be convert different but equivilent characters
		   otherwise edges cannot be distinguished from levels */

		if( yyvsp[-2].c=='?' ) {	
			yyvsp[-2].c = '!';
		} else if( yyvsp[-2].c=='b' ) {
			yyvsp[-2].c = '%';
		}
		append_udp_digits( &current_udp_string,yyvsp[-2].c, yyvsp[-1].c );
		yyval.ttype = NULL;
	}
    break;
case 76:
#line 769 "parse.yy"
{ syn_warning ("Task definition");
		  tmp_tree = build_task (check_task (yyvsp[-1].ttype));
		  make_block_decl (yyvsp[0].ttype, current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  BLOCK_DOWN (current_module) = chainon (current_scope, BLOCK_DOWN (current_module));
		  BLOCK_UP (current_scope) = current_module;
		  push_scope ();
		  in_tf = 1;
		}
    break;
case 77:
#line 779 "parse.yy"
{ BLOCK_BODY (current_scope) = yyvsp[-1].ttype;
		  in_tf = 0;
		  BLOCK_PORTS (current_scope) = nreverse (BLOCK_PORTS (current_scope));
		  BLOCK_DECL (current_scope) = nreverse (BLOCK_DECL (current_scope));
		  current_scope = pop_scope ();
		}
    break;
case 78:
#line 789 "parse.yy"
{ current_scope = build_function (check_function (yyvsp[-1].ttype));
		  make_block_decl (yyvsp[-1].ttype, current_module, current_scope);
		  push_scope ();  /* funct name becomes var inside of funct */
		  FUNCT_DECL (current_scope) = make_decl (yyvsp[-1].ttype, yyvsp[-2].ttype, NULL_TREE, NULL_TREE);
		  BLOCK_DOWN (current_module) = chainon (current_scope, BLOCK_DOWN (current_module));
		  BLOCK_UP (current_scope) = current_module;
		  in_tf = in_function = 1;
		}
    break;
case 79:
#line 798 "parse.yy"
{ BLOCK_BODY (current_scope) = yyvsp[-1].ttype;
		  in_tf = in_function = 0;
		  BLOCK_PORTS (current_scope) = nreverse (BLOCK_PORTS (current_scope));
		  BLOCK_DECL (current_scope) = nreverse (BLOCK_DECL (current_scope));
		  current_scope = pop_scope ();
		}
    break;
case 80:
#line 808 "parse.yy"
{ yyval.ttype = make_reg_spec (NULL_TREE); }
    break;
case 81:
#line 810 "parse.yy"
{ yyval.ttype = make_reg_spec (yyvsp[0].ttype); }
    break;
case 82:
#line 812 "parse.yy"
{ yyval.ttype = make_integer_spec (NULL_TREE); }
    break;
case 83:
#line 814 "parse.yy"
{ yyval.ttype = make_real_spec (NULL_TREE); }
    break;
case 84:
#line 819 "parse.yy"
{}
    break;
case 89:
#line 835 "parse.yy"
{ current_spec = make_param_spec (yyvsp[0].ttype); }
    break;
case 90:
#line 837 "parse.yy"
{ BLOCK_DECL (current_scope) =
			chainon (yyvsp[-1].ttype, BLOCK_DECL (current_scope));
		 }
    break;
case 92:
#line 845 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype);
		}
    break;
case 94:
#line 850 "parse.yy"
{ yyerrok; }
    break;
case 96:
#line 856 "parse.yy"
{ yyval.ttype = make_param_decl (check_non_reg (yyvsp[-2].ttype), current_spec, yyvsp[0].ttype);
		}
    break;
case 97:
#line 859 "parse.yy"
{ yyval.ttype = make_param_decl (check_non_reg (yyvsp[-2].ttype), current_spec, yyvsp[0].ttype);
		}
    break;
case 98:
#line 865 "parse.yy"
{}
    break;
case 99:
#line 870 "parse.yy"
{ BLOCK_DECL (current_scope) =
			chainon (yyvsp[-1].ttype, BLOCK_DECL (current_scope)); }
    break;
case 100:
#line 873 "parse.yy"
{ BLOCK_DECL (current_scope) =
			chainon (yyvsp[-1].ttype, BLOCK_DECL (current_scope)); }
    break;
case 101:
#line 876 "parse.yy"
{ BLOCK_DECL (current_scope) =
			chainon (yyvsp[-1].ttype, BLOCK_DECL (current_scope)); }
    break;
case 102:
#line 883 "parse.yy"
{ BLOCK_PORTS (current_scope) =
			chainon (yyvsp[-1].ttype, BLOCK_PORTS (current_scope)); }
    break;
case 103:
#line 891 "parse.yy"
{ if (in_tf)
		    yyval.ttype = current_spec = make_reg_spec (yyvsp[0].ttype);
		  else
		    yyval.ttype = current_spec = make_net_spec (default_net_type, yyvsp[0].ttype, NULL_TREE);
		  PORT_INPUT_ATTR (yyval.ttype) = 1;
		}
    break;
case 104:
#line 898 "parse.yy"
{ function_error;
		  if (in_tf)
		    yyval.ttype = current_spec = make_reg_spec (yyvsp[0].ttype);
		  else
		    yyval.ttype = current_spec = make_net_spec (default_net_type, yyvsp[0].ttype, NULL_TREE);
		  PORT_OUTPUT_ATTR (yyval.ttype) = 1;
		}
    break;
case 105:
#line 906 "parse.yy"
{ function_error;
		  if (in_tf)
		    yyval.ttype = current_spec = make_reg_spec (yyvsp[0].ttype);
		  else
		    yyval.ttype = current_spec = make_net_spec (default_net_type, yyvsp[0].ttype, NULL_TREE);
		  PORT_INPUT_ATTR (yyval.ttype) = 1;
		  PORT_OUTPUT_ATTR (yyval.ttype) = 1;
		}
    break;
case 106:
#line 918 "parse.yy"
{ yyval.ttype = make_decl (check_port (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE); }
    break;
case 107:
#line 920 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (make_decl (check_port (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE), yyvsp[-2].ttype);
		}
    break;
case 110:
#line 926 "parse.yy"
{ yyerrok; }
    break;
case 112:
#line 932 "parse.yy"
{ current_spec = make_reg_spec (yyvsp[0].ttype); }
    break;
case 113:
#line 934 "parse.yy"
{ if (!yyvsp[0].ttype)
		    syn_warning ("Integer Range");
		  current_spec = make_integer_spec (yyvsp[0].ttype);
		}
    break;
case 114:
#line 939 "parse.yy"
{ syn_warning ("TIME");
		  current_spec = make_time_spec (yyvsp[0].ttype);
		}
    break;
case 115:
#line 946 "parse.yy"
{ current_spec = make_real_spec (NULL_TREE); }
    break;
case 116:
#line 951 "parse.yy"
{ syn_warning ("EVENT");
		  current_spec = make_event_spec (); }
    break;
case 117:
#line 957 "parse.yy"
{ lval_type = LVAL_NEW_NET; }
    break;
case 118:
#line 962 "parse.yy"
{ BLOCK_DECL (current_scope) =
			chainon (yyvsp[-1].ttype, BLOCK_DECL (current_scope));
 			lval_type = LVAL_REG;
		  current_delay = NULL_TREE;
		}
    break;
case 120:
#line 969 "parse.yy"
{ lval_type = LVAL_REG; 
		  MODULE_ASSIGNMENTS (current_module) =
			chainon (yyvsp[-1].ttype, MODULE_ASSIGNMENTS (current_module));
		  current_delay = NULL_TREE;
		}
    break;
case 121:
#line 978 "parse.yy"
{ yyval.ttype = current_spec = make_net_spec (yyvsp[-2].ntype, yyvsp[-1].ttype, yyvsp[0].ttype);
		  current_delay = yyvsp[0].ttype;
		}
    break;
case 122:
#line 985 "parse.yy"
{ syn_warning ("TRIREG net");
		  sorry ("TRIREG net type is not supported in this version");
		}
    break;
case 123:
#line 989 "parse.yy"
{ yyval.ttype = error_mark_node; }
    break;
case 125:
#line 995 "parse.yy"
{ syn_warning ("SUPPLY0 net"); }
    break;
case 126:
#line 997 "parse.yy"
{ syn_warning ("SUPPLY1 net"); }
    break;
case 128:
#line 1010 "parse.yy"
{ sorry ("SCALARED keyword not supported; all nets treated as vectored");
		  NET_SCALARED_ATTR (yyvsp[0].ttype) = 1;
		  yyval.ttype = yyvsp[0].ttype;
		}
    break;
case 129:
#line 1015 "parse.yy"
{ sorry ("VECTORED keyword is not supported; all nets treated as vectored");
		  NET_VECTORED_ATTR (yyvsp[0].ttype) = 1;
		  yyval.ttype = yyvsp[0].ttype;
		}
    break;
case 130:
#line 1020 "parse.yy"
{ yyval.ttype = NULL_TREE; }
    break;
case 131:
#line 1025 "parse.yy"
{ lval_type = LVAL_NET; }
    break;
case 132:
#line 1027 "parse.yy"
{ lval_type = LVAL_REG; 
		  MODULE_ASSIGNMENTS (current_module) =
			chainon (yyvsp[-1].ttype, MODULE_ASSIGNMENTS (current_module));
		}
    break;
case 133:
#line 1035 "parse.yy"
{ current_delay = yyvsp[0].ttype;   /* strength not supported */
		  yyval.ttype = NULL;
		}
    break;
case 134:
#line 1042 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 135:
#line 1044 "parse.yy"
{ yyerrok;
		  yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype);
		}
    break;
case 138:
#line 1050 "parse.yy"
{ yyerrok; }
    break;
case 140:
#line 1055 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 141:
#line 1057 "parse.yy"
{ yyval.ttype = build_cont_assign (
				check_lval (yyvsp[-3].ttype, lval_type, current_spec),
				yyvsp[0].ttype, yyvsp[-1].ltype, current_delay, current_delay!=0);
		}
    break;
case 142:
#line 1066 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 143:
#line 1068 "parse.yy"
{ yyerrok;
		  yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype);
		}
    break;
case 146:
#line 1074 "parse.yy"
{ yyerrok; }
    break;
case 148:
#line 1079 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 149:
#line 1081 "parse.yy"
{ yyval.ttype = build_cont_assign (yyvsp[-3].ttype, yyvsp[0].ttype, yyvsp[-1].ltype, current_delay,
			 current_delay!=0); }
    break;
case 150:
#line 1086 "parse.yy"
{ syn_warning ("DEFPARAM"); }
    break;
case 151:
#line 1091 "parse.yy"
{ MODULE_DEFPARAMS (current_module) = chainon (
			build_stmt (ASSIGN_STMT, stmt_lineno, yyvsp[-2].ttype, yyvsp[0].ttype),
			MODULE_DEFPARAMS (current_module));
		}
    break;
case 152:
#line 1096 "parse.yy"
{
		  MODULE_DEFPARAMS (current_module) = chainon (
			build_stmt (ASSIGN_STMT, stmt_lineno, yyvsp[-2].ttype, yyvsp[0].ttype),
			MODULE_DEFPARAMS (current_module));
		}
    break;
case 154:
#line 1106 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype);
		}
    break;
case 157:
#line 1112 "parse.yy"
{ yyerrok; }
    break;
case 159:
#line 1118 "parse.yy"
{ yyval.ttype = make_decl (check_reg (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE); }
    break;
case 160:
#line 1120 "parse.yy"
{ yyval.ttype = make_decl (check_reg (yyvsp[-5].ttype), current_spec, yyvsp[-3].ttype, yyvsp[-1].ttype);
		  syn_warning ("REG/INTEGER arrays");
		}
    break;
case 161:
#line 1127 "parse.yy"
{ yyval.ttype = make_decl (check_non_reg (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE); }
    break;
case 162:
#line 1129 "parse.yy"
{ yyval.ttype = chainon (make_decl (check_non_reg (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE), yyvsp[-2].ttype); }
    break;
case 163:
#line 1134 "parse.yy"
{ yyval.ttype = make_decl (check_net (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE); }
    break;
case 164:
#line 1136 "parse.yy"
{ yyval.ttype = chainon (make_decl (check_net (yyvsp[0].ttype), current_spec, NULL_TREE, NULL_TREE), yyvsp[-2].ttype); }
    break;
case 165:
#line 1141 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 166:
#line 1143 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 167:
#line 1145 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 168:
#line 1147 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 169:
#line 1152 "parse.yy"
{}
    break;
case 170:
#line 1154 "parse.yy"
{
warning ("Strength specification is not supported and is ignored", NULL_CHAR, NULL_CHAR);
		}
    break;
case 171:
#line 1162 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 172:
#line 1164 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 177:
#line 1188 "parse.yy"
{ yyval.ttype = NULL_TREE;}
    break;
case 179:
#line 1194 "parse.yy"
{ TREE_OPERAND (range_handle, 0) = yyvsp[-3].ttype;
		  TREE_OPERAND (range_handle, 1) = yyvsp[-1].ttype;
		  yyval.ttype = range_handle;
		}
    break;
case 180:
#line 1210 "parse.yy"
{ current_gatetype = yyvsp[0].gtype;
		  in_instantiation = 1;
		}
    break;
case 181:
#line 1214 "parse.yy"
{ in_instantiation = 0; }
    break;
case 183:
#line 1223 "parse.yy"
{ yyval.gtype = GATE_OR_TYPE; }
    break;
case 184:
#line 1225 "parse.yy"
{ syn_warning ("Buffer gate"); }
    break;
case 185:
#line 1227 "parse.yy"
{ syn_warning ("Bufif gate"); }
    break;
case 186:
#line 1229 "parse.yy"
{ syn_warning ("CMOS gate"); }
    break;
case 187:
#line 1231 "parse.yy"
{ syn_warning ("PULL gate"); }
    break;
case 188:
#line 1233 "parse.yy"
{ syn_warning ("TRAN gate"); }
    break;
case 189:
#line 1246 "parse.yy"
{ current_delay = NULL_TREE; }
    break;
case 191:
#line 1252 "parse.yy"
{ current_delay = yyvsp[0].ttype; }
    break;
case 195:
#line 1263 "parse.yy"
{ yyval.ttype = build_gate_instance (current_gatetype, input_filename, lineno,
                        yyvsp[-3].ttype, nreverse (yyvsp[-1].ttype), current_delay, NULL_TREE);
                  MODULE_INSTANCES (current_module) = chainon (yyval.ttype,
                        MODULE_INSTANCES (current_module));
                }
    break;
case 196:
#line 1269 "parse.yy"
{ yyval.ttype = build_gate_instance (current_gatetype, input_filename, lineno,
                        NULL_TREE, nreverse (yyvsp[-1].ttype), current_delay, NULL_TREE);
                  MODULE_INSTANCES (current_module) = chainon (yyval.ttype,
                        MODULE_INSTANCES (current_module));
                }
    break;
case 197:
#line 1280 "parse.yy"
{ current_instance_module = yyvsp[-1].ttype;
		  current_instance_param = yyvsp[0].ttype;
		  in_instantiation = 1;
		}
    break;
case 198:
#line 1285 "parse.yy"
{ current_instance_module = NULL_TREE;
		  current_instance_param = NULL_TREE;
		  in_instantiation = 0;
		}
    break;
case 199:
#line 1316 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 200:
#line 1318 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 201:
#line 1320 "parse.yy"
{ yyval.ttype = nreverse (yyvsp[-1].ttype); }
    break;
case 202:
#line 1322 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[-1].ttype, NULL_TREE); }
    break;
case 203:
#line 1324 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[-1].ttype, NULL_TREE);
		  yyval.ttype = tree_cons (yyvsp[-3].ttype, NULL_TREE, yyval.ttype);
		}
    break;
case 204:
#line 1328 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[-1].ttype, NULL_TREE);
		  yyval.ttype = tree_cons (yyvsp[-3].ttype, NULL_TREE, yyval.ttype);
		  yyval.ttype = tree_cons (yyvsp[-5].ttype, NULL_TREE, yyval.ttype);
		}
    break;
case 205:
#line 1336 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 206:
#line 1338 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); }
    break;
case 207:
#line 1365 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 208:
#line 1367 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); yyerrok; }
    break;
case 211:
#line 1372 "parse.yy"
{ yyerrok; }
    break;
case 214:
#line 1385 "parse.yy"
{ yyerrok; }
    break;
case 217:
#line 1389 "parse.yy"
{ yyerrok; }
    break;
case 219:
#line 1395 "parse.yy"
{ yyval.ttype = build_stmt (INSTANCE_NODE, lineno, check_non_reg (yyvsp[-3].ttype),
				current_instance_module,
				nreverse (yyvsp[-1].ttype), current_instance_param);
		  MODULE_INSTANCES (current_module) = chainon (yyval.ttype,
				MODULE_INSTANCES (current_module));
		//  make_block_decl ($1, current_module, NULL_TREE);
		}
    break;
case 220:
#line 1403 "parse.yy"
{ yyval.ttype = build_stmt (INSTANCE_NODE, lineno, NULL_TREE,
				current_instance_module,
				nreverse (yyvsp[-1].ttype), current_instance_param);
		  MODULE_INSTANCES (current_module) = chainon (yyval.ttype,
				MODULE_INSTANCES (current_module));
		}
    break;
case 224:
#line 1419 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype);
		}
    break;
case 227:
#line 1428 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 228:
#line 1430 "parse.yy"
{ yyval.ttype = build_tree_list (NULL_TREE, NULL_TREE); }
    break;
case 230:
#line 1436 "parse.yy"
{ yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype); }
    break;
case 231:
#line 1441 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[-1].ttype, yyvsp[-3].ttype); }
    break;
case 232:
#line 1443 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 233:
#line 1451 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("INITIAL"); }
    break;
case 234:
#line 1454 "parse.yy"
{ yyval.ttype = build_stmt (INITIAL_BLOCK, yyvsp[-1].ltype, yyvsp[0].ttype); }
    break;
case 235:
#line 1458 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 236:
#line 1460 "parse.yy"
{ yyval.ttype = build_stmt (ALWAYS_BLOCK, yyvsp[-1].ltype, yyvsp[0].ttype); }
    break;
case 238:
#line 1466 "parse.yy"
{ yyval.ttype = build_stmt (NULL_STMT, lineno); }
    break;
case 240:
#line 1473 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 241:
#line 1475 "parse.yy"
{ yyval.ttype = build_stmt (IF_STMT, yyvsp[-5].ltype, yyvsp[-3].ttype, yyvsp[-1].ttype, yyvsp[0].ttype); }
    break;
case 242:
#line 1477 "parse.yy"
{ yyval.ttype = current_case; }
    break;
case 243:
#line 1479 "parse.yy"
{ current_case = yyvsp[0].ttype; }
    break;
case 244:
#line 1481 "parse.yy"
{ STMT_CASE_EXPR (yyvsp[-6].ttype) = yyvsp[-3].ttype;
		  STMT_CASE_LIST (yyvsp[-6].ttype) = nreverse (yyvsp[-1].ttype);
		  current_case = yyvsp[-7].ttype;
		  yyval.ttype = yyvsp[-6].ttype;
		}
    break;
case 245:
#line 1488 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("FOREVER"); }
    break;
case 246:
#line 1491 "parse.yy"
{ yyval.ttype = build_stmt (FOREVER_STMT, yyvsp[-1].ltype, yyvsp[0].ttype); }
    break;
case 247:
#line 1494 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("REPEAT"); }
    break;
case 248:
#line 1497 "parse.yy"
{ yyval.ttype = build_stmt (REPEAT_INIT_STMT, yyvsp[-4].ltype, yyvsp[0].ttype, yyvsp[-2].ttype);
		  tmp_tree = build_stmt (REPEAT_STMT, yyvsp[-4].ltype, yyval.ttype, yyvsp[-2].ttype);
		  chainon (yyvsp[0].ttype, tmp_tree);
		}
    break;
case 249:
#line 1503 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("WHILE"); }
    break;
case 250:
#line 1506 "parse.yy"
{ yyval.ttype = build_stmt (WHILE_STMT, yyvsp[-4].ltype, yyvsp[0].ttype, yyvsp[-2].ttype); }
    break;
case 251:
#line 1508 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 252:
#line 1510 "parse.yy"
{ stmt_lineno = yyvsp[-8].ltype;
		  tmp_tree = build_stmt (FOR_STMT, yyvsp[-8].ltype, yyvsp[0].ttype, yyvsp[-2].ttype, yyvsp[-4].ttype);
		  STMT_SURROGATE_ATTR (tmp_tree) = 1;
		  yyval.ttype = build_stmt (FOR_STMT, yyvsp[-8].ltype, yyvsp[0].ttype, yyvsp[-6].ttype, yyvsp[-4].ttype);
		  TREE_CHAIN (tmp_tree) = yyval.ttype;	/* Fix this in pass 3 */
		  chainon (yyvsp[0].ttype, tmp_tree);	/* at end of statement sequence */
		}
    break;
case 253:
#line 1519 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("delay control");
		  function_error;
		}
    break;
case 254:
#line 1524 "parse.yy"
{ yyval.ttype = build_stmt (DELAY_STMT, yyvsp[-1].ltype, yyvsp[0].ttype, yyvsp[-2].ttype); }
    break;
case 255:
#line 1527 "parse.yy"
{ stmt_lineno = lineno;
		  function_error;
		}
    break;
case 256:
#line 1531 "parse.yy"
{ STMT_BODY (yyvsp[-2].ttype) = yyvsp[0].ttype;
		  yyval.ttype = yyvsp[-2].ttype;
		}
    break;
case 257:
#line 1536 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  function_error;
		}
    break;
case 258:
#line 1540 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_DELAY_STMT, yyvsp[-3].ltype, yyvsp[-5].ttype,
			implicit_conversion (yyvsp[-5].ttype, yyvsp[-1].ttype),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, yyvsp[-3].ltype, yyvsp[-5].ttype, yyvsp[-2].ttype));
		}
    break;
case 259:
#line 1547 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  function_error;
		}
    break;
case 260:
#line 1551 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_EVENT_STMT, yyvsp[-3].ltype, yyvsp[-5].ttype,
			implicit_conversion (yyvsp[-5].ttype, yyvsp[-1].ttype),
			NULL_TREE, NULL_TREE, yyvsp[-2].ttype);
		}
    break;
case 261:
#line 1557 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  function_error;
		}
    break;
case 262:
#line 1561 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_NONBLK_DELAY_STMT, yyvsp[-2].ltype, yyvsp[-4].ttype,
			implicit_conversion (yyvsp[-4].ttype, yyvsp[-1].ttype),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, yyvsp[-2].ltype, yyvsp[-4].ttype, integer_zero_node));
		}
    break;
case 263:
#line 1568 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  function_error;
		}
    break;
case 264:
#line 1572 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_NONBLK_DELAY_STMT, yyvsp[-3].ltype, yyvsp[-5].ttype,
			implicit_conversion (yyvsp[-5].ttype, yyvsp[-1].ttype),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, yyvsp[-3].ltype, yyvsp[-5].ttype, yyvsp[-2].ttype));
		}
    break;
case 265:
#line 1579 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  function_error;
		}
    break;
case 266:
#line 1583 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_NONBLK_EVENT_STMT, yyvsp[-3].ltype, yyvsp[-5].ttype,
			implicit_conversion (yyvsp[-5].ttype, yyvsp[-1].ttype),
			NULL_TREE, NULL_TREE, yyvsp[-2].ttype);
		}
    break;
case 267:
#line 1595 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("WAIT");
		  function_error;
		}
    break;
case 268:
#line 1600 "parse.yy"
{ yyval.ttype = build_stmt (WAIT_STMT, yyvsp[-4].ltype, yyvsp[0].ttype, yyvsp[-2].ttype); }
    break;
case 269:
#line 1603 "parse.yy"
{ yyval.ltype = stmt_lineno;
		  syn_warning ("Event trigger");
		  function_error;
		}
    break;
case 270:
#line 1608 "parse.yy"
{ yyval.ttype= build_stmt (ARROW_STMT, yyvsp[-2].ltype, check_named_event (yyvsp[-1].ttype)); }
    break;
case 275:
#line 1614 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 276:
#line 1616 "parse.yy"
{ yyval.ttype = build_stmt (DISABLE_STMT, yyvsp[-2].ltype, yyvsp[-1].ttype);
		  HIERARCHICAL_ATTR (yyval.ttype) = HIERARCHICAL_ATTR (yyvsp[-1].ttype);
		}
    break;
case 277:
#line 1621 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 278:
#line 1623 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_PROC_STMT, yyvsp[-4].ltype, yyvsp[-3].ttype, yyvsp[-1].ttype, NULL_TREE,
			NULL_TREE); }
    break;
case 279:
#line 1627 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 280:
#line 1629 "parse.yy"
{ yyval.ttype = build_stmt (DEASSIGN_STMT, yyvsp[-2].ltype, yyvsp[-1].ttype); }
    break;
case 281:
#line 1631 "parse.yy"
{ yyval.ltype = stmt_lineno;
				  lval_type = LVAL_REG_NET; }
    break;
case 282:
#line 1634 "parse.yy"
{ syn_warning ("FORCE");
		  yyval.ttype = build_stmt (FORCE_STMT, yyvsp[-4].ltype, yyvsp[-3].ttype, yyvsp[-1].ttype, NULL_TREE,
			NULL_TREE, NULL_TREE, NULL_TREE);
		  lval_type = LVAL_REG;
		}
    break;
case 283:
#line 1640 "parse.yy"
{ yyval.ltype = stmt_lineno;
				  lval_type = LVAL_REG_NET; }
    break;
case 284:
#line 1643 "parse.yy"
{ syn_warning ("RELEASE");
		  yyval.ttype = build_stmt (RELEASE_STMT, yyvsp[-2].ltype, yyvsp[-1].ttype);
		  lval_type = LVAL_REG;
		}
    break;
case 285:
#line 1653 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 286:
#line 1655 "parse.yy"
{ yyval.ttype = yyvsp[0].ttype; }
    break;
case 287:
#line 1660 "parse.yy"
{ yyval.ttype = build_stmt (CASE_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
    break;
case 288:
#line 1662 "parse.yy"
{ yyval.ttype = build_stmt (CASEZ_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
    break;
case 289:
#line 1664 "parse.yy"
{ yyval.ttype = build_stmt (CASEX_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
    break;
case 290:
#line 1675 "parse.yy"
{ yyval.ltype = stmt_lineno; }
    break;
case 291:
#line 1677 "parse.yy"
{ yyval.ttype = build_stmt (ASSIGN_STMT, yyvsp[-1].ltype, yyvsp[-3].ttype,
				implicit_conversion (yyvsp[-3].ttype, yyvsp[0].ttype)); }
    break;
case 293:
#line 1684 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-1].ttype);
		}
    break;
case 296:
#line 1693 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 297:
#line 1696 "parse.yy"
{ if (STMT_CASE_DEFAULT (current_case))
		    error ("More than one 'DEFAULT' within CASE", NULL_CHAR, NULL_CHAR);
		  STMT_CASE_DEFAULT (current_case) = yyvsp[0].ttype;
		}
    break;
case 298:
#line 1701 "parse.yy"
{ if (STMT_CASE_DEFAULT (current_case))
		    error ("More than one 'DEFAULT' within CASE", NULL_CHAR, NULL_CHAR);
		  STMT_CASE_DEFAULT (current_case) = yyvsp[0].ttype;
		}
    break;
case 299:
#line 1709 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 300:
#line 1711 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); }
    break;
case 303:
#line 1718 "parse.yy"
{ if( yyvsp[0].ttype != NULL_TREE ) {
		    yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); 
		  } else {
		    yyval.ttype = yyvsp[0].ttype;
		  }
		}
    break;
case 304:
#line 1725 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); }
    break;
case 305:
#line 1734 "parse.yy"
{ yyval.ttype = NULL_TREE; }
    break;
case 307:
#line 1740 "parse.yy"
{ yyval.ttype = build_stmt (BEGIN_STMT, stmt_lineno); }
    break;
case 308:
#line 1742 "parse.yy"
{ STMT_BODY (yyvsp[-2].ttype) = nreverse
			(chainon (build_stmt (END_STMT, stmt_lineno), yyvsp[-1].ttype));
		  yyval.ttype = yyvsp[-2].ttype;
		}
    break;
case 309:
#line 1747 "parse.yy"
{ tmp_tree = make_node (NAMED_BLOCK);
		  BLOCK_DOWN (current_scope) = chainon (tmp_tree, BLOCK_DOWN (current_scope));
		  BLOCK_UP (tmp_tree) = current_scope;
		  BLOCK_NAME (tmp_tree) = yyvsp[0].ttype;
		  BLOCK_BODY (tmp_tree) = NULL_TREE;
                  BLOCK_CONTEXT_LIST (tmp_tree) = NULL;
		  make_block_decl (check_block (yyvsp[0].ttype), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  push_scope ();
		  yyval.ttype = build_stmt (BEGIN_NAMED_STMT, stmt_lineno,
			NULL_TREE, tmp_tree);
		}
    break;
case 310:
#line 1760 "parse.yy"
{ tmp_tree = build_stmt (END_NAMED_STMT, stmt_lineno,
			NULL_TREE, STMT_BLOCK (yyvsp[-3].ttype));
		  /* STMT_BEGIN_NAMED_END ($<ttype>4) = tmp_tree; */
		  STMT_BODY (yyvsp[-3].ttype) = nreverse (chainon (tmp_tree, yyvsp[-1].ttype));
		  current_scope = pop_scope ();
		  yyval.ttype = yyvsp[-3].ttype;
		}
    break;
case 311:
#line 1771 "parse.yy"
{ yyval.ttype = build_stmt (FORK_STMT, stmt_lineno); }
    break;
case 312:
#line 1773 "parse.yy"
{ STMT_BODY (yyvsp[-2].ttype) = chainon  /* thread JOIN to BEGINNING */
			(build_stmt (JOIN_STMT, stmt_lineno, NULL_TREE),
			nreverse (yyvsp[-1].ttype));
		  yyval.ttype = yyvsp[-2].ttype;
		}
    break;
case 313:
#line 1779 "parse.yy"
{ tmp_tree = make_node (NAMED_BLOCK);
		  BLOCK_DOWN (current_scope) = chainon (tmp_tree, BLOCK_DOWN (current_scope));
		  BLOCK_UP (tmp_tree) = current_scope;
		  BLOCK_NAME (tmp_tree) = yyvsp[0].ttype;
		  BLOCK_BODY (tmp_tree) = NULL_TREE;
		  make_block_decl (check_block (yyvsp[0].ttype), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  push_scope ();
		  yyval.ttype = build_stmt (FORK_NAMED_STMT, stmt_lineno,
			NULL_TREE, tmp_tree);
		}
    break;
case 314:
#line 1791 "parse.yy"
{ tmp_tree = build_stmt (JOIN_NAMED_STMT, stmt_lineno,
			NULL_TREE, STMT_BLOCK (yyvsp[-3].ttype));
		  /* STMT_BEGIN_NAMED_END ($<ttype>4) = tmp_tree; */
		  STMT_BODY (yyvsp[-3].ttype) = chainon (tmp_tree, nreverse (yyvsp[-1].ttype));
		  current_scope = pop_scope ();
		  yyval.ttype = yyvsp[-3].ttype;
		}
    break;
case 315:
#line 1802 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 316:
#line 1804 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-1].ttype);
		}
    break;
case 318:
#line 1812 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 319:
#line 1814 "parse.yy"
{ yyerrok;
		  yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		}
    break;
case 321:
#line 1822 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 325:
#line 1833 "parse.yy"
{ syn_warning ("task enable");
		  function_error;
		  yyval.ttype = build_stmt (TASK_STMT, lineno, IDENTIFIER_POINTER (yyvsp[-1].ttype), NULL_TREE);
		  HIERARCHICAL_ATTR (yyval.ttype) = HIERARCHICAL_ATTR (yyvsp[-1].ttype);
		}
    break;
case 326:
#line 1839 "parse.yy"
{ syn_warning ("task enable");
		  yyval.ttype = build_stmt (TASK_STMT, lineno, IDENTIFIER_POINTER (yyvsp[-4].ttype), nreverse (yyvsp[-2].ttype));
		  HIERARCHICAL_ATTR (yyval.ttype) = HIERARCHICAL_ATTR (yyvsp[-4].ttype);
		}
    break;
case 327:
#line 1847 "parse.yy"
{ yyval.ttype = build_stmt (SYSTASK_STMT, lineno, IDENTIFIER_POINTER (yyvsp[-1].ttype), NULL_TREE); }
    break;
case 328:
#line 1849 "parse.yy"
{ in_systask = 1; }
    break;
case 329:
#line 1851 "parse.yy"
{ yyval.ttype = build_stmt (SYSTASK_STMT, lineno, IDENTIFIER_POINTER (yyvsp[-5].ttype), nreverse (yyvsp[-2].ttype));
		  in_systask = 0;
		}
    break;
case 330:
#line 1858 "parse.yy"
{ stmt_lineno = lineno; }
    break;
case 331:
#line 1860 "parse.yy"
{ stmt_lineno = lineno; }
    break;
case 332:
#line 1865 "parse.yy"
{ yyval.ttype = check_lval (yyvsp[0].ttype, lval_type, current_spec); }
    break;
case 333:
#line 1867 "parse.yy"
{ yyval.ttype = build_bit_ref (
			check_lval_nocheck (yyvsp[-3].ttype, lval_type, current_spec), yyvsp[-1].ttype); }
    break;
case 334:
#line 1870 "parse.yy"
{ yyval.ttype = build_part_ref (
			check_lval_nocheck (yyvsp[-5].ttype, lval_type, current_spec), yyvsp[-3].ttype, yyvsp[-1].ttype); }
    break;
case 335:
#line 1880 "parse.yy"
{
	}
    break;
case 336:
#line 1883 "parse.yy"
{}
    break;
case 337:
#line 1889 "parse.yy"
{}
    break;
case 338:
#line 1894 "parse.yy"
{ yyval.ttype = NULL; }
    break;
case 339:
#line 1896 "parse.yy"
{ yyerrok;
		    yyval.ttype = NULL;	
		//  $$ = chainon ($2, $1);
		}
    break;
case 343:
#line 1906 "parse.yy"
{
	    if( yyvsp[0].ttype!=NULL_TREE ) { 
		STMT_SOURCE_LINE(yyvsp[0].ttype) = stmt_lineno;
		STMT_SOURCE_FILE(yyvsp[0].ttype) = input_filename;
		MODULE_SPECDEFS(current_module) = 
			chainon(MODULE_SPECDEFS(current_module),yyvsp[0].ttype);
	    }
	}
    break;
case 344:
#line 1924 "parse.yy"
{ current_spec = make_param_spec (NULL_TREE); }
    break;
case 345:
#line 1926 "parse.yy"
{
		specify_save_param_list( current_module, yyvsp[-1].ttype );
	 }
    break;
case 346:
#line 1933 "parse.yy"
{
		PATH_CONDITION(yyvsp[-3].ttype) = yyvsp[-4].ttype;
	 	PATH_DELAYS(yyvsp[-3].ttype) = yyvsp[-1].ttype;	
		MODULE_SPECDEFS(current_module) = 
			chainon(MODULE_SPECDEFS(current_module),check_path(yyvsp[-3].ttype));
	}
    break;
case 347:
#line 1947 "parse.yy"
{
		yyval.ttype = build_path( nreverse(yyvsp[-7].ttype), nreverse(yyvsp[-4].ttype), yyvsp[-2].ttype, NULL_TREE, NULL_TREE,
			yyvsp[-6].integer, 0, 0 );
	}
    break;
case 348:
#line 1955 "parse.yy"
{
		yyval.ttype = build_path( nreverse(yyvsp[-7].ttype), nreverse(yyvsp[-4].ttype), yyvsp[-2].ttype, NULL_TREE, NULL_TREE,
			yyvsp[-6].integer, yyvsp[-8].integer, 0 );
	}
    break;
case 349:
#line 1963 "parse.yy"
{
		yyval.ttype = build_path( nreverse(yyvsp[-8].ttype), nreverse(yyvsp[-5].ttype), yyvsp[-2].ttype, NULL_TREE, NULL_TREE,
			yyvsp[-7].integer, 0, yyvsp[-4].integer );
	}
    break;
case 350:
#line 1971 "parse.yy"
{
		yyval.ttype = build_path( nreverse(yyvsp[-8].ttype), nreverse(yyvsp[-5].ttype), yyvsp[-2].ttype, NULL_TREE, NULL_TREE,
			yyvsp[-7].integer, yyvsp[-9].integer, yyvsp[-4].integer );
	}
    break;
case 351:
#line 1977 "parse.yy"
{
		yyval.ttype = build_path( nreverse(yyvsp[-3].ttype), nreverse(yyvsp[-1].ttype), NULL_TREE, NULL_TREE, NULL_TREE,
			yyvsp[-2].integer, 0, 0 );
	}
    break;
case 352:
#line 1983 "parse.yy"
{
		yyval.ttype = build_path( nreverse(yyvsp[-4].ttype), nreverse(yyvsp[-1].ttype), NULL_TREE, NULL_TREE, NULL_TREE,
			yyvsp[-2].integer, 0, yyvsp[-3].integer );
	}
    break;
case 354:
#line 1991 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype);
		}
    break;
case 356:
#line 1996 "parse.yy"
{ yyerrok; }
    break;
case 359:
#line 2002 "parse.yy"
{ yyerrok;
		  yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-2].ttype);
		}
    break;
case 361:
#line 2007 "parse.yy"
{ yyerrok; }
    break;
case 363:
#line 2012 "parse.yy"
{
		yyval.ttype = build_tree_list( check_input_port(yyvsp[0].ttype), NULL_TREE );
	}
    break;
case 364:
#line 2016 "parse.yy"
{
		yyval.ttype = build_tree_list( build_bit_ref (check_input_port(yyvsp[-3].ttype), yyvsp[-1].ttype), NULL_TREE ); 
	}
    break;
case 365:
#line 2020 "parse.yy"
{
		yyval.ttype = build_tree_list( build_part_ref (check_input_port(yyvsp[-5].ttype), yyvsp[-3].ttype, yyvsp[-1].ttype), NULL_TREE ); 
	}
    break;
case 366:
#line 2026 "parse.yy"
{
		yyval.ttype = build_tree_list( check_output_port(yyvsp[0].ttype), NULL_TREE );
	}
    break;
case 367:
#line 2030 "parse.yy"
{
		yyval.ttype = build_tree_list( build_bit_ref (check_output_port(yyvsp[-3].ttype), yyvsp[-1].ttype), NULL_TREE ); 
	}
    break;
case 368:
#line 2034 "parse.yy"
{
		yyval.ttype = build_tree_list( build_part_ref (check_output_port(yyvsp[-5].ttype), yyvsp[-3].ttype, yyvsp[-1].ttype), NULL_TREE ); 
	}
    break;
case 370:
#line 2041 "parse.yy"
{
		yyval.ttype = chainon( yyvsp[-3].ttype, yyvsp[-1].ttype );
	}
    break;
case 371:
#line 2046 "parse.yy"
{
		tree t;
		t = chainon( yyvsp[-5].ttype, yyvsp[-3].ttype );
		yyval.ttype = chainon( t, yyvsp[-1].ttype );
	}
    break;
case 372:
#line 2054 "parse.yy"
{
		tree t;
		t = chainon( yyvsp[-11].ttype, yyvsp[-9].ttype );
		t = chainon( t, yyvsp[-7].ttype );
		t = chainon( t, yyvsp[-5].ttype );
		t = chainon( t, yyvsp[-3].ttype );
		yyval.ttype = chainon( t, yyvsp[-1].ttype );
	}
    break;
case 373:
#line 2065 "parse.yy"
{
		yyval.ttype = build_tree_list(yyvsp[0].ttype,NULL_TREE);
	}
    break;
case 374:
#line 2071 "parse.yy"
{	
		yyval.integer = 1;
	}
    break;
case 375:
#line 2075 "parse.yy"
{	
		yyval.integer = -1;
	}
    break;
case 376:
#line 2081 "parse.yy"
{
		yyval.integer = FALSE;
	}
    break;
case 377:
#line 2085 "parse.yy"
{
		yyval.integer = TRUE;
	}
    break;
case 380:
#line 2099 "parse.yy"
{
		yyval.integer = 1;
	}
    break;
case 381:
#line 2103 "parse.yy"
{
		yyval.integer = -1;
	}
    break;
case 382:
#line 2109 "parse.yy"
{
		yyval.ttype = 0;
	}
    break;
case 383:
#line 2113 "parse.yy"
{
		yyval.ttype = yyvsp[-1].ttype;
	}
    break;
case 384:
#line 2121 "parse.yy"
{
		yyval.ttype = build_timing_statement( SETUP, yyvsp[-7].ttype, yyvsp[-5].ttype, yyvsp[-3].ttype, NULL_TREE, yyvsp[-2].ttype );
	}
    break;
case 385:
#line 2126 "parse.yy"
{
		yyval.ttype = build_timing_statement( HOLD, yyvsp[-7].ttype, yyvsp[-5].ttype, yyvsp[-3].ttype, NULL_TREE, yyvsp[-2].ttype );
	}
    break;
case 386:
#line 2131 "parse.yy"
{
		yyval.ttype = build_timing_statement( PERIOD, yyvsp[-5].ttype, NULL_TREE, yyvsp[-3].ttype, NULL_TREE, yyvsp[-2].ttype );
	}
    break;
case 387:
#line 2136 "parse.yy"
{
		yyval.ttype = build_timing_statement( WIDTH, yyvsp[-4].ttype, NULL_TREE, yyvsp[-2].ttype, NULL_TREE, NULL_TREE );
	}
    break;
case 388:
#line 2141 "parse.yy"
{
		yyval.ttype = build_timing_statement( WIDTH, yyvsp[-7].ttype, NULL_TREE, yyvsp[-5].ttype, yyvsp[-3].ttype, yyvsp[-2].ttype );
	}
    break;
case 389:
#line 2146 "parse.yy"
{
		yyval.ttype = build_timing_statement( SKEW, yyvsp[-7].ttype, yyvsp[-5].ttype, yyvsp[-3].ttype, NULL_TREE, yyvsp[-2].ttype );
	}
    break;
case 390:
#line 2151 "parse.yy"
{
		yyval.ttype = build_timing_statement( RECOVERY, yyvsp[-7].ttype, yyvsp[-5].ttype, yyvsp[-3].ttype, NULL_TREE, yyvsp[-2].ttype );
	}
    break;
case 391:
#line 2156 "parse.yy"
{
		yyval.ttype = build_timing_statement( SETUPHOLD, yyvsp[-9].ttype, yyvsp[-7].ttype, yyvsp[-5].ttype, yyvsp[-3].ttype, yyvsp[-2].ttype );
	}
    break;
case 392:
#line 2161 "parse.yy"
{
		yyval.ttype = NULL_TREE;
	}
    break;
case 393:
#line 2167 "parse.yy"
{
		yyval.ttype = 0;
	}
    break;
case 394:
#line 2171 "parse.yy"
{
		yyval.ttype = yyvsp[0].ttype;
	}
    break;
case 396:
#line 2180 "parse.yy"
{ 
		yyval.ttype = build_timing_event( yyvsp[-1].integer, yyvsp[0].ttype, NULL_TREE );
	}
    break;
case 397:
#line 2185 "parse.yy"
{ 
		yyval.ttype = build_timing_event( yyvsp[-3].integer, yyvsp[-2].ttype, yyvsp[0].ttype );
	}
    break;
case 398:
#line 2191 "parse.yy"
{
		yyval.ttype = build_timing_event( yyvsp[-1].integer, yyvsp[0].ttype, NULL_TREE );
	}
    break;
case 399:
#line 2196 "parse.yy"
{
		yyval.ttype = build_timing_event( yyvsp[-3].integer, yyvsp[-2].ttype, yyvsp[0].ttype );
	}
    break;
case 400:
#line 2202 "parse.yy"
{
		extern unsigned int alledgeMask;
		yyval.integer = alledgeMask;
	}
    break;
case 401:
#line 2207 "parse.yy"
{
		extern unsigned int posedgeMask;
		yyval.integer = posedgeMask;
	}
    break;
case 402:
#line 2212 "parse.yy"
{
		extern unsigned int negedgeMask;
		yyval.integer = negedgeMask;
	}
    break;
case 403:
#line 2217 "parse.yy"
{  	extern int enable_edge_digit;
		enable_edge_digit = TRUE; 
	}
    break;
case 404:
#line 2221 "parse.yy"
{ 
		extern int enable_edge_digit;
		enable_edge_digit = FALSE;
		yyval.integer = yyvsp[-1].integer;
	}
    break;
case 405:
#line 2229 "parse.yy"
{
		extern unsigned int posedgeMask;
		yyval.integer = posedgeMask;
	}
    break;
case 406:
#line 2234 "parse.yy"
{
		extern unsigned int negedgeMask;
		yyval.integer = negedgeMask;
	}
    break;
case 407:
#line 2239 "parse.yy"
{  	extern int enable_edge_digit;
		enable_edge_digit = TRUE; 
	}
    break;
case 408:
#line 2243 "parse.yy"
{ 
		extern int enable_edge_digit;
		enable_edge_digit = FALSE;
		yyval.integer = yyvsp[-1].integer;
	}
    break;
case 409:
#line 2251 "parse.yy"
{
		extern unsigned int edge_mask[4][4];
		yyval.integer = edge_mask[yyvsp[0].double_digit[0]][yyvsp[0].double_digit[1]];
	}
    break;
case 410:
#line 2256 "parse.yy"
{
		extern unsigned int edge_mask[4][4];
		yyval.integer = yyvsp[0].integer | edge_mask[yyvsp[-2].double_digit[0]][yyvsp[-2].double_digit[1]];
	}
    break;
case 411:
#line 2261 "parse.yy"
{
		yyval.integer = 0;
	}
    break;
case 413:
#line 2268 "parse.yy"
{ yyval.ttype = yyvsp[-1].ttype; }
    break;
case 414:
#line 2270 "parse.yy"
{ yyval.ttype = build_unary_op (BIT_NOT_EXPR, yyvsp[0].ttype); }
    break;
case 415:
#line 2272 "parse.yy"
{ yyval.ttype = build_binary_op (EQ_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 416:
#line 2274 "parse.yy"
{ yyval.ttype = build_binary_op (EQ_CASE_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 417:
#line 2276 "parse.yy"
{ yyval.ttype = build_binary_op (NE_CASE_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 418:
#line 2278 "parse.yy"
{ yyval.ttype = build_binary_op (NE_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 419:
#line 2282 "parse.yy"
{ yyval.ttype = check_rval (yyvsp[0].ttype); }
    break;
case 420:
#line 2286 "parse.yy"
{
		yyval.ttype = check_one_bit_constant(yyvsp[0].ttype);
	}
    break;
case 421:
#line 2290 "parse.yy"
{
		yyval.ttype = check_one_bit_constant(yyvsp[0].ttype);
	}
    break;
case 422:
#line 2297 "parse.yy"
{ 
		check_input_port(yyvsp[0].ttype);
		yyval.ttype = check_rval (yyvsp[0].ttype);
	 }
    break;
case 423:
#line 2302 "parse.yy"
{ 
		check_input_port(yyvsp[-3].ttype);
		yyval.ttype = build_bit_ref ( check_rval_nocheck (yyvsp[-3].ttype), yyvsp[-1].ttype);
	 }
    break;
case 424:
#line 2307 "parse.yy"
{ 
		check_input_port(yyvsp[-5].ttype);
		yyval.ttype = build_part_ref ( check_rval_nocheck (yyvsp[-5].ttype), yyvsp[-3].ttype, yyvsp[-1].ttype);
	}
    break;
case 425:
#line 2320 "parse.yy"
{ yyval.ttype = make_node (CONCAT_REF);
		  CONCAT_LIST (yyval.ttype) = yyvsp[-1].ttype;
		  concat_labels (yyval.ttype);
		}
    break;
case 426:
#line 2328 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 427:
#line 2330 "parse.yy"
{ yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); }
    break;
case 428:
#line 2332 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); }
    break;
case 429:
#line 2334 "parse.yy"
{ yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-2].ttype); }
    break;
case 436:
#line 2348 "parse.yy"
{ if (!TREE_CONSTANT_ATTR (yyvsp[0].ttype)) {
		    error ("Illegal non-constant expression", NULL_CHAR, NULL_CHAR);
		    yyval.ttype = error_mark_node;
		  }
		  else
		    yyval.ttype = yyvsp[0].ttype;
		}
    break;
case 438:
#line 2360 "parse.yy"
{ switch (delay_type) {
		    case (MIN_DELAY):  yyval.ttype = yyvsp[-4].ttype; break;
		    case (TYP_DELAY):  yyval.ttype = yyvsp[-2].ttype; break;
		    case (MAX_DELAY):  yyval.ttype = yyvsp[0].ttype; break;
		    }
		}
    break;
case 439:
#line 2370 "parse.yy"
{ switch (delay_type) {
		    case (MIN_DELAY):  yyval.ttype = yyvsp[-4].ttype; break;
		    case (TYP_DELAY):  yyval.ttype = yyvsp[-2].ttype; break;
		    case (MAX_DELAY):  yyval.ttype = yyvsp[0].ttype; break;
		    }
		}
    break;
case 441:
#line 2381 "parse.yy"
{ yyval.ttype = yyvsp[0].ttype; }
    break;
case 442:
#line 2383 "parse.yy"
{ yyval.ttype = build_unary_op (NEGATE_EXPR, yyvsp[0].ttype); }
    break;
case 443:
#line 2385 "parse.yy"
{ yyval.ttype = build_unary_op (TRUTH_NOT_EXPR, yyvsp[0].ttype); }
    break;
case 444:
#line 2387 "parse.yy"
{ yyval.ttype = build_unary_op (BIT_NOT_EXPR, yyvsp[0].ttype); }
    break;
case 445:
#line 2389 "parse.yy"
{ yyval.ttype = build_unary_op (AND_REDUCE_EXPR, yyvsp[0].ttype); }
    break;
case 446:
#line 2391 "parse.yy"
{ yyval.ttype = build_unary_op (NAND_REDUCE_EXPR, yyvsp[0].ttype); }
    break;
case 447:
#line 2393 "parse.yy"
{ yyval.ttype = build_unary_op (OR_REDUCE_EXPR, yyvsp[0].ttype); }
    break;
case 448:
#line 2395 "parse.yy"
{ yyval.ttype = build_unary_op (NOR_REDUCE_EXPR, yyvsp[0].ttype); }
    break;
case 449:
#line 2397 "parse.yy"
{ yyval.ttype = build_unary_op (XOR_REDUCE_EXPR, yyvsp[0].ttype); }
    break;
case 450:
#line 2399 "parse.yy"
{ yyval.ttype = build_unary_op (XNOR_REDUCE_EXPR, yyvsp[0].ttype); }
    break;
case 451:
#line 2401 "parse.yy"
{ yyval.ttype = build_binary_op (PLUS_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 452:
#line 2403 "parse.yy"
{ yyval.ttype = build_binary_op (MINUS_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 453:
#line 2405 "parse.yy"
{ yyval.ttype = build_binary_op (MULT_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 454:
#line 2407 "parse.yy"
{ syn_warning ("Division operator"); }
    break;
case 455:
#line 2409 "parse.yy"
{ yyval.ttype = build_binary_op (DIV_EXPR, yyvsp[-3].ttype, yyvsp[0].ttype); }
    break;
case 456:
#line 2411 "parse.yy"
{ syn_warning ("Modulus operator"); }
    break;
case 457:
#line 2413 "parse.yy"
{ yyval.ttype = build_binary_op (MOD_EXPR, yyvsp[-3].ttype, yyvsp[0].ttype); }
    break;
case 458:
#line 2415 "parse.yy"
{ yyval.ttype = build_binary_op (EQ_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 459:
#line 2417 "parse.yy"
{ yyval.ttype = build_binary_op (NE_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 460:
#line 2419 "parse.yy"
{ syn_warning ("Case equality"); }
    break;
case 461:
#line 2421 "parse.yy"
{ yyval.ttype = build_binary_op (EQ_CASE_EXPR, yyvsp[-3].ttype, yyvsp[0].ttype); }
    break;
case 462:
#line 2423 "parse.yy"
{ syn_warning ("Case inequality"); }
    break;
case 463:
#line 2425 "parse.yy"
{ yyval.ttype = build_binary_op (NE_CASE_EXPR, yyvsp[-3].ttype, yyvsp[0].ttype); }
    break;
case 464:
#line 2427 "parse.yy"
{ yyval.ttype = build_binary_op (TRUTH_ANDIF_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 465:
#line 2429 "parse.yy"
{ yyval.ttype = build_binary_op (TRUTH_ORIF_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 466:
#line 2431 "parse.yy"
{ yyval.ttype = build_binary_op (LT_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 467:
#line 2433 "parse.yy"
{ yyval.ttype = build_binary_op (LE_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 468:
#line 2435 "parse.yy"
{ yyval.ttype = build_binary_op (GT_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 469:
#line 2437 "parse.yy"
{ yyval.ttype = build_binary_op (GE_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 470:
#line 2439 "parse.yy"
{ yyval.ttype = build_binary_op (BIT_AND_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 471:
#line 2441 "parse.yy"
{ yyval.ttype = build_binary_op (BIT_OR_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 472:
#line 2443 "parse.yy"
{ yyval.ttype = build_binary_op (BIT_XOR_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 473:
#line 2445 "parse.yy"
{ yyval.ttype = build_binary_op (BIT_XNOR_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 474:
#line 2447 "parse.yy"
{ yyval.ttype = build_binary_op (LSHIFT_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 475:
#line 2449 "parse.yy"
{ yyval.ttype = build_binary_op (RSHIFT_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 476:
#line 2451 "parse.yy"
{ yyval.ttype = build_cond_expr (yyvsp[-4].ttype, yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 480:
#line 2475 "parse.yy"
{ yyval.ttype = check_rval (yyvsp[0].ttype); }
    break;
case 481:
#line 2477 "parse.yy"
{ yyval.ttype = build_bit_ref (check_rval_nocheck (yyvsp[-3].ttype), yyvsp[-1].ttype); }
    break;
case 482:
#line 2479 "parse.yy"
{ yyval.ttype = build_part_ref (check_rval_nocheck (yyvsp[-5].ttype), yyvsp[-3].ttype, yyvsp[-1].ttype); }
    break;
case 485:
#line 2483 "parse.yy"
{ yyval.ttype = yyvsp[-1].ttype; }
    break;
case 486:
#line 2485 "parse.yy"
{ yyval.ttype = error_mark_node; }
    break;
case 487:
#line 2518 "parse.yy"
{ yyval.ttype = make_node (CONCAT_REF);
		  CONCAT_LIST (yyval.ttype) = yyvsp[-1].ttype;
		  concat_labels (yyval.ttype);
		}
    break;
case 488:
#line 2523 "parse.yy"
{ yyval.ttype = make_node (CONCAT_REP_REF);
		  CONCAT_LIST (yyval.ttype) = yyvsp[-2].ttype;
		  CONCAT_EXPR (yyval.ttype) = yyvsp[-4].ttype;
		  concat_labels (yyval.ttype);
		}
    break;
case 489:
#line 2532 "parse.yy"
{ yyval.ttype = build_function_call (yyvsp[-3].ttype, nreverse (yyvsp[-1].ttype));
		  HIERARCHICAL_ATTR (yyval.ttype) = HIERARCHICAL_ATTR (yyvsp[-3].ttype);
                  UNKNOWN_TYPE_ATTR (yyval.ttype) = 1;
		}
    break;
case 490:
#line 2538 "parse.yy"
{ yyval.ttype = build_nt (SYSFUNCTION_REF, IDENTIFIER_POINTER (yyvsp[-3].ttype),
			nreverse (yyvsp[-1].ttype), NULL_TREE, NULL_TREE, lineno, input_filename); }
    break;
case 491:
#line 2542 "parse.yy"
{ yyval.ttype = build_nt (SYSFUNCTION_REF, IDENTIFIER_POINTER (yyvsp[0].ttype),
			NULL_TREE, NULL_TREE, NULL_TREE, lineno, input_filename); }
    break;
case 492:
#line 2552 "parse.yy"
{
		  syn_warning ("Hierachical name");
		/*  hierarchical_list = link_tree_cons ($1, NULL_TREE, hierarchical_list); */
		  HIERARCHICAL_ATTR (yyvsp[0].ttype) = 1;
                  UNKNOWN_TYPE_ATTR (yyvsp[0].ttype) = 1;
		  TREE_CONSTANT_ATTR (yyvsp[0].ttype) = 1;  /* assume this is true for now */
		}
    break;
case 495:
#line 2566 "parse.yy"
{ yyval.ttype = get_identifier ("or"); }
    break;
case 496:
#line 2585 "parse.yy"
{ yyval.ttype = NULL_TREE; }
    break;
case 498:
#line 2591 "parse.yy"
{ yyval.ttype = build_nt (DELAY_EXPR, yyvsp[0].ttype, yyvsp[0].ttype, NULL_TREE);
 		  TREE_LABEL (yyval.ttype) = 1;
		}
    break;
case 499:
#line 2595 "parse.yy"
{ yyval.ttype = check_rval (yyvsp[0].ttype);
		  yyval.ttype = build_nt (DELAY_EXPR, yyval.ttype, yyval.ttype, NULL_TREE);
		  TREE_LABEL (yyval.ttype) = 1;
		}
    break;
case 500:
#line 2600 "parse.yy"
{ yyval.ttype = yyvsp[-1].ttype; }
    break;
case 501:
#line 2609 "parse.yy"
{ yyval.ttype = build_nt (DELAY_EXPR, yyvsp[0].ttype, yyvsp[0].ttype, NULL_TREE);
		  TREE_LABEL (yyval.ttype) = 1;
		}
    break;
case 502:
#line 2613 "parse.yy"
{ yyval.ttype = build_nt (DELAY_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE);
		  TREE_LABEL (yyval.ttype) = 2;
		}
    break;
case 503:
#line 2617 "parse.yy"
{ yyval.ttype = build_nt (DELAY_EXPR, yyvsp[-4].ttype, yyvsp[-2].ttype, yyvsp[0].ttype);
		  TREE_LABEL (yyval.ttype) = 3;
		}
    break;
case 504:
#line 2624 "parse.yy"
{ yyval.ttype = yyvsp[0].ttype; }
    break;
case 505:
#line 2626 "parse.yy"
{ yyval.ttype = check_rval (yyvsp[0].ttype); }
    break;
case 506:
#line 2628 "parse.yy"
{ yyval.ttype = yyvsp[-1].ttype; }
    break;
case 507:
#line 2632 "parse.yy"
{ in_event = 1; }
    break;
case 508:
#line 2634 "parse.yy"
{if (!HIERARCHICAL_ATTR (yyvsp[0].ttype) &&
			TREE_CODE (IDENT_CURRENT_DECL (yyvsp[0].ttype)) == EVENT_DECL)
		    yyval.ttype = build_stmt (EVENT_STMT, lineno, NULL_TREE,
			build_tree_list (IDENT_CURRENT_DECL (yyvsp[0].ttype), NULL_TREE));
		  else
		    yyval.ttype = build_stmt (EVENT_STMT, lineno, NULL_TREE,
			build_tree_list (build_unary_op
			(ANYEDGE_EXPR, check_rval (yyvsp[0].ttype)), NULL_TREE));
		  in_event = 0;
		}
    break;
case 509:
#line 2644 "parse.yy"
{ in_event = 1; }
    break;
case 510:
#line 2646 "parse.yy"
{ yyval.ttype = build_stmt (EVENT_STMT, lineno, NULL_TREE, yyvsp[-1].ttype);
		  in_event = 0; }
    break;
case 511:
#line 2652 "parse.yy"
{ /*if (TREE_CODE ($1) == EVENT_DECL)
		    $$ = build_tree_list ($1, NULL_TREE);
		  else */
		    yyval.ttype = build_tree_list
			(build_unary_op (ANYEDGE_EXPR, yyvsp[0].ttype), NULL_TREE);
		}
    break;
case 512:
#line 2659 "parse.yy"
{ yyval.ttype = build_tree_list 
			(build_unary_op (POSEDGE_EXPR, yyvsp[0].ttype), NULL_TREE); }
    break;
case 513:
#line 2662 "parse.yy"
{ yyval.ttype = build_tree_list 
			(build_unary_op (NEGEDGE_EXPR, yyvsp[0].ttype), NULL_TREE); }
    break;
case 514:
#line 2665 "parse.yy"
{ yyval.ttype = chainon (yyvsp[-2].ttype, yyvsp[0].ttype); }
    break;
case 515:
#line 2674 "parse.yy"
{ yyerrok; }
    break;
case 516:
#line 2679 "parse.yy"
{ yyerrok; }
    break;
case 517:
#line 2681 "parse.yy"
{ yyerrok; }
    break;
case 518:
#line 2686 "parse.yy"
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
#line 2689 "parse.yy"

void
eat_block (short token)
{
  YYSTYPE lval;

  for (;;)
    if (yylex (&lval) == token)
      return;
}

