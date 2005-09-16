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

/* CHECK.H - CHECK.c routine prototypes */

#ifndef CHECK_H
#define CHECK_H


tree build_binary_op(enum tree_code code, tree op1, tree op2);
tree build_unary_op(enum tree_code code, tree op);
tree build_cond_expr(tree op1, tree op2, tree op3);
void concat_labels(tree node);
tree check_lval(tree node, enum lval_type lval_type, tree spec);
tree check_lval_nocheck(tree node, enum lval_type lval_type, tree spec);
tree make_net_source(tree decl);
tree check_lval_port(tree port);
void set_immediate_attr(tree port, int attr);
tree check_rval(tree ident);
tree check_rval_nocheck(tree ident);
tree check_block(tree ident);
tree build_bit_ref(tree decl, tree index);
tree build_part_ref(tree decl, tree msb, tree lsb);
tree build_function_call(tree ident, tree args);
tree check_task(tree node);
tree check_function(tree node);
tree check_reg(tree node);
tree check_non_reg(tree node);
tree check_net(tree node);
tree check_port(tree node);
tree check_named_event(tree ident);
tree double_check_named_event(tree decl);
tree double_check_block(tree ident);
tree implicit_conversion(tree lval, tree rval);
void init_check(void);


#endif				// CHECK_H
