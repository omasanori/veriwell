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

/************************************************************************
 *
 *		nsched.c
 *			- experimental framework for new scheduler for gates
 *
 *********************************************************************** 
 */


#define NSCHED_C

#include <stdio.h>
#include "vtypes.h"
#include "tree.h"
#include "glue.h"
#include "nsched.h"
#include "veriwell.h"

static timeq_t *gateList = (timeq_t *) BADPTR;
static timeq_t *nodeFreeList = (timeq_t *) BADPTR;

extern time64 CurrentTime;


/**************************************************************************
 
	ScheduleGate
		-	add gate to list of gates ready to fire

 **************************************************************************/

void ScheduleGate(tree gate, delay_t delay)
{
    time64 timeIndex;
    timeq_t *newNode;
    tree head;
    timeq_t *node;
    int cmpResult;

    ASSERT(gate != NULL);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    ASSERT(gateList != BADPTR);
    ASSERT(nodeFreeList != BADPTR);

    ADDTIME6432(&timeIndex, &CurrentTime, delay);

    /* this is a hack to attempt to match Cadence */
    /*  (also, reschedule if new input before delay is up */

    if (IsGateScheduled(gate) &&
	!CMPTIME64(&(GATE_TIMEQ(gate)->timeIndex), &timeIndex) &&
	!(TREE_TYPE(gate) == GATE_UDP_TYPE
	  && GATE_PATH_OUTPUT(gate) == NULL_TREE)
	&& !(TREE_TYPE(gate) == GATE_NOT_TYPE
	     && TREE_CHAIN(GATE_OUTPUT_LIST(gate)) != NULL_TREE)
	&& !(TREE_TYPE(gate) == GATE_BUF_TYPE
	     && TREE_CHAIN(GATE_OUTPUT_LIST(gate)) != NULL_TREE)) {
	return;
    }


    if (IsGateScheduled(gate)) {
	RemoveGate(gate);
    }

    if (gateList == NULL) {
	// insert a new node at the beginning of list 

	if (nodeFreeList != NULL) {
	    newNode = nodeFreeList;
	    nodeFreeList = nodeFreeList->next;
	} else {
	    newNode = (timeq_t *) xmalloc(sizeof(timeq_t));
	}

	newNode->gate = gate;
	GATE_NEXT(gate) = gate;
	GATE_PREV(gate) = gate;
	GATE_TIMEQ(gate) = newNode;
	newNode->next = newNode;
	newNode->prev = newNode;
	ASSIGNTIME64(&newNode->timeIndex, &timeIndex);
	gateList = newNode;
	return;
    }

    node = gateList;
    while (TRUE) {
	cmpResult = CMPTIME64(&timeIndex, &node->timeIndex);
	if (cmpResult == 0) {
	    // add to this node
	    head = node->gate;
	    GATE_NEXT(gate) = head;
	    GATE_PREV(gate) = GATE_PREV(head);
	    GATE_NEXT(GATE_PREV(head)) = gate;
	    GATE_PREV(head) = gate;
	    GATE_TIMEQ(gate) = node;
	    return;
	} else if (cmpResult < 0) {
	    // insert a new node before this node
	    if (nodeFreeList != NULL) {
		newNode = nodeFreeList;
		nodeFreeList = nodeFreeList->next;
	    } else {
		newNode = (timeq_t *) xmalloc(sizeof(timeq_t));
	    }

	    newNode->gate = gate;
	    ASSIGNTIME64(&newNode->timeIndex, &timeIndex);
	    GATE_TIMEQ(gate) = newNode;
	    GATE_NEXT(gate) = gate;
	    GATE_PREV(gate) = gate;
	    newNode->next = node;
	    newNode->prev = node->prev;
	    node->prev->next = newNode;
	    node->prev = newNode;
	    if (node == gateList) {
		gateList = newNode;
	    }
	    return;
	} else if (node->next == gateList) {
	    // insert a new node at the end of queue
	    if (nodeFreeList != NULL) {
		newNode = nodeFreeList;
		nodeFreeList = nodeFreeList->next;
	    } else {
		newNode = (timeq_t *) xmalloc(sizeof(timeq_t));
	    }

	    newNode->gate = gate;
	    ASSIGNTIME64(&newNode->timeIndex, &timeIndex);
	    GATE_TIMEQ(gate) = newNode;
	    GATE_NEXT(gate) = gate;
	    GATE_PREV(gate) = gate;
	    newNode->next = gateList;
	    newNode->prev = gateList->prev;
	    gateList->prev->next = newNode;
	    gateList->prev = newNode;
	    return;
	}
	node = node->next;
    }
    ASSERT(FALSE);
    return;
}

