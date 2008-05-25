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

/* SCHED.C - Scheduler and queues for simulation engine */

#define SCHED_C

#include <stdio.h>
#include <ctype.h>
#include "vtypes.h"
#include "tree.h"
#include "schedule.h"
#include "flags.h"
#include "runtime.h"
#include "dumpvar.h"
#include "strobe.h"
#include "acc_user.h"
#include "gates.h"
#include "eval.h"
#include "glue.h"
#include "exec.h"
#include "nsched.h"
#include "veriwell.h"
#include "pli.h"
#include "systask.h"

SCB *eventlist = NULL;		/* List of SCBs waiting for an event */
SCB *timelist = NULL;		/* List of SCBs waiting for time to pass */
SCB *timelist_currentTail = NULL;	/* last entry for current time */
SCB *netlist = NULL;		/* List of SCBs containing pending net changes */
SCB *netlist_last = NULL;	/* Points to last SCB on net list */
SCB *readylist = NULL;		/* List of SCBs previously waiting for an event */
SCB *readylist_last = NULL;	/* Points to last SCB on ready list */
SCB *freelist = NULL;		/* List of available SCBs (eg. fork/join) */
SCB *retrigger = NULL;          /* set if scb was retrigger while active */

tree deferred_markers = NULL_TREE;

time64 CurrentTime;

static int BLK_SIZE = 1024;

SCB *BuildSCB(union tree_node *start, enum which_list list)
{
    SCB *scb, *tmp, *s;
    int i;
    extern tree current_scope;

    if (!freelist) {
	SCB *scbArray = (SCB *) xmalloc(sizeof(SCB) * BLK_SIZE);
	for (i = 0; i < BLK_SIZE; i++) {
	    ADD_LIST_SCB(FREE_LIST, &freelist, scbArray + i);
	}
    }
    scb = freelist;
    REMOVE_LIST_SCB(scb);

    scb->pc = start;
//  scb->old_pc = NULL_TREE;
    ASSIGNTIME64(&(scb->time), &CurrentTime);
//  scb->type = NULL;
    scb->prev = NULL;
    scb->next = NULL;
//  scb->drive[0] = scb->drive[1] = 0;
//  scb->rising = scb->falling = scb->turnoff = 0;
//  scb->tf_stack = NULL;
    scb->fork_count = 0;
    scb->fork = NULL;
//  scb->thread = NULL;
    scb->here.scb = NULL;
    scb->mode = RUN;
    scb->context = NULL;
    enter_context(scb, current_scope, NULL_TREE);
//  scb->list = TIMELIST;

    /* Each block points to all SCBs in scope (for DISABLE statement) */
//  if (current_scope)
//    BLOCK_SCB_LIST (current_scope) = tree_cons ((tree)scb, NULL_TREE, BLOCK_SCB_LIST (current_scope));
//  scb->current_scope = current_scope;

    switch (list) {
    case TIME_LIST:
	if (!timelist) {
	    ADD_LIST_SCB(TIME_LIST, &timelist, scb);
	} else {
	    /* put new entries at end of list */
	    for (s = timelist; s->next; s = s->next);
	    ADD_LIST_SCB(TIME_LIST, &s->next, scb);
	}
	break;
    case READY_LIST:
	if (!readylist) {
	    ADD_LIST_SCB(READY_LIST, &readylist, scb);
	} else {
	    /* put new entries at end of list */
	    for (s = readylist; s->next; s = s->next);
	    ADD_LIST_SCB(READY_LIST, &s->next, scb);
	}
	break;
    default:
	ADD_LIST_SCB(NOLIST, &eventlist, scb);
    }

//  PrintSCB(scb);
    return scb;
}

void PrintSCB(SCB * scb)
{
    printf("building SCB at %p\n", scb);
//  printf("  scb->type: %d\n",scb->type);
//  printf("  scb->new_pc: %p\n",scb->new_pc);
//  printf("  scb->old_pc: %p\n",scb->old_pc);
//  printf("  scb->time: %d\n",scb->time);
    printf("  scb->prev: %p\n", scb->prev);
//  printf("  scb->drive[0]: %d\n",scb->drive[0]);
//  printf("  scb->drive[1]: %d\n",scb->drive[1]);
//  printf("  scb->rising: %d\n",scb->rising);
//  printf("  scb->falling: %d\n",scb->falling);
//  printf("  scb->turnoff: %d\n",scb->turnoff);
    printf("  scb->list: %p\n", scb->list);
}

