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

/* SYSTASK.C - System tasks and functions */

#define SYSTASK_C

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include "vtypes.h"
#include "tree.h"
#include "lex.h"
#include "flags.h"
#include "runtime.h"
#include "schedule.h"
#include "print.h"
#include "io.h"
#include "macro.h"
#include "glue.h"
#include "dumpvar.h"
#include "pass3.h"
#include "strobe.h"
#include "scope.h"
#include "veriuser.h"
#include "eval.h"
#include "timescal.h"
#include "specify.h"
#include "veriwell.h"
#include "pass2.h"
#include "store.h"
#include "trace.h"
#include "acc_user.h"
#include "pli.h"
#include "systask.h"
#include "usertask.h"
#include "random.h"


int systime_once = 0;		/* initialize the following only once */
tree systime_return;		/* variable for returning $time value */

int sysstime_once = 0;		/* initialize the following only once */
tree sysstime_return;		/* variable for returning $stime value */

int sysrealtime_once = 0;	/* initialize the following only once */
tree sysrealtime_return;	/* variable for returning $realtime value */

int sysopen_once = 0;		/* initialize the following only once */
tree sysopen_return;		/* variable for returning $fopen value */

int sysrand_once = 0;		/* initialize the following only once */
tree sysrand_return;		/* variable for returning $random value */

int sysplus_once = 0;		/* initialize the following only once */
tree sysplus_return;		/* variable for returning $test$plusargs value */

int roFlag = FALSE;		/* true when in a misc routine for rosynch */

static tree pliInstanceList;	/* linked list of all pli instances */

struct monitor_info monitor_info = { 1, NULL_TREE };
struct monitor_info mwaves_info = { 1, NULL_TREE };

/* There are 3 kinds of system tasks/functions (names that start with '$'):
   - internal tasks implemented either before there were PLI hooks or that
     require direct access to VeriWell structures -- these are implemented
     here;
   - internal tasks written using all PLI functions as if they were
     user-defined functions -- they are implemented elsewhere but the
     name resolution is here;
   - user-defined tasks using PLI functions implemented outside VeriWell --
     the name resolution is also done here.
     */

/* [tf: task/function] */

/* These are the internal non-PLI tasks/functions: (some could eventually
   be re-written into PLI...) */

struct systask_info systask_info[] = {
    "$scope", SCOPE,
    "$showscopes", SHOWSCOPES,
    "$display", DISPLAY,
    "$displayh", DISPLAYH,
    "$displayb", DISPLAYB,
    "$displayo", DISPLAYO,
    "$write", WRITE,
    "$writeh", WRITEH,
    "$writeb", WRITEB,
    "$writeo", WRITEO,
    "$fdisplay", FDISPLAY,
    "$fdisplayh", FDISPLAYH,
    "$fdisplayb", FDISPLAYB,
    "$fdisplayo", FDISPLAYO,
    "$fwrite", T_FWRITE,
    "$fwriteh", T_FWRITEH,
    "$fwriteb", T_FWRITEB,
    "$fwriteo", T_FWRITEO,
    "$monitor", MONITOR,
    "$monitorb", MONITORB,
    "$monitorh", MONITORH,
    "$monitoro", MONITORO,
    "$fmonitor", FMONITOR,
    "$fmonitorb", FMONITORB,
    "$fmonitorh", FMONITORH,
    "$fmonitoro", FMONITORO,
    "$strobe", STROBE,
    "$strobeb", STROBEB,
    "$strobeh", STROBEH,
    "$strobeo", STROBEO,
    "$fstrobe", FSTROBE,
    "$fstrobeb", FSTROBEB,
    "$fstrobeh", FSTROBEH,
    "$fstrobeo", FSTROBEO,
    "$monitoron", T_MONITORON,
    "$monitoroff", T_MONITOROFF,
    "$stop", STOP,
    "$finish", FINISH,
    "$settrace", SETTRACE,
    "$cleartrace", CLEARTRACE,
    "$showstats", SHOWSTATS,
    "$fclose", T_FCLOSE,
    "$log", T_LOG,
    "$nolog", T_NOLOG,
    "$key", T_KEY,
    "$nokey", T_NOKEY,
    "$input", T_INPUT,
    "$showvars", T_SHOWVARS,
    "$readmemh", T_READMEMH,
    "$readmemb", T_READMEMB,
    "$dumpvars", T_DUMPVARS,
    "$dumpon", T_DUMPON,
    "$dumpoff", T_DUMPOFF,
    "$dumpfile", T_DUMPFILE,
    "$dumpall", T_DUMPALL,
    "$dumpflush", T_DUMPFLUSH,
    "$update_timings", T_UPDATETIMINGS,
    "$timeformat", T_TIMEFORMAT,
    0, (enum systask_type) 0
};

struct sysfunction_info sysfunction_info[] = {
    "$stime", F_STIME,
    "$time", F_TIME,
    "$realtime", F_REALTIME,
    "$fopen", F_FOPEN,
    "$random", F_RANDOM,
    "$test$plusargs", F_TEST_PLUSARGS,
    0, (enum sysfunction_type) 0
};

tree current_tf_instance;	/* node of systask being executed or compiled */
int globalSeed = 0;		/* seed for random number generator */

extern s_tfcell veriusertfs[];	/* Hooks into user tfs */
extern s_tfcell verisystfs[];	/* Hooks into internal tfs */

void broadcast_tf(int reason)
{
    tree t;

    t = pliInstanceList;
    while (t) {
	current_tf_instance = t;
	switch (TREE_CODE(t)) {
	case SYSFUNCTION_REF:
	    call_misc_tf(FUNC_REF_USERTF(t), reason);
	    t = FUNC_REF_NEXT(t);
	    break;
	case SYSTASK_STMT:
	    call_misc_tf(STMT_SYSTASK_USERTF(t), reason);
	    t = STMT_SYSTASK_NEXT(t);
	    break;
	}
    }

}

/* lookup by name a PLI tf either user or internal (user first since it can
   override an internal tf); retun pointer to structure. */

p_tfcell lookup_user_tf(char *name)
{
    int i;

    p_tfcell userFunction = Usertask_Lookup(name);
    if (userFunction) {
	return userFunction;
    }
    for (i = 0; verisystfs[i].type; i++) {
	if (!strcmp(name, verisystfs[i].tfname))
	    return &verisystfs[i];
    }
    return 0;
}

/* Lookup a PLI _task_ (either internal or user-defined); return pointer
   to structure */

p_tfcell lookup_user_task(char *name)
{
    p_tfcell p_tf = lookup_user_tf(name);	/* look it up in both tables */
    if (p_tf == 0)		/* not found */
	return 0;

    if (p_tf->type != USERTASK)	/* its not a task */
	return 0;
    else
	return p_tf;
}

/* Same as above but for a function */

p_tfcell lookup_user_func(char *name)
{
    p_tfcell p_tf = lookup_user_tf(name);	/* look it up in both tables */
    if (p_tf == 0)		/* not found */
	return 0;

    if (p_tf->type != USERFUNCTION)	/* its not a function */
	return 0;
    else
	return p_tf;
}

/* Call the correct _check_ routine for the given user task/function */

void call_check_tf(p_tfcell p_tf)
{
    if (p_tf->checktf)
	(p_tf->checktf) (p_tf->data, (int) REASON_CHECKTF);
}

/* Call the correct _call_ routine for the given user task/function */

void call_call_tf(p_tfcell p_tf)
{
    if (p_tf->calltf)
	(p_tf->calltf) (p_tf->data, (int) REASON_CALLTF);
}

/* Call the correct _size_ routine for the given user task/function */

int call_size_tf(p_tfcell p_tf)
{
    if (p_tf->sizetf)
	return (p_tf->sizetf) (p_tf->data, (int) REASON_SIZETF);
    return 0;
}

/* Call the correct _misc_ routine for the given user task/function */

void call_misc_tf(p_tfcell p_tf, int reason)
{
    if (p_tf->misctf)
	(p_tf->misctf) (p_tf->data, reason);
}

void call_misc_tf1(p_tfcell p_tf, int reason, int arg)
{
    if (p_tf->misctf)
	((int (*)(int, int, int)) p_tf->misctf) (p_tf->data, reason, arg);
}

