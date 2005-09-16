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

/* SCOPE.C - Routines for resolving hierarchical names, setting scopes, etc. */

#define SCOPE_C

#include <string.h>
#include "vtypes.h"
#include "tree.h"
#include "scope.h"

/* Pointer to the current scope's block */
tree current_scope;


/* This is used at compile time mostly to save outer-level declarations
    for vars that are redefined at as inner level but then have to be restored
    upon leaving the scope. */

struct scoping_level {
    struct scoping_level *previous;	/* points to outer-level scope */
    tree block;			/* points to current block node */
    tree shadow;		/* points to tree list of outer-level decls to be */
    /*   restored upon exit from this scope */
};
/* level0 and scope0 will hold names to top-level modules */
static struct scoping_level level0;
tree scope0 = NULL;		/* Initialized in pass2.s */

static struct scoping_level *current_level = &level0;
static struct scoping_level *free_level = (struct scoping_level *) NULL;

/* Remember the previous declaration of an ident by making a TREE_LIST
   with iden as TREE_PURPOSE and its old declaration as TREE_VALUE. */

void set_decl(tree ident, tree decl)
{
    current_level->shadow =
	link_tree_cons(ident, IDENT_CURRENT_DECL(ident),
		       current_level->shadow);
    IDENT_CURRENT_DECL(ident) = decl;
}

void push_scope()
{
    struct scoping_level *tmp;

/* create a new scoping level, or resuse one */

    if (!free_level)
	tmp =
	    (struct scoping_level *)
	    linkalloc(sizeof(struct scoping_level));
    else {
	tmp = free_level;
	free_level = tmp->previous;
    }

    tmp->block = current_scope;
    tmp->previous = current_level;
    tmp->shadow = NULL_TREE;
    current_level = tmp;
}

tree pop_scope()
{
    tree decl_list, next;
    struct scoping_level *level;

    /* restore declarations of previous scope (set in set_decl).  TREE_PURPOSE
       has the IDENTIFIER_NODE; TREE_VALUE has the DECL_NODE.  */

    for (decl_list = current_level->shadow; decl_list; decl_list = next) {
	next = TREE_CHAIN(decl_list);
	IDENT_CURRENT_DECL(TREE_PURPOSE(decl_list)) =
	    TREE_VALUE(decl_list);
	free_link_list(decl_list);
    }

/* pop the scoping level and save the old frame for later use */

    level = current_level;
    current_level = current_level->previous;
    level->previous = free_level;
    free_level = level;
    if (!current_level)
	return NULL_TREE;
    return current_level->block;
}

void set_scope(tree new_scope)
{
    tree t;

    current_scope = new_scope;
    push_scope();
    /* first, do ports */
    for (t = BLOCK_PORTS(new_scope); t; t = TREE_CHAIN(t))
	set_decl(DECL_NAME(t), t);
    /* then do DECLs */
    for (t = BLOCK_DECL(new_scope); t; t = TREE_CHAIN(t)) {
	t = t;
	set_decl(DECL_NAME(t), t);
    }
    /* if scope is a module then do specparams */

    if (TREE_CODE(new_scope) == MODULE_BLOCK) {
	for (t = MODULE_SPECDEFS(new_scope); t; t = TREE_CHAIN(t)) {
	    if (TREE_CODE(t) == SPECPARAM_DECL) {
		set_decl(DECL_NAME(t), t);
	    }
	}
    }
//  for (t = BLOCK_DOWN (new_scope); t; t = TREE_CHAIN (t))
//    set_decl (BLOCK_NAME (t), t);
}

void reset_scope()
{
    tree decl_list, next;

    for (decl_list = current_level->shadow; decl_list; decl_list = next) {
	next = TREE_CHAIN(decl_list);
	IDENT_CURRENT_DECL(TREE_PURPOSE(decl_list)) =
	    TREE_VALUE(decl_list);
	free_link_list(decl_list);
    }
}

/* At runtime, puts the list of top-level modules in the symbol table
   and sets the scope to the first listed top-level module. */

void initialize_scope(tree top_level)
{
    tree t;

    current_level = &level0;
    current_scope = scope0;

/* Read top-level block names, only if called after pass2 */
    if (top_level) {
	for (t = BLOCK_DECL(scope0); t; t = TREE_CHAIN(t))
	    set_decl(DECL_NAME(t), t);
	set_scope(top_level);
    }
}


tree search_scope_across(tree starting_scope, char *name, int set)
{
    tree scope;

    for (scope = starting_scope; scope; scope = TREE_CHAIN(scope))
	if (!strcmp(name, IDENT(BLOCK_NAME(scope)))) {
	    if (set)
		set_scope(scope);
	    return scope;
	}
    return NULL_TREE;
}

tree search_scope_up(tree starting_scope, char *name, int set)
{
    tree scope, new_scope;

    for (scope = starting_scope; scope; scope = BLOCK_UP(scope)) {
	if (set)
	    current_scope = pop_scope();
//    if (!strcmp (name, IDENT (BLOCK_NAME (scope))))
	if ((new_scope =
	     search_scope_across(scope, name, set)) != NULL_TREE)
	    return new_scope;
    }
    return search_scope_across(top_level, name, set);
}

