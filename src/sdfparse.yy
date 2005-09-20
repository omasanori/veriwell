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

/* sdfparse.y - BISON/YACC parser for sdf input */

%{
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

%}
%pure_parser

%union {
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
};

%nonassoc	IF
%nonassoc	ELSE

%right	<code>	'?' ':'
%left	<code>	OROR
%left	<code>	ANDAND
%left	<code> 	ANDANDAND
%left	<code>	'|'
%left	<code>	'^' XNOR
%left	<code>	'&'
%left	<code>	LOGICAL_EQUALITY LOGICAL_INEQUALITY CASE_EQUALITY CASE_INEQUALITY
%left	<code>	LT LE GT GE
%left	<code>	LEFT_SHIFT RIGHT_SHIFT
%left	<code>	'+' '-'
%left	<code>	'*' '/' '%'
%left	<code>	'(' '['
%right	<code>	'!' '~'
%right	<code>	REDUCTION_NOR
%right	<code>	REDUCTION_NAND
%right	<code>	UNARY


%token	<integer> INTEGER
%token	<integer> SCALAR_CONSTANT
%token  <floating> FLOATING
%token	<ptr>	QSTRING
%token	<ptr>	IDENTIFIER
%token <ptr> BLOB 

%token POSEDGE
%token NEGEDGE
%token DELAYFILE
%token SDFVERSION
%token DESIGN
%token DATE
%token VENDOR
%token PROGRAM
%token VERSION_
%token PROGRAM
%token DIVIDER
%token VOLTAGE
%token PROCESS
%token TEMPERATURE
%token TIMESCALE
%token CELL	
%token CELLTYPE
%token INSTANCE
%token DELAY
%token TIMINGCHECK
%token INCLUDE
%token ABSOLUTE
%token INCREMENT
%token USERDEF
%token PATHPULSE
%token GLOBALPATHPULSE
%token IOPATH
%token COND
%token PORT
%token INTERCONNECT
%token NETDELAY
%token DEVICE
%token SETUP
%token HOLD
%token SETUPHOLD
%token RECOVERY
%token SKEW
%token WIDTH
%token PERIOD
%token NOCHANGE
%token PATHCONSTRAINT
%token SUM
%token DIFF
%token SKEWCONSTRAINT
%token MICROSECOND
%token NANOSECOND
%token PICOSECOND
%token EDGE_01
%token EDGE_10
%token EDGE_0z
%token EDGE_1z
%token EDGE_z0
%token EDGE_z1



%token	EOF_

%type	<floating> 	real
%type	<floating> 	preal
%type	<ptr>		nstring
%type	<creal>		creal
%type	<triple>	rexpression
%type	<creal>		cpreal
%type	<triple>	expression
%type	<elist>		exp_list
%type	<elist>		rexp_list
%type	<elist>		tc_rvalue	
%type	<elist>		double_tc_rvalue	
%type	<elist>		value
%type	<elist>		rvalue
%type	<port>		port
%type	<port>		port_spec
%type	<port>		port_path
%type 	<port>		port_instance
%type 	<port>		port_edge
%type 	<port>		name
%type 	<port>		net	
%type 	<port>		net_instance
%type	<ptr>		string_instance
%type	<integer>	ts

%%

sdf
	:	 delay_file EOF_
	;

delay_file
	:	'(' DELAYFILE sdf_version design_name date vendor program_name 
			program_version hierarchy_divider voltage process
			temperature time_scale cells rp
		{}
	;

sdf_version
	:	'(' SDFVERSION nstring rp
		{}
	;

design_name
	:	'('	DESIGN	nstring rp
		{}
	;

date
	:	'('	DATE	nstring rp
		{}
	;

vendor
	:	'('	VENDOR	nstring rp
		{}
	;

program_name
	:	'('	PROGRAM	nstring rp
		{}
	;

program_version
	:	'('	VERSION_	nstring rp
		{}
	;

