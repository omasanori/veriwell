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

/* FLAGS.C - Global storage for command line options */

#include "vtypes.h"
#include "tree.h"
#include "flags.h"

/* For internal debugging: */
int program = 0;		/* displays program */
int debug = 0;			/* turns on internal trace */
#if VDEBUG != 0
int vdebug = 0;
#endif

/* if 0, parse only */
int simulate = 1;

/* Read more command line options from file */
int f_flag = 0;

/* Enter interactive mode before running */
int s_flag = 0;

/* Read commands from file */
int i_flag = 0;

/* Set log file */
int l_flag = 0;

/* Set key file */
int k_flag = 0;

/* Set library search path */
int y_flag = 0;

/* Set to enable synopsys 2.0 conformance warnings */
int synopsys_flag = 0;

/* Suppress warnings */
int inhibit_warnings = 0;

/* Enter trace mode upon entry.  Also used when $settrace is enabled. */
int trace_flag = 0;

/* Turn on variable debugging (default on) */
int var_debug_flag = 1;

/* Runtime state bits */
/* (trace_flag is in both flags.h and runtime.h) */
/* This is set if not trace nor single-step */
int normal_flag = 1;

/* set after ctl-c is pressed */
int break_flag = 0;

/* Set for either of the single-step commands: comma and semicolon */
int single_step_flag = 0;

/* set if comma (single-step with trace) */
int single_trace_flag = 0;

/* Capture all the user-defined plus args in a TREE_LIST */
union tree_node *plusargs = 0;

/* Print memory during pass 2 */
int MemTrace;

/***************************************************************
 *
 *  init_flags
 *	- initialize all global and static variables in flags.c
 *
 ***************************************************************
 */

void init_flags()
{
    program = 0;
    debug = 0;
#if VDEBUG != 0
    vdebug = 0;
#endif
    y_flag = 0;
    simulate = 1;
    f_flag = 0;
    s_flag = 0;
    i_flag = 0;
    l_flag = 0;
    k_flag = 0;
    synopsys_flag = 0;
    inhibit_warnings = 0;
    trace_flag = 0;
    var_debug_flag = 1;
    normal_flag = 1;
    break_flag = 0;
    single_step_flag = 0;
    single_trace_flag = 0;
    plusargs = 0;
    MemTrace = 0;
}
