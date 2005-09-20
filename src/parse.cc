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




/* Copy the first part of user declarations.  */
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
/* Line 190 of yacc.c.  */
#line 425 "parse.cc"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 437 "parse.cc"

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
#define YYFINAL  74
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2571

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  212
/* YYNRULES -- Number of rules. */
#define YYNRULES  519
/* YYNRULES -- Number of states. */
#define YYNSTATES  998

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   368

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     8,     9,    10,    13,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    17,    19,
      21,    22,    23,    24,    34,    35,    38,    41,    45,    46,
      48,    52,    54,    58,    60,    66,    68,    72,    73,    75,
      79,    81,    86,    93,    95,    97,    99,   101,   103,   105,
     107,   109,   111,   113,   115,   117,   119,   120,   121,   122,
     134,   135,   138,   141,   143,   145,   149,   150,   156,   157,
     162,   164,   167,   170,   171,   174,   176,   178,   180,   182,
     184,   186,   191,   193,   195,   197,   199,   201,   202,   210,
     211,   220,   221,   223,   225,   227,   228,   231,   233,   235,
     237,   238,   244,   246,   250,   252,   256,   260,   264,   268,
     269,   274,   279,   284,   289,   292,   295,   298,   300,   304,
     306,   309,   313,   317,   320,   323,   326,   328,   330,   331,
     336,   341,   346,   350,   351,   357,   359,   361,   363,   365,
     368,   371,   372,   373,   379,   383,   385,   389,   391,   394,
     398,   402,   403,   408,   410,   414,   416,   419,   423,   427,
     428,   433,   437,   441,   447,   449,   453,   455,   458,   462,
     466,   468,   475,   477,   481,   483,   487,   488,   492,   496,
     500,   501,   503,   509,   515,   517,   519,   521,   523,   524,
     526,   532,   533,   537,   539,   541,   543,   545,   547,   549,
     551,   552,   556,   557,   562,   564,   568,   573,   577,   578,
     584,   585,   588,   593,   598,   605,   614,   616,   620,   622,
     626,   628,   631,   635,   639,   641,   645,   647,   650,   654,
     658,   663,   667,   669,   671,   673,   677,   679,   683,   685,
     686,   688,   692,   698,   703,   704,   708,   709,   713,   715,
     717,   720,   721,   729,   730,   731,   740,   741,   745,   746,
     753,   754,   761,   762,   773,   774,   778,   779,   783,   784,
     791,   792,   799,   800,   806,   807,   814,   815,   822,   823,
     830,   831,   836,   838,   840,   842,   844,   845,   850,   851,
     858,   859,   864,   865,   872,   873,   878,   879,   882,   884,
     886,   888,   889,   894,   896,   899,   901,   904,   908,   912,
     915,   917,   921,   923,   927,   929,   933,   934,   936,   937,
     942,   943,   951,   952,   957,   958,   966,   967,   970,   973,
     974,   977,   980,   981,   984,   986,   988,   991,   997,  1000,
    1001,  1008,  1010,  1012,  1014,  1019,  1026,  1027,  1031,  1034,
    1035,  1038,  1041,  1043,  1045,  1047,  1048,  1053,  1059,  1069,
    1080,  1091,  1103,  1109,  1116,  1118,  1122,  1124,  1128,  1132,
    1134,  1138,  1140,  1144,  1148,  1150,  1155,  1162,  1164,  1169,
    1176,  1178,  1184,  1192,  1206,  1208,  1210,  1212,  1214,  1216,
    1218,  1220,  1222,  1224,  1225,  1230,  1241,  1252,  1261,  1269,
    1280,  1291,  1302,  1315,  1327,  1328,  1331,  1333,  1336,  1341,
    1344,  1349,  1350,  1352,  1354,  1355,  1361,  1363,  1365,  1366,
    1372,  1374,  1378,  1380,  1382,  1386,  1389,  1393,  1397,  1401,
    1405,  1407,  1409,  1411,  1413,  1418,  1425,  1429,  1431,  1433,
    1437,  1441,  1443,  1446,  1450,  1454,  1458,  1460,  1462,  1464,
    1470,  1476,  1478,  1481,  1484,  1487,  1490,  1493,  1496,  1499,
    1502,  1505,  1508,  1512,  1516,  1520,  1521,  1526,  1527,  1532,
    1536,  1540,  1541,  1546,  1547,  1552,  1556,  1560,  1564,  1568,
    1572,  1576,  1580,  1584,  1588,  1592,  1596,  1600,  1606,  1608,
    1610,  1612,  1614,  1619,  1626,  1628,  1630,  1634,  1638,  1642,
    1649,  1654,  1659,  1661,  1663,  1665,  1667,  1669,  1670,  1672,
    1675,  1678,  1683,  1685,  1689,  1695,  1698,  1701,  1706,  1707,
    1711,  1712,  1718,  1720,  1723,  1726,  1730,  1732,  1734,  1736
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     139,     0,    -1,   242,    -1,   140,   127,    -1,     1,    -1,
     127,    -1,   141,    -1,   140,   141,    -1,   142,    -1,   154,
      -1,    -1,    -1,    -1,    50,   143,   338,   144,   147,   349,
     145,   146,    52,    -1,    -1,   146,   153,    -1,   146,     1,
      -1,    30,   148,   347,    -1,    -1,   149,    -1,   148,   128,
     149,    -1,     1,    -1,   148,   128,     1,    -1,   150,    -1,
     129,    37,    30,   150,   347,    -1,   152,    -1,   130,   151,
     131,    -1,    -1,   152,    -1,   151,   128,   152,    -1,    37,
      -1,    37,    31,   326,   132,    -1,    37,    31,   326,     7,
     326,   132,    -1,   176,    -1,   182,    -1,   189,    -1,   181,
      -1,   217,    -1,   225,    -1,   204,    -1,   195,    -1,   288,
      -1,   237,    -1,   239,    -1,   169,    -1,   171,    -1,    -1,
      -1,    -1,    55,   155,    37,   156,   147,   349,   157,   158,
     161,   162,    56,    -1,    -1,   158,   159,    -1,   158,     1,
      -1,   182,    -1,   160,    -1,    71,    37,   349,    -1,    -1,
      88,    37,   133,   321,   349,    -1,    -1,    57,   163,   164,
      58,    -1,   165,    -1,   164,   165,    -1,   166,   349,    -1,
      -1,   166,   167,    -1,   121,    -1,   126,    -1,   123,    -1,
     122,    -1,   125,    -1,   124,    -1,    30,   168,   168,   347,
      -1,   124,    -1,   123,    -1,   122,    -1,   125,    -1,   126,
      -1,    -1,    59,    37,   349,   170,   174,   241,    60,    -1,
      -1,    61,   173,    37,   349,   172,   174,   241,    62,    -1,
      -1,   216,    -1,    73,    -1,    74,    -1,    -1,   174,   175,
      -1,   176,    -1,   182,    -1,   181,    -1,    -1,    63,   215,
     177,   178,   349,    -1,   179,    -1,   178,   128,   179,    -1,
       1,    -1,   178,     1,   179,    -1,   178,   128,     1,    -1,
      37,   133,   326,    -1,    37,   133,   328,    -1,    -1,   185,
     180,   206,   349,    -1,   187,   180,   208,   349,    -1,   186,
     180,   209,   349,    -1,   183,   180,   184,   349,    -1,    64,
     215,    -1,    65,   215,    -1,    66,   215,    -1,    37,    -1,
     184,   128,    37,    -1,     1,    -1,   184,     1,    -1,   184,
       1,    37,    -1,   184,   128,     1,    -1,    71,   215,    -1,
      73,   215,    -1,    72,   215,    -1,    74,    -1,    75,    -1,
      -1,   190,   188,   209,   349,    -1,   191,   180,   209,   349,
      -1,   190,   188,   198,   349,    -1,   193,   194,   339,    -1,
      -1,    68,   192,   210,   194,   339,    -1,    67,    -1,    80,
      -1,    81,    -1,   216,    -1,    69,   216,    -1,    70,   216,
      -1,    -1,    -1,   197,   180,   196,   201,   349,    -1,    76,
     211,   339,    -1,   199,    -1,   198,   128,   199,    -1,     1,
      -1,   198,     1,    -1,   198,     1,   199,    -1,   198,   128,
       1,    -1,    -1,    37,   133,   200,   329,    -1,   202,    -1,
     201,   128,   202,    -1,     1,    -1,   201,     1,    -1,   201,
       1,   202,    -1,   201,   128,     1,    -1,    -1,   286,   133,
     203,   329,    -1,    77,   205,   349,    -1,   337,   133,   326,
      -1,   205,   128,   337,   133,   326,    -1,   207,    -1,   206,
     128,   207,    -1,     1,    -1,   206,     1,    -1,   206,     1,
     207,    -1,   206,   128,     1,    -1,    37,    -1,    37,    31,
     326,     7,   326,   132,    -1,    37,    -1,   208,   128,    37,
      -1,    37,    -1,   209,   128,    37,    -1,    -1,    30,   116,
     347,    -1,    30,   117,   347,    -1,    30,   118,   347,    -1,
      -1,   212,    -1,    30,   213,   128,   214,   347,    -1,    30,
     214,   128,   213,   347,    -1,    78,    -1,    80,    -1,    79,
      -1,    81,    -1,    -1,   216,    -1,    31,   326,     7,   326,
     348,    -1,    -1,   219,   218,   220,    -1,    82,    -1,   115,
      -1,    83,    -1,    84,    -1,    85,    -1,    86,    -1,    87,
      -1,    -1,   221,   223,   349,    -1,    -1,   340,   222,   223,
     349,    -1,   224,    -1,   223,   128,   224,    -1,    37,    30,
     229,   347,    -1,    30,   229,   347,    -1,    -1,    37,   227,
     226,   230,   349,    -1,    -1,   134,   326,    -1,   134,    30,
     228,   135,    -1,   134,    30,   327,   135,    -1,   134,    30,
     327,   128,   327,   135,    -1,   134,    30,   327,   128,   327,
     128,   327,   135,    -1,   326,    -1,   228,   128,   326,    -1,
     329,    -1,   229,   128,   329,    -1,     1,    -1,   229,     1,
      -1,   229,     1,   329,    -1,   229,   128,     1,    -1,   231,
      -1,   230,   128,   231,    -1,     1,    -1,   230,     1,    -1,
     230,     1,   231,    -1,   230,   128,     1,    -1,    37,    30,
     232,   347,    -1,    30,   232,   347,    -1,   233,    -1,   235,
      -1,   234,    -1,   233,   128,   234,    -1,     1,    -1,   233,
     128,     1,    -1,   329,    -1,    -1,   236,    -1,   235,   128,
     236,    -1,   129,    37,    30,   329,   347,    -1,   129,    37,
      30,   347,    -1,    -1,    88,   238,   242,    -1,    -1,    89,
     240,   242,    -1,   242,    -1,   349,    -1,   266,   349,    -1,
      -1,     4,   243,    30,   329,   347,   241,   264,    -1,    -1,
      -1,   244,   265,   245,    30,   329,   347,   268,    93,    -1,
      -1,    94,   246,   242,    -1,    -1,    95,   247,    30,   329,
     347,   242,    -1,    -1,    96,   248,    30,   329,   347,   242,
      -1,    -1,    97,   249,    30,   266,   349,   329,   349,   266,
     347,   242,    -1,    -1,   342,   250,   241,    -1,    -1,   343,
     251,   241,    -1,    -1,   286,   133,   252,   342,   329,   349,
      -1,    -1,   286,   133,   253,   343,   329,   349,    -1,    -1,
     286,    21,   254,   329,   349,    -1,    -1,   286,    21,   255,
     342,   329,   349,    -1,    -1,   286,    21,   256,   343,   329,
     349,    -1,    -1,    98,   257,    30,   325,   347,   241,    -1,
      -1,    99,   258,   337,   349,    -1,   273,    -1,   276,    -1,
     283,    -1,   284,    -1,    -1,   100,   259,   337,   349,    -1,
      -1,    76,   260,   286,   133,   329,   349,    -1,    -1,   101,
     261,   286,   349,    -1,    -1,   102,   262,   286,   133,   329,
     349,    -1,    -1,   103,   263,   286,   349,    -1,    -1,     5,
     241,    -1,    90,    -1,    91,    -1,    92,    -1,    -1,   286,
     133,   267,   329,    -1,   269,    -1,   268,   269,    -1,     1,
      -1,   268,     1,    -1,   270,     7,   241,    -1,   104,     7,
     241,    -1,   104,   241,    -1,   329,    -1,   270,   128,   329,
      -1,     1,    -1,   270,   128,     1,    -1,   272,    -1,   271,
     128,   272,    -1,    -1,   329,    -1,    -1,   105,   274,   279,
     106,    -1,    -1,   105,     7,    37,   275,   281,   279,   106,
      -1,    -1,   107,   277,   280,   108,    -1,    -1,   107,     7,
      37,   278,   281,   280,   108,    -1,    -1,   279,   242,    -1,
     279,     1,    -1,    -1,   280,   242,    -1,   280,     1,    -1,
      -1,   281,   282,    -1,   176,    -1,   181,    -1,   337,   349,
      -1,   337,    30,   270,   347,   349,    -1,   109,   349,    -1,
      -1,   109,   285,    30,   271,   347,   349,    -1,   287,    -1,
     323,    -1,   337,    -1,   337,    31,   329,   132,    -1,   337,
      31,   329,     7,   329,   132,    -1,    -1,    53,   289,   290,
      -1,   291,    54,    -1,    -1,   291,   292,    -1,   291,     1,
      -1,   293,    -1,   295,    -1,   309,    -1,    -1,    41,   294,
     178,   349,    -1,   308,   296,   133,   301,   349,    -1,    30,
     297,   304,    30,   298,     7,   306,   347,   347,    -1,    30,
     307,   297,   304,    30,   298,     7,   306,   347,   347,    -1,
      30,   297,   304,    30,   298,   303,     7,   306,   347,   347,
      -1,    30,   307,   297,   304,    30,   298,   303,     7,   306,
     347,   347,    -1,    30,   297,   304,   298,   347,    -1,    30,
     297,   303,   304,   298,   347,    -1,   299,    -1,   297,   128,
     299,    -1,     1,    -1,   297,     1,   299,    -1,   297,   128,
       1,    -1,   300,    -1,   298,   128,   300,    -1,     1,    -1,
     298,     1,   300,    -1,   298,   128,     1,    -1,    37,    -1,
      37,    31,   326,   348,    -1,    37,    31,   326,     7,   326,
     348,    -1,    37,    -1,    37,    31,   326,   348,    -1,    37,
      31,   326,     7,   326,   348,    -1,   302,    -1,    30,   302,
     128,   302,   347,    -1,    30,   302,   128,   302,   128,   302,
     347,    -1,    30,   302,   128,   302,   128,   302,   128,   302,
     128,   302,   128,   302,   347,    -1,   327,    -1,    25,    -1,
      26,    -1,    39,    -1,    40,    -1,   329,    -1,   329,    -1,
     113,    -1,   114,    -1,    -1,     4,    30,   305,   347,    -1,
      42,    30,   312,   128,   312,   128,   329,   310,   347,   349,
      -1,    43,    30,   312,   128,   312,   128,   329,   310,   347,
     349,    -1,    44,    30,   313,   128,   329,   310,   347,   349,
      -1,    45,    30,   313,   128,   329,   347,   349,    -1,    45,
      30,   313,   128,   329,   128,   329,   310,   347,   349,    -1,
      46,    30,   312,   128,   312,   128,   329,   310,   347,   349,
      -1,    47,    30,   313,   128,   312,   128,   329,   310,   347,
     349,    -1,    48,    30,   312,   128,   312,   128,   329,   128,
     329,   310,   347,   349,    -1,    49,    30,   313,   128,   312,
     128,   329,   128,   329,   347,   349,    -1,    -1,   128,   311,
      -1,    37,    -1,   314,   322,    -1,   314,   322,    10,   319,
      -1,   316,   322,    -1,   316,   322,    10,   319,    -1,    -1,
     113,    -1,   114,    -1,    -1,   119,    31,   315,   318,   348,
      -1,   113,    -1,   114,    -1,    -1,   119,    31,   317,   318,
     348,    -1,   120,    -1,   120,   128,   318,    -1,     1,    -1,
     320,    -1,    30,   319,   135,    -1,    33,   320,    -1,   320,
      18,   321,    -1,   320,    16,   321,    -1,   320,    15,   321,
      -1,   320,    17,   321,    -1,    37,    -1,   112,    -1,   111,
      -1,   337,    -1,   337,    31,   329,   132,    -1,   337,    31,
     326,     7,   326,   132,    -1,   130,   324,   131,    -1,   287,
      -1,   323,    -1,   324,   128,   287,    -1,   324,   128,   323,
      -1,     1,    -1,   324,     1,    -1,   324,     1,   287,    -1,
     324,     1,   323,    -1,   324,   128,     1,    -1,   329,    -1,
     329,    -1,   329,    -1,   329,     7,   329,     7,   329,    -1,
     329,     7,   329,     7,   329,    -1,   334,    -1,    25,   334,
      -1,    26,   334,    -1,    32,   334,    -1,    33,   334,    -1,
      14,   334,    -1,    35,   334,    -1,    11,   334,    -1,    34,
     334,    -1,    12,   334,    -1,    13,   334,    -1,   329,    25,
     329,    -1,   329,    26,   329,    -1,   329,    27,   329,    -1,
      -1,   329,    28,   330,   329,    -1,    -1,   329,    29,   331,
     329,    -1,   329,    18,   329,    -1,   329,    17,   329,    -1,
      -1,   329,    16,   332,   329,    -1,    -1,   329,    15,   333,
     329,    -1,   329,     9,   329,    -1,   329,     8,   329,    -1,
     329,    22,   329,    -1,   329,    21,   329,    -1,   329,    20,
     329,    -1,   329,    19,   329,    -1,   329,    14,   329,    -1,
     329,    11,   329,    -1,   329,    12,   329,    -1,   329,    13,
     329,    -1,   329,    24,   329,    -1,   329,    23,   329,    -1,
     329,     6,   329,     7,   329,    -1,   110,    -1,   111,    -1,
     112,    -1,   337,    -1,   337,    31,   329,   132,    -1,   337,
      31,   326,     7,   326,   132,    -1,   335,    -1,   336,    -1,
      30,   327,   347,    -1,    30,     1,   347,    -1,   130,   270,
     131,    -1,   130,   326,   130,   270,   131,   131,    -1,   337,
      30,   271,   347,    -1,   109,    30,   271,   347,    -1,   109,
      -1,    38,    -1,    37,    -1,    37,    -1,   115,    -1,    -1,
     340,    -1,   134,   111,    -1,   134,   337,    -1,   134,    30,
     341,   347,    -1,   327,    -1,   327,   128,   327,    -1,   327,
     128,   327,   128,   327,    -1,   134,   111,    -1,   134,   337,
      -1,   134,    30,   327,   347,    -1,    -1,   136,   344,   337,
      -1,    -1,   136,    30,   345,   346,   347,    -1,   329,    -1,
     113,   329,    -1,   114,   329,    -1,   346,   115,   346,    -1,
     135,    -1,   132,    -1,     1,    -1,   137,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   426,   426,   435,   436,   445,   454,   455,   467,   468,
     473,   478,   487,   472,   499,   501,   503,   507,   510,   514,
     515,   519,   525,   529,   535,   540,   541,   546,   550,   552,
     557,   559,   564,   572,   574,   576,   578,   580,   581,   583,
     585,   587,   589,   593,   597,   599,   615,   620,   627,   614,
     637,   638,   640,   644,   645,   649,   663,   664,   683,   682,
     694,   695,   701,   710,   711,   715,   720,   725,   730,   735,
     740,   745,   760,   761,   762,   763,   764,   769,   768,   789,
     788,   808,   809,   811,   813,   819,   820,   824,   825,   826,
     835,   834,   843,   844,   848,   849,   851,   855,   858,   865,
     869,   872,   875,   882,   890,   897,   905,   917,   919,   923,
     924,   925,   927,   931,   933,   938,   945,   950,   957,   961,
     967,   968,   977,   985,   984,   993,   994,   996,  1008,  1009,
    1014,  1020,  1025,  1024,  1034,  1041,  1043,  1047,  1048,  1049,
    1051,  1055,  1055,  1065,  1067,  1071,  1072,  1073,  1075,  1079,
    1079,  1085,  1090,  1095,  1104,  1105,  1109,  1110,  1111,  1113,
    1117,  1119,  1126,  1128,  1133,  1135,  1141,  1142,  1144,  1146,
    1152,  1153,  1161,  1163,  1168,  1169,  1173,  1174,  1188,  1189,
    1193,  1210,  1209,  1220,  1222,  1224,  1226,  1228,  1230,  1232,
    1246,  1246,  1252,  1251,  1257,  1258,  1262,  1268,  1280,  1279,
    1316,  1317,  1319,  1321,  1323,  1327,  1335,  1337,  1364,  1366,
    1369,  1370,  1371,  1373,  1383,  1384,  1386,  1387,  1388,  1390,
    1394,  1402,  1412,  1413,  1417,  1418,  1422,  1423,  1427,  1430,
    1434,  1435,  1440,  1443,  1451,  1450,  1458,  1458,  1464,  1465,
    1470,  1473,  1473,  1477,  1479,  1477,  1488,  1487,  1494,  1493,
    1503,  1502,  1508,  1508,  1519,  1518,  1527,  1526,  1536,  1535,
    1547,  1546,  1557,  1556,  1568,  1567,  1579,  1578,  1595,  1594,
    1603,  1602,  1610,  1611,  1612,  1613,  1614,  1614,  1621,  1620,
    1627,  1626,  1631,  1631,  1640,  1640,  1652,  1654,  1659,  1661,
    1663,  1675,  1675,  1682,  1683,  1687,  1688,  1692,  1695,  1700,
    1708,  1710,  1712,  1713,  1717,  1724,  1734,  1735,  1740,  1739,
    1747,  1746,  1771,  1770,  1779,  1778,  1802,  1803,  1807,  1812,
    1813,  1817,  1822,  1823,  1827,  1828,  1832,  1838,  1846,  1849,
    1848,  1857,  1859,  1864,  1866,  1869,  1880,  1879,  1888,  1894,
    1895,  1900,  1903,  1904,  1905,  1924,  1923,  1932,  1943,  1951,
    1959,  1967,  1975,  1981,  1989,  1990,  1994,  1995,  1997,  2000,
    2001,  2005,  2006,  2008,  2011,  2015,  2019,  2025,  2029,  2033,
    2039,  2040,  2044,  2051,  2064,  2070,  2074,  2080,  2084,  2090,
    2095,  2098,  2102,  2109,  2112,  2119,  2124,  2129,  2134,  2139,
    2144,  2149,  2154,  2159,  2167,  2170,  2176,  2179,  2183,  2190,
    2194,  2202,  2206,  2211,  2217,  2216,  2228,  2233,  2239,  2238,
    2250,  2255,  2260,  2266,  2267,  2269,  2271,  2273,  2275,  2277,
    2281,  2285,  2289,  2296,  2301,  2306,  2319,  2327,  2329,  2331,
    2333,  2335,  2336,  2337,  2338,  2339,  2343,  2347,  2358,  2359,
    2369,  2379,  2380,  2382,  2384,  2386,  2388,  2390,  2392,  2394,
    2396,  2398,  2400,  2402,  2404,  2407,  2406,  2411,  2410,  2414,
    2416,  2419,  2418,  2423,  2422,  2426,  2428,  2430,  2432,  2434,
    2436,  2438,  2440,  2442,  2444,  2446,  2448,  2450,  2452,  2472,
    2473,  2474,  2476,  2478,  2480,  2481,  2482,  2484,  2517,  2522,
    2531,  2536,  2540,  2551,  2560,  2564,  2565,  2585,  2586,  2590,
    2594,  2599,  2608,  2612,  2616,  2623,  2625,  2627,  2632,  2632,
    2644,  2644,  2651,  2658,  2661,  2664,  2673,  2678,  2680,  2685
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LOWEST", "IF", "ELSE", "'?'", "':'",
  "OROR", "ANDAND", "ANDANDAND", "'|'", "'^'", "XNOR", "'&'",
  "CASE_INEQUALITY", "CASE_EQUALITY", "LOGICAL_INEQUALITY",
  "LOGICAL_EQUALITY", "GE", "GT", "LE", "LT", "RIGHT_SHIFT", "LEFT_SHIFT",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'('", "'['", "'!'", "'~'",
  "REDUCTION_NOR", "REDUCTION_NAND", "UNARY", "IDENTIFIER",
  "HIERARCHY_IDENTIFIER", "STARARROW", "EQUALARROW", "SPECPARAM", "SETUP",
  "HOLD", "PERIOD", "WIDTH", "SKEW", "RECOVERY", "SETUPHOLD", "NOCHANGE",
  "MODULE", "MACROMODULE", "ENDMODULE", "SPECIFY", "ENDSPECIFY",
  "PRIMITIVE", "ENDPRIMITIVE", "TABLE", "ENDTABLE", "TASK", "ENDTASK",
  "FUNCTION", "ENDFUNCTION", "PARAMETER", "INPUT", "OUTPUT", "INOUT",
  "NETTYPE", "TRIREG", "SCALARED", "VECTORED", "REG", "TIME", "INTEGER",
  "REAL", "EVENT", "ASSIGN", "DEFPARAM", "STRENGTH0", "STRENGTH1",
  "SUPPLY0", "SUPPLY1", "LOGIC", "BUFFER", "BUFIF", "CMOS", "PULL", "TRAN",
  "INITIAL", "ALWAYS", "CASE", "CASEZ", "CASEX", "ENDCASE", "FOREVER",
  "REPEAT", "WHILE", "FOR", "WAIT_", "ARROW_", "DISABLE", "DEASSIGN",
  "FORCE", "RELEASE", "DEFAULT", "BEGIN", "END", "FORK", "JOIN",
  "SYSTEM_IDENTIFIER", "STRING", "NUMBER", "BASE_NUMBER", "POSEDGE",
  "NEGEDGE", "OR", "SMALL", "MEDIUM", "LARGE", "EDGE", "DOUBLE_DIGIT",
  "UDP_CHAR", "UDP_ONE", "UDP_ZERO", "UDP_X", "UDP_B", "UDP_HOOK", "EOF_",
  "','", "'.'", "'{'", "'}'", "']'", "'='", "'#'", "')'", "'@'", "';'",
  "$accept", "source_text", "description_list", "description", "module",
  "@1", "@2", "@3", "module_item_list", "list_of_ports_o", "port_clist",
  "port", "port_expression_o", "port_reference_clist", "port_reference",
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
  "list_of_param_overrides", "reg_decl_identifiers", "reg_decl_identifier",
  "non_reg_decl_identifiers", "decl_identifiers", "charge_strength_o",
  "drive_strength_o", "drive_strength", "strength0", "strength1", "xrange",
  "range", "gate_declaration", "@15", "gatetype", "gate_description",
  "@16", "@17", "gate_instance_clist", "gate_instance",
  "UDP_or_module_instantiation", "@18", "xparameter_value_assignment",
  "constant_expression_clist", "terminal_clist", "module_instance_clist",
  "module_instance", "list_of_module_connections_o",
  "module_port_connection_clist", "module_port_connection",
  "named_port_connection_clist", "named_port_connection",
  "initial_statement", "@19", "always_statement", "@20",
  "statement_or_null", "statement", "@21", "@22", "@23", "@24", "@25",
  "@26", "@27", "@28", "@29", "@30", "@31", "@32", "@33", "@34", "@35",
  "@36", "@37", "@38", "@39", "@40", "@41", "elsepart", "case",
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
  "mintypmax_expression_triplet", "expression", "@52", "@53", "@54", "@55",
  "primary", "concatenation", "function_call", "identifier",
  "IDENTIFIER_or_OR", "delay_o", "delay", "mintypmax_clist",
  "delay_control", "event_control", "@56", "@57", "event_expression", "rp",
  "rb", "sc", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,    63,    58,   261,   262,
     263,   124,    94,   264,    38,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,    43,    45,    42,    47,    37,
      40,    91,    33,   126,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,    44,    46,
     123,   125,    93,    61,    35,    41,    64,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   138,   139,   139,   139,   139,   140,   140,   141,   141,
     143,   144,   145,   142,   146,   146,   146,   147,   147,   148,
     148,   148,   148,   149,   149,   150,   150,   150,   151,   151,
     152,   152,   152,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   155,   156,   157,   154,
     158,   158,   158,   159,   159,   160,   161,   161,   163,   162,
     164,   164,   165,   166,   166,   167,   167,   167,   167,   167,
     167,   167,   168,   168,   168,   168,   168,   170,   169,   172,
     171,   173,   173,   173,   173,   174,   174,   175,   175,   175,
     177,   176,   178,   178,   178,   178,   178,   179,   179,   180,
     181,   181,   181,   182,   183,   183,   183,   184,   184,   184,
     184,   184,   184,   185,   185,   185,   186,   187,   188,   189,
     189,   189,   190,   192,   191,   193,   193,   193,   194,   194,
     194,   194,   196,   195,   197,   198,   198,   198,   198,   198,
     198,   200,   199,   201,   201,   201,   201,   201,   201,   203,
     202,   204,   205,   205,   206,   206,   206,   206,   206,   206,
     207,   207,   208,   208,   209,   209,   210,   210,   210,   210,
     211,   211,   212,   212,   213,   213,   214,   214,   215,   215,
     216,   218,   217,   219,   219,   219,   219,   219,   219,   219,
     221,   220,   222,   220,   223,   223,   224,   224,   226,   225,
     227,   227,   227,   227,   227,   227,   228,   228,   229,   229,
     229,   229,   229,   229,   230,   230,   230,   230,   230,   230,
     231,   231,   232,   232,   233,   233,   233,   233,   234,   234,
     235,   235,   236,   236,   238,   237,   240,   239,   241,   241,
     242,   243,   242,   244,   245,   242,   246,   242,   247,   242,
     248,   242,   249,   242,   250,   242,   251,   242,   252,   242,
     253,   242,   254,   242,   255,   242,   256,   242,   257,   242,
     258,   242,   242,   242,   242,   242,   259,   242,   260,   242,
     261,   242,   262,   242,   263,   242,   264,   264,   265,   265,
     265,   267,   266,   268,   268,   268,   268,   269,   269,   269,
     270,   270,   270,   270,   271,   271,   272,   272,   274,   273,
     275,   273,   277,   276,   278,   276,   279,   279,   279,   280,
     280,   280,   281,   281,   282,   282,   283,   283,   284,   285,
     284,   286,   286,   287,   287,   287,   289,   288,   290,   291,
     291,   291,   292,   292,   292,   294,   293,   295,   296,   296,
     296,   296,   296,   296,   297,   297,   297,   297,   297,   298,
     298,   298,   298,   298,   299,   299,   299,   300,   300,   300,
     301,   301,   301,   301,   302,   303,   303,   304,   304,   305,
     306,   307,   307,   308,   308,   309,   309,   309,   309,   309,
     309,   309,   309,   309,   310,   310,   311,   312,   312,   313,
     313,   314,   314,   314,   315,   314,   316,   316,   317,   316,
     318,   318,   318,   319,   319,   319,   319,   319,   319,   319,
     320,   321,   321,   322,   322,   322,   323,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   325,   326,   327,   327,
     328,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   330,   329,   331,   329,   329,
     329,   332,   329,   333,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   335,   335,
     336,   336,   336,   337,   337,   338,   338,   339,   339,   340,
     340,   340,   341,   341,   341,   342,   342,   342,   344,   343,
     345,   343,   346,   346,   346,   346,   347,   348,   348,   349
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     2,     1,     1,
       0,     0,     0,     9,     0,     2,     2,     3,     0,     1,
       3,     1,     3,     1,     5,     1,     3,     0,     1,     3,
       1,     4,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     0,    11,
       0,     2,     2,     1,     1,     3,     0,     5,     0,     4,
       1,     2,     2,     0,     2,     1,     1,     1,     1,     1,
       1,     4,     1,     1,     1,     1,     1,     0,     7,     0,
       8,     0,     1,     1,     1,     0,     2,     1,     1,     1,
       0,     5,     1,     3,     1,     3,     3,     3,     3,     0,
       4,     4,     4,     4,     2,     2,     2,     1,     3,     1,
       2,     3,     3,     2,     2,     2,     1,     1,     0,     4,
       4,     4,     3,     0,     5,     1,     1,     1,     1,     2,
       2,     0,     0,     5,     3,     1,     3,     1,     2,     3,
       3,     0,     4,     1,     3,     1,     2,     3,     3,     0,
       4,     3,     3,     5,     1,     3,     1,     2,     3,     3,
       1,     6,     1,     3,     1,     3,     0,     3,     3,     3,
       0,     1,     5,     5,     1,     1,     1,     1,     0,     1,
       5,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     0,     4,     1,     3,     4,     3,     0,     5,
       0,     2,     4,     4,     6,     8,     1,     3,     1,     3,
       1,     2,     3,     3,     1,     3,     1,     2,     3,     3,
       4,     3,     1,     1,     1,     3,     1,     3,     1,     0,
       1,     3,     5,     4,     0,     3,     0,     3,     1,     1,
       2,     0,     7,     0,     0,     8,     0,     3,     0,     6,
       0,     6,     0,    10,     0,     3,     0,     3,     0,     6,
       0,     6,     0,     5,     0,     6,     0,     6,     0,     6,
       0,     4,     1,     1,     1,     1,     0,     4,     0,     6,
       0,     4,     0,     6,     0,     4,     0,     2,     1,     1,
       1,     0,     4,     1,     2,     1,     2,     3,     3,     2,
       1,     3,     1,     3,     1,     3,     0,     1,     0,     4,
       0,     7,     0,     4,     0,     7,     0,     2,     2,     0,
       2,     2,     0,     2,     1,     1,     2,     5,     2,     0,
       6,     1,     1,     1,     4,     6,     0,     3,     2,     0,
       2,     2,     1,     1,     1,     0,     4,     5,     9,    10,
      10,    11,     5,     6,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     3,     1,     4,     6,     1,     4,     6,
       1,     5,     7,    13,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     4,    10,    10,     8,     7,    10,
      10,    10,    12,    11,     0,     2,     1,     2,     4,     2,
       4,     0,     1,     1,     0,     5,     1,     1,     0,     5,
       1,     3,     1,     1,     3,     2,     3,     3,     3,     3,
       1,     1,     1,     1,     4,     6,     3,     1,     1,     3,
       3,     1,     2,     3,     3,     3,     1,     1,     1,     5,
       5,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     0,     4,     0,     4,     3,
       3,     0,     4,     0,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     1,     1,
       1,     1,     4,     6,     1,     1,     3,     3,     3,     6,
       4,     4,     1,     1,     1,     1,     1,     0,     1,     2,
       2,     4,     1,     3,     5,     2,     2,     4,     0,     3,
       0,     5,     1,     2,     2,     3,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,     4,   241,   494,   493,    10,    46,   278,   246,   248,
     250,   252,   268,   270,   276,   280,   282,   284,   308,   312,
     329,     5,     0,     0,   508,     0,     0,     6,     8,     9,
       2,     0,     0,   272,   273,   274,   275,     0,   331,   332,
     333,   254,   256,     0,     0,     0,     0,   243,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   316,     0,
     319,   519,     0,   328,   431,   427,   428,     0,   333,     0,
     505,   506,   510,     0,     1,     3,     7,   288,   289,   290,
     244,   240,   262,   291,     0,     0,   326,   243,   243,     0,
     495,   496,    11,    47,     0,   247,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   310,     0,   314,     0,   306,
     432,     0,   426,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   492,   478,   479,   480,     0,     0,
     438,   441,   484,   485,   481,     0,   509,     0,     0,     0,
       0,     0,     0,     0,   302,     0,   300,     0,   255,   238,
     239,   257,     0,    18,    18,     0,     0,     0,     0,     0,
       0,   436,   271,   277,   281,     0,   285,   322,   318,   309,
     317,   322,   321,   313,   320,     0,   304,   307,   433,   434,
     435,   429,   430,   448,   450,   451,   446,   442,   443,     0,
       0,   444,   445,   449,   447,   306,     0,     0,   300,   516,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   463,
     461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   455,   457,   306,     0,     0,     0,   512,     0,
       0,     0,     0,     0,     0,     0,   292,     0,     0,     0,
     334,   243,     0,     0,     0,     0,   243,   243,     0,   291,
     243,     0,   316,   319,   306,     0,   487,   486,     0,   488,
       0,     0,     0,   466,   465,   472,   473,   474,   471,     0,
       0,   460,   459,   470,   469,   468,   467,   476,   475,   452,
     453,   454,     0,     0,     0,     0,   437,   513,   514,     0,
     511,     0,   263,     0,     0,     0,     0,   303,   301,   327,
       0,   286,    21,    30,     0,     0,     0,    19,    23,    25,
      12,    48,   279,   249,   251,     0,   269,   283,   178,   178,
     178,   178,   116,   117,   324,   325,    99,    99,    99,     0,
     323,     0,   305,   330,   491,     0,     0,     0,   464,   462,
     456,   458,   490,     0,   482,   515,     0,   265,   267,   259,
     261,   335,   243,   242,     0,     0,     0,    28,     0,    17,
      14,    50,     0,     0,    90,   179,   113,   115,   114,     0,
       0,     0,   311,   315,     0,   477,   439,     0,   437,   295,
     243,     0,   293,     0,   287,     0,    27,     0,    26,    22,
      20,     0,     0,     0,     0,     0,   156,   160,     0,   154,
     164,     0,   162,     0,   489,   483,   243,   299,   296,   245,
     294,   243,     0,    31,     0,    29,    16,   200,    13,   336,
       0,    81,   178,   178,   178,   125,   123,   170,     0,   126,
     127,   183,   185,   186,   187,   188,   189,   234,   236,   184,
      15,    44,    45,    33,    36,    34,    99,    35,   118,    99,
     131,    40,    99,    39,    37,   181,    38,    42,    43,    41,
      52,     0,     0,    51,    54,     0,    53,   243,     0,    94,
       0,     0,    92,     0,   157,     0,   100,     0,   102,     0,
     101,   298,   297,     0,    24,     0,   198,   339,     0,    83,
      84,     0,    82,   104,   105,   106,   166,     0,   497,   171,
       0,     0,   243,   243,     0,     0,     0,     0,     0,   497,
     128,   132,   190,     0,     0,    58,     0,   253,     0,     0,
       0,     0,    91,     0,   158,   159,   155,   165,   163,    32,
       0,   201,     0,   337,     0,    77,     0,     0,   131,   174,
     176,   175,   177,     0,     0,     0,   134,   498,     0,   151,
       0,   235,   237,   109,   107,     0,   137,   164,     0,   135,
       0,     0,   129,   130,   122,     0,   182,     0,   192,    55,
       0,    63,    49,   518,   517,   180,    97,    98,   437,    95,
      96,    93,     0,     0,   206,     0,   437,   216,     0,     0,
       0,   214,   341,     0,   345,     0,     0,     0,     0,     0,
       0,     0,     0,   338,   340,   342,   343,     0,   344,    85,
      79,     0,     0,     0,   497,     0,     0,     0,   499,   500,
       0,   152,   110,     0,   103,   141,   138,     0,   121,   119,
     120,   145,     0,   143,     0,     0,     0,     0,   194,     0,
     422,   421,     0,    63,    60,     0,     0,     0,     0,   202,
       0,   516,   226,     0,     0,   222,   224,   223,   230,   228,
       0,   217,     0,   199,     0,     0,   401,   401,     0,     0,
     401,     0,   401,     0,     0,     0,   243,    85,   167,   168,
     169,   124,     0,     0,   502,     0,     0,   111,   112,   108,
       0,     0,   139,   140,   136,   146,     0,   133,   149,   210,
       0,   208,     0,     0,   191,     0,    57,    59,    61,     0,
      65,    68,    67,    70,    69,    66,    64,    62,     0,   161,
     207,     0,     0,   221,     0,     0,     0,   218,   219,   215,
       0,   379,     0,   402,   403,     0,     0,     0,     0,   406,
     407,     0,     0,     0,     0,     0,     0,     0,     0,   356,
     364,   381,   382,     0,   354,     0,     0,    86,    87,    89,
      88,     0,   243,   172,   173,     0,   501,   153,   142,   147,
     148,   144,     0,   211,     0,   197,     0,   195,   193,    74,
      73,    72,    75,    76,     0,     0,     0,   204,     0,   227,
     225,   231,   220,   384,   346,   404,   401,   397,   423,   401,
     408,     0,   399,     0,   401,   401,   401,   401,     0,     0,
     375,   376,   377,   378,     0,     0,     0,     0,     0,     0,
     370,   374,    78,     0,   503,   150,   212,   213,   209,   196,
       0,   440,     0,     0,   233,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,   357,
     358,   355,     0,   361,     0,   367,     0,   359,     0,     0,
     374,   347,    80,     0,    71,   205,   232,   412,   410,     0,
       0,     0,     0,   420,   398,   413,     0,   437,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
     365,     0,     0,     0,     0,     0,   352,     0,     0,   504,
       0,   405,   394,     0,   415,     0,     0,     0,     0,     0,
     424,   394,   409,   396,   395,     0,   394,   388,   394,   394,
       0,     0,     0,   353,     0,     0,     0,   362,   363,   360,
       0,     0,   411,     0,   414,   418,   417,   419,   416,     0,
       0,   387,     0,     0,     0,     0,     0,   366,     0,   380,
       0,     0,   368,     0,     0,     0,   371,     0,   425,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,   385,   386,   389,   390,   391,     0,     0,   348,     0,
     369,     0,     0,     0,   372,     0,   393,   350,   349,     0,
       0,   392,   351,     0,     0,     0,     0,   373
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    25,    26,    27,    28,    44,   153,   360,   391,   243,
     306,   307,   308,   356,   309,   440,    29,    45,   154,   361,
     392,   463,   464,   465,   516,   571,   643,   644,   645,   716,
     784,   441,   609,   442,   677,   491,   676,   757,   324,   395,
     471,   472,   369,   325,   760,   446,   555,   326,   327,   328,
     505,   447,   448,   449,   496,   450,   509,   451,   565,   452,
     558,   559,   690,   632,   633,   772,   453,   500,   398,   399,
     403,   401,   538,   498,   499,   543,   544,   364,   365,   454,
     512,   455,   566,   567,   639,   637,   638,   456,   532,   486,
     583,   700,   590,   591,   654,   655,   656,   657,   658,   457,
     502,   458,   503,   148,   149,    43,    31,   137,    47,    48,
      49,    50,    87,    88,   141,   142,   138,   139,   140,    51,
      52,    53,    46,    54,    55,    56,   353,    80,    32,   143,
     381,   382,   383,   175,   176,    33,    58,   167,    34,    60,
     171,   106,   108,   252,   330,    35,    36,    62,    37,    38,
     459,   487,   533,   534,   604,   605,   665,   606,   675,   753,
     856,   754,   857,   819,   820,   815,   816,   730,   948,   755,
     607,   608,   881,   914,   736,   742,   737,   835,   743,   840,
     869,   874,   875,   642,   797,    39,    67,   160,   197,   821,
     577,   378,   282,   283,   270,   269,   131,   132,   133,   134,
      92,   546,   547,   685,    41,    42,    73,   135,   229,   257,
     575,   150
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -826
static const short int yypact[] =
{
     538,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,    58,   117,
      51,  -826,    53,    10,   107,   237,   -19,  -826,  -826,  -826,
    -826,   513,    51,  -826,  -826,  -826,  -826,     1,  -826,  -826,
      80,  -826,  -826,   239,    -9,   248,   245,  1349,   280,   301,
     309,   392,   184,   184,   245,   245,   245,   257,  -826,   397,
    -826,  -826,   398,  -826,  -826,  -826,  -826,     4,   416,  2182,
    -826,  -826,  -826,   184,  -826,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,   -51,   -16,  1643,  2182,  -826,  1305,  1305,  2182,
    -826,  -826,  -826,  -826,   356,  -826,  2182,  2182,   245,  2182,
      51,    51,    51,   359,    51,  -826,   926,  -826,  1065,  2182,
     245,    85,  -826,   361,   361,   361,   361,   361,   361,  1682,
     361,   361,   361,   361,   470,  -826,  -826,  -826,  1643,   380,
    2383,  -826,  -826,  -826,   296,   676,  -826,   504,  2182,   424,
     407,   424,   407,  2182,  -826,   244,  2505,  1890,  -826,  -826,
    -826,  -826,  1526,   521,   521,  2182,  1526,  1526,    51,   430,
     380,  2505,  -826,  -826,  -826,  2182,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,   303,  -826,  2505,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,   380,
     380,  -826,  -826,  -826,  -826,  2182,   153,   443,  2012,  -826,
    -826,  2182,  2182,  2182,  2182,  2182,  2182,  2182,  2182,  -826,
    -826,  2182,  2182,  2182,  2182,  2182,  2182,  2182,  2182,  2182,
    2182,  2182,  -826,  -826,  2182,  2182,  2182,  2182,  2505,   -63,
    2182,  1478,  2182,  2182,  2182,  2182,  2505,  1731,    51,  2182,
    -826,  1305,    24,    51,    51,  1478,  1349,  1349,  2182,  -826,
    1305,  1478,   411,   411,  2182,    51,  -826,  -826,   303,  -826,
    1643,  2409,  2433,  1448,  2524,  2542,  1848,  1848,  2059,  2182,
    2182,   483,   483,   712,   712,   712,   712,   558,   558,   468,
     468,  -826,  2182,  2182,   303,   582,  1939,  2505,  2505,   676,
    -826,  1526,  -826,  1478,  1478,  1478,  1478,  -826,  2505,  -826,
    1963,   586,  -826,   570,   574,   576,   324,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  1478,  -826,  -826,   590,   590,
     590,   590,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  1112,
    -826,  1185,  -826,  -826,  -826,   259,  2182,  2182,   483,   483,
    -826,  -826,  -826,  2182,  -826,  -826,  1594,  -826,  -826,  -826,
    -826,  -826,  1305,  -826,  2182,   593,   276,  -826,   132,  -826,
    -826,  -826,   245,  2182,  -826,  -826,  -826,  -826,  -826,    36,
     594,   605,  -826,  -826,   530,  2505,  2505,   531,  2505,     0,
     423,  1555,  -826,    28,  -826,    12,   -14,   576,  -826,  -826,
    -826,  2299,   379,   380,   657,    41,  -826,   635,     8,  -826,
    -826,   -82,  -826,   -41,  -826,  -826,  1305,  -826,     0,  -826,
    -826,  1305,  2182,  -826,   380,  -826,  -826,   535,  -826,  -826,
     633,    40,   590,   590,   590,  -826,  -826,   641,   184,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,
     290,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,
    -826,   638,   639,  -826,  -826,   616,  -826,  1349,  2182,  -826,
     545,    23,  -826,  2182,   642,    52,  -826,   643,  -826,   645,
    -826,  -826,  -826,   551,  -826,  2210,  -826,  -826,    51,  -826,
    -826,   649,  -826,  -826,  -826,  -826,   666,   500,   564,  -826,
     216,   566,  1349,  1349,   204,   279,   594,   590,   590,   564,
    -826,  -826,   564,    51,   567,  -826,   647,  -826,    38,  2182,
     667,   298,  -826,   698,  -826,  -826,  -826,  -826,  -826,  -826,
    1682,  -826,    67,  -826,   855,  -826,    51,   491,   290,  -826,
    -826,  -826,  -826,   579,   587,   282,  -826,  -826,   184,  -826,
    2182,  -826,  -826,  -826,  -826,    61,  -826,   584,    73,  -826,
     -82,   -82,  -826,  -826,  -826,   195,  -826,   434,  -826,  -826,
     265,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  2457,  -826,
    -826,  -826,  2182,   341,  -826,   353,  2383,  -826,   840,   689,
      75,  -826,  -826,   690,  -826,   692,   693,   697,   699,   700,
     701,   714,   717,  -826,  -826,  -826,  -826,   719,  -826,  -826,
    -826,   380,   380,   380,   564,   219,   278,  2182,  -826,  -826,
     595,  -826,   705,   317,  -826,  -826,   715,   363,  -826,  -826,
    -826,  -826,    76,  -826,   618,  1770,   723,   277,  -826,   434,
    -826,  -826,    51,   702,  -826,   226,  2182,   623,  2182,  -826,
    2182,   271,  -826,   720,   380,   628,  -826,   630,  -826,  2505,
     840,   457,   306,  -826,  2182,    41,   453,   453,   455,   455,
     453,   455,   453,   455,   190,   626,  1261,  -826,  -826,  -826,
    -826,  -826,   380,   380,   634,   380,  2182,  -826,  -826,  -826,
    2182,   584,  -826,  -826,  -826,   245,   215,  -826,  -826,  -826,
      74,  2505,  1770,   434,  -826,   277,  -826,  -826,  -826,   474,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  2481,  -826,
    -826,   401,   734,  -826,  1219,   637,   380,  -826,  -826,  -826,
     380,  2505,    23,  -826,  -826,   740,   644,   184,   646,  -826,
    -826,   742,   648,   184,   651,   652,   653,   655,   656,  -826,
     744,  -826,  -826,   238,  -826,   381,  2238,  -826,  -826,  -826,
    -826,   731,  1261,  -826,  -826,  2182,  -826,  -826,  2505,  -826,
    -826,  -826,  2182,  2182,  1819,  -826,    74,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,   474,  2182,  2182,  -826,  2035,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,   453,   755,   746,   453,
    -826,  2182,   782,  2182,   453,   453,   453,   453,  2182,   756,
    -826,  -826,  -826,  -826,   396,   370,   428,   205,  1682,    51,
    -826,  -826,  -826,   732,   668,  2505,  2505,  -826,  2505,  -826,
     380,  2505,   662,  1526,  -826,    20,   672,   387,  2182,   673,
      20,  2085,   387,  1502,   674,   675,   679,   680,    25,  -826,
    -826,  -826,   405,  -826,   405,   773,    91,  -826,   780,   684,
     380,  -826,  -826,  2182,  -826,  -826,  -826,  -826,   685,    38,
    2182,   387,   777,  -826,  -826,   636,   808,  1987,  2182,    38,
     779,   380,  -826,  2182,    51,  2182,  2182,  2182,  2182,  2182,
    -826,    91,   264,  2182,   783,   418,  -826,   405,  2182,  -826,
      20,  -826,  2085,   686,  -826,   265,   265,   265,   265,  2182,
    -826,  2085,  -826,  -826,  -826,    51,  2085,  -826,  2085,  2085,
    2109,  2160,    38,  -826,  2182,   812,    26,  -826,  -826,  -826,
     304,   409,  -826,   380,  -826,  -826,  -826,  -826,  -826,   694,
     380,  -826,   380,   380,   380,  2182,  2182,  -826,   380,  2505,
    2182,  2182,  -826,  2182,   815,  2182,  -826,    51,  -826,    51,
      51,    51,    51,  2085,  1526,   380,   380,    38,   380,  2182,
     417,  -826,  -826,  -826,  -826,  -826,   380,    51,  -826,   380,
    -826,   380,   380,  2182,  -826,    51,  -826,  -826,  -826,   380,
     696,  -826,  -826,  2182,   703,  2182,   380,  -826
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -826,  -826,  -826,   801,  -826,  -826,  -826,  -826,  -826,   678,
    -826,   471,   442,  -826,  -275,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,   191,  -826,  -826,
      49,  -826,  -826,  -826,  -826,  -826,   158,  -826,  -389,  -826,
     171,    34,  -198,  -388,   155,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,   299,  -826,  -826,  -826,
    -826,   -11,  -826,  -826,   -77,  -826,  -826,  -826,  -826,   152,
    -826,   143,  -826,  -826,  -826,   228,   234,   -28,  -407,  -826,
    -826,  -826,  -826,  -826,  -826,   211,   159,  -826,  -826,  -826,
    -826,   166,  -826,    -6,   201,  -826,   131,  -826,   144,  -826,
    -826,  -826,  -826,     5,    11,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,   -94,  -826,
    -826,   490,   -78,  -125,   622,  -826,  -826,  -826,  -826,  -826,
    -826,   627,   629,   710,  -826,  -826,  -826,  -826,   -38,    16,
    -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,  -826,   133,
    -788,  -707,  -235,  -826,  -755,  -825,  -378,  -826,  -703,  -826,
    -826,  -826,  -462,  -826,  -657,   -79,  -826,  -826,  -826,  -826,
    -806,  -635,    14,  -214,   146,   212,  -826,  -826,   -84,   -68,
    -826,   -40,  -826,  -826,  -826,  -826,   803,  -826,  -826,   315,
    -826,  -468,   378,  -826,   336,   391,  -826,  -826,   602,   157,
    -514,   -20
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -438
static const short int yytable[] =
{
      63,   129,   443,   444,   158,   110,   145,  -302,    94,   474,
     738,    30,    81,   745,   492,   747,   102,   103,   104,   412,
      86,   867,    82,   303,   520,   302,   573,   573,    90,   130,
     357,     5,   889,   951,   879,   411,     6,   396,    65,   573,
      69,   564,   469,   510,   146,   147,   477,     3,     4,   152,
     196,   190,   289,   525,    64,    61,   156,   157,    95,   161,
     159,   303,   622,   859,   891,    57,   892,   925,   587,   177,
     258,   363,   199,   397,   626,   773,   661,   695,   470,   130,
     162,   163,   164,  -264,   166,  -266,   180,   479,   198,   397,
       3,     4,   894,   151,   932,   228,    61,   588,   231,   284,
     562,   563,   849,   236,   589,   954,    91,   851,    75,   930,
      84,    85,   415,   489,   490,   245,   305,   170,  -258,   174,
    -260,    70,     3,     4,    59,   251,   178,   181,  -302,   370,
     371,   510,   111,   389,    83,   112,   475,    72,   248,   836,
     868,   285,   839,   931,   413,    61,   681,   844,   845,   846,
     847,   521,   -27,   304,   305,   177,   237,   574,   574,   -27,
      61,   261,   262,   263,   264,   265,   266,   267,   268,   303,
     574,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   335,    22,   177,   286,   287,   288,    61,   623,
     291,   749,   293,   294,   295,   296,   631,   298,    61,   300,
     970,   627,   774,   662,   696,   553,   809,   882,   315,   199,
      61,   292,    61,    61,   177,    22,   770,    61,   299,   895,
     146,     3,     4,   310,   311,   312,   199,   750,   990,   338,
     339,   317,     3,     4,    66,   333,   903,    74,   994,   809,
     996,   554,   340,   341,   812,   813,   301,   966,   504,   228,
     968,   506,     3,     4,   511,   316,   709,   313,   314,   377,
     -27,   304,   305,   810,   811,   894,   982,   -27,   393,    89,
     385,   924,  -203,   347,   348,   349,   350,   812,   813,   394,
     556,   237,     3,     4,   259,    93,   200,   758,   759,   810,
     811,   366,   367,   368,   105,   362,   375,   376,   540,   580,
     542,  -203,   238,   751,   752,   894,   146,   728,  -203,   241,
      96,   953,   617,   246,   247,    40,   557,   250,   688,     3,
       4,   363,   179,   182,   159,    22,   224,   225,   483,   810,
     811,    97,   255,   814,   890,   470,   588,    68,    71,    98,
     170,   146,   174,   589,   548,    22,   256,   710,   711,   712,
     713,   714,   715,    61,   689,   901,   539,   384,   541,   507,
     508,    68,    40,    61,   693,   912,   814,   100,   101,    68,
      68,    68,   237,   758,   759,    22,   640,   641,   476,   199,
     460,   478,   749,   480,   518,   407,   290,   237,   136,   523,
     374,   119,   895,   618,   493,   494,   495,   850,     3,     4,
     691,   531,    40,    40,   387,   703,   853,   388,   947,   812,
     813,   481,   952,    68,    61,   334,   482,   871,   750,   928,
     872,    40,    99,    40,   873,    68,    68,     2,   109,   853,
     406,   254,   895,   750,   107,   576,   -56,   852,   199,   858,
     933,   342,   855,   422,   423,   424,   584,    85,   346,   940,
     461,   522,   358,   980,   942,   855,   943,   944,   854,   199,
       3,     4,   585,   359,   635,   855,   621,   462,   535,   648,
     124,   636,   126,   127,   318,   232,   649,   234,   517,   578,
     549,   650,   319,   320,   321,   322,   323,   588,   651,   155,
     586,   128,   165,   569,   589,   221,   222,   223,   647,     7,
     195,   976,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   551,   552,   199,   610,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,   634,    18,   786,
      19,   233,    20,   235,   230,   624,   787,   955,   628,     1,
     629,   630,     2,    24,   199,   983,   445,   466,   659,   684,
     467,   242,   199,    22,   579,   581,    40,    23,    23,    24,
      61,    40,    40,   249,   720,    40,   733,   734,   739,   740,
     663,   484,   735,   260,   741,     3,     4,   130,   539,   540,
     541,   542,   721,   219,   220,   221,   222,   223,     5,   343,
     744,   352,   746,     6,   748,   701,   779,   780,   781,   782,
     783,   354,   767,    77,    78,    79,   718,   611,   612,   613,
     130,   355,   697,   303,     7,   692,   694,   704,   769,   771,
     659,   363,   706,   386,   731,   717,   524,   526,  -243,  -243,
    -243,   400,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   402,    18,    40,    19,    40,    20,   560,   561,
     768,   905,   906,   907,   908,   727,   729,   634,   634,   927,
     929,   404,   701,   405,   468,    21,   473,    40,    22,   485,
     488,   497,    23,   515,    24,   513,   514,    68,   519,   397,
     527,   761,   528,   529,   659,   778,   536,   113,   114,   115,
     116,   935,   936,   937,   938,    40,   537,   824,   545,   550,
     570,   117,   118,   572,   470,   582,   119,   615,   120,   121,
     122,   123,   794,     3,     4,   616,   130,   625,   832,   660,
     664,    40,   666,   667,   848,   130,    40,   668,   686,   669,
     670,   671,   825,   826,   828,   217,   218,   219,   220,   221,
     222,   223,   687,   501,   672,   831,   130,   673,   833,   674,
     860,   698,   691,   702,   876,   719,   724,   722,   725,   756,
     707,   841,   765,   843,   788,   837,   653,   823,   678,   679,
     680,   795,   796,   800,   799,   808,   801,   838,   130,   803,
     804,   805,    40,   806,   807,   124,   125,   126,   127,   226,
     227,   822,   842,   750,   862,   899,   863,   865,   877,   861,
     870,   878,   885,   886,   893,   922,   128,   887,   888,   926,
     897,   723,   898,   900,   873,   909,   913,    40,    40,   950,
     855,   934,   969,   130,   993,   939,   958,    76,   414,   390,
     902,   995,   244,   830,   708,   762,   732,   614,   911,   763,
     764,   652,   766,   916,   683,   918,   919,   920,   921,   682,
     705,   113,   114,   115,   116,   790,   592,   775,   130,   593,
     619,   726,   777,   620,   917,   117,   118,   967,   776,   791,
     119,   410,   120,   121,   122,   123,   332,     3,     4,   329,
      68,   253,   331,   792,   949,  -383,   904,   793,   817,   802,
     568,   345,     0,     0,     0,   941,   594,   595,   596,   597,
     598,   599,   600,   601,   602,   963,   964,     0,     0,   603,
     949,     0,     0,   949,     0,   130,   183,   184,   185,   186,
     187,   188,     0,   191,   192,   193,   194,   168,     0,   949,
       2,     0,     0,   829,     0,     0,     0,   971,     0,   972,
     973,   974,   975,   130,     0,   834,     0,     0,     0,   124,
     125,   126,   127,   130,     0,   130,     0,   986,     0,     0,
       0,     0,     0,     3,     4,   991,     0,     0,  -229,   653,
     128,     0,     0,     0,     0,  -229,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   864,     0,     0,
     866,    40,     0,     0,     0,     0,     0,     0,     0,     0,
     884,     0,     7,     0,     0,     0,     0,     0,     0,     0,
      68,    68,     0,   896,     0,     0,  -243,  -243,  -243,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
       0,    18,   169,    19,     0,    20,     0,     0,   915,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   923,     0,
       0,     0,   798,     0,     0,     0,    22,     0,   798,     0,
      23,     0,    24,     0,     0,     0,   172,     0,     0,     2,
       0,     0,     0,     0,     0,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   956,     0,
     957,     0,     0,     0,     0,     0,     0,   959,     0,   960,
     961,   962,     3,     4,     0,   965,     0,     0,     0,     0,
       0,     0,     0,   168,     0,     0,     2,     0,     0,     0,
       0,   977,   978,   979,     0,   981,     0,   984,     0,     0,
       0,     0,     0,   985,     0,     0,   987,     0,   988,   989,
       0,     7,     0,     0,     0,     0,   992,     0,     0,     3,
       4,     0,     0,   997,     0,  -243,  -243,  -243,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,   173,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   172,     0,     7,     2,
       0,     0,     0,     0,     0,    22,     0,     0,     0,    23,
       0,    24,  -243,  -243,  -243,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,     0,    18,   372,    19,
     789,    20,     3,     4,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,     0,     0,     0,     0,     0,     0,
       0,     0,    22,     0,   117,   118,    23,     0,    24,   119,
       0,   120,   121,   122,   123,     0,     3,     4,     0,     0,
       0,     7,     0,     0,     0,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -243,  -243,  -243,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,   373,    20,     0,     0,     0,     3,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     0,     0,     0,     0,    22,     0,     0,     0,    23,
       0,    24,     0,     0,   318,   422,   423,   424,   124,   125,
     126,   127,   319,   320,   321,   322,   323,     7,     0,     0,
       0,     0,     3,     4,     0,     0,     0,  -229,     0,   128,
       0,     0,     0,     2,  -229,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,     0,    18,     0,    19,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     7,     0,     0,     0,     0,     3,     4,     0,     0,
       0,    22,     0,     0,     0,    23,     0,    24,    61,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
      18,     0,    19,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     7,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,    23,
       0,    24,    61,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,     0,    18,     0,    19,   204,    20,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,     0,    22,
       0,     0,     0,    23,   201,    24,   203,   204,     0,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   201,     0,
     203,   204,     0,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   201,     0,   203,   204,     0,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   408,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     117,   118,     0,     0,     0,   119,     0,   120,   121,   122,
     123,     0,     3,     4,     0,   379,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   113,   114,   115,   116,     0,
       0,     0,     0,     0,     0,    61,     0,     0,     0,   117,
     118,     0,     0,     0,   119,     0,   120,   121,   122,   123,
     883,     3,     4,     0,     0,     0,     0,   199,     0,     0,
       0,     0,     0,     0,   144,     0,     0,     0,   409,     0,
       0,     0,     0,     0,   113,   114,   115,   116,     0,   380,
       0,   199,     0,     0,   124,   125,   126,   127,   117,   118,
       0,     0,     0,   119,     0,   120,   121,   122,   123,     0,
       3,     4,     0,   189,     0,   128,     0,     0,     0,     0,
       0,     0,     0,   113,   114,   115,   116,     0,   380,     0,
       0,     0,     0,   124,   125,   126,   127,   117,   118,     0,
       0,     0,   119,     0,   120,   121,   122,   123,     0,     3,
       4,     0,     0,     0,   128,     0,     0,     0,     0,     0,
       0,     0,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,     0,     0,     0,     0,
       0,     0,   124,   125,   126,   127,   117,   118,     0,     0,
       0,   119,     0,   120,   121,   122,   123,     0,     3,     4,
       0,   699,     0,   128,     0,     0,     0,     0,     0,     0,
       0,   113,   114,   115,   116,     0,     0,     0,     0,     0,
       0,   124,   125,   126,   127,   117,   118,     0,     0,     0,
     119,     0,   120,   121,   122,   123,     0,     3,     4,     0,
       0,     0,   128,     0,     0,     0,     0,     0,     0,     0,
     827,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,     0,     0,     0,     0,     0,     0,
     124,   125,   126,   127,   117,   118,     0,     0,     0,   119,
       0,   120,   121,   122,   123,     0,     3,     4,     0,     0,
       0,   128,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,     0,   124,
     125,   126,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   201,   239,   203,   204,
     128,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,     0,     0,     0,   124,   125,
     126,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   201,     0,   203,   204,   128,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   201,
       0,   203,   204,     0,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   201,     0,   203,   204,     0,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,     0,   201,     0,
     203,   204,   240,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,     0,     0,     0,     0,   113,   114,   115,   116,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     117,   118,     0,     0,     0,   119,     0,   120,   121,   122,
     123,   344,     3,     4,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,     0,
       0,   201,     0,   203,   204,   351,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   201,     0,   203,   204,   910,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,     0,
       0,     0,  -437,     0,   124,   125,   126,   127,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   128,   201,     0,   203,   204,
     199,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
       0,     0,     0,   113,   114,   115,   116,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   117,   118,     0,
       0,     0,   119,   880,   120,   121,   122,   123,     0,     3,
       4,   113,   114,   115,   116,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   117,   118,   945,     0,     0,
     530,     0,   120,   121,   122,   123,     0,     3,     4,   113,
     114,   115,   116,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   117,   118,     0,     0,     0,   818,     0,
     120,   121,   122,   123,     0,     3,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   946,     0,
       0,   124,   125,   126,   127,     0,     0,     0,     0,     0,
     416,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   128,     0,     0,     0,     0,     0,     0,   124,
     125,   126,   127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   417,     0,     0,     0,
     128,     0,     0,     0,     0,     0,     0,   124,   125,   126,
     127,   418,   419,     0,     0,     0,     0,     0,   420,     0,
     421,     0,   318,   422,   423,   424,   425,   426,   128,     0,
     319,   320,   321,   322,   323,   427,   428,     0,     0,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   201,
     202,   203,   204,     0,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,     0,   439,   201,   336,   203,   204,     0,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   201,
     337,   203,   204,     0,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   201,   646,   203,   204,     0,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   201,   785,   203,
     204,     0,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   201,     0,   203,   204,     0,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223
};

static const short int yycheck[] =
{
      20,    69,   391,   391,    98,     1,    84,     7,    46,     1,
     667,     0,    32,   670,   421,   672,    54,    55,    56,     7,
      40,     1,    21,    37,     1,     1,     1,     1,    37,    69,
     305,    50,     7,     7,   840,     7,    55,     1,    22,     1,
      30,   509,     1,   450,    84,    85,   128,    37,    38,    89,
     128,   119,   115,     1,     1,   137,    96,    97,    47,    99,
      98,    37,     1,   818,   852,     7,   854,   892,     1,   109,
     195,    31,   135,    37,     1,     1,     1,     1,    37,   119,
     100,   101,   102,   134,   104,   136,     1,   128,   128,    37,
      37,    38,     1,    88,   900,   135,   137,    30,   138,   224,
     507,   508,   809,   143,    37,   930,   115,   814,   127,   897,
      30,    31,   387,    73,    74,   155,   130,   106,   134,   108,
     136,   111,    37,    38,     7,   165,   110,   111,   128,   327,
     328,   538,   128,     1,   133,   131,   128,    30,   158,   796,
     120,   225,   799,   898,   132,   137,   614,   804,   805,   806,
     807,   128,   128,   129,   130,   195,   128,   132,   132,   135,
     137,   201,   202,   203,   204,   205,   206,   207,   208,    37,
     132,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   260,   130,   224,   225,   226,   227,   137,   128,
     230,     1,   232,   233,   234,   235,     1,   237,   137,   239,
     955,   128,   128,   128,   128,     1,     1,   842,   248,   135,
     137,   231,   137,   137,   254,   130,     1,   137,   238,   128,
     260,    37,    38,   243,   244,   245,   135,    37,   983,   269,
     270,   251,    37,    38,    22,   255,   871,     0,   993,     1,
     995,    37,   282,   283,    39,    40,   241,   950,   446,   289,
     953,   449,    37,    38,   452,   250,    30,   246,   247,   343,
     128,   129,   130,    25,    26,     1,   969,   135,   362,    30,
     354,     7,     1,   293,   294,   295,   296,    39,    40,   363,
       1,   128,    37,    38,   131,    37,   129,   676,   676,    25,
      26,   319,   320,   321,    37,   315,   336,   337,    79,     1,
      81,    30,   145,   113,   114,     1,   346,     1,    37,   152,
      30,     7,    30,   156,   157,     0,    37,   160,     1,    37,
      38,    31,   110,   111,   362,   130,    30,    31,   412,    25,
      26,    30,   175,   128,   848,    37,    30,    22,    23,    30,
     329,   381,   331,    37,   128,   130,   189,   121,   122,   123,
     124,   125,   126,   137,    37,   869,    78,   352,    80,    69,
      70,    46,    47,   137,     1,   879,   128,    52,    53,    54,
      55,    56,   128,   762,   762,   130,   111,   112,   398,   135,
       1,   401,     1,   403,   468,   380,   229,   128,    73,   473,
     131,    30,   128,   111,   422,   423,   424,     1,    37,    38,
      37,   485,    87,    88,   128,   128,     1,   131,   922,    39,
      40,   406,   926,    98,   137,   258,   411,    30,    37,     1,
      33,   106,    30,   108,    37,   110,   111,     4,    30,     1,
       7,   128,   128,    37,    37,   519,    57,   815,   135,   817,
     902,   284,    37,    64,    65,    66,   530,    31,   291,   911,
      71,   471,   128,   967,   916,    37,   918,   919,    30,   135,
      37,    38,   530,   306,    30,    37,   550,    88,   488,   128,
     109,    37,   111,   112,    63,   139,   135,   141,   467,   519,
     500,   128,    71,    72,    73,    74,    75,    30,   135,   133,
     530,   130,   133,   513,    37,    27,    28,    29,   582,    76,
      30,   963,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,   502,   503,   135,   536,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   565,   105,   128,
     107,   140,   109,   142,    30,   555,   135,   128,   558,     1,
     560,   561,     4,   136,   135,   128,   391,   392,   588,   617,
     393,    30,   135,   130,   520,   521,   241,   134,   134,   136,
     137,   246,   247,   133,   648,   250,   113,   114,   113,   114,
     590,   414,   119,   130,   119,    37,    38,   617,    78,    79,
      80,    81,   650,    25,    26,    27,    28,    29,    50,     7,
     669,     5,   671,    55,   673,   635,   122,   123,   124,   125,
     126,    31,   686,    90,    91,    92,   646,   116,   117,   118,
     650,    37,   632,    37,    76,   626,   627,   637,   695,   696,
     660,    31,   642,    30,   664,   645,   474,   475,    90,    91,
      92,    37,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,    37,   105,   329,   107,   331,   109,   505,   506,
     690,    15,    16,    17,    18,   661,   662,   695,   696,   894,
     895,   131,   702,   132,     7,   127,    31,   352,   130,   134,
      37,    30,   134,    57,   136,    37,    37,   362,   133,    37,
      37,   676,    37,   132,   724,   705,    37,    11,    12,    13,
      14,   905,   906,   907,   908,   380,    30,   765,   134,   133,
     133,    25,    26,    56,    37,     7,    30,   128,    32,    33,
      34,    35,   732,    37,    38,   128,   756,   133,   786,    30,
      30,   406,    30,    30,   808,   765,   411,    30,   133,    30,
      30,    30,   772,   773,   774,    23,    24,    25,    26,    27,
      28,    29,    37,   428,    30,   785,   786,    30,   788,    30,
     818,   133,    37,    30,   838,   132,   128,    37,   128,   133,
      58,   801,   128,   803,    30,    10,   129,   762,   611,   612,
     613,    31,   128,    31,   128,    31,   128,    31,   818,   128,
     128,   128,   467,   128,   128,   109,   110,   111,   112,   113,
     114,    60,    10,    37,    62,   863,   128,   135,   838,   819,
     128,   128,   128,   128,    31,   889,   130,   128,   128,   893,
      30,   654,   128,   128,    37,     7,    37,   502,   503,     7,
      37,   135,     7,   863,   128,   909,   132,    26,   386,   358,
     870,   128,   154,   784,   643,   677,   665,   538,   878,   682,
     683,     1,   685,   883,   616,   885,   886,   887,   888,   615,
     639,    11,    12,    13,    14,   724,     1,   700,   898,     4,
     545,   660,   703,   548,   884,    25,    26,   951,   702,   725,
      30,   381,    32,    33,    34,    35,   254,    37,    38,   252,
     565,   171,   253,   726,   924,    30,   872,   730,   755,   743,
     512,   289,    -1,    -1,    -1,   915,    41,    42,    43,    44,
      45,    46,    47,    48,    49,   945,   946,    -1,    -1,    54,
     950,    -1,    -1,   953,    -1,   955,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,   122,   123,     1,    -1,   969,
       4,    -1,    -1,   776,    -1,    -1,    -1,   957,    -1,   959,
     960,   961,   962,   983,    -1,   788,    -1,    -1,    -1,   109,
     110,   111,   112,   993,    -1,   995,    -1,   977,    -1,    -1,
      -1,    -1,    -1,    37,    38,   985,    -1,    -1,   128,   129,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   830,    -1,    -1,
     833,   676,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     843,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     695,   696,    -1,   856,    -1,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
      -1,   105,   106,   107,    -1,   109,    -1,    -1,   881,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   891,    -1,
      -1,    -1,   737,    -1,    -1,    -1,   130,    -1,   743,    -1,
     134,    -1,   136,    -1,    -1,    -1,     1,    -1,    -1,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   762,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   931,    -1,
     933,    -1,    -1,    -1,    -1,    -1,    -1,   940,    -1,   942,
     943,   944,    37,    38,    -1,   948,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,    -1,     4,    -1,    -1,    -1,
      -1,   964,   965,   966,    -1,   968,    -1,   970,    -1,    -1,
      -1,    -1,    -1,   976,    -1,    -1,   979,    -1,   981,   982,
      -1,    76,    -1,    -1,    -1,    -1,   989,    -1,    -1,    37,
      38,    -1,    -1,   996,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,   107,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    76,     4,
      -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,    -1,   105,   106,   107,
       1,   109,    37,    38,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    25,    26,   134,    -1,   136,    30,
      -1,    32,    33,    34,    35,    -1,    37,    38,    -1,    -1,
      -1,    76,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,   107,   108,   109,    -1,    -1,    -1,    37,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,
      -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,    -1,    -1,    63,    64,    65,    66,   109,   110,
     111,   112,    71,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    37,    38,    -1,    -1,    -1,   128,    -1,   130,
      -1,    -1,    -1,     4,   135,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,    -1,   105,    -1,   107,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    37,    38,    -1,    -1,
      -1,   130,    -1,    -1,    -1,   134,    -1,   136,   137,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,    -1,
     105,    -1,   107,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,    -1,   134,
      -1,   136,   137,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,    -1,   105,    -1,   107,     9,   109,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,   130,
      -1,    -1,    -1,   134,     6,   136,     8,     9,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     6,    -1,
       8,     9,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     6,    -1,     8,     9,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    30,    -1,    32,    33,    34,
      35,    -1,    37,    38,    -1,     1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    30,    -1,    32,    33,    34,    35,
     128,    37,    38,    -1,    -1,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    11,    12,    13,    14,    -1,   104,
      -1,   135,    -1,    -1,   109,   110,   111,   112,    25,    26,
      -1,    -1,    -1,    30,    -1,    32,    33,    34,    35,    -1,
      37,    38,    -1,     1,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    12,    13,    14,    -1,   104,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,    25,    26,    -1,
      -1,    -1,    30,    -1,    32,    33,    34,    35,    -1,    37,
      38,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,    25,    26,    -1,    -1,
      -1,    30,    -1,    32,    33,    34,    35,    -1,    37,    38,
      -1,     1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,    25,    26,    -1,    -1,    -1,
      30,    -1,    32,    33,    34,    35,    -1,    37,    38,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,    25,    26,    -1,    -1,    -1,    30,
      -1,    32,    33,    34,    35,    -1,    37,    38,    -1,    -1,
      -1,   130,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,     7,     8,     9,
     130,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     6,    -1,     8,     9,   130,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     6,
      -1,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     6,    -1,     8,     9,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,     6,    -1,
       8,     9,   132,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    30,    -1,    32,    33,    34,
      35,   132,    37,    38,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,     6,    -1,     8,     9,   132,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     6,    -1,     8,     9,   132,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      -1,    -1,   130,    -1,   109,   110,   111,   112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   130,     6,    -1,     8,     9,
     135,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    -1,
      -1,    -1,    30,   128,    32,    33,    34,    35,    -1,    37,
      38,    11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,   128,    -1,    -1,
      30,    -1,    32,    33,    34,    35,    -1,    37,    38,    11,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    30,    -1,
      32,    33,    34,    35,    -1,    37,    38,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
      -1,   109,   110,   111,   112,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,    52,    53,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      61,    -1,    63,    64,    65,    66,    67,    68,   130,    -1,
      71,    72,    73,    74,    75,    76,    77,    -1,    -1,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,   115,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     6,     7,     8,     9,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,     6,     7,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,     6,    -1,     8,     9,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,     4,    37,    38,    50,    55,    76,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   105,   107,
     109,   127,   130,   134,   136,   139,   140,   141,   142,   154,
     242,   244,   266,   273,   276,   283,   284,   286,   287,   323,
     337,   342,   343,   243,   143,   155,   260,   246,   247,   248,
     249,   257,   258,   259,   261,   262,   263,     7,   274,     7,
     277,   137,   285,   349,     1,   287,   323,   324,   337,    30,
     111,   337,    30,   344,     0,   127,   141,    90,    91,    92,
     265,   349,    21,   133,    30,    31,   349,   250,   251,    30,
      37,   115,   338,    37,   286,   242,    30,    30,    30,    30,
     337,   337,   286,   286,   286,    37,   279,    37,   280,    30,
       1,   128,   131,    11,    12,    13,    14,    25,    26,    30,
      32,    33,    34,    35,   109,   110,   111,   112,   130,   327,
     329,   334,   335,   336,   337,   345,   337,   245,   254,   255,
     256,   252,   253,   267,     1,   270,   329,   329,   241,   242,
     349,   241,   329,   144,   156,   133,   329,   329,   266,   286,
     325,   329,   349,   349,   349,   133,   349,   275,     1,   106,
     242,   278,     1,   108,   242,   271,   272,   329,   287,   323,
       1,   287,   323,   334,   334,   334,   334,   334,   334,     1,
     327,   334,   334,   334,   334,    30,   270,   326,   329,   135,
     347,     6,     7,     8,     9,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,   113,   114,   329,   346,
      30,   329,   342,   343,   342,   343,   329,   128,   347,     7,
     132,   347,    30,   147,   147,   329,   347,   347,   349,   133,
     347,   329,   281,   281,   128,   347,   347,   347,   271,   131,
     130,   329,   329,   329,   329,   329,   329,   329,   329,   333,
     332,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   330,   331,   271,   326,   329,   329,   329,   115,
     347,   329,   349,   329,   329,   329,   329,     1,   329,   349,
     329,   241,     1,    37,   129,   130,   148,   149,   150,   152,
     349,   349,   349,   242,   242,   329,   241,   349,    63,    71,
      72,    73,    74,    75,   176,   181,   185,   186,   187,   279,
     282,   280,   272,   349,   347,   270,     7,     7,   329,   329,
     329,   329,   347,     7,   132,   346,   347,   349,   349,   349,
     349,   132,     5,   264,    31,    37,   151,   152,   128,   347,
     145,   157,   349,    31,   215,   216,   215,   215,   215,   180,
     180,   180,   106,   108,   131,   329,   329,   326,   329,     1,
     104,   268,   269,   270,   241,   326,    30,   128,   131,     1,
     149,   146,   158,   266,   326,   177,     1,    37,   206,   207,
      37,   209,    37,   208,   131,   132,     7,   241,     1,    93,
     269,     7,     7,   132,   150,   152,     1,    37,    52,    53,
      59,    61,    64,    65,    66,    67,    68,    76,    77,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,   115,
     153,   169,   171,   176,   181,   182,   183,   189,   190,   191,
     193,   195,   197,   204,   217,   219,   225,   237,   239,   288,
       1,    71,    88,   159,   160,   161,   182,   347,     7,     1,
      37,   178,   179,    31,     1,   128,   349,   128,   349,   128,
     349,   241,   241,   326,   347,   134,   227,   289,    37,    73,
      74,   173,   216,   215,   215,   215,   192,    30,   211,   212,
     205,   337,   238,   240,   180,   188,   180,    69,    70,   194,
     216,   180,   218,    37,    37,    57,   162,   242,   326,   133,
       1,   128,   349,   326,   207,     1,   207,    37,    37,   132,
      30,   326,   226,   290,   291,   349,    37,    30,   210,    78,
      79,    80,    81,   213,   214,   134,   339,   340,   128,   349,
     133,   242,   242,     1,    37,   184,     1,    37,   198,   199,
     209,   209,   216,   216,   339,   196,   220,   221,   340,   349,
     133,   163,    56,     1,   132,   348,   326,   328,   329,   179,
       1,   179,     7,   228,   326,   327,   329,     1,    30,    37,
     230,   231,     1,     4,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    54,   292,   293,   295,   308,   309,   170,
     349,   116,   117,   118,   194,   128,   128,    30,   111,   337,
     337,   326,     1,   128,   349,   133,     1,   128,   349,   349,
     349,     1,   201,   202,   286,    30,    37,   223,   224,   222,
     111,   112,   321,   164,   165,   166,     7,   326,   128,   135,
     128,   135,     1,   129,   232,   233,   234,   235,   236,   329,
      30,     1,   128,   349,    30,   294,    30,    30,    30,    30,
      30,    30,    30,    30,    30,   296,   174,   172,   347,   347,
     347,   339,   214,   213,   327,   341,   133,    37,     1,    37,
     200,    37,   199,     1,   199,     1,   128,   349,   133,     1,
     229,   329,    30,   128,   349,   223,   349,    58,   165,    30,
     121,   122,   123,   124,   125,   126,   167,   349,   329,   132,
     326,   327,    37,   347,   128,   128,   232,   231,     1,   231,
     305,   329,   178,   113,   114,   119,   312,   314,   312,   113,
     114,   119,   313,   316,   313,   312,   313,   312,   313,     1,
      37,   113,   114,   297,   299,   307,   133,   175,   176,   181,
     182,   241,   174,   347,   347,   128,   347,   326,   329,   202,
       1,   202,   203,     1,   128,   347,   229,   224,   349,   122,
     123,   124,   125,   126,   168,     7,   128,   135,    30,     1,
     234,   236,   347,   347,   349,    31,   128,   322,   337,   128,
      31,   128,   322,   128,   128,   128,   128,   128,    31,     1,
      25,    26,    39,    40,   128,   303,   304,   297,    30,   301,
     302,   327,    60,   241,   327,   329,   329,     1,   329,   347,
     168,   329,   327,   329,   347,   315,   312,    10,    31,   312,
     317,   329,    10,   329,   312,   312,   312,   312,   326,   299,
       1,   299,   304,     1,    30,    37,   298,   300,   304,   302,
     327,   349,    62,   128,   347,   135,   347,     1,   120,   318,
     128,    30,    33,    37,   319,   320,   326,   329,   128,   318,
     128,   310,   319,   128,   347,   128,   128,   128,   128,     7,
     348,   298,   298,    31,     1,   128,   347,    30,   128,   327,
     128,   348,   329,   319,   320,    15,    16,    17,    18,     7,
     132,   329,   348,    37,   311,   347,   329,   349,   329,   329,
     329,   329,   326,   347,     7,   303,   326,   300,     1,   300,
     298,   302,   318,   310,   135,   321,   321,   321,   321,   326,
     310,   349,   310,   310,   310,   128,   128,   348,   306,   329,
       7,     7,   348,     7,   303,   128,   347,   347,   132,   347,
     347,   347,   347,   329,   329,   347,   306,   326,   306,     7,
     302,   349,   349,   349,   349,   349,   310,   347,   347,   347,
     348,   347,   306,   128,   347,   347,   349,   347,   347,   347,
     302,   349,   347,   128,   302,   128,   302,   347
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
        case 2:
#line 426 "parse.yy"
    { if (!is_interactive)
			    YYFAIL;
#if VDEBUG != 0
			  if (vdebug)
			    printf_error_V ("interactive statement\n");
#endif
			  interactive_statement = (yyvsp[0].ttype);
			  fix_lastchar();
			  YYACCEPT;}
    break;

  case 4:
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

  case 5:
#line 445 "parse.yy"
    { 
#if VDEBUG != 0
if (vdebug)
  printf_error_V ("EOF\n");
#endif
		 (yyval.ttype) = NULL; }
    break;

  case 7:
#line 456 "parse.yy"
    { yyerrok;
#if VDEBUG != 0
if (vdebug)
  printf ("description_list\n");
#endif
	  	}
    break;

  case 10:
#line 473 "parse.yy"
    { if (is_interactive) {
		    YYFAIL;
                  }
		}
    break;

  case 11:
#line 478 "parse.yy"
    {
		  current_module = current_scope = build_module ((yyvsp[0].ttype));
		  push_scope ();
		  /* list of parse modules */
		  module_list = tree_cons (current_module, NULL_TREE, module_list);
		  /* for now, assume all to be at lop level */
//	       	  top_level = chainon (current_module, top_level);
		}
    break;

  case 12:
#line 487 "parse.yy"
    { MODULE_PORT_LIST (current_module) = nreverse ((yyvsp[-1].ttype)); }
    break;

  case 13:
#line 489 "parse.yy"
    { 
		  current_scope = pop_scope (); 
		  BLOCK_BODY (current_module) = nreverse (BLOCK_BODY (current_module));
		  BLOCK_DECL (current_module) = nreverse (BLOCK_DECL (current_module));
		  end_module (current_module);
		}
    break;

  case 15:
#line 502 "parse.yy"
    { yyerrok; }
    break;

  case 17:
#line 508 "parse.yy"
    { (yyval.ttype) = (yyvsp[-1].ttype); }
    break;

  case 18:
#line 510 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 20:
#line 516 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype));
		}
    break;

  case 23:
#line 530 "parse.yy"
    { if ((yyvsp[0].ttype))
		    (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE);
		  else
		    (yyval.ttype) = NULL_TREE;
		}
    break;

  case 24:
#line 536 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), (yyvsp[-3].ttype)); }
    break;

  case 26:
#line 542 "parse.yy"
    { (yyval.ttype) = (yyvsp[-1].ttype); 
		  sorry ("Port concatenations not supported in this version");
		}
    break;

  case 27:
#line 546 "parse.yy"
    { (yyval.ttype) = NULL_TREE; }
    break;

  case 28:
#line 551 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 29:
#line 553 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); }
    break;

  case 30:
#line 558 "parse.yy"
    { set_decl ((yyvsp[0].ttype), (yyvsp[0].ttype)); }
    break;

  case 31:
#line 560 "parse.yy"
    { (yyval.ttype) = build_bit_ref ((yyvsp[-3].ttype), (yyvsp[-1].ttype));
		  set_decl ((yyvsp[-3].ttype), (yyval.ttype));
//		  sorry ("Port bit-select not supported in this version");
		}
    break;

  case 32:
#line 565 "parse.yy"
    { (yyval.ttype) = build_part_ref ((yyvsp[-5].ttype), (yyvsp[-3].ttype), (yyvsp[-1].ttype));
		  set_decl ((yyvsp[-5].ttype), (yyval.ttype));
//		  sorry ("Port part-selects not supported in this version");
		}
    break;

  case 33:
#line 573 "parse.yy"
    {}
    break;

  case 34:
#line 575 "parse.yy"
    {}
    break;

  case 35:
#line 577 "parse.yy"
    {}
    break;

  case 36:
#line 579 "parse.yy"
    {}
    break;

  case 38:
#line 582 "parse.yy"
    {}
    break;

  case 39:
#line 584 "parse.yy"
    {}
    break;

  case 40:
#line 586 "parse.yy"
    {}
    break;

  case 41:
#line 588 "parse.yy"
    {}
    break;

  case 42:
#line 590 "parse.yy"
    { BLOCK_BODY (current_module) = tree_cons ((yyvsp[0].ttype),
			(tree)INITIAL_CODE, BLOCK_BODY (current_module));
		}
    break;

  case 43:
#line 594 "parse.yy"
    { BLOCK_BODY (current_module) = tree_cons ((yyvsp[0].ttype),
			(tree)ALWAYS_CODE, BLOCK_BODY (current_module));
		}
    break;

  case 44:
#line 598 "parse.yy"
    {}
    break;

  case 45:
#line 600 "parse.yy"
    {}
    break;

  case 46:
#line 615 "parse.yy"
    { if (is_interactive) {
		    YYFAIL;
                  }
		}
    break;

  case 47:
#line 620 "parse.yy"
    {
		  current_module = current_scope = build_udp ((yyvsp[0].ttype));
		  push_scope ();
		  /* list of parse modules */
		  module_list = tree_cons (current_module, NULL_TREE, module_list);
		}
    break;

  case 48:
#line 627 "parse.yy"
    { UDP_PORT_LIST (current_module) = nreverse ((yyvsp[-1].ttype)); }
    break;

  case 49:
#line 629 "parse.yy"
    {
		  current_scope = pop_scope (); 
		  UDP_STRING_LIST (current_module) = nreverse (UDP_STRING_LIST (current_module));
		  end_primitive (current_module);
		}
    break;

  case 50:
#line 637 "parse.yy"
    { (yyval.ttype) = NULL_TREE; }
    break;

  case 51:
#line 639 "parse.yy"
    { yyerrok; }
    break;

  case 55:
#line 650 "parse.yy"
    {
		if( strcmp( IDENT((yyvsp[-1].ttype)), 
			IDENT(DECL_NAME(TREE_PURPOSE(MODULE_PORT_LIST(current_module)))) )!=0 ) {
			error( "only the first declared port in a udp can be a reg",
									NULL_CHAR, NULL_CHAR );		
		} else {	
			UDP_REG_NAME(current_module) = (yyvsp[-1].ttype);
		}
		(yyval.ttype) = NULL_TREE;
	}
    break;

  case 56:
#line 663 "parse.yy"
    { (yyval.ttype) = NULL_TREE; }
    break;

  case 57:
#line 665 "parse.yy"
    {
		if( UDP_REG_NAME(current_module)==NULL_TREE ) {
			error( "initial statement is no allowed in combinatorial udp's",
									NULL_CHAR, NULL_CHAR );		
		} else if( strcmp(IDENT((yyvsp[-3].ttype)),IDENT(UDP_REG_NAME(current_module)))!=0 ) {
			error( "initial statement does not reference port output",
									NULL_CHAR, NULL_CHAR );		
		} else {
			UDP_INITIAL_VALUE(current_module) = (yyvsp[-1].ttype);
		}
		(yyval.ttype) = NULL_TREE;
	}
    break;

  case 58:
#line 683 "parse.yy"
    { 	extern int enable_udp_parse;
		enable_udp_parse = TRUE;
	    current_udp_string = NULL_TREE;
	}
    break;

  case 59:
#line 688 "parse.yy"
    { 
		UDP_STRING_LIST(current_module) = (yyvsp[-1].ttype);
		(yyval.ttype) = NULL_TREE;
	}
    break;

  case 61:
#line 696 "parse.yy"
    { 
		(yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-1].ttype));
	}
    break;

  case 62:
