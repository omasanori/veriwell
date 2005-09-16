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

/* MULTDIV.H - Multdiv routine prototypes */


#ifndef MULTDIV_H
#define MULTDIV_H

// public methods

void init_multdiv(void);

Bit BitAdd(Bit * a, Bit * b, Bit * c, unsigned bits);
Bit BitSub(Bit * a, Bit * b, Bit * c, unsigned bits);
void GroupMult(Group * ag, Group * bg, Group * cg, unsigned groups);
void GroupDiv(Group * ag, Group * bg, Group * cg, Group * dg,
	      unsigned words, int do_mod);

#endif				// MULTDIV_H
