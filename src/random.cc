/*****************************************************************************
 * Copyright 1994-2008, Elliot Mednick and Mark Hummel
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
/*
 * Algorithm for probabilistic distribution functions.
 *
 * IEEE Std 1364-2005 Verilog Hardware Description Language (HDL)
 */
#include <limits.h>
#include <math.h>
#include <veriuser.h>
#include <acc_user.h>

static double uniform( int *seed, int start, int end );
static double normal( int *seed, int mean, int deviation);
static double exponential( int *seed, int mean);
static int poisson( int *seed, int mean);
static double chi_square( int *seed, int deg_of_free);
static double t( int *seed, int deg_of_free);
static double erlangian( int *seed, int k, int mean);

void warning(char*,char*,char*);


int
rtl_dist_chi_square( int *seed, int df )
{
    double r;
    int i;

    if(df>0)
    {
        r=chi_square(seed,df);
        if(r>=0)
        {
            i=(int)(r+0.5);
        }
        else
        {
            r = -r;
            i=(int)(r+0.5);
            i = -i;
        }
    }
    else
    {
	warning("Chi_square distribution must have positive degree of freedom\n",0,0);
        i=0;
    }

    return (i);
}

int
rtl_dist_erlang( int *seed, int k, int mean )
{
    double r;
    int i;

    if(k>0)
    {
        r=erlangian(seed,k,mean);
        if(r>=0)
        {
            i=(int)(r+0.5);
        }
        else
        {
            r = -r;
            i=(int)(r+0.5);
            i = -i;
        }
    }
    else
    {
	warning("k-stage erlangian distribution must have positive k\n",0,0);
        i=0;
    }

    return (i);
}

int
rtl_dist_exponential( int* seed, int mean )
{
    double r;
    int i;

    if(mean>0)
    {
        r=exponential(seed,mean);
        if(r>=0)
        {
            i=(int)(r+0.5);
        }
        else
        {
            r = -r;
            i=(int)(r+0.5);
            i = -i;
        }
    }
    else
    {
	warning("Exponential distribution must have a positive mean\n",0,0);
        i=0;
    }

    return (i);
}

int
rtl_dist_normal( int* seed, int mean, int sd )
{
    double r;
    int i;

    r=normal(seed,mean,sd);
    if(r>=0)
    {
        i=(int)(r+0.5);
    }
    else
    {
        r = -r;
        i=(int)(r+0.5);
        i = -i;
    }

    return (i);
}

int
rtl_dist_poisson( int* seed, int mean )
{
    int i;

    if(mean>0)
    {
        i=poisson(seed,mean);
    }
    else
    {
	warning("Poisson distribution must have a positive mean\n",0,0);
        i=0;
    }

    return (i);
}

int
rtl_dist_t( int* seed, int df )
{
    double r;
    int i;

    if(df>0)
    {
        r=t(seed,df);
        if(r>=0)
        {
            i=(int)(r+0.5);
        }
        else
        {
            r = -r;
            i=(int)(r+0.5);
            i = -i;
        }
    }
    else
    {
	warning("t distribution must have positive degree of freedom\n",0,0);
        i=0;
    }

    return (i);
}

int
rtl_dist_uniform(int* seed, int start, int end)
{
	double r;
	int i;

	if (start >= end) return(start);

	if (end != INT_MAX)
	{
		end++;
		r = uniform( seed, start, end );
		if (r >= 0)
		{
			i = (int) r;
		}
		else
		{
			i = (int) (r-1);
		}
		if (i<start) i = start;
		if (i>=end) i = end-1;
	}
	else if (start!=INT_MIN)
	{

		start--;
		r = uniform( seed, start, end) + 1.0;
		if (r>=0)
		{
			i = (int) r;
		}
		else
		{
			i = (int) (r-1);
		}
		if (i<=start) i = start+1;
		if (i>end) i = end;
	}
	else
	{
		r = (uniform(seed,start,end)+2147483648.0)/4294967295.0;
		r = r*4294967296.0-2147483648.0;
		if (r>=0)
		{
			i = (int) r;
		}
		else
		{
			i = (int) (r-1);
		}
	}

	return (i);
}

static double
uniform( int* seed, int start, int end )
{
	union u_s
	{
		float s;
		unsigned stemp;
	} u;

	double d = 0.00000011920928955078125;
	double a,b,c;

	if ((*seed) == 0)
		*seed = 259341593;

	if (start >= end)
	{
		a = 0.0;
		b = 2147483647.0;
	}
	else
	{
		a = (double) start;
		b = (double) end;
	}

	*seed = 69069 * (*seed) + 1;
	u.stemp = *seed;

	/*
	 * This relies on IEEE floating point format
	*/
	u.stemp = (u.stemp >> 9) | 0x3f800000;

	c = (double) u.s;

	c = c+(c*d);
	c = ((b - a) * (c - 1.0)) + a;

	return (c);
}


