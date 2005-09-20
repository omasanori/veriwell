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

/* PARSE.Y - BISON/YACC parser for program input */

%{
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

%}
%pure_parser

%union {
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
};

%nonassoc	LOWEST
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

%token	<ttype>	IDENTIFIER
%token	<ttype>	HIERARCHY_IDENTIFIER
%token	<code> STARARROW
%token  <code> EQUALARROW
%token	SPECPARAM
%token	SETUP
%token	HOLD
%token	PERIOD
%token	WIDTH
%token	SKEW
%token	RECOVERY
%token	SETUPHOLD
%token	NOCHANGE
%token	MODULE
%token	MACROMODULE 
%token	ENDMODULE
%token	SPECIFY
%token	ENDSPECIFY
%token	PRIMITIVE
%token	ENDPRIMITIVE
%token	TABLE
%token	ENDTABLE
%token	TASK
%token	ENDTASK
%token	FUNCTION
%token	ENDFUNCTION
%token	PARAMETER
%token	INPUT
%token	OUTPUT
%token	INOUT
%token	<ntype>	NETTYPE
%token	TRIREG
%token	SCALARED
%token	VECTORED
%token	REG
%token	TIME
%token	INTEGER
%token	REAL
%token	EVENT
%token	ASSIGN
%token	DEFPARAM
%token	<stype>	STRENGTH0 STRENGTH1 SUPPLY0 SUPPLY1
%token	<gtype>	LOGIC
%token	<gtype>	BUFFER
%token	<gtype>	BUFIF
%token	<gtype>	CMOS
%token	<gtype>	PULL
%token	<gtype>	TRAN
%token	INITIAL
%token	ALWAYS
%token	IF
/*%left	ELSE */
%token	CASE
%token	CASEZ
%token	CASEX
%token	ENDCASE
%token	FOREVER
%token	REPEAT
%token	WHILE
%token	FOR
%token	WAIT_
%token	ARROW_
%token	DISABLE
/*%token	<sym>	ASSIGN*/
%token	DEASSIGN
%token	FORCE
%token	RELEASE
%token	DEFAULT
%token	BEGIN
%token	END
%token	FORK
%token	JOIN
%token	<ttype>	SYSTEM_IDENTIFIER
%token	<ttype>	STRING
%token	<ttype> NUMBER
%token	<ttype> BASE_NUMBER
%token	<code>	POSEDGE
%token	<code>	NEGEDGE
%left 	OR
%token	SMALL
%token	MEDIUM
%token	LARGE
/* %token	STOP_STMT */
%token	EDGE
%token	<double_digit> DOUBLE_DIGIT
%token <c> UDP_CHAR
%token <c> UDP_ONE
%token <c> UDP_ZERO
%token <c> UDP_X
%token <c> UDP_B
%token <c> UDP_HOOK


%token	EOF_

%type	<ttype>	source_text
%type	<ttype>	description_list
%type	<ttype>	description
%type	<ttype>	module
/* %type	<ttype>	module_item_list */
/* %type	<ttype>	module_item */
%type	<ttype>	list_of_ports_o
%type	<ttype>	port_clist
%type	<ttype>	port
%type	<ttype>	port_expression_o
%type	<ttype>	port_reference_clist
%type	<ttype>	port_reference
%type	<ttype>	UDP
%type	<ttype>	task
%type	<ttype>	function function_type
%type	<ttype>	tf_declaration_list
%type	<ttype>	tf_declaration

%type	<ttype> specify_stuff
%type	<ttype> specify_items
%type	<ttype> specify_item
%type	<ttype> specparam_declaration
%type	<ttype> path_declaration
%type	<ttype> path_description
%type	<ttype> path_delay_value
%type	<ttype> specify_input_terminal_descriptor
%type	<ttype> specify_output_terminal_descriptor
%type	<ttype> list_of_path_inputs
%type	<ttype> list_of_path_outputs
%type	<ttype> path_delay_expression
%type	<integer> path_operator
%type	<integer> polarity_operator
%type	<ttype> conditional_port_expression
%type	<ttype> data_source_expression
%type	<integer> edge_identifier
%type	<ttype>  specify_if_clause	
%type	<ttype>	timing_input_terminal_descriptor

%type	<ttype>	parameter_declaration
%type	<ttype>	list_of_param_assignments
%type	<ttype>	param_assignment
%type	<ttype>	setspec setnetspec static_declaration port_declaration port_spec
%type	<ttype>	port_decl_identifiers
%type	<ttype>	reg_spec event_spec reg_decl_identifiers reg_decl_identifier
%type	<ttype> real_spec
%type	<ttype>	non_reg_decl_identifiers
%type	<ttype>	decl_identifiers

%type	<ttype>	net_declaration net_spec trireg_spec
%type	<ntype>	nettype
%type	<ttype>	expandrange_o
%type	<ttype>	continuous_assign assign_spec list_of_assignments
%type	<ttype>	cont_assignment
%type	<ttype>	net_assignment list_of_net_assignments
%type	<ttype>	parameter_override list_of_param_overrides
%type	<ttype>	charge_strength_o
%type	<ttype>	drive_strength drive_strength_o
%type	<stype>	strength0 strength1
%type	<ttype>	xrange
%type	<ttype>	range
/* %type	<ttype>	gate_declaration */
/* %type	<ttype>	gate_description */
%type	<gtype>	gatetype
%type	<ttype>	gate_instance_clist
%type	<ttype>	gate_instance
%type	<ttype>	UDP_or_module_instantiation
%type	<ttype>	xparameter_value_assignment constant_expression_clist
/* %type	<ttype>	UDP_instance_clist */
/* %type	<ttype>	UDP_instance */
/* %type	<ttype>	xterminal_clist */
%type	<ttype>	terminal_clist
/*%type	<ttype>	module_instantiation */
%type	<ttype>	module_instance_clist
%type	<ttype>	module_instance
%type	<ttype>	list_of_module_connections_o
%type	<ttype>	module_port_connection_clist
%type	<ttype>	module_port_connection
%type	<ttype>	named_port_connection_clist
%type	<ttype>	named_port_connection
%type	<ttype> system_timing_check
%type	<ttype> notify_clause
%type	<ttype> notify_register
%type	<ttype>	timing_check_event
%type	<ttype> controlled_timing_check_event
%type	<integer> timing_check_event_control
%type	<integer> timing_check_event_control_no_empty
%type	<ttype>	timing_check_condition
%type	<ttype> scalar_expression	
%type	<ttype>	scalar_constant		

%type	<ttype>	specify_block

%type	<ttype>	initial_statement
%type	<ttype>	always_statement
%type	<ttype>	statement_or_null
%type	<ttype>	statement
%type	<ttype>	elsepart
%type	<ttype>	case
/* %type	<ttype> delay_or_event_control */
%type	<ttype>	assignment
%type	<ttype>	case_item_plist
%type	<ttype>	case_expr
%type	<ttype>	expression_clist xexpression_clist xexpression
%type	<ttype>	seq_block
%type	<ttype>	par_block
%type	<ttype>	statement_list
%type	<ttype>	statement_tree_list
%type	<ttype>	block_declaration_list
%type	<ttype>	block_declaration
%type	<ttype>	task_enable
%type	<ttype>	system_task_enable

%type	<ttype>	lvalue
%type	<ttype>	lval_normal
%type	<ttype>	lval_concat
%type	<ttype>	lval_clist
%type	<ttype>	conditional_expression
%type	<ttype>	constant_expression
%type	<ttype>	mintypmax_expression
%type	<ttype>	mintypmax_expression_triplet
%type	<ttype>	expression
%type	<ttype>	primary
%type	<ttype>	concatenation
%type	<ttype>	function_call
%type	<ttype>	identifier
%type	<ttype>	IDENTIFIER_or_OR
%type	<ttype>	delay_o
%type	<ttype>	delay
%type	<ttype>	mintypmax_clist
%type	<ttype>	delay_control
%type	<ttype>	event_control
%type	<ttype>	event_expression
%type	<integer> edge_list

%type	<ttype>	rp
%type	<ttype>	sc

%type	<ttype> UDP_declarations
%type	<ttype> UDP_declaration
%type	<ttype> UDP_reg_statement
%type	<ttype> UDP_initial_statement
%type	<ttype> UDP_table_definition		
%type	<ttype> table_entry_list
%type	<ttype>	table_entry
%type	<ttype> table_digit_list
%type	<ttype>	table_digit
%type	<c>		table_digit1

/*
%type	<ttype>	interactive_command
%type	<ttype>	execute_command
%type	<ttype>	disable_command
%type	<ttype>	continue
%type	<ttype>	step_no_trace
%type	<ttype>	step_trace
%type	<ttype>	where
*/

%type	<ttype>	error

%%

source_text
	: statement	{ if (!is_interactive)
			    YYFAIL;
#if VDEBUG != 0
			  if (vdebug)
			    printf_error_V ("interactive statement\n");
#endif
			  interactive_statement = $1;
			  fix_lastchar();
			  YYACCEPT;}
	| description_list EOF_
	| error
		{ 
#if VDEBUG != 0
if (vdebug)
  printf ("source error\n");
#endif
		  if (is_interactive)
		    YYABORT;
		}
	| EOF_		{ 
#if VDEBUG != 0
if (vdebug)
  printf_error_V ("EOF\n");
#endif
		 $$ = NULL; }
	;

description_list
	: description
	| description_list description
		{ yyerrok;
#if VDEBUG != 0
if (vdebug)
  printf ("description_list\n");
#endif
	  	}
/*	| error */
/*	| description_list error */
	;

description
	: module
 	| UDP
	;

