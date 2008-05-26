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
#ifndef RANDOM_H
#define RANDOM_H

int rtl_dist_chi_square( int* seed, int df );
int rtl_dist_erlang( int* seed, int k, int mean );
int rtl_dist_exponential( int* seed, int mean );
int rtl_dist_normal( int* seed, int mean, int sd );
int rtl_dist_poisson( int* seed, int mean );
int rtl_dist_t( int* seed, int df );
int rtl_dist_uniform(int* seed, int start, int end);
/*
 * pli interfaces
 */
int dist_uniform(int user, int reason);
int dist_uniform(int user, int reason);
int dist_normal(int user, int reason);
int dist_exponential(int user, int reason);
int dist_poisson(int user, int reason);
int dist_chi_square(int user, int reason);
int dist_t(int user, int reason);
int dist_erlang(int user, int reason);

#endif

