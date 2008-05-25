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




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "runtime.h"
#include "flags.h"
#include "io.h"
#include "macro.h"
#include "glue.h"
#include "strobe.h"
#include "store.h"
#include "exec.h"
#include "print.h"
#include "eval.h"
#include "decl.h"
#include "specify.h"
#include "veriuser.h"
#include "acc_user.h"
#include "pli.h"
#include "systask.h"
#include "veriwell.h"
#include "lex.h"
#include "pass3.h"
#include "trace.h"

#ifdef HAVE_LIBREADLINE
#  if defined(HAVE_READLINE_READLINE_H)
#    include <readline/readline.h>
#  elif defined(HAVE_READLINE_H)
#    include <readline.h>
#  else /* !defined(HAVE_READLINE_H) */
extern char *readline ();
#  endif /* !defined(HAVE_READLINE_H) */
char *cmdline = NULL;
#else /* !defined(HAVE_READLINE_READLINE_H) */
  /* no readline */
extern "C" char* readline(char*);
#endif /* HAVE_LIBREADLINE */

#ifdef HAVE_READLINE_HISTORY
#  if defined(HAVE_READLINE_HISTORY_H)
#    include <readline/history.h>
#  elif defined(HAVE_HISTORY_H)
#    include <history.h>
#  else /* !defined(HAVE_HISTORY_H) */
#  endif /* defined(HAVE_READLINE_HISTORY_H) */
  /* no history */
#else /* !HAVE_READLINE_HISTORY */
#endif /* HAVE_READLINE_HISTORY */



#define NULL_GROUP (Group *) NULL

#define TRACE(CURRENT, NEXT, G, SKIP, VAL)		\
{							\
  if( lcbCount != 0 && !normal_flag ) {			\
	handleLcb( CURRENT );				\
  }							\
  if (!normal_flag) {					\
    pc = go_interactive (CURRENT, NEXT, G, SKIP, VAL);  \
  } else {						\
    if ((NEXT) != NULL_TREE) {				\
      pc = NEXT;					\
    }							\
  }							\
}


extern int in_simulation;

int exit_flag = 0;
int in_disable;			/* set while processing the disable statement */

extern SCB *readylist;

static unsigned char brk_check = 0;


void Group2Time(int ngroup, Group * g, struct Time64 *t)
{
    int i;
    // convert X/Z -> 0
    for (i = 0; i < ngroup + 1; ++i) {
	if (BVAL(g + i)) {
	    t->timeh = 0;
	    t->timel = 0;
	    return;
	}
    }
    t->timel = AVAL(g);
    if (ngroup > 0) {
	t->timeh = AVAL(g + 1);
    } else {
	t->timeh = 0;
    }
}

void set_simulation_time(time64 * newtime_p)
{
    ASSIGNTIME64(&CurrentTime, newtime_p);
    if (trace_flag || single_trace_flag) {
	printf_V("SIMULATION TIME IS ");
	print_time(&CurrentTime);
	printf_V("\n");
    }
}

struct context_member *free_context;

struct context_member *make_context(void)
{
    struct context_member *new_t;

    if (free_context) {
	new_t = free_context;
	REMOVE_LIST(new_t);
	return new_t;
    } else {
	return (struct context_member *) xmalloc
	    (sizeof(struct context_member));
    }
}

void enter_context(SCB * scb, tree scope, tree pc)
{
    struct context_member *tmp;
    struct context_member *new_context;

    if (!scope) {		/* If scb is build w/out context, don't bother */
	return;
    }
    new_context = make_context();
    ADD_LIST(BLOCK_CONTEXT_LIST(scope), new_context);
    new_context->shadow = scb->context;
    scb->context = new_context;
    new_context->scb = scb;
    new_context->scope = scope;
    new_context->pc = pc;
}


tree exit_context(SCB * scb)
{
    struct context_member *tmp;
    struct context_member *context = scb->context;

    scb->context = context->shadow;
    REMOVE_LIST(context);
    ADD_LIST(free_context, context);
    return context->pc;
}


/* Starting with the innermost context, unwind until past the given
   context.  Return the outermost PC. */

tree unwind_context(struct context_member * context)
{
    while (context->scb->context != context) {
	exit_context(context->scb);
	free_context->pc = NULL;
    }
    return exit_context(context->scb);
}

void disable_context(struct context_member *context)
{
    SCB *scb;
    SCB *tmp;
    tree next_pc;
    tree current_pc;
    int me;

    /* First, get the SCB. */
    scb = context->scb;
    current_pc = scb->pc;
    me = readylist == scb;

    /* The context being disabled is in one of the following states: waiting
       for a time or event, the beginning of a fork, or at the head of the
       readylist (me).  Do the right thing for each case. */

    if (me) {
	next_pc = unwind_context(context);
    } else {
	switch (TREE_CODE(current_pc)) {
	case EVENT_STMT:
	    event_undo(STMT_EVENT_MARKER(current_pc));
	    next_pc = unwind_context(context);
	    break;

	case WAIT_STMT:
	    event_undo(STMT_WAIT_MARKER(current_pc));
	    next_pc = unwind_context(context);
	    break;

	case ASSIGN_DELAY_STMT:
	case DELAY_STMT:
	    next_pc = unwind_context(context);
	    break;

	case JOIN_STMT:
	case JOIN_NAMED_STMT:
	    while (context->next) {
		disable_context(context->next);
	    }
	    next_pc = context->pc;
	    break;

	    /* If the disable came from within a named fork, let join handle it */
	case FORK_NAMED_STMT:
	    next_pc = STMT_BODY(current_pc);	/* points to join */
	    break;
	}
    }

    ASSIGNTIME64(&(scb->time), &CurrentTime);

    scb->mode = RUN;

    if (!next_pc)		/* must be a fork member */
	if (me) {
	    /*
	     * I am one of the fork members and if I kill the SCB, then ... 
	     * ... I have to put the fork's SCB at the front of the ready list 
	     */
	    SCB *fork_scb = scb->fork;
	    ASSERT(fork_scb);
	    ASSERT(fork_scb->fork_count > 0);
	    FreeSCB(scb);
	    REMOVE_LIST_SCB(fork_scb);
	    ADD_LIST_SCB(READY_LIST, &readylist, fork_scb);
	    readylist->mode = WAIT;
	} else {
	    FreeSCB(scb);
	}

    else {
	int save_normal_flag;
	// ??? mdh - not sure why this matters but it does
	int onReadyList = (scb->list == READY_LIST);

	in_disable = 1;
	REMOVE_LIST_SCB(scb);
	ADD_LIST_SCB(READY_LIST, &readylist, scb);
	scb->pc = next_pc;
	save_normal_flag = normal_flag;
	normal_flag = 0;

	exec_(scb->pc);

	in_disable = 0;
	normal_flag = save_normal_flag;

/* Put the disabled process next of the readylist.
   In some cases, after exec-ing, the scb could have been taken off
   the ready list.  If so, don't bother putting at the end.
   Eg: disabling a named begin inside a fork-join -- this will
   exec the join stmt which calls dump_current_process.
   Also, if I am disabling myself, then there could be only
   my SCB on the readylist, so don't bother moving it  */

	if (!me && onReadyList && scb->list == READY_LIST
	    && readylist->next) {
	    SCB *last;
	    REMOVE_LIST_SCB(scb);
	    last = LAST_SCB(READY_LIST);
	    if (last) {
		ADD_LIST_SCB(READY_LIST, &last->next, scb);
	    } else {
		ADD_LIST_SCB(READY_LIST, &readylist, scb);
	    }
	}
    }
}

