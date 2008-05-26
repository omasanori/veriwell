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

/* PASS3.H - Prototypes for pass3.c */

#ifndef PASS3_H
#define PASS3_H

/* Used to tell pass3_expr to make a conversion, if necessary */

enum convert_to { eNONE, eREAL, eINT };

extern tree dump;		// common dump block
extern ngroups_t stack_size;
extern int max_label;


//      public exports

tree *pass3_expr(tree);
tree *pass3_expr_convert(tree, enum convert_to);
void push_inst(void);
void pop_inst(void);
void push_stack_size(void);
void pop_stack_size(void);
void stack_allocate(void);
tree module_of(tree scope);
tree lookdown_scope(char *ident, tree scope);
void maybe_build_marker(tree decl);
nbits_t get_range(tree node, char *ident);
Group *get_const(tree node, nbits_t *);
Bit get_delay(tree);
int pass3_early_conversion(tree * op1_ptr, tree op2);
int pass3_early_both_conversion(tree * op1_ptr, tree op2);
int pass3_late_conversion(tree * op1_ptr, tree op2, int code_offset);
nbits_t fixup_nbits(tree node);
void pass3_lval(tree * node);
tree *pass3_expr(tree node);
tree *pass3_expr_intrude(tree node, int restore_stack);
tree *pass3_expr_marker(tree node, Marker ** marker_ptr,
			enum marker_flags flags, tree info, tree);
void pass3_delay(tree node);
int pass3_assignment(tree node);
void pass3_args(tree arg_list, char *name, tree * inassign,
		tree * outassign, tree port_list, int stack_args);
void pass3_delay_stmt(tree node);
void pass3_event_stmt(tree node);
void pass3_node(tree node);
void pass3_node_tail(tree node, tree label);
void connect_instances(tree module);
void pass3_tree(tree node);
void pass3_interactive(tree node);
void init_pass3(void);
void adjust_nbits(nbits_t nbits, tree * tree_addr, tree * code_base);

//      private declarations
#ifdef PASS3_C

static nbits_t ensure_stack_space(tree node);
static void reserve_stack_space(int number, nbits_t nbits_push,
				nbits_t nbits_pad);
static nbits_t fixup_reference(tree node);
static struct part_info *cook_part_ref(nbits_t offset_high,
				       nbits_t offset_low, int concat);
static nbits_t pass3_hierarchy(tree);

tree *pass3_expr_match_convert(tree op1, tree op2);

#endif				// PASS3_C

#endif				// PASS3_H
