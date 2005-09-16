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

/* STROBE.H - all maintaince methods for managing strobes */

#ifndef STROBE_H
#define STROBE_H

// public  methods

#define BOOL	int

struct strobe_queue;

/* Thread strobe or monitor request; exec() checks when time changes */
typedef struct sched_strobe {
    tree pc;
    struct strobe_queue *list;	/* Which list; 0 for OFF */
    struct sched_strobe *next;
} sched_strobe_t;


/* monitor structure */

typedef struct monitor_info {
    int enable;
    tree node;
} monitor_info_t;

/*  strobe queue structure */

typedef struct strobe_queue {
    sched_strobe_t *head;
    sched_strobe_t *tail;
    sched_strobe_t *fence;
} strobe_queue_t;

/* extern strobe queue defines */

extern strobe_queue_t idle_strobe_queue;
extern strobe_queue_t final_strobe_queue;

/* access routine prototypes */

void init_strobe(void);
void check_strobe(strobe_queue_t * queue);
void enqueue_strobe(strobe_queue_t * queue, tree pc);
BOOL remove_strobe(strobe_queue_t * queue, struct sched_strobe *strobe);
BOOL dequeue_strobe(strobe_queue_t * queue, tree pc);
strobe_queue_t *is_strobe_active(tree pc);
void tickle_monitor_old(strobe_queue_t * queue,
			monitor_info_t * monitor_info);
void tickle_monitor(strobe_queue_t * queue, tree t);
void init_strobe(void);
int empty_strobe(strobe_queue_t * queue);

#ifdef STROBE_C
// private methods
#endif				// STROBE_C

#endif				/* STROBE_H */
