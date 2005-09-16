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

/* IO.H - IO prototypes */

#ifndef IO_H
#define IO_H

#include <stdarg.h>

// public methods

/*
 * LibPath_t
 * - structure to maintain linked list of library paths 
 */

typedef struct LibPath_t {
    struct LibPath_t *next;
    char *path;
} LibPath_t;


#define NHANDLES 31
//static FILE *file_handles [NHANDLES];
//static int file_used [NHANDLES];

void printf_V_log(char *, ...);
void printf_V(char *, ...);
void printf_error_V(char *, ...);
void printf_V_key(char *, ...);
void fprintf_V(handle_t, char *, ...);
//void  vprintf_V (char *, void *);
//void  vfprintf_V (handle_t, char *, void *);
void vprintf_V_log(char *, va_list);
void vprintf_V(char *, va_list);
void vfprintf_V(handle_t, char *, va_list);
handle_t fopen_V(char *);
handle_t check_handle(handle_t);
void fclose_V(handle_t);
//tree  check_library (char *, int);
void fflush_V(handle_t);

FILE *fopenbuf(char *, int);

void init_io(void);
//tree  check_library (char *name, int numargs);

class File;
extern File *fin;
extern char *input_filename;
extern char *filename;

extern FILE *log_file;
extern FILE *key_file;
extern char *log_file_name;
extern char *key_file_name;
extern int log_enable;
extern int log_available;
extern int key_enable;
extern int key_available;
extern char *ypath;
extern char *ylibext;
extern char *incdir;
extern int global_print_override;
extern char *global_print_buffer;
extern char *global_print_p;

// private methods

#ifdef IO_C
#endif				//IO_C

#endif				//IO_H