void BuildMarker(tree decl, struct Marker_info *marker_info)
{
    Marker *marker;

    /* First, ensure that only one marker per variable is used e.g. a = b + b; */
    for (marker = marker_info->first; marker; marker = marker->link)
	if (marker->decl == decl)
	    return;

    marker = (Marker *) xmalloc(sizeof(Marker));
    marker->next = NULL;
    marker->prev = NULL;
    marker->scb = marker_info->current_scb;
    marker->expr.expr = NULL;
    marker->decl = decl;
    marker->flags = marker_info->flags;
    marker->delay = marker_info->delay;
    marker->link = NULL;

    if (marker_info->last)
	marker_info->last->link = marker;	/* Link this to end of the list */
    else
	marker_info->first = marker;

    marker_info->last = marker;

    if ((marker->flags & M_FIXED))
	WaitOnEvent(marker, marker_info->current_scb);
    return;
}

tree WaitOnTime(delay_t waittime, tree pc)
{
    SCB *scb = readylist;

    /* 
       Find the right spot in the timelist either at the end (index == NULL)
       or when the next SCBs time field is bigger (ie we would come off
       the wait list sooner).
     */

    scb->pc = pc;		/* save next instruction to be executed */
//  scb->new_pc = next_pc;  /* save next instruction to be executed */
//  scb->old_pc = this_pc;  /* save this instruction for tracing */

    ADDTIME6432(&(scb->time), &CurrentTime, waittime);
//  scb->list = TIME_LIST;
    scb->mode = WAIT;

    scb = dispatcher(TIME_LIST);
//  *prev_pc = scb->old_pc; /* return the pc that got us here */
//  return scb->new_pc;     /* as wll as the new pc */
    return scb->pc;		/* as wll as the new pc */
}

tree WaitOnTime64(struct Time64 * waittime, tree pc)
{
    SCB *scb = readylist;

    /* 
       Find the right spot in the timelist either at the end (index == NULL)
       or when the next SCBs time field is bigger (ie we would come off
       the wait list sooner).
     */

    scb->pc = pc;		/* save next instruction to be executed */
//  scb->new_pc = next_pc;  /* save next instruction to be executed */
//  scb->old_pc = this_pc;  /* save this instruction for tracing */

    ADDTIME6464(&(scb->time), &CurrentTime, waittime);
//  scb->list = TIME_LIST;
    scb->mode = WAIT;

    scb = dispatcher(TIME_LIST);
//  *prev_pc = scb->old_pc; /* return the pc that got us here */
//  return scb->new_pc;     /* as wll as the new pc */
    return scb->pc;		/* as wll as the new pc */
}

void Schedule64(struct Time64 *time, SCB * scb, int prepend)
{
    SCB *link, *tmp;

    /*
       Find the right spot in the timelist either at the end (index == NULL)
       or when the next SCBs time field is bigger (ie we would come off
       the wait list sooner).
     */


    scb->time = *time;
    scb->mode = WAIT;

    REMOVE_LIST_SCB(scb);
//check_readylist();
/* Caution: If the next 5 lines are changes, also change Dispatcher! */
    if (!timelist || (CMPTIME64(&(scb->time), &(timelist->time)) < 0)) {
	ADD_LIST_SCB(TIME_LIST, &timelist, scb);	/* empty waitlist; add us */
    } else if (!prepend) {	/* else, find position in list (beg. of time) */
	for (link = timelist;
	     link->next
	     && (CMPTIME64(&(scb->time), &(link->next->time)) >= 0);
	     link = link->next);
	ADD_LIST_SCB(TIME_LIST, &link->next, scb);
    } else {
	for (link = timelist;
	     link->next
	     && (CMPTIME64(&(scb->time), &(link->next->time)) > 0);
	     link = link->next);
	ADD_LIST_SCB(TIME_LIST, &link->next, scb);
    }

}

void ScheduleDelta64(struct Time64 *time, SCB * scb, int prepend)
{
    SCB *link, *tmp;

    /*
       Find the right spot in the timelist either at the end (index == NULL)
       or when the next SCBs time field is bigger (ie we would come off
       the wait list sooner).
     */


    ADDTIME6464(&(scb->time), &CurrentTime, time);
    scb->mode = WAIT;

    REMOVE_LIST_SCB(scb);
//check_readylist();
/* Caution: If the next 5 lines are changes, also change Dispatcher! */
    if (!timelist || (CMPTIME64(&(scb->time), &(timelist->time)) < 0)) {
	ADD_LIST_SCB(TIME_LIST, &timelist, scb);	/* empty waitlist; add us */
    } else if (!prepend
	       && CMPTIME64(&timelist_currentTail->time, &scb->time) == 0)
    {
	ADD_LIST_SCB(TIME_LIST, &timelist_currentTail->next, scb);
    } else if (!prepend) {	/* else, find position in list (beg. of time) */
	for (link = timelist;
	     link->next
	     && (CMPTIME64(&(scb->time), &(link->next->time)) >= 0);
	     link = link->next);
	ADD_LIST_SCB(TIME_LIST, &link->next, scb);
    } else {
	for (link = timelist;
	     link->next
	     && (CMPTIME64(&(scb->time), &(link->next->time)) > 0);
	     link = link->next);
	ADD_LIST_SCB(TIME_LIST, &link->next, scb);
    }

//  *prev_pc = scb->old_pc; /* return the pc that got us here */
//  return scb->new_pc;     /* as wll as the new pc */
}