#line 702 "parse.yy"
    { 
		validate_udp_string( current_module, current_udp_string );
		(yyval.ttype) = current_udp_string;
		current_udp_string = NULL_TREE;
	}
    break;

  case 63:
#line 710 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 64:
#line 712 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 65:
#line 716 "parse.yy"
    {
		append_udp_digits( &current_udp_string, (yyvsp[0].c), (yyvsp[0].c) );
		(yyval.ttype) = NULL;
	}
    break;

  case 66:
#line 721 "parse.yy"
    {
		append_udp_digits( &current_udp_string,'?','?' );
		(yyval.ttype) = NULL;
	}
    break;

  case 67:
#line 726 "parse.yy"
    {
		append_udp_digits( &current_udp_string,'0','0' );
		(yyval.ttype) = NULL;
	}
    break;

  case 68:
#line 731 "parse.yy"
    {
		append_udp_digits( &current_udp_string,'1','1' );
		(yyval.ttype) = NULL;
	}
    break;

  case 69:
#line 736 "parse.yy"
    {
		append_udp_digits( &current_udp_string,'b','b' );
		(yyval.ttype) = NULL;
	}
    break;

  case 70:
#line 741 "parse.yy"
    {
		append_udp_digits( &current_udp_string,'x','x' );
		(yyval.ttype) = NULL;
	}
    break;

  case 71:
