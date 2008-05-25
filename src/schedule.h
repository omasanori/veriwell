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

/* SCHED.H - Declarations for scheduler and queues */

#ifndef SCHED_H
#define SCHED_H

// public methods

struct monitor_info;

#define ADD_LIST(POINT, NEW)			\
  {tmp = POINT;						\
  POINT = NEW;						\
  (NEW)->prev = &(POINT);				\
  (NEW)->next = tmp;					\
  if (tmp) tmp->prev = &(NEW)->next;}

#define REMOVE_LIST(PTR)			\
	{if ((*((PTR)->prev) = (PTR)->next) != 0)	\
	  (PTR)->next->prev = (PTR)->prev;}

//enum { SmallCharge, MediumCharge, LargeCharge };
enum which_list { NOLIST =
	1, READY_LIST, EVENT_LIST, TIME_LIST, NET_LIST, FREE_LIST };
enum marker_flags {
    M_FORCE = 0x1000,		/* used by self referential force */
    M_PRIM_FAST = 0x800,	/* Fast Primitive (no need to re-eval) */
    M_PRIM = 0x400,		/* Primitive marker */
    M_IDLE = 0x200,		/* Schedule for idle at end of simulation time */
    M_VCL = 0x100,		/* VCL (PLI) marker */
    M_VCD = 0x80,		/* VCD (dumpvars) marker */
    M_ASYNCH = 0x40,		/* PLI asynch marker */
    M_PORT = 0x20,		/* if port, evaluate immediately */
    M_STROBE = 0x10,		/* Schedule for end of current simulation time */
    M_NET = 0x8,		/* Evaluate NETs immediately */
    M_CONT = 0x4,		/* Evaluate continuous assignment immediately */
    M_FIXED = 0x2,		/* if set, don't unthread markers after event */
    M_TEST = 0x1,		/* if set, test event expression at notification */
    M_PLAIN = 0
};

extern time64 CurrentTime;
extern struct SCB *readylist;
extern struct SCB *readylist_last;
extern struct SCB *eventlist;
extern struct SCB *timelist;
extern struct SCB *timelist_currentTail;
extern struct SCB *netlist;
extern struct SCB *freelist;
extern struct SCB *retrigger;

enum eMode { RUN, WAIT };

typedef struct SCB {
    struct SCB *next;		/* Next SCB on this linked list */
    struct SCB **prev;		/* Previous SCB on linked list */
    time64 time;		/* For use in Delayed events */
    union tree_node *pc;	/* A pointer to executable code */
    enum eMode mode;		/* Used to determine if entering or exiting events */
//  union tree_node *new_pc;/* A pointer to executable code */
//  union tree_node *old_pc;/* Pointer to statement that caused event */
    enum which_list list;
    struct context_member *context;
//  struct tf_stack *tf_stack;
//  union tree_node *current_scope;
    struct SCB *fork;		/* If inside fork/join, points to "main" SCB */
    unsigned fork_count;	/* Count of number of SCBs still in fork/join */
//  struct SCB *thread; /* Currently used to link all SCBs inside a fork */
    int bit_offset;		/* used in delayed assignments */
    int group_offset;		/* used in delayed assignements */
    union {
	struct SCB *scb;	/* In case there are many SCBs waiting on the same event */
	struct context_member *fork_list;	/* If at fork, point to members */
	union tree_node *decl;	/* for nonblock assigns */
	Marker*  marker;	/* used for gate intialization */
    } here;			/* This is valid while waiting at a particular place */
} SCB;

void ADD_LIST_SCB(enum which_list list, SCB ** POINT, SCB * NEW);
void REMOVE_LIST_SCB(SCB * PTR);
SCB *LAST_SCB(enum which_list);


typedef struct Marker {
    struct Marker **prev;	/* pointer for event chain... */
    struct Marker *next;	/* ...(all from the same decl) */
    struct SCB *scb;
    struct Marker *link;	/* Links markers for an event */
    union tree_node *decl;	/* pointer to the associated decl tree entry */
    union {
	union tree_node **expr;	/* The event expression to evaluate */
	union tree_node *arg;	/* For gates, point to the argument */
	struct monitor_info *mon;	/* For $monitor and $wwaves, etc. */
	union tree_node *tree;	/* Catch-all for typeless usage */
	int vcd_id;		/* For VCD output */
	void *vcl;		/* For PLI VCL */
    } expr;
    enum marker_flags flags;
    union tree_node *delay;	/* if not null, put scb on timelist */
} Marker;

/* global info used during pass3 to build and thread markers as needed */

struct Marker_info {
    SCB *current_scb;
    int in_event;		/* If set, references will build markers */
    Marker *first;		/* Keep track of links during threading at pass3 */
    Marker *last;
    enum marker_flags flags;
    union tree_node *delay;
};

SCB *BuildSCB(union tree_node *, enum which_list);
tree dispatch_pc(enum which_list);
tree dump_current_process(void);
tree suspend_current_process(void);
void BuildMarker(tree decl, struct Marker_info *marker_info);
tree WaitOnTime(delay_t waittime, tree pc);
tree WaitOnTime64(struct Time64 *waittime, tree pc);
void WaitOnEvent(Marker * marker, SCB * scb);
union tree_node *WaitOnEventAll(Marker *, union tree_node *);
void WaitOnEventOther(SCB *, Marker *, union tree_node *);
void NotifyEvent(Marker *, enum logical_value, int);
void NotifyDeferred(union tree_node *);
void FreeSCB(SCB *);
void event_undo(Marker * first);
void cleanup_SCB(SCB *);
void interject_readylist(SCB *, union tree_node *);
void thread_marker(Marker *);
void thread_all_markers(Marker *);
void Schedule(delay_t waittime, SCB * scb, int prepend);
void ScheduleDelta64(struct Time64 *time, SCB * scb, int prepend);
void Schedule64(struct Time64 *time, SCB * scb, int prepend);
void ScheduleNet(Marker * marker, enum logical_value state);
void PrintSCB(SCB * scb);
tree WaitOnEventAll(Marker * marker, tree pc);
SCB *dispatcher(enum which_list remove_list);
void interject_readylist(SCB * scb, tree pc);
void init_sched(void);

// private methods

#ifdef SCHED_C
#endif				// SCHED_C

#endif				// SCHED_H
