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

/* Strobe.c - strobe  tasks and functions */

#define STROBE_C

#include <string.h>
#include "glue.h"
#include "vtypes.h"
#include "tree.h"
#include "acc_user.h"
#include "pli.h"
#include "schedule.h"
#include "strobe.h"
#include "dumpvar.h"
#include "systask.h"
#include "veriwell.h"

/* free strobe queue - one for all strobe queues */

static sched_strobe_t *strobe_free = (sched_strobe_t *) BADPTR;
static sched_strobe_t *active_strobe = (sched_strobe_t *) BADPTR;

/* strobe queue defines */

strobe_queue_t idle_strobe_queue =
    { (sched_strobe_t *) BADPTR, (sched_strobe_t *) BADPTR, NULL };
strobe_queue_t final_strobe_queue =
    { (sched_strobe_t *) BADPTR, (sched_strobe_t *) BADPTR, NULL };

/**************************************************************
 *
 *	tickle_monitor	
 *		- enqueue strobe for monitoring if not already queued  
 *		
 *************************************************************
 */

void tickle_monitor(strobe_queue_t * queue, tree pc)
{
    sched_strobe_t *s;

    ASSERT(queue != NULL);
    ASSERT(queue->head != BADPTR);
    ASSERT(queue->tail != BADPTR);
    ASSERT(pc != NULL);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);


    for (s = queue->head; s; s = s->next) {
	if (s->pc == pc) {
	    return;
	}
    }

    enqueue_strobe(queue, pc);
}

/**************************************************************
 *
 *	tickle_monitor_old	
 *		- old way currently works with markers of type M_STROBE  
 *
 *************************************************************
 */

void
tickle_monitor_old(strobe_queue_t * queue,
		   struct monitor_info *monitor_info)
{
    sched_strobe_t *s;

    ASSERT(queue != NULL);
    ASSERT(queue->head != BADPTR);
    ASSERT(queue->tail != BADPTR);
    ASSERT(monitor_info != NULL);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);

    if (!monitor_info->enable) {
	return;
    }


    for (s = queue->head; s; s = s->next) {
	if (s->pc == monitor_info->node) {
	    return;
	}
    }

    enqueue_strobe(queue, monitor_info->node);
}

/**************************************************************
 *
 *	is_strobe_active	
 *		-  returns TRUE if given pc is the active strobe 
 *		-  returns address of queue is pc is active strobe
 *
 *************************************************************
 */
//BOOL  is_strobe_active( tree pc )
strobe_queue_t *is_strobe_active(tree pc)
{
    ASSERT(pc != NULL);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);
    if (active_strobe && active_strobe->pc == pc) {
	return (active_strobe->list);
    } else {
	return (NULL);
    }
}

/**************************************************************
 *
 *	enqueue_strobe
 *		- enqueue pc to be executed on given strobe queue
 *
 *************************************************************
 */

void enqueue_strobe(strobe_queue_t * queue, tree pc)
{
    sched_strobe_t *strobe_tmp;

    ASSERT(queue != NULL);
    ASSERT(queue->head != BADPTR);
    ASSERT(queue->tail != BADPTR);
    ASSERT(pc != NULL);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);
    if (!strobe_free) {
	strobe_tmp = (sched_strobe_t *) xmalloc(sizeof(sched_strobe_t));
    } else {
	strobe_tmp = strobe_free;
	strobe_free = strobe_free->next;
    }
    strobe_tmp->pc = pc;
    strobe_tmp->next = NULL;
//      strobe_tmp->list = STROBE_ON;
    strobe_tmp->list = queue;
    if (queue->tail) {
	queue->tail->next = strobe_tmp;
	queue->tail = strobe_tmp;
    } else {
	queue->head = strobe_tmp;
	queue->tail = strobe_tmp;
    }
}


/**************************************************************
 *
 *	remove_strobe	
 *		- remove given strobe from queue. returns
 *		  TRUE on success. 
 *
 *************************************************************
 */

BOOL remove_strobe(strobe_queue_t * queue, sched_strobe_t * s)
{
    sched_strobe_t *s1, *prev;

    ASSERT(queue != NULL);
    ASSERT(queue->head != BADPTR);
    ASSERT(queue->tail != BADPTR);
    ASSERT(s != NULL);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);
    for (s1 = queue->head, prev = NULL; s1; prev = s1, s1 = s1->next) {
	if (s1 == s) {
	    if (!prev) {	/* Remove from front of list */
		queue->head = s->next;
		if (!queue->head) {
		    queue->tail = NULL;
		}
	    } else {
		prev->next = s->next;
		if (s == queue->tail) {
		    queue->tail = prev;
		}
	    }

	    /* thread onto free list */
	    s->next = strobe_free;
//              s->list = STROBE_OFF;
	    //           s->list = NULL;
	    strobe_free = s;
	    return (TRUE);
	}
    }
    return (FALSE);
}

/**************************************************************
 *
 *	dequeue_strobe	
 *		- remove strobe entry for a given pc
 *		  returns TRUE if pc is found. 
 *
 *************************************************************
 */

BOOL dequeue_strobe(strobe_queue_t * queue, tree pc)
{
    sched_strobe_t *s;
    BOOL result;

    ASSERT(queue != NULL);
    ASSERT(queue->head != BADPTR);
    ASSERT(queue->tail != BADPTR);
    ASSERT(pc != NULL);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);

    result = FALSE;

    for (s = queue->head; s; s = s->next) {
	if (s->pc == pc) {
	    remove_strobe(queue, s);
	    result = TRUE;
	}
    }
    return (result);
}

/**************************************************************
 *
 *	check_strobe
 *		- execute all strobes on queue, removing them
 *		  as we go
 *
 *************************************************************
 */

void check_strobe(strobe_queue_t * queue)
{
    sched_strobe_t *s;

    ASSERT(queue != NULL);
    ASSERT(queue->head != BADPTR);
    ASSERT(queue->tail != BADPTR);
    ASSERT(strobe_free != BADPTR);
    ASSERT(active_strobe != BADPTR);

    dumpvar.new_time = 1;

    /*
     * set fence at current end of list so
     * newly enqueue strobes are processed in 
     * the next pass
     */
    queue->fence = queue->tail;
    for (s = queue->head; s; s = queue->head) {
	active_strobe = s;
	remove_strobe(queue, s);
	exec_systask(s->pc);
	active_strobe = NULL;
	if (s == queue->fence) {
	    break;
	}
    }
    queue->fence = NULL;
}

/**************************************************************
 *
 *	empty_strobe
 *		- return TRUE if strobe queue is empty
 *
 *************************************************************
 */
int empty_strobe(strobe_queue_t * queue)
{
    return queue->head == NULL;
}

/**************************************************************
 *
 *	init_strobe
 *		- initializes global and static variables for strobe.c
 *
 *************************************************************
 */

void init_strobe()
{
    strobe_free = NULL;
    active_strobe = NULL;
    memset(&idle_strobe_queue, 0, sizeof(idle_strobe_queue));
    memset(&final_strobe_queue, 0, sizeof(final_strobe_queue));
}
