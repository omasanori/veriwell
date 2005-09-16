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

/****************************************************************************
 *
 *	nsched.h
 *		- header for nsched.c
 *
 ***************************************************************************
 */


#ifndef NSCHED_H
#define NSCHED_H

// public definitions

typedef struct timeq_tag {
    struct timeq_tag *next;
    struct timeq_tag *prev;
    time64 timeIndex;
    tree gate;
} timeq_t;

void ScheduleGate(tree gate, delay_t delay);
tree PeekGate(void);
tree RemoveNextReadyGate(void);
void RemoveGate(tree);
int IsGateScheduled(tree);
int IsGateReady(void);
int GateConditionalAdvanceTime(time64 * time);
void InitNSched(void);



#ifdef NSCHED_C

#endif				// NSCHED_C

#endif				// NSCHED_H