hierarchy_divider
	:	'('	DIVIDER	 { setParseBlob(); } BLOB rp
		{}
	;

voltage
	:	'('	VOLTAGE	rexpression rp
		{}
	|	'(' VOLTAGE rp
		{}
	;

process
	:	'('	PROCESS	nstring rp
		{}
	;

temperature
	:	'('	TEMPERATURE	rexpression rp
		{}
	|	'('	TEMPERATURE	rp
		{}
	;

time_scale
	:	'('	TIMESCALE	ts NANOSECOND rp
		{
			/* set time scale for back annotation */
			setTimeScale( $3 - 9 );
		}
	|	'('	TIMESCALE	ts MICROSECOND rp
		{
			/* set time scale for back annotation */
			setTimeScale( $3 - 6 );
		}
	|	'('	TIMESCALE	ts PICOSECOND rp
		{
			/* set time scale for back annotation */
			setTimeScale( $3 - 12 );
		}
	|	'('	TIMESCALE	rp
		{
			/* set default (1ns) time scale for back annotation */
			setTimeScale( -9 );
		}
	;

ts	
	:	INTEGER	
		{
			switch(	$1 ) {
				case 1:
					$$ = 0;
					break;
				case 10:
					$$ = 1;
					break;
				case 100:
					$$ = 2;
					break;
				default:
					sdf_error( "Timescale multiplier must be either 1,10, or 100" );
					$$ = 0;
			} 
		}

cells
	:	cell	cells
	|	cell
	|	/* NULL */
	;
cell	
	:	'('	CELL 
		{
			/* reset current instance */
			resetInstance();
			/* free all allocated strings */
			purgeStrings();
		}
		celltype '(' instances timing_specs rp
	;

celltype
	:	'(' CELLTYPE QSTRING rp
		{	
			/* set current cell type */
			setCellType( $3 );
		}
	;

instances
	:	instance '(' instances
	|	instance '('
	;

instance
	:	 INSTANCE IDENTIFIER rp
		{
			/* set down thru the hierarchy */
			findInstance( $2 );
		}
	|	INSTANCE '*' rp
		{
			/* this is a wild card  */
			findInstance( "*" );
		}
	;

string_instance
	:	INSTANCE IDENTIFIER rp
		{
			$$ = $2;
		}
	;

timing_specs
	:	timing_spec
	|	timing_spec '(' timing_specs

timing_spec
	:	/* NULL */
	|	DELAY deltypes rp
	|	TIMINGCHECK {setAbsoluteDelay(TRUE);} tcdefs rp
	|	INCLUDE { setParseBlob(); } BLOB rp
		{
			sdflexOpenFile( $3 );
		}
	;	

tcdefs
	:	tcdef	tcdefs
	|	tcdef	
	;

tcdef
	:	
		'('	COND	timing_check_condition tcheck rp
		{}
	|	tcheck
		{}
	;

timing_check_condition
	:	/* NULL for now */
	;

tcheck
	:	'(' SETUP port_path port_spec tc_rvalue rp
		{
			setConstraint( eSETUP, &$3, &$4, $5 );
		}
	|	'(' HOLD port_path port_spec tc_rvalue rp
		{
			setConstraint( eHOLD, &$4, &$3, $5 );
		}
	|	'(' SETUPHOLD port_path port_spec double_tc_rvalue rp
		{
			setConstraint( eSETUPHOLD, &$4, &$3, $5 );
		}
	|	'(' RECOVERY port_spec port_spec tc_rvalue rp
		{
			setConstraint( eRECOVERY, &$4, &$3, $5 );
		}
	|	'(' SKEW port_spec port_spec tc_rvalue rp
		{
			setConstraint( eSKEW, &$3, &$4, $5 );
		}
	|	'(' WIDTH port_spec tc_rvalue rp
		{
			setConstraint( eWIDTH, &$3, NULL, $4 );
		}
	|	'(' PERIOD port_spec tc_rvalue rp
		{
			setConstraint( ePERIOD, &$3, NULL, $4 );
		}
	|	'(' NOCHANGE port_spec port_path double_tc_rvalue rp
		{
			setConstraint( eNOCHANGE, &$3, &$4, $5 );
		}
	;
		