#line 746 "parse.yy"
    { 		
		/* ? and b must be convert different but equivilent characters
		   otherwise edges cannot be distinguished from levels */

		if( (yyvsp[-2].c)=='?' ) {	
			(yyvsp[-2].c) = '!';
		} else if( (yyvsp[-2].c)=='b' ) {
			(yyvsp[-2].c) = '%';
		}
		append_udp_digits( &current_udp_string,(yyvsp[-2].c), (yyvsp[-1].c) );
		(yyval.ttype) = NULL;
	}
    break;

  case 77:
#line 769 "parse.yy"
    { syn_warning ("Task definition");
		  tmp_tree = build_task (check_task ((yyvsp[-1].ttype)));
		  make_block_decl ((yyvsp[0].ttype), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  BLOCK_DOWN (current_module) = chainon (current_scope, BLOCK_DOWN (current_module));
		  BLOCK_UP (current_scope) = current_module;
		  push_scope ();
		  in_tf = 1;
		}
    break;

  case 78:
#line 779 "parse.yy"
    { BLOCK_BODY (current_scope) = (yyvsp[-1].ttype);
		  in_tf = 0;
		  BLOCK_PORTS (current_scope) = nreverse (BLOCK_PORTS (current_scope));
		  BLOCK_DECL (current_scope) = nreverse (BLOCK_DECL (current_scope));
		  current_scope = pop_scope ();
		}
    break;

  case 79:
#line 789 "parse.yy"
    { current_scope = build_function (check_function ((yyvsp[-1].ttype)));
		  make_block_decl ((yyvsp[-1].ttype), current_module, current_scope);
		  push_scope ();  /* funct name becomes var inside of funct */
		  FUNCT_DECL (current_scope) = make_decl ((yyvsp[-1].ttype), (yyvsp[-2].ttype), NULL_TREE, NULL_TREE);
		  BLOCK_DOWN (current_module) = chainon (current_scope, BLOCK_DOWN (current_module));
		  BLOCK_UP (current_scope) = current_module;
		  in_tf = in_function = 1;
		}
    break;

  case 80:
#line 798 "parse.yy"
    { BLOCK_BODY (current_scope) = (yyvsp[-1].ttype);
		  in_tf = in_function = 0;
		  BLOCK_PORTS (current_scope) = nreverse (BLOCK_PORTS (current_scope));
		  BLOCK_DECL (current_scope) = nreverse (BLOCK_DECL (current_scope));
		  current_scope = pop_scope ();
		}
    break;

  case 81:
#line 808 "parse.yy"
    { (yyval.ttype) = make_reg_spec (NULL_TREE); }
    break;

  case 82:
#line 810 "parse.yy"
    { (yyval.ttype) = make_reg_spec ((yyvsp[0].ttype)); }
    break;

  case 83:
#line 812 "parse.yy"
    { (yyval.ttype) = make_integer_spec (NULL_TREE); }
    break;

  case 84:
#line 814 "parse.yy"
    { (yyval.ttype) = make_real_spec (NULL_TREE); }
    break;

  case 85:
#line 819 "parse.yy"
    {}
    break;

  case 90:
#line 835 "parse.yy"
    { current_spec = make_param_spec ((yyvsp[0].ttype)); }
    break;

  case 91:
#line 837 "parse.yy"
    { BLOCK_DECL (current_scope) =
			chainon ((yyvsp[-1].ttype), BLOCK_DECL (current_scope));
		 }
    break;

  case 93:
#line 845 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype));
		}
    break;

  case 95:
#line 850 "parse.yy"
    { yyerrok; }
    break;

  case 97:
#line 856 "parse.yy"
    { (yyval.ttype) = make_param_decl (check_non_reg ((yyvsp[-2].ttype)), current_spec, (yyvsp[0].ttype));
		}
    break;

  case 98:
#line 859 "parse.yy"
    { (yyval.ttype) = make_param_decl (check_non_reg ((yyvsp[-2].ttype)), current_spec, (yyvsp[0].ttype));
		}
    break;

  case 99:
#line 865 "parse.yy"
    {}
    break;

  case 100:
#line 870 "parse.yy"
    { BLOCK_DECL (current_scope) =
			chainon ((yyvsp[-1].ttype), BLOCK_DECL (current_scope)); }
    break;

  case 101:
#line 873 "parse.yy"
    { BLOCK_DECL (current_scope) =
			chainon ((yyvsp[-1].ttype), BLOCK_DECL (current_scope)); }
    break;

  case 102:
#line 876 "parse.yy"
    { BLOCK_DECL (current_scope) =
			chainon ((yyvsp[-1].ttype), BLOCK_DECL (current_scope)); }
    break;

  case 103:
#line 883 "parse.yy"
    { BLOCK_PORTS (current_scope) =
			chainon ((yyvsp[-1].ttype), BLOCK_PORTS (current_scope)); }
    break;

  case 104:
#line 891 "parse.yy"
    { if (in_tf)
		    (yyval.ttype) = current_spec = make_reg_spec ((yyvsp[0].ttype));
		  else
		    (yyval.ttype) = current_spec = make_net_spec (default_net_type, (yyvsp[0].ttype), NULL_TREE);
		  PORT_INPUT_ATTR ((yyval.ttype)) = 1;
		}
    break;

  case 105:
#line 898 "parse.yy"
    { function_error;
		  if (in_tf)
		    (yyval.ttype) = current_spec = make_reg_spec ((yyvsp[0].ttype));
		  else
		    (yyval.ttype) = current_spec = make_net_spec (default_net_type, (yyvsp[0].ttype), NULL_TREE);
		  PORT_OUTPUT_ATTR ((yyval.ttype)) = 1;
		}
    break;

  case 106:
#line 906 "parse.yy"
    { function_error;
		  if (in_tf)
		    (yyval.ttype) = current_spec = make_reg_spec ((yyvsp[0].ttype));
		  else
		    (yyval.ttype) = current_spec = make_net_spec (default_net_type, (yyvsp[0].ttype), NULL_TREE);
		  PORT_INPUT_ATTR ((yyval.ttype)) = 1;
		  PORT_OUTPUT_ATTR ((yyval.ttype)) = 1;
		}
    break;

  case 107:
#line 918 "parse.yy"
    { (yyval.ttype) = make_decl (check_port ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE); }
    break;

  case 108:
#line 920 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon (make_decl (check_port ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE), (yyvsp[-2].ttype));
		}
    break;

  case 111:
#line 926 "parse.yy"
    { yyerrok; }
    break;

  case 113:
#line 932 "parse.yy"
    { current_spec = make_reg_spec ((yyvsp[0].ttype)); }
    break;

  case 114:
#line 934 "parse.yy"
    { if (!(yyvsp[0].ttype))
		    syn_warning ("Integer Range");
		  current_spec = make_integer_spec ((yyvsp[0].ttype));
		}
    break;

  case 115:
#line 939 "parse.yy"
    { syn_warning ("TIME");
		  current_spec = make_time_spec ((yyvsp[0].ttype));
		}
    break;

  case 116:
#line 946 "parse.yy"
    { current_spec = make_real_spec (NULL_TREE); }
    break;

  case 117:
#line 951 "parse.yy"
    { syn_warning ("EVENT");
		  current_spec = make_event_spec (); }
    break;

  case 118:
#line 957 "parse.yy"
    { lval_type = LVAL_NEW_NET; }
    break;

  case 119:
#line 962 "parse.yy"
    { BLOCK_DECL (current_scope) =
			chainon ((yyvsp[-1].ttype), BLOCK_DECL (current_scope));
 			lval_type = LVAL_REG;
		  current_delay = NULL_TREE;
		}
    break;

  case 121:
#line 969 "parse.yy"
    { lval_type = LVAL_REG; 
		  MODULE_ASSIGNMENTS (current_module) =
			chainon ((yyvsp[-1].ttype), MODULE_ASSIGNMENTS (current_module));
		  current_delay = NULL_TREE;
		}
    break;

  case 122:
#line 978 "parse.yy"
    { (yyval.ttype) = current_spec = make_net_spec ((yyvsp[-2].ntype), (yyvsp[-1].ttype), (yyvsp[0].ttype));
		  current_delay = (yyvsp[0].ttype);
		}
    break;

  case 123:
#line 985 "parse.yy"
    { syn_warning ("TRIREG net");
		  sorry ("TRIREG net type is not supported in this version");
		}
    break;

  case 124:
#line 989 "parse.yy"
    { (yyval.ttype) = error_mark_node; }
    break;

  case 126:
#line 995 "parse.yy"
    { syn_warning ("SUPPLY0 net"); }
    break;

  case 127:
#line 997 "parse.yy"
    { syn_warning ("SUPPLY1 net"); }
    break;

  case 129:
#line 1010 "parse.yy"
    { sorry ("SCALARED keyword not supported; all nets treated as vectored");
		  NET_SCALARED_ATTR ((yyvsp[0].ttype)) = 1;
		  (yyval.ttype) = (yyvsp[0].ttype);
		}
    break;

  case 130:
#line 1015 "parse.yy"
    { sorry ("VECTORED keyword is not supported; all nets treated as vectored");
		  NET_VECTORED_ATTR ((yyvsp[0].ttype)) = 1;
		  (yyval.ttype) = (yyvsp[0].ttype);
		}
    break;

  case 131:
#line 1020 "parse.yy"
    { (yyval.ttype) = NULL_TREE; }
    break;

  case 132:
#line 1025 "parse.yy"
    { lval_type = LVAL_NET; }
    break;

  case 133:
#line 1027 "parse.yy"
    { lval_type = LVAL_REG; 
		  MODULE_ASSIGNMENTS (current_module) =
			chainon ((yyvsp[-1].ttype), MODULE_ASSIGNMENTS (current_module));
		}
    break;

  case 134:
#line 1035 "parse.yy"
    { current_delay = (yyvsp[0].ttype);   /* strength not supported */
		  (yyval.ttype) = NULL;
		}
    break;

  case 135:
#line 1042 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 136:
#line 1044 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype));
		}
    break;

  case 139:
#line 1050 "parse.yy"
    { yyerrok; }
    break;

  case 141:
#line 1055 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 142:
#line 1057 "parse.yy"
    { (yyval.ttype) = build_cont_assign (
				check_lval ((yyvsp[-3].ttype), lval_type, current_spec),
				(yyvsp[0].ttype), (yyvsp[-1].ltype), current_delay, current_delay!=0);
		}
    break;

  case 143:
#line 1066 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 144:
#line 1068 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype));
		}
    break;

  case 147:
#line 1074 "parse.yy"
    { yyerrok; }
    break;

  case 149:
#line 1079 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 150:
#line 1081 "parse.yy"
    { (yyval.ttype) = build_cont_assign ((yyvsp[-3].ttype), (yyvsp[0].ttype), (yyvsp[-1].ltype), current_delay,
			 current_delay!=0); }
    break;

  case 151:
#line 1086 "parse.yy"
    { syn_warning ("DEFPARAM"); }
    break;

  case 152:
#line 1091 "parse.yy"
    { MODULE_DEFPARAMS (current_module) = chainon (
			build_stmt (ASSIGN_STMT, stmt_lineno, (yyvsp[-2].ttype), (yyvsp[0].ttype)),
			MODULE_DEFPARAMS (current_module));
		}
    break;

  case 153:
#line 1096 "parse.yy"
    {
		  MODULE_DEFPARAMS (current_module) = chainon (
			build_stmt (ASSIGN_STMT, stmt_lineno, (yyvsp[-2].ttype), (yyvsp[0].ttype)),
			MODULE_DEFPARAMS (current_module));
		}
    break;

  case 155:
#line 1106 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype));
		}
    break;

  case 158:
#line 1112 "parse.yy"
    { yyerrok; }
    break;

  case 160:
#line 1118 "parse.yy"
    { (yyval.ttype) = make_decl (check_reg ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE); }
    break;

  case 161:
#line 1120 "parse.yy"
    { (yyval.ttype) = make_decl (check_reg ((yyvsp[-5].ttype)), current_spec, (yyvsp[-3].ttype), (yyvsp[-1].ttype));
		  syn_warning ("REG/INTEGER arrays");
		}
    break;

  case 162:
#line 1127 "parse.yy"
    { (yyval.ttype) = make_decl (check_non_reg ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE); }
    break;

  case 163:
#line 1129 "parse.yy"
    { (yyval.ttype) = chainon (make_decl (check_non_reg ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE), (yyvsp[-2].ttype)); }
    break;

  case 164:
#line 1134 "parse.yy"
    { (yyval.ttype) = make_decl (check_net ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE); }
    break;

  case 165:
#line 1136 "parse.yy"
    { (yyval.ttype) = chainon (make_decl (check_net ((yyvsp[0].ttype)), current_spec, NULL_TREE, NULL_TREE), (yyvsp[-2].ttype)); }
    break;

  case 166:
#line 1141 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 167:
#line 1143 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 168:
#line 1145 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 169:
#line 1147 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 170:
#line 1152 "parse.yy"
    {}
    break;

  case 171:
#line 1154 "parse.yy"
    {
warning ("Strength specification is not supported and is ignored", NULL_CHAR, NULL_CHAR);
		}
    break;

  case 172:
#line 1162 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 173:
#line 1164 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 178:
#line 1188 "parse.yy"
    { (yyval.ttype) = NULL_TREE;}
    break;

  case 180:
#line 1194 "parse.yy"
    { TREE_OPERAND (range_handle, 0) = (yyvsp[-3].ttype);
		  TREE_OPERAND (range_handle, 1) = (yyvsp[-1].ttype);
		  (yyval.ttype) = range_handle;
		}
    break;

  case 181:
#line 1210 "parse.yy"
    { current_gatetype = (yyvsp[0].gtype);
		  in_instantiation = 1;
		}
    break;

  case 182:
#line 1214 "parse.yy"
    { in_instantiation = 0; }
    break;

  case 184:
#line 1223 "parse.yy"
    { (yyval.gtype) = GATE_OR_TYPE; }
    break;

  case 185:
#line 1225 "parse.yy"
    { syn_warning ("Buffer gate"); }
    break;

  case 186:
#line 1227 "parse.yy"
    { syn_warning ("Bufif gate"); }
    break;

  case 187:
#line 1229 "parse.yy"
    { syn_warning ("CMOS gate"); }
    break;

  case 188:
#line 1231 "parse.yy"
    { syn_warning ("PULL gate"); }
    break;

  case 189:
#line 1233 "parse.yy"
    { syn_warning ("TRAN gate"); }
    break;

  case 190:
#line 1246 "parse.yy"
    { current_delay = NULL_TREE; }
    break;

  case 192:
#line 1252 "parse.yy"
    { current_delay = (yyvsp[0].ttype); }
    break;

  case 196:
#line 1263 "parse.yy"
    { (yyval.ttype) = build_gate_instance (current_gatetype, input_filename, lineno,
                        (yyvsp[-3].ttype), nreverse ((yyvsp[-1].ttype)), current_delay, NULL_TREE);
                  MODULE_INSTANCES (current_module) = chainon ((yyval.ttype),
                        MODULE_INSTANCES (current_module));
                }
    break;

  case 197:
#line 1269 "parse.yy"
    { (yyval.ttype) = build_gate_instance (current_gatetype, input_filename, lineno,
                        NULL_TREE, nreverse ((yyvsp[-1].ttype)), current_delay, NULL_TREE);
                  MODULE_INSTANCES (current_module) = chainon ((yyval.ttype),
                        MODULE_INSTANCES (current_module));
                }
    break;

  case 198:
#line 1280 "parse.yy"
    { current_instance_module = (yyvsp[-1].ttype);
		  current_instance_param = (yyvsp[0].ttype);
		  in_instantiation = 1;
		}
    break;

  case 199:
#line 1285 "parse.yy"
    { current_instance_module = NULL_TREE;
		  current_instance_param = NULL_TREE;
		  in_instantiation = 0;
		}
    break;

  case 200:
#line 1316 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 201:
#line 1318 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 202:
#line 1320 "parse.yy"
    { (yyval.ttype) = nreverse ((yyvsp[-1].ttype)); }
    break;

  case 203:
#line 1322 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), NULL_TREE); }
    break;

  case 204:
#line 1324 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), NULL_TREE);
		  (yyval.ttype) = tree_cons ((yyvsp[-3].ttype), NULL_TREE, (yyval.ttype));
		}
    break;

  case 205:
#line 1328 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), NULL_TREE);
		  (yyval.ttype) = tree_cons ((yyvsp[-3].ttype), NULL_TREE, (yyval.ttype));
		  (yyval.ttype) = tree_cons ((yyvsp[-5].ttype), NULL_TREE, (yyval.ttype));
		}
    break;

  case 206:
#line 1336 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 207:
#line 1338 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); }
    break;

  case 208:
#line 1365 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 209:
#line 1367 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); yyerrok; }
    break;

  case 212:
#line 1372 "parse.yy"
    { yyerrok; }
    break;

  case 215:
#line 1385 "parse.yy"
    { yyerrok; }
    break;

  case 218:
#line 1389 "parse.yy"
    { yyerrok; }
    break;

  case 220:
#line 1395 "parse.yy"
    { (yyval.ttype) = build_stmt (INSTANCE_NODE, lineno, check_non_reg ((yyvsp[-3].ttype)),
				current_instance_module,
				nreverse ((yyvsp[-1].ttype)), current_instance_param);
		  MODULE_INSTANCES (current_module) = chainon ((yyval.ttype),
				MODULE_INSTANCES (current_module));
		//  make_block_decl ($1, current_module, NULL_TREE);
		}
    break;

  case 221:
#line 1403 "parse.yy"
    { (yyval.ttype) = build_stmt (INSTANCE_NODE, lineno, NULL_TREE,
				current_instance_module,
				nreverse ((yyvsp[-1].ttype)), current_instance_param);
		  MODULE_INSTANCES (current_module) = chainon ((yyval.ttype),
				MODULE_INSTANCES (current_module));
		}
    break;

  case 225:
#line 1419 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype));
		}
    break;

  case 228:
#line 1428 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 229:
#line 1430 "parse.yy"
    { (yyval.ttype) = build_tree_list (NULL_TREE, NULL_TREE); }
    break;

  case 231:
#line 1436 "parse.yy"
    { (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype)); }
    break;

  case 232:
#line 1441 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[-1].ttype), (yyvsp[-3].ttype)); }
    break;

  case 233:
#line 1443 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 234:
#line 1451 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("INITIAL"); }
    break;

  case 235:
#line 1454 "parse.yy"
    { (yyval.ttype) = build_stmt (INITIAL_BLOCK, (yyvsp[-1].ltype), (yyvsp[0].ttype)); }
    break;

  case 236:
#line 1458 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 237:
#line 1460 "parse.yy"
    { (yyval.ttype) = build_stmt (ALWAYS_BLOCK, (yyvsp[-1].ltype), (yyvsp[0].ttype)); }
    break;

  case 239:
#line 1466 "parse.yy"
    { (yyval.ttype) = build_stmt (NULL_STMT, lineno); }
    break;

  case 241:
#line 1473 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 242:
#line 1475 "parse.yy"
    { (yyval.ttype) = build_stmt (IF_STMT, (yyvsp[-5].ltype), (yyvsp[-3].ttype), (yyvsp[-1].ttype), (yyvsp[0].ttype)); }
    break;

  case 243:
#line 1477 "parse.yy"
    { (yyval.ttype) = current_case; }
    break;

  case 244:
#line 1479 "parse.yy"
    { current_case = (yyvsp[0].ttype); }
    break;

  case 245:
#line 1481 "parse.yy"
    { STMT_CASE_EXPR ((yyvsp[-6].ttype)) = (yyvsp[-3].ttype);
		  STMT_CASE_LIST ((yyvsp[-6].ttype)) = nreverse ((yyvsp[-1].ttype));
		  current_case = (yyvsp[-7].ttype);
		  (yyval.ttype) = (yyvsp[-6].ttype);
		}
    break;

  case 246:
#line 1488 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("FOREVER"); }
    break;

  case 247:
#line 1491 "parse.yy"
    { (yyval.ttype) = build_stmt (FOREVER_STMT, (yyvsp[-1].ltype), (yyvsp[0].ttype)); }
    break;

  case 248:
#line 1494 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("REPEAT"); }
    break;

  case 249:
#line 1497 "parse.yy"
    { (yyval.ttype) = build_stmt (REPEAT_INIT_STMT, (yyvsp[-4].ltype), (yyvsp[0].ttype), (yyvsp[-2].ttype));
		  tmp_tree = build_stmt (REPEAT_STMT, (yyvsp[-4].ltype), (yyval.ttype), (yyvsp[-2].ttype));
		  chainon ((yyvsp[0].ttype), tmp_tree);
		}
    break;

  case 250:
#line 1503 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("WHILE"); }
    break;

  case 251:
#line 1506 "parse.yy"
    { (yyval.ttype) = build_stmt (WHILE_STMT, (yyvsp[-4].ltype), (yyvsp[0].ttype), (yyvsp[-2].ttype)); }
    break;

  case 252:
#line 1508 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 253:
#line 1510 "parse.yy"
    { stmt_lineno = (yyvsp[-8].ltype);
		  tmp_tree = build_stmt (FOR_STMT, (yyvsp[-8].ltype), (yyvsp[0].ttype), (yyvsp[-2].ttype), (yyvsp[-4].ttype));
		  STMT_SURROGATE_ATTR (tmp_tree) = 1;
		  (yyval.ttype) = build_stmt (FOR_STMT, (yyvsp[-8].ltype), (yyvsp[0].ttype), (yyvsp[-6].ttype), (yyvsp[-4].ttype));
		  TREE_CHAIN (tmp_tree) = (yyval.ttype);	/* Fix this in pass 3 */
		  chainon ((yyvsp[0].ttype), tmp_tree);	/* at end of statement sequence */
		}
    break;

  case 254:
#line 1519 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("delay control");
		  function_error;
		}
    break;

  case 255:
#line 1524 "parse.yy"
    { (yyval.ttype) = build_stmt (DELAY_STMT, (yyvsp[-1].ltype), (yyvsp[0].ttype), (yyvsp[-2].ttype)); }
    break;

  case 256:
#line 1527 "parse.yy"
    { stmt_lineno = lineno;
		  function_error;
		}
    break;

  case 257:
#line 1531 "parse.yy"
    { STMT_BODY ((yyvsp[-2].ttype)) = (yyvsp[0].ttype);
		  (yyval.ttype) = (yyvsp[-2].ttype);
		}
    break;

  case 258:
#line 1536 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  function_error;
		}
    break;

  case 259:
#line 1540 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_DELAY_STMT, (yyvsp[-3].ltype), (yyvsp[-5].ttype),
			implicit_conversion ((yyvsp[-5].ttype), (yyvsp[-1].ttype)),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, (yyvsp[-3].ltype), (yyvsp[-5].ttype), (yyvsp[-2].ttype)));
		}
    break;

  case 260:
#line 1547 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  function_error;
		}
    break;

  case 261:
#line 1551 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_EVENT_STMT, (yyvsp[-3].ltype), (yyvsp[-5].ttype),
			implicit_conversion ((yyvsp[-5].ttype), (yyvsp[-1].ttype)),
			NULL_TREE, NULL_TREE, (yyvsp[-2].ttype));
		}
    break;

  case 262:
#line 1557 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  function_error;
		}
    break;

  case 263:
#line 1561 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_NONBLK_DELAY_STMT, (yyvsp[-2].ltype), (yyvsp[-4].ttype),
			implicit_conversion ((yyvsp[-4].ttype), (yyvsp[-1].ttype)),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, (yyvsp[-2].ltype), (yyvsp[-4].ttype), integer_zero_node));
		}
    break;

  case 264:
#line 1568 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  function_error;
		}
    break;

  case 265:
#line 1572 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_NONBLK_DELAY_STMT, (yyvsp[-3].ltype), (yyvsp[-5].ttype),
			implicit_conversion ((yyvsp[-5].ttype), (yyvsp[-1].ttype)),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, (yyvsp[-3].ltype), (yyvsp[-5].ttype), (yyvsp[-2].ttype)));
		}
    break;

  case 266:
#line 1579 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  function_error;
		}
    break;

  case 267:
#line 1583 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_NONBLK_EVENT_STMT, (yyvsp[-3].ltype), (yyvsp[-5].ttype),
			implicit_conversion ((yyvsp[-5].ttype), (yyvsp[-1].ttype)),
			NULL_TREE, NULL_TREE, (yyvsp[-2].ttype));
		}
    break;

  case 268:
#line 1595 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("WAIT");
		  function_error;
		}
    break;

  case 269:
#line 1600 "parse.yy"
    { (yyval.ttype) = build_stmt (WAIT_STMT, (yyvsp[-4].ltype), (yyvsp[0].ttype), (yyvsp[-2].ttype)); }
    break;

  case 270:
#line 1603 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
		  syn_warning ("Event trigger");
		  function_error;
		}
    break;

  case 271:
#line 1608 "parse.yy"
    { (yyval.ttype)= build_stmt (ARROW_STMT, (yyvsp[-2].ltype), check_named_event ((yyvsp[-1].ttype))); }
    break;

  case 276:
#line 1614 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 277:
#line 1616 "parse.yy"
    { (yyval.ttype) = build_stmt (DISABLE_STMT, (yyvsp[-2].ltype), (yyvsp[-1].ttype));
		  HIERARCHICAL_ATTR ((yyval.ttype)) = HIERARCHICAL_ATTR ((yyvsp[-1].ttype));
		}
    break;

  case 278:
#line 1621 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 279:
#line 1623 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_PROC_STMT, (yyvsp[-4].ltype), (yyvsp[-3].ttype), (yyvsp[-1].ttype), NULL_TREE,
			NULL_TREE); }
    break;

  case 280:
#line 1627 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 281:
#line 1629 "parse.yy"
    { (yyval.ttype) = build_stmt (DEASSIGN_STMT, (yyvsp[-2].ltype), (yyvsp[-1].ttype)); }
    break;

  case 282:
#line 1631 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
				  lval_type = LVAL_REG_NET; }
    break;

  case 283:
#line 1634 "parse.yy"
    { syn_warning ("FORCE");
		  (yyval.ttype) = build_stmt (FORCE_STMT, (yyvsp[-4].ltype), (yyvsp[-3].ttype), (yyvsp[-1].ttype), NULL_TREE,
			NULL_TREE, NULL_TREE, NULL_TREE);
		  lval_type = LVAL_REG;
		}
    break;

  case 284:
#line 1640 "parse.yy"
    { (yyval.ltype) = stmt_lineno;
				  lval_type = LVAL_REG_NET; }
    break;

  case 285:
#line 1643 "parse.yy"
    { syn_warning ("RELEASE");
		  (yyval.ttype) = build_stmt (RELEASE_STMT, (yyvsp[-2].ltype), (yyvsp[-1].ttype));
		  lval_type = LVAL_REG;
		}
    break;

  case 286:
#line 1653 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 287:
#line 1655 "parse.yy"
    { (yyval.ttype) = (yyvsp[0].ttype); }
    break;

  case 288:
#line 1660 "parse.yy"
    { (yyval.ttype) = build_stmt (CASE_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
    break;

  case 289:
#line 1662 "parse.yy"
    { (yyval.ttype) = build_stmt (CASEZ_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
    break;

  case 290:
#line 1664 "parse.yy"
    { (yyval.ttype) = build_stmt (CASEX_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
    break;

  case 291:
#line 1675 "parse.yy"
    { (yyval.ltype) = stmt_lineno; }
    break;

  case 292:
#line 1677 "parse.yy"
    { (yyval.ttype) = build_stmt (ASSIGN_STMT, (yyvsp[-1].ltype), (yyvsp[-3].ttype),
				implicit_conversion ((yyvsp[-3].ttype), (yyvsp[0].ttype))); }
    break;

  case 294:
#line 1684 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-1].ttype));
		}
    break;

  case 297:
#line 1693 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 298:
#line 1696 "parse.yy"
    { if (STMT_CASE_DEFAULT (current_case))
		    error ("More than one 'DEFAULT' within CASE", NULL_CHAR, NULL_CHAR);
		  STMT_CASE_DEFAULT (current_case) = (yyvsp[0].ttype);
		}
    break;

  case 299:
#line 1701 "parse.yy"
    { if (STMT_CASE_DEFAULT (current_case))
		    error ("More than one 'DEFAULT' within CASE", NULL_CHAR, NULL_CHAR);
		  STMT_CASE_DEFAULT (current_case) = (yyvsp[0].ttype);
		}
    break;

  case 300:
#line 1709 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 301:
#line 1711 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); }
    break;

  case 304:
#line 1718 "parse.yy"
    { if( (yyvsp[0].ttype) != NULL_TREE ) {
		    (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); 
		  } else {
		    (yyval.ttype) = (yyvsp[0].ttype);
		  }
		}
    break;

  case 305:
#line 1725 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); }
    break;

  case 306:
#line 1734 "parse.yy"
    { (yyval.ttype) = NULL_TREE; }
    break;

  case 308:
#line 1740 "parse.yy"
    { (yyval.ttype) = build_stmt (BEGIN_STMT, stmt_lineno); }
    break;

  case 309:
#line 1742 "parse.yy"
    { STMT_BODY ((yyvsp[-2].ttype)) = nreverse
			(chainon (build_stmt (END_STMT, stmt_lineno), (yyvsp[-1].ttype)));
		  (yyval.ttype) = (yyvsp[-2].ttype);
		}
    break;

  case 310:
#line 1747 "parse.yy"
    { tmp_tree = make_node (NAMED_BLOCK);
		  BLOCK_DOWN (current_scope) = chainon (tmp_tree, BLOCK_DOWN (current_scope));
		  BLOCK_UP (tmp_tree) = current_scope;
		  BLOCK_NAME (tmp_tree) = (yyvsp[0].ttype);
		  BLOCK_BODY (tmp_tree) = NULL_TREE;
                  BLOCK_CONTEXT_LIST (tmp_tree) = NULL;
		  make_block_decl (check_block ((yyvsp[0].ttype)), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  push_scope ();
		  (yyval.ttype) = build_stmt (BEGIN_NAMED_STMT, stmt_lineno,
			NULL_TREE, tmp_tree);
		}
    break;

  case 311:
#line 1760 "parse.yy"
    { tmp_tree = build_stmt (END_NAMED_STMT, stmt_lineno,
			NULL_TREE, STMT_BLOCK ((yyvsp[-3].ttype)));
		  /* STMT_BEGIN_NAMED_END ($<ttype>4) = tmp_tree; */
		  STMT_BODY ((yyvsp[-3].ttype)) = nreverse (chainon (tmp_tree, (yyvsp[-1].ttype)));
		  current_scope = pop_scope ();
		  (yyval.ttype) = (yyvsp[-3].ttype);
		}
    break;

  case 312:
#line 1771 "parse.yy"
    { (yyval.ttype) = build_stmt (FORK_STMT, stmt_lineno); }
    break;

  case 313:
#line 1773 "parse.yy"
    { STMT_BODY ((yyvsp[-2].ttype)) = chainon  /* thread JOIN to BEGINNING */
			(build_stmt (JOIN_STMT, stmt_lineno, NULL_TREE),
			nreverse ((yyvsp[-1].ttype)));
		  (yyval.ttype) = (yyvsp[-2].ttype);
		}
    break;

  case 314:
#line 1779 "parse.yy"
    { tmp_tree = make_node (NAMED_BLOCK);
		  BLOCK_DOWN (current_scope) = chainon (tmp_tree, BLOCK_DOWN (current_scope));
		  BLOCK_UP (tmp_tree) = current_scope;
		  BLOCK_NAME (tmp_tree) = (yyvsp[0].ttype);
		  BLOCK_BODY (tmp_tree) = NULL_TREE;
		  make_block_decl (check_block ((yyvsp[0].ttype)), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  push_scope ();
		  (yyval.ttype) = build_stmt (FORK_NAMED_STMT, stmt_lineno,
			NULL_TREE, tmp_tree);
		}
    break;

  case 315:
#line 1791 "parse.yy"
    { tmp_tree = build_stmt (JOIN_NAMED_STMT, stmt_lineno,
			NULL_TREE, STMT_BLOCK ((yyvsp[-3].ttype)));
		  /* STMT_BEGIN_NAMED_END ($<ttype>4) = tmp_tree; */
		  STMT_BODY ((yyvsp[-3].ttype)) = chainon (tmp_tree, nreverse ((yyvsp[-1].ttype)));
		  current_scope = pop_scope ();
		  (yyval.ttype) = (yyvsp[-3].ttype);
		}
    break;

  case 316:
#line 1802 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 317:
#line 1804 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-1].ttype));
		}
    break;

  case 319:
#line 1812 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 320:
#line 1814 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-1].ttype));
		}
    break;

  case 322:
#line 1822 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 326:
#line 1833 "parse.yy"
    { syn_warning ("task enable");
		  function_error;
		  (yyval.ttype) = build_stmt (TASK_STMT, lineno, IDENTIFIER_POINTER ((yyvsp[-1].ttype)), NULL_TREE);
		  HIERARCHICAL_ATTR ((yyval.ttype)) = HIERARCHICAL_ATTR ((yyvsp[-1].ttype));
		}
    break;

  case 327:
#line 1839 "parse.yy"
    { syn_warning ("task enable");
		  (yyval.ttype) = build_stmt (TASK_STMT, lineno, IDENTIFIER_POINTER ((yyvsp[-4].ttype)), nreverse ((yyvsp[-2].ttype)));
		  HIERARCHICAL_ATTR ((yyval.ttype)) = HIERARCHICAL_ATTR ((yyvsp[-4].ttype));
		}
    break;

  case 328:
#line 1847 "parse.yy"
    { (yyval.ttype) = build_stmt (SYSTASK_STMT, lineno, IDENTIFIER_POINTER ((yyvsp[-1].ttype)), NULL_TREE); }
    break;

  case 329:
#line 1849 "parse.yy"
    { in_systask = 1; }
    break;

  case 330:
#line 1851 "parse.yy"
    { (yyval.ttype) = build_stmt (SYSTASK_STMT, lineno, IDENTIFIER_POINTER ((yyvsp[-5].ttype)), nreverse ((yyvsp[-2].ttype)));
		  in_systask = 0;
		}
    break;

  case 331:
#line 1858 "parse.yy"
    { stmt_lineno = lineno; }
    break;

  case 332:
#line 1860 "parse.yy"
    { stmt_lineno = lineno; }
    break;

  case 333:
#line 1865 "parse.yy"
    { (yyval.ttype) = check_lval ((yyvsp[0].ttype), lval_type, current_spec); }
    break;

  case 334:
#line 1867 "parse.yy"
    { (yyval.ttype) = build_bit_ref (
			check_lval_nocheck ((yyvsp[-3].ttype), lval_type, current_spec), (yyvsp[-1].ttype)); }
    break;

  case 335:
#line 1870 "parse.yy"
    { (yyval.ttype) = build_part_ref (
			check_lval_nocheck ((yyvsp[-5].ttype), lval_type, current_spec), (yyvsp[-3].ttype), (yyvsp[-1].ttype)); }
    break;

  case 336:
#line 1880 "parse.yy"
    {
	}
    break;

  case 337:
#line 1883 "parse.yy"
    {}
    break;

  case 338:
#line 1889 "parse.yy"
    {}
    break;

  case 339:
#line 1894 "parse.yy"
    { (yyval.ttype) = NULL; }
    break;

  case 340:
#line 1896 "parse.yy"
    { yyerrok;
		    (yyval.ttype) = NULL;	
		//  $$ = chainon ($2, $1);
		}
    break;

  case 344:
#line 1906 "parse.yy"
    {
	    if( (yyvsp[0].ttype)!=NULL_TREE ) { 
		STMT_SOURCE_LINE((yyvsp[0].ttype)) = stmt_lineno;
		STMT_SOURCE_FILE((yyvsp[0].ttype)) = input_filename;
		MODULE_SPECDEFS(current_module) = 
			chainon(MODULE_SPECDEFS(current_module),(yyvsp[0].ttype));
	    }
	}
    break;

  case 345:
#line 1924 "parse.yy"
    { current_spec = make_param_spec (NULL_TREE); }
    break;

  case 346:
#line 1926 "parse.yy"
    {
		specify_save_param_list( current_module, (yyvsp[-1].ttype) );
	 }
    break;

  case 347:
#line 1933 "parse.yy"
    {
		PATH_CONDITION((yyvsp[-3].ttype)) = (yyvsp[-4].ttype);
	 	PATH_DELAYS((yyvsp[-3].ttype)) = (yyvsp[-1].ttype);	
		MODULE_SPECDEFS(current_module) = 
			chainon(MODULE_SPECDEFS(current_module),check_path((yyvsp[-3].ttype)));
	}
    break;

  case 348:
#line 1947 "parse.yy"
    {
		(yyval.ttype) = build_path( nreverse((yyvsp[-7].ttype)), nreverse((yyvsp[-4].ttype)), (yyvsp[-2].ttype), NULL_TREE, NULL_TREE,
			(yyvsp[-6].integer), 0, 0 );
	}
    break;

  case 349:
#line 1955 "parse.yy"
    {
		(yyval.ttype) = build_path( nreverse((yyvsp[-7].ttype)), nreverse((yyvsp[-4].ttype)), (yyvsp[-2].ttype), NULL_TREE, NULL_TREE,
			(yyvsp[-6].integer), (yyvsp[-8].integer), 0 );
	}
    break;

  case 350:
#line 1963 "parse.yy"
    {
		(yyval.ttype) = build_path( nreverse((yyvsp[-8].ttype)), nreverse((yyvsp[-5].ttype)), (yyvsp[-2].ttype), NULL_TREE, NULL_TREE,
			(yyvsp[-7].integer), 0, (yyvsp[-4].integer) );
	}
    break;

  case 351:
#line 1971 "parse.yy"
    {
		(yyval.ttype) = build_path( nreverse((yyvsp[-8].ttype)), nreverse((yyvsp[-5].ttype)), (yyvsp[-2].ttype), NULL_TREE, NULL_TREE,
			(yyvsp[-7].integer), (yyvsp[-9].integer), (yyvsp[-4].integer) );
	}
    break;

  case 352:
#line 1977 "parse.yy"
    {
		(yyval.ttype) = build_path( nreverse((yyvsp[-3].ttype)), nreverse((yyvsp[-1].ttype)), NULL_TREE, NULL_TREE, NULL_TREE,
			(yyvsp[-2].integer), 0, 0 );
	}
    break;

  case 353:
#line 1983 "parse.yy"
    {
		(yyval.ttype) = build_path( nreverse((yyvsp[-4].ttype)), nreverse((yyvsp[-1].ttype)), NULL_TREE, NULL_TREE, NULL_TREE,
			(yyvsp[-2].integer), 0, (yyvsp[-3].integer) );
	}
    break;

  case 355:
#line 1991 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype));
		}
    break;

  case 357:
#line 1996 "parse.yy"
    { yyerrok; }
    break;

  case 360:
#line 2002 "parse.yy"
    { yyerrok;
		  (yyval.ttype) = chainon ((yyvsp[0].ttype), (yyvsp[-2].ttype));
		}
    break;

  case 362:
#line 2007 "parse.yy"
    { yyerrok; }
    break;

  case 364:
#line 2012 "parse.yy"
    {
		(yyval.ttype) = build_tree_list( check_input_port((yyvsp[0].ttype)), NULL_TREE );
	}
    break;

  case 365:
#line 2016 "parse.yy"
    {
		(yyval.ttype) = build_tree_list( build_bit_ref (check_input_port((yyvsp[-3].ttype)), (yyvsp[-1].ttype)), NULL_TREE ); 
	}
    break;

  case 366:
#line 2020 "parse.yy"
    {
		(yyval.ttype) = build_tree_list( build_part_ref (check_input_port((yyvsp[-5].ttype)), (yyvsp[-3].ttype), (yyvsp[-1].ttype)), NULL_TREE ); 
	}
    break;

  case 367:
#line 2026 "parse.yy"
    {
		(yyval.ttype) = build_tree_list( check_output_port((yyvsp[0].ttype)), NULL_TREE );
	}
    break;

  case 368:
#line 2030 "parse.yy"
    {
		(yyval.ttype) = build_tree_list( build_bit_ref (check_output_port((yyvsp[-3].ttype)), (yyvsp[-1].ttype)), NULL_TREE ); 
	}
    break;

  case 369:
#line 2034 "parse.yy"
    {
		(yyval.ttype) = build_tree_list( build_part_ref (check_output_port((yyvsp[-5].ttype)), (yyvsp[-3].ttype), (yyvsp[-1].ttype)), NULL_TREE ); 
	}
    break;

  case 371:
#line 2041 "parse.yy"
    {
		(yyval.ttype) = chainon( (yyvsp[-3].ttype), (yyvsp[-1].ttype) );
	}
    break;

  case 372:
#line 2046 "parse.yy"
    {
		tree t;
		t = chainon( (yyvsp[-5].ttype), (yyvsp[-3].ttype) );
		(yyval.ttype) = chainon( t, (yyvsp[-1].ttype) );
	}
    break;

  case 373:
#line 2054 "parse.yy"
    {
		tree t;
		t = chainon( (yyvsp[-11].ttype), (yyvsp[-9].ttype) );
		t = chainon( t, (yyvsp[-7].ttype) );
		t = chainon( t, (yyvsp[-5].ttype) );
		t = chainon( t, (yyvsp[-3].ttype) );
		(yyval.ttype) = chainon( t, (yyvsp[-1].ttype) );
	}
    break;

  case 374:
#line 2065 "parse.yy"
    {
		(yyval.ttype) = build_tree_list((yyvsp[0].ttype),NULL_TREE);
	}
    break;

  case 375:
#line 2071 "parse.yy"
    {	
		(yyval.integer) = 1;
	}
    break;

  case 376:
#line 2075 "parse.yy"
    {	
		(yyval.integer) = -1;
	}
    break;

  case 377:
#line 2081 "parse.yy"
    {
		(yyval.integer) = FALSE;
	}
    break;

  case 378:
#line 2085 "parse.yy"
    {
		(yyval.integer) = TRUE;
	}
    break;

  case 381:
#line 2099 "parse.yy"
    {
		(yyval.integer) = 1;
	}
    break;

  case 382:
#line 2103 "parse.yy"
    {
		(yyval.integer) = -1;
	}
    break;

  case 383:
#line 2109 "parse.yy"
    {
		(yyval.ttype) = 0;
	}
    break;

  case 384:
#line 2113 "parse.yy"
    {
		(yyval.ttype) = (yyvsp[-1].ttype);
	}
    break;

  case 385:
#line 2121 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( SETUP, (yyvsp[-7].ttype), (yyvsp[-5].ttype), (yyvsp[-3].ttype), NULL_TREE, (yyvsp[-2].ttype) );
	}
    break;

  case 386:
#line 2126 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( HOLD, (yyvsp[-7].ttype), (yyvsp[-5].ttype), (yyvsp[-3].ttype), NULL_TREE, (yyvsp[-2].ttype) );
	}
    break;

  case 387:
#line 2131 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( PERIOD, (yyvsp[-5].ttype), NULL_TREE, (yyvsp[-3].ttype), NULL_TREE, (yyvsp[-2].ttype) );
	}
    break;

  case 388:
#line 2136 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( WIDTH, (yyvsp[-4].ttype), NULL_TREE, (yyvsp[-2].ttype), NULL_TREE, NULL_TREE );
	}
    break;

  case 389:
#line 2141 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( WIDTH, (yyvsp[-7].ttype), NULL_TREE, (yyvsp[-5].ttype), (yyvsp[-3].ttype), (yyvsp[-2].ttype) );
	}
    break;

  case 390:
#line 2146 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( SKEW, (yyvsp[-7].ttype), (yyvsp[-5].ttype), (yyvsp[-3].ttype), NULL_TREE, (yyvsp[-2].ttype) );
	}
    break;

  case 391:
#line 2151 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( RECOVERY, (yyvsp[-7].ttype), (yyvsp[-5].ttype), (yyvsp[-3].ttype), NULL_TREE, (yyvsp[-2].ttype) );
	}
    break;

  case 392:
#line 2156 "parse.yy"
    {
		(yyval.ttype) = build_timing_statement( SETUPHOLD, (yyvsp[-9].ttype), (yyvsp[-7].ttype), (yyvsp[-5].ttype), (yyvsp[-3].ttype), (yyvsp[-2].ttype) );
	}
    break;

  case 393:
#line 2161 "parse.yy"
    {
		(yyval.ttype) = NULL_TREE;
	}
    break;

  case 394:
#line 2167 "parse.yy"
    {
		(yyval.ttype) = 0;
	}
    break;

  case 395:
#line 2171 "parse.yy"
    {
		(yyval.ttype) = (yyvsp[0].ttype);
	}
    break;

  case 397:
#line 2180 "parse.yy"
    { 
		(yyval.ttype) = build_timing_event( (yyvsp[-1].integer), (yyvsp[0].ttype), NULL_TREE );
	}
    break;

  case 398:
#line 2185 "parse.yy"
    { 
		(yyval.ttype) = build_timing_event( (yyvsp[-3].integer), (yyvsp[-2].ttype), (yyvsp[0].ttype) );
	}
    break;

  case 399:
#line 2191 "parse.yy"
    {
		(yyval.ttype) = build_timing_event( (yyvsp[-1].integer), (yyvsp[0].ttype), NULL_TREE );
	}
    break;

  case 400:
#line 2196 "parse.yy"
    {
		(yyval.ttype) = build_timing_event( (yyvsp[-3].integer), (yyvsp[-2].ttype), (yyvsp[0].ttype) );
	}
    break;

  case 401:
#line 2202 "parse.yy"
    {
		extern unsigned int alledgeMask;
		(yyval.integer) = alledgeMask;
	}
    break;

  case 402:
#line 2207 "parse.yy"
    {
		extern unsigned int posedgeMask;
		(yyval.integer) = posedgeMask;
	}
    break;

  case 403:
#line 2212 "parse.yy"
    {
		extern unsigned int negedgeMask;
		(yyval.integer) = negedgeMask;
	}
    break;

  case 404:
#line 2217 "parse.yy"
    {  	extern int enable_edge_digit;
		enable_edge_digit = TRUE; 
	}
    break;

  case 405:
#line 2221 "parse.yy"
    { 
		extern int enable_edge_digit;
		enable_edge_digit = FALSE;
		(yyval.integer) = (yyvsp[-1].integer);
	}
    break;

  case 406:
#line 2229 "parse.yy"
    {
		extern unsigned int posedgeMask;
		(yyval.integer) = posedgeMask;
	}
    break;

  case 407:
#line 2234 "parse.yy"
    {
		extern unsigned int negedgeMask;
		(yyval.integer) = negedgeMask;
	}
    break;

  case 408:
#line 2239 "parse.yy"
    {  	extern int enable_edge_digit;
		enable_edge_digit = TRUE; 
	}
    break;

  case 409:
#line 2243 "parse.yy"
    { 
		extern int enable_edge_digit;
		enable_edge_digit = FALSE;
		(yyval.integer) = (yyvsp[-1].integer);
	}
    break;

  case 410:
#line 2251 "parse.yy"
    {
		extern unsigned int edge_mask[4][4];
		(yyval.integer) = edge_mask[(yyvsp[0].double_digit)[0]][(yyvsp[0].double_digit)[1]];
	}
    break;

  case 411:
#line 2256 "parse.yy"
    {
		extern unsigned int edge_mask[4][4];
		(yyval.integer) = (yyvsp[0].integer) | edge_mask[(yyvsp[-2].double_digit)[0]][(yyvsp[-2].double_digit)[1]];
	}
    break;

  case 412:
#line 2261 "parse.yy"
    {
		(yyval.integer) = 0;
	}
    break;

  case 414:
#line 2268 "parse.yy"
    { (yyval.ttype) = (yyvsp[-1].ttype); }
    break;

  case 415:
#line 2270 "parse.yy"
    { (yyval.ttype) = build_unary_op (BIT_NOT_EXPR, (yyvsp[0].ttype)); }
    break;

  case 416:
#line 2272 "parse.yy"
    { (yyval.ttype) = build_binary_op (EQ_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 417:
#line 2274 "parse.yy"
    { (yyval.ttype) = build_binary_op (EQ_CASE_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 418:
#line 2276 "parse.yy"
    { (yyval.ttype) = build_binary_op (NE_CASE_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 419:
#line 2278 "parse.yy"
    { (yyval.ttype) = build_binary_op (NE_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 420:
#line 2282 "parse.yy"
    { (yyval.ttype) = check_rval ((yyvsp[0].ttype)); }
    break;

  case 421:
#line 2286 "parse.yy"
    {
		(yyval.ttype) = check_one_bit_constant((yyvsp[0].ttype));
	}
    break;

  case 422:
#line 2290 "parse.yy"
    {
		(yyval.ttype) = check_one_bit_constant((yyvsp[0].ttype));
	}
    break;

  case 423:
#line 2297 "parse.yy"
    { 
		check_input_port((yyvsp[0].ttype));
		(yyval.ttype) = check_rval ((yyvsp[0].ttype));
	 }
    break;

  case 424:
#line 2302 "parse.yy"
    { 
		check_input_port((yyvsp[-3].ttype));
		(yyval.ttype) = build_bit_ref ( check_rval_nocheck ((yyvsp[-3].ttype)), (yyvsp[-1].ttype));
	 }
    break;

  case 425:
#line 2307 "parse.yy"
    { 
		check_input_port((yyvsp[-5].ttype));
		(yyval.ttype) = build_part_ref ( check_rval_nocheck ((yyvsp[-5].ttype)), (yyvsp[-3].ttype), (yyvsp[-1].ttype));
	}
    break;

  case 426:
#line 2320 "parse.yy"
    { (yyval.ttype) = make_node (CONCAT_REF);
		  CONCAT_LIST ((yyval.ttype)) = (yyvsp[-1].ttype);
		  concat_labels ((yyval.ttype));
		}
    break;

  case 427:
#line 2328 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 428:
#line 2330 "parse.yy"
    { (yyval.ttype) = build_tree_list ((yyvsp[0].ttype), NULL_TREE); }
    break;

  case 429:
#line 2332 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); }
    break;

  case 430:
#line 2334 "parse.yy"
    { (yyval.ttype) = tree_cons ((yyvsp[0].ttype), NULL_TREE, (yyvsp[-2].ttype)); }
    break;

  case 437:
#line 2348 "parse.yy"
    { if (!TREE_CONSTANT_ATTR ((yyvsp[0].ttype))) {
		    error ("Illegal non-constant expression", NULL_CHAR, NULL_CHAR);
		    (yyval.ttype) = error_mark_node;
		  }
		  else
		    (yyval.ttype) = (yyvsp[0].ttype);
		}
    break;

  case 439:
#line 2360 "parse.yy"
    { switch (delay_type) {
		    case (MIN_DELAY):  (yyval.ttype) = (yyvsp[-4].ttype); break;
		    case (TYP_DELAY):  (yyval.ttype) = (yyvsp[-2].ttype); break;
		    case (MAX_DELAY):  (yyval.ttype) = (yyvsp[0].ttype); break;
		    }
		}
    break;

  case 440:
#line 2370 "parse.yy"
    { switch (delay_type) {
		    case (MIN_DELAY):  (yyval.ttype) = (yyvsp[-4].ttype); break;
		    case (TYP_DELAY):  (yyval.ttype) = (yyvsp[-2].ttype); break;
		    case (MAX_DELAY):  (yyval.ttype) = (yyvsp[0].ttype); break;
		    }
		}
    break;

  case 442:
#line 2381 "parse.yy"
    { (yyval.ttype) = (yyvsp[0].ttype); }
    break;

  case 443:
#line 2383 "parse.yy"
    { (yyval.ttype) = build_unary_op (NEGATE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 444:
#line 2385 "parse.yy"
    { (yyval.ttype) = build_unary_op (TRUTH_NOT_EXPR, (yyvsp[0].ttype)); }
    break;

  case 445:
#line 2387 "parse.yy"
    { (yyval.ttype) = build_unary_op (BIT_NOT_EXPR, (yyvsp[0].ttype)); }
    break;

  case 446:
#line 2389 "parse.yy"
    { (yyval.ttype) = build_unary_op (AND_REDUCE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 447:
#line 2391 "parse.yy"
    { (yyval.ttype) = build_unary_op (NAND_REDUCE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 448:
#line 2393 "parse.yy"
    { (yyval.ttype) = build_unary_op (OR_REDUCE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 449:
#line 2395 "parse.yy"
    { (yyval.ttype) = build_unary_op (NOR_REDUCE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 450:
#line 2397 "parse.yy"
    { (yyval.ttype) = build_unary_op (XOR_REDUCE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 451:
#line 2399 "parse.yy"
    { (yyval.ttype) = build_unary_op (XNOR_REDUCE_EXPR, (yyvsp[0].ttype)); }
    break;

  case 452:
#line 2401 "parse.yy"
    { (yyval.ttype) = build_binary_op (PLUS_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 453:
#line 2403 "parse.yy"
    { (yyval.ttype) = build_binary_op (MINUS_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 454:
#line 2405 "parse.yy"
    { (yyval.ttype) = build_binary_op (MULT_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 455:
#line 2407 "parse.yy"
    { syn_warning ("Division operator"); }
    break;

  case 456:
#line 2409 "parse.yy"
    { (yyval.ttype) = build_binary_op (DIV_EXPR, (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
    break;

  case 457:
#line 2411 "parse.yy"
    { syn_warning ("Modulus operator"); }
    break;

  case 458:
#line 2413 "parse.yy"
    { (yyval.ttype) = build_binary_op (MOD_EXPR, (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
    break;

  case 459:
#line 2415 "parse.yy"
    { (yyval.ttype) = build_binary_op (EQ_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 460:
#line 2417 "parse.yy"
    { (yyval.ttype) = build_binary_op (NE_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 461:
#line 2419 "parse.yy"
    { syn_warning ("Case equality"); }
    break;

  case 462:
#line 2421 "parse.yy"
    { (yyval.ttype) = build_binary_op (EQ_CASE_EXPR, (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
    break;

  case 463:
#line 2423 "parse.yy"
    { syn_warning ("Case inequality"); }
    break;

  case 464:
#line 2425 "parse.yy"
    { (yyval.ttype) = build_binary_op (NE_CASE_EXPR, (yyvsp[-3].ttype), (yyvsp[0].ttype)); }
    break;

  case 465:
#line 2427 "parse.yy"
    { (yyval.ttype) = build_binary_op (TRUTH_ANDIF_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 466:
#line 2429 "parse.yy"
    { (yyval.ttype) = build_binary_op (TRUTH_ORIF_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 467:
#line 2431 "parse.yy"
    { (yyval.ttype) = build_binary_op (LT_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 468:
#line 2433 "parse.yy"
    { (yyval.ttype) = build_binary_op (LE_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 469:
#line 2435 "parse.yy"
    { (yyval.ttype) = build_binary_op (GT_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 470:
#line 2437 "parse.yy"
    { (yyval.ttype) = build_binary_op (GE_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 471:
#line 2439 "parse.yy"
    { (yyval.ttype) = build_binary_op (BIT_AND_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 472:
#line 2441 "parse.yy"
    { (yyval.ttype) = build_binary_op (BIT_OR_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 473:
#line 2443 "parse.yy"
    { (yyval.ttype) = build_binary_op (BIT_XOR_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 474:
#line 2445 "parse.yy"
    { (yyval.ttype) = build_binary_op (BIT_XNOR_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 475:
#line 2447 "parse.yy"
    { (yyval.ttype) = build_binary_op (LSHIFT_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 476:
#line 2449 "parse.yy"
    { (yyval.ttype) = build_binary_op (RSHIFT_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 477:
#line 2451 "parse.yy"
    { (yyval.ttype) = build_cond_expr ((yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 481:
#line 2475 "parse.yy"
    { (yyval.ttype) = check_rval ((yyvsp[0].ttype)); }
    break;

  case 482:
#line 2477 "parse.yy"
    { (yyval.ttype) = build_bit_ref (check_rval_nocheck ((yyvsp[-3].ttype)), (yyvsp[-1].ttype)); }
    break;

  case 483:
#line 2479 "parse.yy"
    { (yyval.ttype) = build_part_ref (check_rval_nocheck ((yyvsp[-5].ttype)), (yyvsp[-3].ttype), (yyvsp[-1].ttype)); }
    break;

  case 486:
#line 2483 "parse.yy"
    { (yyval.ttype) = (yyvsp[-1].ttype); }
    break;

  case 487:
#line 2485 "parse.yy"
    { (yyval.ttype) = error_mark_node; }
    break;

  case 488:
#line 2518 "parse.yy"
    { (yyval.ttype) = make_node (CONCAT_REF);
		  CONCAT_LIST ((yyval.ttype)) = (yyvsp[-1].ttype);
		  concat_labels ((yyval.ttype));
		}
    break;

  case 489:
#line 2523 "parse.yy"
    { (yyval.ttype) = make_node (CONCAT_REP_REF);
		  CONCAT_LIST ((yyval.ttype)) = (yyvsp[-2].ttype);
		  CONCAT_EXPR ((yyval.ttype)) = (yyvsp[-4].ttype);
		  concat_labels ((yyval.ttype));
		}
    break;

  case 490:
#line 2532 "parse.yy"
    { (yyval.ttype) = build_function_call ((yyvsp[-3].ttype), nreverse ((yyvsp[-1].ttype)));
		  HIERARCHICAL_ATTR ((yyval.ttype)) = HIERARCHICAL_ATTR ((yyvsp[-3].ttype));
                  UNKNOWN_TYPE_ATTR ((yyval.ttype)) = 1;
		}
    break;

  case 491:
#line 2538 "parse.yy"
    { (yyval.ttype) = build_nt (SYSFUNCTION_REF, IDENTIFIER_POINTER ((yyvsp[-3].ttype)),
			nreverse ((yyvsp[-1].ttype)), NULL_TREE, NULL_TREE, lineno, input_filename); }
    break;

  case 492:
#line 2542 "parse.yy"
    { (yyval.ttype) = build_nt (SYSFUNCTION_REF, IDENTIFIER_POINTER ((yyvsp[0].ttype)),
			NULL_TREE, NULL_TREE, NULL_TREE, lineno, input_filename); }
    break;

  case 493:
#line 2552 "parse.yy"
    {
		  syn_warning ("Hierachical name");
		/*  hierarchical_list = link_tree_cons ($1, NULL_TREE, hierarchical_list); */
		  HIERARCHICAL_ATTR ((yyvsp[0].ttype)) = 1;
                  UNKNOWN_TYPE_ATTR ((yyvsp[0].ttype)) = 1;
		  TREE_CONSTANT_ATTR ((yyvsp[0].ttype)) = 1;  /* assume this is true for now */
		}
    break;

  case 496:
#line 2566 "parse.yy"
    { (yyval.ttype) = get_identifier ("or"); }
    break;

  case 497:
#line 2585 "parse.yy"
    { (yyval.ttype) = NULL_TREE; }
    break;

  case 499:
#line 2591 "parse.yy"
    { (yyval.ttype) = build_nt (DELAY_EXPR, (yyvsp[0].ttype), (yyvsp[0].ttype), NULL_TREE);
 		  TREE_LABEL ((yyval.ttype)) = 1;
		}
    break;

  case 500:
#line 2595 "parse.yy"
    { (yyval.ttype) = check_rval ((yyvsp[0].ttype));
		  (yyval.ttype) = build_nt (DELAY_EXPR, (yyval.ttype), (yyval.ttype), NULL_TREE);
		  TREE_LABEL ((yyval.ttype)) = 1;
		}
    break;

  case 501:
#line 2600 "parse.yy"
    { (yyval.ttype) = (yyvsp[-1].ttype); }
    break;

  case 502:
#line 2609 "parse.yy"
    { (yyval.ttype) = build_nt (DELAY_EXPR, (yyvsp[0].ttype), (yyvsp[0].ttype), NULL_TREE);
		  TREE_LABEL ((yyval.ttype)) = 1;
		}
    break;

  case 503:
#line 2613 "parse.yy"
    { (yyval.ttype) = build_nt (DELAY_EXPR, (yyvsp[-2].ttype), (yyvsp[0].ttype), NULL_TREE);
		  TREE_LABEL ((yyval.ttype)) = 2;
		}
    break;

  case 504:
#line 2617 "parse.yy"
    { (yyval.ttype) = build_nt (DELAY_EXPR, (yyvsp[-4].ttype), (yyvsp[-2].ttype), (yyvsp[0].ttype));
		  TREE_LABEL ((yyval.ttype)) = 3;
		}
    break;

  case 505:
#line 2624 "parse.yy"
    { (yyval.ttype) = (yyvsp[0].ttype); }
    break;

  case 506:
#line 2626 "parse.yy"
    { (yyval.ttype) = check_rval ((yyvsp[0].ttype)); }
    break;

  case 507:
#line 2628 "parse.yy"
    { (yyval.ttype) = (yyvsp[-1].ttype); }
    break;

  case 508:
#line 2632 "parse.yy"
    { in_event = 1; }
    break;

  case 509:
#line 2634 "parse.yy"
    {if (!HIERARCHICAL_ATTR ((yyvsp[0].ttype)) &&
			TREE_CODE (IDENT_CURRENT_DECL ((yyvsp[0].ttype))) == EVENT_DECL)
		    (yyval.ttype) = build_stmt (EVENT_STMT, lineno, NULL_TREE,
			build_tree_list (IDENT_CURRENT_DECL ((yyvsp[0].ttype)), NULL_TREE));
		  else
		    (yyval.ttype) = build_stmt (EVENT_STMT, lineno, NULL_TREE,
			build_tree_list (build_unary_op
			(ANYEDGE_EXPR, check_rval ((yyvsp[0].ttype))), NULL_TREE));
		  in_event = 0;
		}
    break;

  case 510:
#line 2644 "parse.yy"
    { in_event = 1; }
    break;

  case 511:
#line 2646 "parse.yy"
    { (yyval.ttype) = build_stmt (EVENT_STMT, lineno, NULL_TREE, (yyvsp[-1].ttype));
		  in_event = 0; }
    break;

  case 512:
#line 2652 "parse.yy"
    { /*if (TREE_CODE ($1) == EVENT_DECL)
		    $$ = build_tree_list ($1, NULL_TREE);
		  else */
		    (yyval.ttype) = build_tree_list
			(build_unary_op (ANYEDGE_EXPR, (yyvsp[0].ttype)), NULL_TREE);
		}
    break;

  case 513:
#line 2659 "parse.yy"
    { (yyval.ttype) = build_tree_list 
			(build_unary_op (POSEDGE_EXPR, (yyvsp[0].ttype)), NULL_TREE); }
    break;

  case 514:
#line 2662 "parse.yy"
    { (yyval.ttype) = build_tree_list 
			(build_unary_op (NEGEDGE_EXPR, (yyvsp[0].ttype)), NULL_TREE); }
    break;

  case 515:
#line 2665 "parse.yy"
    { (yyval.ttype) = chainon ((yyvsp[-2].ttype), (yyvsp[0].ttype)); }
    break;

  case 516:
#line 2674 "parse.yy"
    { yyerrok; }
    break;

  case 517:
#line 2679 "parse.yy"
    { yyerrok; }
    break;

  case 518:
#line 2681 "parse.yy"
    { yyerrok; }
    break;

  case 519:
#line 2686 "parse.yy"
    { yyerrok; }
    break;


    }

/* Line 1037 of yacc.c.  */
#line 5321 "parse.cc"

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


#line 2689 "parse.yy"

void
eat_block (short token)
{
  YYSTYPE lval;

  for (;;)
    if (yylex (&lval) == token)
      return;
}


