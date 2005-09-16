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

/* VERISYS.H - verisys.c routine prototypes */

#ifndef VERISYS_H
#define VERISYS_H

// public methods

void init_verisys(void);

// private methods

#ifdef VERISYS_C

#define SHOWALL_HASH_SIZE (256)

typedef struct ShowallEntry_tag {
    struct ShowallEntry_tag *next;
    char *name;
    char *filename;
    int type;
    int count;
} ShowallEntry_t;

int showallinstances_call(int, int);
int showall_init(void);
int showall_register(handle);
int showall_search(handle);
void showall_output(void);
int timeformatcall(int user, int reason);
int timeformatcheck(int user, int reason);
void timeformatprint(handle_t file_handle, tree scope, double this_time);

#endif				// VERISYS_C

#endif				// VERIWELL_H