deltypes
	:	deltype	deltypes
	|	deltype
	;

deltype
	:	'(' ABSOLUTE 
		{
			/* mark delay type as absolute */
			setAbsoluteDelay( TRUE );
		}
		absvals rp
	|	'(' INCREMENT 
		{
			/* mark delay type as incremental */
			setAbsoluteDelay( FALSE );
		}
		incvals rp
	;

absvals
	:	absval	absvals
	|	absval
	;

incvals
	:	incval	incval
	|	incval
	;

name
	:	port_instance
	|	net_instance
	;

net_instance
	:	net
	|	string_instance net
		{
			$$ = $2;
			$$.path = $1;
		}
	;

net
	:	IDENTIFIER
		{
			$$.path = NULL;
			$$.name = $1;
			$$.scalar = TRUE;
			$$.edge = accNoEdge;
		}
	;

port_instance
	:	port_path
	|	string_instance port_path
		{
			$$ = $2;
			$$.path = $1;
		}
	;	


absval
	:	'(' IOPATH port_spec port_path value rp
		{
			setConstraint( eIOPATH, &$3, &$4, $5 );
		}
	|	'(' COND conditional_port_expr '(' IOPATH port_spec port_path value rp rp
		{
			/* -------- support for COND needs to be added ----------- */
			setConstraint( eIOPATH, &$6, &$7, $8 );
		}
	|	'(' PORT port_path value rp
		{
			setConstraint( ePORT, &$3, (port_t*)NULL, $4 );
		}
	|	'(' INTERCONNECT port_instance port_instance value rp
		{
			setConstraint( eINTERCONNECT, &$3, &$4, $5 );
		}
	|	'(' NETDELAY name value rp
		{
			setConstraint( eNETDELAY, &$3, (port_t*)NULL, $4 );
		}
	|	'(' DEVICE value rp
		{
			setConstraint( eDEVICE, (port_t*)NULL, (port_t*)NULL, $3 );
		}
	|	'(' DEVICE port_instance value rp
		{
			setConstraint( eDEVICE, &$3, (port_t*)NULL, $4 );
		}
	;

conditional_port_expr
	:	/* NULL for now....... */
	;

incval
	:	'(' IOPATH port_spec port_path rvalue rp
		{
			setConstraint( eIOPATH, &$3, &$4, $5 );
		}
	|	'(' COND conditional_port_expr '(' IOPATH port_spec port_path rvalue rp rp
		{
			/* -------- support for COND needs to be added ----------- */
			setConstraint( eIOPATH, &$6, &$7, $8 );
		}
	|	'(' PORT port_path rvalue rp
		{
			setConstraint( ePORT, &$3, (port_t*)NULL, $4 );
		}
	|	'(' INTERCONNECT port_instance port_instance rvalue rp
		{
			setConstraint( eINTERCONNECT, &$3, &$4, $5 );
		}
	|	'(' NETDELAY name rvalue rp
		{
			setConstraint( eNETDELAY, &$3, (port_t*)NULL, $4 );
		}
	|	'(' DEVICE value rp
		{
			setConstraint( eDEVICE, (port_t*)NULL, (port_t*)NULL, $3 );
		}
	|	'(' DEVICE port_instance rvalue rp
		{
			setConstraint( eDEVICE, &$3, (port_t*)NULL, $4 );
		}
	;
port_spec
	:	port_path
	|	'(' {sdfEnableEdges( TRUE ); } port_edge
		{ 
			sdfEnableEdges( FALSE );
			$$ = $3;
		}
	;
port_path
	:	port
	;

