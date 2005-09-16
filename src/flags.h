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

/* FLAGS.H - Global access to command line options */

#ifndef FLAGS_H
#define FLAGS_H

extern int program;
extern int debug;
#if VDEBUG != 0
extern int vdebug;
#endif
extern int y_flag;
extern int simulate;
extern int f_flag;
extern int s_flag;
extern int i_flag;
extern int l_flag;
extern int k_flag;
extern int y_flag;
extern int trace_flag;
extern int synopsys_flag;
extern int var_debug_flag;
extern int inhibit_warnings;
extern int MemTrace;

extern enum delay_type delay_type;
extern enum tree_type default_net_type;

/* all the user-defined plusargs in a TREE_LIST */
extern tree plusargs;

void init_flags(void);


#endif				// FLAGS_H
