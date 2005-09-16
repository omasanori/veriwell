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

/* TRACE.H - trace.c routine prototypes */

#ifndef TRACE_H
#define TRACE_H

// public methods

void trace_result(Group * val, nbits_t print_nbits,
		  nbits_t nbits, int integer_attr);
void trace_net_type(enum tree_type type);
void trace_stmt(tree node, Group * g, int skip, int val);
void init_trace(void);

// private methods

#ifdef TRACE_C

static void trace_lval(tree lval);
static void trace_assign_result(tree lval, Group * val, nbits_t nbits,
				int integer_attr);

#endif				// TRACE_C

#endif				// TRACE_H