void systaskDoDelay(tree delayNode)
{
    tree node;

    ASSERT(delayNode != NULL);
    ASSERT(TREE_CODE(delayNode) == PLI_DELAY);

    node = PLI_DELAY_NODE(delayNode);
    ASSERT(node != NULL);
    current_tf_instance = node;
    switch (TREE_CODE(node)) {
    case SYSFUNCTION_REF:
	call_misc_tf(FUNC_REF_USERTF(node), reason_reactivate);
	break;
    case SYSTASK_STMT:
	call_misc_tf(STMT_SYSTASK_USERTF(node), reason_reactivate);
	break;
    }
}


void pli_asynccall(PliInfo_t * info)
{
    tree node;

    node = info->instance;

    if (ASYNCH_ATTR(node)) {
	roFlag = TRUE;
	info->pvc = TRUE;
	current_tf_instance = node;
	switch (TREE_CODE(node)) {
	case SYSFUNCTION_REF:
	    call_misc_tf1(FUNC_REF_USERTF(node),
			  reason_paramvc, info->argNumber);
	    break;
	case SYSTASK_STMT:
	    call_misc_tf1(STMT_SYSTASK_USERTF(node),
			  reason_paramvc, info->argNumber);
	    break;
	}
	roFlag = FALSE;
    }
}



/* "top-level" lookup called from pass3; look for user PLI, internal PLI,
   internal custom, in that order.  Return the enum of the internal
   tf and the pointer to the PLI tf through the arg list */

enum systask_type lookup_systask(char *name, p_tfcell * pp_tf)
{
    int i;

    *pp_tf = lookup_user_task(name);
    if (*pp_tf)
	return USER_TASK;	/* found PLI, pass pointer through arg list */

    for (i = 0; systask_info[i].name; i++) {
	if (!strcmp(name, systask_info[i].name))
	    return systask_info[i].type;
    }
    return (enum systask_type) -1;
}

enum sysfunction_type lookup_sysfunction(char *name, p_tfcell * pp_tf)
{
    int i;

    *pp_tf = lookup_user_func(name);
    if (*pp_tf)
	return USER_FUNC;

    for (i = 0; sysfunction_info[i].name; i++) {
	if (!strcmp(name, sysfunction_info[i].name))
	    return sysfunction_info[i].type;
    }
    return (enum sysfunction_type) -1;
}

/* Convert bits in a group to a null-terminated string.  It is assumed that
   'buffer' is large enough to handle the string. */

void bits_to_string(char *buffer, Group * g, nbits_t nbits)
{
    char *p = buffer;
    ngroups_t ngroups, i;
    int j, c;
    Group *g1;

    ngroups = bits_to_groups(nbits);
//  start_bit = ((nbits - 1) % (sizeof (Bit) * 8)) + 1;
    g1 = g + ngroups;

    for (i = ngroups; i >= 0; i--, g1--)
	for (j = sizeof(Bit) - 1; j >= 0; j--) {
#ifdef WORDS_BIGENDIAN
	    c = *(((char *) &AVAL(g1)) + sizeof(Bit) - 1 - j);	/* endianess problem */
#else
	    c = (AVAL(g1) >> j * 8) & 255;
#endif
	    if (c)
		*p++ = c;
	}
    *p = 0;
}

handle_t get_handle(tree * code)
{
    nbits_t nbits;
    Group *val;

    val = eval_(code, &nbits);
    return check_handle((handle_t) AVAL(val));
}

tree init_function_return(char *name, int size, enum tree_code code)
{
    tree var_name = make_node(IDENTIFIER_NODE);
    tree rtn = make_node(code);

    IDENTIFIER_LENGTH(var_name) = strlen(name);
    IDENTIFIER_POINTER(var_name) = name;
    DECL_NAME(rtn) = var_name;
//  if (code == REG_VECTOR_DECL)
    TREE_NBITS(rtn) = size;
    DECL_STORAGE(rtn) = (Group *) malloc_X(bits_to_groups(size) + 1);
    if (!DECL_STORAGE(rtn))
	fatal("Not enough memory to allocate for '%s' system function",
	      name);
//  STMT_TASK_ARGS (node) = NULL_TREE;
    return rtn;
}

int args_one(int num_args, char *taskname)
{
    if (num_args > 1)
	error("Unexpected arguments in system function '%s'", taskname,
	      NULL);
    else if (num_args == 0)
	error("Missing arguments in call to system function '%s'",
	      taskname, NULL);
    else
	return 1;
    return 0;
}

int args_zero(int num_args, char *taskname)
{
    if (num_args != 0)
	error("Unexpected arguments in system function '%s'", taskname,
	      NULL);
    else
	return 1;
    return 0;
}

int args_zero_or_one(int num_args, char *taskname)
{
    if (num_args > 1)
	error("Unexpected arguments in system function '%s'", taskname,
	      NULL);
    else
	return 1;
    return 0;
}

void arg_common(tree args)
{
    tree t;

    for (t = args; t; t = TREE_CHAIN(t))
	if (TREE_EXPR(t))
	    TREE_EXPR_CODE(t) = pass3_expr(TREE_EXPR(t));
}

int count_args(tree node)
{
    tree t;
    int arg_count = 0;

    if (TREE_CODE(node) == SYSTASK_STMT)
	t = STMT_TASK_ARGS(node);
    else if (TREE_CODE(node) == SYSFUNCTION_REF)
	t = FUNC_REF_ARGS(node);
    else {
	error("Illegal node type", NULL, NULL);	/* do error handling later */
	return 0;
    }

    if (!t || !TREE_EXPR(t))	/* This covers $foo; and $foo(); */
	return 0;
    for (; t; t = TREE_CHAIN(t))
	arg_count++;
    return arg_count;
}

void init_sysfunction(tree node)
{
    int num_args;
    tree t;
    enum tree_code code;
    PliInfo_t *info;
    int count;

/* First, count the number of arguments */
    num_args = count_args(node);
    TREE_NBITS(node) = 1;	/* make sure this is something if error causes early exit */

    switch (FUNC_REF_SYSTYPE(node)) {

    case USER_FUNC:

	FUNC_REF_NEXT(node) = pliInstanceList;
	pliInstanceList = node;
	FUNC_REF_DELAYSCB(node) = NULL;
	// setup for async marker
	count = 1;

// ??? mdh - change
//  for (t = STMT_TASK_ARGS (node); t; t = TREE_CHAIN (t)) {
	for (t = FUNC_REF_ARGS(node); t; t = TREE_CHAIN(t)) {
	    info = (PliInfo_t *) xmalloc(sizeof(PliInfo_t));
	    info->storage = NULL;
	    info->pvc = FALSE;
	    info->saved_pvc = FALSE;
	    info->vclSet = FALSE;
	    info->instance = node;
	    info->argNumber = count;
	    info->marker = NULL;
	    TREE_PLIINFO(t) = (tree) info;
	    count++;

	    if (TREE_EXPR(t)) {
		// ??? mdh - but this version doen't install markers so
		//           pvc calls won't be performed
		TREE_EXPR_CODE(t) = pass3_expr_intrude(TREE_EXPR(t), 1);
		// ??? mdh - this destroys function return code
//            TREE_EXPR_CODE (t) = pass3_expr_marker (TREE_EXPR (t),
//                &info->marker, (enum marker_flags)(M_ASYNCH + M_FIXED), (tree)info, NULL_TREE );
	    }
	}

	current_tf_instance = node;
	ASYNCH_ATTR(node) = FALSE;
	call_check_tf(FUNC_REF_USERTF(node));
//    arg_common (FUNC_REF_ARGS (node));
	TREE_NBITS(node) = call_size_tf(FUNC_REF_USERTF(node));
	FUNC_REF_RETURNDECL(node) =
	    init_function_return(((p_tfcell) FUNC_REF_USERTF(node))->
				 tfname, TREE_NBITS(node),
				 REG_VECTOR_DECL);
	return;


    case F_STIME:
	if (!args_zero(num_args, "$stime"))
	    break;

	TREE_NBITS(node) = 32;
	if (!sysstime_once) {
	    sysstime_return =
		init_function_return("$stime", 32, REG_VECTOR_DECL);
	    sysstime_once = 1;
	}
	break;

    case F_TIME:
	if (!args_zero(num_args, "$time"))
	    break;

	TREE_NBITS(node) = 64;
	if (!systime_once) {
	    systime_return =
		init_function_return("$time", 64, REG_VECTOR_DECL);
	    systime_once = 1;
	}
	break;

    case F_REALTIME:
	if (!args_zero(num_args, "$realtime"))
	    break;

	TREE_NBITS(node) = 32;
	TREE_REAL_ATTR(node) = 1;	/* make $realtime a real (not int) node */
	if (!sysrealtime_once) {
	    sysrealtime_return =
		init_function_return("$realtime", 32, REAL_DECL);
	    sysrealtime_once = 1;
	}
	break;

    case F_RANDOM:
	if (!args_zero_or_one(num_args, "$random"))
	    break;

	if (num_args == 1) {
	    t = TREE_PURPOSE(FUNC_REF_ARGS(node));	/* 1st arg */
	    if (HIERARCHICAL_ATTR(t)) {
		FUNC_REF_INASSIGN(node) = resolve_hierarchical_name(t);
		if (FUNC_REF_INASSIGN(node) == error_mark_node) {
		    TREE_NBITS(t) = 1;
		    break;
		}
	    } else
		FUNC_REF_INASSIGN(node) = t;
	    code = TREE_CODE(FUNC_REF_INASSIGN(node));
	    if (code != REG_VECTOR_DECL && code != INTEGER_DECL
		&& code != TIME_DECL) {
		error("Illegal arguments is call to $random", NULL, NULL);
		break;
	    }
	}
	TREE_NBITS(node) = 32;
	if (!sysrand_once) {
	    sysrand_return =
		init_function_return("$random", 32, REG_VECTOR_DECL);
	    sysrand_once = 1;
	}
	break;

    case F_FOPEN:
	if (!args_one(num_args, "$fopen"))
	    break;

	FUNC_REF_INASSIGN(node) = build_tree_list((tree)
						  pass3_expr_intrude
						  (TREE_PURPOSE
						   (FUNC_REF_ARGS(node)),
						   1), NULL_TREE);

	TREE_NBITS(node) = 32;
	if (!sysopen_once) {
	    sysopen_return =
		init_function_return("$fopen", 32, REG_VECTOR_DECL);
	    sysopen_once = 1;
	}
	break;

    case F_TEST_PLUSARGS:
	if (!args_one(num_args, "$test$plusargs"))
	    break;

	if (num_args == 0)
	    yyerror
		("Missing arguments in call to system function '$test$plusargs'");
	FUNC_REF_INASSIGN(node) = build_tree_list((tree)
						  pass3_expr_intrude
						  (TREE_PURPOSE
						   (FUNC_REF_ARGS(node)),
						   1), NULL_TREE);

	TREE_NBITS(node) = 1;
	if (!sysplus_once) {
	    sysplus_return =
		init_function_return("$test$plusargs", 1, REG_SCALAR_DECL);
	    sysplus_once = 1;
	}
	break;
    }
}

