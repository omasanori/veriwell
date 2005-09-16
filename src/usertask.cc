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

/**********************************************************
 *
 *      usertask.c
 *	    manage user tasks
 *
 **********************************************************
 */

#include <string.h>
#include "usertask.h"
#include "vtypes.h"

/*
 * existing compiled in list of user tasks
 */
extern s_tfcell veriusertfs[];

/**********************************************
  Usertask_Init
    - initialize usertask module. pass in 
      command line arguments to be parsed.
 **********************************************/
void Usertask_Init(int argc, char *argv[])
{
}

/**********************************************
  Usertask_Lookup
    - Lookup user task/function name. Return
      task data structure. Return 0 if task/function
      is not found.
 **********************************************/
p_tfcell Usertask_Lookup(char *name)
{
    int i;

    for (i = 0; veriusertfs[i].type; i++) {
	if (!strcmp(name, veriusertfs[i].tfname)) {
	    return &veriusertfs[i];
	}
    }
    return 0;
}
