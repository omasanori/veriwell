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

/* SYSTASK.H - Enums for all internal system tasks and functions */

/* These are temporary until PLI is used internally */

#ifndef SYSTASK_H
#define SYSTASK_H

// public methods

#define NUM_SYSTEM_TASKS 0
#define NUM_SYSTEM_FUNCS 0

enum systask_type {
    NO_SYSTYPE = 0,
    DISPLAY,
    DISPLAYH,
    DISPLAYB,
    DISPLAYO,
    WRITE,
    WRITEH,
    WRITEB,
    WRITEO,
    FDISPLAY,
    FDISPLAYH,
    FDISPLAYB,
    FDISPLAYO,
    T_FWRITE,
    T_FWRITEH,
    T_FWRITEB,
    T_FWRITEO,
    MONITOR,
    MONITORB,
    MONITORH,
    MONITORO,
    FMONITOR,
    FMONITORB,
    FMONITORH,
    FMONITORO,
    STROBE,
    STROBEB,
    STROBEH,
    STROBEO,
    FSTROBE,
    FSTROBEB,
    FSTROBEH,
    FSTROBEO,
    STOP,
    FINISH,
    SETTRACE,
    CLEARTRACE,
    SCOPE,
    SHOWSCOPES,
    SHOWSTATS,
    T_FCLOSE,
    T_LOG,
    T_NOLOG,
    T_KEY,
    T_NOKEY,
    T_INPUT,
    T_SHOWVARS,
    T_MONITORON,
    T_MONITOROFF,
    T_READMEMH,
    T_READMEMB,
    T_WWAVES,
    T_DUMPVARS,
    T_DUMPON,
    T_DUMPOFF,
    T_DUMPFILE,
    T_DUMPALL,
    T_DUMPFLUSH,
    T_UPDATETIMINGS,
    T_TIMEFORMAT,
    T_GWAVES,
    SYSTEM_TASK,
    USER_TASK,
    LAST_TASK
};

enum sysfunction_type {
//  NO_SYSFTYPE = LAST_TASK,
    NO_SYSFTYPE = 1000,
    F_TIME,
    F_STIME,
    F_REALTIME,
    F_FOPEN,
    F_RANDOM,
    F_TEST_PLUSARGS,
    SYSTEM_FUNC,
    USER_FUNC,
    LAST_FUNCTION
};

struct systask_info {
    char *name;
    enum systask_type type;
};

struct sysfunction_info {
    char *name;
    enum sysfunction_type type;
};


enum systask_type lookup_systask(char *, p_tfcell *);
enum sysfunction_type lookup_sysfunction(char *, p_tfcell *);

extern int roFlag;

void init_sysfunction(tree);
void init_systask(tree);
p_tfcell lookup_user_func(char *);
p_tfcell lookup_user_task(char *);
void call_check_tf(p_tfcell);
void call_misc_tf(p_tfcell, int);
void call_misc_tf1(p_tfcell, int, int);
void call_call_tf(p_tfcell);
int call_size_tf(p_tfcell);
int count_args(tree);
void exec_systask(tree node);
void exec_sysfunc(tree node, nbits_t);
void monitor_args(tree node, struct monitor_info *monitor_info);
int disp_common(tree node, enum radii default_radix, int is_file,
		int check_only);
tree print_next_arg(handle_t handle, tree node, enum radii radix,
		    int check, int is_integer, int fill_space, char *spec,
		    tree);
void broadcast_tf(int reason);
void showvar(tree decl);
void do_readmem(tree node, enum radii radix);
void init_systask_1(void);

p_tfcell lookup_user_tf(char *name);
void bits_to_string(char *buffer, Group * g, nbits_t nbits);
handle_t get_handle(tree * code);
tree init_function_return(char *name, int size, enum tree_code);
int args_one(int num_args, char *taskname);
int args_zero(int num_args, char *taskname);
void arg_common(tree args);
void do_stop(void);
void do_finish(void);

void pli_asynccall(PliInfo_t *);
int args_zero_or_one(int, char *);
void systaskDoDelay(tree);
int call_size_tf(p_tfcell p_tf);

// private methods

#ifdef SYSTASK_C
#endif				// SYSTASK_C

#endif				// SYSTASK_H
