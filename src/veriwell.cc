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

/* veriwell.c - Main, parse arguments, misc utilities */

#define VERIWELL_C

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <setjmp.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "veriwell.h"
#include "lex.h"
#include "flags.h"
#include "runtime.h"
#include "macro.h"
#include "io.h"
#include "glue.h"
#include "scope.h"
#include "veriwell.h"
#include "pass3.h"
#include "gates.h"
#include "exec.h"
#include "pass2.h"
#include "acc_user.h"
#include "pli.h"
#include "systask.h"
#include "build.h"

int dump_it = 0;
FILE *dumpfile;
unsigned _ovrbuffer = 0x400;
void *zero_ptr = 0;		/* for monitoring 0000:0000 in debugger */
tree dbg;			/* for debugging */
tree dbg1;			/* for debugging */

extern LibPath_t *ypathList;

char *versionString = VERSION;
char copyright[] = "Copyright (C) 1993-2008 Elliot Mednick and Mark Hummel\n\n"
	     "Veriwell comes with ABSOLUTELY NO WARRANTY; This is free\n"
	     "software, and you are welcome to redistribute it under the\n"
	     "terms of the GNU General Public License as published by\n"
	     "the Free Software Foundation; either version 2 of the License,\n"
	     "or (at your option) any later version. "
	     "\n\n";

#define MAX_LEVEL 25
Bit mask_right[sizeof(Bit) * 8 + 1];
Bit mask_right1[sizeof(Bit) * 8 + 1];

int globalArgc;
char **globalArgv;

clock_t clock_start;
clock_t clock_compile;
clock_t clock_load;
clock_t clock_simulate;
clock_t clock_pause = 0;

/* The current program counter (actually a pointer to a tree node to be
   "executed" */

#define CPS sysconf(_SC_CLK_TCK)
static clock_t mclock()
{
    struct tms tms;
    times(&tms);
    return tms.tms_utime + tms.tms_stime;
}

tree save_pc;

/* The name of this program (Verilog) */
char *progname;

extern int yydebug;		/* was in BISON.SIM; now here to avoid multiple definitions
				   (now that there are two parsers) */

/* Ignore the argument after an unsupported option for now */
int extra_flag = 0;


/* Default net type and delay type (min/typ/max) can be changed by
     compiler directive or command-line option */
enum tree_type default_net_type = NET_WIRE_TYPE;
enum delay_type delay_type = TYP_DELAY;


/* Print the header, check for key/passwd/whatever, init a few other things */
void init()
{
    extern char *veriuser_version_str;
    time_t t = time(NULL);

    /* Print out our header */
    printf_V("\n\n%s version %s, \n", VERIWELL, VERSION );
    printf_V( copyright );
    printf_V(veriuser_version_str);


    /* initialize tree structures */

    /* mask_right is 0:0, 1:1, 2:3, 3:7, 4:15, ..., 32:-1
     * mask_right1 is 0:-1, 1:1, 2:3, ..., 32:0
     */
    mask_right[0] = 0;
    mask_right1[0] = (Bit) - 1;
    mask_right[sizeof(Bit) * 8] = (Bit) - 1;
    mask_right1[sizeof(Bit) * 8] = 0;

    for (int i = 1; i < sizeof(Bit) * 8; i++) {
	mask_right[sizeof(Bit) * 8 - i] = (Bit) - 1 >> i;
	mask_right1[sizeof(Bit) * 8 - i] = (Bit) - 1 >> i;
    }
    if (dump_it)
	dumpfile = shell_fopen("dump.out", "wb");
}

/* Number of error messages and warning messages so far.  */

int errorcount = 0;
int warningcount = 0;

/* Count an error or warning.  Return 1 if the message should be printed.  */

int count_error(int warningp)
{
    if (warningp && inhibit_warnings) {
	return 0;
    }

    if (warningp) {
	warningcount++;
    } else {
	errorcount++;
    }
    return 1;
}

