/*****************************************************************************
 * Copyright 2008, Mark Hummel
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

/* veriwellinterface.h - external interface to core simulator */

#ifndef VERIWELL_INTERFACE
#define VERIWELL_INTERFACE

#include "vtypes.h"
#include "flags.h"
#include "schedule.h"
#include "pass3.h"
#include "gates.h"
#include "acc_user.h"
#include "pli.h"
#include "systask.h"
#include "pass2.h"
#include "runtime.h"
#include "tree.h"
#include "eval.h"
#include "timescal.h"
#include "obstack.h"
#include "exec.h"
#include "scope.h"
#include "check.h"
#include "tree.h"
#include "specify.h"
#include "decl.h"
#include "copy.h"
#include "udp.h"
#include "store.h"
#include "usertask.h"
#include "trace.h"
#include "multdiv.h"
#include "strobe.h"
#include "nsched.h"
#include "verisys.h"
#include "file.h"
#include "dumpvar.h"
#include "print.h"
#include "io.h"
#include "file.h"

/***************************************************************
 * simulator exports used to create simulator structures
 * ************************************************************/

/*
 * objects
 */
using veriwell::integer_zero_node;
using veriwell::debug;
// current scope should be cleaned up
using veriwell::current_scope;
// the in_* variables are unsafe usages of globals
using veriwell::in_tf;
using veriwell::in_event;
using veriwell::in_systask;
using veriwell::in_instantiation;
using veriwell::module_list;
using veriwell::is_interactive;
// lineno unsafe usage of global 
using veriwell::lineno;
using veriwell::new_cmd_level;
using veriwell::cmd_level;
using veriwell::plusargs;
using veriwell::in_simulation;
using veriwell::var_debug_flag;
using veriwell::error_mark_node;
using veriwell::default_net_type;
using veriwell::normal_flag;
using veriwell::trace_flag;
using veriwell::alledgeMask;
using veriwell::posedgeMask;
using veriwell::negedgeMask;
using veriwell::edge_mask;
// fin is bad use of global variable
using veriwell::fin;
using veriwell::ylibext;
using veriwell::ypathList;
using veriwell::key_enable;
using veriwell::key_file;
using veriwell::key_file_name;
using veriwell::log_file;
using veriwell::log_enable;
using veriwell::log_file_name;
using veriwell::key_available;
using veriwell::log_available;
using veriwell::stmt_lineno;
// input_filename needs to be untangled, but needs a major cleanup
using veriwell::input_filename;

/*
 * types
 */
using veriwell::nbits_t;
using veriwell::Bit;
using veriwell::radii;
using veriwell::tree;
using veriwell::lineno_t;
using veriwell::tree_type;
using veriwell::tree_code;
using veriwell::lval_type;
using veriwell::File;
using veriwell::obstack_t;
using veriwell::ngroups_t;
using veriwell::LibPath_t;

/*
 * procedures
 */
using veriwell::build_stmt;
using veriwell::build_int_cst;
using veriwell::build_real_cst;
using veriwell::nreverse;
using veriwell::make_node;
using veriwell::pop_scope;
using veriwell::push_scope;
using veriwell::make_event_spec;
using veriwell::build_string;
using veriwell::get_identifier;
using veriwell::process_timescale;
using veriwell::printf_error_V;
using veriwell::printf_V;
using veriwell::parse_base_const1g;
using veriwell::obstack_alloc;
using veriwell::obstack_init;
using veriwell::obstack_free;
using veriwell::warning;
using veriwell::error;
using veriwell::sorry;

/*
 * constants used by ASTs
 */