void init_systask(tree node)
{
    int num_args;
    int is_file = 0;
    tree t;
    tree decl;
    PliInfo_t *info;
    int count;

/* First, count the number of arguments */
    num_args = count_args(node);
    R_alloc(10, 10);
    TREE_NBITS(node) = 1;	/* in case an error causes an early exit */


    switch (STMT_SYSTASK_TYPE(node)) {

    case USER_TASK:
	// setup for async marker
	STMT_SYSTASK_NEXT(node) = pliInstanceList;
	pliInstanceList = node;
	STMT_SYSTASK_DELAYSCB(node) = NULL;
	count = 1;
	for (t = STMT_TASK_ARGS(node); t; t = TREE_CHAIN(t)) {
	    info = (PliInfo_t *) xmalloc(sizeof(PliInfo_t));
	    info->storage = NULL;
	    info->pvc = FALSE;
	    info->saved_pvc = FALSE;
	    info->instance = node;
	    info->argNumber = count;
	    info->marker = NULL;
	    TREE_PLIINFO(t) = (tree) info;
	    count++;

	    if (TREE_EXPR(t)) {
		TREE_EXPR_CODE(t) = pass3_expr_marker(TREE_EXPR(t),
						      &info->marker,
						      (enum
						       marker_flags)
						      (M_ASYNCH + M_FIXED),
						      (tree) info,
						      NULL_TREE);
	    }

	}

	current_tf_instance = node;
	call_check_tf(STMT_SYSTASK_USERTF(node));
//      arg_common (STMT_TASK_ARGS (node));
	ASYNCH_ATTR(node) = FALSE;
	break;

    case SCOPE:
	if (!args_one(num_args, "$scope"))
	    break;

	t = TREE_PURPOSE(STMT_TASK_ARGS(node));
//  if (*tree_code_type [TREE_CODE (t)] != 'b' && !HIERARCHICAL_ATTR (t))
	if (TREE_CODE(t) != BLOCK_DECL && !HIERARCHICAL_ATTR(t))
	    yyerror("Illegal argument type for $scope system task");
	break;

    case SHOWSCOPES:
    case T_NOLOG:
    case T_NOKEY:
    case T_MONITORON:
    case T_MONITOROFF:
    case T_DUMPON:
    case T_DUMPOFF:
    case T_DUMPALL:
    case T_DUMPFLUSH:
    case T_UPDATETIMINGS:

	if (!args_zero(num_args, STMT_TASK_NAME(node)))
	    break;
	break;

    case T_SHOWVARS:
	if (num_args > 1)
	    yyerror("Unexpected arguments in system task $showvars");
	else if (num_args == 1)
	    arg_common(STMT_TASK_ARGS(node));
	break;

    case T_LOG:
    case T_KEY:
	if (num_args > 1 || STMT_TASK_ARGS(node)
	    && !TREE_EXPR(STMT_TASK_ARGS(node)))
	    error("Unexpected arguments in system task %s",
		  STMT_TASK_NAME(node), NULL_CHAR);
	else
	    arg_common(STMT_TASK_ARGS(node));
	break;

    case T_FCLOSE:
    case T_INPUT:
    case T_DUMPFILE:
	if (args_one(num_args, STMT_TASK_NAME(node)))
	    arg_common(STMT_TASK_ARGS(node));
	break;

    case FDISPLAY:
    case FDISPLAYB:
    case FDISPLAYH:
    case FDISPLAYO:
    case T_FWRITE:
    case T_FWRITEB:
    case T_FWRITEH:
    case T_FWRITEO:
    case FSTROBE:
    case FSTROBEB:
    case FSTROBEH:
    case FSTROBEO:
    case FMONITOR:
    case FMONITORB:
    case FMONITORH:
    case FMONITORO:
	is_file = 1;
    case DISPLAY:
    case DISPLAYB:
    case DISPLAYH:
    case DISPLAYO:
    case WRITE:
    case WRITEB:
    case WRITEH:
    case WRITEO:
    case STROBE:
    case STROBEB:
    case STROBEH:
    case STROBEO:
    case MONITOR:
    case MONITORB:
    case MONITORH:
    case MONITORO:
	if (!disp_common(node, DEC, is_file, 1)) {
	    error("Illegal arguments in call to %s", STMT_TASK_NAME(node),
		  NULL_CHAR);
	    break;
	}
	arg_common(STMT_TASK_ARGS(node));
	break;

    case T_READMEMH:
    case T_READMEMB:
	if (num_args < 2) {
	    error("Missing arguments in call to %s", STMT_TASK_NAME(node),
		  NULL_CHAR);
	    break;
	}
	if (num_args > 4) {
	    error("Too many arguments in call to %s", STMT_TASK_NAME(node),
		  NULL_CHAR);
	    break;
	}
	/* Process file name */
	t = STMT_TASK_ARGS(node);
	if (TREE_EXPR(t))
	    TREE_EXPR_CODE(t) = pass3_expr(TREE_EXPR(t));
	else {
	    error("Missing file name in call to %s", STMT_TASK_NAME(node),
		  NULL_CHAR);
	    break;
	}

	/* Make sure 2nd argument is an array */
	t = TREE_CHAIN(t);
	decl = TREE_EXPR(t);
	if (HIERARCHICAL_ATTR(decl))
	    decl = resolve_hierarchical_name(decl);
	if (TREE_CODE(decl) != ARRAY_DECL) {
	    error("Second argument to %s must be an array",
		  STMT_TASK_NAME(node), NULL_CHAR);
	    break;
	}
	for (t = TREE_CHAIN(t); t; t = TREE_CHAIN(t))
	    if (TREE_EXPR(t)) {
		TREE_EXPR_CODE(t) = pass3_expr(TREE_EXPR(t));
		if (TREE_NBITS(*TREE_EXPR_CODE(t)) > MAX_BITS_IN_NUMBER)
		    yyerror("Array index must be within 32 bits");
	    } else
		error("Missing argument in call to %s",
		      STMT_TASK_NAME(node), NULL_CHAR);

	break;

    case T_WWAVES:
	/* For waves, make sure arguments are in label-signal pairs */
	for (t = STMT_TASK_ARGS(node); t; t = TREE_CHAIN(t)) {
	    if (is_string(TREE_EXPR(t)))
		TREE_EXPR_CODE(t) = pass3_expr(TREE_EXPR(t));
	    else {
		error("Missing label in call to %s", STMT_TASK_NAME(node),
		      NULL_CHAR);
		break;
	    }
	    if (((t = TREE_CHAIN(t)) == NULL_TREE) || !TREE_EXPR(t)) {
		error("Missing argument in call to %s",
		      STMT_TASK_NAME(node), NULL_CHAR);
		break;
	    }
	    if (is_array(TREE_EXPR(t))) {
		error("Illegal array reference in call to %s",
		      STMT_TASK_NAME(node), NULL_CHAR);
		break;
	    }
	    if (is_string(TREE_EXPR(t))) {
		error("Missing signal in call to %s",
		      STMT_TASK_NAME(node), NULL_CHAR);
		break;
	    }
	    TREE_EXPR_CODE(t) = pass3_expr(TREE_EXPR(t));
	}
	break;

    case T_DUMPVARS:
	dumpvar.markers_first = NULL;
	dumpvar.markers_last = NULL;
	dumpvar.printed = NULL;
	dumpvar.unprinted = NULL;
	dumpvar.disallow_dumpvars = 0;
	dumpvar.first_dumpvars = 1;
	dumpvar.enabled = 0;
	dumpvar.new_time = 1;

	if (!num_args)		/* No args, simple case */
	    break;

	t = STMT_TASK_ARGS(node);
	if (is_block(TREE_EXPR(t), &decl))
	    error("Illegal type in first argument of $dumpvars", NULL,
		  NULL);
	else
	    TREE_EXPR_CODE(t) = pass3_expr(TREE_EXPR(t));

	/* Put actual decl or block node in TREE_STMT */
	for (t = TREE_CHAIN(t); t; t = TREE_CHAIN(t))
	    if (is_var(TREE_EXPR(t), &TREE_STMT(t))
		|| is_block(TREE_EXPR(t), &TREE_STMT(t)));
	    else
		error("Illegal argument in $dumpvars list", NULL, NULL);
	break;

    case T_TIMEFORMAT:
    default:
	arg_common(STMT_TASK_ARGS(node));
    }
}

