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
 *		udp.h
 *			-	header file for udp.c	
 *
 *******************************************************************
 */


#ifndef UDP_H
#define UDP_H

extern int deltaTable[10][4][4];

tree build_udp(tree identifier);
tree build_udp_string(void);
void end_primitive(tree node);
void append_udp_digits(tree * node, char c1, char c2);
void validate_udp_string(tree udp, tree udp_string);
int is_edge(char *ptr);
int is_valid_output(char *ptr, int combFlag);
int is_valid_input(char *ptr, int combFlag);
void set_udp_table_entry(char *table, int positin, int state, char *last,
			 char *next, char *rest, int from_mask,
			 int *reportFlag);
void fill_udp_table(char *table, int inputNumber, int inputCount,
		    tree string_list, int seqFlag);
int is_edge_string(tree t, int *position);
int int_power(int b, int p);
void instantiate_udp(tree module, tree caller, tree udp_def);
void udp_exec(struct Marker *marker);
void init_udp(void);

// private methods

#ifdef UDP_C
#endif				// UDP_C

#endif				// UDP_H