using veriwell::RANGE_HANDLE;
using veriwell::NET_WIRE_TYPE;
using veriwell::MIN_DELAY;
using veriwell::TYP_DELAY;
using veriwell::MAX_DELAY;
using veriwell::NO_TYPE;
using veriwell::GATE_RTRAN_TYPE;
using veriwell::NET_TRIOR_TYPE;
using veriwell::NET_TRIAND_TYPE;
using veriwell::NET_TRI_TYPE;
using veriwell::GATE_NOR_TYPE;
using veriwell::GATE_NOT_TYPE;
using veriwell::GATE_RNMOS_TYPE;
using veriwell::GATE_XOR_TYPE;
using veriwell::GATE_XNOR_TYPE;
using veriwell::GATE_NMOS_TYPE;
using veriwell::GATE_AND_TYPE;
using veriwell::GATE_NAND_TYPE;
using veriwell::GATE_TRAN_TYPE;
using veriwell::GATE_RPMOS_TYPE;
using veriwell::GATE_PMOS_TYPE;
using veriwell::NET_WOR_TYPE;
using veriwell::GATE_RTRANIF1_TYPE;
using veriwell::GATE_RCMOS_TYPE;
using veriwell::NET_TRI1_TYPE;
using veriwell::GATE_RTRANIF0_TYPE;
using veriwell::STRENGTH_STRONG1_TYPE;
using veriwell::NET_TRI0_TYPE;
using veriwell::GATE_PULLDN_TYPE;
using veriwell::GATE_NOTIF1_TYPE;
using veriwell::GATE_CMOS_TYPE;
using veriwell::STRENGTH_STRONG0_TYPE;
using veriwell::NET_WAND_TYPE;
using veriwell::GATE_BUF_TYPE;
using veriwell::GATE_NOTIF0_TYPE;
using veriwell::NET_TRIREG_TYPE;
using veriwell::GATE_PULLUP_TYPE;
using veriwell::NET_SUPPLY1_TYPE;
using veriwell::GATE_TRANIF1_TYPE;
using veriwell::NET_SUPPLY0_TYPE;
using veriwell::GATE_TRANIF0_TYPE;
using veriwell::GATE_BUFIF1_TYPE;
using veriwell::GATE_BUFIF0_TYPE;
using veriwell::STRENGTH_PULL1_TYPE;
using veriwell::STRENGTH_PULL0_TYPE;
using veriwell::STRENGTH_WEAK1_TYPE;
using veriwell::STRENGTH_WEAK0_TYPE;
using veriwell::STRENGTH_HIGHZ1_TYPE;
using veriwell::STRENGTH_HIGHZ0_TYPE;
using veriwell::STRING_;
using veriwell::HEX;
using veriwell::BIN;
using veriwell::OCT;
using veriwell::DEC;
using veriwell::X;
using veriwell::ZERO;
using veriwell::ONE;
using veriwell::ALWAYS_CODE;
using veriwell::ALWAYS_BLOCK;
using veriwell::INITIAL_CODE;
using veriwell::INITIAL_BLOCK;
using veriwell::LVAL_NEW_NET;
using veriwell::LVAL_REG;
using veriwell::LVAL_NET;
using veriwell::LVAL_REG_NET;
using veriwell::ASSIGN_STMT;
using veriwell::ASSIGN_DELAY_STMT;
using veriwell::ASSIGN_NONBLK_DELAY_STMT;
using veriwell::ASSIGN_EVENT_STMT;
using veriwell::ASSIGN_NONBLK_DELAY_STMT;
using veriwell::ASSIGN_NONBLK_EVENT_STMT;
using veriwell::ASSIGN_PROC_STMT;
using veriwell::DEASSIGN_STMT;
using veriwell::ASSIGN_STMT;
using veriwell::GATE_OR_TYPE;
using veriwell::INSTANCE_NODE;
using veriwell::NULL_STMT;
using veriwell::IF_STMT;
using veriwell::FOREVER_STMT;
using veriwell::FOR_STMT;
using veriwell::FORCE_STMT;
using veriwell::FORK_STMT;
using veriwell::FORK_NAMED_STMT;
using veriwell::REPEAT_INIT_STMT;
using veriwell::REPEAT_STMT;
using veriwell::WHILE_STMT;
using veriwell::DELAY_STMT;
using veriwell::WAIT_STMT;
using veriwell::ARROW_STMT;
using veriwell::DISABLE_STMT;
using veriwell::RELEASE_STMT;
using veriwell::CASE_STMT;
using veriwell::CASEZ_STMT;
using veriwell::CASEX_STMT;
using veriwell::BEGIN_STMT;
using veriwell::END_STMT;
using veriwell::NAMED_BLOCK;
using veriwell::BEGIN_NAMED_STMT;
using veriwell::END_NAMED_STMT;
using veriwell::JOIN_STMT;
using veriwell::JOIN_NAMED_STMT;
using veriwell::TASK_STMT;
using veriwell::SYSTASK_STMT;
using veriwell::BIT_NOT_EXPR;
using veriwell::EQ_EXPR;
using veriwell::EQ_CASE_EXPR;
using veriwell::NE_CASE_EXPR;
using veriwell::NE_EXPR;
using veriwell::CONCAT_REF;
using veriwell::NEGATE_EXPR;
using veriwell::TRUTH_NOT_EXPR;
using veriwell::BIT_NOT_EXPR;
using veriwell::AND_REDUCE_EXPR;
using veriwell::NAND_REDUCE_EXPR;
using veriwell::OR_REDUCE_EXPR;
using veriwell::NOR_REDUCE_EXPR;
using veriwell::XOR_REDUCE_EXPR;
using veriwell::XNOR_REDUCE_EXPR;
using veriwell::PLUS_EXPR;
using veriwell::MINUS_EXPR;
using veriwell::MULT_EXPR;
using veriwell::DIV_EXPR;
using veriwell::MOD_EXPR;
using veriwell::TRUTH_ANDIF_EXPR;
using veriwell::TRUTH_ORIF_EXPR;
using veriwell::LT_EXPR;
using veriwell::LE_EXPR;
using veriwell::GT_EXPR;
using veriwell::GE_EXPR;
using veriwell::BIT_AND_EXPR;
using veriwell::BIT_OR_EXPR;
using veriwell::BIT_XOR_EXPR;
using veriwell::BIT_XNOR_EXPR;
using veriwell::LSHIFT_EXPR;
using veriwell::RSHIFT_EXPR;
using veriwell::CONCAT_REP_REF;
using veriwell::SYSFUNCTION_REF;
using veriwell::DELAY_EXPR;
using veriwell::EVENT_DECL;
using veriwell::EVENT_STMT;
using veriwell::ANYEDGE_EXPR;
using veriwell::POSEDGE_EXPR;
using veriwell::NEGEDGE_EXPR;