void exec_systask(tree node)
{
    tree t, t1;
    extern tree current_scope;
    extern char *token_buffer;
    enum radii radix;
    int is_file = 0;		/* to distiguish between $display and $fdisplay, etc */
    int level = 0;

    if (TREE_CODE(node) == TIMING_CHECK) {
	timingCheck(node);
	return;
    }

    switch (STMT_SYSTASK_TYPE(node)) {

    case USER_TASK:
	current_tf_instance = node;
	{
	    strobe_queue_t *queue = is_strobe_active(node);

	    /* Get here because of call to user task, call call_tf */
	    if (!queue)
		call_call_tf(STMT_SYSTASK_USERTF(node));
	    /* Get here at end of time unit and [ro]synchronize had been armed */
	    else {
		if (queue == &idle_strobe_queue)
		    call_misc_tf(STMT_SYSTASK_USERTF(node), REASON_SYNCH);
		else {
		    roFlag = TRUE;
		    call_misc_tf(STMT_SYSTASK_USERTF(node),
				 REASON_ROSYNCH);
		    roFlag = FALSE;
		}
	    }
	}
	break;

    case SCOPE:
	/* arg is either string or block node */
	t1 = TREE_PURPOSE(STMT_TASK_ARGS(node));
//      if (*tree_code_type [TREE_CODE (t1)] != 'b') {
	if (TREE_CODE(t1) != BLOCK_DECL) {
	    strcpy(token_buffer, IDENTIFIER_POINTER(t1));
	    t = search_scope(token_buffer, 0);	/* traverse the scope */
	    if (t != error_mark_node) {
		strcpy(token_buffer, IDENTIFIER_POINTER(t1));
		search_scope(token_buffer, 1);	/* actually set the scope */
	    }
	} else {
	    t = DECL_THREAD(t1);
	    if (!t)
		error("'%s' not declared", IDENT(DECL_NAME(t)), NULL_CHAR);
	    else
		look_for_and_set_scope(t);
	}

//        if (BLOCK_UP (t) == current_scope)    /* down one */
//          set_scope (t);
//        else
//          lookupward_scope (t);           /* go up and set scope */
//      }
	broadcast_tf(reason_scope);

	break;

    case SHOWSCOPES:
	printf_V("Directory of scopes at current scope level:\n");
	for (t = BLOCK_DOWN(current_scope); t; t = TREE_CHAIN(t)) {
	    if (TREE_CODE(t) == MODULE_BLOCK) {
		printf_V("module (%s), instance (%s)\n",
			 MODULE_NAME(t), IDENT(BLOCK_NAME(t)));
	    } else if (TREE_CODE(t) == TASK_BLOCK) {
		printf_V("task (%s)\n", IDENT(BLOCK_NAME(t)));
	    } else if (TREE_CODE(t) == FUNCTION_BLOCK) {
		printf_V("function (%s)\n", IDENT(BLOCK_NAME(t)));
	    } else if (TREE_CODE(t) == NAMED_BLOCK) {
		printf_V("named block (%s)\n", IDENT(BLOCK_NAME(t)));
	    }
	}
	printf_V("\nCurrent scope is (");
	print_scope((handle_t) 1, current_scope);
	printf_V(")\n");
	print_top_modules();
	break;

    case T_SHOWVARS:
	if (STMT_TASK_ARGS(node) && TREE_EXPR(STMT_TASK_ARGS(node)))
	    showvar(*TREE_EXPR_CODE(STMT_TASK_ARGS(node)));
	else {
	    for (t = BLOCK_PORTS(current_scope); t; t = TREE_CHAIN(t))
		showvar(t);
	    for (t = BLOCK_DECL(current_scope); t; t = TREE_CHAIN(t))
		showvar(t);

	    /* only module blocks have spec parameters */
	    if (TREE_CODE(current_scope) == MODULE_BLOCK) {
		for (t = MODULE_SPECDEFS(current_scope); t;
		     t = TREE_CHAIN(t))
		    if (TREE_CODE(t) == SPECPARAM_DECL) {
			showvar(t);
		    }
	    }

	}
	break;

    case SETTRACE:
	trace_flag = 1;
	normal_flag = 0;
	break;

    case CLEARTRACE:
	trace_flag = 0;
	normal_flag = 0;
	break;

    case T_UPDATETIMINGS:
	if (TREE_CODE(current_scope) == MODULE_BLOCK) {
	    specify_update_timings(current_scope);
	}
	break;

    case T_NOLOG:
	log_enable = 0;
	break;

    case T_NOKEY:
	key_enable = 0;
	break;

    case T_LOG:
	if (STMT_TASK_ARGS(node)) {
	    eval(TREE_EXPR_CODE(STMT_TASK_ARGS(node)));
	    bits_to_string(token_buffer, *--R, R_nbits);
	    fclose(log_file);
	    log_file = shell_fopen(token_buffer, "w+t");
	    if (!log_file) {
		warning("Cannot open log file '%s'", token_buffer,
			NULL_CHAR);
		log_available = 0;
		log_enable = 0;
	    } else {
		log_file_name = (char *) xmalloc(strlen(token_buffer));
		strcpy(log_file_name, token_buffer);
		log_enable = 1;
		log_available = 1;
	    }
	} else			/* no arguments */
	    log_enable = 1;
	break;

    case T_KEY:
	if (STMT_TASK_ARGS(node)) {
	    eval(TREE_EXPR_CODE(STMT_TASK_ARGS(node)));
	    bits_to_string(token_buffer, *--R, R_nbits);
	    fclose(key_file);
	    key_file = shell_fopen(token_buffer, "wt");
	    if (!key_file) {
		warning("Cannot open key file '%s'", token_buffer,
			NULL_CHAR);
		key_available = 0;
		key_enable = 0;
	    } else {
		key_file_name = (char *) xmalloc(strlen(token_buffer));
		strcpy(key_file_name, token_buffer);
		key_enable = 1;
		key_available = 1;
	    }
	} else			/* no arguments */
	    key_enable = 1;
	break;

    case T_INPUT:
	{
	    File *input;
	    extern int prompt_flag;	/* in exec.c */

	    eval(TREE_EXPR_CODE(STMT_TASK_ARGS(node)));
	    bits_to_string(token_buffer, *--R, R_nbits);
	    input = File::fopen(token_buffer, "r+t");
	    if (!input) {
		error("Cannot open input file '%s'", token_buffer,
		      NULL_CHAR);
	    } else {
//    extern char i_buf[];
		push_stream(fin, 0);
		fin = input;
		prompt_flag = 1;
//    setbuf (fin, i_buf);
	    }
	}
	break;

    case STOP:
	do_stop();
	if (STMT_TASK_ARGS(node) && TREE_EXPR(STMT_TASK_ARGS(node)))
	    print_info();
	break;

    case FINISH:
	do_finish();
	break;

    case T_FCLOSE:
	fclose_V(get_handle(TREE_EXPR_CODE(STMT_TASK_ARGS(node))));
	break;

    case T_MONITOROFF:
	monitor_info.enable = 0;
	dequeue_strobe(&final_strobe_queue, monitor_info.node);
	break;

    case T_MONITORON:
	monitor_info.enable = 1;
	tickle_monitor_old(&final_strobe_queue, &monitor_info);
	break;

    case FDISPLAY:
    case T_FWRITE:
	is_file = 1;
    case DISPLAY:
    case WRITE:
	disp_common(node, DEC, is_file, 0);
	break;

    case FDISPLAYH:
    case T_FWRITEH:
	is_file = 1;
    case DISPLAYH:
    case WRITEH:
	disp_common(node, HEX, is_file, 0);
	break;

    case FDISPLAYB:
    case T_FWRITEB:
	is_file = 1;
    case DISPLAYB:
    case WRITEB:
	disp_common(node, BIN, is_file, 0);
	break;

    case FDISPLAYO:
    case T_FWRITEO:
	is_file = 1;
    case DISPLAYO:
    case WRITEO:
	disp_common(node, OCT, is_file, 0);
	break;

/* Strobes get here in one of two ways: 1) a call to $strobe; this queues
   a message to strobe_first. 2) at the end of a simulation time; this
   actually performs the action */
    case FSTROBE:
	is_file = 1;
    case STROBE:
	if (!is_strobe_active(node))
	    enqueue_strobe(&final_strobe_queue, node);
	else
	    disp_common(node, DEC, is_file, 0);
	break;

    case FSTROBEB:
	is_file = 1;
    case STROBEB:
	if (!is_strobe_active(node))
	    enqueue_strobe(&final_strobe_queue, node);
	else
	    disp_common(node, BIN, is_file, 0);
	break;

    case FSTROBEH:
	is_file = 1;
    case STROBEH:
	if (!is_strobe_active(node))
	    enqueue_strobe(&final_strobe_queue, node);
	else
	    disp_common(node, HEX, is_file, 0);
	break;

    case FSTROBEO:
	is_file = 1;
    case STROBEO:
	if (!is_strobe_active(node))
	    enqueue_strobe(&final_strobe_queue, node);
	else
	    disp_common(node, OCT, is_file, 0);
	break;

/* Monitor gets here in one of two ways: 1) a call to $monitor which
   sets up the markers; 2) at the end of a simulation time when the action
   happens.  When an argument changes, NotifyEvent calls enqueue_strobe
   to queue the action.  Note that this is slightly different from XL;
   monitors in XL trigger when the argument expression changes -- here,
   we trigger is any operand in the expression changes.  Eg: $monitor (x+y);
   if x changes from 2->3 and y changes from 3->2, we will trigger, XL
   would not.  If we implement PLI later, we'll change it then. */

    case FMONITOR:
	is_file = 1;
    case MONITOR:
	if (!is_strobe_active(node))
	    monitor_args(node, &monitor_info);
	else
	    disp_common(node, DEC, is_file, 0);
	break;

    case FMONITORB:
	is_file = 1;
    case MONITORB:
	if (!is_strobe_active(node))
	    monitor_args(node, &monitor_info);
	else
	    disp_common(node, BIN, is_file, 0);
	break;

    case FMONITORH:
	is_file = 1;
    case MONITORH:
	if (!is_strobe_active(node))
	    monitor_args(node, &monitor_info);
	else
	    disp_common(node, HEX, is_file, 0);
	break;

    case FMONITORO:
	is_file = 1;
    case MONITORO:
	if (!is_strobe_active(node))
	    monitor_args(node, &monitor_info);
	else
	    disp_common(node, OCT, is_file, 0);
	break;

    case SHOWSTATS:
	print_stats();
	break;

    case T_READMEMH:
	radix = HEX;
	do_readmem(node, radix);
	break;

    case T_READMEMB:
	radix = BIN;
	do_readmem(node, radix);
	break;

    case T_DUMPVARS:
	if (!is_strobe_active(node)) {
	    if (dumpvar.disallow_dumpvars) {
		runtime_error(node);
		warning("Too late to call $dumpvars, this call ignored",
			NULL, NULL);
		break;
	    }

	    /* Case 1: no arguments */
	    if (!STMT_TASK_ARGS(node))
		dumpvars_all();
	    /* Case 2: Parse arguments */
	    else {
		nbits_t nbits;
		Group *val =
		    eval_(TREE_EXPR_CODE(STMT_TASK_ARGS(node)), &nbits);

		if (BVAL(val)) {
		    runtime_error(node);
		    error("Illegal value in first argument of $dumpvar",
			  NULL, NULL);
		}
		level = AVAL(val);
		if (!level)
		    level = 64000;
		for (t = TREE_CHAIN(STMT_TASK_ARGS(node)); t;
		     t = TREE_CHAIN(t))
		    if (*tree_code_type[TREE_CODE(TREE_STMT(t))] == 'b')
			dumpvars_scope_down(TREE_STMT(t), level);
		    else
			dumpvars_mark(TREE_STMT(t));
	    }
	    if (dumpvar.first_dumpvars)
		enqueue_strobe(&final_strobe_queue, node);	/* Schedule this to run at end of time */
	    dumpvar.first_dumpvars = 0;
	}

	/* Get here at the end of the time unit in which all dumpvars are
	   called */
	else {
	    dumpvar.disallow_dumpvars = 1;	/* don't allow any more calls */
	    dumpvar.unprinted = dumpvar.markers_first;
	    if (dumpvars_header(node)) {
		dumpvars_enable();
		dumpvars_checkpoint("$dumpvars");
	        dumpvar.new_time = 1;
	    }
	}
	break;

    case T_DUMPFILE:
	if (dumpvar.disallow_dumpvars) {
	    runtime_error(node);
	    error("Too late to change dump file name", NULL, NULL);
	    break;
	}
	eval(TREE_EXPR_CODE(STMT_TASK_ARGS(node)));
	bits_to_string(token_buffer, *--R, R_nbits);
	dumpvar.file_name = (char *) xmalloc(strlen(token_buffer) + 1);
	strcpy(dumpvar.file_name, token_buffer);
	break;

    case T_DUMPON:
	/* 2nd condition is needed in case $dumpon is called before dump
	   file is opened in dumpvar_header */
	if (!dumpvar.enabled && dumpvar.disallow_dumpvars) {
	    dumpvars_enable();
	    dumpvars_checkpoint("$dumpon");
	}
	break;

    case T_DUMPOFF:
	if (dumpvar.enabled) {
	    dumpvars_disable();
	    dumpvars_x("$dumpoff");
	}
	break;

    case T_DUMPALL:
	if (!dumpvar.enabled && dumpvar.disallow_dumpvars)
	    dumpvars_checkpoint("$dumpall");
	break;

    case T_DUMPFLUSH:
	dumpvars_flush();
	break;

/* $timeformat (units, prec, string, width)
   default: time_precision, 0, "", 20  */
#if 0
    case T_TIMEFORMAT:
	nbits_t nbits;
	int units, prec, width;

	t = STMT_TASK_ARGS(node);	/* first arg: units */
	Group *val = eval_(TREE_EXPR_CODE(t), &nbits);

	if (BVAL(val)) {
	    runtime_error(node);
	    error("Illegal value in first argument of $timeformat", NULL,
		  NULL);
	}
	units = AVAL(val);

	t = TREE_CHAIN(t);	/* 2nd arg: precision */
	Group *val = eval_(TREE_EXPR_CODE(t), &nbits);

	if (BVAL(val)) {
	    runtime_error(node);
	    error("Illegal value in second argument of $timeformat", NULL,
		  NULL);
	}
	prec = AVAL(val);

	t = TREE_CHAIN(t);	/* 3rd arg: suffix string */
	eval(TREE_EXPR_CODE(t));
	bits_to_string(token_buffer, *--R, R_nbits);

	t = TREE_CHAIN(t);	/* 2nd arg: precision */
	Group *val = eval_(TREE_EXPR_CODE(t), &nbits);

	if (BVAL(val)) {
	    runtime_error(node);
	    error("Illegal value in fourth argument of $timeformat", NULL,
		  NULL);
	}
	width = AVAL(val);

	timeformat(units, prec, token_buffer, width);
	break;
#endif
    }
}