/* Print a fatal error message.  NAME is the text.
   Also include a system error message based on `errno'.  */

void fatal(char *s, char *v)
{
    error(s, v, (char *) NULL);
    shell_exit(34);
}

/* Prints out, if necessary, the name of the current function
   which caused an error.  Called from all error and warning functions.  */

void report_error_function(char *file)
{
}

/* Report an error at line LINE of file FILE.
   S and V are a string and an arg for `printf'.  */

void
error_with_file_and_line(char *file,
			 lineno_t line, char *s, char *v, char *v2)
{
    count_error(0);
    report_error_function(file);
    if (is_interactive) {
	printf_error_V("C%d: ", cmd_level);
	new_cmd_level = cmd_level;
    } else if (file) {
	printf_error_V("%s: L%ld: ", file, line);
    } else if (progname) {
	printf_error_V("VeriWell: ");
    }

    printf_error_V("error: ");
    printf_error_V(s, v, v2);
    printf_error_V("\n");
}

/* Report an error at the current line number.
   S and V are a string and an arg for `printf'.  */

void error(char *s, char *v, char *v2)
{
    error_with_file_and_line(input_filename, lineno, s, v, v2);
}

/* Report a warning at line LINE.
   S and V are a string and an arg for `printf'.  */

void
warning_with_file_and_line(char *file, lineno_t line, char *s, char *v,
			   char *v2)
{
    if (count_error(1) == 0) {
	return;
    }

    report_error_function(file);

    if (file) {
	printf_error_V("%s: L%ld: ", file, line);
    } else {
	printf_error_V("VeriWell: ");
    }
    printf_error_V("warning: ");
    printf_error_V(s, v, v2);
    printf_error_V("\n");
}

/* Report a warning at the current line number.
   S and V are a string and an arg for `printf'.  */

void warning(char *s, char *v, char *v2)
{
    warning_with_file_and_line(input_filename, lineno, s, v, v2);
}

/* Report a warning at line LINE.
   S and V are a string and an arg for `printf'.  */

void
sorry_with_file_and_line(char *file, lineno_t line,
			 char *s, char *v, char *v2)
{
    if (count_error(0) == 0) {
	return;
    }

    report_error_function(file);

    if (file) {
	printf_error_V("%s: L%ld: ", file, line);
    } else {
	printf_error_V("VeriWell: ");
    }
    printf_error_V("sorry: ");
    printf_error_V(s, v, v2);
    printf_error_V("\n");
}

/* Report a warning at the current line number.
   S and V are a string and an arg for `printf'.  */

void sorry(char *s)
{
    sorry_with_file_and_line(input_filename, lineno, s, NULL_CHAR,
			     NULL_CHAR);
}

void sorry1(char *s, char *v)
{
    sorry_with_file_and_line(input_filename, lineno, s, v, NULL_CHAR);
}

/* Warn about incompatibilities with Synopsys 2.0 */

void synopsys_warn(char *s)
{
    char msg[64];

    strcpy(msg, s);
    strcat(msg, " not supported by Synopsys HDL Compiler(tm)");
    warning(msg, NULL_CHAR, NULL_CHAR);
}

void runtime_error(tree node)
{
    input_filename = STMT_SOURCE_FILE(node);
    lineno = STMT_SOURCE_LINE(node);
}


/* Same as `malloc' but report error if no memory available.  */

char *xmalloc(unsigned size)
{
    char *value;
    value = (char *) malloc(size);
    if (value == 0) {
	fatal("Memory exhausted.", NULL);
    }
    return value;
}

/* Same as `realloc' but report error if no memory available.  */

char *xrealloc(char *ptr, int size)
{
    char *result = (char *) realloc(ptr, size);
    if (!result) {
	fatal("Memory exhausted.", NULL);
    }
    return result;
}

typedef struct filelist {
    char *name;
    struct filelist *next;
} Filelist;