static double
normal(int *seed,int mean,int deviation)
{
    double v1,v2,s;

    s = 1.0;
    while((s >= 1.0) || (s == 0.0))
    {
        v1 = uniform(seed,-1,1);
        v2 = uniform(seed,-1,1);
        s = v1 * v1 + v2 * v2;
    }
    s = v1 * sqrt(-2.0 * log(s) / s);
    v1 = (double) deviation;
    v2 = (double) mean;
    return(s * v1 + v2);
}

static double
exponential(int* seed,int mean)
{
    double n;

    n = uniform(seed,0,1);
    if(n != 0)
    {
        n = -log(n) * mean;
    }
    return(n);
}

static int
poisson(int* seed, int mean)
{
    int n;
    double p,q;

    n = 0;
    q = -(double)mean;
    p = exp(q);
    q = uniform(seed,0,1);
    while(p < q)
    {
        n++;
        q = uniform(seed,0,1) * q;
    }
    return(n);
}

static double
chi_square(int* seed,int deg_of_free)
{
    double x;
    int k;
    if(deg_of_free % 2)
    {
        x = normal(seed,0,1);
        x = x * x;
    }
    else
    {
        x = 0.0;
    }
    for(k = 2;k <= deg_of_free;k = k + 2)
    {
        x = x + 2 * exponential(seed,1);
    }
    return(x);
}

static double
t(int* seed,int deg_of_free)
{    
    double x;
    double chi2 = chi_square(seed,deg_of_free);
    double div = chi2 / (double)deg_of_free;
    double root = sqrt(div);
    x = normal(seed,0,1) / root;
    return(x);
}

static double
erlangian(int* seed,int k,int mean)
{
    double x,a,b;
    int i;

    x=1.0;
    for(i=1;i<=k;i++)
    {
        x = x * uniform(seed,0,1);
    }
    a=(double)mean;
    b=(double)k;
    x= -a*log(x)/b;
    return(x);
}

/*
 * pli interfaces
 */
/********************************************
 * dist_uniform
 *******************************************/
int dist_uniform(int user, int reason)
{
    const char name[] = "dist_uniform";
    const int ARG_COUNT = 3;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int imin = acc_fetch_tfarg_int(2);
	    int imax = acc_fetch_tfarg_int(3);
	    int value = rtl_dist_uniform(&seed, imin, imax);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}
/********************************************
 * dist_normal
 *******************************************/
int dist_normal(int user, int reason)
{
    const char name[] = "dist_normal";
    const int ARG_COUNT = 3;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int mean = acc_fetch_tfarg_int(2);
	    int sd = acc_fetch_tfarg_int(3);
	    int value = rtl_dist_normal(&seed, mean, sd);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}
/********************************************
 * dist_exponential
 *******************************************/
int dist_exponential(int user, int reason)
{
    const char name[] = "dist_exponential";
    const int ARG_COUNT = 2;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int mean = acc_fetch_tfarg_int(2);
	    int value = rtl_dist_exponential(&seed, mean);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}
/********************************************
 * dist_poisson
 *******************************************/
int dist_poisson(int user, int reason)
{
    const char name[] = "dist_poisson";
    const int ARG_COUNT = 2;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int mean = acc_fetch_tfarg_int(2);
	    int value = rtl_dist_poisson(&seed, mean);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}
/********************************************
 * dist_chi_square
 *******************************************/
int dist_chi_square(int user, int reason)
{
    const char name[] = "dist_chi_square";
    const int ARG_COUNT = 2;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int dof = acc_fetch_tfarg_int(2);
	    int value = rtl_dist_chi_square(&seed, dof);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}
/********************************************
 * dist_t
 *******************************************/
int dist_t(int user, int reason)
{
    const char name[] = "dist_t";
    const int ARG_COUNT = 2;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int dof = acc_fetch_tfarg_int(2);
	    int value = rtl_dist_t(&seed, dof);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}

/********************************************
 * dist_erlang
 *******************************************/
int dist_erlang(int user, int reason)
{
    const char name[] = "dist_erlang";
    const int ARG_COUNT = 3;
    int result = 0;
    int argCount = tf_nump();
    handle args[ARG_COUNT];

    acc_initialize();

    switch( reason ) {
    case reason_sizetf:
	result = 32;
	break;
    case reason_checktf: {
	    if( argCount != ARG_COUNT ) {
	        tf_error("illegal number of arguments to %s", name);
	    }
    	    for (int i = 1; i <= argCount; i++) {
	        args[i-1] = acc_handle_tfarg(i);	
	        if( acc_error_flag ) {
	            tf_error("illegal argument #%d to %s", i, name);
	        }
	    }
	    if( acc_fetch_type(args[0]) != accReg &&
		acc_fetch_type(args[0]) != accTimeVar && 
		acc_fetch_type(args[0]) != accIntVar ) {
	        tf_error("illegal argument 0 to %s", name);
	    }	
        } break;
    case reason_calltf: {
	    int seed = acc_fetch_tfarg_int(1);
	    int k_stage = acc_fetch_tfarg_int(2);
	    int mean = acc_fetch_tfarg_int(3);
	    int value = rtl_dist_normal(&seed, k_stage, mean);
	    tf_putp(1,seed);
	    tf_putp(0,value);
	} break;
    } 

    acc_close();
    return result;
}