void exec_sysfunc(tree node, nbits_t nbits)
{
    Group *g;
    struct Time64 tmpTime;

    switch FUNC_REF_SYSTYPE
	(node) {
    case USER_FUNC:
	current_tf_instance = node;
	{
	    strobe_queue_t *queue = is_strobe_active(node);

	    /* Get here because of call to user task, call call_tf */
	    if (!queue) {
		call_call_tf(FUNC_REF_USERTF(node));
		eval_nbits(FUNC_REF_RETURNDECL(node), TREE_NBITS(node));
		/* Get here at end of time unit and [ro]synchronize had been armed */
	    } else {
		if (queue == &idle_strobe_queue)
		    call_misc_tf(FUNC_REF_USERTF(node), REASON_SYNCH);
		else
		    call_misc_tf(FUNC_REF_USERTF(node), REASON_ROSYNCH);
	    }
	}
	break;
    case F_TIME:
	tmpTime = CurrentTime;
	timescale_divide(&tmpTime, FUNC_REF_CONTEXT(node));
	AVAL(DECL_STORAGE(systime_return)) = tmpTime.timel;
	BVAL(DECL_STORAGE(systime_return)) = 0;
	AVAL(DECL_STORAGE(systime_return) + 1) = tmpTime.timeh;
	BVAL(DECL_STORAGE(systime_return) + 1) = 0;
	eval_nbits(systime_return, nbits);
	break;
    case F_STIME:
	tmpTime = CurrentTime;
	timescale_divide(&tmpTime, FUNC_REF_CONTEXT(node));
	AVAL(DECL_STORAGE(sysstime_return)) = tmpTime.timel;
	BVAL(DECL_STORAGE(sysstime_return)) = 0;
	eval_nbits(sysstime_return, nbits);
	break;
    case F_REALTIME:
	tmpTime = CurrentTime;
	REAL_(DECL_STORAGE(sysrealtime_return)) =
	    timescale_divide(&tmpTime, FUNC_REF_CONTEXT(node));
	eval_nbits(sysrealtime_return, nbits);
	break;
    case F_FOPEN:
	eval((tree *) TREE_PURPOSE(FUNC_REF_INASSIGN(node)));
	{
	    unsigned_32_t handle;
	    extern char *token_buffer;

	    bits_to_string(token_buffer, *--R, R_nbits);
	    handle = fopen_V(token_buffer);
	    AVAL(DECL_STORAGE(sysopen_return)) = handle;
	    BVAL(DECL_STORAGE(sysopen_return)) = 0;
	    eval_nbits(sysopen_return, nbits);
	    break;
	}
    case F_RANDOM:
	{
	    int result;
	    if (FUNC_REF_INASSIGN(node)) {	/* seed present? */
	        int seed;
	        eval_1(FUNC_REF_INASSIGN(node));
	        seed = AVAL(*--R);
	        g = *R;
	        result = rtl_dist_uniform (&seed, INT_MIN, INT_MAX);
	        AVAL(g) = (unsigned)seed;
	        *++R = g + 1;
	        store(FUNC_REF_INASSIGN(node), node);
	    } else {
	        result = rtl_dist_uniform (&globalSeed, INT_MIN, INT_MAX);
	    }
	    AVAL(DECL_STORAGE(sysrand_return)) = result;
	    BVAL(DECL_STORAGE(sysrand_return)) = 0;
	    eval_nbits(sysrand_return, nbits);
	}
	break;
    case F_TEST_PLUSARGS:
	eval((tree *) TREE_PURPOSE(FUNC_REF_INASSIGN(node)));
	{
	    extern char *token_buffer;
	    tree t;

	    bits_to_string(token_buffer, *--R, R_nbits);
	    AVAL(DECL_STORAGE(sysplus_return)) = 0;
	    BVAL(DECL_STORAGE(sysplus_return)) = 0;
	    for (t = plusargs; t; t = TREE_CHAIN(t)) {
		if (!strcmp(token_buffer, (char *) TREE_PURPOSE(t))) {
		    /* Found a plusarg */
		    AVAL(DECL_STORAGE(sysplus_return)) = 1;
		    break;
		}
	    }
	    eval_nbits(sysplus_return, nbits);
	    break;
	}

	}			/* switch */
}