port
	:	IDENTIFIER
		{
			$$.path = NULL;
			$$.name = $1;
			$$.scalar = TRUE;
			$$.edge = accNoEdge;
		}
	|	IDENTIFIER '[' INTEGER rb
		{
			$$.path = NULL;
			$$.name = $1;
			$$.scalar = FALSE;
			$$.msb = $3;
			$$.lsb = $3;
			$$.edge = accNoEdge;
		}
	|	IDENTIFIER '[' INTEGER ':' INTEGER rb
		{
			$$.path = NULL;
			$$.name = $1;
			$$.scalar = FALSE;
			$$.msb = $3;
			$$.lsb = $5;
			$$.edge = accNoedge;
		}
	;	

port_edge
	:	POSEDGE port_path rp
		{
			$$ = $2;
			$$.edge = accPosedge;
		}
	|	NEGEDGE port_path rp
		{
			$$ = $2;
			$$.edge = accNegedge;
		}
	|	EDGE_01 port_path rp
		{
			$$ = $2;
			$$.edge = accEdge01;
		}
	|	EDGE_10 port_path rp
		{
			$$ = $2;
			$$.edge = accEdge10;
		}
	|	EDGE_z1 port_path rp
		{
			$$ = $2;
			$$.edge = accEdgex1;
		}
	|	EDGE_z0 port_path rp
		{
			$$ = $2;
			$$.edge = accEdgex0;
		}
	|	EDGE_1z port_path rp
		{
			$$ = $2;
			$$.edge = accEdge1x;
		}
	|	EDGE_0z port_path rp
		{
			$$ = $2;
			$$.edge = accEdge0x;
		}
	;


value
	:	'(' preal rp
		{
			memset( &$$, 0, sizeof($$) );
			$$.values[0].triple[0].value = $2;
			$$.values[0].triple[0].valid = TRUE;
			$$.values[0].triple[1].value = $2;
			$$.values[0].triple[1].valid = TRUE;
			$$.values[0].triple[2].value = $2;
			$$.values[0].triple[2].valid = TRUE;
			$$.argCount = 1;
		}
	|	exp_list
	;

double_tc_rvalue
	:	tc_rvalue tc_rvalue
		{
			$$.values[0] = $1.values[0];
			$$.values[1] = $2.values[0];
			$$.argCount = 2;
		}	
	;

tc_rvalue
	:	'(' real rp
		{
			memset( &$$, 0, sizeof($$) );
			$$.values[0].triple[0].value = $2;
			$$.values[0].triple[0].valid = TRUE;
			$$.values[0].triple[1].value = $2;
			$$.values[0].triple[1].valid = TRUE;
			$$.values[0].triple[2].value = $2;
			$$.values[0].triple[2].valid = TRUE;
			$$.argCount = 1;
		}
	|	'(' rexpression rp
		{ 
			$$.values[0] = $2;
			$$.argCount = 1;
		}
	;

rvalue
	:	'(' real rp
		{
			memset( &$$, 0, sizeof($$) );
			$$.values[0].triple[0].value = $2;
			$$.values[0].triple[0].valid = TRUE;
			$$.values[0].triple[1].value = $2;
			$$.values[0].triple[1].valid = TRUE;
			$$.values[0].triple[2].value = $2;
			$$.values[0].triple[2].valid = TRUE;
			$$.argCount = 1;
		}
	|	rexp_list
	;

exp_list
	:	'(' expression rp
		{
			$$.values[0] = $2;
			$$.argCount = 1;
		}
	|	'(' expression rp '(' expression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.argCount = 2;
		}
	|	'(' expression rp '(' expression rp '(' expression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.argCount = 3;
		}
	|	'(' expression rp '(' expression rp '(' expression rp
		'(' expression rp 
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.values[3] = $11;
			$$.argCount = 4;
		}
	|	'(' expression rp '(' expression rp '(' expression rp
		'(' expression rp '(' expression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.values[3] = $11;
			$$.values[4] = $14;
			$$.argCount = 5;
		}
	|	'(' expression rp '(' expression rp '(' expression rp
		'(' expression rp '(' expression rp '(' expression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.values[3] = $11;
			$$.values[4] = $14;
			$$.values[5] = $17;
			$$.argCount = 6;
		}
	;