module
	: MODULE
		{ if (is_interactive) {
		    YYFAIL;
                  }
		}
	  IDENTIFIER_or_OR
		{
		  current_module = current_scope = build_module ($3);
		  push_scope ();
		  /* list of parse modules */
		  module_list = tree_cons (current_module, NULL_TREE, module_list);
		  /* for now, assume all to be at lop level */
//	       	  top_level = chainon (current_module, top_level);
		}
	 list_of_ports_o sc
		{ MODULE_PORT_LIST (current_module) = nreverse ($5); }
	  module_item_list ENDMODULE
		{ 
		  current_scope = pop_scope (); 
		  BLOCK_BODY (current_module) = nreverse (BLOCK_BODY (current_module));
		  BLOCK_DECL (current_module) = nreverse (BLOCK_DECL (current_module));
		  end_module (current_module);
		}

/*	| MACROMODULE IDENTIFIER list_of_ports_o sc module_item_list ENDMODULE */
	;

module_item_list
	: /* empty */
	| module_item_list module_item
		{ yyerrok; }
	| module_item_list error
	;

list_of_ports_o
	: '(' port_clist rp
		{ $$ = $2; }
	| /* empty */
		{ $$ = NULL; }
	;

port_clist
	: port
	| port_clist ',' port
		{ yyerrok;
		  $$ = chainon ($3, $1);
		}
	| error
/*
	| port_clist error
	| port_clist error port
		{ yyerrok; }
*/
	| port_clist ',' error
	;

port
	: port_expression_o
		{ if ($1)
		    $$ = build_tree_list ($1, NULL_TREE);
		  else
		    $$ = NULL_TREE;
		}
	| '.' IDENTIFIER '(' port_expression_o rp
		{ $$ = build_tree_list ($4, $2); }
	;

port_expression_o
	: port_reference
	| '{' port_reference_clist '}'
		{ $$ = $2; 
		  sorry ("Port concatenations not supported in this version");
		}
	| /* empty */
		{ $$ = NULL_TREE; }
	;

port_reference_clist
	: port_reference
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| port_reference_clist ',' port_reference
		{ $$ = tree_cons ($3, NULL_TREE, $1); }
	;

port_reference
	: IDENTIFIER
		{ set_decl ($1, $1); }  /* point to itself to mark as port */
	| IDENTIFIER '[' constant_expression ']'
		{ $$ = build_bit_ref ($1, $3);
		  set_decl ($1, $$);
//		  sorry ("Port bit-select not supported in this version");
		}
	| IDENTIFIER '[' constant_expression ':' constant_expression ']'
		{ $$ = build_part_ref ($1, $3, $5);
		  set_decl ($1, $$);
//		  sorry ("Port part-selects not supported in this version");
		}
	;

module_item
	: parameter_declaration
	{}
	| port_declaration
	{}
	| net_declaration
	{}
	| static_declaration
	{}
	| gate_declaration
	| UDP_or_module_instantiation
	{}
	| parameter_override
	{}
	| continuous_assign
	{}
	| specify_block
	{}
	| initial_statement
		{ BLOCK_BODY (current_module) = tree_cons ($1,
			(tree)INITIAL_CODE, BLOCK_BODY (current_module));
		}
	| always_statement
		{ BLOCK_BODY (current_module) = tree_cons ($1,
			(tree)ALWAYS_CODE, BLOCK_BODY (current_module));
		}
	| task
	{}
	| function
	{}
	;
/*
UDP
	: PRIMITIVE IDENTIFIER
		{ syn_warning ("primitive definition");
		  sorry ("User-defined primitives not supported in this version");
		  eat_block (ENDPRIMITIVE);
		  $$ = NULL;
		}
	;
*/

UDP
	: PRIMITIVE  
		{ if (is_interactive) {
		    YYFAIL;
                  }
		}
	  IDENTIFIER
		{
		  current_module = current_scope = build_udp ($3);
		  push_scope ();
		  /* list of parse modules */
		  module_list = tree_cons (current_module, NULL_TREE, module_list);
		}
	 list_of_ports_o sc
		{ UDP_PORT_LIST (current_module) = nreverse ($5); }
	  UDP_declarations UDP_initial_statement UDP_table_definition ENDPRIMITIVE
		{
		  current_scope = pop_scope (); 
		  UDP_STRING_LIST (current_module) = nreverse (UDP_STRING_LIST (current_module));
		  end_primitive (current_module);
		}
	;
UDP_declarations
	: /* empty */
	{ $$ = NULL_TREE; }
	| UDP_declarations UDP_declaration
		{ yyerrok; }
	| UDP_declarations error
	;

UDP_declaration
	: port_declaration
	| UDP_reg_statement 
	;

UDP_reg_statement
	: REG IDENTIFIER sc
	{
		if( strcmp( IDENT($2), 
			IDENT(DECL_NAME(TREE_PURPOSE(MODULE_PORT_LIST(current_module)))) )!=0 ) {
			error( "only the first declared port in a udp can be a reg",
									NULL_CHAR, NULL_CHAR );		
		} else {	
			UDP_REG_NAME(current_module) = $2;
		}
		$$ = NULL_TREE;
	}
	;
UDP_initial_statement
	: /* NULL */
	{ $$ = NULL_TREE; }
	| INITIAL IDENTIFIER '=' scalar_constant sc
	{
		if( UDP_REG_NAME(current_module)==NULL_TREE ) {
			error( "initial statement is no allowed in combinatorial udp's",
									NULL_CHAR, NULL_CHAR );		
		} else if( strcmp(IDENT($2),IDENT(UDP_REG_NAME(current_module)))!=0 ) {
			error( "initial statement does not reference port output",
									NULL_CHAR, NULL_CHAR );		
		} else {
			UDP_INITIAL_VALUE(current_module) = $4;
		}
		$$ = NULL_TREE;
	}
	; 
	


UDP_table_definition
	: TABLE 
	{ 	extern int enable_udp_parse;
		enable_udp_parse = TRUE;
	    current_udp_string = NULL_TREE;
	}
	table_entry_list ENDTABLE	
	{ 
		UDP_STRING_LIST(current_module) = $3;
		$$ = NULL_TREE;
	}
	;
table_entry_list
	: table_entry 
	| table_entry_list table_entry
	{ 
		$$ = chainon ($2, $1);
	}
	;
table_entry
	:	table_digit_list sc
	{ 
		validate_udp_string( current_module, current_udp_string );
		$$ = current_udp_string;
		current_udp_string = NULL_TREE;
	}
	;
table_digit_list
	:	/* NULL */
	{ $$ = NULL; }
	|	table_digit_list table_digit
	{ $$ = NULL; }
	;
table_digit
	:	UDP_CHAR
	{
		append_udp_digits( &current_udp_string, $1, $1 );
		$$ = NULL;
	}
	|	UDP_HOOK
	{
		append_udp_digits( &current_udp_string,'?','?' );
		$$ = NULL;
	}
	|	UDP_ZERO
	{
		append_udp_digits( &current_udp_string,'0','0' );
		$$ = NULL;
	}
	|	UDP_ONE
	{
		append_udp_digits( &current_udp_string,'1','1' );
		$$ = NULL;
	}
	|	UDP_B
	{
		append_udp_digits( &current_udp_string,'b','b' );
		$$ = NULL;
	}
	|	UDP_X
	{
		append_udp_digits( &current_udp_string,'x','x' );
		$$ = NULL;
	}
	| 	'(' table_digit1 table_digit1 rp
	{ 		
		/* ? and b must be convert different but equivilent characters
		   otherwise edges cannot be distinguished from levels */

		if( $2=='?' ) {	
			$2 = '!';
		} else if( $2=='b' ) {
			$2 = '%';
		}
		append_udp_digits( &current_udp_string,$2, $3 );
		$$ = NULL;
	}
	;
table_digit1
	:	UDP_X
	|	UDP_ZERO
	|	UDP_ONE
	|	UDP_B
	|	UDP_HOOK
	;

