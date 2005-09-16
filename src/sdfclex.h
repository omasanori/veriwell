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

/*********************************************************************
 *
 *		sdfclex.h
 *			definitions for sdfclex.c
 *
 *********************************************************************
 */


#ifndef SDFCLEX_H
#define SDFCLEX_H

// public definitions

void sdfconfig_error(char *message);
FILE *sdfclexOpenFile(char *filename);
void sdfclexInit(void);



// private definitions

#ifdef SDFCLEX_C
static void sdflexPushFile(void);
static void sdflexPopFile(void);
#endif				// SDFLEX_C


#endif				// SDFCLEX_H