void Schedule(delay_t waittime, SCB * scb, int prepend)
{
    SCB *link, *tmp;

    /*
       Find the right spot in the timelist either at the end (index == NULL)
       or when the next SCBs time field is bigger (ie we would come off
       the wait list sooner).
     */


    ADDTIME6432(&(scb->time), &CurrentTime, waittime);
    scb->mode = WAIT;

    REMOVE_LIST_SCB(scb);
//check_readylist();
/* Caution: If the next 5 lines are changes, also change Dispatcher! */
    if (!timelist || (CMPTIME64(&(scb->time), &(timelist->time)) < 0)) {
	ADD_LIST_SCB(TIME_LIST, &timelist, scb);	/* empty waitlist; add us */
    } else if (!prepend
	       && CMPTIME64(&timelist_currentTail->time, &scb->time) == 0)
    {
	ADD_LIST_SCB(TIME_LIST, &timelist_currentTail->next, scb);
    } else if (!prepend) {	/* else, find position in list (beg. of time) */
	for (link = timelist;
	     link->next
	     && (CMPTIME64(&(scb->time), &(link->next->time)) >= 0);
	     link = link->next);
	ADD_LIST_SCB(TIME_LIST, &link->next, scb);
    } else {
	for (link = timelist;
	     link->next
	     && (CMPTIME64(&(scb->time), &(link->next->time)) > 0);
	     link = link->next);
	ADD_LIST_SCB(TIME_LIST, &link->next, scb);
    }

//  *prev_pc = scb->old_pc; /* return the pc that got us here */
//  return scb->new_pc;     /* as wll as the new pc */
}

/* Schedule nets */

void ScheduleNet(Marker * marker, enum logical_value state)
{
    SCB *scb = marker->scb, *tmp;

/* If a port has just been changed, propigate immediately (also during
   continuous assignment initialization) */

    if (marker->flags & M_PORT /*|| NET_ASSIGN_ATTR (marker->decl) */ )	// || in_initial)
//  if (marker->flags & M_PORT || in_initial)
	net_exec(scb->pc);
    else {
	delay_t delay = eval_delay(NET_DELAY(scb->pc), state);

/* If there is no delay, schedule on net list, else put on time list.
   Note that in the case of inouts, we could be working with our own
   net in which case, we're finished. */

	if (!delay) {
            if (scb == readylist) {
                retrigger = scb;
            } else if (scb->list != NET_LIST) {
		REMOVE_LIST_SCB(scb);
//  if (!readylist)
//    fatal ("Caught it in schedulenet1");
//check_readylist();
		if (netlist) {
		    ADD_LIST_SCB(NET_LIST, &netlist_last->next, scb);
		} else {
		    ADD_LIST_SCB(NET_LIST, &netlist, scb);
		}
		netlist_last = scb;
		scb->time = CurrentTime;
	    }
	} else {
	    Schedule(delay, scb, 0);
//  if (!readylist)
//    fatal ("Caught it in schedulenet2");
	}
    }
}

void thread_marker(Marker * marker)
{
    Marker *mark, *tmp;
    tree decl = marker->decl;

    if (!DECL_EVENT_CHAIN(decl)) {
	ADD_LIST(DECL_EVENT_CHAIN(decl), marker);
    }
/* Add marker to the tail of the decl's event chain */
    else {
	for (mark = DECL_EVENT_CHAIN(decl); mark->next; mark = mark->next);
	;
	ADD_LIST(mark->next, marker);
    }
}

/* Start with a marker and thread on all markers on the link chain */

void thread_all_markers(Marker * marker)
{
    Marker *mark;

    for (mark = marker; mark->link != marker; mark = mark->link)
	thread_marker(mark);
    thread_marker(mark);
}

/* Thread marker onto an event chain, headed by a decl node */