static File *i_file;
char i_buf[BUFSIZ];
static Filelist *f_first = 0;
static Filelist *f_last = 0;
static Filelist *source_first = 0;
static Filelist *source_last = 0;


int in_simulation = 0;

/* test file; return 1 if file exists */

int test_file(char *filename)
{
    FILE *tmp = shell_fopen(filename, "r");
    if (!tmp) {
	return 0;
    }
    fclose(tmp);
    return 1;
}

/* Parse the command line */

void Cmdline(int argc, char **argv)
{
    char input[256];
    Filelist *fp;

    /* First, read command line */

    while (--argc > 0) {
	process_cmdline(*++argv);
    }

    if (f_flag) {
	printf_error_V("Specifying -f without a filename is illegal\n");
	CmdlineHelp();
	shell_exit(1);
    }
    if (i_flag) {
	printf_error_V("Specifying -i without a filename is illegal\n");
	CmdlineHelp();
	shell_exit(1);
    }
    if (l_flag) {
	printf_error_V("Specifying -l without a filename is illegal\n");
	CmdlineHelp();
	shell_exit(1);
    }
    if (k_flag) {
	printf_error_V("Specifying -k without a filename is illegal\n");
	CmdlineHelp();
	shell_exit(1);
    }

    /* Open log file */
    if (!log_available) {
	/* set if new file name given in command line */
	log_file_name = (char *) xmalloc(13);
	strcpy(log_file_name, "veriwell.log");
    }
    if (!strcmp(log_file_name, "nolog")) {
	log_file = 0;
	log_available = 0;
	log_enable = 0;
	warning("Proceeding without a log file", NULL_CHAR, NULL_CHAR);
    } else {
	log_file = shell_fopen(log_file_name, "w+t");
	if (!log_file) {
	    log_available = 0;
	    log_enable = 0;
	    warning("Cannot open log file '%s', proceeding without it",
		    log_file_name, NULL_CHAR);
	} else {
	    log_available = 1;
	    log_enable = 1;
	}
    }

    /* Open key file */
    if (!key_available) {
	/* set if new file name given in command line */
	key_file_name = (char *) xmalloc(13);
	strcpy(key_file_name, "veriwell.key");
    }
    if (!strcmp(key_file_name, "nokey")) {
	key_file = 0;
	key_available = 0;
	key_enable = 0;
	warning("Proceeding without a key file", NULL_CHAR, NULL_CHAR);
    } else {
	key_file = shell_fopen(key_file_name, "w+t");
	if (!key_file) {
	    key_available = 0;
	    key_enable = 0;
	    warning("Cannot open key file '%s', proceeding without it",
		    key_file_name, NULL_CHAR);
	} else {
	    key_available = 1;
	    key_enable = 1;
	}
    }

    init_macro();
    init_input();

    /* Now, parse all cmd arg files */

    for (fp = f_first; fp != (Filelist *) 0; fp = fp->next) {
	int c;
	printf_V("Processing command file : %s\n", fp->name);
	push_stream(fin, 0);
	input_filename = fp->name;
	lineno = 0;
	if ((fin = File::fopen(fp->name, "r")) == NULL) {
	    printf_error_V("%s: can't open %s\n", progname, fp->name);
	    shell_exit(1);
	}
	while ((c = skip_white_space(fin->fgetc())) != EOF) {
	    fin->fungetc(c);
	    fin->fscanf("%s", input);
	    process_cmdline(input);
	}
	if (f_flag) {
	    printf_error_V
		("Specifying -f without a filename is illegal\n");
	    CmdlineHelp();
	    shell_exit(1);
	}
	if (i_flag) {
	    printf_error_V
		("Specifying -i without a filename is illegal\n");
	    CmdlineHelp();
	    shell_exit(1);
	}

	if (fin) {
	    /* skipwhitespace will close this, but let's make sure */
	    fin->fclose();
	}
	pop_stream();
    }

    init();
    init_interactive();

    /* Now, parse all source files */
    clock_start = mclock();

    if (!errorcount) {
	printf_V("Entering Phase I...\n");
	for (fp = source_first; fp != (Filelist *) 0; fp = fp->next) {
	    input_filename = fp->name;
	    moreinput(fp->name);
	    printf_V("Compiling source file : %s\n", fp->name);
	    PhaseI();
	}
    }
    clock_compile = mclock() - clock_start;
    if (!errorcount) {
	printf_V("\nEntering Phase II...\n");
	PhaseII();
    }
    if (!errorcount) {
	printf_V("Entering Phase III...\n");
	PhaseIII();
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
	shell_exit(1);
    }
    printf_V("No errors in compilation\n");
    clock_load = mclock() - clock_start - clock_compile;

    if (!simulate) {
	shell_exit(0);
    }
    initialize_scope(top_level);
    initialize_gates();
    initialize_cont_assignments();
    save_pc = dispatch_pc((enum which_list) NULL);
    init_interactive();
    print_top_modules();
    in_simulation = 1;
    broadcast_tf(reason_endofcompile);
    fin = File::Stdin();
    if (i_file) {
	push_stream(fin, 0);
	fin = i_file;
    }
}