/**************************************************************************
 
	PeekGate
		-	return pointer to next gate ready to fire	

 **************************************************************************/

tree PeekGate(void)
{

    if (gateList) {
	return (gateList->gate);
    } else {
	return (NULL_TREE);
    }
}

/**************************************************************************
 
	RemoveNextReadyGate
		-	return pointer to next gate ready to fire and remove it from list	

 **************************************************************************/

tree RemoveNextReadyGate(void)
{
    tree next;

    next = PeekGate();
    if (next != NULL_TREE &&
	CMPTIME64(&(GATE_TIMEQ(next)->timeIndex), &CurrentTime) == 0) {
	RemoveGate(next);
	return (next);
    }
    return (NULL_TREE);
}

/**************************************************************************
 
	RemoveGate
		-	remove the given gate from the gatelist	

 **************************************************************************/

void RemoveGate(tree gate)
{
    timeq_t *head;


    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);

    head = GATE_TIMEQ(gate);

    if (GATE_NEXT(gate) == gate) {
	// last gate for this time so remove node
	GATE_TIMEQ(gate) = NULL;
	if (head->next == head) {
	    // gatelist is now empty
	    gateList = NULL;
	    head->next = nodeFreeList;
	    nodeFreeList = head;
	    return;
	}
	// unlink node
	if (head == gateList) {
	    gateList = head->next;
	}
	head->next->prev = head->prev;
	head->prev->next = head->next;
	return;
    }
    // unlink gate only
    if (head->gate == gate) {
	head->gate = GATE_NEXT(gate);
    }
    GATE_NEXT(GATE_PREV(gate)) = GATE_NEXT(gate);
    GATE_PREV(GATE_NEXT(gate)) = GATE_PREV(gate);
    GATE_TIMEQ(gate) = NULL;

}

/**************************************************************************
 
	IsGateScheduled	
		-	returns TRUE if gate is currently scheduled	

 **************************************************************************/

int IsGateScheduled(tree gate)
{
    ASSERT(gate != NULL_TREE);
    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    return (GATE_TIMEQ(gate) != NULL);
}

/**************************************************************************
 
	IsGateReady	
		-	returns TRUE if gate is currently ready to fire	

 **************************************************************************/

int IsGateReady()
{
    tree gate;

    gate = PeekGate();

    if (gate == NULL_TREE) {
	return (FALSE);
    }

    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    return (CMPTIME64(&(GATE_TIMEQ(gate)->timeIndex), &CurrentTime) == 0);
}

/**************************************************************************
 
	GateConditionalAdvanceTime	
		-	advance time if it is less than or equal to given time	
		    return TRUE if time advanced

 **************************************************************************/

int GateConditionalAdvanceTime(time64 * timePtr)
{
    extern void set_simulation_time(time64 *);	/* in exec.c */
    tree gate;

    gate = PeekGate();

    if (gate == NULL_TREE) {
	return (FALSE);
    }

    ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
    if (CMPTIME64(&(GATE_TIMEQ(gate)->timeIndex), timePtr) >= 0) {
	return (FALSE);
    }
//  ASSIGNTIME64( &CurrentTime, &(GATE_TIMEQ(gate)->timeIndex) );
    set_simulation_time(&(GATE_TIMEQ(gate)->timeIndex));
    return (TRUE);
}

/**************************************************************************
 
	InitNSched	
		- initialize module global data	

 **************************************************************************/
void InitNSched()
{
    gateList = NULL;
    nodeFreeList = NULL;
}
