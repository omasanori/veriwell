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

/* GLUE.C - Dummy stubs for easy interface into other environments */

/******************************************************************************
 Glue.c

                This file contains dummy stubs for the glue routines to interface the shell
                to the back end.
        
        
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "vtypes.h"
#include "tree.h"
#include "glue.h"
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include "copy.h"
#include "eval.h"
#include "exec.h"
#include "lex.h"
#include "macro.h"
#include "multdiv.h"
#include "acc_user.h"
#include "pli.h"
#include "schedule.h"
#include "pass3.h"
#include "pass2.h"
#include "print.h"
#include "scope.h"
#include "store.h"
#include "strobe.h"
#include "systask.h"
#include "timescal.h"
#include "trace.h"
#include "udp.h"
#include "check.h"
#include "decl.h"
#include "flags.h"
#include "io.h"
#include "parse.h"
#include "dumpvar.h"
#include "verisys.h"
#include "veriwell.h"
#include "nsched.h"
#include "usertask.h"





File *File::stdin_ = new File(stdin);
static long totalAllocated;

typedef struct mem_header_tag {
    struct mem_header_tag *next;
    struct mem_header_tag *prev;
    unsigned long size;
    char signature[4];
} MemHeader, *MemHeaderPtr;

MemHeader memList;
/******************************************************************************
 shellinit

                This routine will setup the inteface between the shell and the back end.
        
        
 ******************************************************************************/

void shellinit()
{
    totalAllocated = 0;
    memList.next = &memList;
    memList.prev = &memList;
}

/******************************************************************************
 shell_fopen

                This is the emulation routine for openning a file using the search list.
        
        
 ******************************************************************************/
FILE *shell_fopen(char *name, char *mode)
{
    return (fopen(name, mode));
}

/******************************************************************************
 shell_close

                This is the emulation routine for a file.
        
        
 ******************************************************************************/
void shell_fclose(FILE * file)
{
#undef fclose
    fclose(file);
}

/******************************************************************************
 shell_exit

                extended exit handler
        
        
 ******************************************************************************/
void shell_exit(int reason)
{
    exit(reason);
}

/******************************************************************************
 shell_abort

                Extended abort handler
        
        
 ******************************************************************************/
void shell_abort(void)
{
    abort();
}

/******************************************************************************
 main

                This routine will be called to start the back end.
        
        
 ******************************************************************************/

int main(int argc, char **argv)
{
    // initialize verwell module in case of reentry

    shellinit();
    init_check();
    init_copy();
    init_decl();
    init_flags();
    init_io();
    init_macro_1();
    init_print();
    init_sched();
    init_tree_1();
    init_parse();
    init_eval();
    init_pass3();
    init_exec();
    init_store();
    init_lex_1();
    init_pass2();
    Usertask_Init(argc, argv);
    init_systask_1();
    init_verilog();
    init_trace();
    init_multdiv();
    init_dumpvar();
    init_strobe();
    init_scope();
    init_udp();
    init_timescale();
    InitNSched();
    init_verisys();
    init_pli();
    __main_v(argc, argv);
}


/******************************************************************************
 shell_process

                This routine is called periodically to give the shell time.
        
        
 ******************************************************************************/
void shell_process()
{
    ftell(stdin);
}

/******************************************************************************
 shell_signal

                This routine intervenes in signal handling.
        
        
 ******************************************************************************/
sig_handler shell_signal(int sig, void (*func) (int))
{
    return (signal(sig, func));
}

int shell_getchar(void)
{
#undef getchar
    return (getchar());
}

int shell_vprintf(const char *format, va_list argptr)
{
#undef vprintf
    return (vprintf(format, argptr));
}

int shell_vfprintf(FILE * fp, const char *format, va_list argptr)
{
#undef vfprintf
    return (vfprintf(fp, format, argptr));
}

void shell_ungetc(int c, FILE * f)
{
#undef ungetc
    ungetc(c, f);
}

void shell_fflush(FILE * f)
{
#undef fflush
    fflush(f);
}

int shell_printf(const char *fmt, ...)
{
    va_list args;
    int result;

#undef vprintf
    va_start(args, fmt);
    result = vprintf(fmt, args);
    va_end(args);
    return (result);
}
