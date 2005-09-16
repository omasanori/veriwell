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

/*******************************************************************
 *
 *	gates.h
 *		- header file for gates.c
 *		  
 *
 *******************************************************************
 */

#ifndef GATES_H
#define GATES_H


#define VAL_TO_STATE(g) ((enum logical_value)(((BVAL (g) & 1) << 1) | (AVAL (g) & 1)))

enum gate_style {
    PRIM_LOGIC = 1,
    PRIM_BUF,
    PRIM_TRI,
    PRIM_CMOS,
    PRIM_TRAN,
    PRIM_PULL
};

tree build_gate_instance(enum tree_type type, char *filename,
			 lineno_t lineno, tree ident, tree arg_list,
			 tree delay, tree udp_def);
void initialize_gates(void);
void gate_check_delay(tree gate, unsigned int max_delays);
int gate_ok_input(tree port);
int gate_ok_output(tree port);
tree next_gate_port(tree gate, tree * port_list);
void gate_check_ports(tree gate, enum gate_style style);
void pass3_gate(tree gate);
void handle_gate(tree gate);
void propagate_unigate_output(tree gate);
void propagate_bigate_output(tree gate);
void propagate_specify_output(tree gate, enum logical_value v);
void propagate_pull_output(tree gate);


#endif				// GATES_H
