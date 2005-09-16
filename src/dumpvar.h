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

/* DUMPVAR.H - VCD structures and prototypes */

#ifndef DUMPVAR_H
#define DUMPVAR_H


struct Dump {
    Marker *markers_first;
    Marker *markers_last;
    Marker *unprinted;		/* Before printin header, links are here */
    Marker *printed;		/* When printed, move to here */
    unsigned disallow_dumpvars:1;	/* Too late, dumpvars already processed */
    unsigned first_dumpvars:1;	/* First call to $dumpvars */
    unsigned enabled:1;		/* Enable/disabled */
    unsigned new_time:1;	/* Time changed; print time */
    int total;
    char *file_name;
};

extern struct Dump dumpvar;

void dumpvars_all(void);	/* Dump all vars in a simulation */
void dumpvars_scope_down(tree, int);	/* Dump all in a scope and down */
void dumpvars_mark(tree);	/* Dump a single qualified var */
int dumpvars_header(tree);	/* Print the VCD header */
void dumpvars_enable(void);	/* Turn on dumpvars */
void dumpvars_disable(void);	/* Turn off dumpvars */
void dumpvars_checkpoint(char *);	/* Print current values of all */
void dumpvars_x(char *);	/* Print Xs for all */
void dumpvars_flush(void);	/* flush dumpfile */
void dumpvars_dump(tree decl, int index);
void init_dumpvar(void);

#endif				// DUMPVAR_H
