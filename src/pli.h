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

/* PLI.H - PLI structures and routine prototypes */

#ifndef PLI_H
#define PLI_H

// public methods


//#define PLI_ARG_MARKERS(NODE) (TREE_VALUE (TREE_STMT (NODE)))
//#define PLI_ARG_PVC(NODE) (TREE_PURPOSE (TREE_STMT (NODE)))
//#define PLI_ARG_INFO(NODE) (TREE_3RD (TREE_STMT (NODE)))

enum pli_arg_type { PLI_FREE, PLI_ASYNCH, PLI_VCL };

typedef tree instance_t;

typedef struct PliInfo_tag {
    int pvc:1;			/* This argument has changed */
    int saved_pvc:1;		/* Saved for the user */
    int vclSet:1;		/* driver vcls are set */
    int pad:29;			/* bit padding */
    int argNumber;		/* number of argument,starting at 1 */
    tree instance;		/* Pointer to the task or function */
    Group *storage;		/* Pointer to data storage */
    struct Marker *marker;	/* placeholder for markers */
} PliInfo_t;


struct pli_vcl {
    struct Marker *marker;
    int (*routine) (s_vc_record *);
    tree decl;
    char *user_data;
};

typedef struct pli_lcb {
    struct pli_lcb *next;
    void (*routine) (char *, unsigned int, char *);
    tree module;
    char *userdata;
} pli_lcb;

extern tree current_tf_instance;
extern int lcbCount;

void vcl_dispatch(struct pli_vcl *);	/* pli.i 17/05/94 22.58.34 */
void freePliDelayNode(tree);
void handleLcb(tree);
void init_pli(void);

// private methods

#ifdef PLI_C
static Group *fetch_storage(tree object);
static char *pli_write_string(char *string);
static int lookup_vcl(tree object, int (*routine) (s_vc_record *),
		      char *user_data, struct Marker **marker);
static tree nth_arg(int n, tree instance);
static tree nth_parameter(int n, tree instance);
static int is_scalar(handle object);
static int is_vector(handle object);
static int is_expanded(handle object);
static int is_collapsed(handle object);
static char *GroupToString(Group * g, int nbits, int ngroups);
static void StringToGroup(char *string, Group * g, int nbits, int ngroups);
char *append_name(handle object);
static handle resolve_path(handle anchor, char *path);
#endif				// PLI_C

#endif				// PLI_H