void WaitOnEvent(Marker * marker, SCB * scb)
{
//   EventQueue *queue;
//   tree decl;
//   Marker *link, *tmp;

//   decl = marker->decl;

/* we could already be on the event chain */
//   for (link = queue->first; link; link=link->next)
//     if (link == marker) return;
    if (marker->prev) {
	ASSERT(scb != 0);

	/* In a task, it is possible that more than one caller can be waiting on
	   the same event.  If so, add the SCB to the list. */
	if (marker->scb != scb) {
	    scb->here.scb = marker->scb;
	    marker->scb = scb;
	}
	return;
    }

    marker->scb = scb;
//   marker->next = marker->prev = NULL;
    thread_marker(marker);
/*
   if (debug) {
     printf ("Waiting On Event Queue at: %p\n",
	      DECL_EVENT_CHAIN (decl));
     for (marker = DECL_EVENT_CHAIN (decl); marker; marker = marker->next)
printf ("Marker[%p]: scb: %p prev: %p next: %p link: %p expr: %p scb: %p\n",
    marker,marker->scb,marker->prev,marker->next,marker->link,marker->expr,
marker->scb);
   }
*/
}

tree WaitOnEventAll(Marker * marker, tree pc)
{
    Marker *mark;
    SCB *scb;

    readylist->pc = pc;
//  readylist->old_pc = this_pc;
    readylist->mode = WAIT;

    // pathological case: wait (0); -- wait forever, no markers, skip past Waits
    if (marker)
	 {
	for (mark = marker; mark->link != marker; mark = mark->link)
	    WaitOnEvent(mark, readylist);
	WaitOnEvent(mark, readylist);
	}
    scb = dispatcher(EVENT_LIST);
//  *prev_pc = scb->old_pc; /* return the pc that got us here */
    return scb->pc;
}

void WaitOnEventOther(SCB * scb, Marker * marker, tree pc)
{
    Marker *mark;
    SCB *tmp;

//  scb->new_pc = next_pc;
//  scb->old_pc = this_pc;
    scb->pc = pc;
    scb->mode = WAIT;

    for (mark = marker; mark->link != marker; mark = mark->link)
	WaitOnEvent(mark, scb);
    WaitOnEvent(mark, scb);

    REMOVE_LIST_SCB(scb);
    ADD_LIST_SCB(EVENT_LIST, &eventlist, scb);
    return;
}

/* Notify everything on the deferred list (used for lval concats) */

void NotifyDeferred(tree deferred_markers)
{
    tree tmp;

    while (deferred_markers) {
	NotifyEvent((Marker *) TREE_PURPOSE(deferred_markers),
		    (enum logical_value) (unsigned long)
		    TREE_VALUE(deferred_markers), (int) 0);

	tmp = deferred_markers;
	deferred_markers = TREE_CHAIN(deferred_markers);
	free_link_list(tmp);
    }
}

/* Place marker's SCB onto ready list; unthread event chain if not fixed */

void NotifyEvent(Marker * marker, enum logical_value state, int deferred)
{
    Marker *ring, *NextEntry;
    SCB *scb, *tmp, *next_scb;

    if (!marker)
	return;

    if (deferred) {
	deferred_markers = link_tree_cons((tree) marker, (tree) state,
					  deferred_markers);
	return;
    }

    do {
	NextEntry = marker->next;
	scb = marker->scb;

	/* If this is a primitive (built-in or UDP), call the processing routine */

	if (marker->flags & M_PRIM) {
	    ASSERT(GATE_ROUTINE((tree) marker->scb));
	    (*GATE_ROUTINE((tree) marker->scb)) (marker);
	}

	/* If this is a continuous assignment marker, do it now */


	else if (marker->delay) {	/* if cont assignment, put at beginning */
	    if (in_initial)
		do_net_assignment(scb->pc);
	    else
		Schedule(eval_delay(marker->delay, state), scb,
			 marker->flags & M_CONT);
	} else if ((marker->flags & M_CONT)) {
	    do_net_assignment(marker->expr.tree, (marker->flags & M_FORCE));
	}

	/* If there is a ref to a net (from a cont assignment), schedule it
	   or do it now if we are in initialization or if we are a port. */

	else if (marker->flags & M_NET) {
	    ScheduleNet(marker, state);
	}

	else if (marker->flags & M_VCD)
	    dumpvars_dump(marker->decl, marker->expr.vcd_id);

	else if (marker->flags & M_IDLE)
	    tickle_monitor(&idle_strobe_queue, marker->expr.tree);	/* in systask.c; points to mon info */

	else if (marker->flags & M_STROBE)	/* Special for $monitor */
	    tickle_monitor_old(&final_strobe_queue, marker->expr.mon);	/* in systask.c; points to mon info */

	else if (marker->flags & M_VCL)
	    vcl_dispatch((struct pli_vcl *) marker->expr.vcl);

	else if (marker->flags & M_ASYNCH) {
	    pli_asynccall((PliInfo_t *) marker->expr.tree);
	}

/* If marker says test the expression (for edge triggered), then do
   so.  But, we may already be on the ready list, so we don't need to
   do the threading of the SCB.  */

	else if (scb->list != READY_LIST &&
		 (!(marker->flags & M_TEST) || test(marker->expr.expr))) {
	    for (; scb; scb = next_scb) {
		SCB *last;
		next_scb = scb->here.scb;
		scb->here.scb = NULL;

		REMOVE_LIST_SCB(scb);
//check_readylist();

#if VDEBUG != 0
		if (debug)
		    printf
			("Adding Marker to Ready List[%p]: %p next: %p prev: %p\n",
			 marker, scb, scb->next, scb->prev);

		if (vdebug)
		    printf("*** Notifying line: %lu\n",
			   STMT_SOURCE_LINE(scb->pc));
#endif

		/* Thread to the end of the ready list (don't let prev point
		   to readylist->last) */
		last = LAST_SCB(READY_LIST);
		if (last) {
		    ADD_LIST_SCB(READY_LIST, &last->next, scb);
		} else {
		    ADD_LIST_SCB(READY_LIST, &readylist, scb);
		}
	    }

//check_readylist ();
/* Now, unthread our marker and all the other markers associated with
   this event, if the fixed bit is not set, of course.  Note that the
   markers are threaded by the links in a circular list.  We know to
   stop when we hit a marker that has its prev pointer set to NULL. */

	    if (!(marker->flags & M_FIXED))
		for (ring = marker; ring->prev; ring = ring->link) {
		    REMOVE_LIST(ring);
		    ring->prev = NULL;	/* This tells us that the marker is not threaded */
		}
	}

	/* else if... */
	marker = NextEntry;
    }
    while (marker);
}