/* Process monitor  */

void monitor_args(tree node, struct monitor_info *monitor_info)
{
    tree t;
    extern struct Marker_info marker_info;


    if (monitor_info->node != NULL_TREE) {
	/* clear the markers from previous invocation */
	for (t = STMT_TASK_ARGS(monitor_info->node); t; t = TREE_CHAIN(t))
	    if (TREE_EXPR(t))
		event_undo((Marker *) TREE_STMT(t));
	dequeue_strobe(&final_strobe_queue, monitor_info->node);

    }
    monitor_info->node = node;

    for (t = STMT_TASK_ARGS(node); t; t = TREE_CHAIN(t))
	if (TREE_EXPR(t)) {
	    tree save_current_scope = current_scope;
	    current_scope = STMT_SCOPE(node);	/* use this when resolving
						   hierarchical names */
	    TREE_EXPR_CODE(t) = pass3_expr_marker(TREE_EXPR(t),
						  (Marker **) &
						  TREE_STMT(t),
						  (enum
						   marker_flags) (M_STROBE
								  +
								  M_FIXED),
						  (tree) monitor_info,
						  NULL_TREE);
	    current_scope = save_current_scope;
	}

    enqueue_strobe(&final_strobe_queue, node);	/* Tickle it now */
}

#if 0
void monitor_args(tree node, struct monitor_info *monitor_info)
{
    SCB *save_scb;
    tree t;
    extern struct Marker_info marker_info;


    save_scb = marker_info.current_scb;
    if (monitor_info->scb) {
	/* clear the markers from previous invocation */
	for (t = STMT_TASK_ARGS(monitor_info->scb->pc); t;
	     t = TREE_CHAIN(t))
	    if (TREE_EXPR(t))
		event_undo(TREE_STMT(t));
	dequeue_strobe(&final_strobe_queue, monitor_info->scb->pc);

	FreeSCB(monitor_info->scb);
    }
    /* Only one $monitor allow at on time */
    monitor_info->scb = marker_info.current_scb = BuildSCB(node, NOLIST);
//  monitor_info->scb->context->scope = readylist->current_scope;
//printf ("Scope is %s\n", IDENT(readylist->current_scope->block_common.name));
    /* Save a pointer to out info in "fork" (ok, so its available). */
//  monitor_info->scb->fork = (SCB*) monitor_info;
    monitor_info->node = node;

    for (t = STMT_TASK_ARGS(node); t; t = TREE_CHAIN(t))
	if (TREE_EXPR(t))
	    TREE_EXPR_CODE(t) = pass3_expr_marker(TREE_EXPR(t),
						  (Marker **) &
						  TREE_STMT(t),
						  M_STROBE + M_FIXED,
						  (tree) monitor_info);
    enqueue_strobe(&final_strobe_queue, node);	/* Tickle it now */
    marker_info.current_scb = save_scb;
}
#endif

