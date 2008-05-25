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

/* veriwell.h - veriwell.c routine prototypes */

#ifndef VERIWELL_H
#define VERIWELL_H

// public methods

extern int globalArgc;
extern char **globalArgv;

void runtime_error(tree node);
void print_info(void);
void finish(void);
void flush_files(void);
void synopsys_warn(char *s);
void init(void);
int count_error(int warningp);
void fatal(char *s, char *v);
void report_error_function(char *file);
void error_with_file_and_line(char *file, lineno_t line, char *s,
			      char *v, char *v2);
void error(char *s, char *v, char *v2);
void warning_with_file_and_line(char *file, lineno_t line, char *s,
				char *v, char *v2);
void warning(char *s, char *v, char *v2);
void sorry_with_file_and_line(char *file, lineno_t line, char *s,
			      char *v, char *v2);
void sorry(char *s);
void sorry1(char *s, char *v);
char *xmalloc(unsigned size);
char *xrealloc(char *ptr, int size);
int test_file(char *filename);
void Cmdline(int argc, char **argv);
void process_cmdline(char *input);
int moreinput(char *infile);
void CmdlineHelp(void);
void PhaseI(void);
void PhaseII(void);
void PhaseIII(void);
void segv(int);
void breaker(int sig_number);
void checkdate(void);
void __main_v(int argc, char **argv);
void print_info(void);
void finish(void);
unsigned long mem_avail(void);
void heap_test(char *msg);
void init_verilog(void);
void _Assert(char *strFile, unsigned uLine);

#endif				// VERIWELL_H
