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

/* VERISYS.C - PLI system routine hooks */
#define VERISYS_C

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "veriuser.h"
#include "acc_user.h"
#include "glue.h"
#include "vtypes.h"
#include "tree.h"
#include "veriwell.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "verisys.h"

int timeformatcheck(int, int);
int timeformatcall(int, int);
int p(int user, int reason);
int pcheck(int user, int reason);
int mon_check(int user, int reason);
int mon_call(int user, int reason);
int mon_misc(int data, int reason);
int pmisc(int user, int reason);


#include "sdf.h"
#include "random.h"


#include "plihacks.h"

s_tfcell verisystfs[] = {
	/*** Template for an entry:
    { usertask|userfunction, data, checktf(), sizetf(), calltf(), misctf(),
      "$tfname", forwref?, Vtool?, ErrMsg? },
    Example:
    { usertask, 0, my_check, 0, my_func, my_misctf, "$my_task" },
    ***/

	/*** add user entries here ***/
    {usertask, 0, timeformatcheck, 0, timeformatcall, 0, "$timeformat"},
    {usertask, 0, showallinstances_call, 0, showallinstances_call, 0,
     "$showallinstances"},
    {usertask, 0, pcheck, 0, p, pmisc, "$p"},
    {usertask, 1, mon_check, 0, mon_call, mon_misc, "$my_monitor"},
    {usertask, 0, dumpstructure_call, 0, dumpstructure_call,
     dumpstructure_call,
     "$$dumpstructure"},
    {usertask, 0, 0, 0, abort_call, 0, "$$abort"},
    {usertask, 0, 0, 0, setvalue_call, 0, "$$testsetvalue"},

    {usertask, 0, sdf_check, 0, sdf_call, sdf_misc, "$sdf_annotate"},
    {userfunction, 0, dist_uniform, dist_uniform, dist_uniform, 
						0, "$dist_uniform"},
    {userfunction, 0, dist_normal, dist_normal, dist_normal, 
						0, "$dist_normal"},
    {userfunction, 0, dist_exponential, dist_exponential, dist_exponential, 
						0, "$dist_exponential"},
    {userfunction, 0, dist_poisson, dist_poisson, dist_poisson, 0, 
						"$dist_poisson"},
    {userfunction, 0, dist_chi_square, dist_chi_square, dist_chi_square, 
						0, "$dist_chi_square"},
    {userfunction, 0, dist_t, dist_t, dist_t, 0, "$dist_t"},
    {userfunction, 0, dist_erlang, dist_erlang, dist_erlang, 0, "$dist_erlang"},


    {0}	/*** final entry must be 0 ***/
};

#include <stdio.h>

int p(int user, int reason)
{
    printf("Get here on execution, user is %d, reason is %d\n", user,
	   reason);

    printf("Number of arguments: %d\n", tf_nump());
    tf_rosynchronize();
    tf_synchronize();
    return (0);
}

int pcheck(int user, int reason)
{
    printf("Get here on compile, user is %d, reason is %d\n", user,
	   reason);
    return (0);
}

int pmisc(int user, int reason)
{
    return (0);
}

/**********************************************
 * vcl example from manual
 **********************************************/

int mon_consume(p_vc_record);

typedef struct t_mon_node {
    handle param;
    char name[256];
    short int value;
} s_mon_node, *p_mon_node;

p_mon_node mon_array;
char *mon_instance_p;
int mon_num_params;

int mon_check(int data, int reason)
{
    int i;
    handle object;
    static int net_reg_list[3] = { accNet, accRegister, 0 };

    acc_initialize();
    acc_configure(accDevelopmentVersion, "1.6a");

    for (i = 1; i <= tf_nump(); i++) {
	if (!(object = acc_handle_tfarg(i)))
	    tf_error("parameter #%d to $my_monitor illegal", i);

	if (acc_fetch_type(object) != accNet
	    && acc_fetch_type(object) != accRegister)
	    tf_error("parameter #%d to $my_monitor illegal", i);

	if (acc_fetch_size(object) != 1)
	    tf_error("parameter #%d to $my_monitor illegal", i);
    }

    acc_close();
    return (0);
}

int mon_call(int data, int reason)
{
    int i;
    char *value_str;

    acc_initialize();
    acc_configure(accDevelopmentVersion, "1.6a");

    mon_instance_p = tf_getinstance();
    mon_num_params = tf_nump();

    mon_array = (p_mon_node) malloc(mon_num_params * (sizeof(s_mon_node)));

    for (i = 0; i < mon_num_params; i++) {
	mon_array[i].param = acc_handle_tfarg(i + 1);
	strcpy(mon_array[i].name, acc_fetch_name(mon_array[i].param));

	value_str = acc_fetch_value(mon_array[i].param, "%b");
	switch (value_str[0]) {
	case '0':
	    mon_array[i].value = vcl0;
	    break;
	case '1':
	    mon_array[i].value = vcl1;
	    break;
	case 'x':
	    mon_array[i].value = vclX;
	    break;
	case 'z':
	    mon_array[i].value = vclZ;
	    break;
	default:
	    tf_error("Unknown value...");
	    break;
	}
	acc_vcl_add(mon_array[i].param, mon_consume,
		    (char *) &mon_array[i], vcl_verilog_logic);
    }
    acc_close();
    tf_synchronize();
    return (0);
}