void process_cmdline(char *input)
{
    Filelist *fp;
    char c;
    int f_nest = 0;
    int extra_flag = 0;

    /*
     * Consider the "-f" filename case
     */
    if (f_flag) {
	if (!test_file(input)) {
	    fatal("Could not open (-f) input file '%s'\n", input);
	}
	f_flag = 0;
	fp = (Filelist *) xmalloc(sizeof(Filelist));
	fp->name = (char *) xmalloc(strlen(input) + 1);	/* +1 for '/0' */
	strcpy(fp->name, input);
	fp->next = (Filelist *) 0;
	if (f_last) {
	    f_last->next = fp;
	} else {
	    f_first = fp;
	}
	f_last = fp;
    } else if (i_flag) {
	if ((i_file = File::fopen(input, "r")) == 0) {
	    fatal("Could not open (-i) input file '%s'\n", input);
	}
	i_flag = 0;
    } else if (l_flag) {
	l_flag = 0;
	log_file_name = (char *) xmalloc(strlen(input) + 1);
	strcpy(log_file_name, input);
	/* in this context, indicates explicit log file name */
	log_available = 1;
    } else if (k_flag) {
	k_flag = 0;
	key_file_name = (char *) xmalloc(strlen(input) + 1);
	strcpy(key_file_name, input);
	/* in this context, indicates explicit log file name */
	key_available = 1;
    } else if (y_flag) {
	LibPath_t *libPath;
	y_flag = 0;
	libPath = (LibPath_t *) xmalloc(sizeof(LibPath_t));
	libPath->next = ypathList;
	ypathList = libPath;
	libPath->path = (char *) xmalloc(strlen(input) + 1);
	strcpy(libPath->path, input);
    } else if (extra_flag) {
	extra_flag = 0;		/* ignore argument after option that takes args */
	/*
	 * Consider the command line switches
	 */
    } else if (!strcmp( input, "--help" ) ) {
	CmdlineHelp();
	shell_exit(0);
    } else if (!strcmp( input, "--version" ) ) {
        printf("%s %s, \n\n", VERIWELL, VERSION );
        printf( copyright );
	printf( "\nWritten by Elliot Mednick and Mark Hummel\n" );
	shell_exit(0);
    } else if (!strcmp( input, "--bindir" ) ) {
	printf( "%s\n", BINDIR );
	shell_exit(0);
    } else if (!strcmp( input, "--libdir" ) ) {
	printf( "%s\n", LIBDIR );
	shell_exit(0);
    } else if (!strcmp( input, "--includedir" ) ) {
	printf( "%s\n", INCLUDEDIR );
	shell_exit(0);
    } else if (!strcmp( input, "--cflags" ) ) {
	printf( "%s\n", CFLAGS );
	shell_exit(0);
    } else if (!strcmp( input, "--ldflags" ) ) {
	printf( "%s\n", LDFLAGS );
	shell_exit(0);
    } else if (!strcmp( input, "--libs" ) ) {
	printf( "%s\n", LIBS );
	shell_exit(0);
    } else if (input[0] == '-') {
	while ((c = *++input) != 0) {
	    if (f_flag) {
		fprintf(stderr, "-f option must be alone in"
			" command line argument\n");
		shell_exit(1);
	    }

	    if (i_flag) {
		fprintf(stderr,
			"-i option must be alone in command line argument\n");
		shell_exit(1);
	    } else {
		switch (c) {
		case 'd':
		    debug++;
		    break;
#if VDEBUG != 0
		case 'v':
		    vdebug = 1;
		    break;
		case 'P':
		    program++;
		    break;
#endif
		case 'c':
		    simulate = 0;
		    break;
		case 'f':
		    if (++f_nest > MAX_LEVEL) {
			fprintf(stderr, "Exceeded -f nesting capacity!\n");
			shell_exit(1);
		    }
		    if (input != &input[0]) {
			fprintf(stderr, "-f option must be alone in"
				" command line argument\n");
			shell_exit(1);
		    }
		    f_flag++;
		    break;
		case 's':
		    s_flag = 1;
		    break;
		case 't':
		    trace_flag = 1;
		    normal_flag = 0;
		    break;
		case 'i':
		    if (input != &input[0]) {
			fprintf(stderr, "-i option must be alone in"
				" command line argument\n");
			shell_exit(1);
		    }
		    i_flag = 1;
		    break;
		case 'l':
		    l_flag = 1;
		    break;
		case 'k':
		    k_flag = 1;
		    break;
		case 'r':
		case 'y':
		    y_flag = 1;
		    break;
		case '?':
		case 'h':
		    CmdlineHelp();
		    shell_exit(0);
		    break;
		default:;
		}
	    }
	}			/* while */
    } else {
	/* Might be one of those plus options - compare against the list */
	if (input[0] == '+') {
	    input++;
	    if (!strcmp("synopsys", input)) {
		synopsys_flag = 1;
	    } else if (!strcmp("mindelays", input)) {
		delay_type = MIN_DELAY;
	    } else if (!strcmp("typdelays", input)) {
		delay_type = TYP_DELAY;
	    } else if (!strcmp("maxdelays", input)) {
		delay_type = MAX_DELAY;
	    } else if (!strncmp("define", input, 6)) {
		char *p = strtok(input, "+");	/* skip "define+" */
		for (p = strtok(NULL, "+"); p; p = strtok(NULL, "+")) {
		    char *b = strchr(p, '=');
		    if (b == NULL) {
			define_macro(build_string(strlen(p), p),
				     NULL_TREE);
		    } else {
			*b++ = 0;
			define_macro(build_string(strlen(p), p),
				     build_string(strlen(b), b));
		    }
		}
	    } else if (!strcmp("noshow_var_change", input)) {
		var_debug_flag = 0;
	    } else if (!strncmp("libext", input, 6)) {
		ylibext = (char *) xmalloc(strlen(input) - 5);
		strcpy(ylibext, &input[7]);	/* copy from past "libext" */
	    } else if (!strncmp("incdir", input, 6)) {
		int len = strlen(incdir);
		char *prevIncDir = incdir;
		incdir = (char *) xmalloc(len + 1 + strlen(input) - 5);
		strcpy(incdir, prevIncDir);	/* "incdir" is cummulative */
		if (len != 0) {
		    strcat(incdir, "+");
		}
		strcat(incdir, &input[7]);	/* copy from past "incdir" */
		free(prevIncDir);
	    } else if (!strcmp("yydebug", input)) {
		yydebug = 1;
#ifdef DUMP
	    } else if (!strcmp("dump", input)) {
		dump_it = 1;
#endif
	    } else if (!strcmp("MemTrace", input)) {
		MemTrace = 1;
	    } else {
		plusargs = tree_cons((tree) input, NULL_TREE, plusargs);
	    }
	} else {
	    /*  Is it a source file? */
	    fp = (Filelist *) xmalloc(sizeof(Filelist));
	    fp->name = (char *) xmalloc(strlen(input) + 1);
	    strcpy(fp->name, input);
	    fp->next = NULL;
	    if (!source_last) {
		source_first = fp;
	    } else {
		source_last->next = fp;
	    }
	    source_last = fp;
	}
    }
}