/* Disconnect the markers associate with this event or wait stmt; usually
     called from disable.  Watch for cases where marker was already notified.
*/

void event_undo(Marker * first)
{
    Marker *mark;

    if (!first)
	return;

    for (mark = first; mark->link != NULL && mark->link != first;
	 mark = mark->link) {
	if (mark->prev) {	/* if null, marker was already notified */
	    REMOVE_LIST(mark);
	    mark->prev = NULL;
	}

    }				/* handle the last one */
    if (mark->prev) {
	REMOVE_LIST(mark);
	mark->prev = NULL;
    }
}

/* Get the next process on the ready list, return a pointer to an
   executable tree; place the incoming SCB onto the wait list (possibly
   ordered by time). */

SCB *dispatcher(enum which_list remove_list)
{
    SCB *link, *scb = readylist, *tmp;
    time64 new_time;
//  int in_net;

//  in_net = 1;
//  if (!readylist)
//    goto go;

//  if (TREE_CODE (readylist -> pc) == NET_VECTOR_DECL ||
//          TREE_CODE (readylist -> pc) == NET_VECTOR_DECL)
//    in_net = 1;
//  else
//    in_net = 0;

/* If RemoveFlag is set, take scb off the list.  This happens at the end
   of INITIAL blocks. */

    switch (remove_list) {

    case NOLIST:
	REMOVE_LIST_SCB(scb);
	ADD_LIST_SCB(NOLIST, &eventlist, scb);
	break;

    case EVENT_LIST:
	REMOVE_LIST_SCB(scb);
	ADD_LIST_SCB(EVENT_LIST, &eventlist, scb);
	break;

    case TIME_LIST:
	REMOVE_LIST_SCB(scb);
	if (!timelist || (CMPTIME64(&(scb->time), &(timelist->time)) < 0)) {
	    ADD_LIST_SCB(TIME_LIST, &timelist, scb);	/* empty waitlist; add us */
	} else {		/* else, find position in list */
/* WARNING: this must be the same code as in Schedule!! */
	    for (link = timelist;
		 link->next
		 && (CMPTIME64(&(scb->time), &(link->next->time)) >= 0);
		 link = link->next);
	    ADD_LIST_SCB(TIME_LIST, &link->next, scb);
	}
//if (CMPTIME64 (&(scb->time), &CurrentTime) < 0)
//  printf ("Time less than current time\n");
	break;

    case FREE_LIST:
	REMOVE_LIST_SCB(scb);
	ADD_LIST_SCB(FREE_LIST, &freelist, scb);
	if (scb->context)
	    unwind_context(scb->context);
	break;
    }

//printf ("In dispatch: just before checking readylist (%p)\n", readylist);


/* 
  Ready List Check: if we have something on the Ready list - go with it
  */
  go:
//  if (readylist) {
//    if (debug) {
//      printf ("Current Ready List: [%p]\n",
//        readylist);
//      for (link = readylist,cnt = 0; link; link = link->next)
//  printf ("ENTRY[%d]: %p prev: %p next: %p\n",
//      cnt++,link,link->prev,link->next);
//    }

//    if (debug) PrintSCB(readylist);

//  }

    if (readylist)
	return readylist;

//DEBUGPRINT ("Dispatcher: nothing in readylist\n");
/* Nothing in the ready list, see if any gates need to
   be processed. */

    {
	tree gate;

	while (TRUE) {
//          gate = PeekGate();
//          if( gate == NULL_TREE ) {
//              break;
//          }
//          ASSERT( CMPTIME64 (&(GATE_TIME (gate)), &CurrentTime) >= 0);
//          if( CMPTIME64 (&(GATE_TIME (gate)), &CurrentTime) == 0 ) {
//
//              DEBUGPRINT ("Inside gatelist\n");
//              RemoveGate( gate );
//                  handle_gate (gate);
//          } else {
//              break;
//          }
	    gate = RemoveNextReadyGate();
	    if (gate == NULL_TREE) {
		break;
	    }
	    DEBUGPRINT("Inside gatelist\n");
	    handle_gate(gate);
	}
    }

    if (readylist)
	return readylist;
    if (debug)
	printf("This is a dummy stmt\n");

//DEBUGPRINT ("Dispatcher: (past gates) nothing in readylist\n");

/* See if there is a net change pending */

    if (netlist)		// && (!readylist ||
//          TREE_CODE (readylist -> pc) != NET_VECTOR_DECL &&
//          TREE_CODE (readylist -> pc) != NET_SCALAR_DECL))
    {
	do {
	    scb = netlist;
//DEBUGPRINT ("inside netlist, scb is %p, readylist %p, last %p\n", scb, readylist, readylist_last);
	    if (debug)
		printf
		    ("inside netlist, scb %p, prev %p, next %p, readylist %p, last %p, netlist %p, last %p, (at %p)\n",
		     scb, scb->prev, scb->next, readylist, readylist_last,
		     netlist, netlist_last, &netlist);
	    REMOVE_LIST_SCB(scb);
	    if (debug)
		printf
		    ("inside netlist2, scb is %p, readylist %p, last %p, netlist %p, last %p\n",
		     scb, readylist, readylist_last, netlist,
		     netlist_last);
	    if (!netlist)
		netlist_last = NULL;
	    if (!readylist) {
		ADD_LIST_SCB(READY_LIST, &readylist, scb);
	    } else {
		SCB *last = LAST_SCB(READY_LIST);
		ADD_LIST_SCB(READY_LIST, &last->next, scb);
	    }
	}
	while (netlist);
    }
//DEBUGPRINT ("Dispatcher: (past nets) nothing in readylist\n");
    if (debug)
	printf("This is a dummy stmt\n");
    if (readylist)
	return readylist;

    if (debug)
	printf("This is a dummy stmt\n");

/* Nothing on the ready list; check the time list for SCBs that
   are waiting for time to pass. */

    if (timelist) {

///*
	if (debug) {
	    printf("Current Time List:\n");
//      for (link = timelist, cnt = 0; link ; link = link->next)
//        printf ("ENTRY[%d]: %p prev: %p next: %p pc: %p time: %d\n",
//      cnt++,link,link->prev,link->next,link->new_pc,link->vtime);
	}
//*/


/* ------------ added this here mdh */
	while (timelist
	       && (CMPTIME64(&(timelist->time), &CurrentTime) == 0)) {
	    scb = timelist;
	    REMOVE_LIST_SCB(scb);
	    if (!readylist) {
		ADD_LIST_SCB(READY_LIST, &readylist, scb);
	    } else {
		SCB *last = LAST_SCB(READY_LIST);
		ADD_LIST_SCB(READY_LIST, &last->next, scb);
	    }
	}

	if (readylist)
	    return (readylist);

//DEBUGPRINT ("Dispatcher: (past time2) nothing in readylist\n");

	if (netlist) {
	    goto go;
	}

	if (IsGateReady()) {
	    goto go;
	}
/* Before moving stuff on the readylist, check for things that want to happen
   at the end of the current time unit. */

	check_strobe(&idle_strobe_queue);
//DEBUGPRINT ("Dispatcher: (past time1) checking strobe\n");
	if (debug)
	    printf("This is a dummy stmt\n");
	if (readylist)		/* check_strobe may have indirectly caused new events */
	    return readylist;
	if (debug)
	    printf("This is a dummy stmt\n");

//DEBUGPRINT ("Dispatcher: (past time1) nothing in readylist\n");

/* ------------ added this here mdh */
	while (timelist
	       && (CMPTIME64(&(timelist->time), &CurrentTime) == 0)) {
	    scb = timelist;
	    REMOVE_LIST_SCB(scb);
	    if (!readylist) {
		ADD_LIST_SCB(READY_LIST, &readylist, scb);
	    } else {
		SCB *last = LAST_SCB(READY_LIST);
		ADD_LIST_SCB(READY_LIST, &last->next, scb);
	    }
	}

	if (readylist)
	    return (readylist);

//DEBUGPRINT ("Dispatcher: (past time2) nothing in readylist\n");

	if (netlist) {
	    goto go;
	}

	if (IsGateReady()) {
	    goto go;
	}
	if (!empty_strobe(&idle_strobe_queue)) {
	    goto go;
	}

	check_strobe(&final_strobe_queue);
	ASSERT(readylist == NULL);

	if (GateConditionalAdvanceTime(&(timelist->time))) {
	    goto go;
	}

	ASSIGNTIME64(&new_time, &(timelist->time));
	do {
	    scb = timelist;
	    REMOVE_LIST_SCB(scb);
	    if (!readylist) {
		ADD_LIST_SCB(READY_LIST, &readylist, scb);
	    } else {
		SCB *last = LAST_SCB(READY_LIST);
		ADD_LIST_SCB(READY_LIST, &last->next, scb);
	    }
	}
	while (timelist && (CMPTIME64(&(timelist->time), &new_time) == 0));

/* advance time, if necessary */
//    if (time != readylist -> time) {
//      time = readylist -> time;
//      if (trace_flag || single_trace_flag)
//  printf ("SIMULATION TIME IS %lu\n", time);
//    }
    }
/* Nothing on Ready list or Time list, exit. */
    else {
	extern SCB *finish_scb;
	check_strobe(&idle_strobe_queue);
	/*
	 * there is a chance that pli code has added some 
	 * events. If so continue on.
	 */
	if (timelist != NULL || PeekGate() != NULL) {
	    goto go;
	}
	if (!empty_strobe(&idle_strobe_queue)) {
	    goto go;
	}
	check_strobe(&final_strobe_queue);
	return finish_scb;
//    return null;
//      printf ("All done\n");  /* Later, print useful information */
//    exit(0);
    }
//printf ("in dispatch: about to return\n");
    return readylist;
}