task
	: TASK IDENTIFIER sc
		{ syn_warning ("Task definition");
		  tmp_tree = build_task (check_task ($2));
		  make_block_decl ($3, current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  BLOCK_DOWN (current_module) = chainon (current_scope, BLOCK_DOWN (current_module));
		  BLOCK_UP (current_scope) = current_module;
		  push_scope ();
		  in_tf = 1;
		}
	  tf_declaration_list statement_or_null ENDTASK
		{ BLOCK_BODY (current_scope) = $6;
		  in_tf = 0;
		  BLOCK_PORTS (current_scope) = nreverse (BLOCK_PORTS (current_scope));
		  BLOCK_DECL (current_scope) = nreverse (BLOCK_DECL (current_scope));
		  current_scope = pop_scope ();
		}
	;

function
	: FUNCTION function_type IDENTIFIER sc
 		{ current_scope = build_function (check_function ($3));
		  make_block_decl ($3, current_module, current_scope);
		  push_scope ();  /* funct name becomes var inside of funct */
		  FUNCT_DECL (current_scope) = make_decl ($3, $2, NULL_TREE, NULL_TREE);
		  BLOCK_DOWN (current_module) = chainon (current_scope, BLOCK_DOWN (current_module));
		  BLOCK_UP (current_scope) = current_module;
		  in_tf = in_function = 1;
		}
	  tf_declaration_list statement_or_null ENDFUNCTION
		{ BLOCK_BODY (current_scope) = $7;
		  in_tf = in_function = 0;
		  BLOCK_PORTS (current_scope) = nreverse (BLOCK_PORTS (current_scope));
		  BLOCK_DECL (current_scope) = nreverse (BLOCK_DECL (current_scope));
		  current_scope = pop_scope ();
		}
	;

function_type
	: /* empty */
		{ $$ = make_reg_spec (NULL_TREE); }
	| range
		{ $$ = make_reg_spec ($1); }
	| INTEGER
		{ $$ = make_integer_spec (NULL_TREE); }
	| REAL
		{ $$ = make_real_spec (NULL_TREE); }
	;

tf_declaration_list
	: /* empty */
		{}
	| tf_declaration_list tf_declaration
	;

tf_declaration
	: parameter_declaration
	| port_declaration
	| static_declaration
	;

/*
A.2 Declarations
*/

parameter_declaration
	: PARAMETER xrange
		{ current_spec = make_param_spec ($2); }
	  list_of_param_assignments sc
		{ BLOCK_DECL (current_scope) =
			chainon ($4, BLOCK_DECL (current_scope));
		 }
	;

list_of_param_assignments
	: param_assignment
	| list_of_param_assignments ',' param_assignment
		{ yyerrok;
		  $$ = chainon ($3, $1);
		}
	| error
	| list_of_param_assignments error param_assignment
		{ yyerrok; }
	| list_of_param_assignments ',' error
	;

param_assignment
	: IDENTIFIER '=' constant_expression
		{ $$ = make_param_decl (check_non_reg ($1), current_spec, $3);
		}
	| IDENTIFIER '=' mintypmax_expression_triplet
		{ $$ = make_param_decl (check_non_reg ($1), current_spec, $3);
		}
	;

setspec
	: /* empty */
		{}
	;

static_declaration
	: reg_spec setspec reg_decl_identifiers sc
		{ BLOCK_DECL (current_scope) =
			chainon ($3, BLOCK_DECL (current_scope)); }
	| event_spec setspec non_reg_decl_identifiers sc
		{ BLOCK_DECL (current_scope) =
			chainon ($3, BLOCK_DECL (current_scope)); }
	| real_spec setspec decl_identifiers sc
		{ BLOCK_DECL (current_scope) =
			chainon ($3, BLOCK_DECL (current_scope)); }
	;

/* Used for ports and arguments to tasks and functions. */
port_declaration
	: port_spec setspec port_decl_identifiers sc
		{ BLOCK_PORTS (current_scope) =
			chainon ($3, BLOCK_PORTS (current_scope)); }
	;

/* If inside a module, ports are nets by default, so make them so.  Inside
   of a task or function, ports are REGs. */
port_spec
	: INPUT xrange
		{ if (in_tf)
		    $$ = current_spec = make_reg_spec ($2);
		  else
		    $$ = current_spec = make_net_spec (default_net_type, $2, NULL_TREE);
		  PORT_INPUT_ATTR ($$) = 1;
		}
	| OUTPUT xrange
		{ function_error;
		  if (in_tf)
		    $$ = current_spec = make_reg_spec ($2);
		  else
		    $$ = current_spec = make_net_spec (default_net_type, $2, NULL_TREE);
		  PORT_OUTPUT_ATTR ($$) = 1;
		}
	| INOUT xrange
		{ function_error;
		  if (in_tf)
		    $$ = current_spec = make_reg_spec ($2);
		  else
		    $$ = current_spec = make_net_spec (default_net_type, $2, NULL_TREE);
		  PORT_INPUT_ATTR ($$) = 1;
		  PORT_OUTPUT_ATTR ($$) = 1;
		}
	;

port_decl_identifiers
	: IDENTIFIER
		{ $$ = make_decl (check_port ($1), current_spec, NULL_TREE, NULL_TREE); }
	| port_decl_identifiers ',' IDENTIFIER
		{ yyerrok;
		  $$ = chainon (make_decl (check_port ($3), current_spec, NULL_TREE, NULL_TREE), $1);
		}
	| error
	| port_decl_identifiers error
	| port_decl_identifiers error IDENTIFIER
		{ yyerrok; }
	| port_decl_identifiers ',' error
	;

reg_spec
	: REG xrange
		{ current_spec = make_reg_spec ($2); }
	| INTEGER xrange
		{ if (!$2)
		    syn_warning ("Integer Range");
		  current_spec = make_integer_spec ($2);
		}
	| TIME xrange
		{ syn_warning ("TIME");
		  current_spec = make_time_spec ($2);
		}
	;

real_spec
	: REAL
		{ current_spec = make_real_spec (NULL_TREE); }
	;

event_spec
	: EVENT
		{ syn_warning ("EVENT");
		  current_spec = make_event_spec (); }
	;

setnetspec
	: /* empty */
		{ lval_type = LVAL_NEW_NET; }
	;

net_declaration
	: net_spec setnetspec decl_identifiers sc
		{ BLOCK_DECL (current_scope) =
			chainon ($3, BLOCK_DECL (current_scope));
 			lval_type = LVAL_REG;
		  current_delay = NULL_TREE;
		}
	| trireg_spec setspec decl_identifiers sc
	| net_spec setnetspec list_of_net_assignments sc
		{ lval_type = LVAL_REG; 
		  MODULE_ASSIGNMENTS (current_module) =
			chainon ($3, MODULE_ASSIGNMENTS (current_module));
		  current_delay = NULL_TREE;
		}
	;

net_spec
	: nettype expandrange_o delay_o
		{ $$ = current_spec = make_net_spec ($1, $2, $3);
		  current_delay = $3;
		}
	;

trireg_spec
	: TRIREG
		{ syn_warning ("TRIREG net");
		  sorry ("TRIREG net type is not supported in this version");
		}
	  charge_strength_o expandrange_o delay_o
		{ $$ = error_mark_node; }
	;

nettype
	: NETTYPE
	| SUPPLY0
		{ syn_warning ("SUPPLY0 net"); }
	| SUPPLY1
		{ syn_warning ("SUPPLY1 net"); }
	;

/*
NETTYPE is one of the following keywords
	wire	tri	  tri1	  supply0
	wand	triand   tri0	  supply1
	wor	 trior	trireg
*/

expandrange_o
	: range
	| SCALARED range
		{ sorry ("SCALARED keyword not supported; all nets treated as vectored");
		  NET_SCALARED_ATTR ($2) = 1;
		  $$ = $2;
		}
	| VECTORED range
		{ sorry ("VECTORED keyword is not supported; all nets treated as vectored");
		  NET_VECTORED_ATTR ($2) = 1;
		  $$ = $2;
		}
	| /* empty */
		{ $$ = NULL_TREE; }
	;

continuous_assign
	: assign_spec setspec
		{ lval_type = LVAL_NET; }
	  list_of_assignments sc
		{ lval_type = LVAL_REG; 
		  MODULE_ASSIGNMENTS (current_module) =
			chainon ($4, MODULE_ASSIGNMENTS (current_module));
		}
	;

assign_spec
	: ASSIGN drive_strength_o delay_o
		{ current_delay = $3;   /* strength not supported */
		  $$ = NULL;
		}
	;

list_of_net_assignments
	: net_assignment
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| list_of_net_assignments ',' net_assignment
		{ yyerrok;
		  $$ = tree_cons ($3, NULL_TREE, $1);
		}
	| error
	| list_of_net_assignments error
	| list_of_net_assignments error net_assignment
		{ yyerrok; }
	| list_of_net_assignments ',' error
	;

net_assignment
	: IDENTIFIER '='	{ $<ltype>$ = stmt_lineno; }
	  expression
		{ $$ = build_cont_assign (
				check_lval ($1, lval_type, current_spec),
				$4, $<ltype>3, current_delay, current_delay!=0);
		}

	;

list_of_assignments
	: cont_assignment
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| list_of_assignments ',' cont_assignment
		{ yyerrok;
		  $$ = tree_cons ($3, NULL_TREE, $1);
		}
	| error
	| list_of_assignments error
	| list_of_assignments error cont_assignment
		{ yyerrok; }
	| list_of_assignments ',' error
	;

cont_assignment
	: lvalue '='	{ $<ltype>$ = stmt_lineno; }
	  expression
		{ $$ = build_cont_assign ($1, $4, $<ltype>3, current_delay,
			 current_delay!=0); }
	;
parameter_override
	: DEFPARAM list_of_param_overrides sc
		{ syn_warning ("DEFPARAM"); }
	;

list_of_param_overrides
	: identifier '=' constant_expression
		{ MODULE_DEFPARAMS (current_module) = chainon (
			build_stmt (ASSIGN_STMT, stmt_lineno, $1, $3),
			MODULE_DEFPARAMS (current_module));
		}
	| list_of_param_overrides ',' identifier '=' constant_expression
		{
		  MODULE_DEFPARAMS (current_module) = chainon (
			build_stmt (ASSIGN_STMT, stmt_lineno, $3, $5),
			MODULE_DEFPARAMS (current_module));
		}
	;

reg_decl_identifiers
	: reg_decl_identifier
	| reg_decl_identifiers ',' reg_decl_identifier
		{ yyerrok;
		  $$ = chainon ($3, $1);
		}
	| error
	| reg_decl_identifiers error
	| reg_decl_identifiers error reg_decl_identifier
		{ yyerrok; }
	| reg_decl_identifiers ',' error
	;

reg_decl_identifier
	: IDENTIFIER
		{ $$ = make_decl (check_reg ($1), current_spec, NULL_TREE, NULL_TREE); }
	| IDENTIFIER '[' constant_expression ':' constant_expression ']'
		{ $$ = make_decl (check_reg ($1), current_spec, $3, $5);
		  syn_warning ("REG/INTEGER arrays");
		}
	;

non_reg_decl_identifiers
	: IDENTIFIER
		{ $$ = make_decl (check_non_reg ($1), current_spec, NULL_TREE, NULL_TREE); }
	| non_reg_decl_identifiers ',' IDENTIFIER
		{ $$ = chainon (make_decl (check_non_reg ($3), current_spec, NULL_TREE, NULL_TREE), $1); }
	;

decl_identifiers
	: IDENTIFIER
		{ $$ = make_decl (check_net ($1), current_spec, NULL_TREE, NULL_TREE); }
	| decl_identifiers ',' IDENTIFIER
		{ $$ = chainon (make_decl (check_net ($3), current_spec, NULL_TREE, NULL_TREE), $1); }
	;

charge_strength_o
	: /* empty */
		{ $$ = NULL; }
	| '(' SMALL rp
		{ $$ = NULL; }
	| '(' MEDIUM rp
		{ $$ = NULL; }
	| '(' LARGE rp
		{ $$ = NULL; }
	;

drive_strength_o
	: /* empty */
		{}
	| drive_strength
		{
warning ("Strength specification is not supported and is ignored", NULL_CHAR, NULL_CHAR);
		}

	;

drive_strength
	: '(' strength0 ',' strength1 rp
		{ $$ = NULL; }
	| '(' strength1 ',' strength0 rp
		{ $$ = NULL; }
	;

strength0
	: STRENGTH0
	| SUPPLY0 /* Should be part of STRENGTH0, but used as keyword elsewhere */
	;

strength1
	: STRENGTH1
	| SUPPLY1 /* Should be part of STRENGTH0, but used as keyword elsewhere */
	;

/*
STRENGTH0 is one of the following keywords:
	supply0   strong0   pull0   weak0   highz0

STRENGTH1 is one of the following keywords:
	supply1   strong1   pull1   weak1   highz1

*/

xrange
	: /* empty */
		{ $$ = NULL_TREE;}
	| range
	;

range
	: '[' constant_expression ':' constant_expression rb
		{ TREE_OPERAND (range_handle, 0) = $2;
		  TREE_OPERAND (range_handle, 1) = $4;
		  $$ = range_handle;
		}
/*	| '[' constant_expression ':' constant_expression error
		{ TREE_OPERAND (range_handle, 0) = $2;
		  TREE_OPERAND (range_handle, 1) = $4;
		  $$ = range_handle;
		} */
	;

/*
A.3 Primitive instances
*/
gate_declaration
        : gatetype
                { current_gatetype = $1;
		  in_instantiation = 1;
		}
          gate_description
		{ in_instantiation = 0; }
        ;

/* The keywords have been removed (except OR) while gate support is
   done externally.  Reenable them when gate support is brought in. */
gatetype
        : LOGIC
        /* is a LOGIC but is also a keyword for event expressions */
		| OR
                { $$ = GATE_OR_TYPE; }
        | BUFFER
                { syn_warning ("Buffer gate"); }
        | BUFIF
                { syn_warning ("Bufif gate"); }
        | CMOS
                { syn_warning ("CMOS gate"); }
        | PULL
                { syn_warning ("PULL gate"); }
        | TRAN
                { syn_warning ("TRAN gate"); }
        ;
/*
GATETYPE is one of the following keywords
        and     buf       nmos   pullup  tran
        nand   not        pmos   pulldown   tranif0
        nor     bufif0   cmos                           tranif1
        or       bufif1   rnmos                    rtran
        xor     notif0   rpmos                     rtranif0
        xnor   notif1   rcmos                      rtranif1
*/

gate_description
        :       { current_delay = NULL_TREE; }
          gate_instance_clist sc
/*        | drive_strength delay_o
                { current_delay = $2; }
          gate_instance_clist sc */
        | delay
                { current_delay = $1; }
          gate_instance_clist sc
        ;

gate_instance_clist
        : gate_instance
        | gate_instance_clist ',' gate_instance
        ;

gate_instance
        : IDENTIFIER '(' terminal_clist rp
                { $$ = build_gate_instance (current_gatetype, input_filename, lineno,
                        $1, nreverse ($3), current_delay, NULL_TREE);
                  MODULE_INSTANCES (current_module) = chainon ($$,
                        MODULE_INSTANCES (current_module));
                }
        | '(' terminal_clist rp
                { $$ = build_gate_instance (current_gatetype, input_filename, lineno,
                        NULL_TREE, nreverse ($2), current_delay, NULL_TREE);
                  MODULE_INSTANCES (current_module) = chainon ($$,
                        MODULE_INSTANCES (current_module));
                }
    ;



UDP_or_module_instantiation
	: IDENTIFIER /* drive_strength_o */ xparameter_value_assignment
		{ current_instance_module = $1;
		  current_instance_param = $2;
		  in_instantiation = 1;
		}
	  module_instance_clist sc
		{ current_instance_module = NULL_TREE;
		  current_instance_param = NULL_TREE;
		  in_instantiation = 0;
		}

	/* OR is a keyword, treat it seperately */
/*	| OR xparameter_value_assignment
		{ current_instance_module = get_identifier ("or");
		  current_instance_param = $2;
		}
	  module_instance_clist sc
		{ current_instance_module = NULL_TREE;
		  current_instance_param = NULL_TREE;
		}
*/
/*	| IDENTIFIER drive_strength_o delay_o UDP_instance_clist sc */
	;
/*	: IDENTIFIER UDP_instance_clist sc
		{ drive[0] = drive[1] = 0;
		  $$ = 0;
		}
	| IDENTIFIER delay UDP_instance_clist sc
		{ drive[0] = drive[1] = 0;
		  $$ = 0;
		}
	: IDENTIFIER drive_strength_o delay_o UDP_instance_clist sc
		{ $$ = 0; }
	; */

xparameter_value_assignment
	: /* empty */
		{ $$ = NULL; }
	| '#' constant_expression
		{ $$ = build_tree_list ($2, NULL_TREE); }
	| '#' '(' constant_expression_clist ')'
		{ $$ = nreverse ($3); }
	| '#' '(' mintypmax_expression ')'
		{ $$ = build_tree_list ($3, NULL_TREE); }
	| '#' '(' mintypmax_expression ',' mintypmax_expression')'
		{ $$ = build_tree_list ($5, NULL_TREE);
		  $$ = tree_cons ($3, NULL_TREE, $$);
		}
	| '#' '(' mintypmax_expression ',' mintypmax_expression ',' mintypmax_expression ')'
		{ $$ = build_tree_list ($7, NULL_TREE);
		  $$ = tree_cons ($5, NULL_TREE, $$);
		  $$ = tree_cons ($3, NULL_TREE, $$);
		}
	;

constant_expression_clist
	: constant_expression
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| constant_expression_clist ',' constant_expression
		{ $$ = tree_cons ($3, NULL_TREE, $1); }
	;

/*
UDP_instance_clist
	: UDP_instance
	| UDP_instance_clist ',' UDP_instance
	;
*/

/* UDP_instance
	: IDENTIFIER '(' xterminal_clist rp
	| '(' terminal_clist rp
		{ $$ = $2; }
	;
*/

/*
xterminal_clist
	: 
		{ $$ = NULL_TREE; }
	| terminal_clist
	;
*/

terminal_clist
	: expression
             { $$ = build_tree_list ($1, NULL_TREE); }
	| terminal_clist ',' expression
      		{ $$ = tree_cons ($3, NULL_TREE, $1); yyerrok; }

	| error
	| terminal_clist error
	| terminal_clist error expression
		{ yyerrok; }
	| terminal_clist ',' error
	;

/*
module_instantiation
	: name_of_module module_instance_clist sc 
	;
*/

module_instance_clist
	: module_instance
	| module_instance_clist ',' module_instance
		{ yyerrok; }
	| error
	| module_instance_clist error
	| module_instance_clist error module_instance
		{ yyerrok; }
	| module_instance_clist ',' error
	;

module_instance
	: IDENTIFIER '(' list_of_module_connections_o rp
		{ $$ = build_stmt (INSTANCE_NODE, lineno, check_non_reg ($1),
				current_instance_module,
				nreverse ($3), current_instance_param);
		  MODULE_INSTANCES (current_module) = chainon ($$,
				MODULE_INSTANCES (current_module));
		//  make_block_decl ($1, current_module, NULL_TREE);
		}
	| '(' list_of_module_connections_o rp
		{ $$ = build_stmt (INSTANCE_NODE, lineno, NULL_TREE,
				current_instance_module,
				nreverse ($2), current_instance_param);
		  MODULE_INSTANCES (current_module) = chainon ($$,
				MODULE_INSTANCES (current_module));
		}
	;

list_of_module_connections_o
	: module_port_connection_clist
	| named_port_connection_clist
	;

module_port_connection_clist
	: module_port_connection
	| module_port_connection_clist ',' module_port_connection
		{ yyerrok;
		  $$ = chainon ($3, $1);
		}
	| error
	| module_port_connection_clist ',' error
	;

module_port_connection
	: expression
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| /* empty */
		{ $$ = build_tree_list (NULL_TREE, NULL_TREE); }
	;

named_port_connection_clist
	: named_port_connection
	| named_port_connection_clist ',' named_port_connection
		{ $$ = chainon ($3, $1); }
	;

named_port_connection
	: '.' IDENTIFIER '(' expression rp
		{ $$ = build_tree_list ($4, $2); }

	| '.' IDENTIFIER '(' rp		{ $$ = NULL; }
	;
/*
A.5 Behavioral Statements
*/

initial_statement
	: INITIAL
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("INITIAL"); }
	  statement
		{ $$ = build_stmt (INITIAL_BLOCK, $<ltype>2, $3); }
	;