int mon_consume(p_vc_record change_data)
{
    p_mon_node curr_mon_node;

    switch (change_data->vc_reason) {
    case logic_value_change:
    case sregister_value_change:
	curr_mon_node = (p_mon_node) change_data->user_data;
	curr_mon_node->value = change_data->out_value.logic_value;
	tf_isynchronize(mon_instance_p);
	break;
    default:
	tf_error("Illegal callback from VCL");
    }
    return (TRUE);
}

int mon_misc(int data, int reason)
{
    int i;
    char *value;

    switch (reason) {
    case reason_synch:
	io_printf("%s ", tf_strgettime());

	for (i = 0; i < mon_num_params; i++) {
	    switch (mon_array[i].value) {
	    case vcl0:
		value = "0";
		break;

	    case vcl1:
		value = "1";
		break;

	    case vclX:
		value = "x";
		break;

	    case vclZ:
		value = "z";
		break;
	    }
	    io_printf("%s=%s ", mon_array[i].name, value);
	}
	io_printf("\n");
	break;

    }
    return (0);
}


/* $timeformat (units, prec, suffix, width) */

/* This is for $timeformat.  NOTE: This is imported by print.c! */

struct timeformat {
    int units;
    int precision;
    char *suffix;
    int width;
    int use_defaults;
} timeformat;


int timeformatcall(int user, int reason)
{
    char *tmp_suffix;

    acc_initialize();
    timeformat.units = tf_getp(1);	/* units */
    timeformat.precision = tf_getp(2);	/* precision */

    if (!timeformat.use_defaults)	/* deallocate old stuff, if any */
	free(timeformat.suffix);
    tmp_suffix = tf_getcstringp(3);	/* suffix string */
    timeformat.suffix = (char *) xmalloc(strlen(tmp_suffix) + 1);
    strcpy(timeformat.suffix, tmp_suffix);	/* make perm copy */

    timeformat.width = tf_getp(4);
    timeformat.use_defaults = 0;

    return 0;
}

int timeformatcheck(int user, int reason)
{
    return 0;
}

/* this is called from print.c when %t is used (file_handle is the VeriWell file
   handle) */

void timeformatprint(handle_t file_handle, tree scope, double this_time)
{
    char spec_format[100];	/* this holds the print spec */
    s_timescale_info timescale;
    int printunits;
    int scaleunits;		/* how much to rescale this_time */
    double realmult;		/* multiplier */
    double printtime;		/* The time to print after rescaling */
    int numwidth;		/* width of number part */

/* get the timescale for the module containing the display */

    acc_fetch_timescale_info((handle) scope, &timescale);


    if (timeformat.use_defaults) {
	printunits = acc_fetch_precision();	/* get smallest precision */
	strcpy(spec_format, "%20.0f");	/* default time format */
    } else {
	printunits = timeformat.units;
	numwidth = timeformat.width - strlen(timeformat.suffix);
	if (numwidth < 0)
	    numwidth = 0;
	sprintf(spec_format, "%%%d.%df%s", timeformat.width,
		timeformat.precision, timeformat.suffix);
    }

/* the given time has already been scaled to local units; unscale and
   scale to timeformat units */

    scaleunits = timescale.unit - printunits;
    realmult = pow(10, scaleunits);

    printtime = this_time * realmult;	/* rescale */

    fprintf_V(file_handle, spec_format, printtime);	/* finally, print it */

}

/*****************************************************************
 *
 *	showallinstances_call
 *		implements $showallinstances
 *
 *****************************************************************
 */


static ShowallEntry_t **showallTable;

int showallinstances_call(int data, int reason)
{
    handle object;

    acc_initialize();

    switch (reason) {
    case reason_checktf:
	if (tf_nump() != 0) {
	    tf_error("$showallinstances takes no arguments");
	}
	break;

    case reason_calltf:
	if (!showall_init()) {
	    break;
	}
	object = null;
	while (TRUE) {
	    object = acc_next_topmod(object);
	    if (object == null) {
		break;
	    }
	    if (!showall_search(object)) {
		acc_close();
		return 0;
	    }
	}
	showall_output();
	break;
    }

    acc_close();
    return 0;

}

