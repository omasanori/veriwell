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

/* VERIUSER.C - PLI user routine hooks */

/* Filename: veriuser.c */
#define VERIUSER_C

#if HAVE_CONFIG_H
#  include <config.h>
#endif
#include "veriuser.h"
#include "acc_user.h"

extern "C" {
    int lxt_recordvars( int data, int reason );
    int lxt_recordon( int data, int reason );
    int lxt_recordoff( int data, int reason );
    int lxt_recordclose( int data, int reason );
    int lxt_recordfile( int data, int reason );
    int lxt_recordsetup( int data, int reason );
    int lxt2_recordvars( int data, int reason );
    int lxt2_recordon( int data, int reason );
    int lxt2_recordoff( int data, int reason );
    int lxt2_recordclose( int data, int reason );
    int lxt2_recordfile( int data, int reason );
    int lxt2_recordsetup( int data, int reason );
}

char *veriuser_version_str = ""
#ifdef HAVE_LXT
    "lxt  support compiled in\n"
#endif
#ifdef HAVE_LXT2
    "lxt2 support compiled in\n\n"
#endif
;
int (*endofcompile_routines[]) () = {
	/*** my_eoc_routine, ***/
    0	  /*** final entry must be 0 ***/
};

s_tfcell veriusertfs[] = {
#ifdef HAVE_LXT
  { usertask, 0, lxt_recordvars, 0, lxt_recordvars, lxt_recordvars, "$lxt_recordvars" },
  { usertask, 0, lxt_recordon, 0, lxt_recordon, lxt_recordon, "$lxt_recordon" },
  { usertask, 0, lxt_recordoff, 0, lxt_recordoff, lxt_recordoff, "$lxt_recordoff" },
  { usertask, 0, lxt_recordclose, 0, lxt_recordclose, lxt_recordclose, "$lxt_recordclose" },
  { usertask, 0, lxt_recordfile, 0, lxt_recordfile, lxt_recordfile, "$lxt_recordfile" },
  { usertask, 0, lxt_recordsetup, 0, lxt_recordsetup, lxt_recordsetup, "$lxt_recordsetup" },
#endif
#ifdef HAVE_LXT2
  { usertask, 0, lxt2_recordvars, 0, lxt2_recordvars, lxt2_recordvars, "$lxt2_recordvars" },
  { usertask, 0, lxt2_recordon, 0, lxt2_recordon, lxt2_recordon, "$lxt2_recordon" },
  { usertask, 0, lxt2_recordoff, 0, lxt2_recordoff, lxt2_recordoff, "$lxt2_recordoff" },
  { usertask, 0, lxt2_recordclose, 0, lxt2_recordclose, lxt2_recordclose, "$lxt2_recordclose" },
  { usertask, 0, lxt2_recordfile, 0, lxt2_recordfile, lxt2_recordfile, "$lxt2_recordfile" },
  { usertask, 0, lxt2_recordsetup, 0, lxt2_recordsetup, lxt2_recordsetup, "$lxt2_recordsetup" },
#endif
    {0}	
};