/* process deassign statement; find matching assignment and unthread
  the markers.  Return pc of ASSIGN or FORCE if something was actually
  released.  Note that the deassign will not happen under certain
  circumstances.  Specifically, if it was requested by a assign or deassign
  but was generated by a force, it will not happen. */

tree deassign(tree lval_arg, enum tree_code code)
{
    Marker *marker;
    tree pc;
    tree lval;

    /* if lval is hierarchical, it will be a "shadow" lval".  Point to the real
       lval. */
    lval = the_lval(lval_arg);

    for (marker = DECL_EVENT_CHAIN(lval); marker; marker = marker->next) {
	/* marker must be for continuous assignment, then it must point to
	   procedual cont assignment, then lvals must match */
	if (marker->flags & M_CONT &&
	    (TREE_CODE(marker->expr.tree) == ASSIGN_PROC_STMT ||
	     TREE_CODE(marker->expr.tree) == FORCE_STMT) &&
	    the_lval(STMT_ASSIGN_LVAL(marker->expr.tree)) == lval) {
	    pc = marker->expr.tree;
	    if (TREE_CODE(pc) == FORCE_STMT &&
		(code == ASSIGN_PROC_STMT || code == DEASSIGN_STMT)) {
		return NULL_TREE;
	    }
	    event_undo(STMT_ASSIGN_EVENT_LIST(pc));
	    return pc;
	}
    }
    return NULL_TREE;
}

int first_time_exec = 1;