int moreinput(char *infile)
{
    if (fin && fin != File::Stdin()) {
	fin->fclose();
    }
    init_lex();
    if ((fin = File::fopen(infile, "r")) == NULL) {
	printf_error_V("%s: cannot open %s\n", progname, infile);
	shell_exit(1);
    }
    return 1;
}

/*
 This routine just prints out the command line help message
*/

void CmdlineHelp()
{
    printf( "'VeriWell' is a Verilog simulator supporting the IEEE 1364-1995 standard.\n\n" );
    printf("Usage: veriwell [options] <file1> [<file2>...]\n\n");
    printf("Options:\n");
    printf(" -c                      Compile only - do not simulate\n");
    printf(" -s                      Enter interactive mode before running\n");
    printf(" -t                      Turn trace on\n");
    printf(" -f <filename>           Read options from <filename>\n");
    printf(" -i <filename>           Read commands from <filename>\n");
    printf(" -l <filename>           Set log file to <filename>\n");
    printf(" -l nolog                Disable log file\n");
    printf(" -k <filename>           Set key file to <filename>\n");
    printf(" -k nokey                Disable key file\n");
    printf(" -y <directory>          Search directory for module definitions\n");
    printf(" -?, -h                  This message\n");
    printf(" +synopsys               Enable Synopsys HDL Compiler(tm) 2.x checking\n");
    printf(" +mindelays              Use minimum delays\n");
    printf(" +maxdelays              Use maximum delays\n");
    printf(" +typdelays              Use typical delays\n");
    printf(" +define+<macro>=<value> Define macros\n");
    printf(" +libext+<extension>     Specify library suffix \n");
    printf(" +incdir+<directory>     Specify include search path\n");
    printf(" +<name>+<value>         Define plusargs\n");
    printf(" --help                  This message\n");
    printf(" --version               Print version\n");
    printf(" --bindir                Binary install path\n");
    printf(" --libdir                Library install path\n");
    printf(" --includedir            Include install path\n");
    printf(" --cflags                Compiler flags used\n");
    printf(" --ldflags               Linker flags used\n");
    printf(" --libs                  Libraries used\n");
    printf(" All other options are tested for arguments and ignored\n");
    printf("\nExamples:\n");
    printf("The following system tasks are supported:\n\n");
#ifdef HAVE_LXT
    printf(" $lxt_recordon                        Enable waveform dump\n");
    printf(" $lxt_recordoff                       Disable waveform dump\n");
    printf(" $lxt_recordclose                     End waveform dump\n");
    printf(" $lxt_recordfile filename options     Set filename and options\n");
    printf(" $lxt_recordsetup options             Set options\n");
    printf(" $lxt_recordvars options              Start waveform dump\n\n");
#endif
#ifdef HAVE_LXT2
    printf(" $lxt2_recordon                        Enable waveform dump\n");
    printf(" $lxt2_recordoff                       Disable waveform dump\n");
    printf(" $lxt2_recordclose                     End waveform dump\n");
    printf(" $lxt2_recordfile filename options     Set filename and options\n");
    printf(" $lxt2_recordsetup options             Set options\n");
    printf(" $lxt2_recordvars options              Start waveform dump\n\n");
#endif
#if defined(HAVE_LXT) || defined(HAVE_LXT2)
    printf("Lxt Options:\n");
    printf("   incsize=<file_size>  Specify incremental file size in bytes\n");
    printf("   space                optimize for space\n");
    printf("   speed                optimize for speed\n");
    printf("   sequence             Record 0 time signal changes\n");
    printf("   nosequence           Do not record 0 time signal changes\n");
    printf("   design=<name>        Specify design name\n");
    printf("   depth=<depth>        Specify extraction depth\n");
#endif
    printf("\nReport bugs at <http://sourceforge.net/projects/veriwell>\n");
}