always_statement
	: ALWAYS		{ $<ltype>$ = stmt_lineno; }
	  statement
		{ $$ = build_stmt (ALWAYS_BLOCK, $<ltype>2, $3); }
	;

statement_or_null
	: statement
	| sc
		{ $$ = build_stmt (NULL_STMT, lineno); }
	;

statement
	: assignment sc
/*	| STOP_STMT sc */

	| IF			{ $<ltype>$ = stmt_lineno; }
	  '(' expression rp statement_or_null elsepart
		{ $$ = build_stmt (IF_STMT, $<ltype>2, $4, $6, $7); }

	| 	{ $<ttype>$ = current_case; }
	  case
		{ current_case = $2; }
	  '(' expression rp case_item_plist ENDCASE
		{ STMT_CASE_EXPR ($2) = $5;
		  STMT_CASE_LIST ($2) = nreverse ($7);
		  current_case = $<ttype>1;
		  $$ = $2;
		}

	| FOREVER
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("FOREVER"); }
	  statement
		{ $$ = build_stmt (FOREVER_STMT, $<ltype>2, $3); }

	| REPEAT
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("REPEAT"); }
	  '(' expression rp statement
		{ $$ = build_stmt (REPEAT_INIT_STMT, $<ltype>2, $6, $4);
		  tmp_tree = build_stmt (REPEAT_STMT, $<ltype>2, $$, $4);
		  chainon ($6, tmp_tree);
		}

	| WHILE
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("WHILE"); }
	  '(' expression rp statement
		{ $$ = build_stmt (WHILE_STMT, $<ltype>2, $6, $4); }

	| FOR			{ $<ltype>$ = stmt_lineno; }
	  '(' assignment sc  expression sc assignment rp statement
		{ stmt_lineno = $<ltype>2;
		  tmp_tree = build_stmt (FOR_STMT, $<ltype>2, $10, $8, $6);
		  STMT_SURROGATE_ATTR (tmp_tree) = 1;
		  $$ = build_stmt (FOR_STMT, $<ltype>2, $10, $4, $6);
		  TREE_CHAIN (tmp_tree) = $$;	/* Fix this in pass 3 */
		  chainon ($10, tmp_tree);	/* at end of statement sequence */
		}

	| delay_control
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("delay control");
		  function_error;
		}
	  statement_or_null
		{ $$ = build_stmt (DELAY_STMT, $<ltype>2, $3, $1); }

	| event_control
		{ stmt_lineno = lineno;
		  function_error;
		}
	  statement_or_null
		{ STMT_BODY ($1) = $3;
		  $$ = $1;
		}

	| lvalue '='
		{ $<ltype>$ = stmt_lineno;
		  function_error;
		}
	  delay_control expression sc
		{ $$ = build_stmt (ASSIGN_DELAY_STMT, $<ltype>3, $1,
			implicit_conversion ($1, $5),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, $<ltype>3, $1, $4));
		}

	| lvalue '='
		{ $<ltype>$ = stmt_lineno;
		  function_error;
		}
	  event_control expression sc
		{ $$ = build_stmt (ASSIGN_EVENT_STMT, $<ltype>3, $1,
			implicit_conversion ($1, $5),
			NULL_TREE, NULL_TREE, $4);
		}

	| lvalue LE
		{ $<ltype>$ = stmt_lineno;
		  function_error;
		}
	  expression sc
		{ $$ = build_stmt (ASSIGN_NONBLK_DELAY_STMT, $<ltype>3, $1,
			implicit_conversion ($1, $4),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, $<ltype>3, $1, integer_zero_node));
		}

	| lvalue LE
		{ $<ltype>$ = stmt_lineno;
		  function_error;
		}
	  delay_control expression sc
		{ $$ = build_stmt (ASSIGN_NONBLK_DELAY_STMT, $<ltype>3, $1,
			implicit_conversion ($1, $5),
			NULL_TREE, NULL_TREE,
			build_stmt (DELAY_STMT, $<ltype>3, $1, $4));
		}

	| lvalue LE
		{ $<ltype>$ = stmt_lineno;
		  function_error;
		}
	  event_control expression sc
		{ $$ = build_stmt (ASSIGN_NONBLK_EVENT_STMT, $<ltype>3, $1,
			implicit_conversion ($1, $5),
			NULL_TREE, NULL_TREE, $4);
		}

