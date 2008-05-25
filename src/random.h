/*
 * Algorithm for probabilistic distribution functions.
 *
 * IEEE Std 1364-2005 Verilog Hardware Description Language (HDL)
 */
#ifndef RANDOM_H
#define RANDOM_H

int rtl_dist_chi_square( int* seed, int df );
int rtl_dist_erlang( int* seed, int k, int mean );
int rtl_dist_exponential( int* seed, int mean );
int rtl_dist_normal( int* seed, int mean, int sd );
int rtl_dist_poisson( int* seed, int mean );
int rtl_dist_t( int* seed, int df );
int rtl_dist_uniform(int* seed, int start, int end);

#endif