tree exec_(tree pc)
{
    Group *g1, *g2;
    enum logical_value cond;
    ngroups_t i;
    int found;
    enum tree_code code;
    tree t, t1, new_pc, old_pc;
    tree scope;			// for fork

    if (!is_interactive && first_time_exec && in_simulation) {
	first_time_exec = 0;
	if (s_flag) {
	    single_step_flag = 1;
	    /* enter command line mode w/no trace */
	    pc = go_interactive(NULL_TREE, pc, NULL_GROUP, 0, 0);
	}
    } else {
	if (in_disable) {
	    pc = go_interactive(NULL_TREE, pc, NULL_GROUP, 1, 0);
	}
    }

    for (;;) {


//    if (!brk_check++) /* interrupt the process every so often to */
//      ftell (stdout); /*  guarentee that a break will be detected */

	code = TREE_CODE(pc);

	switch (code) {
	case BEGIN_STMT:
	    TRACE(pc, STMT_BODY(pc), NULL_GROUP, 0, 0);
	    break;

	case BEGIN_NAMED_STMT:
	    enter_context(readylist, STMT_BLOCK(pc), TREE_CHAIN(pc));
	    TRACE(pc, STMT_BODY(pc), NULL_GROUP, 0, 0);
	    break;

	case END_NAMED_STMT:
	    exit_context(readylist);
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0) break;

	case FORK_NAMED_STMT:
	    /* Point to PAST Join in case of disable */
	    scope = STMT_BLOCK(pc);
	    enter_context(readylist, STMT_BLOCK(pc),
			  TREE_CHAIN(TREE_CHAIN(pc)));
	    goto fork_common;
	case FORK_STMT:
	    scope = STMT_SCOPE(pc);
	    enter_context(readylist, STMT_SCOPE(pc),
			  TREE_CHAIN(TREE_CHAIN(pc)));
	  fork_common:
	    {
		SCB *fork = readylist;
		SCB *scb;
		SCB *tmp;
		SCB *where = readylist;

		fork->fork_count = 0;
		for (t = STMT_BODY(pc); t; t = TREE_CHAIN(t)) {
		    /* SCBs are built dynamically instead of at compile time.  This
		       is because in tasks, more than one process could enter an
		       SCB at one time. */
		    /* we will use the SCB's make_context to point to the innermost
		       scope, which may be the named fork scope */
		    tree saved_scope = current_scope;
		    current_scope = scope;
		    scb = BuildSCB(TREE_PURPOSE(t), NOLIST);
		    current_scope = saved_scope;
		    /* Each member has its own context.  This way, disable 
		       will work because there is a binding between the 
		       fork and all its branches */
		    REMOVE_LIST_SCB(scb);
		    /* thread in order */
		    ADD_LIST_SCB(READY_LIST, &where->next, scb);
		    fork->fork_count++;	/* Keep count of concurrent stmts */
		    scb->fork = fork;	/* point back to "main" SCB */
		    where = scb;
		}
	    }
	    readylist->pc = TREE_CHAIN(pc);
	    readylist->mode = WAIT;
	    new_pc = dispatch_pc(EVENT_LIST);
	    readylist->pc = NULL;	/* Don't trigger event stmt */
	    TRACE(pc, new_pc, NULL_GROUP, 0, 0);
	    break;

	case JOIN_STMT:
	case JOIN_NAMED_STMT:
	    /* If this is the last fork member, get fork control back on the
	       ready list and go.  Note that exit_context frees up 
	       context_member but subsequent code assumes the pointers 
	       are valid (freed context_members go on a free list and 
	       are not freed in memory).  */
	    /* Get here if FORK is disabled */
	    if (readylist->mode == WAIT) {
		readylist->mode = RUN;
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    } else {
		/* One of the members exited; get rid of it */
		SCB *fork_scb = readylist->fork;
		tree scope = readylist->context->scope;
		ASSERT(fork_scb);
		ASSERT(fork_scb->fork_count > 0);
		fork_scb->fork_count--;
		/* if all branches are finished, then scope will point to the
		   fork context; if there are some to finish, then scope 
		   will point to all the contexts still to run */
		if (fork_scb->fork_count == 0) {
		    SCB *tmp;
		    /* put mother fork next on ready lest */
		    REMOVE_LIST_SCB(fork_scb);
		    ADD_LIST_SCB(READY_LIST, &readylist->next, fork_scb);
		    readylist->next->mode = RUN;
		    exit_context(readylist);	// for the last branch
		    exit_context(readylist->next);	// for the fork
		    pc = dump_current_process();
		    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 1, 0);
		} else {
		    exit_context(readylist);
		    new_pc = dump_current_process();
		    TRACE(pc, new_pc, NULL_GROUP, 0, 0);
		}
	    }
	    break;

	case ALWAYS_BLOCK:
	    INITIALIZED_ATTR(pc) = 1;	/* make trace consistant w/XL */
	case INITIAL_BLOCK:
	    TRACE(pc, STMT_BODY(pc), NULL_GROUP, 0, 0);
	    break;

	case IF_STMT:
	    if (test(STMT_COND_CODE(pc)) == ONE) {
		TRACE(pc, STMT_THEN(pc), NULL_GROUP, 0, 0);
	    } else if (STMT_ELSE(pc)) {
		TRACE(pc, STMT_ELSE(pc), NULL_GROUP, 1, 0);
	    } else {
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 1, 0);
	    }
	    break;

	case ASSIGN_STMT:
	    eval(STMT_ASSIGN_RVAL_CODE(pc));
	    TRACE(pc, pc, *(R - 1), 1, 1);
	    store(STMT_ASSIGN_LVAL(pc), pc);
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case ASSIGN_PROC_STMT:
	case FORCE_STMT:
	    broadcast_tf(reason_force);
	    /* Make sure we start over */
	    t = deassign(STMT_ASSIGN_LVAL(pc), code);
	    /* save pc of assign that we are overriding */
	    if (code == FORCE_STMT) {
		if (t && TREE_CODE(t) == ASSIGN_PROC_STMT) {
		    STMT_ASSIGN_TMP(pc) = t;
		} else {
		    STMT_ASSIGN_TMP(pc) = NULL_TREE;
		}
	    }
	    thread_all_markers(STMT_ASSIGN_EVENT_LIST(pc));
	    eval(STMT_ASSIGN_RVAL_CODE(pc));
	    /* store force value for later use */
	    store(STMT_FORCE_TMP_DECL(pc), pc);
	    eval_1(STMT_FORCE_TMP_DECL(pc));
	    TRACE(pc, pc, *(R - 1), 1, 1);
	    store(STMT_ASSIGN_LVAL(pc), pc);
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case DEASSIGN_STMT:
	case RELEASE_STMT:
	    broadcast_tf(reason_release);
	    t = deassign(the_lval(STMT_BODY(pc)), code);
	    /* If was we are releasing still has an ASSIGN on it,
	       reactivate the assign */
	    if (t && code == RELEASE_STMT && STMT_ASSIGN_TMP(t)) {
		thread_all_markers(STMT_ASSIGN_EVENT_LIST
				   (STMT_ASSIGN_TMP(t)));
		do_net_assignment(STMT_ASSIGN_TMP(t));
		/* if net, reevaluate */
	    } else if (is_all_net(the_lval(STMT_BODY(pc)))) {
		/* if part or bit */
		t1 = root_port_decl(the_lval(STMT_BODY(pc)));
		do_net_eval(t1, 0, NULL_TREE);	/* evaluate it */
		store(t1, pc);	/* and store it */
	    }

	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case ASSIGN_CONT_STMT:
	    if (CMPTIME64(&CurrentTime, &(readylist->time))) {
		set_simulation_time(&(readylist->time));
	    }
	    eval(STMT_ASSIGN_RVAL_CODE(pc));
	    TRACE(pc, pc, *(R - 1), 1, 1);
	    store(STMT_ASSIGN_LVAL(pc), pc);
	    if (NET_ASSIGN_ATTR(STMT_ASSIGN_LVAL(pc)) &&
		!PORT_COLLAPSED_ATTR(TREE_CHAIN(STMT_ASSIGN_LVAL(pc)))) {
		tree net = TREE_CHAIN(STMT_ASSIGN_LVAL(pc));
		do_net_eval(net, 0, NULL_TREE);
		TRACE(net, net, *(R - 1), 1, 1);
		store(net, net);
	    }
	    new_pc = suspend_current_process();
	    TRACE(pc, new_pc, *R, 0, 0);
	    break;

	case NET_SCALAR_DECL:
	case NET_VECTOR_DECL:
	    DEBUGPRINT("Doing net exec\n");
	    if (CMPTIME64(&CurrentTime, &(readylist->time))) {
		set_simulation_time(&(readylist->time));
	    }
	    do_net_eval(pc, 0, NULL_TREE);
	    TRACE(pc, pc, *(R - 1), 1, 1);
            retrigger = NULL;
	    store(pc, pc);
	    /* Calculate output port seperately and only if connected */
	    if ((PORT_OUTPUT_ATTR(pc) ||
		 PORT_COLLAPSED_ATTR(pc)) && DECL_THREAD(pc)) {
		do_net_eval(pc, 1, NULL_TREE);
		store(DECL_THREAD(pc), pc);
	    }
	    if (!readylist) {
		fatal("Caught it!", NULL_CHAR);
	    }
            if( retrigger ) {
                retrigger = NULL;
                ASSERT( readylist == retrigger );
                readylist->time = CurrentTime;
                new_pc = dispatch_pc(TIME_LIST);
            } else {
                new_pc = suspend_current_process();
            }

	    TRACE(pc, new_pc, NULL_GROUP, 0, 0);
	    break;

	case FOR_STMT:
	    eval(STMT_ASSIGN_RVAL_CODE(STMT_FOR_ASSIGN(pc)));
	    TRACE(pc, pc, *(R - 1), 0, 1);
	    store(STMT_ASSIGN_LVAL(STMT_FOR_ASSIGN(pc)), pc);
	    eval(STMT_FOR_COND_CODE(pc));
	    DETERMINE_TRUE_COND(g1);
	    if (cond == ONE) {
		TRACE(pc, STMT_BODY(pc), NULL_GROUP, 1, 0);
	    } else {
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    }
	    break;

	case WHILE_STMT:
	    if (test(STMT_WHILE_COND_CODE(pc)) == ONE) {
		TRACE(pc, STMT_BODY(pc), NULL_GROUP, 0, 0);
	    } else {
		TRACE(pc, TREE_CHAIN(pc), (Group *) TREE_CHAIN(pc), 1, 0);
	    }
	    break;

	case REPEAT_INIT_STMT:
	    eval(STMT_REPEAT_EXPR_CODE(pc));
	    AVAL(STMT_REPEAT_VAR(pc)) = AVAL(*--R);
	    BVAL(STMT_REPEAT_VAR(pc)) = BVAL(*R);

	    t = pc;
	    if (BVAL(STMT_REPEAT_VAR(pc))) {
		AVAL(STMT_REPEAT_VAR(pc)) = (unsigned int) -1;
		BVAL(STMT_REPEAT_VAR(pc)) = (unsigned int) -1;
		pc = TREE_CHAIN(pc);
	    } else if (AVAL(STMT_REPEAT_VAR(pc))) {
		pc = STMT_BODY(pc);
	    } else {
		pc = TREE_CHAIN(pc);
	    }
	    TRACE(t, pc, STMT_REPEAT_VAR(t), 1, 0);
	    break;

	case REPEAT_STMT:
	    t = pc;
	    if (BVAL(STMT_REPEAT_VAR(pc))) {
		AVAL(STMT_REPEAT_VAR(pc)) = (unsigned int) -1;
		BVAL(STMT_REPEAT_VAR(pc)) = (unsigned int) -1;
		pc = TREE_CHAIN(pc);
	    } else if (--AVAL(STMT_REPEAT_VAR(pc))) {
		pc = STMT_BODY(pc);
	    } else {
		pc = TREE_CHAIN(pc);
	    }
	    TRACE(t, pc, STMT_REPEAT_VAR(t), 1, 0);
	    break;

	case CASE_STMT:
	case CASEZ_STMT:
	case CASEX_STMT:
	    /* evaluate the case expression and leave on stack.  Eval
	       choices until match.  If no match, go to default, if there
	       is one. */
	    eval(STMT_CASE_EXPR_CODE(pc));
	    if (!INTERRUPT_FLAG_ATTR(pc)) {
		old_pc = pc;
		TRACE(pc, pc, *(R - 1), 0, 0);
		if (pc != old_pc) {	/* A new statement has been entered... */
		    --R;	/*    unwind the stack... */
		    /* abd remember that this happened. */
		    INTERRUPT_FLAG_ATTR(old_pc) = 1;
		    break;
		}
	    } else {
		INTERRUPT_FLAG_ATTR(pc) = 0;
	    }
	    found = 0;
	    for (t = STMT_CASE_LIST(pc); !found && t; t = TREE_CHAIN(t)) {
		for (t1 = TREE_EXPR(t); !found && t1; t1 = TREE_CHAIN(t1)) {
		    eval(TREE_EXPR_CODE(t1));
		    found = 1;
		    g2 = *--R;
		    g1 = *(R - 1);
		    if (code == CASE_STMT) {
			for (i = 0; i < R_ngroups; i++, g1++, g2++) {
			    if ((AVAL(g1) != AVAL(g2))
				|| (BVAL(g1) != BVAL(g2))) {
				found = 0;
				break;
			    }
			}

			found = found && !(((AVAL(g1) ^ AVAL(g2)) & R_mask)	/* all zeros if equal */
					   ||((BVAL(g1) ^ BVAL(g2)) &
					      R_mask));
		    } else if (code == CASEZ_STMT) {
			Bit aval1;
			Bit bval1;
			Bit aval2;
			Bit bval2;
			for (i = 0; i < R_ngroups; i++, g1++, g2++) {
			    aval1 = AVAL(g1);
			    bval1 = BVAL(g1);
			    aval2 = AVAL(g2);
			    bval2 = BVAL(g2);
			    /* All ones if equal (Z is don't care) */
			    if (((aval1 ^ aval2) | (bval1 ^ bval2))
				& (~bval1 | aval1) & (~bval2 | aval2)) {
				found = 0;
				break;
			    }
			}

			aval1 = AVAL(g1);
			bval1 = BVAL(g1);
			aval2 = AVAL(g2);
			bval2 = BVAL(g2);
			/* If equal, then all zeros.  Zs force zeros. */
			found = found
			    && !(((aval1 ^ aval2) | (bval1 ^ bval2))
				 & (~bval1 | aval1) & (~bval2 | aval2)
				 & R_mask);
		    } else if (code == CASEX_STMT) {
			Bit aval1;
			Bit bval1;
			Bit aval2;
			Bit bval2;
			for (i = 0; i < R_ngroups; i++, g1++, g2++) {
			    aval1 = AVAL(g1);
			    bval1 = BVAL(g1);
			    aval2 = AVAL(g2);
			    bval2 = BVAL(g2);
			    /* All zeros if equal (Z and X are don't care) */
			    if (((aval1 ^ aval2) | (bval1 ^ bval2))
				& ~bval1 & ~bval2) {
				found = 0;
				break;
			    }
			}

			aval1 = AVAL(g1);
			bval1 = BVAL(g1);
			aval2 = AVAL(g2);
			bval2 = BVAL(g2);
			found = found
			    && !(((aval1 ^ aval2) | (bval1 ^ bval2))
				 & ~bval1 & ~bval2 & R_mask);
		    } else {
			fatal("Bad CASE Exec", NULL_CHAR);
		    }

		    if (found) {
			pc = TREE_STMT(t);
		    }
		}
	    }
	    if (!found) {
		if (STMT_CASE_DEFAULT(pc)) {
		    pc = STMT_CASE_DEFAULT(pc);
		} else {
		    pc = TREE_CHAIN(pc);
		}
	    }
	    R--;
	    break;

	case ASSIGN_DELAY_STMT:
	    if (readylist->mode == RUN) {
		eval(STMT_ASSIGN_RVAL_CODE(pc));
		store(STMT_ASSIGN_TMP(pc), pc);
		eval(STMT_DELAY_EXPR_CODE(STMT_ASSIGN_DELAY(pc)));
		struct Time64 t;
		Group2Time(R_ngroups, *--R, &t);
		new_pc = WaitOnTime64(&t, pc);
		TRACE(pc, pc, *R, 1, 1);
		TRACE(pc, new_pc, NULL_GROUP, 0, 1);
	    } else {
		readylist->mode = RUN;
		if (CMPTIME64(&CurrentTime, &(readylist->time))) {
		    set_simulation_time(&(readylist->time));
		}
		eval(STMT_ASSIGN_TMPCODE(pc));
		TRACE(pc, pc, *(R - 1), 1, 0);
		store(STMT_ASSIGN_LVAL(pc), pc);
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    }
	    break;

	case ASSIGN_NONBLK_DELAY_STMT:
	    if (readylist->mode == RUN) {
		SCB *scb;
		scb = BuildSCB(pc, NOLIST);
		enter_context(scb, readylist->context->scope, NULL_TREE);
		/* each execution of nonblocking 
		   delay requires new temp var */
		scb->here.decl = make_tmp_decl(STMT_ASSIGN_LVAL(pc));
		STMT_ASSIGN_TMP(pc) = (tree) scb;	/* save for trace */
		eval(STMT_ASSIGN_RVAL_CODE(pc));
		store(scb->here.decl, pc);
		store_computeOnly = 1;
		store_scb = scb;
		store(STMT_ASSIGN_LVAL(pc), pc);
		store_computeOnly = 0;
		if (STMT_ASSIGN_DELAY(pc)) {
		    eval(STMT_DELAY_EXPR_CODE(STMT_ASSIGN_DELAY(pc)));
		    struct Time64 t;
		    Group2Time(R_ngroups, *--R, &t);
		    ScheduleDelta64(&t, scb, 0);
		} else {
		    Schedule((delay_t) 0, scb, 0);
		}
		scb->pc = pc;
		TRACE(pc, pc, *R, 1, 1);
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 1);
	    } else {
		readylist->mode = RUN;
		if (CMPTIME64(&CurrentTime, &(readylist->time)) != 0) {
		    set_simulation_time(&(readylist->time));
		}
		eval_1(readylist->here.decl);
		TRACE(pc, pc, *(R - 1), 1, 0);
		store_useCached = 1;
		store(STMT_ASSIGN_LVAL(pc), pc);
		store_useCached = 0;
		free_tmp_decl(readylist->here.decl);
		exit_context(readylist);
		new_pc = dump_current_process();
		TRACE(pc, new_pc, NULL_GROUP, 0, 0);
	    }
	    break;

	case PATH_OUTPUT:
	    if (CMPTIME64(&CurrentTime, &(readylist->time)) != 0) {
		set_simulation_time(&(readylist->time));
	    }
	    propagate_specify_path(pc);
	    new_pc = suspend_current_process();
	    TRACE(pc, new_pc, NULL_GROUP, 0, 0);
	    break;

	case DELAY_STMT:
	    if (readylist->mode == RUN) {
		eval(STMT_DELAY_EXPR_CODE(pc));
		struct Time64 t;
		Group2Time(R_ngroups, *--R, &t);
		new_pc = WaitOnTime64(&t, pc);
		TRACE(pc, new_pc, *R, 0, 1);
	    } else {
		readylist->mode = RUN;
		if (CMPTIME64(&CurrentTime, &(readylist->time)) != 0) {
		    set_simulation_time(&(readylist->time));
		}
		TRACE(pc, STMT_BODY(pc), NULL_GROUP, 1, 0);
	    }
	    break;

	case WAIT_STMT:
	    if (test(STMT_WAIT_EXPR_CODE(pc)) == ONE) {
		readylist->mode = RUN;
		TRACE(pc, STMT_BODY(pc), NULL_GROUP, 1, 0);
	    } else {
		new_pc = WaitOnEventAll(STMT_WAIT_MARKER(pc), pc);
		TRACE(pc, new_pc, NULL_GROUP, 0, 0);
		/* Make it so WAIT_STMT will be reexecuted when notified */
	    }
	    break;

	case ASSIGN_EVENT_STMT:
	    if (readylist->mode == RUN) {
		eval(STMT_ASSIGN_RVAL_CODE(pc));
		store(STMT_ASSIGN_TMP(pc), pc);
		/* Set the state from which to detect edge */
		for (t = STMT_EVENT_EXPR_LIST(STMT_ASSIGN_EVENT(pc)); t;
		     t = TREE_CHAIN(t)) {
		    if (TREE_EXPR_CODE(t)) {
			eval(TREE_EXPR_CODE(t));
			--R;
		    }
		}
		new_pc = WaitOnEventAll(STMT_EVENT_MARKER
					(STMT_ASSIGN_EVENT(pc)), pc);
		TRACE(pc, pc, *R, 1, 1);
		TRACE(pc, new_pc, NULL_GROUP, 0, 1);
	    } else {
		readylist->mode = RUN;
		eval(STMT_ASSIGN_TMPCODE(pc));
		TRACE(pc, pc, *(R - 1), 1, 0);
		store(STMT_ASSIGN_LVAL(pc), pc);
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    }
	    break;

	case ASSIGN_NONBLK_EVENT_STMT:
	    if (readylist->mode == RUN) {
		eval(STMT_ASSIGN_RVAL_CODE(pc));
		store(STMT_ASSIGN_TMP(pc), pc);
		/* Set the state from which to detect edge */
		for (t = STMT_EVENT_EXPR_LIST(STMT_ASSIGN_EVENT(pc)); t;
		     t = TREE_CHAIN(t)) {
		    if (TREE_EXPR_CODE(t)) {
			eval(TREE_EXPR_CODE(t));
			--R;
		    }
		}

		WaitOnEventOther(STMT_ASSIGN_SCB(pc),
				 STMT_EVENT_MARKER(STMT_ASSIGN_EVENT(pc)),
				 pc);
		TRACE(pc, pc, *R, 1, 1);
		TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 1);
	    } else {
		readylist->mode = RUN;
		eval(STMT_ASSIGN_TMPCODE(pc));
		TRACE(pc, pc, *(R - 1), 1, 0);
		store(STMT_ASSIGN_LVAL(pc), pc);
		new_pc = dispatch_pc(EVENT_LIST);
		TRACE(pc, new_pc, NULL_GROUP, 0, 0);
	    }
	    break;

	case EVENT_STMT:
	    if (readylist->mode == RUN) {
		/* Set the state from which to detect edge */
		for (t = STMT_EVENT_EXPR_LIST(pc); t; t = TREE_CHAIN(t)) {
		    if (TREE_EXPR_CODE(t)) {
			eval(TREE_EXPR_CODE(t));
			--R;
		    }
		}
		new_pc = WaitOnEventAll(STMT_EVENT_MARKER(pc), pc);
		TRACE(pc, new_pc, NULL_GROUP, 0, 0);
	    } else {
		readylist->mode = RUN;
		TRACE(pc, STMT_BODY(pc), NULL_GROUP, 1, 0);
	    }
	    break;

	case ARROW_STMT:
	    if (DECL_EVENT_CHAIN(STMT_ARROW_EVENT(pc))) {
		NotifyEvent(DECL_EVENT_CHAIN(STMT_ARROW_EVENT(pc)),
			    ZERO, (int) 0);
	    }
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case FOREVER_STMT:
	    TRACE(pc, STMT_BODY(pc), NULL_GROUP, 0, 0);
	    break;

	case TASK_STMT:
	    if (!free_context || free_context->pc != pc) {
		t1 = TASK_INPUT(STMT_TASK_TASK(pc));
		for (t = STMT_TASK_INASSIGN(pc); t;
		     t = TREE_CHAIN(t), t1 = TREE_CHAIN(t1)) {
		    eval((tree *) TREE_PURPOSE(t));
		    store(TREE_PURPOSE(t1), STMT_TASK_TASK(pc));
		}
		enter_context(readylist, STMT_TASK_TASK(pc), pc);
		TRACE(pc, BLOCK_BODY(STMT_TASK_TASK(pc)), NULL_GROUP, 0,
		      0);
	    } else {
		/* Get here on DISABLE */
		free_context->pc = NULL;
		pc = TREE_CHAIN(pc);
	    }
	    break;

	case TASK_BLOCK:
	    new_pc = exit_context(readylist);
	    free_context->pc = NULL;
	    t1 = TASK_OUTPUT(STMT_TASK_TASK(new_pc));
	    for (t = STMT_TASK_OUTASSIGN(new_pc); t;
		 t = TREE_CHAIN(t), t1 = TREE_CHAIN(t1)) {
		eval((tree *) TREE_PURPOSE(t1));
		store(TREE_PURPOSE(t), new_pc);
	    }
	    TRACE(pc, TREE_CHAIN(new_pc), NULL_GROUP, 0, 0);
	    break;

	case SYSTASK_STMT:
	    TRACE(pc, pc, NULL_GROUP, 1, 0);
	    exec_systask(pc);
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case FUNCTION_REF:
	    TRACE(pc, BLOCK_BODY(FUNC_REF_FUNC(pc)), NULL_GROUP, 0, 0);
	    break;

	case FUNCTION_BLOCK:
	    TRACE(pc, pc, NULL_GROUP, 0, 0);
	    return pc;

	case DISABLE_STMT:
	    broadcast_tf(reason_disable);
	    {
		tree scope = STMT_DISABLE_SCOPE(pc);
		TRACE(pc, pc, NULL_GROUP, 1, 0);
		readylist->pc = TREE_CHAIN(pc);
		while (BLOCK_CONTEXT_LIST(scope)) {
		    disable_context(BLOCK_CONTEXT_LIST(scope));
		}
	    }
	    TRACE(pc, readylist->pc, NULL_GROUP, 0, 0);
	    break;

	case DUMMY_RETURN_STMT:
	    return pc;

	case NULL_STMT:
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case LOOPBACK_STMT:
	    pc = STMT_BODY(pc);
	    break;

	case DUMP_STMT:	/* removes INITIAL blocks and interactive stmts */
	    pc = dump_current_process();
	    break;

	case FINISH_STMT:
	    print_info();
	    finish();

	case END_STMT:
	    TRACE(pc, TREE_CHAIN(pc), NULL_GROUP, 0, 0);
	    break;

	case PLI_DELAY:
	    {
		struct SCB *scb;
		tree task;

		if (CMPTIME64(&CurrentTime, &(readylist->time))) {
		    set_simulation_time(&(readylist->time));
		}
		scb = readylist;
		if (PLI_DELAY_LVAL(pc) == NULL) {
		    scb->pc = PLI_DELAY_NEXT(pc);
		    task = PLI_DELAY_NODE(pc);
		    systaskDoDelay(pc);
		    freePliDelayNode(pc);
		    if (scb->pc == NULL) {
			switch (TREE_CODE(task)) {
			case SYSTASK_STMT:
			    FUNC_REF_DELAYSCB(task) = NULL;
			    break;
			case SYSFUNCTION_REF:
			    STMT_SYSTASK_DELAYSCB(task) = NULL;
			    break;
			}
			pc = dump_current_process();
		    } else {
			scb->time = PLI_DELAY_TIME(scb->pc);
			pc = dispatch_pc(TIME_LIST);
		    }
		} else {
		    freePliDelayNode(pc);
		    *R++ = PLI_DELAY_STORAGE(pc);
		    store(PLI_DELAY_LVAL(pc), pc);
		    pc = dump_current_process();
		}
	    }
	    break;
	case GATE_INSTANCE:
	    {
		SCB* scb = readylist;
		if( scb->here.marker ) {
		    NotifyEvent( scb->here.marker, ZERO, 0 );
		}
		pc = dispatch_pc(FREE_LIST);
	    }
	    break;

	case 0:
	    printf_error_V("Attempt to execute NULL instruction!\n");
	    abort();
	    break;

	default:
	    printf_error_V("Unimplemented instruction: %s\n",
			   tree_code_name[TREE_CODE(pc)]);
	    abort();
	    break;

	}			/* switch */
    }				/* while */
}