tree dispatch_pc(enum which_list list)
{
    return dispatcher(list)->pc;
}

/* Note that neither dump_current_process, nor dispatch, must change the
   current value of the evaluation stack. */

tree dump_current_process(void)
{
    SCB *scb = dispatcher(FREE_LIST);
//  *prev_pc = scb->old_pc;
    return scb->pc;
}

tree suspend_current_process(void)
{
    SCB *scb = dispatcher(NOLIST);
//  *prev_pc = scb->old_pc;
    return scb->pc;
}

void interject_readylist(SCB * scb, tree pc)
{
    SCB *tmp;			/* for ADD_LIST */

    /* */
    if (readylist) {
//    readylist -> old_pc = current_pc;
//    readylist -> new_pc = next_pc;
	readylist->pc = pc;
    }

    REMOVE_LIST_SCB(scb);
    ADD_LIST_SCB(READY_LIST, &readylist, scb);
}

void FreeSCB(SCB * scb)
{
    SCB *tmp;

    REMOVE_LIST_SCB(scb);
    ADD_LIST_SCB(READY_LIST, &freelist, scb);
    if (scb->context)
	unwind_context(scb->context);
}

#if 0
void cleanup_SCB(SCB * scb)
{
/* Clean up scb -- clear out markers and set time back to current time */
    if (scb->list == FREE_LIST)
	return;
    ASSIGNTIME64(&(scb->time), &CurrentTime);
    {
	Marker *first_marker;
	switch (TREE_CODE(scb->pc)) {
	case EVENT_STMT:
	    first_marker = STMT_EVENT_MARKER(scb->pc);
	    break;
	case WAIT_STMT:
	    first_marker = STMT_WAIT_MARKER(scb->pc);
	    break;
	case DELAY_STMT:
	case DISABLE_STMT:
	    first_marker = NULL;
	    break;
//      case FORK_STMT:
//      case FORK_NAMED_STMT:
	case JOIN_STMT:
	    /* Funny case: DISABLE is last line of a FORK branch; pc
	       will be pointing to JOIN_STMT.  This needs to be differentiated
	       from the case where the FORK stmt is being cleaned since new_pc
	       will also be pointing to JOIN */
//  if (scb->fork)
//    return;   /* inside FORK, let the FORK clean itself */
	    first_marker = NULL;
//        cleanup_fork (STMT_JOIN_FORK (scb->pc));
	    break;
	default:
	    runtime_error(scb->pc);
	    fatal
		("Attempting to disable stmt that is not WAIT, EVENT, DELAY, or DISABLE");
	}

	event_undo(first_marker);	/* disconnect the markers */
    }
}
#endif
#if 0
check_readylist()
{
    SCB *s;
    if (!readylist)
	return;
    for (s = readylist; s; s = s->next)
	if (!s->next)
	    if (s == readylist_last)
		return;
	    else
		fatal("REadylist_last dislodged");

    fatal("Lost readylist_last!");
}
#endif

