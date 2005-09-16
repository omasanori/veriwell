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

#include "veriuser.h"
#include "acc_user.h"

//char *veriuser_version_str = "Sample PLI version string 1.0\n";
char *veriuser_version_str = "";
int (*endofcompile_routines[]) () = {
	/*** my_eoc_routine, ***/
    0	  /*** final entry must be 0 ***/
};

/*
bool err_intercept(level,facility,code)
int level; char *facility; char *code;
{ return(true); }
*/

s_tfcell veriusertfs[] = {
	/*** Template for an entry:
    { usertask|userfunction, data, checktf(), sizetf(), calltf(), misctf(),
      "$tfname", forwref?, Vtool?, ErrMsg? },
    Example:
    { usertask, 0, my_check, 0, my_func, my_misctf, "$my_task" },
    ***/

	/*** add user entries here ***/

    {0}	/*** final entry must be 0 ***/
};
