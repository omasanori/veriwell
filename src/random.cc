/*
 * Algorithm for probabilistic distribution functions.
 *
 * IEEE Std 1364-2005 Verilog Hardware Description Language (HDL)
 */
#include <limits.h>
#include <math.h>

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