/*	| lvalue LE delay_control expression sc
		{ $$ = build_stmt (ASSIGN_NOBLOCK_STMT, $1, $4, $3); }

	| lvalue LE event_control expression sc
		{ $$ = build_stmt (ASSIGN_NOBLOCK_STMT, $1, $4, $3); }
*/
	| WAIT_
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("WAIT");
		  function_error;
		}
	  '(' conditional_expression rp statement_or_null
		{ $$ = build_stmt (WAIT_STMT, $<ltype>2, $6, $4); }

	| ARROW_
		{ $<ltype>$ = stmt_lineno;
		  syn_warning ("Event trigger");
		  function_error;
		}
	  identifier sc
		{ $$= build_stmt (ARROW_STMT, $<ltype>2, check_named_event ($3)); }

	| seq_block
	| par_block
	| task_enable
	| system_task_enable
	| DISABLE		{ $<ltype>$ = stmt_lineno; }
	  identifier sc
		{ $$ = build_stmt (DISABLE_STMT, $<ltype>2, $3);
		  HIERARCHICAL_ATTR ($$) = HIERARCHICAL_ATTR ($3);
		}

	| ASSIGN
		{ $<ltype>$ = stmt_lineno; }
	  lvalue '=' expression sc
		{ $$ = build_stmt (ASSIGN_PROC_STMT, $<ltype>2, $3, $5, NULL_TREE,
			NULL_TREE); }

	| DEASSIGN
		{ $<ltype>$ = stmt_lineno; }
	  lvalue sc
		{ $$ = build_stmt (DEASSIGN_STMT, $<ltype>2, $3); }

	| FORCE			{ $<ltype>$ = stmt_lineno;
				  lval_type = LVAL_REG_NET; }
	  lvalue '=' expression sc
		{ syn_warning ("FORCE");
		  $$ = build_stmt (FORCE_STMT, $<ltype>2, $3, $5, NULL_TREE,
			NULL_TREE, NULL_TREE, NULL_TREE);
		  lval_type = LVAL_REG;
		}

	| RELEASE		{ $<ltype>$ = stmt_lineno;
				  lval_type = LVAL_REG_NET; }
	  lvalue sc
		{ syn_warning ("RELEASE");
		  $$ = build_stmt (RELEASE_STMT, $<ltype>2, $3);
		  lval_type = LVAL_REG;
		}

 /*   | error sc		  { printf("error sc \n"); } */
	;

elsepart
	: /* empty */		%prec IF
		{ $$ = NULL; }
	| ELSE statement_or_null
		{ $$ = $2; }
	;

