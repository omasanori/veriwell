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


/**********************************************************************
 *
 *	specify.h
 *		- header file for specify block code
 *
 **********************************************************************
 */


#ifndef SPECIFY_H
#define SPECIFY_H



//      public prototypes

void specify_save_param_list(tree scope, tree list);
tree build_path(tree inputs, tree outputs, tree source, tree delays,
		tree condition, int parallelFlag, int edge, int polarity);
tree check_input_port(tree identifier);
tree check_output_port(tree identifier);
tree check_path(tree path);
void insert_specify_paths(tree module);
tree get_first_driver(tree decl, int bit);
int get_driver_count(tree decl, int bit);
int is_driven_by(tree gate, tree decl, int bit);
void add_spec_to_gate(tree module, tree driver, tree pathdesc,
		      int outputNumber);
void pass3_specify(tree spec);
int handle_specify_path(tree gate);
void propagate_specify_path(tree pc);
void calculate_delays(tree constraint);
void specify_update_timings(tree module);
tree build_timing_statement(int checkType, tree event1, tree event2,
			    tree param1, tree param2, tree notifier);

tree build_timing_event(int edge, tree expression, tree condition);
void pass3_timing_check(tree check, tree module);
tree make_timing_check(tree module, tree check, unsigned int edgeSpec1,
		       tree expression1, tree condition1,
		       unsigned int edgeSpec2, tree expression2,
		       tree condition2, tree notifier);
int check_scalar(tree node);
tree check_one_bit_constant(tree constant);
void validate_timing_checks(tree module);
void timing_check(tree node);
int setupCheck(tree node, int event1, int event2);
int holdCheck(tree node, int event1, int event2);
int widthCheck(tree node, int event1, int event2);
int periodCheck(tree node, int event1, int event2);
int skewCheck(tree node, int event1, int event2);
int recoveryCheck(tree node, int event1, int event2);
int setupholdCheck(tree node, int event1, int event2);
void trace_timing_event(tree event, unsigned int edge, tree condition);
void calculate_params(tree timing_check);
tree make_notifier(tree ident);
void toggle_notifier(tree notifier);
void timingCheck(tree node);
void setXPathConstraints(tree constraint);

// private methods
#ifdef SPECIFY_C

static enum logical_value eval_bit(tree * code);

#endif				// SPECIFY_C

#endif				// SPECIFY_H
