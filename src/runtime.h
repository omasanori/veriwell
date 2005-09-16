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

/* RUNTIME.H - Global variables and routine prototypes used during execution */

/* cleared if either trace or single-step enabled */
extern int normal_flag;

extern int trace_flag;

extern int single_step_flag;
extern int single_trace_flag;
extern int break_flag;

extern int in_disable;		/* for the DISABLE statment processing */

extern int is_interactive;
extern int cmd_level;
extern int new_cmd_level;

extern int in_initial;		/* set while initializing nets */

extern Group **R;		/* Virtual register (accumulator) stack pointer */
extern Group **R_base;		/* Virtual register (accumulator) base pointer */
extern nbits_t R_nbits;		/* current expression length in bits */
extern ngroups_t R_ngroups;	/* Current expression size in groups */
extern Bit R_mask;

//extern void trace_stmt ();
// extern void print_datum ();
//extern void print_delay ();

//extern void eval_1 (union tree_node *);
//extern void exec_sysfunc (union tree_node *, nbits_t);

//extern tree systime_return;   /* function $time return variable */
//extern tree sysopen_return;   /* function $open return variable */
//extern tree sysrand_return;   /* function $random return variable */
//extern tree sysplus_return;   /* function $test$plusargs return variable */