/*---------------------------------------------------
	showall_init
		- initialize hash table 
-----------------------------------------------------*/
int showall_init(void)
{
    int i;

    showallTable = (ShowallEntry_t **) xmalloc(SHOWALL_HASH_SIZE *
						    sizeof(ShowallEntry_t
							   *));
    if (showallTable == NULL) {
	tf_error("$showallinstances: not enough memory");
	return FALSE;
    }
    for (i = 0; i < SHOWALL_HASH_SIZE; i++) {
	showallTable[i] = NULL;
    }
    return TRUE;
}

/*---------------------------------------------------
	showall_register
		- log the object 
-----------------------------------------------------*/
int showall_register(handle object)
{
    char *ptr;
    int type;
    char *name;
    char *filename;
    s_location location;
    int index;
    ShowallEntry_t *entry;
    int fulltype;

    name = acc_fetch_defname(object);
    acc_fetch_location(&location, object);

    type = acc_fetch_type(object);
    fulltype = acc_fetch_fulltype(object);
    if (type == accModule || fulltype == accCombPrim
	|| fulltype == accSeqPrim) {
	if (type == accModule && fulltype != accTopModule) {
	    type = accModule;
	} else {
	    type = fulltype;
	}
    }

    index = 0;
    for (ptr = name; *ptr; ptr = ptr + 1) {
	index = index ^ (unsigned) *ptr;
    }
    index &= 0xff;

    for (entry = showallTable[index]; entry; entry = entry->next) {
	if (!strcmp(entry->name, name) && entry->type == type) {
	    entry->count++;
	    return TRUE;
	}
    }
    entry = (ShowallEntry_t *) xmalloc(sizeof(ShowallEntry_t));
    if (entry == NULL) {
	tf_error("$showallinstances: Out of memory");
	return FALSE;
    }

    entry->name = name;
    entry->type = type;
    entry->count = 1;
    entry->next = showallTable[index];
    entry->filename = location.filename;
    showallTable[index] = entry;

    return TRUE;

}

/*---------------------------------------------------
	showall_search
		- traverse the hierarchy 
-----------------------------------------------------*/
int showall_search(handle object)
{
    int type;
    handle newObject;

    type = acc_fetch_type(object);

    switch (type) {
    case accModule:
	if (!showall_register(object)) {
	    return FALSE;
	}
    case accTask:
    case accFunction:
	newObject = null;
	while (TRUE) {
	    newObject = acc_next_primitive(object, newObject);
	    if (newObject == null) {
		break;
	    }
	    if (!showall_register(newObject)) {
		return FALSE;
	    }
	}

	newObject = null;
	while (TRUE) {
	    newObject = acc_next_scope(object, newObject);
	    if (newObject == null) {
		break;
	    }
	    if (!showall_search(newObject)) {
		return FALSE;
	    }
	}
	break;
    }
    return TRUE;
}

/*---------------------------------------------------
	showall_output
		- dump object list and free structures 
-----------------------------------------------------*/
void showall_output(void)
{
    ShowallEntry_t *entry;
    ShowallEntry_t *temp;
    int i;
    int moduleCount;
    int primitiveCount;
    int gateCount;


    gateCount = 0;
    primitiveCount = 0;
    moduleCount = 0;

    for (i = 0; i < SHOWALL_HASH_SIZE; i++) {
	entry = showallTable[i];
	while (entry) {
	    switch (entry->type) {
	    case accModule:
		moduleCount += entry->count;
		break;
	    case accPrimitive:
		gateCount += entry->count;
		break;
	    case accSeqPrim:
	    case accCombPrim:
		gateCount += entry->count;
		primitiveCount += entry->count;
		break;
	    }
	    entry = entry->next;
	}
    }
    io_printf("Total number of module instances = %d\n", moduleCount);
    io_printf("Total number of primitive instances = %d\n",
	      primitiveCount);

    for (i = 0; i < SHOWALL_HASH_SIZE; i++) {
	entry = showallTable[i];
	while (entry) {
	    switch (entry->type) {
	    case accTopModule:
		io_printf("\ttop level module %s, from file \"%s\"\n",
			  entry->name, entry->filename);
		break;
	    case accSeqPrim:
	    case accCombPrim:
		io_printf("\t%d of primitive %s, from file \"%s\"\n",
			  entry->count, entry->name, entry->filename);
		break;
	    case accModule:
		io_printf("\t%d of module %s, from file \"%s\"\n",
			  entry->count, entry->name, entry->filename);
		break;
	    }
	    entry = entry->next;
	}
    }

    io_printf("Total number of gates = %d\n", gateCount);

    for (i = 0; i < SHOWALL_HASH_SIZE; i++) {
	entry = showallTable[i];
	while (entry) {
	    switch (entry->type) {
	    case accPrimitive:
		io_printf("\t%d of %s\n", entry->count, entry->name);
		break;
	    }
	    temp = entry;
	    entry = entry->next;
	    free(temp);
	}
    }

    free(showallTable);
    showallTable = NULL;
}

void init_verisys()
{
    timeformat.use_defaults = 1;
}
