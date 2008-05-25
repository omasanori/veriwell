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

/* EXEC.H - Macros and prototypes for Exec */

#ifndef EXEC_H
#define EXEC_H

void print_prompt(void);
tree exec_(tree pc);
void set_simulation_time(time64 * newtime_p);
struct context_member *make_context(void);
void enter_context(struct SCB *scb, tree scope, tree pc);
tree exit_context(struct SCB *scb);
tree unwind_context(struct context_member *context);
void disable_context(struct context_member *context);
tree deassign(tree lval, enum tree_code code);
void do_net_assignment(tree pc, int force=0);
void initialize_cont_assignments(void);
void net_exec(tree pc);
void exec_history();
void exec_remove_history();
tree go_interactive(tree current_pc, tree next_pc, Group * g,
		    int skip, int val);
void handle_gate(tree gate);
void init_interactive(void);
void init_exec(void);

#endif				// EXEC_H
