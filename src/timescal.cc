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

/* TIMESCAL.C - routines to support timescale and related stuff */

#define TIMESCAL_C

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "scope.h"
#include "pass3.h"
#include "timescal.h"
#include "acc_user.h"
#include "io.h"
#include "pli.h"
#include "systask.h"
#include "glue.h"


#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

/* Timescales global info */

enum any_enum { T_NONE, T_YES, T_NO };

static struct {
    int smallest_scale;
    enum any_enum any;
    int unit;
    int prec;
} timescale_global;

/* For timescales, convert units into a numerical form */

static int timescaleunits(char *units)
{
    if (!strncmp(units, "s", 1))
	return 0;
    else if (!strncmp(units, "ms", 2))
	return -3;
    else if (!strncmp(units, "us", 2))
	return -6;
    else if (!strncmp(units, "ns", 2))
	return -9;
    else if (!strncmp(units, "ps", 2))
	return -12;
    else if (!strncmp(units, "fs", 2))
	return -15;
    else
	return 1;
}

static char *unitnames[] = { "s", "ms", "us", "ns", "ps", "fs" };
static int unitnums[] = { 1, 100, 10 };

char *timescale_string(int units, char *unitbuf)
{
    int unit;
    int remain;

    unit = (units + 2) / 3;
    remain = units % 3;

//  printf ("(%d,%d,%d)\n", remain, unit, units);
    sprintf(unitbuf, "%d%s", unitnums[remain], unitnames[unit]);
    return unitbuf;
}

void timescale_get(tree scope, int *unit, int *prec)
{
    tree module;		/* The current module */

    module = module_of(scope);
    *unit = MODULE_TIME_UNIT(module);
    *prec = MODULE_TIME_PREC(module);
}

/* Return the scaling factor for a particular scope. */

double timescale_precision(tree scope)
{
    double multiplier = 1;
    int unit, prec, i;

    timescale_get(scope, &unit, &prec);	/* get unit, prec */

    /* Determine scaling factor.  This is based on the units for this module
       and the smallest precision of all the modules. */

    for (i = timescale_global.smallest_scale - unit; i > 0; i--)
	multiplier *= 10;

    return multiplier;
}

/* Scale the simulation time to the timescale of the scope */

double timescale_divide(struct Time64 *ptime, tree scope)
{
    double x, y;
    const double pow2_32 = ldexp((double) 1, 32);

    /* First, convert time to real */
    x = ptime->timeh;
    x = ldexp(x, sizeof(Bit) * 8);	/* shift by 32 */
    x += ptime->timel;

    /* Next, divide by the precision */
    x /= timescale_precision(scope);

    /* Finally, convert back into integer */

    y = x + 0.5;		/* round to compensate for small errors in conversion */

    ptime->timel = (unsigned int) fmod(y, pow2_32);	/* mod 2^32 */
    ptime->timeh = (unsigned int) ldexp(y, -(sizeof(Bit) * 8));	/* div 2^32 */
    return x;
}

/* process the `timescale directive; save info in global table.  Subsequent
   module definitions will copy the global data into the module block.
   Return non-zero if successful. */

int process_timescale(void)
{
    int magnitude, unit, prec, u;
    char buffer[32];
    char *ptr;
    int c;
    if (current_scope) {
	warning("`timescale directive must be outside a module", NULL,
		NULL);
	return 0;
    }

    if (timescale_global.any == T_NO) {
	error("Modules defined before `timescale encountered", NULL, NULL);
	return 0;
    }
    c = fin->fgetc();
    while (strchr(" \t", c)) {
	c = fin->fgetc();
    }
    ptr = buffer;
    while (isdigit(c) && ptr < buffer + sizeof(buffer) - 1) {
	*ptr++ = c;
	c = fin->fgetc();
    }
    *ptr = 0;
    magnitude = atoi(buffer);
    unit = magnitude == 1 ? 0 :
	magnitude == 10 ? -1 : magnitude == 100 ? -2 : 1;

    if (unit == 1) {
	error("`timescale unit spec must be 1, 10 or 100", NULL, NULL);
	return 0;
    }
    while (strchr(" \t", c)) {
	c = fin->fgetc();
    }
    ptr = buffer;
    while (isalpha(c) && ptr < buffer + sizeof(buffer) - 1) {
	*ptr++ = c;
	c = fin->fgetc();
    }
    *ptr = 0;
    u = timescaleunits(buffer);	/* Convert units to number */
    unit -= u;

    if (u == 1) {
	error("`timescale units must be s, ms, us, ps, ot fs", NULL, NULL);
	return 0;
    }
    while (strchr(" \t", c)) {
	c = fin->fgetc();
    }
    if (c != '/') {
	error("`timescale precision spec is missing /", NULL, NULL);
    }
    c = fin->fgetc();
    while (strchr(" \t", c)) {
	c = fin->fgetc();
    }
    ptr = buffer;
    while (isdigit(c) && ptr < buffer + sizeof(buffer) - 1) {
	*ptr++ = c;
	c = fin->fgetc();
    }
    *ptr = 0;
    magnitude = atoi(buffer);
    prec = magnitude == 1 ? 0 :
	magnitude == 10 ? -1 : magnitude == 100 ? -2 : 1;

    if (prec == 1) {
	error("`timescale precision spec must be 1, 10 or 100", NULL,
	      NULL);
	return 0;
    }
    while (strchr(" \t", c)) {
	c = fin->fgetc();
    }
    ptr = buffer;
    while (isalpha(c) && ptr < buffer + sizeof(buffer) - 1) {
	*ptr++ = c;
	c = fin->fgetc();
    }
    *ptr = 0;
    u = timescaleunits(buffer);	/* Convert units to number */
    prec -= u;

    if (u == 1) {
	error("`timescale precision must be s, ms, us, ps, or fs", NULL,
	      NULL);
	return 0;
    }
    if (isspace(c)) {
	fin->fungetc(c);
    }
    timescale_global.unit = unit;
    timescale_global.prec = prec;

    /* keep track of the smallest precision in the model. */

    timescale_global.smallest_scale =
	MAX(timescale_global.smallest_scale, prec);
    timescale_global.any = T_YES;	/* into timescales  */
    return 1;
}