int
disp_common(tree node, enum radii default_radix, int is_file,
	    int check_only)
{
    tree t, t1;
    Group *g;
    Group *val;
    nbits_t nbits;
    int c, j;
    ngroups_t i;
    int attr = 0;
    handle_t handle;
    int is_integer = 0, fill_space = 1;
    char length_spec[10] = "%", *ls = &length_spec[1];

    t = STMT_TASK_ARGS(node);
    if (is_file) {
	if (!check_only)
	    handle = get_handle(TREE_EXPR_CODE(t));
	t = TREE_CHAIN(t);
    } else
	handle = 1;

    for (; t; t = t ? TREE_CHAIN(t) : t)	/* If too few args, t will be NULL */
	//      for (; t; t = TREE_CHAIN (t))  /* If too few args, t will be NULL */
    {

	t1 = TREE_EXPR(t);

	if (!t1) {		/* Null argument */
	    if (!check_only)
		fprintf_V(handle, " ");
	} else if (TREE_CODE(t1) == BIT_CST
		   && BIT_CST_RADIX(t1) == STRING_) {
	    is_integer = TREE_INTEGER_ATTR(t1);
	    g = BIT_CST_GROUP(t1) + bits_to_groups(TREE_NBITS(t1));
	    for (i = bits_to_groups(TREE_NBITS(t1)); i >= 0; i--, g--) {
		for (j = sizeof(Bit) - 1; j >= 0; j--) {
		    c = (AVAL(g) >> j * 8) & 255;
		    if (attr) {
			if (!t)
			    return 0;
			attr = 0;
			*ls = 0;
			switch (tolower(c)) {
			case 't':
			    t = print_next_arg(handle, t, TIME_,
					       check_only, 1, fill_space,
					       NULL, STMT_SCOPE(node));
			    ls = &length_spec[1];
			    break;
			case 'd':
			    t = print_next_arg(handle, t, DEC, check_only,
					       1, fill_space, NULL, NULL);
//                  is_integer, fill_space, NULL, NULL);
			    ls = &length_spec[1];
			    break;
			case 'o':
			    t = print_next_arg(handle, t, OCT, check_only,
					       is_integer, fill_space,
					       NULL, NULL);
			    ls = &length_spec[1];
			    break;
			case 'h':
			case 'x':
			    t = print_next_arg(handle, t, HEX, check_only,
					       is_integer, fill_space,
					       NULL, NULL);
			    ls = &length_spec[1];
			    break;
			case 'b':
			    t = print_next_arg(handle, t, BIN, check_only,
					       is_integer, fill_space,
					       NULL, NULL);
			    ls = &length_spec[1];
			    break;
			case 's':
			    t = print_next_arg(handle, t, STRING_,
					       check_only, is_integer,
					       fill_space, NULL, NULL);
			    ls = &length_spec[1];
			    break;
			case 'c':
			    t = print_next_arg(handle, t, CHAR_,
					       check_only, is_integer,
					       fill_space, NULL, NULL);
			    ls = &length_spec[1];
			    break;
			case 'e':
			    strcat(length_spec, "le");
			    t = print_next_arg(handle, t, EXP_, check_only,
					       is_integer, fill_space,
					       length_spec, NULL);
			    ls = &length_spec[1];
			    break;
			case 'f':
			    strcat(length_spec, "lf");
			    t = print_next_arg(handle, t, FLOAT_,
					       check_only, is_integer,
					       fill_space, length_spec,
					       NULL);
			    ls = &length_spec[1];
			    break;
			case 'g':
			    strcat(length_spec, "lg");
			    t = print_next_arg(handle, t, EXPFLOAT_,
					       check_only, is_integer,
					       fill_space, length_spec,
					       NULL);
			    ls = &length_spec[1];
			    break;
			case 'm':
			    if (!check_only)
//            print_scope (handle, readylist -> current_scope);
				print_scope(handle, STMT_SCOPE(node));
			    ls = &length_spec[1];
			    break;
			case '%':
			    if (!check_only)
				fprintf_V(handle, "%%%%");
			    break;
			case '0':
//          if (check_only)
//            warning ("%%0 spec not supported and will be ignored");
			    fill_space = 0;
			    attr = 1;	/* look at next character anyway */
			    *ls++ = c;
			    break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
			    if (ls < length_spec + 10) {
				*ls++ = c;
				attr = 1;
				break;
			    }
			default:
			    if (check_only)
				return 0;
			    if (length_spec[1] != '\0') {
				*ls = 0;
				fprintf_V(handle, "%%%s", length_spec);
				ls = &length_spec[1];
				*ls = 0;
			    }
			    fprintf_V(handle, "%%%c", c);
			}
		    } else if (c == '%') {
			attr = 1;
			fill_space = 1;
		    } else if (c != 0 && !check_only)
			fprintf_V(handle, "%c", c);
		}
	    }
	}

	else {
	    if (check_only) {
		if (TREE_REAL_ATTR(TREE_EXPR(t)) && default_radix != DEC)
		    error
			("Illegal arguments in call to display system task",
			 NULL, NULL);
	    } else {
		is_integer = TREE_INTEGER_ATTR(t1);
		val = eval_(TREE_EXPR_CODE(t), &nbits);
		if (TREE_REAL_ATTR(TREE_EXPR(t))) {
		    print_datum_file(handle, val, (nbits_t) 0, nbits,
				     FLOAT_, is_integer, fill_space, "%g");
		} else {
		    print_datum_file(handle, val, (nbits_t) 0, nbits,
				     default_radix, is_integer, fill_space,
				     NULL);
		}
	    }
	}
    }
    if (!check_only) {
	if (STMT_SYSTASK_TYPE(node) != WRITE &&
	    STMT_SYSTASK_TYPE(node) != WRITEH &&
	    STMT_SYSTASK_TYPE(node) != WRITEB &&
	    STMT_SYSTASK_TYPE(node) != T_FWRITE &&
	    STMT_SYSTASK_TYPE(node) != T_FWRITEH &&
	    STMT_SYSTASK_TYPE(node) != T_FWRITEB)
	    fprintf_V(handle, "\n");
	fflush_V(handle);
    }
    return 1;
}

tree
print_next_arg(handle_t handle, tree node, enum radii radix, int check,
	       int is_integer, int fill_space, char *spec, tree scope)
{
    Group *val;
    nbits_t nbits;
    tree t = TREE_CHAIN(node);
    extern void timeformatprint(handle_t, tree, double);	/* in verisys */

    if (check) {
	if (!t) {
	    error("More format specs than arguments", NULL, NULL);
	    return t;
	}

	switch (radix) {
	case HEX:
	case OCT:
	case BIN:
	case DEC:
	case STRING_:
	case CHAR_:
	    if (radix != DEC && TREE_REAL_ATTR(TREE_EXPR(t)))
		error("Illegal argument in call to display system task",
		      NULL, NULL);
	    break;
//      case EXP_: case FLOAT_: case EXPFLOAT_:
//        if (!TREE_REAL_ATTR (TREE_EXPR (t)))
//          error ("Illegal argument in call to display system task", NULL, NULL);
//  break;
	}
	return t;
    }
    val = eval_(TREE_EXPR_CODE(t), &nbits);
    is_integer = TREE_INTEGER_ATTR(*TREE_EXPR_CODE(t));
    switch (radix) {
    case EXP_:
    case FLOAT_:
    case EXPFLOAT_:
    case TIME_:
	if (!TREE_REAL_ATTR(TREE_EXPR(t)))
	    REAL_(val) = reg_to_real(val, nbits);
	if (radix == TIME_) {
	    timeformatprint(handle, scope, REAL_(val));
	    return t;
	}

	break;
    case DEC:
	if (TREE_REAL_ATTR(TREE_EXPR(t)))
	    real_to_reg(REAL_(val), val, bits_to_groups(nbits));
    }
    print_datum_file(handle, val, (nbits_t) 0, nbits, radix,
		     is_integer, fill_space, spec);
    return t;
}