SCB *last_readylist()
{
    SCB *s;

    for (s = readylist; s; s = s->next)
	if (!s->next)
	    return s;
    return NULL;
}

void ADD_LIST_SCB(enum which_list list, SCB ** POINT, SCB * NEW)
{
    SCB *tmp = *POINT;
#ifdef CHECKSCB
    SCB *ptr;
    if (timelist) {
	for (ptr = timelist; ptr; ptr = ptr->next) {
	    if (timelist_currentTail == ptr) {
		goto OK2;
	    }
	}
	ASSERT(FALSE);
    }
  OK2:
#endif
    switch (list) {
    case READY_LIST:
	if (*POINT == 0) {
	    readylist_last = NEW;
	}
	break;
    case TIME_LIST:
	if (timelist_currentTail == NULL) {
	    timelist_currentTail = NEW;
	} else if (CMPTIME64(&timelist_currentTail->time, &NEW->time) == 0
		   && tmp && CMPTIME64(&tmp->time, &NEW->time) != 0) {
	    timelist_currentTail = NEW;
	} else if (CMPTIME64(&timelist_currentTail->time, &NEW->time) >= 0) {
	    timelist_currentTail = NEW;
	}
	break;
    default:;
    }
    NEW->list = list;
    *POINT = NEW;
    (NEW)->prev = POINT;
    (NEW)->next = tmp;
    if (tmp)
	tmp->prev = &(NEW)->next;
#ifdef CHECKSCB
    switch (list) {
    case READY_LIST:
	for (ptr = readylist; ptr; ptr = ptr->next) {
	    if (NEW == ptr) {
		goto OK;
	    }
	}
	ASSERT(FALSE);
	break;
    case TIME_LIST:
	for (ptr = timelist; ptr; ptr = ptr->next) {
	    if (NEW == ptr) {
		goto OK;
	    }
	}
	ASSERT(FALSE);
	break;
    default:;
    }
  OK:
    if (timelist) {
	for (ptr = timelist; ptr; ptr = ptr->next) {
	    if (timelist_currentTail == ptr) {
		goto OK1;
	    }
	}
	ASSERT(FALSE);
    }
  OK1:
#endif
    return;
}