rexp_list
	:	'(' rexpression rp
		{
			$$.values[0] = $2;
			$$.argCount = 1;
		}
	|	'(' rexpression rp '(' rexpression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.argCount = 2;
		}
	|	'(' rexpression rp '(' rexpression rp '(' rexpression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.argCount = 3;
		}
	|	'(' rexpression rp '(' rexpression rp '(' rexpression rp
		'(' rexpression rp 
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.values[3] = $11;
			$$.argCount = 4;
		}
	|	'(' rexpression rp '(' rexpression rp '(' rexpression rp
		'(' rexpression rp '(' rexpression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.values[3] = $11;
			$$.values[4] = $14;
			$$.argCount = 5;
		}
	|	'(' rexpression rp '(' rexpression rp '(' rexpression rp
		'(' rexpression rp '(' rexpression rp '(' rexpression rp
		{
			$$.values[0] = $2;
			$$.values[1] = $5;
			$$.values[2] = $8;
			$$.values[3] = $11;
			$$.values[4] = $14;
			$$.values[5] = $17;
			$$.argCount = 6;
		}
	;

rexpression
	:	real ':' creal ':' creal
		{
			$$.triple[0].value = $1;
			$$.triple[0].valid = TRUE;
			$$.triple[1] = $3;
			$$.triple[2] = $5;
		}
	|	creal ':' real ':' creal
		{
			$$.triple[0] = $1;
			$$.triple[1].value = $3;
			$$.triple[1].valid = TRUE; 
			$$.triple[2] = $5;
		}
	|	creal ':' creal ':' real
		{
			$$.triple[0] = $1;
			$$.triple[1] = $3;
			$$.triple[2].value = $5;
			$$.triple[2].valid = TRUE;
		}
	|	/* NULL */
		{
			$$.triple[0].valid = FALSE;
			$$.triple[1].valid = FALSE;
			$$.triple[2].valid = FALSE;
		}
	;

expression
	:	preal ':' cpreal ':' cpreal
		{
			$$.triple[0].value = $1;
			$$.triple[0].valid = TRUE;
			$$.triple[1] = $3;
			$$.triple[2] = $5;
		}
	|	cpreal ':' preal ':' cpreal
		{
			$$.triple[0] = $1;
			$$.triple[1].value = $3;
			$$.triple[1].valid = TRUE; 
			$$.triple[2] = $5;
		}
	|	cpreal ':' cpreal ':' preal
		{
			$$.triple[0] = $1;
			$$.triple[1] = $3;
			$$.triple[2].value = $5;
			$$.triple[2].valid = TRUE;
		}
	|	/* NULL */
		{
			$$.triple[0].valid = FALSE;
			$$.triple[1].valid = FALSE;
			$$.triple[2].valid = FALSE;
		}
	;

real
	:	preal
	|	'+' preal
		{ $$ = $2; }
	|	'-' preal
		{ $$ = -$2; }
	;
preal
	:	FLOATING
	|	INTEGER
		{ $$ = (double) $1; }
	;

creal
	:	real
		{ 	
			$$.value = $1; 
			$$.valid = TRUE;
		}
	|	/* NULL */
		{ $$.valid = FALSE; }
	;

cpreal
	:	preal
		{ 	
			$$.value = $1; 
			$$.valid = TRUE;
		}
	|	/* NULL */
		{ $$.valid = FALSE; }
	;

nstring
	:	QSTRING
	|	/* NULL */
		{ $$ = ""; }
	;

/*
"Important" terminals
*/

rp
	: ')'
		{ yyerrok; }
	;

rb
	: ']'
		{ yyerrok; }
	| error
		{ yyerrok; }
	;

%%