void showvar(tree decl)
{
    enum tree_code code = TREE_CODE(decl);

    /* If this is a hierarchical reference, sort it out */
    if (code == SHADOW_REF) {
	decl = TREE_CHAIN(decl);
	code = TREE_CODE(decl);
    } else if (code == ERROR_MARK)
	return;

    if (code == ARRAY_DECL)
	return;
    printf_V("%s (", IDENT(DECL_NAME(decl)));
    print_scope((handle_t) 1, DECL_CONTEXT(decl));
    printf_V(") ");
    if (code != BLOCK_DECL)
	if (DECL_THREAD(decl))
	    decl = DECL_THREAD(decl);

    switch (code) {
    case REG_VECTOR_DECL:
	printf_V("reg [%lu, %lu] = ", MSB(decl), LSB(decl));
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case REG_SCALAR_DECL:
	printf_V("reg = ");
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case NET_VECTOR_DECL:
	trace_net_type((enum tree_type) TREE_TYPE(decl));
	printf_V("[%lu, %lu] = ", MSB(decl), LSB(decl));
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case NET_SCALAR_DECL:
	trace_net_type((enum tree_type) TREE_TYPE(decl));
	printf_V("= ");
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case INTEGER_DECL:
	printf_V("integer = ");
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case PARAM_DECL:
	printf_V("parameter = ");
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case SPECPARAM_DECL:
	printf_V("specparam = ");
	trace_result(DECL_STORAGE(decl), TREE_NBITS(decl),
		     TREE_NBITS(decl), 0);
	break;
    case BLOCK_DECL:
	switch (TREE_CODE(DECL_THREAD(decl))) {
	case MODULE_BLOCK:
	    printf_V("MODULE");
	    break;
	case NAMED_BLOCK:
	    printf_V("NAMED BLOCK");
	    break;
	case TASK_BLOCK:
	    printf_V("TASK");
	    break;
	case FUNCTION_BLOCK:
	    printf_V("FUNCTION");
	    break;
	}
	printf_V("\n");
	return;

    default:
	break;
    }
    if (var_debug_flag) {
//    printf_V ("   [line %lu, file '%s', time %lu]\n", DECL_UPDATE_LINE (decl),
//          DECL_UPDATE_FILE (decl), DECL_UPDATE_TIME (decl));
	if (DECL_UPDATE_FILE(decl)) {
	    printf_V("\t[line %lu, file '%s', time ",
		     DECL_UPDATE_LINE(decl), DECL_UPDATE_FILE(decl));
	    print_time(&DECL_UPDATE_TIME(decl));
	    printf_V("]\n");
	} else
	    printf("\t[** Untouched **]\n");
    } else
	printf_V("\n");
}

void do_readmem(tree node, enum radii radix)
{
    tree cst_node, decl, arg;
    Group *val;
    nbits_t nbits, length, numchars;
    Bit addr, start_addr, end_addr, expected_words, num_words = 0;
    File *readmem_file;
    File *save_fin;
    char *readmem_file_name;
    int c, enable_check = 0;
    int inc = 1;		/* assume increment by one; could be -1 */
    extern char *token_buffer;	/* defined in lex.c */

    lineno = STMT_SOURCE_LINE(node);
    input_filename = STMT_SOURCE_FILE(node);
    arg = STMT_TASK_ARGS(node);	/* 1st arg: file name */

    /* First, try to open file */
    eval(TREE_EXPR_CODE(arg));
    bits_to_string(token_buffer, *--R, R_nbits);
    readmem_file_name = (char *) xmalloc((strlen(token_buffer) + 1)
					 * sizeof(char));
    strcpy(readmem_file_name, token_buffer);
    readmem_file = File::fopen(readmem_file_name, "rt");
    if (!readmem_file) {
	error("Cannot open readmem file '%s'", token_buffer, NULL_CHAR);
	return;
    }
    save_fin = fin;
    fin = readmem_file;

    arg = TREE_CHAIN(arg);	/* 2nd arg: array name */
    decl = TREE_PURPOSE(arg);
    if (HIERARCHICAL_ATTR(decl))
	decl = resolve_hierarchical_name(decl);

    /* If low bounds not specified, use array's low bounds */
    arg = TREE_CHAIN(arg);	/* 3rd arg: low bounds */
    if (!arg)
	start_addr = addr = ARRAY_LO(decl);
    else {
	val = eval_(TREE_EXPR_CODE(arg), &nbits);
	if (BVAL(val)) {
	    error("Illegal index value in call to %s",
		  STMT_TASK_NAME(node), NULL_CHAR);
	    goto readmem_cleanup;
	} else
	    start_addr = addr = AVAL(val);

	arg = TREE_CHAIN(arg);	/* 4th arg: high bounds */
    }
    /* If array high bounds not specified, use infinity */
    if (!arg)
	end_addr = (Bit) - 1;
    else {
	val = eval_(TREE_EXPR_CODE(arg), &nbits);
	if (BVAL(val)) {
	    error("Illegal index value in call to %s",
		  STMT_TASK_NAME(node), NULL_CHAR);
	    goto readmem_cleanup;
	} else
	    end_addr = AVAL(val);

	enable_check = 1;	/* turn on line checking */
	if (VECTOR_DIRECTION_ATTR(decl))
	    expected_words = end_addr - start_addr + 1;
	else
	    expected_words = start_addr - end_addr + 1;
    }
    /* if specified bounds is backwards or vector is backwards, set negative
       increment */
    if (addr > end_addr || !VECTOR_DIRECTION_ATTR(decl))
	inc = -1;

    /* allocate a single constant node to be used for all entries */
    cst_node = build_bit_cst(TREE_NBITS(decl), radix);

    length = TREE_NBITS(decl);

    /* Read file, look for '@' or anything else.  If too many digits,
       give warning and stop. */

    lineno = 1;
    input_filename = readmem_file_name;
    for (c = readmem_file->fgetc(); c != EOF; c = readmem_file->fgetc()) {
	c = skip_white_space(c);
	if (c == EOF)
	    break;
	if (c == '@') {
	    readmem_file->fscanf("%lx", &addr);
	    if (enable_check) {
		if (VECTOR_DIRECTION_ATTR(decl)) {
		    if (addr < start_addr || addr > end_addr)
			yyerror
			    ("Address outside specified range; aborting readmem");
		} else {
		    if (addr > start_addr || addr < end_addr)
			yyerror
			    ("Address outside specified range; aborting readmem");
		}
	    }
	    continue;
	}
	fin->fungetc(c);

	numchars = read_num_token(token_buffer);
	if (radix == HEX) {
	    if ((numchars - 1) * 4 >= length) {
		yyerror("Line too long during $readmemh; aborting");
		goto readmem_cleanup;
	    }
	} else {
	    if (numchars > length) {
		yyerror("Line too long during $readmemb; aborting");
		goto readmem_cleanup;
	    }
	}
	parse_base_const1(radix, token_buffer + numchars - 1, length,
			  numchars, cst_node);

	eval_1(cst_node);
	store_array(decl, addr);

	num_words++;
	addr += inc;
    }

  readmem_cleanup:
    if (enable_check)
	if (num_words != expected_words)
	    warning
		("Number of words read is not the number of words expected",
		 NULL_CHAR, NULL_CHAR);

    /*
     * check if file has already been closed
     */
    if (fin) {
	readmem_file->fclose();
    }
    fin = save_fin;
}

/**************************************************************
 *
 *	do_stop
 *		- stop the simulation 
 *
 *************************************************************
 */

void do_stop()
{
    normal_flag = 0;
    single_step_flag = 1;
    printf_V("Stop at simulation time ");
    print_time(&CurrentTime);
    printf_V("\n");
    broadcast_tf(reason_interactive);
}

/**************************************************************
 *
 *	do_finish
 *		- exit the simulation 
 *
 *************************************************************
 */

void do_finish()
{
    broadcast_tf(reason_finish);
    printf_V("Exiting %s at time ", VERIWELL);
    print_time(&CurrentTime);
    printf_V("\n");
    print_info();
    finish();
}

/**************************************************************
 *
 *	init_systask
 *		- initializes global and static variables for systask.c
 *
 *************************************************************
 */

void init_systask_1()
{
    systime_once = 0;		/* initialize the following only once */
    sysstime_once = 0;		/* initialize the following only once */
    sysopen_once = 0;		/* initialize the following only once */
    sysrand_once = 0;
    sysrealtime_once = 0;	/* initialize the following only once */
    sysplus_once = 0;		/* initialize the following only once */
    monitor_info.enable = 1;
    monitor_info.node = NULL_TREE;
    systime_return = NULL_TREE;
    sysstime_return = NULL_TREE;
    sysopen_return = NULL_TREE;
    sysrand_return = NULL_TREE;
    sysrealtime_return = NULL_TREE;
    sysplus_return = NULL_TREE;
    roFlag = FALSE;
    pliInstanceList = NULL_TREE;
}
