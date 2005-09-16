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

/* DECL.H - Decl and Check routine prototypes */

#ifndef DECL_H
#define DECL_H

extern tree integer_zero_node;
extern tree integer_one_node;
extern tree current_scope;

void set_decl(tree, tree);
tree make_net_spec(enum tree_type, tree, tree);
tree make_reg_spec(tree);
tree make_integer_spec(tree);
tree make_real_spec(tree);
tree make_time_spec(tree);
tree make_event_spec(void);
tree make_decl(tree, tree, tree, tree);
tree make_param_spec(tree);
tree make_param_decl(tree, tree, tree);
tree make_block_decl(tree, tree, tree);
tree make_default_net(tree);
void init_decl(void);
tree root_port_decl(tree);
tree root_port_name(tree);
void redirect_port(tree ident, tree node);
tree make_tmp_decl(tree lval);
void free_tmp_decl(tree tmp);


#endif				// DECL_H
