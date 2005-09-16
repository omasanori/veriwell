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

/* PASS2.H - Pass2 routine prototypes */

#ifndef PASS2_H
#define PASS2_H

// public methods

Group *malloc_X(ngroups_t ngroups);
Group *realloc_X(Group * g, ngroups_t ngroups);
Group *malloc_Z(ngroups_t ngroups);
Group *realloc_Z(Group * g, ngroups_t ngroups);
void pass3_decl(tree decl);
void build_hierarchy(void);
void print_top_modules(void);
void build_hierarchy(void);
void print_top_modules(void);
void init_pass2();
tree check_library(char *name);

// private methods

#ifdef PASS2_C

static ngroups_t do_vector_stuff(tree decl);
static tree copy_module(tree node);
static tree module_lookup(tree instance);
static void do_connect_ports(tree port, tree arg_tree, tree instance);
static void connect_ports(tree inside, tree outside);
static void resolve_defparams(tree scope);
static void initialize_decls(tree scope);
static void do_instantiation(tree node);

#endif				// PASS2_C

#endif				// PASS2_H
