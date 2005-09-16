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

/* TIMESCALE.H - Prototypes for timecale.c */

#ifndef TIMESCALE_H
#define TIMESCALE_H

int process_timescale(void);
void module_timescale(tree);
tree *timescale_scale(tree);
double timescale_divide(struct Time64 *, tree);
void init_timescale(void);
int get_tick_precision(void);
void timescale_get(tree, int *, int *);
void time_format(int, int, Group *, nbits_t, int);
double timescale_precision(tree scope);
char *timescale_string(int, char *);

#ifdef TIMSCAL_C
#endif				// TIMSCAL_C
#endif				/* TIMESCALE_H */
