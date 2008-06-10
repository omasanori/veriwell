/*****************************************************************************
 * Copyright 2008, Mark Hummel
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

/* veriwellinterface.cc - external interface to core simulator */

#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <signal.h>
#include "veriwellinterface.h"

extern char *veriuser_version_str;
#define CPS sysconf(_SC_CLK_TCK)

namespace veriwell {


Simulator simulator;
char *versionString = VERSION;
char copyright[] = "Copyright (C) 1993-2008 Elliot Mednick and Mark Hummel\n\n"
             "Veriwell comes with ABSOLUTELY NO WARRANTY; This is free\n"
             "software, and you are welcome to redistribute it under the\n"
             "terms of the GNU General Public License as published by\n"
             "the Free Software Foundation; either version 2 of the License,\n"
             "or (at your option) any later version. "
             "\n\n";

static clock_t mclock()
{
    struct tms tms;
    times(&tms);
    return tms.tms_utime + tms.tms_stime;
}


/*
 * Initialize simulator
 */
void Simulator::Init( int argc, char** argv )
{
    veriwell::globalArgc = argc;
    veriwell::globalArgv = argv;
    veriwell::init_check();
    veriwell::init_copy();
    veriwell::init_decl();
    veriwell::init_flags();
    veriwell::init_io();
    veriwell::init_print();
    veriwell::init_sched();
    veriwell::init_tree_1();
    veriwell::init_eval();
    veriwell::init_pass3();
    veriwell::init_exec();
    veriwell::init_store();
    veriwell::init_pass2();
    veriwell::Usertask_Init(argc, argv);
    veriwell::init_systask_1();
    veriwell::init_trace();
    veriwell::init_multdiv();
    veriwell::init_dumpvar();
    veriwell::init_strobe();
    veriwell::init_scope();
    veriwell::init_udp();
    veriwell::init_timescale();
    veriwell::InitNSched();
    veriwell::init_verisys();
    init_pli();
    init_tree();
}

/*
 * Precomilation initialization
 */
void Simulator::Precompile()
{
    time_t t = time(NULL);

    /* Print out our header */
    printf_V("\n\n%s version %s, \n", VERIWELL, VERSION );
    printf_V( copyright );
    printf_V(veriuser_version_str);

    init_interactive();

    clock_start = mclock();
    clock_compile = 0;
    clock_load = 0;
    clock_simulate = 0;
}

/*
 * Compilation stage
 */
void Simulator::CompileI()
{
    clock_compile = mclock() - clock_start;
    if (!errorcount) {
        printf_V("\nEntering Phase II...\n");
        build_hierarchy();
    }
    if (!errorcount) {
        printf_V("Entering Phase III...\n");
        pass3_tree(top_level);
        /*
         * allocate extra size for sysfunctions
         * - mdh ??? doesn't appear as easy as this
         *   as this seem to create memory corruption issues
         */
    //    max_label += 64;
    //    stack_size += 1024;
        stack_allocate();
    }
#if VDEBUG != 0
    if (vdebug)
        print_stats();
#endif

    if (warningcount) {
        if (warningcount == 1) {
            printf_V("1 warning in compilation\n");
        } else {
            printf_V("%d warnings in compilation\n", warningcount);
        }
    }
    if (errorcount) {
        if (errorcount == 1) {
            printf_V("1 error in compilation\n");
        } else {
            printf_V("%d errors in compilation\n", errorcount);
        }
        goto DONE;
    }
    printf_V("No errors in compilation\n");
DONE:
    clock_load = mclock() - clock_start - clock_compile;

}

static void Breaker(int sig_number)
{
    if (!in_simulation) {
        fputs("Compilation terminated by user\n", stdout);
        fflush(stdout);
        exit(1);
    } else {
        normal_flag = 0;
        break_flag = 1;
        signal(SIGINT, Breaker);
    }
}


/*
 * Simulate code
 */
void Simulator::Simulate()
{
    if( errorcount ) {
	return;
    }
    signal(SIGINT,Breaker);
    initialize_scope(top_level);
    initialize_gates();
    initialize_cont_assignments();
    tree save_pc = dispatch_pc((enum which_list) NULL);
    init_interactive();
    print_top_modules();
    in_simulation = 1;
    broadcast_tf(reason_endofcompile);
    fin = File::Stdin();
    exec_(save_pc);
    signal(SIGINT,SIG_DFL);
}

/*
 * Print simulation statistics
 */

void Simulator::PrintInfo(void)
{
    clock_t clock_current = mclock();

    clock_simulate = clock_current - clock_start - clock_compile -
        clock_load - clock_pause;

    printf_V("%d Error", errorcount);
    if (errorcount != 1) {
        printf_V("%c", (int) 's');
    }
    printf_V(", %d Warning", warningcount);
    if (warningcount != 1) {
        printf_V("%c", (int) 's');
    }
    printf_V(", Compile time = %.1f, Load time = %.1f,"
             " Simulation time = %.1f\n",
             (double) clock_compile / CPS, (double) clock_load / CPS,
             (double) clock_simulate / CPS);
}

/*
 * increment error count
 */
int Simulator::CountError( int warning )
{
    if (warning) {
        warningcount++;
    } else {
        errorcount++;
    }
    return 1;
}

}
