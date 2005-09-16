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

/******************************************************************************
 *
 *	plihacks.h
 *		definitions for plihacks.c 	
 *
 ******************************************************************************/

#ifndef PLIHACKS_H
#define PLIHACKS_H

// global definitions

int abort_call(int data, int reason);
int dumpstructure_call(int data, int reason);
int setvalue_call(int data, int reason);
void lineTrace(char *file, unsigned int line, char *udata);

#ifdef PLIHACKS_C

// private definitions

static void push(void);
static void pop(void);
static void indent(void);
static void DumpTask(handle object);
static void DumpFunction(handle object);
static void DumpModule(handle object);
static void dumpDecl(handle object);
static void DumpObject(handle);
#endif				// PLIHACKS_C

#endif				// PLIHACKS_H