/* Used by continuous assignments from the scheduler.  PC points to
   a assign_cont_stmt node.  If there is a delay in the sourced net,
   put its SCB onto the time list. */

void do_net_assignment(tree pc, int force )
{
    tree net;

    /* 
     * if this is a force and activation is caused by
     * net or varible being changed, don't reevaluate
     * rval, used cached value.
     */
    if (force) {
	eval_1(STMT_FORCE_TMP_DECL(pc));
    } else {
        eval(STMT_ASSIGN_RVAL_CODE(pc));
	if( TREE_CODE(pc) == ASSIGN_PROC_STMT ||
	    TREE_CODE(pc) == FORCE_STMT ) {
	    store( STMT_FORCE_TMP_DECL(pc),pc );
	    eval_1(STMT_FORCE_TMP_DECL(pc));
	}
    }
    if (in_simulation && (trace_flag || single_trace_flag)) {
	printf_V("** Triggering Continuous Assignment(force=%d):\n", force);
	trace_stmt(pc, *(R - 1), 1, 0);
    }
    store(STMT_ASSIGN_LVAL(pc), pc);

    /* If continuous assignment is actually a net assignment (wire foo = bar;)
       then go ahead and evaluate it now.  This is here for compatibility;
       presumably, XL does this naturally. */

    if (NET_ASSIGN_ATTR(STMT_ASSIGN_LVAL(pc)) &&
	!PORT_COLLAPSED_ATTR(TREE_CHAIN(STMT_ASSIGN_LVAL(pc)))) {
	net = TREE_CHAIN(STMT_ASSIGN_LVAL(pc));
	do_net_eval(net, 0, NULL_TREE);
	TRACE(net, net, *(R - 1), 1, 1);
	store(net, net);
    }
}