/*
static
void
lookupward_scope (tree scope)
{
  tree t;
  int found = 0;

  for (t = current_scope; t; t = BLOCK_UP (t))
  {
    current_scope = pop_scope ();
    if (scope == t)
    {
      found = 1;
      set_scope (t);
      break;
    }
  }
  if (!found)
  {
    for (t = top_level; t; t = TREE_CHAIN (t))
      if (scope == t) {
        set_scope (t);
        found = 1;
      }
  }
  if (!found)
    error ("Scope not found", NULL_CHAR, NULL_CHAR);
}
*/
tree resolve_hierarchical_path(char *path)
{
    extern char *token_buffer;	/* Will already be as large as needed */
    char *path_name = token_buffer, *decl_name;

    strcpy(path_name, path);

    /* seperate the last component of the path */
    decl_name = strrchr(path_name, '.');
    *decl_name = '\0';
    decl_name++;

    return search_up_decl(decl_name, search_scope(path_name, 0));
}

tree resolve_hierarchical_name(tree ident)
{
    return (resolve_hierarchical_path(IDENT(ident)));
}

/* Traverse the tree following the hierichical name.  If "set" is non-zero,
   then also set the scope as the tree is traversed.  Not that "set"
   should be set _only_ if the hierachical name is known to be good,
   usually (always?) by first calling this without setting "set".
   If "set" is set, but the name is bad, the the scope will be screwed up
   with no chance of recovery. */

tree search_scope(char *path_name, int set)
{
    extern char *token_buffer;	/* Will already be as large as needed */
    tree scope, set_scope;
    extern tree scope0;		/* defined in decl.c */
    char *p, *path = token_buffer;

    strcpy(path, path_name);
    p = strtok(path, ".");

    set_scope = current_scope;

    scope = search_scope_across(BLOCK_DOWN(set_scope), p, set);
    if (!scope) {
	scope = search_scope_up(set_scope, p, set);
	if (!scope) {
	    if (set) {
		for (set_scope = current_scope; set_scope != scope0;
		     set_scope = BLOCK_UP(set_scope))
		    current_scope = pop_scope();
	    }
	    scope = search_scope_across(top_level, p, set);
	    if (!scope) {
		for (set_scope = current_scope;
		     TREE_CODE(set_scope) != MODULE_BLOCK;
		     set_scope = BLOCK_UP(set_scope))
		    if (set)
			current_scope = pop_scope();
		scope = search_scope_across(BLOCK_DOWN(set_scope), p, set);
		if (!scope) {
		    error
			("Component '%s' is not in up, down, or top-level path",
			 p, NULL_CHAR);
		    return error_mark_node;
		}
	    }
	}
    }

    for (p = strtok(NULL, "."); p; p = strtok(NULL, ".")) {
	set_scope = scope;
	scope = search_scope_across(BLOCK_DOWN(set_scope), p, set);
	if (!scope) {
	    scope = search_scope_up(set_scope, p, set);
	    if (!scope) {
		error
		    ("Component '%s' is not in up, down, or top-level path",
		     p, NULL_CHAR);
		return error_mark_node;
	    } else if (!set)
		warning("Hierarchical component '%s' found on upward path",
			IDENT(BLOCK_NAME(scope)), NULL_CHAR);
	}
    }
    return scope;
}

/* Look for a decl node with the give name in the current scope.  If
   not there, search up the hierarchy.  If found higher up, give
   a warning.  If not found, return error_mark_node and print error.
   Don't look any higher than the first MODULE_BLOCK. */

tree search_up_decl(char *name, tree starting_scope)
{
    tree scope, t, t1;

    if (starting_scope == error_mark_node)
	return error_mark_node;

    for (scope = starting_scope; scope;
//  scope = (TREE_CODE (scope) == MODULE_BLOCK ? NULL_TREE : BLOCK_UP (scope)))
	 scope = BLOCK_UP(scope)) {
	for (t = BLOCK_DECL(scope); t; t = TREE_CHAIN(t)) {
//      if (TREE_CODE (t) == ARRAY_DECL)
//  t1 = ARRAY_DECL (t);
//      else
	    t1 = t;
	    if (!strcmp(name, IDENT(DECL_NAME(t1)))) {
		if (scope != starting_scope)
		    warning("Identifier '%s' found on upward path", name,
			    NULL_CHAR);
		return t;
	    }
	}
	for (t = BLOCK_PORTS(scope); t; t = TREE_CHAIN(t)) {
	    if (TREE_CODE(t) == ARRAY_REF)
		t1 = ARRAY_REF_DECL(t);
	    else
		t1 = t;
	    if (!strcmp(name, IDENT(DECL_NAME(t1)))) {
		if (scope != starting_scope)
		    warning("Identifier '%s' found on upward path", name,
			    NULL_CHAR);
		return t;
	    }
	}
    }
    error("Identifier '%s' not declared", name, NULL_CHAR);
    return error_mark_node;
}

/* Given the current scope and a new scope, trace the path down or up
   to the new scope and then call set_scope() and pop_scope() enough times
   to set the new scope.  This will be called only when the new scope is
   in the symbol table, so it can only be one down, across, or top_level.
*/

void look_for_and_set_scope(tree scope)
{
    tree s;

/* Case 1: top level - pop until top of hierarchy, then set */
    if (!BLOCK_UP(scope)) {
	for (s = current_scope; s; s = BLOCK_UP(s))
	    current_scope = pop_scope();
	set_scope(scope);
    }
/* Case 2: one down - simply set the scope */
    else if (BLOCK_UP(scope) == current_scope)
	set_scope(scope);
    else
	error("Scope case not handled yet", NULL_CHAR, NULL_CHAR);
}

void init_scope()
{
    scope0 = NULL;
    current_level = &level0;
    free_level = NULL;
    current_scope = NULL_TREE;
    memset(&level0, 0, sizeof(level0));
}
