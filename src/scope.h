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

/* SCOPE.H - Scope and hierarchical name prototypes and globals */

#ifndef SCOPE_H
#define SCOPE_H

// public methods

void set_decl(tree, tree);
void push_scope(void);
tree pop_scope(void);
void set_scope(tree);
void reset_scope(void);
void initialize_scope(tree);
tree resolve_hierarchical_path(char *);
tree resolve_hierarchical_name(tree);
tree search_scope(char *, int);
tree search_up_decl(char *, tree);
void look_for_and_set_scope(tree);
void init_scope(void);
tree search_scope_across(tree starting_scope, char *name, int set);
tree search_scope_up(tree starting_scope, char *name, int set);

extern tree current_scope;

// private methods

#ifdef SCOPE_C

static void lookupward_scope(tree scope);

#endif				// SCOPE_C

#endif				// SCOPE_H