int in_initial = 0;

/* Called by verilog.c to initialize nets before execution */
void initialize_cont_assignments()
{
    tree t, pc;
    int i = 0;

    in_initial = 1;
    for (t = nreverse(continuous_assignments); t; t = TREE_CHAIN(t), i++) {
	pc = TREE_PURPOSE(t);
	if (TREE_CODE(pc) == NET_SCALAR_DECL
	    || TREE_CODE(pc) == NET_VECTOR_DECL) {
	    ScheduleNet(DECL_EVENT_CHAIN(pc), ZERO);
	    continue;
	}
	eval(STMT_ASSIGN_RVAL_CODE(pc));
	store(STMT_ASSIGN_LVAL(pc), pc);
	if (NET_ASSIGN_ATTR(STMT_ASSIGN_LVAL(pc))) {
	    tree net = TREE_CHAIN(STMT_ASSIGN_LVAL(pc));
	    do_net_eval(net, 0, NULL_TREE);
	    TRACE(net, net, *(R - 1), 1, 1);
	    store(net, net);
	}
    }
    in_initial = 0;
}

/* During net initialization, sched will call this */
void net_exec(tree pc)
{
    DEBUGPRINT("Doing net exec\n");
    do_net_eval(pc, 0, NULL_TREE);
    TRACE(pc, pc, *(R - 1), 1, 0);
    store(pc, pc);
    if ((PORT_OUTPUT_ATTR(pc) || PORT_COLLAPSED_ATTR(pc))
	&& DECL_THREAD(pc)) {
	do_net_eval(pc, 1, NULL_TREE);
	store(DECL_THREAD(pc), pc);
    }
}

