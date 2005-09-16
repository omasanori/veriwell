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

/* STORE.H - Macros and prototypes for Store.c */

#ifndef STORE_H
#define STORE_H

// public methods

extern int store_computeOnly;
extern int store_useCached;
extern SCB *store_scb;
void notify(tree LVAL, enum logical_value STATE, int CHANGED, tree pc);
void store_array(tree decl, Bit index);
void store(tree lval, tree pc);
void init_store(void);

// private methods

#ifdef STORE_C
#endif				// STORE_C

#endif				// STORE_H