/* Parse the input files and generate a syntax tree */

void PhaseI()
{				/* compile all input */
    prog_parse();
}

void PhaseII()
{
    build_hierarchy();
}

void PhaseIII()
{
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

void segv(int sign_number)
{
    fputs("Caught internal error\n", stdout);
    fflush(stdout);
    shell_exit(1);
}

void breaker(int sig_number)
{
    if (!in_simulation) {
	fputs("Compilation terminated by user\n", stdout);
	fflush(stdout);
#if VDEBUG != 0
	if (vdebug) {
	    print_stats();
	}
#endif
	shell_exit(1);
    } else {
#if VDEBUG != 0
	if (vdebug) {
	    printf_V("Ctl-C detected\n");
	}
#endif
	normal_flag = 0;
	break_flag = 1;
	shell_signal(SIGINT, breaker);
    }
}

void __main_v(int argc, char **argv)
{				/* hoc6 */
    shell_signal(SIGINT, breaker);

    globalArgc = argc;
    globalArgv = argv;
    progname = argv[0];
    if (argc == 1) {		/* print the standard "bad command line" */
	CmdlineHelp();
	shell_exit(0);
    }
    init_tree();
    Cmdline(argc, argv);

    exec_(save_pc);
    shell_exit(0);
}

void print_info(void)
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

void finish()
{
    broadcast_tf(reason_finish);
    printf_V("\nNormal exit\n");
    printf_V("Thank you for using %s\n", VERIWELL);
    shell_exit(0);
}

unsigned long mem_avail()
{
    return (0);
}


void heap_test(char *msg)
{
}

void flush_files()
{
    if (log_enable) {
	fflush(log_file);
    }
    if (key_enable) {
	fflush(key_file);
    }
}

#if defined (DEBUG) || defined(ASSERTIONS)
void _Assert(char *strFile, unsigned uLine)
{
    fflush(stdout);
    fprintf(stderr, "\nAssertion failed: %s, line %lu\n", strFile, uLine);
    fflush(stderr);
    shell_abort();
}
#endif

/***************************************************************
 *
 *	initverilog
 *	- initialize global and static variables for verilog.c
 *
 ***************************************************************
 */

void init_verilog()
{
    dump_it = 0;
    _ovrbuffer = 0x400;
    zero_ptr = 0;		/* for monitoring 0000:0000 in debugger */
    clock_pause = 0;
    extra_flag = 0;
    default_net_type = NET_WIRE_TYPE;
    delay_type = TYP_DELAY;
    errorcount = 0;
    warningcount = 0;
    f_first = 0;
    f_last = 0;
    source_first = 0;
    source_last = 0;
    in_simulation = 0;
    dumpfile = 0;
    memset(&mask_right, 0, sizeof(mask_right));
    memset(&mask_right1, 0, sizeof(mask_right1));
    clock_start = 0;
    clock_compile = 0;
    clock_load = 0;
    clock_simulate = 0;
    save_pc = NULL_TREE;
    progname = 0;
#ifdef VDEBUG
    yydebug = 0;
#endif
    i_file = 0;
    memset(&i_buf, 0, sizeof(i_buf));
}
