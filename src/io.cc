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

/* IO.C - Compiler-dependent input/output routines */

/* fbufopen "opens" a buffer so that getc will read from it as if it
   were a file */

#define IO_C

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "vtypes.h"
#include "tree.h"
#include "macro.h"
#include "io.h"
#include "glue.h"
#include "lex.h"
#include "veriwell.h"

File *fin;			/* current input file */
char *input_filename;
char *filename;

int global_print_override;
char *global_print_buffer;
char *global_print_p;

FILE *log_file;
FILE *key_file;
char *log_file_name;
char *key_file_name;
int log_enable = 1;
int log_available = 0;
int key_enable = 1;
int key_available = 0;

LibPath_t *ypathList;
char *ylibext;
char *incdir;

FILE *file_handles[NHANDLES];
int file_used[NHANDLES];
char print_buffer[2000];


void fclose_V(handle_t handle)
{
    int i;
    handle_t tmp_handle;

    if (handle & 1)
	warning("Cannot close standard output", NULL_CHAR, NULL_CHAR);

    if (handle - 1) {
	tmp_handle = handle >> 1;
	for (i = 0; i < NHANDLES; i++)
	    if (tmp_handle >> i & 1) {
		fclose(file_handles[i]);
		file_used[i] = 0;
	    }
    }
}

/* Open a file, assign one of 31 bits to the handle of the file */

handle_t fopen_V(char *file)
{
    int i;
    FILE *f_handle;

    /* find an available handle */
    for (i = 0; i < NHANDLES; i++)
	if (!file_used[i])
	    break;
    if (i >= NHANDLES) {
	warning("Exeeded limit on number of open files", NULL_CHAR,
		NULL_CHAR);
	return 0;
    }
    f_handle = shell_fopen(file, "w+t");
    if (!f_handle) {
	warning("Cannot open file '%s'", file, NULL_CHAR);
	return 0;
    }
    file_handles[i] = f_handle;
    file_used[i] = 1;
    return 1 << (i + 1);
}

/* Given a handle, test to make sure each handle is valid; filter
   out invalid handles and give warnings if necessary */

handle_t check_handle(handle_t handle)
{
    int i;
    handle_t tmp_handle;

    /* Only check for file handles */
    if (handle - 1) {
	tmp_handle = handle >> 1;
	for (i = 0; i < NHANDLES; i++)
	    if (tmp_handle >> i & 1)
		if (!file_used[i]) {
		    warning("File not open", NULL_CHAR, NULL_CHAR);
		    handle &= ~((handle_t) 1 << (i + 1));
		}
    }
    return handle;
}

void fflush_V(handle_t handle)
{
    int i;
    handle_t tmp_handle;

    /* First, flush stdout (and log file) */
    if (handle & 1) {
	fflush(stdout);
	if (log_enable)
	    fflush(log_file);
    }
    /* If there is some file output, flush it */
    if (handle - 1) {
	tmp_handle = handle >> 1;
	for (i = 0; i < NHANDLES; i++)
	    if (tmp_handle >> i & 1)
		if (file_used[i])
		    fflush(file_handles[i]);
		else
		    warning("File not open", NULL_CHAR, NULL_CHAR);
    }
}

void vfprintf_V(handle_t handle, char *fmt, va_list args)
{
    int i;
    handle_t tmp_handle;

    if (global_print_override) {
	/* Watcom chokes on this... */
	global_print_p += vsprintf(global_print_p, fmt, args);
	return;
    }

    /* Do this because some compilers (and ANSI?) don't like to use "args"
       more than once */

    vsprintf(print_buffer, fmt, args);

    /* First, do output to stdout (and to log file) */
    if (handle & 1) {
	printf(print_buffer);
	if (log_enable)
	    fprintf(log_file, print_buffer);
    }
    /* If there is some file output to do, do it */
    if (handle - 1) {
	tmp_handle = handle >> 1;
	for (i = 0; i < NHANDLES; i++)
	    if (tmp_handle >> i & 1)
		if (file_used[i])
		    fprintf(file_handles[i], print_buffer);
		else
		    warning("File not open", NULL_CHAR, NULL_CHAR);
    }
}

void fprintf_V(handle_t handle, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf_V(handle, fmt, args);
    va_end(args);
}

void vprintf_V(char *fmt, va_list args)
{
    if (global_print_override) {
	global_print_p += vsprintf(global_print_p, fmt, args);
	return;
    } else
	/* Do this because some compilers (and ANSI?) don't like to use "args"
	   more than once */

	vsprintf(print_buffer, fmt, args);

    printf(print_buffer);

    if (log_enable)
	fprintf(log_file, print_buffer);
}

void printf_V(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf_V(fmt, args);
    va_end(args);
}

void vprintf_V_log(char *fmt, va_list args)
{

    vsprintf(print_buffer, fmt, args);

    if (log_enable)
	fprintf(log_file, print_buffer);
}

void printf_V_log(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf_V_log(fmt, args);
    va_end(args);
}

void printf_error_V(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    if (log_enable) {
	va_start(args, fmt);
	vfprintf(log_file, fmt, args);
	va_end(args);
    }
}

void printf_V_key(char *fmt, ...)
{
    va_list args;

    if (!fin->isatty()) {
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
    }
    if (log_enable) {
	va_start(args, fmt);
	vfprintf(log_file, fmt, args);
	va_end(args);
    }
    if (key_enable) {
	va_start(args, fmt);
	vfprintf(key_file, fmt, args);
	va_end(args);
    }
}

void init_io()
{
    incdir = xmalloc(1);
    incdir[0] = 0;
    log_enable = 0;
    log_available = 0;
    key_enable = 0;
    key_available = 0;
    global_print_override = 0;
    global_print_buffer = 0;
    global_print_p = 0;
    ypathList = (LibPath_t *) malloc(sizeof(LibPath_t));
    ypathList->path = ".";
    ypathList->next = NULL;

    memset(&fin, 0, sizeof(fin));
    memset(&input_filename, 0, sizeof(input_filename));
    memset(&filename, 0, sizeof(filename));
    memset(&log_file, 0, sizeof(log_file));
    memset(&key_file, 0, sizeof(key_file));
    memset(&log_file_name, 0, sizeof(log_file_name));
    memset(&file_handles, 0, sizeof(file_handles));
    memset(&file_used, 0, sizeof(file_used));
    memset(&ylibext, 0, sizeof(ylibext));

}
