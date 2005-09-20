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

/* sdfcpars.y - BISON/YACC parser for sdf configuration info */

%{
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

%}
%expect 0
%pure_parser

%union {
	int				integer;
	double			floating;
	char			*ptr;
	eScaleType_t	etype;
	eDelayType_t  	dtype;
	eSDFConstruct_t	stype;
	eVerilogConstruct_t vtype;
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
%token	<stype>	IOPATH
%token	<stype>	PORT
%token	<stype>	INTERCONNECT
%token	<stype>	NETDELAY
%token	<stype> DEVICE
%token	<stype>	SETUP
%token	<stype>	HOLD
%token	<stype>	SETUPHOLD
%token	<stype>	RECOVERY
%token	<stype>	SKEW
%token	<stype>	WIDTH
%token	<stype>	PERIOD
%token	<stype>	NOCHANGE
%token	<stype>	PATHPULSE
%token	<vtype>	IGNORE
%token	<vtype>	INTERMOD_PATH
%token	<vtype>	MIPD
%token	<vtype>	CELL
%token	<vtype> USE
%token	INTERCONNECT_MIPD
%token	<dtype> MINIMUM
%token	<dtype> MAXIMUM
%token	<dtype> AVERAGE
%token	MTM
%token	TOOL_CONTROL
%token	<dtype> TYPICAL
%token	SCALE_FACTORS
%token	SCALE_TYPE
%token	<etype>	FROM_MINIMUM
%token	<etype> FROM_TYPICAL
%token	<etype> FROM_MAXIMUM
%token	<etype> FROM_MTM
%token	FROM_FILE
%token	TURNOFF_DELAY
%token	TURNON_DELAY
%token	MODULE
%token	MAP_INNER
%token	ADD
%token	OVERRIDE
%token	IGNORE


%token	EOF_

%type	<floating> real
%type	<dtype>	   min_typ_max
%type	<dtype>    min_max_avg
%type	<etype>	   stype
%type	<stype>	   sdf_construct
%type	<vtype>	   veritool_map


%%

config
	:	 statements EOF_
	|	 EOF_
	;

statements
	:	statement 
	|	statement statements
	;

statement
	:	map 
	;

map	
	:	sdf_construct '=' veritool_map sc
		{
			if( ! setMap( $1, $3 ) ) {
				sdfconfig_error( "illegal construct mapping" );
			}
		}
	|	INTERCONNECT_MIPD '=' min_max_avg sc
		{
			setInterconnectMipd( $3 );
		}
	|	MTM '=' min_typ_max sc
		{
			setMTMSpec( $3 );
		}
	|	MTM '=' TOOL_CONTROL sc
		{
			setMTMSpec( eTOOL_CONTROL );
		}
	|	SCALE_FACTORS '=' real ':' real ':' real  sc
		{
			setScaleFactors( $3, $5, $7 );
		}
	|	SCALE_TYPE '=' stype sc
		{
			setScaleType( $3 );
		}
	|	TURNOFF_DELAY '=' min_max_avg sc
		{
			setTurnOffDelay( $3 );
		}
	|	TURNOFF_DELAY '=' FROM_FILE sc
		{
			setTurnOffDelay( eFROM_FILE );
		}
	;

stype
	:	FROM_MINIMUM
		{ $$ = eFROM_MINIMUM; }
	|	FROM_TYPICAL
		{ $$ = eFROM_TYPICAL; }
	|	FROM_MAXIMUM
		{ $$ = eFROM_MAXIMUM; }
	|	FROM_MTM
		{ $$ = eFROM_MTM; }
	;

real
	:	FLOATING
	|	INTEGER
		{
			$$ = (double) $1;
		}
	;

sdf_construct
	:	IOPATH
		{ $$ = eIOPATH; }
	|	PORT
		{ $$ = ePORT; }
	|	INTERCONNECT
		{ $$ = eINTERCONNECT; }
	|	NETDELAY
		{ $$ = eNETDELAY; }
	|	DEVICE
		{ $$ = eDEVICE; }
	|	SETUP
		{ $$ = eSETUP; }
	|	HOLD
		{ $$ = eHOLD; }
	|	SETUPHOLD
		{ $$ = eSETUPHOLD; }
	|	RECOVERY
		{ $$ = eRECOVERY; }
	|	SKEW
		{ $$ = eSKEW; }
	|	WIDTH
		{ $$ = eWIDTH; }
	|	PERIOD
		{ $$ = ePERIOD; }
	|	NOCHANGE
		{ $$ = eNOCHANGE; }
	|	PATHPULSE
		{ $$ = ePATHPULSE; }
	;

veritool_map
	:	IGNORE
		{ $$ = eVIGNORE; }
	|	INTERMOD_PATH
		{ $$ = eVINTERMODPATH; }
	|	MIPD
		{ $$ = eVMIPD; }
	|	CELL
		{ $$ = eVCELL; }
	|	USE
		{ $$ = eVUSE; }
	;

min_max_avg
	:	MINIMUM
		{ $$ = eMINIMUM; }
	|	MAXIMUM
		{ $$ = eMAXIMUM; }
	|	AVERAGE
		{ $$ = eAVERAGE; }
	;

min_typ_max
	:	MINIMUM
		{ $$ = eMINIMUM; }
	|	TYPICAL
		{ $$ = eTYPICAL; }
	|	MAXIMUM
		{ $$ = eMAXIMUM; }
	;

/*
"Important" terminals
*/


sc
	: ';'
		{ yyerrok; }
	;

%%