/* This is called from build_module to set up timescales or the module */

void module_timescale(tree module)
{
    if (timescale_global.any == T_NONE || timescale_global.any == T_NO) {
	MODULE_TIME_UNIT(module) = 0;
	MODULE_TIME_PREC(module) = 0;
	timescale_global.any = T_NO;	/* no timescale; don't allow any now */
    } else {
	MODULE_TIME_UNIT(module) = timescale_global.unit;
	MODULE_TIME_PREC(module) = timescale_global.prec;
    }
}

/* Modify the tree structure for delay to compensate for timescales,
   constant folding, and real conversion. This is called by pass3_delay
   and pass3_delay_stmt.
   */

tree *timescale_scale(tree delay)
{
    tree const_node;		/* constant node for folding */
    Bit intdelay;
    double multiplier;		/* precision multiplier */
    double tmpfloat;
    Group *val;
    nbits_t nbits;
    tree *expr;

    multiplier = timescale_precision(current_scope);

    /* Scale the delay using the units and the smallest precision of
       each timescale directive in the model.  If scaling isn't necessary,
       just convert from real, if necessary, and return.  If it is, multiply by
       a power of 10.  If the delay is a constant, do it here.  If not,
       insert a unary operator to the expression.
     */

    /* Note that hierarchical references will have the constant bit enabled.
       Make sure that this isn't counted! */
    if (TREE_CONSTANT_ATTR(delay) && TREE_CODE(delay) != IDENTIFIER_NODE) {
	val = get_const(delay, &nbits);
	// ??? mdh - this needs to convert to 64 bits cleanly
	if (TREE_REAL_ATTR(delay)) {
	    tmpfloat = REAL_(val) * multiplier + 0.5;
	    intdelay = (unsigned int) tmpfloat;
	} else {
	    if (!BVAL(val))
		intdelay = AVAL(val) * (unsigned int) multiplier;
	    else
		intdelay = 0;	/* delay had X or Z */
	}
	const_node = build_int_cst(intdelay);
	// force all time to 64 bits
	expr = pass3_expr(const_node);	/* Use this for the delay expression */
	adjust_nbits(64, &const_node, expr);
	return expr;
    }

    /* Not a constant, and requires scaling.  Add the scaling op (which will
       work with both real and integral delays).
     */

    if (multiplier != 1) {
	// ??? mdh - time needs to 64 bits
	delay =
	    build_binary_op(MULT_EXPR, delay, build_int_cst(multiplier));
    }

    /* return the code for either scaled or unscaled delays */

    expr = pass3_expr_convert(delay, eINT);
    // force all time to 64 bits
    adjust_nbits(64, &delay, expr);
    return expr;
}

/* This is for $timeformat */


/* Execute $timeformat system call.  Assumes pass3 checked everything out. */
#if 0
void time_format(int units, int prec, char *suffix, int width)
{
    timeformat.units = units;
    timeformat.precision = prec;
    timeformat.width = width;

    if (timeformat.suffix)
	free(timeformat.suffix);

    timeformat.suffix = (char *) xmalloc(strlen(suffix) + 2);
    if (!timeformat.suffix) {
	error("Not enough memory to save timeformat suffix!", NULL, NULL);
	return;
    }
    strcpy(timeformat.suffix, suffix);
}
#endif
int get_tick_precision()
{
    return (timescale_global.smallest_scale);
}


void init_timescale()
{
    timescale_global.smallest_scale = 0;
    timescale_global.any = T_NONE;


}