case
	: CASE
		{ $$ = build_stmt (CASE_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
	| CASEZ
		{ $$ = build_stmt (CASEZ_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
	| CASEX
		{ $$ = build_stmt (CASEX_STMT, stmt_lineno, NULL_TREE, NULL_TREE, NULL_TREE); }
	;

/*
delay_or_event_control
	: delay_control
	| event_control
	;
*/

assignment
	: lvalue '='	{ $<ltype>$ = stmt_lineno; }
	  expression
		{ $$ = build_stmt (ASSIGN_STMT, $<ltype>3, $1,
				implicit_conversion ($1, $4)); }
	;

case_item_plist
	: case_expr
	| case_item_plist case_expr
		{ yyerrok;
		  $$ = chainon ($2, $1);
		}
	| error
	| case_item_plist error
	;

case_expr
	: expression_clist ':' statement_or_null
		{ $$ = build_tree_list ($1, $3); }

	| DEFAULT ':' statement_or_null
		{ if (STMT_CASE_DEFAULT (current_case))
		    error ("More than one 'DEFAULT' within CASE", NULL_CHAR, NULL_CHAR);
		  STMT_CASE_DEFAULT (current_case) = $3;
		}
	| DEFAULT statement_or_null
		{ if (STMT_CASE_DEFAULT (current_case))
		    error ("More than one 'DEFAULT' within CASE", NULL_CHAR, NULL_CHAR);
		  STMT_CASE_DEFAULT (current_case) = $2;
		}
	;

expression_clist
	: expression
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| expression_clist ',' expression
		{ $$ = tree_cons ($3, NULL_TREE, $1); }
	| error
	| expression_clist ',' error
	;

xexpression_clist
	: xexpression
		{ if( $1 != NULL_TREE ) {
		    $$ = build_tree_list ($1, NULL_TREE); 
		  } else {
		    $$ = $1;
		  }
		}
	| xexpression_clist ',' xexpression
		{ $$ = tree_cons ($3, NULL_TREE, $1); }
/*	| error
*	| xexpression_clist error
*	| xexpression_clist error xexpression
*	| xexpression_clist ',' error  */
	;

xexpression
	: /* empty */
		{ $$ = NULL_TREE; }
	| expression
	;

seq_block
	: BEGIN
		{ $$ = build_stmt (BEGIN_STMT, stmt_lineno); }
	  statement_list END
		{ STMT_BODY ($<ttype>2) = nreverse
			(chainon (build_stmt (END_STMT, stmt_lineno), $3));
		  $$ = $<ttype>2;
		}
	| BEGIN ':' IDENTIFIER
		{ tmp_tree = make_node (NAMED_BLOCK);
		  BLOCK_DOWN (current_scope) = chainon (tmp_tree, BLOCK_DOWN (current_scope));
		  BLOCK_UP (tmp_tree) = current_scope;
		  BLOCK_NAME (tmp_tree) = $3;
		  BLOCK_BODY (tmp_tree) = NULL_TREE;
                  BLOCK_CONTEXT_LIST (tmp_tree) = NULL;
		  make_block_decl (check_block ($3), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  push_scope ();
		  $$ = build_stmt (BEGIN_NAMED_STMT, stmt_lineno,
			NULL_TREE, tmp_tree);
		}
	  block_declaration_list statement_list END
		{ tmp_tree = build_stmt (END_NAMED_STMT, stmt_lineno,
			NULL_TREE, STMT_BLOCK ($<ttype>4));
		  /* STMT_BEGIN_NAMED_END ($<ttype>4) = tmp_tree; */
		  STMT_BODY ($<ttype>4) = nreverse (chainon (tmp_tree, $6));
		  current_scope = pop_scope ();
		  $$ = $<ttype>4;
		}
	;

par_block
	: FORK
		{ $$ = build_stmt (FORK_STMT, stmt_lineno); }
	  statement_tree_list JOIN
		{ STMT_BODY ($<ttype>2) = chainon  /* thread JOIN to BEGINNING */
			(build_stmt (JOIN_STMT, stmt_lineno, NULL_TREE),
			nreverse ($3));
		  $$ = $<ttype>2;
		}
	| FORK ':' IDENTIFIER
		{ tmp_tree = make_node (NAMED_BLOCK);
		  BLOCK_DOWN (current_scope) = chainon (tmp_tree, BLOCK_DOWN (current_scope));
		  BLOCK_UP (tmp_tree) = current_scope;
		  BLOCK_NAME (tmp_tree) = $3;
		  BLOCK_BODY (tmp_tree) = NULL_TREE;
		  make_block_decl (check_block ($3), current_scope, tmp_tree);
		  current_scope = tmp_tree;
		  push_scope ();
		  $$ = build_stmt (FORK_NAMED_STMT, stmt_lineno,
			NULL_TREE, tmp_tree);
		}
	  block_declaration_list statement_tree_list JOIN
		{ tmp_tree = build_stmt (JOIN_NAMED_STMT, stmt_lineno,
			NULL_TREE, STMT_BLOCK ($<ttype>4));
		  /* STMT_BEGIN_NAMED_END ($<ttype>4) = tmp_tree; */
		  STMT_BODY ($<ttype>4) = chainon (tmp_tree, nreverse ($6));
		  current_scope = pop_scope ();
		  $$ = $<ttype>4;
		}
	;

statement_list
	: /* empty */
		{ $$ = NULL; }
	| statement_list statement
		{ yyerrok;
		  $$ = chainon ($2, $1);
		}
	| statement_list error
	;

statement_tree_list
	: /* empty */
		{ $$ = NULL; }
	| statement_tree_list statement
		{ yyerrok;
		  $$ = tree_cons ($2, NULL_TREE, $1);
		}
	| statement_tree_list error
	;

block_declaration_list
	: /* empty */
		{ $$ = NULL; }
	| block_declaration_list block_declaration
	;

block_declaration
	: parameter_declaration
	| static_declaration
	;

task_enable
	: identifier sc
		{ syn_warning ("task enable");
		  function_error;
		  $$ = build_stmt (TASK_STMT, lineno, IDENTIFIER_POINTER ($1), NULL_TREE);
		  HIERARCHICAL_ATTR ($$) = HIERARCHICAL_ATTR ($1);
		}
	| identifier '(' expression_clist rp sc
		{ syn_warning ("task enable");
		  $$ = build_stmt (TASK_STMT, lineno, IDENTIFIER_POINTER ($1), nreverse ($3));
		  HIERARCHICAL_ATTR ($$) = HIERARCHICAL_ATTR ($1);
		}
	;

system_task_enable
	: SYSTEM_IDENTIFIER sc
		{ $$ = build_stmt (SYSTASK_STMT, lineno, IDENTIFIER_POINTER ($1), NULL_TREE); }
	| SYSTEM_IDENTIFIER
		{ in_systask = 1; }
	  '(' xexpression_clist rp sc
		{ $$ = build_stmt (SYSTASK_STMT, lineno, IDENTIFIER_POINTER ($1), nreverse ($4));
		  in_systask = 0;
		} 
	;

lvalue
	: lval_normal
		{ stmt_lineno = lineno; }
	| lval_concat
		{ stmt_lineno = lineno; }
	;

lval_normal
	: identifier
		{ $$ = check_lval ($1, lval_type, current_spec); }
	| identifier '[' expression ']'
		{ $$ = build_bit_ref (
			check_lval_nocheck ($1, lval_type, current_spec), $3); }
	| identifier '[' expression ':' expression ']'
		{ $$ = build_part_ref (
			check_lval_nocheck ($1, lval_type, current_spec), $3, $5); }
	;

/*
A.6 Specify Section
*/

specify_block
	: SPECIFY
	{
	}
	  specify_stuff
	{}
	;

/* If specify blocks are disabled, The rest will be eaten by the parser */
specify_stuff
	: specify_items ENDSPECIFY
	{}
	;

specify_items
	: /* empty */
		{ $$ = NULL; }
	| specify_items specify_item 
		{ yyerrok;
		    $$ = NULL;	
		//  $$ = chainon ($2, $1);
		}
	| specify_items error
	;
specify_item	
	: specparam_declaration
	| path_declaration
	| system_timing_check
	{
	    if( $1!=NULL_TREE ) { 
		STMT_SOURCE_LINE($1) = stmt_lineno;
		STMT_SOURCE_FILE($1) = input_filename;
		MODULE_SPECDEFS(current_module) = 
			chainon(MODULE_SPECDEFS(current_module),$1);
	    }
	}
/*
	These are implemented using the path_declaration

	| level_sensitive_path_declaration
	| edge_sensitive_path_declaration
	| sdpd
*/
	;
specparam_declaration
	: SPECPARAM 
		{ current_spec = make_param_spec (NULL_TREE); }
	list_of_param_assignments sc
	{
		specify_save_param_list( current_module, $3 );
	 }
	;

path_declaration
	: specify_if_clause path_description '=' path_delay_value sc
	{
		PATH_CONDITION($2) = $1;
	 	PATH_DELAYS($2) = $4;	
		MODULE_SPECDEFS(current_module) = 
			chainon(MODULE_SPECDEFS(current_module),check_path($2));
	}
	;
path_description
	/* all types of path descriptors are bundled together - its was the
	   only way I could get bision to parse correctly */
	: '(' 
	  list_of_path_inputs path_operator '('
	  list_of_path_outputs ':'
	  data_source_expression rp rp 
	{
		$$ = build_path( nreverse($2), nreverse($5), $7, NULL_TREE, NULL_TREE,
			$3, 0, 0 );
	}
	| '(' edge_identifier
	  list_of_path_inputs path_operator '('
	  list_of_path_outputs ':'
	  data_source_expression rp rp 
	{
		$$ = build_path( nreverse($3), nreverse($6), $8, NULL_TREE, NULL_TREE,
			$4, $2, 0 );
	}
	| '('
	  list_of_path_inputs path_operator '('
	  list_of_path_outputs polarity_operator ':'
	  data_source_expression rp rp 
	{
		$$ = build_path( nreverse($2), nreverse($5), $8, NULL_TREE, NULL_TREE,
			$3, 0, $6 );
	}
	| '(' edge_identifier
	  list_of_path_inputs path_operator '('
	  list_of_path_outputs polarity_operator ':'
	  data_source_expression rp rp 
	{
		$$ = build_path( nreverse($3), nreverse($6), $9, NULL_TREE, NULL_TREE,
			$4, $2, $7 );
	}
	| '(' list_of_path_inputs  path_operator
					 list_of_path_outputs rp
	{
		$$ = build_path( nreverse($2), nreverse($4), NULL_TREE, NULL_TREE, NULL_TREE,
			$3, 0, 0 );
	}
	| '(' list_of_path_inputs  polarity_operator path_operator
					 list_of_path_outputs rp
	{
		$$ = build_path( nreverse($2), nreverse($5), NULL_TREE, NULL_TREE, NULL_TREE,
			$4, 0, $3 );
	}
	;
list_of_path_inputs
	: specify_input_terminal_descriptor 
	| list_of_path_inputs ','specify_input_terminal_descriptor 
		{ yyerrok;
		  $$ = chainon ($3, $1);
		}
	| error
	| list_of_path_inputs error specify_input_terminal_descriptor
		{ yyerrok; }
	| list_of_path_inputs ',' error
	;
list_of_path_outputs
	: specify_output_terminal_descriptor 
	| list_of_path_outputs ','specify_output_terminal_descriptor 
		{ yyerrok;
		  $$ = chainon ($3, $1);
		}
	| error
	| list_of_path_outputs error specify_output_terminal_descriptor
		{ yyerrok; }
	| list_of_path_outputs ',' error
	;
specify_input_terminal_descriptor
	: IDENTIFIER
	{
		$$ = build_tree_list( check_input_port($1), NULL_TREE );
	}
	| IDENTIFIER '[' constant_expression rb
	{
		$$ = build_tree_list( build_bit_ref (check_input_port($1), $3), NULL_TREE ); 
	}
	| IDENTIFIER '[' constant_expression ':' constant_expression rb
	{
		$$ = build_tree_list( build_part_ref (check_input_port($1), $3, $5), NULL_TREE ); 
	}
	;
specify_output_terminal_descriptor
	: IDENTIFIER
	{
		$$ = build_tree_list( check_output_port($1), NULL_TREE );
	}
	| IDENTIFIER '[' constant_expression rb
	{
		$$ = build_tree_list( build_bit_ref (check_output_port($1), $3), NULL_TREE ); 
	}
	| IDENTIFIER '[' constant_expression ':' constant_expression rb
	{
		$$ = build_tree_list( build_part_ref (check_output_port($1), $3, $5), NULL_TREE ); 
	}
	;
path_delay_value
	: path_delay_expression
	| '(' path_delay_expression ',' path_delay_expression rp
	{
		$$ = chainon( $2, $4 );
	}
	| '(' path_delay_expression ',' path_delay_expression ','
	      path_delay_expression rp  
	{
		tree t;
		t = chainon( $2, $4 );
		$$ = chainon( t, $6 );
	}
	| '(' path_delay_expression ',' path_delay_expression ','
	      path_delay_expression ',' path_delay_expression ','
	      path_delay_expression ',' path_delay_expression rp 
	{
		tree t;
		t = chainon( $2, $4 );
		t = chainon( t, $6 );
		t = chainon( t, $8 );
		t = chainon( t, $10 );
		$$ = chainon( t, $12 );
	}
	;
path_delay_expression
	: mintypmax_expression
	{
		$$ = build_tree_list($1,NULL_TREE);
	}
	;	
polarity_operator
	:	'+'
	{	
		$$ = 1;
	}
	|	'-'
	{	
		$$ = -1;
	}
	;
path_operator
	:	STARARROW
	{
		$$ = FALSE;
	}
	|	EQUALARROW
	{
		$$ = TRUE;
	}
	;
conditional_port_expression
	: expression	/* according to the BNF this should be more elaborate */
		/* if further checking for conformance is required it 	
		   could be done in a later pass */
	;
data_source_expression
	: expression
	;
edge_identifier
	: POSEDGE
	{
		$$ = 1;
	}
	| NEGEDGE
	{
		$$ = -1;
	}
	;
specify_if_clause
	: /* empty */
	{
		$$ = 0;
	}
	| IF '(' conditional_port_expression rp
	{
		$$ = $3;
	}
	;

system_timing_check
	:	SETUP '(' timing_check_event ',' timing_check_event ','
			expression notify_clause rp sc 
	{
		$$ = build_timing_statement( SETUP, $3, $5, $7, NULL_TREE, $8 );
	}
	|	HOLD '(' timing_check_event ',' timing_check_event ',' 
			expression  notify_clause rp sc 
	{
		$$ = build_timing_statement( HOLD, $3, $5, $7, NULL_TREE, $8 );
	}
	|	PERIOD '(' controlled_timing_check_event ',' 
			expression notify_clause rp sc 
	{
		$$ = build_timing_statement( PERIOD, $3, NULL_TREE, $5, NULL_TREE, $6 );
	}
	|	WIDTH '(' controlled_timing_check_event ',' 
			expression  rp sc 
	{
		$$ = build_timing_statement( WIDTH, $3, NULL_TREE, $5, NULL_TREE, NULL_TREE );
	}
	|	WIDTH '(' controlled_timing_check_event ','
			expression ',' expression notify_clause  rp sc 
	{
		$$ = build_timing_statement( WIDTH, $3, NULL_TREE, $5, $7, $8 );
	}
	|	SKEW '(' timing_check_event ',' timing_check_event ',' 
			expression notify_clause rp sc 
	{
		$$ = build_timing_statement( SKEW, $3, $5, $7, NULL_TREE, $8 );
	}
	|	RECOVERY '(' controlled_timing_check_event ','  timing_check_event ','
			expression notify_clause rp sc 
	{
		$$ = build_timing_statement( RECOVERY, $3, $5, $7, NULL_TREE, $8 );
	}
	|	SETUPHOLD '(' timing_check_event ',' timing_check_event ','
			expression ',' expression notify_clause rp sc 
	{
		$$ = build_timing_statement( SETUPHOLD, $3, $5, $7, $9, $10 );
	}
	|	NOCHANGE '(' controlled_timing_check_event ','  timing_check_event ','
			expression ',' expression rp sc 
	{
		$$ = NULL_TREE;
	}
	;		
notify_clause
	:	/* EMPTY */
	{
		$$ = 0;
	}
	|	',' notify_register
	{
		$$ = $2;
	}
	;
notify_register
	:	IDENTIFIER
	;
timing_check_event
	:	timing_check_event_control timing_input_terminal_descriptor	
	{ 
		$$ = build_timing_event( $1, $2, NULL_TREE );
	}
	|	timing_check_event_control timing_input_terminal_descriptor	
		ANDANDAND timing_check_condition	
	{ 
		$$ = build_timing_event( $1, $2, $4 );
	}
	;
controlled_timing_check_event
	:	timing_check_event_control_no_empty timing_input_terminal_descriptor	
	{
		$$ = build_timing_event( $1, $2, NULL_TREE );
	}
	|	timing_check_event_control_no_empty timing_input_terminal_descriptor	
		ANDANDAND timing_check_condition	
	{
		$$ = build_timing_event( $1, $2, $4 );
	}
	;
timing_check_event_control
	: /* EMPTY */
	{
		extern unsigned int alledgeMask;
		$$ = alledgeMask;
	}
	| POSEDGE
	{
		extern unsigned int posedgeMask;
		$$ = posedgeMask;
	}
	| NEGEDGE
	{
		extern unsigned int negedgeMask;
		$$ = negedgeMask;
	}
	| EDGE '[' 
	{  	extern int enable_edge_digit;
		enable_edge_digit = TRUE; 
	}
	edge_list rb
	{ 
		extern int enable_edge_digit;
		enable_edge_digit = FALSE;
		$$ = $4;
	}
	;
timing_check_event_control_no_empty
	: POSEDGE
	{
		extern unsigned int posedgeMask;
		$$ = posedgeMask;
	}
	| NEGEDGE
	{
		extern unsigned int negedgeMask;
		$$ = negedgeMask;
	}
	| EDGE '[' 
	{  	extern int enable_edge_digit;
		enable_edge_digit = TRUE; 
	}
	edge_list rb
	{ 
		extern int enable_edge_digit;
		enable_edge_digit = FALSE;
		$$ = $4;
	}
	;
edge_list
	:	DOUBLE_DIGIT
	{
		extern unsigned int edge_mask[4][4];
		$$ = edge_mask[$1[0]][$1[1]];
	}
	|	DOUBLE_DIGIT ',' edge_list
	{
		extern unsigned int edge_mask[4][4];
		$$ = $3 | edge_mask[$1[0]][$1[1]];
	}
	|	error
	{
		$$ = 0;
	}
	;
timing_check_condition
	:	scalar_expression
	|	'(' timing_check_condition ')'
		{ $$ = $2; }
	|	'~' scalar_expression %prec UNARY
		{ $$ = build_unary_op (BIT_NOT_EXPR, $2); }
	|	scalar_expression LOGICAL_EQUALITY scalar_constant
		{ $$ = build_binary_op (EQ_EXPR, $1, $3); }
	|	scalar_expression CASE_EQUALITY scalar_constant
		{ $$ = build_binary_op (EQ_CASE_EXPR, $1, $3); }
	|	scalar_expression CASE_INEQUALITY scalar_constant
		{ $$ = build_binary_op (NE_CASE_EXPR, $1, $3); }
	|	scalar_expression LOGICAL_INEQUALITY scalar_constant
		{ $$ = build_binary_op (NE_EXPR, $1, $3); }
	;	
scalar_expression	/* expression must be a Scalar Net */
	:	IDENTIFIER	
		{ $$ = check_rval ($1); }
	;
scalar_constant		
	:	BASE_NUMBER
	{
		$$ = check_one_bit_constant($1);
	}
	|	NUMBER
	{
		$$ = check_one_bit_constant($1);
	}
	;	

timing_input_terminal_descriptor
	: identifier
	{ 
		check_input_port($1);
		$$ = check_rval ($1);
	 }
	| identifier '[' expression ']'
	{ 
		check_input_port($1);
		$$ = build_bit_ref ( check_rval_nocheck ($1), $3);
	 }
	| identifier '[' constant_expression ':' constant_expression ']'
	{ 
		check_input_port($1);
		$$ = build_part_ref ( check_rval_nocheck ($1), $3, $5);
	}
	;	


/*
A.7 Expressions
*/

lval_concat
	: '{' lval_clist '}'
		{ $$ = make_node (CONCAT_REF);
		  CONCAT_LIST ($$) = $2;
		  concat_labels ($$);
		}
	;

lval_clist
	: lval_normal
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| lval_concat
		{ $$ = build_tree_list ($1, NULL_TREE); }
	| lval_clist ',' lval_normal
		{ $$ = tree_cons ($3, NULL_TREE, $1); }
	| lval_clist ',' lval_concat
		{ $$ = tree_cons ($3, NULL_TREE, $1); }
	| error
	| lval_clist error
	| lval_clist error lval_normal
	| lval_clist error lval_concat
	| lval_clist ',' error
	;

conditional_expression
	: expression
	;

constant_expression
	: expression
		{ if (!TREE_CONSTANT_ATTR ($1)) {
		    error ("Illegal non-constant expression", NULL_CHAR, NULL_CHAR);
		    $$ = error_mark_node;
		  }
		  else
		    $$ = $1;
		}
	;

mintypmax_expression
	: expression
	| expression ':' expression ':' expression
		{ switch (delay_type) {
		    case (MIN_DELAY):  $$ = $1; break;
		    case (TYP_DELAY):  $$ = $3; break;
		    case (MAX_DELAY):  $$ = $5; break;
		    }
		}
    ;

mintypmax_expression_triplet
	: expression ':' expression ':' expression
		{ switch (delay_type) {
		    case (MIN_DELAY):  $$ = $1; break;
		    case (TYP_DELAY):  $$ = $3; break;
		    case (MAX_DELAY):  $$ = $5; break;
		    }
		}
    ;

expression
	: primary
	| '+' primary %prec UNARY
		{ $$ = $2; }
	| '-' primary %prec UNARY
		{ $$ = build_unary_op (NEGATE_EXPR, $2); }
	| '!' primary %prec UNARY
		{ $$ = build_unary_op (TRUTH_NOT_EXPR, $2); }
	| '~' primary %prec UNARY
		{ $$ = build_unary_op (BIT_NOT_EXPR, $2); }
	| '&' primary %prec UNARY
		{ $$ = build_unary_op (AND_REDUCE_EXPR, $2); }
	| REDUCTION_NAND primary %prec UNARY
		{ $$ = build_unary_op (NAND_REDUCE_EXPR, $2); }
	| '|' primary %prec UNARY
		{ $$ = build_unary_op (OR_REDUCE_EXPR, $2); }
	| REDUCTION_NOR primary %prec UNARY
		{ $$ = build_unary_op (NOR_REDUCE_EXPR, $2); }
	| '^' primary %prec UNARY
		{ $$ = build_unary_op (XOR_REDUCE_EXPR, $2); }
	| XNOR primary %prec UNARY
		{ $$ = build_unary_op (XNOR_REDUCE_EXPR, $2); }
	| expression '+' expression
		{ $$ = build_binary_op (PLUS_EXPR, $1, $3); }
	| expression '-' expression
		{ $$ = build_binary_op (MINUS_EXPR, $1, $3); }
	| expression '*' expression
		{ $$ = build_binary_op (MULT_EXPR, $1, $3); }
	| expression '/'
		{ syn_warning ("Division operator"); }
	  expression
		{ $$ = build_binary_op (DIV_EXPR, $1, $4); }
	| expression '%'
		{ syn_warning ("Modulus operator"); }
	  expression
		{ $$ = build_binary_op (MOD_EXPR, $1, $4); }
	| expression LOGICAL_EQUALITY expression
		{ $$ = build_binary_op (EQ_EXPR, $1, $3); }
	| expression LOGICAL_INEQUALITY expression
		{ $$ = build_binary_op (NE_EXPR, $1, $3); }
	| expression CASE_EQUALITY
		{ syn_warning ("Case equality"); }
	  expression
		{ $$ = build_binary_op (EQ_CASE_EXPR, $1, $4); }
	| expression CASE_INEQUALITY
		{ syn_warning ("Case inequality"); }
	  expression
		{ $$ = build_binary_op (NE_CASE_EXPR, $1, $4); }
	| expression ANDAND expression
		{ $$ = build_binary_op (TRUTH_ANDIF_EXPR, $1, $3); }
	| expression OROR expression
		{ $$ = build_binary_op (TRUTH_ORIF_EXPR, $1, $3); }
	| expression LT expression
		{ $$ = build_binary_op (LT_EXPR, $1, $3); }
	| expression LE expression
		{ $$ = build_binary_op (LE_EXPR, $1, $3); }
	| expression GT expression
		{ $$ = build_binary_op (GT_EXPR, $1, $3); }
	| expression GE expression
		{ $$ = build_binary_op (GE_EXPR, $1, $3); }
	| expression '&' expression
		{ $$ = build_binary_op (BIT_AND_EXPR, $1, $3); }
	| expression '|' expression
		{ $$ = build_binary_op (BIT_OR_EXPR, $1, $3); }
	| expression '^' expression
		{ $$ = build_binary_op (BIT_XOR_EXPR, $1, $3); }
	| expression XNOR expression
		{ $$ = build_binary_op (BIT_XNOR_EXPR, $1, $3); }
	| expression LEFT_SHIFT expression
		{ $$ = build_binary_op (LSHIFT_EXPR, $1, $3); }
	| expression RIGHT_SHIFT expression
		{ $$ = build_binary_op (RSHIFT_EXPR, $1, $3); }
	| expression '?' expression ':' expression
		{ $$ = build_cond_expr ($1, $3, $5); }
	| STRING
	;

/*
UNARY_OPERATOR is one of the following tokens:
	+	   -	   !	   ~
	&	   ~&	  |	   ^|	  ^	   ~^

BINARY_OPERATOR is one of the following tokens:
	+	   -	   *	   /	   %
	==	  !=	  ===	 !==
	&&	  ||
	<	   <=	  >	   >=
	&	   |	   ^	   ^~
	>>	  <<

STRING is text enclosed in "" and contained on one line
*/

primary
	: NUMBER
	| BASE_NUMBER
	| identifier 		%prec LOWEST
		{ $$ = check_rval ($1); }
	| identifier '[' expression ']'
		{ $$ = build_bit_ref (check_rval_nocheck ($1), $3); }
	| identifier '[' constant_expression ':' constant_expression ']'
		{ $$ = build_part_ref (check_rval_nocheck ($1), $3, $5); }
	| concatenation
	| function_call
	| '(' mintypmax_expression rp
		{ $$ = $2; }
	| '(' error rp
		{ $$ = error_mark_node; }
	;

/*
NUMBER
	Numbers can be specified in decimal, hexadecimal, octal or
	binary, may optionally start with a + or -, and can be given
	in one of two forms.  One form is a normal decimal number
	specified as a sequence of the digits 0 to 9. The other takes
	the form

		ss..s 'f nn..n

	where the characters ss..s are decimal digits and specify the
	size of the constant in terms of the exact number of bits
	that make up the constant. The size specification is
	optional.  The character f represents the base format and
	must be one of d, h, o, or b, for the bases decimal,
	hexadecimal, octal and binary respectively.  The characters
	nn..n represent the constants value, and must follow the base
	specification and be one of the following characters

	0123456789abcdefABCDEFxXzZ

	White space is allowed between ss..s and the characters 'f,
	and between 'f and nn..n.

BASE is one of the following tokens:
	'b	 'B	 'o	 'O	 'd	 'D	 'h	 'H
*/

concatenation
	: '{' expression_clist '}'
		{ $$ = make_node (CONCAT_REF);
		  CONCAT_LIST ($$) = $2;
		  concat_labels ($$);
		}
	| '{' constant_expression '{' expression_clist '}' '}'
		{ $$ = make_node (CONCAT_REP_REF);
		  CONCAT_LIST ($$) = $4;
		  CONCAT_EXPR ($$) = $2;
		  concat_labels ($$);
		}
	;

function_call
	: identifier '(' xexpression_clist rp
		{ $$ = build_function_call ($1, nreverse ($3));
		  HIERARCHICAL_ATTR ($$) = HIERARCHICAL_ATTR ($1);
                  UNKNOWN_TYPE_ATTR ($$) = 1;
		}
	| SYSTEM_IDENTIFIER '(' xexpression_clist rp
/*		{ $$ = build_function_call ($1, nreverse ($3)); } */
		{ $$ = build_nt (SYSFUNCTION_REF, IDENTIFIER_POINTER ($1),
			nreverse ($3), NULL_TREE, NULL_TREE, lineno, input_filename); }
	| SYSTEM_IDENTIFIER		%prec LOWEST
/*		{ $$ = build_function_call ($1, NULL_TREE); } */
		{ $$ = build_nt (SYSFUNCTION_REF, IDENTIFIER_POINTER ($1),
			NULL_TREE, NULL_TREE, NULL_TREE, lineno, input_filename); }
	;

/*
A.8 General
*/

identifier
	: HIERARCHY_IDENTIFIER
		{
		  syn_warning ("Hierachical name");
		/*  hierarchical_list = link_tree_cons ($1, NULL_TREE, hierarchical_list); */
		  HIERARCHICAL_ATTR ($1) = 1;
                  UNKNOWN_TYPE_ATTR ($1) = 1;
		  TREE_CONSTANT_ATTR ($1) = 1;  /* assume this is true for now */
		}

	| IDENTIFIER
	;

IDENTIFIER_or_OR
	: IDENTIFIER
	| OR
		{ $$ = get_identifier ("or"); }
	;
/*
IDENTIFIER
	An identifier is any sequence of letters, digits and the
	underscore (_) symbol, except that the first must be a letter
	or the underscore.  Upper and lower case letters are
	considered to be different (unless the upper case option is
	used when compiling).  Identifiers may be of any size, i.e.
	all characters are significant.
	Escaped identifiers start with the \ character and provide a
	means of including any of the printable ascii characters in
	an identifier.  An escaped identifier ends with white space.
	The leading \ character is not considered to be part of the
	identifier.
*/

delay_o
	: /* empty */
		{ $$ = NULL_TREE; }
	| delay
	;

delay
	: '#' NUMBER
		{ $$ = build_nt (DELAY_EXPR, $2, $2, NULL_TREE);
 		  TREE_LABEL ($$) = 1;
		}
	| '#' identifier
		{ $$ = check_rval ($2);
		  $$ = build_nt (DELAY_EXPR, $$, $$, NULL_TREE);
		  TREE_LABEL ($$) = 1;
		}
	| '#' '(' mintypmax_clist rp
		{ $$ = $3; }
/*	| '#' '(' mintypmax_clist rp
		{ $$ = build_nt (DELAY_EXPR, $3, $3, NULL_TREE);
		  TREE_LABEL ($$) = 1;
		}
*/	;

mintypmax_clist
	: mintypmax_expression
		{ $$ = build_nt (DELAY_EXPR, $1, $1, NULL_TREE);
		  TREE_LABEL ($$) = 1;
		}
	| mintypmax_expression ',' mintypmax_expression
		{ $$ = build_nt (DELAY_EXPR, $1, $3, NULL_TREE);
		  TREE_LABEL ($$) = 2;
		}
	| mintypmax_expression ',' mintypmax_expression ',' mintypmax_expression
		{ $$ = build_nt (DELAY_EXPR, $1, $3, $5);
		  TREE_LABEL ($$) = 3;
		}
	;

delay_control
	: '#' NUMBER
		{ $$ = $2; }
	| '#' identifier
		{ $$ = check_rval ($2); }
	| '#' '(' mintypmax_expression rp
		{ $$ = $3; }
	;

event_control
	: '@' { in_event = 1; }
	  identifier /* statement_or_null */
		{if (!HIERARCHICAL_ATTR ($3) &&
			TREE_CODE (IDENT_CURRENT_DECL ($3)) == EVENT_DECL)
		    $$ = build_stmt (EVENT_STMT, lineno, NULL_TREE,
			build_tree_list (IDENT_CURRENT_DECL ($3), NULL_TREE));
		  else
		    $$ = build_stmt (EVENT_STMT, lineno, NULL_TREE,
			build_tree_list (build_unary_op
			(ANYEDGE_EXPR, check_rval ($3)), NULL_TREE));
		  in_event = 0;
		}
	| '@' '(' { in_event = 1; }
	  event_expression rp /* statement_or_null */
		{ $$ = build_stmt (EVENT_STMT, lineno, NULL_TREE, $4);
		  in_event = 0; }
	;

event_expression
	: expression
		{ /*if (TREE_CODE ($1) == EVENT_DECL)
		    $$ = build_tree_list ($1, NULL_TREE);
		  else */
		    $$ = build_tree_list
			(build_unary_op (ANYEDGE_EXPR, $1), NULL_TREE);
		}
	| POSEDGE expression
		{ $$ = build_tree_list 
			(build_unary_op (POSEDGE_EXPR, $2), NULL_TREE); }
	| NEGEDGE expression
		{ $$ = build_tree_list 
			(build_unary_op (NEGEDGE_EXPR, $2), NULL_TREE); }
	| event_expression OR event_expression
		{ $$ = chainon ($1, $3); }
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

sc
	: ';'
		{ yyerrok; }
	;

%%
void
eat_block (short token)
{
  YYSTYPE lval;

  for (;;)
    if (yylex (&lval) == token)
      return;
}