void REMOVE_LIST_SCB(SCB * PTR)
{
    SCB *ptr;
#ifdef CHECKSCB
    switch (PTR->list) {
    case READY_LIST:
	for (ptr = readylist; ptr; ptr = ptr->next) {
	    if (PTR == ptr) {
		goto OK;
	    }
	}
	ASSERT(FALSE);
	break;
    case TIME_LIST:
	for (ptr = timelist; ptr; ptr = ptr->next) {
	    if (PTR == ptr) {
		goto OK;
	    }
	}
	ASSERT(FALSE);
	break;
    default:;
    }
  OK:
#endif
    if ((*((PTR)->prev) = (PTR)->next) != 0)
	(PTR)->next->prev = (PTR)->prev;
    switch (PTR->list) {
    case READY_LIST:
	if (readylist_last == PTR) {
	    readylist_last = last_readylist();
	}
	break;
    case TIME_LIST:
	if (PTR == timelist_currentTail) {
	    timelist_currentTail = timelist;
	    for (; timelist_currentTail && timelist_currentTail->next &&
		 (CMPTIME64(&(timelist_currentTail->time),
			    &(timelist_currentTail->next->time)) == 0);
		 timelist_currentTail = timelist_currentTail->next);
	}
	break;
    default:;
    }
#ifdef CHECKSCB
    if (timelist) {
	for (ptr = timelist; ptr; ptr = ptr->next) {
	    if (timelist_currentTail == ptr) {
		goto OK1;
	    }
	}
	ASSERT(FALSE);
    }
  OK1:
#endif
}

SCB *LAST_SCB(enum which_list list)
{
    switch (list) {
    case READY_LIST:
//  return last_readylist();
	return readylist_last;
    default:
	ASSERT(FALSE);
	return NULL;
    }
}


/*****************************************************************
 *	
 *	init_sched
 *		- initialize global and static variables for sched.c
 *
 *****************************************************************
 */

void init_sched()
{
    eventlist = NULL;		/* List of SCBs waiting for an event */
    timelist = NULL;		/* List of SCBs waiting for time to pass */
    timelist_currentTail = NULL;	/* points to last entry of current time */
    netlist = NULL;		/* List of SCBs containing pending net changes */
    netlist_last = NULL;	/* Points to last SCB on net list */
    readylist = NULL;		/* List of SCBs previously waiting for an event */
    readylist_last = NULL;	/* Points to last SCB on ready list */
    freelist = NULL;		/* List of available SCBs (eg. fork/join) */
    deferred_markers = NULL_TREE;
    CurrentTime.timeh = 0;
    CurrentTime.timel = 0;
}