void exec_history()
{
    sorry("Command history not supported in this version");
}

void exec_remove_history()
{
    sorry("Command history not supported in this version");
}

int new_cmd_level = 1;
int cmd_level;
int prompt_flag = 1;
static int save_char = 0;

tree
go_interactive(tree current_pc, tree next_pc, Group * g, int skip, int val)
{
    int c;
    int ignore_cr = 0;
    int fail;
    extern tree interactive_statement;
    extern int errorcount;	/* from verilog.c */
    int tmp_error;
    extern tree dummy_return;	/* def'd in pass3.c; used for disable stmt */
    extern clock_t clock_pause;	/* def'd in verilog.c */
    clock_t clk_pause;		/* pause timer during interaction */

    if ((trace_flag || single_trace_flag) && current_pc) {
	switch (TREE_CODE(current_pc)) {
	case SYSTASK_STMT:
	case DISABLE_STMT:
	case ASSIGN_STMT:
	case ASSIGN_DELAY_STMT:
	case ASSIGN_EVENT_STMT:
	case ASSIGN_NONBLK_DELAY_STMT:
	case ASSIGN_NONBLK_EVENT_STMT:
	case ASSIGN_CONT_STMT:
	case ASSIGN_PROC_STMT:
	case FORCE_STMT:
	case NET_VECTOR_DECL:
	case NET_SCALAR_DECL:
	case JOIN_STMT:
	case JOIN_NAMED_STMT:
	    if (skip) {
		trace_stmt(current_pc, g, skip, val);
		return next_pc;
	    }
	    break;
	default:
	    trace_stmt(current_pc, g, skip, val);
	}
    }

    if (!single_step_flag && !break_flag && !in_disable) {
	/* Don't go interactive */
	return next_pc;
    }

    /* Certain statements should stop after, not before */
    if (current_pc) {
	switch (TREE_CODE(current_pc)) {
	case SYSTASK_STMT:
	case DISABLE_STMT:
	case ASSIGN_STMT:
	case ASSIGN_DELAY_STMT:
	case ASSIGN_EVENT_STMT:
	case ASSIGN_NONBLK_DELAY_STMT:
	case ASSIGN_NONBLK_EVENT_STMT:
	case ASSIGN_CONT_STMT:
	case ASSIGN_PROC_STMT:
	case FORCE_STMT:
	case NET_VECTOR_DECL:
	case NET_SCALAR_DECL:
	    if (skip) {
		return next_pc;
	    }
	    break;
	}


	switch (TREE_CODE(next_pc)) {
	case FOR_STMT:
	    if (STMT_SURROGATE_ATTR(next_pc) || val) {
		return next_pc;
	    }
	    break;
	case END_STMT:
	case END_NAMED_STMT:
	case JOIN_STMT:
	case JOIN_NAMED_STMT:
	case REPEAT_STMT:
	case TASK_BLOCK:
	case FUNCTION_BLOCK:
	    return next_pc;
	case ALWAYS_BLOCK:
	    if (INITIALIZED_ATTR(next_pc)) {
		return next_pc;
	    }
	}
    }

    if (break_flag) {		/* we got here on a break, now exit */
	printf_V("Interrupt at time ");
	print_time(&CurrentTime);
	printf_V("\n");
	fin = File::Stdin();
    }

    if (in_disable) {
	readylist->pc = next_pc;
	return dummy_return;
    }

    break_flag = 0;
    normal_flag = !trace_flag;
    single_step_flag = 0;
    single_trace_flag = 0;

    cmd_level = new_cmd_level;

    for (;;) {
	int endOfInput = 0;
	clk_pause = clock();	/* start counting time away from simulation */
	if (prompt_flag) {
	    flush_files();	/* make sure that log and waves file are flushed */
	    char prompt[256];
	    snprintf(prompt, sizeof(prompt), "C%i> ", cmd_level);
	    char *line;
	    if (fin == NULL || fin == File::Stdin()) {
		line = readline(prompt);
#ifdef HAVE_READLINE_HISTORY
		if (line && line[0]) {
		    add_history(line);
		}
#endif // HAVE_READLINE_HISTORY
	    } else {
		char buffer[1024];
		if (fin->fgets(buffer, sizeof(buffer))) {
		    if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = 0;
		    } else {
			printf_error_V("lines in input file must be"
				       " less than %d characters\n",
				       sizeof(buffer));
		    }
		    line = strdup(buffer);
		} else {
		    line = 0;
		}
	    }
	    if (line) {
		printf_V_log("%s", prompt);
		printf_V_key("%s\n", line);
		push_stream(fin, 2);
		fin = new File(line, strlen(line), 1);
	    } else {
		endOfInput = 1;
	    }
	}

	/* if there are no characters in the buffer, this is where 
	   we'll wait for user input.  Some compilers add the 
	   '\n' at the end of the line.  We'll watch for that 
	   and strip it out. */

	if (endOfInput) {
	    c = EOF;
	} else {
	    do {
		c = fin->fgetc();
	    }
	    while (c == ' ' || c == '\t');
	}

	clock_pause += clock() - clk_pause;

	/* get here either at end of macro (if a macro was 
	   entered at the command line), or end of input 
	   file (-i or $input). */

	if (c == EOF) {
	    File *savefin = fin;
	    fin = pop_stream();	/* This was either an input file or macro */
	    if (!fin) {		/* if Ctl-D is pressed under Unix, EOF is */
		print_info();	/*  is returned from getc so fin is NULL */
		finish();
	    }
	    prompt_flag = 1;
	    continue;
	}

	/* Normal case: print what was in the I/O buffer to the log file */

	if (c == '\n') {
	    prompt_flag = 1;
	    continue;
	} else {
	    prompt_flag = 0;
	}

	switch (c) {
	case ',':
	    single_trace_flag = 1;

	case ';':
	    normal_flag = 0;
	    single_step_flag = 1;

	case '.':
	    is_interactive = 0;
	    return next_pc;

	case ':':
	    printf_V("Current position: line %d\n",
		     STMT_SOURCE_LINE(current_pc));
	    break;

	case '-':
	    break;

	case ' ':
	    break;

	    /* ^D: exit immediately */
	case (4):
	    print_info();
	    finish();

	    /* Command history */
	case ('0'):
	case ('1'):
	case ('2'):
	case ('3'):
	case ('4'):
	case ('5'):
	case ('6'):
	case ('7'):
	case ('8'):
	case ('9'):
	    exec_history();
	    break;

	case '!':
	    {
		char cmd[128];
		int i;

		fin->fgets(cmd, 128);
		if (cmd[0] == '\n') {
		    strcpy(cmd, getenv("COMSPEC"));
		    if (cmd == NULL) {
			strcpy(cmd, "command");
		    }
		    printf("Type EXIT to return to VeriWell...\n");
		}
		i = system(cmd);
		if (i) {
		    perror("Command error\n");
		}
	    }
	    prompt_flag = 1;
	    break;

	default:
	    fin->fungetc(c);
	    is_interactive = 1;
	    reinit_lex();
	    tmp_error = errorcount;
	    fail = prog_parse();
	    if (!fail && tmp_error == errorcount) {
		extern struct Marker_info marker_info;
		marker_info.current_scb->pc = interactive_statement;
		ASSIGNTIME64(&(marker_info.current_scb->time),
			     &CurrentTime);
		tmp_error = errorcount;
		new_cmd_level = cmd_level + 1;
		pass3_interactive((tree) interactive_statement);
		if (tmp_error == errorcount) {
		    stack_allocate();
		    interject_readylist(marker_info.current_scb, next_pc);
		    /* for next time */
		    marker_info.current_scb = BuildSCB(NULL_TREE, NOLIST);
		    single_step_flag = 1;
		    normal_flag = 0;
		    return interactive_statement;
		}
	    }
	    prompt_flag = 1;
	}
    }
}

/***********************************************************************
 *
 *      handle_gate
 *              - gate is ready to execute 
 *
 ***********************************************************************
 */
void handle_gate(tree gate)
{
    tree pc;			/* dummy for TRACE macro */

    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);


    if (GATE_PATH_OUTPUT(gate) != NULL_TREE) {
	if (handle_specify_path(gate)) {
	    return;
	}
    }
    TRACE(gate, gate, 0, 0, 0);
    (*GATE_PROPAGATE_ROUTINE(gate)) (gate);
}

void print_prompt()
{
    printf_V("> ");
}

void init_interactive()
{
    extern struct Marker_info marker_info;
    marker_info.current_scb = BuildSCB(NULL_TREE, NOLIST);
}

/*****************************************************************
 *
 *  init_exec
 *	- initialize all global and static variables for exec.c
 *
 *****************************************************************
 */

void init_exec()
{
    exit_flag = 0;
    brk_check = 0;
    first_time_exec = 1;
    in_initial = 0;
    new_cmd_level = 1;
    prompt_flag = 1;
    save_char = 0;
    in_disable = 0;
    free_context = NULL;
}
