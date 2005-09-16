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

/* PRINT.H - Print routine prototypes */

#ifndef PRINT_H
#define PRINT_H

// public methods

enum print_state { START, EXTEND_X, EXTEND_Z, EXTEND_0, EXTEND_DONE };

void print_bcd(handle_t handle, Group * val, nbits_t nbits,
	       int integer_attr, int pad);
void print_dec(handle_t handle, Group * g, nbits_t nbits,
	       int int_attr, int pad);
void print_binary_file(FILE * file, Group * g, nbits_t nbits);
void print_datum_file(handle_t handle, Group * g, nbits_t print_nbits,
		      nbits_t nbits, enum radii radix, int is_integer,
		      int fill_space, char *spec);
void print_datum(Group * g, nbits_t print_nbits, nbits_t nbits,
		 enum radii radix, int is_integer, int fill_space);
void print_common(tree node);
void print_stats(void);
void print_delay(tree node);
void print_expr(tree node);
int print_node(tree node);
void print_scope(handle_t handle, tree scope);
void print_time(time64 * time_p);
char *time_string(time64 * time_p);
void init_print(void);
char *set_print_buf(unsigned_32_t size);
void print_assignment(tree node);
char *sprint_decimal(Group * g, nbits_t nbits);
char *sprint_hex(Group * g, nbits_t nbits);
char *sprint_binary(Group * g, nbits_t nbits);


// private methods

#ifdef PRINT_C

static void print_char(handle_t handle, int c, int fill);
static void print_string(handle_t handle, Group * g_base,
			 nbits_t nbits, int fill);
static enum print_state print_group(Bit aval, Bit bval, int size,
				    enum print_state state, int last,
				    enum radii radix, int *octal_shift,
				    Bit * residue_aval,
				    Bit * residue_bval);
static void print_block_common(tree node);
static void print_decl_common(tree node);
static void print_args(tree node);
static void print_ref(tree node);
static void print_delay_stmt(tree node);
static void print_event_stmt(tree node);
static unsigned bcd_add(char a[], char b[], unsigned ndigits);
static unsigned bcd_inc(char a[], unsigned ndigits);
static void print_bcd_(Group * val, nbits_t nbits,
		       int integer_attr, int pad);

#endif				// PRINT_C

#endif				// PRINT_H