namespace veriwell {
    class Simulator {
    private:
	/*
 	 * lex/parse callbacks for
 	 * interactive mode and library
 	 * parsing
 	 */
	void  (*reinit_lex_fptr)();
	int   (*prog_parse_fptr)(tree&);
	void  (*push_stream_fptr)(File*,int);
	File* (*pop_stream_fptr)();
	/* 
 	 * these callback's need to be untangled and removed
 	 */
	void  (*finish_fptr)();
	/*
  	 * global simulator state
  	 */
        clock_t clock_start;
        clock_t clock_compile;
        clock_t clock_load;
        clock_t clock_simulate;
    public:
	Simulator() : 
		reinit_lex_fptr(0),
		prog_parse_fptr(0),
		push_stream_fptr(0),
		pop_stream_fptr(0),
		finish_fptr(0)
	{}
	void reinit_lex() { (*reinit_lex_fptr)(); }
	void reinit_lex( void (*fptr)() ) { reinit_lex_fptr = fptr; }
	int prog_parse(tree& t) { return (*prog_parse_fptr)(t); }
	void prog_parse( int (*fptr)(tree&) ) { prog_parse_fptr = fptr; }
	void push_stream(File* f,int i) { (*push_stream_fptr)(f,i); }
	void push_stream( void (*fptr)(File*,int) ) 
					{ push_stream_fptr = fptr; }
	File* pop_stream() { return (*pop_stream_fptr)(); }
	void pop_stream( File* (*fptr)() ) { pop_stream_fptr = fptr; }
	void finish() { (*finish_fptr)(); }
	void finish( void (*fptr)() ) { finish_fptr = fptr; }
	/*
         * simulator control
         */
	void DelayType( delay_type dt ) { delayType = dt; }
	delay_type DelayType() { return delayType; }
        void Init(int,char**);
	void Precompile();
	void CompileI();
	void Simulate();
	void PrintInfo();
	int CountError( int warning );
    };
    extern Simulator simulator;
    inline void finish() { 
        broadcast_tf(reason_finish);
	simulator.finish(); 
    }
}




#endif // VERIWELL_INTERFACE
