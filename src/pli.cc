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

/* PLI.C - Programming language interface support routines */

#define PLI_C

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "vtypes.h"
#include "tree.h"
#include "glue.h"
#include "decl.h"
#include "veriuser.h"
#include "acc_user.h"
#include "pli.h"
#include "schedule.h"
#include "io.h"
#include "print.h"
#include "strobe.h"
#include "systask.h"
#include "scope.h"
#include "pass3.h"
#include "parse.h"
#include "runtime.h"
#include "eval.h"
#include "veriwell.h"
#include "store.h"
#include "specify.h"
#include "timescal.h"
#include "pass2.h"
#include "flags.h"
#include "multdiv.h"
#include "nsched.h"
#include "lex.h"


#ifndef min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif				// min

#ifndef max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif				// max

#define	avg(a,b)	(((a)+(b))/2)

typedef enum { eMapMax, eMapMin, eMapLatest } MapToMipd_t;
typedef enum
    { eDelayAverage, eDelayMax, eDelayMin, eDelayFromUser } ToHiZDelay_t;
struct pli_global_info {
    char *stringbuf;		/* beginning of buffer */
    char *p;			/* current pointer */
    char *lastCopy;		/* pointer to begin of last copied string */
    int stringbuf_size;		/* size of buffer */
    handle scope;		/* scope for acc_handle_object */
    char *text;			/* used for buffer for tf_text */
    char *nextText;		/* points to next useable location */
    int textSize;		/* size of text buffer */
    struct {
	int accDefaultAttr0_;
	char accDevelopmentVersion_[256];
	int accDisplayWarnings_;
	int accDisplayErrors_;
	int accEnableArgsModpath_;
	int accEnableArgsTchk_;
	int accEnableArgsSetScope_;
	MapToMipd_t accMapToMipd_;
	int accMinTypMaxDelays_;
	int accPathDelayCount_;
	char accPathDelimStr_[256];
	ToHiZDelay_t accToHiZDelay_;
    } config;
} pli_info = {
NULL, NULL, 0};

int acc_error_flag = FALSE;
tree delayFreeList = NULL;
int lcbCount = 0;


/************************************************************************
 * support routines							*
/***********************************************************************/

/************************************************************************
 * acc_initialize							
 *********************************************************************** *
 */

int acc_initialize(void)
{
    acc_error_flag = FALSE;
    pli_info.config.accDefaultAttr0_ = FALSE;
    pli_info.config.accDisplayWarnings_ = 0;
    pli_info.config.accDisplayErrors_ = 1;
    pli_info.config.accEnableArgsModpath_ = FALSE;
    pli_info.config.accEnableArgsTchk_ = FALSE;
    pli_info.config.accEnableArgsSetScope_ = FALSE;
    pli_info.config.accMapToMipd_ = eMapMax;
    pli_info.config.accMinTypMaxDelays_ = FALSE;
    pli_info.config.accPathDelayCount_ = 6;
    pli_info.scope = (handle) current_scope;
    strcpy(pli_info.config.accPathDelimStr_, "$");
    pli_info.config.accToHiZDelay_ = eDelayFromUser;

    if (pli_info.text == NULL) {
	pli_info.textSize = 1024;
	pli_info.text = (char *) xmalloc(pli_info.textSize);
	if (pli_info.text == NULL) {
	    tf_error("Out of Memory");
	    return (0);
	}
    }
    if (pli_info.stringbuf == NULL) {
	pli_info.stringbuf_size = 4096;
	pli_info.stringbuf =
	    (char *) xmalloc(pli_info.stringbuf_size);
	if (pli_info.stringbuf == NULL) {
	    tf_error("Out of Memory");
	    return (0);
	}
    }
    pli_info.p = pli_info.stringbuf;
    pli_info.lastCopy = NULL;
    pli_info.nextText = pli_info.text;
    return (1);
}

/************************************************************************
	init_pli	
		- initialize pli module 	
*************************************************************************
*/

void init_pli(void)
{
    strcpy(pli_info.config.accDevelopmentVersion_, "IEEE 1364 PLI 1.0");
    pli_info.p = NULL;
    pli_info.lastCopy = NULL;
    pli_info.stringbuf = NULL;
    pli_info.stringbuf_size = 0;
    pli_info.text = NULL;
    pli_info.nextText = NULL;
    pli_info.textSize = 0;

    delayFreeList = NULL;

    lcbCount = 0;

    acc_initialize();
}

/************************************************************************
	fetch_storage
 		Get the pointer to the storage of a decl, etc
*************************************************************************
*/

static Group *fetch_storage(tree object)
{
    return DECL_STORAGE(object);
}

/*********************************************************************
	nth_arg
		-   Generic routine for returning nth argument 
**********************************************************************/

static tree nth_arg(int n, tree instance)
{
    tree arg;

    arg = nth_parameter(n, instance);
    if (arg != NULL) {
	return (TREE_PURPOSE(arg));
    } else {
	return (NULL);
    }
}

/*********************************************************************
	nth_parameter
		-   Generic routine for returning nth parameter 
**********************************************************************/

static tree nth_parameter(int n, tree instance)
{
    int i;
    int N = tf_inump((char *) instance);
    tree arg;

    if (n < 1 || n > N)
	return NULL;

    if (TREE_CODE(instance) == SYSTASK_STMT)
	arg = STMT_TASK_ARGS(instance);
    else if (TREE_CODE(instance) == SYSFUNCTION_REF)
	arg = FUNC_REF_ARGS(instance);

    for (i = 1; i < n; i++, arg = TREE_CHAIN(arg));

    return arg;
}

/*********************************************************************
	pli_write_string
		-	Write a string into the pli buffer, 
			return a pointer to the buffer.
  			if the buffer is full, sent a warning (if enabled) 
  			and reset the buffer
**********************************************************************/


static char *pli_write_string(char *string)
{
    int size;
    char *p;

    if (string == NULL) {
	pli_info.lastCopy = NULL;
	return (pli_info.p);
    }

    size = strlen(string);
    if (pli_info.p + size + 1 >
	pli_info.stringbuf + pli_info.stringbuf_size) {
	tf_warning("String pointer being reset");
	pli_info.p = pli_info.stringbuf;
    }

    strcpy(pli_info.p, string);
    p = pli_info.p;
    pli_info.p += size + 1;

    pli_info.lastCopy = p;
    return p;
}

/*********************************************************************
	pli_append_string
		-	Append a string to the string currently in the pli buffer, 
			return a pointer to the start of the whole string.
  			if the buffer is full, sent a warning (if enabled) 
  			and reset the buffer
**********************************************************************/


static char *pli_append_string(char *string)
{
    int size;
    char *p;


    if (string == NULL) {
	return pli_info.lastCopy;
    }

    size = strlen(string);
    if (pli_info.p + size + 1 >
	pli_info.stringbuf + pli_info.stringbuf_size) {
	tf_warning("String pointer being reset");
	pli_info.p = pli_info.stringbuf;
	if (pli_info.lastCopy != NULL) {
	    strcpy(pli_info.stringbuf, pli_info.lastCopy);
	    pli_info.lastCopy = pli_info.stringbuf;
	    pli_info.p =
		pli_info.stringbuf + strlen(pli_info.lastCopy) + 1;
	}
    }

    if (pli_info.lastCopy != NULL) {
	pli_info.p--;
    } else {
	pli_info.lastCopy = pli_info.p;
    }

    strcpy(pli_info.p, string);
    p = pli_info.p;
    pli_info.p += size + 1;

    return pli_info.lastCopy;
}

/************************************************************************
 * acc_configure							*
/***********************************************************************/

int acc_configure(int what, char *string)
{
    acc_error_flag = FALSE;

    switch (what) {
    case accDefaultAttr0:
	if (!strcmp(string, "true")) {
	    pli_info.config.accDefaultAttr0_ = TRUE;
	} else if (!strcmp(string, "false")) {
	    pli_info.config.accDefaultAttr0_ = FALSE;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    case accDevelopmentVersion:
	if (strcmp(pli_info.config.accDevelopmentVersion_, string)) {
	    acc_error_flag = TRUE;
	}
	break;

    case accDisplayErrors:
	if (!strcmp(string, "true")) {
	    pli_info.config.accDisplayErrors_ = TRUE;
	} else if (!strcmp(string, "false")) {
	    pli_info.config.accDisplayErrors_ = FALSE;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    case accDisplayWarnings:
	if (!strcmp(string, "true")) {
	    pli_info.config.accDisplayWarnings_ = TRUE;
	} else if (!strcmp(string, "false")) {
	    pli_info.config.accDisplayWarnings_ = FALSE;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    case accEnableArgs:
	if (!strcmp(string, "acc_handle_modpath")) {
	    pli_info.config.accEnableArgsModpath_ = TRUE;
	} else if (!strcmp(string, "no_acc_handle_modpath")) {
	    pli_info.config.accEnableArgsModpath_ = FALSE;
	} else if (!strcmp(string, "acc_handle_tchk")) {
	    pli_info.config.accEnableArgsTchk_ = TRUE;
	} else if (!strcmp(string, "no_acc_handle_tchk")) {
	    pli_info.config.accEnableArgsTchk_ = FALSE;
	} else if (!strcmp(string, "acc_set_scope")) {
	    pli_info.config.accEnableArgsSetScope_ = TRUE;
	} else if (!strcmp(string, "no_acc_set_scope")) {
	    pli_info.config.accEnableArgsSetScope_ = FALSE;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    case accMapToMipd:
	if (!strcmp(string, "max")) {
	    pli_info.config.accMapToMipd_ = eMapMax;
	} else if (!strcmp(string, "min")) {
	    pli_info.config.accMapToMipd_ = eMapMin;
	} else if (!strcmp(string, "latest")) {
	    pli_info.config.accMapToMipd_ = eMapLatest;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    case accMinTypMaxDelays:
	if (!strcmp(string, "true")) {
	    pli_info.config.accMinTypMaxDelays_ = TRUE;
	} else if (!strcmp(string, "false")) {
	    pli_info.config.accMinTypMaxDelays_ = FALSE;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    case accPathDelayCount:
	if (strlen(string) != 1) {
	    acc_error_flag = TRUE;
	    break;
	}
	switch (string[0]) {
	case '1':
	case '2':
	case '3':
	case '6':
	    pli_info.config.accPathDelayCount_ = atoi(string);
	    break;
	default:
	    acc_error_flag = TRUE;
	    break;
	}
	break;

    case accPathDelimStr:
	strncpy(pli_info.config.accPathDelimStr_, string,
		sizeof(pli_info.config.accPathDelimStr_) - 1);
	break;


    case accToHiZDelay:
	if (!strcmp(string, "average")) {
	    pli_info.config.accToHiZDelay_ = eDelayAverage;
	} else if (!strcmp(string, "max")) {
	    pli_info.config.accToHiZDelay_ = eDelayMax;
	} else if (!strcmp(string, "min")) {
	    pli_info.config.accToHiZDelay_ = eDelayMin;
	} else if (!strcmp(string, "from_user")) {
	    pli_info.config.accToHiZDelay_ = eDelayFromUser;
	} else {
	    acc_error_flag = TRUE;
	}
	break;

    default:
	acc_error_flag = TRUE;
	tf_error("Unknown configuration parameter %d in acc_configure",
		 what);
	return (0);
    }

    if (acc_error_flag) {
	tf_error("Unknown configuration value \"%s\" in acc_configure",
		 string);
    }

    return (acc_error_flag);
}

/************************************************************************
 * acc_close								*
/***********************************************************************/

void acc_close()
{
    acc_error_flag = FALSE;
    acc_initialize();
    pli_info.scope = null;
}

/************************************************************************
 	acc_collect							
*************************************************************************
*/

handle *acc_collect(handle(*nextFunction) (handle, handle),
		    handle reference, int *numberOfObjects)
{
    handle object;
    handle *list;
    int count;
    int listLength;

    acc_error_flag = FALSE;

    listLength = 64;
    list = (handle *) xmalloc(listLength * sizeof(handle *));
    count = 0;
    object = NULL;
    while (list != NULL) {
	object = (nextFunction) (reference, object);
	if (object == NULL) {
	    break;
	}
	if (count > listLength) {
	    listLength += 64;
	    list =
		(handle *) xrealloc((char*)list,
					 listLength * sizeof(handle *));
	    if (list == NULL) {
		break;
	    }
	}
	list[count] = object;
	count++;

    }

    if (list == NULL) {
	count = 0;
	acc_error_flag = TRUE;
	tf_error("Memory allocation failed in acc_collect");
    }

    *numberOfObjects = count;
    return (list);
}

/************************************************************************
 	acc_free							
*************************************************************************
*/
void acc_free(handle * list)
{
    acc_error_flag = FALSE;
    free(list);
}

/************************************************************************
 	acc_count							
*************************************************************************
*/

int acc_count(handle(*nextFunction) (handle, handle), handle reference)
{
    handle object;
    int count;

    acc_error_flag = FALSE;

    count = 0;
    object = NULL;
    while (TRUE) {
	object = (nextFunction) (reference, object);
	if (object == NULL) {
	    break;
	}
	count++;

    }

    return (count);
}

/************************************************************************
 	acc_object_in_typelist							
*************************************************************************
*/

bool acc_object_in_typelist(handle object, int *typeList)
{
    int *ptr;

    acc_error_flag = FALSE;
    for (ptr = typeList; *ptr != 0; ptr++) {
	if (acc_object_of_type(object, *ptr)) {
	    return (TRUE);
	}
    }
    return (FALSE);
}

/************************************************************************
 	acc_object_of_type							
*************************************************************************
*/

bool acc_object_of_type(handle object, int type)
{
    acc_error_flag = FALSE;

    /* check type first */

    if (acc_fetch_type(object) == type) {
	return (TRUE);
    }

    /* check full type next */

    if (acc_fetch_fulltype(object) == type) {
	return (TRUE);
    }

    /* check propertys next */

    switch (type) {
    case accScalar:
	return (is_scalar(object));
    case accVector:
	return (is_vector(object));
    case accCollapsedNet:
	return (is_collapsed(object));
    case accExpandedVector:
	return (is_expanded(object));
    case accUnexpandedVector:
	return (!is_expanded(object) && is_vector(object));
    }

    return (FALSE);
}

/************************************************************************
 	is_scalar
 		returns true if object is scalar							
*************************************************************************
*/

static int is_scalar(handle object)
{
    int type;

    type = acc_fetch_type(object);
    switch (type) {
    case accNet:
    case accPort:
    case accRegister:
	return (acc_fetch_size(object) == 1);
    }
    return (FALSE);
}

/************************************************************************
 	is_vector
 		returns true if object is vector							
*************************************************************************
*/

static int is_vector(handle object)
{
    int type;

    type = acc_fetch_type(object);
    switch (type) {
    case accNet:
    case accPort:
    case accRegister:
	return (acc_fetch_size(object) > 1);
    }
    return (FALSE);
}

/************************************************************************
 	is_collapsed
 		returns true if object is collapsed							
*************************************************************************
*/

static int is_collapsed(handle object)
{
    /* collapsed nets aren't supported */
    return (FALSE);
}

/************************************************************************
 	is_expanded
 		returns true if object is expanded							
*************************************************************************
*/

static int is_expanded(handle object)
{
    /* expanded nets aren't supported */
    return (FALSE);
}

/************************************************************************
 	acc_product_type					
*************************************************************************
*/
int acc_product_type(void)
{
    acc_error_flag = FALSE;
    return (accSimulator);
}

/************************************************************************
 	acc_product_version					
*************************************************************************
*/
char *acc_product_version(void)
{
    char *ptr;

    acc_error_flag = FALSE;
    pli_write_string("VeriWell Simulator Version ");
    ptr = pli_append_string(pli_info.config.accDevelopmentVersion_);
    return (ptr);
}

/************************************************************************
 	acc_version					
*************************************************************************
*/
char *acc_version(void)
{
    char *ptr;

    acc_error_flag = FALSE;
    pli_write_string("Access Routines Version ");
    ptr = pli_append_string(pli_info.config.accDevelopmentVersion_);
    return (ptr);
}


/************************************************************************
 * acc_next_topmod							*
/***********************************************************************/

handle acc_next_topmod(handle topmod)
{
    acc_error_flag = FALSE;

    if (!topmod) {
	return (handle) top_level;
    } else {
	return (handle) TREE_CHAIN((tree) topmod);
    }
}

/****************************************************************

	acc_next_child	
		- return  the handle to the next child of the given module 

*****************************************************************/

handle acc_next_child(handle module, handle child)
{
    tree t;

    acc_error_flag = FALSE;

    // acts like acc_next_topmod if module handle is NULL 

    if (module == NULL) {
	return (acc_next_topmod(child));
    }

    if (child == NULL) {
	t = BLOCK_DOWN((tree) module);
    } else {
	t = TREE_CHAIN((tree) child);
    }

    // sift through other things on list

    while (t != NULL && TREE_CODE(t) != MODULE_BLOCK) {
	t = TREE_CHAIN(t);
    }

    return ((handle) t);
}

/************************************************************************
 * acc_next_cell							*
/***********************************************************************/

handle acc_next_cell(handle reference, handle cell)
{
    // cells are not currently supported
    acc_error_flag = FALSE;
    return (NULL);
}

/************************************************************************
 * acc_next_cell_load							*
/***********************************************************************/

handle acc_next_cell_load(handle reference, handle load)
{
    // cells are not currently supported
    acc_error_flag = FALSE;
    return (NULL);
}

/************************************************************************
 * acc_next_load							*
/***********************************************************************/

handle acc_next_load(handle reference, handle load)
{
    handle parent;
    handle primitive;
    handle terminal;
    int match;

    acc_error_flag = FALSE;

    match = FALSE;

    parent = acc_handle_parent(reference);
    primitive = null;
    while (TRUE) {
	primitive = acc_next_primitive(parent, primitive);
	if (primitive == null) {
	    break;
	}
	terminal = null;
	while (TRUE) {
	    terminal = acc_next_terminal(primitive, terminal);
	    if (terminal == null) {
		break;
	    }
	    if (acc_fetch_direction(terminal) != accOutput) {
		if (load == null || match) {
		    return (terminal);
		}
		if (acc_handle_conn(terminal) == load) {
		    match = TRUE;
		}
	    }
	}
    }


    return (NULL);
}

/************************************************************************
 * acc_next_bit							*
/***********************************************************************/

handle acc_next_bit(handle reference, handle bit)
{
    // expanded vectors or ports are not currently supported
    acc_error_flag = FALSE;
    return (NULL);
}

/****************************************************************

	acc_next

*****************************************************************/
handle acc_next(int *object_type_list, handle reference, handle aObject)
{
    int i;
    handle object;

    acc_error_flag = FALSE;
    object = aObject;

    do {
	if (object == NULL) {
	    object = acc_next_primitive(reference, object);
	    if (object == NULL) {
		object = acc_next_child(reference, object);
	    }
	    if (object == NULL) {
		object = (handle) BLOCK_DECL((tree) reference);
		while (TRUE) {
		    if (object == NULL || acc_fetch_type(object) != accNet) {
			break;
		    }
		    object = (handle) TREE_CHAIN((tree) object);
		}
	    }
	    if (object == NULL) {
		object = acc_next_net(reference, null);
	    }
	} else {
	    switch (acc_fetch_type(object)) {
	    case accPrimitive:
		object = acc_next_primitive(reference, object);
		if (object == NULL) {
		    object = acc_next_child(reference, object);
		}
		if (object == NULL) {
		    object = (handle) BLOCK_DECL((tree) reference);
		    while (TRUE) {
			if (object == NULL
			    || acc_fetch_type(object) != accNet) {
			    break;
			}
			object = (handle) TREE_CHAIN((tree) object);
		    }
		}
		if (object == NULL) {
		    object = acc_next_net(reference, null);
		}
		break;
	    case accModule:
		object = acc_next_child(reference, object);
		if (object == NULL) {
		    object = (handle) BLOCK_DECL((tree) reference);
		    while (TRUE) {
			if (object == NULL
			    || acc_fetch_type(object) != accNet) {
			    break;
			}
			object = (handle) TREE_CHAIN((tree) object);
		    }
		}
		if (object == NULL) {
		    object = acc_next_net(reference, null);
		}
		break;
	    case accNet:
		object = acc_next_net(reference, object);
		break;
	    default:
		object = (handle) TREE_CHAIN((tree) object);
		while (TRUE) {
		    if (object == NULL || acc_fetch_type(object) != accNet) {
			break;
		    }
		    object = (handle) TREE_CHAIN((tree) object);
		}
		if (object == NULL) {
		    object = acc_next_net(reference, null);
		}
	    }
	}

	if (object == NULL) {
	    return (NULL);
	}


	for (i = 0; object_type_list[i] != 0; i++) {
	    if (acc_fetch_type(object) == object_type_list[i]) {
		return (object);
	    }
	    if (acc_fetch_fulltype(object) == object_type_list[i]) {
		return (object);
	    }
	}
    }
    while (object != NULL);


    return (NULL);
}

/****************************************************************

	acc_next_modpath
		- retrieve all module paths in given scope 

*****************************************************************/

handle acc_next_modpath(handle module, handle path)
{
    tree t;

    acc_error_flag = FALSE;
    if (path == NULL) {
	t = MODULE_SPECINST((tree) module);
    } else {
	t = TREE_CHAIN((tree) path);
    }
    while (t && TREE_CODE(t) != PATH_INSTANCE) {
	t = TREE_CHAIN(t);
    }
    return ((handle) t);
}

/****************************************************************

	acc_next_primitive
		- retrieve all primitives in given scope 

*****************************************************************/

handle acc_next_primitive(handle module, handle gate)
{
    tree t;

    acc_error_flag = FALSE;
    if (gate == NULL) {
	t = BLOCK_BODY((tree) module);
    } else {
	t = TREE_CHAIN((tree) gate);
    }
    while (t && TREE_CODE(t) != GATE_INSTANCE) {
	t = TREE_CHAIN(t);
    }
    return ((handle) t);
}

/****************************************************************

	acc_next_tchk
		- retrieve all timing checks in given scope 

*****************************************************************/

handle acc_next_tchk(handle module, handle check)
{
    tree t;

    acc_error_flag = FALSE;

    if (check == NULL) {
	t = MODULE_SPECINST((tree) module);
    } else {
	t = TREE_CHAIN((tree) check);
    }
    while (t && TREE_CODE(t) != TIMING_CHECK) {
	t = TREE_CHAIN(t);
    }
    return ((handle) t);
}

/****************************************************************

	acc_handle_conn
		- return net connected to terminal 

*****************************************************************/
handle acc_handle_conn(handle terminal)
{
    tree t = (tree) terminal;
    tree n;
    tree desc;
    tree module;
    tree instance;
    tree out;
    tree gate;
    tree constraint;

    ASSERT(t != NULL);
    ASSERT(TREE_CODE(t) == TREE_LIST);
    acc_error_flag = FALSE;
    if (terminal == NULL) {
	acc_error_flag = TRUE;
	tf_error("Illegal object in acc_handle_conn()");
	return null;
    }

    if (TREE_CODE(t) == PATH_CONSTRAINT) {
	desc = PATH_CONSTRAINT_PATHDESC(t);
	instance = PATH_CONSTRAINT_INSTANCE(t);
	module = PATH_INSTANCE_MODULE(instance);
	for (out = MODULE_SPECINST(module); out; out = TREE_CHAIN(out)) {
	    if (TREE_CODE(out) != PATH_OUTPUT) {
		continue;
	    }
	    for (constraint = PATH_OUTPUT_CONSTRAINTS(out);
		 constraint; constraint = TREE_CHAIN(constraint)) {
		if (constraint == t) {
		    gate = PATH_OUTPUT_GATE(out);
		    t = GATE_OUTPUT_LIST(gate);
		    goto done;
		}
	    }
	}
    }
  done:
    n = TREE_PURPOSE(t);
    if (n != NULL && STMT_SURROGATE_ATTR(n)) {
	n = TREE_CHAIN(n);
    }
    return ((handle) n);
}

/************************************************************************
 * acc_fetch_size							*
/***********************************************************************/

int acc_fetch_size(handle object)
{
    tree t = (tree) object;

    acc_error_flag = FALSE;

    if (PORT_REDEFINED_ATTR(t)) {
	return (TREE_NBITS(TREE_CHAIN(t)));
    }

    return TREE_NBITS(t);
}

/************************************************************************
 * acc_fetch_range							*
/***********************************************************************/

int acc_fetch_range(handle object, int *pMsb, int *pLsb)
{
    tree t = (tree) object;

    acc_error_flag = FALSE;
    switch (TREE_CODE(t)) {
    case NET_VECTOR_DECL:
    case REG_VECTOR_DECL:
    case PORT_VECTOR_DECL:
    case INTEGER_DECL:
	*pMsb = MSB(t);
	*pLsb = LSB(t);
	return TRUE;
    case TIME_DECL:
	*pMsb = 63;
	*pLsb = 0;
	break;
    case NET_SCALAR_DECL:
    case REG_SCALAR_DECL:
    case PORT_SCALAR_DECL:
	*pMsb = 0;
	*pLsb = 0;
	break;
    default:
	acc_error_flag = TRUE;
	return FALSE;
    }
}

/****************************************************************

	acc_handle_parent
		- return handle of parent of object

*****************************************************************/

handle acc_handle_parent(handle object)
{
    tree gate;
    tree timingCheck;
    tree decl;
    tree modpathEntity;


    acc_error_flag = FALSE;

    switch (TREE_CODE((tree) object)) {
    case PATH_CONSTRAINT:
	return ((handle) PATH_CONSTRAINT_INSTANCE((tree) object));
    case PATH_INSTANCE:
	return ((handle) PATH_INSTANCE_MODULE((tree) object));
    case GATE_INSTANCE:
	return ((handle) GATE_SCOPE((tree) object));
    case TIMING_CHECK:
	return ((handle) TIMING_CHECK_MODULE((tree) object));
    case TREE_LIST:
	/* check for primitive terminal */
	gate = GATE_TERMINAL_GATE((tree) object);
	if (gate != NULL && TREE_CODE(gate) == GATE_INSTANCE) {
	    return ((handle) gate);
	}
	timingCheck = TIMING_TERMINAL_CHECK((tree) object);
	if (timingCheck != NULL && TREE_CODE(timingCheck) == TIMING_CHECK) {
	    return (handle) TIMING_CHECK_MODULE(timingCheck);
	}
	modpathEntity = MODPATH_INPUT_TERMINAL_PARENT((tree) object);
	if (modpathEntity != NULL &&
	    TREE_CODE(modpathEntity) == PATH_CONSTRAINT) {
	    return (handle) PATH_CONSTRAINT_INSTANCE(modpathEntity);
	}
	/* check for a port terminal */
	decl = TREE_PURPOSE((tree) object);
	if ((TREE_CODE(decl) == NET_SCALAR_DECL ||
	     TREE_CODE(decl) == NET_VECTOR_DECL) &&
	    (PORT_INPUT_ATTR(decl) || PORT_OUTPUT_ATTR(decl))) {
	    return ((handle) DECL_CONTEXT(decl));
	}
	break;
    }

    switch (*tree_code_type[TREE_CODE((tree) object)]) {
    case 'd':			/* decls */
	return ((handle) DECL_CONTEXT((tree) object));
    case 'b':			/* blocks */
	return ((handle) BLOCK_UP((tree) object));
    case 's':			/* statements */
	return ((handle) STMT_SCOPE((tree) object));
    case 'g':
	return ((handle) GATE_SCOPE((tree) object));
    default:
	acc_error_flag = TRUE;
	return NULL;
    }
}

/****************************************************************

	acc_next_port
		- return the next port of module 

*****************************************************************/

handle acc_next_port(handle module, handle port)
{

    acc_error_flag = FALSE;

    if (port == NULL && acc_fetch_type(module) == accModule) {
	return (handle) MODULE_PORT_LIST((tree) module);
    } else if (port == NULL) {
	return (handle) BLOCK_PORTS((tree) module);
    }

    return (handle) TREE_CHAIN((tree) port);
}

/****************************************************************

	acc_handle_port

*****************************************************************/

handle acc_handle_port(handle module, int argumentNumber)
{
    int i;
    tree port;

    acc_error_flag = FALSE;

    if (acc_fetch_type(module) == accModule) {
	port = MODULE_PORT_LIST((tree) module);
    } else {
	port = BLOCK_PORTS((tree) module);
    }
    for (i = 0; i < argumentNumber; i++) {
	port = TREE_CHAIN(port);
	if (port == NULL) {
	    return NULL;
	}
    }

    return (handle) port;
}

/****************************************************************

	acc_next_net

*****************************************************************/
handle acc_next_net(handle reference, handle net)
{
    handle object;
    handle port;

    acc_error_flag = FALSE;

    object = net;
    while (TRUE) {
	if (object == NULL) {
	    object = acc_next_port(reference, object);
	    if (object != NULL) {
		object = acc_handle_loconn(object);
	    } else {
		object = (handle) BLOCK_DECL((tree) reference);
		while (TRUE) {
		    if (object == NULL
			|| (!PORT_INPUT_ATTR((tree) object)
			    && !PORT_OUTPUT_ATTR((tree) object))) {
			break;
		    }
		    object = (handle) TREE_CHAIN((tree) object);
		}
	    }
	} else {
	    if (PORT_OUTPUT_ATTR((tree) object) ||
		PORT_INPUT_ATTR((tree) object)) {
		port = NULL;
		while (TRUE) {
		    port = acc_next_port(reference, port);
		    if (port == NULL) {
			break;
		    }
		    if (acc_handle_loconn(port) == object) {
			break;
		    }
		}
		if (port != NULL) {
		    port = acc_next_port(reference, port);
		}
		if (port != NULL) {
		    object = acc_handle_loconn(port);
		} else {
		    object = (handle) BLOCK_DECL((tree) reference);
		    while (TRUE) {
			if (object == NULL
			    || (!PORT_INPUT_ATTR((tree) object)
				&& !PORT_OUTPUT_ATTR((tree) object))) {
			    break;
			}
			object = (handle) TREE_CHAIN((tree) object);
		    }
		}
	    } else {
		object = (handle) TREE_CHAIN((tree) object);
		while (TRUE) {
		    if (object == NULL
			|| (!PORT_INPUT_ATTR((tree) object)
			    && !PORT_OUTPUT_ATTR((tree) object))) {
			break;
		    }
		    object = (handle) TREE_CHAIN((tree) object);
		}
	    }
	}
	if (object == NULL || acc_fetch_type(object) == accNet) {
	    break;
	}
    }
    return (object);
}

/************************************************************************
 * 			
 * 		tf_isizep
 *		tf_sizep	
 * 			
/***********************************************************************/

int tf_sizep(int nparam)
{
    return (tf_isizep(nparam, tf_getinstance()));
}

int tf_isizep(int nparam, char *instance)
{
    tree arg;
    int nbits;
    tree node;

    arg = nth_parameter(nparam, (tree) instance);
    if (arg == NULL) {
	return (0);
    }


    node = TREE_PURPOSE(arg);
    switch (TREE_CODE(node)) {
    case BIT_CST:
	nbits = BIT_CST_NBITS(node);
	switch (BIT_CST_RADIX(node)) {
	case EXP_:
	case EXPFLOAT_:
	case FLOAT_:
	    nbits = 0;
	    break;
	case STRING_:
	    nbits = nbits / 8;
	    break;
	default:
	    // do nothing
	    break;
	}
	return (nbits);

    case INTEGER_DECL:
    case REAL_DECL:
    case REG_SCALAR_DECL:
    case REG_VECTOR_DECL:
    case NET_SCALAR_DECL:
    case NET_VECTOR_DECL:
    case PARAM_DECL:
    case SPECPARAM_DECL:
	return (TREE_NBITS(node));
    }

    return (TREE_NBITS(*TREE_EXPR_CODE(arg)));
}

/************************************************************************
 * 			
 * 		tf_iasynchon
 *		tf_asynchon	
 * 			
/***********************************************************************/

static int driverCallback(p_vc_record record)
{
    PliInfo_t *info = (PliInfo_t *) record->user_data;

    if (!ASYNCH_ATTR(info->instance)) {
	return 0;
    }
    switch (TREE_CODE(info->instance)) {
    case SYSFUNCTION_REF:
	call_misc_tf1(FUNC_REF_USERTF(info->instance),
		      reason_paramdrc, info->argNumber);
	break;
    case SYSTASK_STMT:
	call_misc_tf1(STMT_SYSTASK_USERTF(info->instance),
		      reason_paramdrc, info->argNumber);
	break;
    }
    return 0;
}

void tf_iasynchon(char *instance)
{
    tree node = (tree) instance;
    int i;
    int argumentCount;
    handle net;
    handle driver;
    tree terminal;
    PliInfo_t *info;


    ASYNCH_ATTR(node) = TRUE;

    // look for value changes on all drivers of nets 
    // connected to task


    argumentCount = tf_inump(instance);
    for (i = 1; i <= argumentCount; i++) {
	net = acc_handle_tfarg(i);
	if (net == NULL || acc_fetch_type(net) != accNet) {
	    continue;
	}
	driver = null;
	while (TRUE) {
	    driver = acc_next_driver(net, driver);
	    if (driver == null) {
		break;
	    }
	    terminal = nth_parameter(i, node);
	    info = (PliInfo_t *) TREE_PLIINFO(terminal);
	    if (info->vclSet) {
		break;
	    }
	    acc_vcl_add(driver, driverCallback, (char *) info,
			vcl_verilog_logic);
	}
	info->vclSet = TRUE;
    }
    return;
}

void tf_asynchon()
{
    tf_iasynchon((char *) tf_getinstance());
    return;
}

/************************************************************************
 * 			
 * 		tf_iasynchoff
 *		tf_asynchoff	
 * 			
/***********************************************************************/

void tf_iasynchoff(char *instance)
{
    tree node = (tree) instance;

    ASYNCH_ATTR(node) = FALSE;
    return;
}

void tf_asynchoff()
{
    tf_iasynchoff((char *) tf_getinstance());
    return;
}

/************************************************************************
 * io_printf								*
/***********************************************************************/

void io_printf(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf_V(fmt, args);
    va_end(args);
    return;
}

/**************************************************************************/
/* tf_getinstance                                                       */
/**************************************************************************/

char *tf_getinstance(void)
{
    return (char *) current_tf_instance;
}

/************************************************************************
 * tf_synchronize							*
 * tf_isynchronize							*
 * tf_rosynchronize							*
 * tf_irosynchronize							*
 *  Add an item to either the idle queue (synchronize) or to the	*
 *  final queue (rosynchronize).  Later, add flags to send error	*
 *  messages is certain PLI routines are called in violation of the	*
 *  rules (i.e., can't call tf_strdelputp, etc on reason_rosynch).	*
 ***********************************************************************/

void tf_synchronize(void)
{
    tf_isynchronize(tf_getinstance());
    return;
}

void tf_isynchronize(char *instance)
{
    tickle_monitor(&idle_strobe_queue, (tree) instance);
    return;
}

void tf_rosynchronize(void)
{
    tf_irosynchronize(tf_getinstance());
    return;
}

void tf_irosynchronize(char *instance)
{
    tickle_monitor(&final_strobe_queue, (tree) instance);
    return;
}

/************************************************************************
 * tf_nump								*
 * Return the number of (possibly null) arguments			*
/***********************************************************************/

int tf_nump(void)
{
    return tf_inump(tf_getinstance());
}

int tf_inump(char *instance)
{
    return count_args((tree) instance);
}

/****************************************************************

	tf_mipname	
	tf_imipname
		- return  the pathname of the module containing the current 
			instance

*****************************************************************/

char *tf_mipname(void)
{
    return (tf_imipname(tf_getinstance()));
}

char *tf_imipname(char *instance)
{
    handle statement;
    handle parent;

    statement = (handle) instance;
    parent = acc_handle_parent(statement);
    return (acc_fetch_fullname((handle) parent));
}

/****************************************************************

	tf_itypep
	tf_typep
		- return the type of the given argument 

*****************************************************************/

int tf_typep(int argnum)
{
    return (tf_itypep(argnum, tf_getinstance()));
}

int tf_itypep(int argnum, char *instance)
{
    tree arg;

    arg = nth_arg(argnum, (tree) instance);

    if (arg == NULL_TREE) {
	return (tf_nullparam);
    }


    switch (TREE_CODE(arg)) {
    case BIT_CST:
	switch (BIT_CST_RADIX(arg)) {
	case STRING_:
	    return (tf_string);
	case EXP_:
	case FLOAT_:
	case EXPFLOAT_:
	    return (tf_readonlyreal);
	default:
	    return (tf_readonly);
	}
	break;
    case REAL_CST:
	return (tf_readonlyreal);
    case STRING_CST:
	return (tf_string);
    case INTEGER_CST:
	return (tf_readonly);
    case REAL_DECL:
	return (tf_readwritereal);
    case NET_SCALAR_DECL:
    case NET_VECTOR_DECL:
	return (tf_readonly);
    case INTEGER_DECL:
    case REG_SCALAR_DECL:
    case REG_VECTOR_DECL:
	if (PORT_INPUT_ATTR(arg)) {
	    return (tf_readonly);
	} else {
	    return (tf_readwrite);
	}
    case BIT_REF:
	return (tf_rwbitselect);
    case PART_REF:
	return (tf_rwpartselect);
    default:
	if (TREE_REAL_ATTR(arg)) {
	    return (tf_readonlyreal);
	} else {
	    return (tf_readonly);
	}
    }

    return (0);
}

/****************************************************************

	tf_gettimeprecision	
	tf_igettimeprecision	
		- return time precision of instance 

*****************************************************************/

int tf_gettimeprecision()
{
    return (tf_igettimeprecision(tf_getinstance()));
}

int tf_igettimeprecision(char *instance)
{
    int prec;
    int unit;

    if (instance == null) {
	return (-get_tick_precision());
    }
    timescale_get((tree) acc_handle_parent((handle) instance), &unit,
		  &prec);
    return (-prec);
}

/****************************************************************

	tf_gettimeunit	
	tf_igettimeunit
		- return time units of instance 

*****************************************************************/

int tf_gettimeunit()
{
    return (tf_igettimeunit(tf_getinstance()));
}

int tf_igettimeunit(char *instance)
{
    int prec;
    int unit;

    timescale_get((tree) acc_handle_parent((handle) instance), &unit,
		  &prec);
    return (-unit);
}

/****************************************************************

	tf_finish
	tf_dofinish

*****************************************************************/
void tf_finish()
{
    do_finish();
}

void tf_dofinish()
{
    tf_finish();
}

/****************************************************************

	tf_stop
	tf_dostop

*****************************************************************/
void tf_stop()
{
    do_stop();
}

void tf_dostop()
{
    tf_stop();
}

/*****************************************************************
	acc_fetch_timescale_info
******************************************************************/

void acc_fetch_timescale_info(handle object, p_timescale_info p_timescale)
{
    int unit;
    int precision;

/* make sure object points to a module */

    if (object != NULL && acc_fetch_type(object) != accModule) {
	object = acc_handle_parent(object);
    }
    timescale_get((tree) object, &unit, &precision);
    p_timescale->unit = -unit;
    p_timescale->precision = -precision;
}

/*****************************************************************
	acc_fetch_precision
******************************************************************/

int acc_fetch_precision()
{
    return -get_tick_precision();
}

/****************************************************************

	acc_fetch_defname
		- return defining name of module or primitive  

*****************************************************************/

char *acc_fetch_defname(handle object)
{
    char *name;
    tree t = (tree) object;

    acc_error_flag = FALSE;

    switch (TREE_CODE(t)) {
    case MODULE_BLOCK:
	return (MODULE_NAME(t));
    case GATE_INSTANCE:
	switch (TREE_TYPE(t)) {
	case GATE_AND_TYPE:
	    name = "and";
	    break;
	case GATE_BUFIF0_TYPE:
	    name = "bufif0";
	    break;
	case GATE_BUFIF1_TYPE:
	    name = "bufif1";
	    break;
	case GATE_BUF_TYPE:
	    name = "buf";
	    break;
	case GATE_CMOS_TYPE:
	    name = "cmos";
	    break;
	case GATE_NAND_TYPE:
	    name = "nand";
	    break;
	case GATE_NMOS_TYPE:
	    name = "nmos";
	    break;
	case GATE_NOR_TYPE:
	    name = "nor";
	    break;
	case GATE_NOTIF0_TYPE:
	    name = "notif0";
	    break;
	case GATE_NOTIF1_TYPE:
	    name = "notif1";
	    break;
	case GATE_NOT_TYPE:
	    name = "not";
	    break;
	case GATE_OR_TYPE:
	    name = "or";
	    break;
	case GATE_PMOS_TYPE:
	    name = "pmos";
	    break;
	case GATE_PULLDN_TYPE:
	    name = "pulldown";
	    break;
	case GATE_PULLUP_TYPE:
	    name = "pullup";
	    break;
	case GATE_RCMOS_TYPE:
	    name = "rcmos";
	    break;
	case GATE_RNMOS_TYPE:
	    name = "rnmos";
	    break;
	case GATE_RPMOS_TYPE:
	    name = "rpmos";
	    break;
	case GATE_RTRANIF0_TYPE:
	    name = "rtranif0";
	    break;
	case GATE_RTRANIF1_TYPE:
	    name = "rtranif1";
	    break;
	case GATE_RTRAN_TYPE:
	    name = "rtran";
	    break;
	case GATE_TRANIF0_TYPE:
	    name = "tranif0";
	    break;
	case GATE_TRANIF1_TYPE:
	    name = "tranif1";
	    break;
	case GATE_TRAN_TYPE:
	    name = "tran";
	    break;
	case GATE_UDP_TYPE:
	    name = UDP_NAME(GATE_UDP_DEF(t));
	    break;
	case GATE_XNOR_TYPE:
	    name = "xnor";
	    break;
	case GATE_XOR_TYPE:
	    name = "xor";
	    break;
	}
	return (name);
    }
    acc_error_flag = TRUE;
    return (NULL);
}

/****************************************************************

	acc_fetch_type_str
		- add delays for given object 

*****************************************************************/

char *acc_fetch_type_str(int type)
{
    char *string;

    acc_error_flag = FALSE;
    switch (type) {
    case accModule:
	string = "accModule";
	break;
    case accScope:
	string = "accScope";
	break;
    case accNet:
	string = "accNet";
	break;
    case accRegister:
	string = "accRegister";
	break;
    case accPort:
	string = "accPort";
	break;
    case accTerminal:
	string = "accTerminal";
	break;
    case accInputTerminal:
	string = "accInputTerminal";
	break;
    case accOutputTerminal:
	string = "accOutputTerminal";
	break;
    case accInoutTerminal:
	string = "accInoutTerminal";
	break;
    case accCombPrim:
	string = "accCombPrim";
	break;
    case accSeqPrim:
	string = "accSeqPrim";
	break;
    case accAndGate:
	string = "accAndGate";
	break;
    case accNandGate:
	string = "accNandGate";
	break;
    case accNorGate:
	string = "accNorGate";
	break;
    case accOrGate:
	string = "accOrGate";
	break;
    case accXorGate:
	string = "accXorGate";
	break;
    case accXnorGate:
	string = "accXnorGate";
	break;
    case accBufGate:
	string = "accBufGate";
	break;
    case accNotGate:
	string = "accNotGate";
	break;
    case accBufif0Gate:
	string = "accBufif0Gate";
	break;
    case accBufif1Gate:
	string = "accBufif1Gate";
	break;
    case accNotif0Gate:
	string = "accNotif0Gate";
	break;
    case accNotif1Gate:
	string = "accNotif1Gate";
	break;
    case accNmosGate:
	string = "accNmosGate";
	break;
    case accPmosGate:
	string = "accPmosGate";
	break;
    case accCmosGate:
	string = "accCmosGate";
	break;
    case accRnmosGate:
	string = "accRnmosGate";
	break;
    case accRpmosGate:
	string = "accRpmosGate";
	break;
    case accRcmosGate:
	string = "accRcmosGate";
	break;
    case accRtranGate:
	string = "accRtranGate";
	break;
    case accRtranif0Gate:
	string = "accRtranif0Gate";
	break;
    case accRtranif1Gate:
	string = "accRtranif1Gate";
	break;
    case accTranGate:
	string = "accTranGate";
	break;
    case accTranif0Gate:
	string = "accTranif0Gate";
	break;
    case accTranif1Gate:
	string = "accTranif1Gate";
	break;
    case accPullupGate:
	string = "accPullupGate";
	break;
    case accPulldownGate:
	string = "accPulldownGate";
	break;
    case accIntegerParam:
	string = "accIntegerParam";
	break;
    case accRealParam:
	string = "accRealParam";
	break;
    case accStringParam:
	string = "accStringParam";
	break;
    case accPath:
	string = "accPath";
	break;
    case accTchk:
	string = "accTchk";
	break;
    case accPrimitive:
	string = "accPrimitive";
	break;
    case accBit:
	string = "accBit";
	break;
    case accPortBit:
	string = "accPortBit";
	break;
    case accNetBit:
	string = "accNetBit";
	break;
    case accRegBit:
	string = "accRegBit";
	break;
    case accParameter:
	string = "accParameter";
	break;
    case accSpecparam:
	string = "accSpecparam";
	break;
    case accTopModule:
	string = "accTopModule";
	break;
    case accModuleInstance:
	string = "accModuleInstance";
	break;
    case accCellInstance:
	string = "accCellInstance";
	break;
    case accModPath:
	string = "accModPath";
	break;
    case accPrimPath:
	string = "accPrimPath";
	break;
    case accWirePath:
	string = "accWirePath";
	break;
    case accInterModPath:
	string = "accInterModPath";
	break;
    case accTermPath:
	string = "accTermPath";
	break;
    case accModTermPath:
	string = "accModTermPath";
	break;
    case accTermModPath:
	string = "accTermModPath";
	break;
    case accScalarPort:
	string = "accScalarPort";
	break;
    case accBitSelectPort:
	string = "accBitSelectPort";
	break;
    case accPartSelectPort:
	string = "accPartSelectPort";
	break;
    case accVectorPort:
	string = "accVectorPort";
	break;
    case accConcatPort:
	string = "accConcatPort";
	break;
    case accWire:
	string = "accWire";
	break;
    case accWand:
	string = "accWand";
	break;
    case accWor:
	string = "accWor";
	break;
    case accTri:
	string = "accTri";
	break;
    case accTriand:
	string = "accTriand";
	break;
    case accTrior:
	string = "accTrior";
	break;
    case accTri0:
	string = "accTri0";
	break;
    case accTri1:
	string = "accTri1";
	break;
    case accTrireg:
	string = "accTrireg";
	break;
    case accSupply0:
	string = "accSupply0";
	break;
    case accSupply1:
	string = "accSupply1";
	break;
    case accNamedEvent:
	string = "accNamedEvent";
	break;
    case accIntegerVar:
	string = "accIntegerVar";
	break;
    case accRealVar:
	string = "accRealVar";
	break;
    case accTimeVar:
	string = "accTimeVar";
	break;
    case accScalar:
	string = "accScalar";
	break;
    case accVector:
	string = "accVector";
	break;
    case accCollapsedNet:
	string = "accCollapsedNet";
	break;
    case accExpandedVector:
	string = "accExpandedVector";
	break;
    case accProtected:
	string = "accProtected";
	break;
    case accVlogSimPath:
	string = "accVlogSimPath";
	break;
    case accExpandedPath:
	string = "accExpandedPath";
	break;
    case accSwXlInvisibleNet:
	string = "accSwXlInvisibleNet";
	break;
    case accAcceleratedNet:
	string = "accAcceleratedNet";
	break;
    case accSetup:
	string = "accSetup";
	break;
    case accHold:
	string = "accHold";
	break;
    case accWidth:
	string = "accWidth";
	break;
    case accPeriod:
	string = "accPeriod";
	break;
    case accRecovery:
	string = "accRecovery";
	break;
    case accSkew:
	string = "accSkew";
	break;
    case accNoChange:
	string = "accNoChange";
	break;
    case accSetuphold:
	string = "accSetuphold";
	break;
    case accInput:
	string = "accInput";
	break;
    case accOutput:
	string = "accOutput";
	break;
    case accInout:
	string = "accInout";
	break;
    case accPositive:
	string = "accPositive";
	break;
    case accNegative:
	string = "accNegative";
	break;
    case accUnknown:
	string = "accUnknown";
	break;
    case accPathTerminal:
	string = "accPathTerminal";
	break;
    case accPathInput:
	string = "accPathInput";
	break;
    case accPathOutput:
	string = "accPathOutput";
	break;
    case accDataPath:
	string = "accDataPath";
	break;
    case accTchkTerminal:
	string = "accTchkTerminal";
	break;
    case accBitSelect:
	string = "accBitSelect";
	break;
    case accPartSelect:
	string = "accPartSelect";
	break;
    case accTask:
	string = "accTask";
	break;
    case accFunction:
	string = "accFunction";
	break;
    case accStatement:
	string = "accStatement";
	break;
    case accTaskCall:
	string = "accTaskCall";
	break;
    case accFunctionCall:
	string = "accFunctionCall";
	break;
    case accSystemTask:
	string = "accSystemTask";
	break;
    case accSystemFunction:
	string = "accSystemFunction";
	break;
    case accSystemRealFunction:
	string = "accSystemRealFunction";
	break;
    case accUserTask:
	string = "accUserTask";
	break;
    case accUserFunction:
	string = "accUserFunction";
	break;
    case accUserRealFunction:
	string = "accUserRealFunction";
	break;
    case accAssignmentStat:
	string = "accAssignmentStat";
	break;
    case accContAssignStat:
	string = "accContAssignStat";
	break;
    case accNullStat:
	string = "accNullStat";
	break;
    case accDelayStat:
	string = "accDelayStat";
	break;
    case accAssignDelayStat:
	string = "accAssignDelayStat";
	break;
    case accRtlDelayStat:
	string = "accRtlDelayStat";
	break;
    case accAssignEventStat:
	string = "accAssignEventStat";
	break;
    case accAssignMultiStat:
	string = "accAssignMultiStat";
	break;
    case accRtlEventStat:
	string = "accRtlEventStat";
	break;
    case accRtlMultiStat:
	string = "accRtlMultiStat";
	break;
    case accGenEventStat:
	string = "accGenEventStat";
	break;
    case accDisableStat:
	string = "accDisableStat";
	break;
    case accAssignStat:
	string = "accAssignStat";
	break;
    case accDeassignStat:
	string = "accDeassignStat";
	break;
    case accForceStat:
	string = "accForceStat";
	break;
    case accReleaseStat:
	string = "accReleaseStat";
	break;
    case accInitialStat:
	string = "accInitialStat";
	break;
    case accAlwaysStat:
	string = "accAlwaysStat";
	break;
    case accAtEventStat:
	string = "accAtEventStat";
	break;
    case accUnnamedBeginStat:
	string = "accUnnamedBeginStat";
	break;
    case accNamedBeginStat:
	string = "accNamedBeginStat";
	break;
    case accUnnamedForkStat:
	string = "accUnnamedForkStat";
	break;
    case accNamedForkStat:
	string = "accNamedForkStat";
	break;
    case accIfStat:
	string = "accIfStat";
	break;
    case accCaseStat:
	string = "accCaseStat";
	break;
    case accCaseZStat:
	string = "accCaseZStat";
	break;
    case accCaseXStat:
	string = "accCaseXStat";
	break;
    case accForeverStat:
	string = "accForeverStat";
	break;
    case accRepeatStat:
	string = "accRepeatStat";
	break;
    case accWhileStat:
	string = "accWhileStat";
	break;
    case accForStat:
	string = "accForStat";
	break;
    case accWaitStat:
	string = "accWaitStat";
	break;
    case accConstant:
	string = "accConstant";
	break;
    case accConcat:
	string = "accConcat";
	break;
    case accOperator:
	string = "accOperator";
	break;
    default:
	acc_error_flag = TRUE;
	string = NULL;
    }
    return (string);
}


/************************************************************************
 * acc_fetch_type							*
/***********************************************************************/

int acc_fetch_type(handle object)
{
    tree gate;
    tree timingCheck;
    tree decl;
    tree modpathEntity;

    acc_error_flag = FALSE;

    // missing things:
    // accDataPath
    // accNetBit - no supported
    // accOperator
    // accPathTerminal
    // accPortBit
    // accRegBit

    switch (TREE_CODE((tree) object)) {
    case SYSTASK_STMT:
	return accUserTask;
    case SYSFUNCTION_REF:
	if (((p_tfcell) FUNC_REF_USERTF((tree) object))->type ==
	    USERFUNCTION) {
	    return accUserFunction;
	} else {
	    return accUserRealFunction;
	}
    case PATH_CONSTRAINT:
	return accPathTerminal;
    case TREE_LIST:
	gate = GATE_TERMINAL_GATE((tree) object);
	if (gate != NULL && TREE_CODE(gate) == GATE_INSTANCE) {
	    return accTerminal;
	}
	timingCheck = TIMING_TERMINAL_CHECK((tree) object);
	if (timingCheck != NULL && TREE_CODE(timingCheck) == TIMING_CHECK) {
	    return accTchkTerminal;
	}
	modpathEntity = MODPATH_INPUT_TERMINAL_PARENT((tree) object);
	if (modpathEntity != NULL
	    && TREE_CODE(modpathEntity) == PATH_CONSTRAINT) {
	    return accPathTerminal;
	}
	/* check for a port terminal */
	decl = TREE_PURPOSE((tree) object);
	if ((TREE_CODE(decl) == NET_SCALAR_DECL ||
	     TREE_CODE(decl) == NET_VECTOR_DECL) &&
	    (PORT_INPUT_ATTR(decl) || PORT_OUTPUT_ATTR(decl))) {
	    return (accPort);
	}
	acc_error_flag = TRUE;
	return (0);

    case BIT_CST:
	return accConstant;
    case EVENT_DECL:
	return accNamedEvent;
    case FUNCTION_BLOCK:
	return accFunction;
    case INTEGER_DECL:
	return accIntegerVar;
    case MODULE_BLOCK:
	return accModule;
    case NET_VECTOR_DECL:
    case NET_SCALAR_DECL:
	return accNet;
    case PARAM_DECL:
	return accParameter;

    case PART_REF:
	return (accPartSelect);
    case PATH_INSTANCE:
	return accPath;
    case REAL_DECL:
	return accRealVar;
    case REG_SCALAR_DECL:
    case REG_VECTOR_DECL:
	return accReg;

    case SPECPARAM_DECL:
	return accSpecparam;
    case TASK_BLOCK:
	return accTask;
    case TIMING_CHECK:
	return accTchk;
    case TIME_DECL:
	return accTimeVar;

    case GATE_INSTANCE:
	return accPrimitive;

    default:
	if (*tree_code_type[TREE_CODE((tree) object)] == 's') {
	    return accStatement;
	} else if (*tree_code_type[TREE_CODE((tree) object)] == 'g') {
	    return (accPrimitive);
	} else {
	    acc_error_flag = TRUE;
	    return 0;
	}
    }
}

 /****************************************************************

	acc_fetch_fulltype
		- return the fulltype of the given object 

*****************************************************************/

int acc_fetch_fulltype(handle object)
{
    int type;
    tree t = (tree) object;
    tree gate;
    tree decl;
    tree modpathEntity;

    // full types not supported:
    // accCellInstance/accCellModule
    // accNet enumerations
    // accParameter enumerations
    // accPort enumerations
    // accSpecparam enumerations
    // accStatement enumerations
    // accNoChange
    //
    acc_error_flag = FALSE;
    type = 0;

    switch (TREE_CODE(t)) {
    case PATH_INSTANCE:
	return accModPath;
    case PATH_CONSTRAINT:
	return accPathOutput;
    case TREE_LIST:
	gate = GATE_TERMINAL_GATE(t);
	if (gate != NULL && TREE_CODE(gate) == GATE_INSTANCE) {
	    if (PORT_INPUT_ATTR(t) && PORT_OUTPUT_ATTR(t)) {
		type = accInoutTerminal;
	    } else if (PORT_INPUT_ATTR(t)) {
		type = accInputTerminal;
	    } else if (PORT_OUTPUT_ATTR(t)) {
		type = accOutputTerminal;
	    }
	    break;
	}
	modpathEntity = MODPATH_INPUT_TERMINAL_PARENT((tree) object);
	if (modpathEntity != NULL &&
	    TREE_CODE(modpathEntity) == PATH_CONSTRAINT) {
	    return accPathInput;
	}
	/* must be a port */
	decl = TREE_PURPOSE(t);
	if (PORT_INPUT_ATTR(decl) || PORT_OUTPUT_ATTR(decl)) {
	    if (TREE_NBITS(decl) == 1) {
		type = accScalarPort;
	    } else {
		type = accVectorPort;
	    }
	}
	break;

    case MODULE_BLOCK:
	if (BLOCK_UP(t) != NULL_TREE) {
	    type = accModuleInstance;
	} else {
	    type = accTopModule;
	}
	break;

    case GATE_INSTANCE:

	switch (TREE_TYPE(t)) {
	case GATE_AND_TYPE:
	    type = accAndGate;
	    break;
	case GATE_BUFIF0_TYPE:
	    type = accBufif0Gate;
	    break;
	case GATE_BUFIF1_TYPE:
	    type = accBufif1Gate;
	    break;
	case GATE_BUF_TYPE:
	    type = accBufGate;
	    break;
	case GATE_CMOS_TYPE:
	    type = accCmosGate;
	    break;
	case GATE_NAND_TYPE:
	    type = accNandGate;
	    break;
	case GATE_NMOS_TYPE:
	    type = accNmosGate;
	    break;
	case GATE_NOR_TYPE:
	    type = accNorGate;
	    break;
	case GATE_NOTIF0_TYPE:
	    type = accNotif0Gate;
	    break;
	case GATE_NOTIF1_TYPE:
	    type = accNotif1Gate;
	    break;
	case GATE_NOT_TYPE:
	    type = accNotGate;
	    break;
	case GATE_OR_TYPE:
	    type = accOrGate;
	    break;
	case GATE_PMOS_TYPE:
	    type = accPmosGate;
	    break;
	case GATE_PULLDN_TYPE:
	    type = accPulldownGate;
	    break;
	case GATE_PULLUP_TYPE:
	    type = accPullupGate;
	    break;
	case GATE_RCMOS_TYPE:
	    type = accRcmosGate;
	    break;
	case GATE_RNMOS_TYPE:
	    type = accRnmosGate;
	    break;
	case GATE_RPMOS_TYPE:
	    type = accRpmosGate;
	    break;
	case GATE_RTRANIF0_TYPE:
	    type = accRtranif0Gate;
	    break;
	case GATE_RTRANIF1_TYPE:
	    type = accRtranif1Gate;
	    break;
	case GATE_RTRAN_TYPE:
	    type = accRtranGate;
	    break;
	case GATE_TRANIF0_TYPE:
	    type = accTranif0Gate;
	    break;
	case GATE_TRANIF1_TYPE:
	    type = accTranif1Gate;
	    break;
	case GATE_TRAN_TYPE:
	    type = accTranGate;
	    break;
	case GATE_UDP_TYPE:
	    {
		tree udpDef = GATE_UDP_DEF(t);
		ASSERT(udpDef != NULL_TREE);
		if (UDP_REG_NAME(udpDef)) {
		    type = accSeqPrim;
		} else {
		    type = accCombPrim;
		}
	    }
	    break;
	case GATE_XNOR_TYPE:
	    type = accXnorGate;
	    break;
	case GATE_XOR_TYPE:
	    type = accXorGate;
	    break;
	}
	break;
    case TIMING_CHECK:
	{
	    tree check;
	    check = TIMING_CHECK_CHECKSPEC(t);
	    ASSERT(check != NULL_TREE);
	    switch (CHECK_SPEC_CHECKTYPE(check)) {
	    case SETUP:
		type = accSetup;
		break;
	    case HOLD:
		type = accHold;
		break;
	    case WIDTH:
		type = accWidth;
		break;
	    case PERIOD:
		type = accPeriod;
		break;
	    case SKEW:
		type = accSkew;
		break;
	    case RECOVERY:
		type = accRecovery;
		break;
	    case SETUPHOLD:
		type = accSetuphold;
		break;
	    default:
		ASSERT(FALSE);
	    }
	}
	break;
    default:
	type = acc_fetch_type(object);
	if (type == accNet) {
	    switch (TREE_TYPE((tree) object)) {
	    case NET_TRI_TYPE:
		return accTri;
	    case NET_WIRE_TYPE:
		return accWire;
	    case NET_TRIOR_TYPE:
		return accTrior;
	    case NET_WOR_TYPE:
		return accWor;
	    case NET_TRIAND_TYPE:
		return accTriand;
	    case NET_WAND_TYPE:
		return accWand;
	    case NET_TRIREG_TYPE:
		return accTrireg;
	    case NET_TRI0_TYPE:
		return accTri0;
	    case NET_TRI1_TYPE:
		return accTri1;
	    case NET_SUPPLY1_TYPE:
		return accSupply0;
	    case NET_SUPPLY0_TYPE:
		return accSupply1;
	    default:
		ASSERT(FALSE);
	    }
	} else if (type == accParameter) {
	    if (TREE_INTEGER_ATTR(DECL_PARAM_RVAL((tree) object))) {
		return (accIntegerParam);
	    } else if (TREE_REAL_ATTR(DECL_PARAM_RVAL((tree) object))) {
		return (accRealParam);
	    } else {
		return (accStringParam);
	    }
	}
    }
    return (type);
}

/******************************************************************************
	io_mcdprintf( int mcd, char* fmt, ... )
*******************************************************************************
*/

void io_mcdprintf(int handle, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf_V(handle, fmt, args);
    va_end(args);
}

/******************************************************************************
	tf_long_to_real
*******************************************************************************
*/

void tf_long_to_real(int low, int high, double *aof_real)
{
    double result;

    result = high;
    result = ldexp(result, 32);	/* shift by 32 */
    result += low;
    *aof_real = result;
}

/******************************************************************************
	tf_real_to_long
*******************************************************************************
*/

void tf_real_to_long(double realvalue, int *aof_int_lo, int *aof_int_hi)
{
    double y;
    const double pow2_32 = ldexp((double) 1, 32);

    y = realvalue + 0.5;	/* round to compensate for small errors in conversion */

    *aof_int_lo = (int) fmod(y, pow2_32);	/* mod 2^32 */
    *aof_int_hi = (int) ldexp(y, -32);	/* div 2^32 */
}

/******************************************************************************
	tf_scale_longdelay
*******************************************************************************
*/

void
tf_scale_longdelay(char *instance_p, int delay_lo, int delay_hi,
		   int *aof_delay_lo, int *aof_delay_hi)
{
    int factor;

    *aof_delay_lo = delay_lo;
    *aof_delay_hi = delay_hi;
    factor = (int) timescale_precision((tree)
				       acc_handle_parent((handle)
							 instance_p));
    tf_multiply_long(aof_delay_lo, aof_delay_hi, factor, 0);
}

/******************************************************************************
	tf_scale_realdelay
*******************************************************************************
*/

void
tf_scale_realdelay(char *instance_p, double realdelay,
		   double *aof_realdelay)
{
    *aof_realdelay = realdelay * timescale_precision((tree)
						     acc_handle_parent((handle) instance_p));
}

/******************************************************************************
	tf_unscale_longdelay
*******************************************************************************
*/

void
tf_unscale_longdelay(char *instance_p, int delay_lo, int delay_hi,
		     int *aof_delay_lo, int *aof_delay_hi)
{
    int factor;

    *aof_delay_lo = delay_lo;
    *aof_delay_hi = delay_hi;
    factor = (int) timescale_precision((tree)
				       acc_handle_parent((handle)
							 instance_p));
    tf_divide_long(aof_delay_lo, aof_delay_hi, factor, 0);
}

/******************************************************************************
	tf_unscale_realdelay
*******************************************************************************
*/

void
tf_unscale_realdelay(char *instance_p, double realdelay,
		     double *aof_realdelay)
{
    *aof_realdelay =
	realdelay /
	timescale_precision((tree) acc_handle_parent((handle) instance_p));
}

/******************************************************************************
	tf_longtime_tostr
*******************************************************************************
*/

char *tf_longtime_tostr(int lowtime, int hightime)
{
    time64 time;

    time.timeh = hightime;
    time.timel = lowtime;
    return (time_string(&time));
}

/**************************************************************************/
/* tf_gettime                                                       */
/**************************************************************************/

int tf_gettime()
{
    int timeh;

    return (tf_getlongtime(&timeh));
}


/**************************************************************************/
/* tf_getlongtime                                                       */
/* tf_igetlongtime                                                       */
/**************************************************************************/

int tf_igetlongtime(int *aof_hightime, char *instance)
{

/*
 * time is unscaled per vcs behavior
 */
/*
	int	timeh;
	int	timel;
	tf_unscale_longdelay( instance, CurrentTime.timel, CurrentTime.timeh,
		&timel, &timeh );
	*aof_hightime = timeh;
*/
    *aof_hightime = CurrentTime.timeh;
    return CurrentTime.timel;
}

int tf_getlongtime(int *aof_hightime)
{
    return tf_igetlongtime(aof_hightime, tf_getinstance());
}

/**************************************************************************/
/* tf_getrealtime                                                       */
/**************************************************************************/

double tf_getrealtime(void)
{
    double result;


    tf_long_to_real(CurrentTime.timel, CurrentTime.timeh, &result);
    return (result);
}

/**************************************************************************/
/* tf_strgettime                                                       */
/**************************************************************************/

char *tf_strgettime()
{
    time64 time;

    time.timel = tf_getlongtime((int *) &time.timeh);
    return time_string(&time);
}

/************************************************************************
  mc_scan_plusargs                            
*************************************************************************
*/

char *mc_scan_plusargs(char *startarg)
{
    tree t;

    for (t = plusargs; t; t = TREE_CHAIN(t)) {
	if (!strncmp(startarg, (char *) TREE_PURPOSE(t), strlen(startarg))) {
	    return ((char *) TREE_PURPOSE(t) + strlen(startarg));
	}
    }
    return (NULL);
}

/****************************************************************
	tf_propagate
	tf_ipropagate
*****************************************************************/

void tf_propagatep(int nparam)
{
    tf_ipropagatep(nparam, tf_getinstance());
}

void tf_ipropagatep(int nparam, char *instance)
{
    tree arg = nth_arg(nparam, (tree) instance);
    tree param = nth_parameter(nparam, (tree) instance);
    int numbits;
    int ngroups;
    PliInfo_t *info;

    numbits = TREE_NBITS(*TREE_EXPR_CODE(param));
    ngroups = (numbits - 1) / 32 + 1;
    info = (PliInfo_t *) TREE_PLIINFO(param);
    ASSERT(info != NULL);

    if (info->storage == NULL) {
	info->storage = (Group *) malloc_X(ngroups);
    }

    memcpy(DECL_STORAGE(arg), info->storage, sizeof(Group) * ngroups);
    notify(arg, DECL_STATE(arg), 1, NULL);
}

/****************************************************************
	tf_evaluatep
	tf_ievaluatep
*****************************************************************/
void tf_evaluatep(int nparam)
{
    tf_ievaluatep(nparam, tf_getinstance());
}

void tf_ievaluatep(int nparam, char *instance)
{
    tree arg;
    Group *g;
    int numbits;
    int ngroups;
    PliInfo_t *info;

    arg = nth_parameter(nparam, (tree) instance);
    if (arg == NULL) {
	return;
    }
    info = (PliInfo_t *) TREE_PLIINFO(arg);
    ASSERT(info != NULL);

    numbits = TREE_NBITS(*TREE_EXPR_CODE(arg));
    ngroups = (numbits - 1) / 32 + 1;
    eval(TREE_EXPR_CODE(arg));
    g = *--R;

    if (info->storage == NULL) {
	info->storage = (Group *) malloc_X(ngroups);
    }

    memcpy(info->storage, g, ngroups * sizeof(Group));
}

/****************************************************************

	tf_exprinfo
	tf_iexprinfo

*****************************************************************/

struct t_tfexprinfo *tf_exprinfo(int nparam,
				 struct t_tfexprinfo *exprinfo_p)
{
    return (tf_iexprinfo(nparam, exprinfo_p, tf_getinstance()));
}

struct t_tfexprinfo *tf_iexprinfo(int nparam,
				  struct t_tfexprinfo *exprinfo_p,
				  char *instance)
{
    int type;
    tree arg;
    tree param;
    int bits;
    PliInfo_t *info;

    tf_ievaluatep(nparam, instance);
    param = nth_parameter(nparam, (tree) instance);
    if (param == NULL) {
	return (NULL);
    }
    arg = TREE_PURPOSE(param);
    info = (PliInfo_t *) TREE_PLIINFO(param);
    ASSERT(info != NULL);

    if (info->storage == NULL) {
	info->storage = (Group *) malloc_X(exprinfo_p->expr_ngroups);
    }

    type = tf_itypep(nparam, instance);
    bits = tf_isizep(nparam, instance);
    switch (type) {
    case tf_nullparam:
	break;
    case tf_string:
	exprinfo_p->expr_sign = 0;
	exprinfo_p->expr_vec_size = 0;
	exprinfo_p->expr_ngroups = 0;
	exprinfo_p->expr_value_p = 0;
	exprinfo_p->expr_string =
	    GroupToString(info->storage, bits * 8, (bits + 3) / 4);
	break;
    case tf_readonlyreal:
    case tf_readwritereal:
	exprinfo_p->expr_vec_size = 0;
	exprinfo_p->expr_ngroups = 0;
	exprinfo_p->real_value = REAL_(info->storage);
	exprinfo_p->expr_value_p = 0;
	if (exprinfo_p->real_value < 0) {
	    exprinfo_p->expr_sign = -1;
	} else {
	    exprinfo_p->expr_sign = 1;
	}
	break;
    case tf_readonly:
    case tf_readwrite:
    case tf_rwbitselect:
    case tf_rwpartselect:
	exprinfo_p->expr_sign = 0;	// this needs fixed
	exprinfo_p->expr_vec_size = bits;
	exprinfo_p->expr_ngroups = 1 + (bits - 1) / (8 * sizeof(int));
	exprinfo_p->expr_value_p = (struct t_vecval *) info->storage;
	break;
	break;
    case tf_rwmemselect:
    default:
	return (NULL);
    }

    exprinfo_p->expr_type = type;
    return (exprinfo_p);
}


/************************************************************************
 * tf_getp								
 * tf_igetp								
 * tf_putp								
 * tf_iputp								
 * tf_putlongp							
 * tf_iputlongp							
 * tf_getlongp							
 * tf_igetlongp							
 * tf_getrealp
 * tf_igetrealp
 * tf_putrealp
 * tf_iputrealp
 ***********************************************************************/

LP64 tf_getp(int nparam)
{
    return tf_igetp(nparam, tf_getinstance());
}

LP64 tf_igetp(int nparam, char *instance)
{
    s_tfexprinfo info;

    tf_iexprinfo(nparam, &info, instance);
    if (info.expr_ngroups < 1) {
	return (0);
    }
    if (info.expr_type == tf_string) {
	return ((LP64) info.expr_string);
    }

    return (info.expr_value_p->avalbits);

}

void tf_putp(int nparam, int value)
{
    tf_iputp(nparam, value, tf_getinstance());
    return;
}

void tf_iputp(int nparam, int value, char *instance)
{
    s_tfexprinfo info;
    Group *g;


    // handle function return here
    if (nparam == 0) {
	g = DECL_STORAGE(FUNC_REF_RETURNDECL((tree) instance));
	AVAL(g) = value;
	BVAL(g) = 0;
	return;
    }

    tf_iexprinfo(nparam, &info, instance);
    if (info.expr_ngroups < 1) {
	return;
    }

    if (info.expr_type == tf_string) {
	return;
    }

    info.expr_value_p->avalbits = value;
    info.expr_value_p->bvalbits = 0;

    tf_ipropagatep(nparam, instance);
    return;
}

void tf_putlongp(int nparam, int lowvalue, int highvalue)
{
    tf_iputlongp(nparam, lowvalue, highvalue, tf_getinstance());
    return;
}

void tf_iputlongp(int nparam, int lowvalue, int highvalue, char *instance)
{
    s_tfexprinfo info;
    Group *g;


    // handle function return here
    if (nparam == 0) {
	g = DECL_STORAGE(FUNC_REF_RETURNDECL((tree) instance));
	AVAL(g) = lowvalue;
	BVAL(g) = 0;
	g++;
	AVAL(g) = highvalue;
	BVAL(g) = 0;
	return;
    }

    tf_iexprinfo(nparam, &info, instance);
    if (info.expr_ngroups < 2) {
	return;
    }

    if (info.expr_type == tf_string) {
	return;
    }

    info.expr_value_p->avalbits = lowvalue;
    info.expr_value_p->bvalbits = 0;
    info.expr_value_p++;
    info.expr_value_p->avalbits = highvalue;
    info.expr_value_p->bvalbits = 0;

    tf_ipropagatep(nparam, instance);
    return;
}

int tf_getlongp(int *aof_highvalue, int nparam)
{
    return tf_igetlongp(aof_highvalue, nparam, tf_getinstance());
}

int tf_igetlongp(int *aof_highvalue, int nparam, char *instance)
{
    s_tfexprinfo info;

    tf_iexprinfo(nparam, &info, instance);
    if (info.expr_ngroups < 2) {
	return (0);
    }

    if (info.expr_type == tf_string) {
	return ((LP64) info.expr_string);
    }

    *aof_highvalue = (info.expr_value_p + 1)->avalbits;
    return (info.expr_value_p->avalbits);
}

double tf_getrealp(int nparam)
{
    return tf_igetrealp(nparam, tf_getinstance());
}

double tf_igetrealp(int nparam, char *instance)
{
    s_tfexprinfo info;

    tf_iexprinfo(nparam, &info, instance);

    if (info.expr_type != tf_readonlyreal
	&& info.expr_type != tf_readwritereal) {
	return (0.0);
    }

    return (info.real_value);
}

void tf_putrealp(int nparam, double value)
{
    tf_iputrealp(nparam, value, tf_getinstance());
    return;
}

void tf_iputrealp(int nparam, double value, char *instance)
{
    s_tfexprinfo info;
    Group *g;


    // handle function return here
    if (nparam == 0) {
	g = DECL_STORAGE(FUNC_REF_RETURNDECL((tree) instance));
	REAL_(g) = value;
	return;
    }

    tf_iexprinfo(nparam, &info, instance);
    if (info.expr_ngroups < 1) {
	return;
    }

    if (info.expr_type != tf_readonlyreal
	&& info.expr_type != tf_readwritereal) {
	return;
    }

    ((Group *) info.expr_value_p)->real = value;

    tf_ipropagatep(nparam, instance);
    return;
}

/************************************************************************
  tf_getcstringp							
  tf_igetcstringp							
/***********************************************************************/


char *tf_getcstringp(int argnum)
{
    return (tf_igetcstringp(argnum, tf_getinstance()));
}

char *tf_igetcstringp(int argnum, char *instance)
{
    s_tfexprinfo info;


    tf_iexprinfo(argnum, &info, instance);

    return (info.expr_string);
}

/************************************************************************
  GroupToString							
/***********************************************************************/

static char *GroupToString(Group * g, int nbits, int ngroups)
{
    char buffer[1024];
    int bufferIndex;
    int groupIndex;
    int byteIndex;
    int charPerBit;
    char *ptr;
    char numberOfChars;
    int limit;

    charPerBit = sizeof(Bit) / sizeof(char);
    numberOfChars = (nbits + 7) / 8;
    bufferIndex = 0;
    for (groupIndex = ngroups - 1; groupIndex >= 0; groupIndex--) {
	ptr = (char *) &AVAL(&g[groupIndex]);
	if (groupIndex != ngroups - 1) {
	    limit = 3;
	} else {
	    limit = numberOfChars % charPerBit;
	    if (limit != 0) {
		limit--;
	    } else {
		limit = 3;
	    }
	}
	for (byteIndex = limit; byteIndex >= 0; byteIndex--) {
#ifdef WORDS_BIGENDIAN
	    buffer[bufferIndex] = ptr[charPerBit - byteIndex - 1];
#else
	    buffer[bufferIndex] = ptr[byteIndex];
#endif
	    bufferIndex += 1;
	    numberOfChars -= 1;
	}
    }

    buffer[bufferIndex] = 0;

    return (pli_write_string(buffer));
}

/************************************************************************
  StringToGroup							
/***********************************************************************/

static void StringToGroup(char *string, Group * g, int nbits, int ngroups)
{
    char buffer[1024];
    int bufferIndex;
    int groupIndex;
    int byteIndex;
    int charPerBit;
    char *ptr;
    char numberOfChars;
    int limit;
    int length;

    charPerBit = sizeof(Bit) / sizeof(char);
    numberOfChars = (nbits + 7) / 8;
    length = strlen(string);


    bufferIndex = 0;
    for (groupIndex = ngroups - 1; groupIndex >= 0; groupIndex--) {
	ptr = (char *) &AVAL(&g[groupIndex]);
	BVAL(&g[groupIndex]) = 0;
	if (groupIndex != ngroups - 1) {
	    limit = 3;
	} else {
	    limit = numberOfChars % charPerBit;
	    if (limit != 0) {
		limit--;
	    } else {
		limit = 3;
	    }
	}
	for (byteIndex = limit; byteIndex >= 0; byteIndex--) {
#ifdef WORDS_BIGENDIAN
	    if (numberOfChars > length) {
		ptr[charPerBit - byteIndex - 1] = 0;
	    } else {
		ptr[charPerBit - byteIndex - 1] = string[bufferIndex];
		bufferIndex += 1;
	    }
#else
	    if (numberOfChars > length) {
		ptr[byteIndex] = 0;
	    } else {
		ptr[byteIndex] = string[bufferIndex];
		bufferIndex += 1;
	    }
#endif
	    numberOfChars -= 1;
	}
    }

}

/****************************************************************
	tf_compare_long
 ****************************************************************
 */
int tf_compare_long(int low1, int high1, int low2, int high2)
{
    Bit a[2];
    Bit b[2];
    Bit c[3];

    b[0] = low1;
    b[1] = high1;

    c[0] = low2;
    c[1] = high2;

    BitSub(a, b, c, 2);

    if (a[0] == 0 && a[1] == 0) {
	return (0);
    } else if (a[0] < 0) {
	return (-1);
    } else {
	return (1);
    }
}

/****************************************************************
	tf_add_long
 ****************************************************************
 */
void tf_add_long(int *low1p, int *high1p, int low2, int high2)
{
    Bit a[2];
    Bit b[2];
    Bit c[3];

    b[0] = *low1p;
    b[1] = *high1p;

    c[0] = low2;
    c[1] = high2;

    BitAdd(a, b, c, 2);

    *low1p = a[0];
    *high1p = a[1];
}

/****************************************************************
	tf_subtract_long
 ****************************************************************
 */
void tf_subtract_long(int *low1p, int *high1p, int low2, int high2)
{
    Bit a[2];
    Bit b[2];
    Bit c[3];

    b[0] = *low1p;
    b[1] = *high1p;

    c[0] = low2;
    c[1] = high2;

    BitSub(a, b, c, 2);

    *low1p = a[0];
    *high1p = a[1];
}

/****************************************************************
	tf_multiply_long
 ****************************************************************
 */
void tf_multiply_long(int *low1p, int *high1p, int low2, int high2)
{
    Group a[4];
    Group b[2];
    Group c[2];
    int sign;

    sign = 1;

    if (*high1p < 0) {
	sign = sign ^ 1;
	AVAL(&b[0]) = ~*low1p;
	AVAL(&b[1]) = ~*high1p;
	AVAL(&b[0])++;
	if (AVAL(&b[0]) == 0) {
	    AVAL(&b[1])++;
	}
    } else {
	AVAL(&b[0]) = *low1p;
	AVAL(&b[1]) = *high1p;
    }

    if (high2 < 0) {
	sign = sign ^ 1;
	AVAL(&c[0]) = ~low2;
	AVAL(&c[1]) = ~high2;
	AVAL(&c[0])++;
	if (AVAL(&c[0]) == 0) {
	    AVAL(&c[1])++;
	}
    } else {
	AVAL(&c[0]) = low2;
	AVAL(&c[1]) = high2;
    }

    GroupMult(a, b, c, 2);

    if (sign) {
	*low1p = AVAL(&a[0]);
	*high1p = AVAL(&a[1]);
    } else {
	*low1p = ~AVAL(&a[0]);
	*high1p = ~AVAL(&a[1]);
	(*low1p)++;
	if (*low1p == 0) {
	    (*high1p)++;
	}
    }

}

/****************************************************************
	tf_divide_long
 ****************************************************************
 */
void tf_divide_long(int *low1p, int *high1p, int low2, int high2)
{
    Group a[2];
    Group b[2];
    Group c[2];
    Group m[2];
    int sign;

    sign = 1;

    if (*high1p < 0) {
	sign = sign ^ 1;
	AVAL(&b[0]) = ~*low1p;
	AVAL(&b[1]) = ~*high1p;
	AVAL(&b[0])++;
	if (AVAL(&b[0]) == 0) {
	    AVAL(&b[1])++;
	}
    } else {
	AVAL(&b[0]) = *low1p;
	AVAL(&b[1]) = *high1p;
    }

    if (high2 < 0) {
	sign = sign ^ 1;
	AVAL(&c[0]) = ~low2;
	AVAL(&c[1]) = ~high2;
	AVAL(&c[0])++;
	if (AVAL(&c[0]) == 0) {
	    AVAL(&c[1])++;
	}
    } else {
	AVAL(&c[0]) = low2;
	AVAL(&c[1]) = high2;
    }

    GroupDiv(a, m, b, c, 2, 0);

    if (sign) {
	*low1p = AVAL(&a[0]);
	*high1p = AVAL(&a[1]);
    } else {
	*low1p = ~AVAL(&a[0]);
	*high1p = ~AVAL(&a[1]);
	(*low1p)++;
	if (*low1p == 0) {
	    (*high1p)++;
	}
    }

}

/****************************************************************

	acc_fetch_delays
		- retrieve delays for given object 

*****************************************************************/
bool acc_fetch_delays(handle object, ...)
{
    tree pathInstance;
    tree pathDesc;
    tree instance;
    tree constraint;
    tree module;
    tree t = (tree) object;
    double scaleFactor;
    int delayCount;
    double d1;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    double *dp[18];
    va_list ap;
    int i;

    va_start(ap, object);
    for (i = 0; i < 18; i++) {
	dp[i] = va_arg(ap, double *);
    }
    va_end(ap);

    acc_error_flag = FALSE;

    switch (acc_fetch_type(object)) {

    case accTchk:
	ASSERT(TREE_CODE((tree) object) == TIMING_CHECK);
	d1 = (double) TIMING_CHECK_PARAM1((tree) object);
	delayCount = 1;
	switch (acc_fetch_fulltype(object)) {
	case accSetuphold:
	case accNochange:
	    d2 = TIMING_CHECK_PARAM2((tree) object);
	    delayCount = 2;
	    break;
	default:
	    break;
	}
	break;

    case accPrimitive:

	if (GATE_DELAY(t)) {
	    d1 = (double) eval_delay(GATE_DELAY(t), ONE);
	    delayCount = 1;
	    if (pli_info.config.accPathDelayCount_ >= 2) {
		d2 = (double) eval_delay(GATE_DELAY(t), ZERO);
		delayCount = 2;
	    }
	    if (pli_info.config.accPathDelayCount_ >= 3) {
		d3 = (double) eval_delay(GATE_DELAY(t), Z);
		delayCount = 3;
	    }
	} else {
	    d1 = 0;
	    delayCount = 1;
	    if (pli_info.config.accPathDelayCount_ >= 2) {
		d2 = 0;
		delayCount = 2;
	    }
	    if (pli_info.config.accPathDelayCount_ >= 3) {
		d3 = 0;
		delayCount = 3;
	    }
	}
	break;

    case accPath:
	pathInstance = t;
	ASSERT(TREE_CODE(pathInstance) == PATH_INSTANCE);

	pathDesc = PATH_INSTANCE_PATHDESC(pathInstance);
	ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

	module = PATH_INSTANCE_MODULE(pathInstance);
	ASSERT(TREE_CODE(module) == MODULE_BLOCK);

	for (instance = MODULE_SPECINST(module);
	     instance != NULL_TREE; instance = TREE_CHAIN(instance)) {

	    if (TREE_CODE(instance) != PATH_OUTPUT) {
		continue;
	    }


	    for (constraint = PATH_OUTPUT_CONSTRAINTS(instance);
		 constraint != NULL_TREE;
		 constraint = TREE_CHAIN(constraint)) {

		ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
		if (PATH_CONSTRAINT_PATHDESC(constraint) == pathDesc) {
		    d1 = (double) PATH_CONSTRAINT_DELAY(constraint, ZERO,
							ONE);
		    delayCount = 1;
		    if (pli_info.config.accPathDelayCount_ >= 2) {
			d2 = (double) PATH_CONSTRAINT_DELAY(constraint,
							    ONE, ZERO);
			delayCount = 2;
		    }
		    if (pli_info.config.accPathDelayCount_ >= 3) {
			d3 = (double) PATH_CONSTRAINT_DELAY(constraint,
							    ZERO, Z);
			delayCount = 3;
		    }
		    if (pli_info.config.accPathDelayCount_ == 6) {
			d4 = (double) PATH_CONSTRAINT_DELAY(constraint, Z,
							    ONE);
			d5 = (double) PATH_CONSTRAINT_DELAY(constraint,
							    ONE, Z);
			d6 = (double) PATH_CONSTRAINT_DELAY(constraint, Z,
							    ZERO);
			delayCount = 6;
		    }
		    goto done;
		}
	    }
	}
	break;

    }
  done:
    scaleFactor = pow(10.0, tf_igettimeprecision(null) -
		      tf_igettimeprecision((char *)
					   acc_handle_parent(object)));

    switch (delayCount) {
    case 6:
	if (pli_info.config.accMinTypMaxDelays_) {
	    *dp[17] = d6 * scaleFactor;
	    *dp[16] = *dp[17];
	    *dp[15] = *dp[17];
	} else {
	    *dp[5] = d6 * scaleFactor;
	}
    case 5:
	if (pli_info.config.accMinTypMaxDelays_) {
	    *dp[14] = d5 * scaleFactor;
	    *dp[13] = *dp[14];
	    *dp[12] = *dp[14];
	} else {
	    *dp[4] = d5 * scaleFactor;
	}
    case 4:
	if (pli_info.config.accMinTypMaxDelays_) {
	    *dp[11] = d4 * scaleFactor;
	    *dp[10] = *dp[11];
	    *dp[9] = *dp[11];
	} else {
	    *dp[3] = d4 * scaleFactor;
	}
    case 3:
	if (pli_info.config.accMinTypMaxDelays_) {
	    *dp[8] = d3 * scaleFactor;
	    *dp[7] = *dp[8];
	    *dp[6] = *dp[8];
	} else {
	    *dp[2] = d3 * scaleFactor;
	}
    case 2:
	if (pli_info.config.accMinTypMaxDelays_) {
	    *dp[5] = d2 * scaleFactor;
	    *dp[4] = *dp[5];
	    *dp[3] = *dp[5];
	} else {
	    *dp[1] = d2 * scaleFactor;
	}
    case 1:
	if (pli_info.config.accMinTypMaxDelays_) {
	    *dp[2] = d1 * scaleFactor;
	    *dp[1] = *dp[2];
	    *dp[0] = *dp[2];
	} else {
	    *dp[0] = d1 * scaleFactor;
	}
	break;
    default:
	acc_error_flag = TRUE;
	tf_error("Illegal object in acc_fetch_delays()");
	return (0);
    }

    return (1);
}

/****************************************************************

	acc_replace_delays
		- replace delays for given object 

*****************************************************************/
bool acc_replace_delays(handle object, ...)
{
    tree pathInstance;
    tree pathDesc;
    tree instance;
    tree constraint;
    tree module;
    tree delayNode;
    int delayCount;
    double scaleFactor;
    double d1;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    va_list ap;

    acc_error_flag = FALSE;

    va_start(ap, object);
    if (pli_info.config.accMinTypMaxDelays_ && delay_type == MIN_DELAY) {
	d1 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d2 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d3 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d4 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d5 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d6 = va_arg(ap, double);
    } else if (pli_info.config.accMinTypMaxDelays_
	       && delay_type == TYP_DELAY) {
	va_arg(ap, double);
	d1 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d2 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d3 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d4 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d5 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d6 = va_arg(ap, double);
    } else if (pli_info.config.accMinTypMaxDelays_
	       && delay_type == MAX_DELAY) {
	va_arg(ap, double);
	va_arg(ap, double);
	d1 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d2 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d3 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d4 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d5 = va_arg(ap, double);
	va_arg(ap, double);
	va_arg(ap, double);
	d6 = va_arg(ap, double);
    } else {
	d1 = va_arg(ap, double);
	d2 = va_arg(ap, double);
	d3 = va_arg(ap, double);
	d4 = va_arg(ap, double);
	d5 = va_arg(ap, double);
	d6 = va_arg(ap, double);
    }
    va_end(ap);

    scaleFactor = pow(10.0, tf_igettimeprecision(null) -
		      tf_igettimeprecision((char *)
					   acc_handle_parent(object)));
    d1 *= scaleFactor;
    d2 *= scaleFactor;
    d3 *= scaleFactor;
    d4 *= scaleFactor;
    d5 *= scaleFactor;
    d6 *= scaleFactor;

    switch (acc_fetch_type(object)) {
    case accTchk:
	ASSERT(TREE_CODE((tree) object) == TIMING_CHECK);
	TIMING_CHECK_PARAM1((tree) object) = (int) max(d1, 0);
	switch (acc_fetch_fulltype(object)) {
	case accSetuphold:
	case accNochange:
	    TIMING_CHECK_PARAM2((tree) object) = (int) max(d2, 0);
	    break;
	default:
	    break;
	}
	break;

    case accPrimitive:
	// determine number of delays
	switch (acc_fetch_fulltype(object)) {
	case accBufif0Gate:
	case accBufif1Gate:
	case accNotif0Gate:
	case accNotif1Gate:
	case accNmosGate:
	case accPmosGate:
	case accRnmosGate:
	case accRpmosGate:
	case accCmosGate:
	case accRcmosGate:
	    delayCount = 3;
	    break;
	case accPulldownGate:
	case accPullupGate:
	case accTranGate:
	case accRtranGate:
	    delayCount = 0;
	    break;
	default:
	    delayCount = 2;
	    break;
	}

	// create a new delay node
	switch (delayCount) {
	case 2:
	    delayNode = build_nt(DELAY_EXPR,
				 build_int_cst((int) max(d1, 0)),
				 build_int_cst((int) max(d2, 0)));
	    TREE_LABEL(delayNode) = 2;
	    break;
	case 3:
	    if (pli_info.config.accToHiZDelay_ == eDelayMin) {
		d3 = min(d1, d2);
	    } else if (pli_info.config.accToHiZDelay_ == eDelayMax) {
		d3 = max(d1, d2);
	    } else if (pli_info.config.accToHiZDelay_ == eDelayAverage) {
		d3 = (d1 + d2) / 2;
	    }

	    delayNode = build_nt(DELAY_EXPR,
				 build_int_cst((int) max(d1, 0)),
				 build_int_cst((int) max(d2, 0)),
				 build_int_cst((int) max(d3, 0)));
	    TREE_LABEL(delayNode) = 3;
	    break;
	default:
	    return (false);
	}


	// create code for delay
	pass3_delay(delayNode);
	ASSERT(TREE_CODE((tree) object) == GATE_INSTANCE);
	GATE_DELAY((tree) object) = delayNode;
	break;
    case accModPath:

	pathInstance = (tree) object;
	ASSERT(TREE_CODE(pathInstance) == PATH_INSTANCE);

	pathDesc = PATH_INSTANCE_PATHDESC(pathInstance);
	ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

	module = PATH_INSTANCE_MODULE(pathInstance);
	ASSERT(TREE_CODE(module) == MODULE_BLOCK);

	for (instance = MODULE_SPECINST(module);
	     instance != NULL_TREE; instance = TREE_CHAIN(instance)) {

	    if (TREE_CODE(instance) != PATH_OUTPUT) {
		continue;
	    }


	    for (constraint = PATH_OUTPUT_CONSTRAINTS(instance);
		 constraint != NULL_TREE;
		 constraint = TREE_CHAIN(constraint)) {

		ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
		if (PATH_CONSTRAINT_PATHDESC(constraint) == pathDesc) {
		    int oldTurnOff[2];
		    int newTurnOff[2];
		    oldTurnOff[0] =
			PATH_CONSTRAINT_DELAY(constraint, ZERO, Z);
		    oldTurnOff[1] =
			PATH_CONSTRAINT_DELAY(constraint, ONE, Z);
		    switch (pli_info.config.accPathDelayCount_) {
		    case 1:
			PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, Z, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) =
			    (int) d1;
			newTurnOff[0] = (int) max(d1, 0);
			newTurnOff[1] = (int) max(d1, 0);
			break;
		    case 2:
			PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) =
			    (int) d2;
			PATH_CONSTRAINT_DELAY(constraint, Z, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) =
			    (int) d2;
			newTurnOff[0] = (int) max(d1, 0);
			newTurnOff[1] = (int) max(d2, 0);
			break;
		    case 3:
			PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) =
			    (int) d2;
			PATH_CONSTRAINT_DELAY(constraint, Z, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) =
			    (int) d2;
			newTurnOff[0] = (int) max(d3, 0);
			newTurnOff[1] = (int) max(d3, 0);
			break;
		    case 6:
			PATH_CONSTRAINT_DELAY(constraint, ZERO, ONE) =
			    (int) d1;
			PATH_CONSTRAINT_DELAY(constraint, ONE, ZERO) =
			    (int) d2;
			PATH_CONSTRAINT_DELAY(constraint, Z, ONE) =
			    (int) d4;
			PATH_CONSTRAINT_DELAY(constraint, Z, ZERO) =
			    (int) d6;
			newTurnOff[0] = (int) max(d3, 0);
			newTurnOff[1] = (int) max(d5, 0);
			break;
		    }
		    if (pli_info.config.accToHiZDelay_ == eDelayMin) {
			PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) =
			    min(newTurnOff[0], newTurnOff[1]);
			PATH_CONSTRAINT_DELAY(constraint, ONE, Z) =
			    min(newTurnOff[0], newTurnOff[1]);
		    } else if (pli_info.config.accToHiZDelay_ == eDelayMax) {
			PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) =
			    max(newTurnOff[0], newTurnOff[1]);
			PATH_CONSTRAINT_DELAY(constraint, ONE, Z) =
			    max(newTurnOff[0], newTurnOff[1]);
		    } else if (pli_info.config.accToHiZDelay_ ==
			       eDelayAverage) {
			PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) =
			    avg(newTurnOff[0], newTurnOff[1]);
			PATH_CONSTRAINT_DELAY(constraint, ONE, Z) =
			    avg(newTurnOff[0], newTurnOff[1]);
		    } else {
			PATH_CONSTRAINT_DELAY(constraint, ZERO, Z) =
			    newTurnOff[0];
			PATH_CONSTRAINT_DELAY(constraint, ONE, Z) =
			    newTurnOff[1];
		    }
		    setXPathConstraints(constraint);
		}
	    }
	}
	break;
    default:
	acc_error_flag = TRUE;
	tf_error("Illegal object in acc_replace_delays()");
	return (0);

    }
    return (1);
}


/****************************************************************

	acc_append_delays
		- add delays for given object 

*****************************************************************/
bool acc_append_delays(handle object, ...)
{
    double xd[18];
    int save_error_flag;
    va_list ap;
    int i;

    acc_error_flag = FALSE;
    save_error_flag = pli_info.config.accDisplayErrors_;
    pli_info.config.accDisplayErrors_ = 0;
    if (!acc_fetch_delays
	(object, &xd[0], &xd[1], &xd[2], &xd[3], &xd[4], &xd[5], &xd[6],
	 &xd[7], &xd[8], &xd[9], &xd[10], &xd[11], &xd[12], &xd[13],
	 &xd[14], &xd[15], &xd[16], &xd[17])) {
	acc_error_flag = TRUE;
	pli_info.config.accDisplayErrors_ = save_error_flag;
	tf_error("Illegal object in acc_append_delays()");
	return (0);

    }

    va_start(ap, object);
    for (i = 0; i < 18; i++) {
	xd[i] += va_arg(ap, double);
    }
    va_end(ap);

    if (!acc_replace_delays
	(object, xd[0], xd[1], xd[2], xd[3], xd[4], xd[5], xd[6], xd[7],
	 xd[8], xd[9], xd[10], xd[11], xd[12], xd[13], xd[14], xd[15],
	 xd[16], xd[17])) {
	acc_error_flag = TRUE;
	pli_info.config.accDisplayErrors_ = save_error_flag;
	tf_error("Illegal object in acc_append_delays()");
	return (0);

    }
    pli_info.config.accDisplayErrors_ = save_error_flag;
    return (1);

}

/****************************************************************
	acc_fetch_pulsere
*****************************************************************/
bool
acc_fetch_pulsere(handle path, double *f1, double *e1, double *f2,
		  double *e2, double *f3, double *e3, double *f4,
		  double *e4, double *f5, double *e5, double *f6,
		  double *e6)
{
    acc_error_flag = FALSE;
    tf_error("acc_fetch_pulsere() not supported");
    return (0);
}

/****************************************************************
	acc_append_pulsere
*****************************************************************/
bool
acc_append_pulsere(handle path, double f1, double e1, double f2, double e2,
		   double f3, double e3, double f4, double e4, double f5,
		   double e5, double f6, double e6)
{
    acc_error_flag = TRUE;
    tf_error("acc_append_pulsere() not supported");
    return (0);
}

/****************************************************************
	acc_replace_pulsere
*****************************************************************/
bool
acc_replace_pulsere(handle path, double f1, double e1, double f2,
		    double e2, double f3, double e3, double f4, double e4,
		    double f5, double e5, double f6, double e6)
{
    acc_error_flag = TRUE;
    tf_error("acc_replace_pulsere() not supported");
    return (0);
}

/****************************************************************
	acc_set_pulsere
*****************************************************************/
bool
acc_set_pulsere(handle path, double reject_percentage, double e_percentage)
{
    acc_error_flag = TRUE;
    tf_error("acc_set_pulsere() not supported");
    return (0);
}

/************************************************************************
 * acc_fetch_value							*
/***********************************************************************/

char *acc_fetch_value(handle object, char *format, ...)
{
    int num_bits = acc_fetch_size(object);
    int num_chars;
    enum radii radix;
    Group *g;
    char *return_pointer;
    int num_groups;
    p_acc_value value;
    va_list ap;
    char *result;

    acc_error_flag = FALSE;
    num_groups = (num_bits - 1) / 32 + 1;
    g = fetch_storage((tree) object);

    switch (format[1]) {
    case 'b':
	radix = BIN;
	num_chars = num_bits;
	break;

    case 'd':
	radix = DEC;
	num_chars = num_bits / 3;
	break;

    case 'o':
	radix = OCT;
	num_chars = num_bits / 3;
	break;

    case 'h':
    case 'x':
	radix = HEX;
	num_chars = num_bits / 4;
	break;

    case '%':
	va_start(ap, format);
	value = va_arg(ap, p_acc_value);
	switch (value->format) {
	case accBinStrVal:
	    result = acc_fetch_value(object, "%b");
	    value->value.str = result;
	    break;
	case accOctStrVal:
	    result = acc_fetch_value(object, "%o");
	    value->value.str = result;
	    break;
	case accDecStrVal:
	    result = acc_fetch_value(object, "%d");
	    value->value.str = result;
	    break;
	case accHexStrVal:
	    result = acc_fetch_value(object, "%h");
	    value->value.str = result;
	    break;
	case accIntVal:
	    value->value.integer = AVAL(g);
	    result = NULL;
	    break;
	case accRealVal:
	    value->value.real = g->real;
	    result = NULL;
	    break;
	case accScalarVal:
	    switch ((AVAL(g) & 1) | ((BVAL(g) & 1) << 1)) {
	    case 0:
		value->value.scalar = vcl0;
		break;
	    case 1:
		value->value.scalar = vcl1;
		break;
	    case 2:
		value->value.scalar = vclZ;
		break;
	    case 3:
		value->value.scalar = vclX;
		break;
	    }
	    result = NULL;
	    break;
	case accStringVal:
	    value->value.str = GroupToString(g, num_bits, num_groups);
	    result = NULL;
	    break;
	case accVectorVal:
	    memcpy(value->value.vector, g,
		   num_groups * sizeof(s_acc_vecval));
	    result = NULL;
	    break;
	}
	va_end(ap);
	return (result);
    case 'v':
    default:
	acc_error_flag = FALSE;
	return (null);
    }
    global_print_override = 1;
    global_print_buffer = (char *) xmalloc(num_chars + 1);
    global_print_p = global_print_buffer;
    print_datum_file(0, g, (nbits_t) 0, num_bits, radix, 0, 1, 0);

    return_pointer = pli_write_string(global_print_buffer);

    free(global_print_buffer);
    global_print_override = 0;
    return return_pointer;
}

/************************************************************************
 * 			
 * 		tf_strgetp
 * 		tf_istrgetp
 * 			
 ***********************************************************************/

char *tf_strgetp(int nparam, char format_character)
{
    return (tf_istrgetp(nparam, format_character, tf_getinstance()));
}

char *tf_istrgetp(int nparam, char format_character, char *instance)
{
    char *result;
    tree arg;
    Group *g;
    int numbits;
    enum radii radix;
    int num_chars;


    result = NULL;
    arg = nth_parameter(nparam, (tree) instance);
    if (arg == NULL) {
	return (result);
    }

    numbits = TREE_NBITS(*TREE_EXPR_CODE(arg));
    eval(TREE_EXPR_CODE(arg));
    g = *--R;

    switch (format_character) {
    case 'd':
    case 'D':
	radix = DEC;
	num_chars = numbits / 3;
	break;
    case 'h':
    case 'H':
	radix = HEX;
	num_chars = numbits / 4;
	break;
    case 'b':
    case 'B':
	radix = BIN;
	num_chars = numbits;
	break;
    case 'o':
    case 'O':
	radix = OCT;
	num_chars = numbits / 3;
	break;
    default:
	return (NULL);
    }
    global_print_override = 1;
    global_print_buffer = (char *) xmalloc(num_chars + 1);
    global_print_p = global_print_buffer;
    print_datum_file(0, g, (nbits_t) 0, numbits, radix, 0, 0, 0);

    result = pli_write_string(global_print_buffer);

    free(global_print_buffer);
    global_print_override = 0;
    return (result);
}

/************************************************************************
 * acc_vcl_add								*
/***********************************************************************/

void
acc_vcl_add(handle hobject, int (*routine) (s_vc_record *),
	    char *user_data, int VCL_flags)
{
    struct pli_vcl *vcl;
    Marker *vcl_marker;
    struct Marker_info marker_info;

    tree object = (tree) hobject;

    acc_error_flag = FALSE;

    if (VCL_flags != vcl_verilog_logic
	&& VCL_flags != vcl_verilog_strength) {
	acc_error_flag = TRUE;
	tf_error
	    ("only 'vcl_verilog_logic' flag is supported in acc_vcl_add()");
	return;
    }
    // check for a port or terminal

    if (TREE_CODE(object) == TREE_LIST) {
	object = TREE_PURPOSE(object);
	if (PORT_REDEFINED_ATTR(object)) {
	    object = DECL_THREAD(object);
	}
    }

    if (*tree_code_type[TREE_CODE(object)] != 'd') {
	tf_error("Illegal object type for acc_vcl_add");
	acc_error_flag = TRUE;
	return;
    }

    if (lookup_vcl(object, routine, user_data, &vcl_marker)) {
	tf_warning("vcl already set in acc_vcl_add()");
	return;
    }

    marker_info.first = NULL;
    marker_info.last = NULL;
    marker_info.current_scb = NULL;	/* This will bypass SCBs */
    marker_info.flags = (enum marker_flags) (M_VCL + M_FIXED);
    marker_info.delay = 0;

    BuildMarker(object, &marker_info);
    vcl_marker = marker_info.first;
    vcl = (struct pli_vcl *) xmalloc(sizeof(struct pli_vcl));
    vcl->marker = vcl_marker;
    vcl->decl = object;
    vcl->routine = routine;
    vcl->user_data = user_data;
    vcl_marker->expr.vcl = vcl;
}

/* Scan the markers for an object (already assumed to be a decl) for one that
   was generated by a similar acc_vcl_add call.  If found, pass make the
   marker address and return nonzero. */

static int
lookup_vcl(tree hobject, int (*routine) (s_vc_record *), char *user_data,
	   Marker ** marker)
{
    Marker *mark;
    tree object = (tree) hobject;
    for (mark = DECL_EVENT_CHAIN(object); mark; mark = mark->next) {
	if (mark->flags & M_VCL) {
	    struct pli_vcl *vcl = (struct pli_vcl *) mark->expr.vcl;
	    if (vcl->routine == routine && vcl->user_data == user_data) {
		*marker = mark;
		return 1;
	    }
	}
    }
    return 0;
}

//struct t_vc_record vc_record;
s_vc_record vc_record;

/* This is called from the scheduler when a vcl var has changed */

void vcl_dispatch(struct pli_vcl *vcl)
{
    Group *g;
    int ival;

    g = DECL_STORAGE(vcl->decl);
    vc_record.vc_hightime = CurrentTime.timeh;
    vc_record.vc_lowtime = CurrentTime.timel;
    vc_record.user_data = vcl->user_data;

    switch (TREE_CODE(vcl->decl)) {
    case REG_SCALAR_DECL:
	vc_record.vc_reason = sregister_value_change;
	goto logic_common;

    case NET_SCALAR_DECL:
	vc_record.vc_reason = logic_value_change;
      logic_common:
	ival = (AVAL(g) & 1) | ((BVAL(g) & 1) << 1);
	switch (ival) {
	case 0:
	    vc_record.out_value.logic_value = vcl0;
	    break;
	case 1:
	    vc_record.out_value.logic_value = vcl1;
	    break;
	case 2:
	    vc_record.out_value.logic_value = vclZ;
	    break;
	case 3:
	    vc_record.out_value.logic_value = vclX;
	    break;
	}
	break;


    case REG_VECTOR_DECL:
	vc_record.vc_reason = vregister_value_change;
	goto vector_common;

    case NET_VECTOR_DECL:
	vc_record.vc_reason = vector_value_change;
	goto vector_common;

    case INTEGER_DECL:
	vc_record.vc_reason = integer_value_change;
	goto vector_common;

    case TIME_DECL:
	vc_record.vc_reason = time_value_change;
      vector_common:
	vc_record.out_value.vector_handle = (handle) vcl->decl;
	break;

    case REAL_DECL:
	vc_record.vc_reason = real_value_change;
	vc_record.out_value.real_value = REAL_(g);
	break;

    case EVENT_DECL:
	vc_record.vc_reason = event_value_change;
	goto vector_common;


    default:
	tf_error("Unknown type while dispatching vcl");
	return;
    }
    (vcl->routine) (&vc_record);
}

/************************************************************************
 * acc_vcl_delete							*
/***********************************************************************/

void
acc_vcl_delete(handle hobject, int (*routine) (s_vc_record *),
	       char *user_data, int VCL_flags)
{
    Marker *vcl_marker;
    tree object = (tree) hobject;

    acc_error_flag = FALSE;
    if (VCL_flags != vcl_verilog_logic) {
	acc_error_flag = TRUE;
	tf_error
	    ("only 'vcl_verilog_logic' flag is supported in acc_vcl_delete()");
	return;
    }

    if (lookup_vcl(object, routine, user_data, &vcl_marker)) {
	event_undo(vcl_marker);
	free(vcl_marker);
    }
}

/****************************************************************
	acc_handle_simulated_net
 *****************************************************************/

handle acc_handle_simulated_net(handle collapsed_net_handle)
{
    acc_error_flag = FALSE;
    // collapsed nets current aren't created
    return collapsed_net_handle;
}

/****************************************************************
	acc_compare_handles
 *****************************************************************/

bool acc_compare_handles(handle object1, handle object2)
{

    acc_error_flag = FALSE;

    if (acc_fetch_type(object1) == acc_fetch_type(object2) &&
	acc_handle_parent(object1) == acc_handle_parent(object2) &&
	!strcmp(acc_fetch_name(object1), acc_fetch_name(object2))) {
	return TRUE;
    }
    return FALSE;
}

/****************************************************************
	acc_set_scope
*****************************************************************/
char *acc_set_scope(handle module, char *name)
{
    acc_error_flag = FALSE;
    if (!pli_info.config.accEnableArgsSetScope_) {
	if (module == NULL) {
	    pli_info.scope = acc_next_topmod(NULL);
	} else {
	    pli_info.scope = module;
	}
    } else {
	if (module == NULL && name == NULL) {
	    pli_info.scope = acc_next_topmod(NULL);
	} else if (module == NULL) {
	    pli_info.scope = resolve_path(NULL, name);
	} else {
	    pli_info.scope = module;
	}
    }
    if (pli_info.scope == NULL) {
	tf_error("Unable to set scope in acc_set_scope()");
	acc_error_flag = TRUE;
	return (NULL);
    }

    return acc_fetch_fullname(pli_info.scope);
}



/***************************************************************
 
	append_hierarchy
		- add name of parents to string

****************************************************************/

static void append_hierarchy(handle object)
{
    handle parent;

    if (object == NULL) {
	return;
    }

    parent = acc_handle_parent(object);
    if (parent != NULL) {
	append_hierarchy(parent);
    }
    append_name(object);
    pli_append_string(".");
}

/***************************************************************

	acc_fetch_fullname
		- return full name of object

****************************************************************/
char *acc_fetch_fullname(handle object)
{
    acc_error_flag = FALSE;

    if (object == NULL) {
	acc_error_flag = TRUE;
	tf_error("Bad handle in acc_fetch_fullname()");
	return (NULL);
    }

    pli_write_string(NULL);	// start a new string
    append_hierarchy(acc_handle_parent(object));
    return (append_name(object));
}

/****************************************************************

	acc_handle_object	
		- return  the handle to named object 

*****************************************************************/

handle acc_handle_object(char *path)
{
    handle object;
    acc_error_flag = FALSE;
    object = acc_handle_by_name(path, pli_info.scope);
    if (object == NULL) {
	object = acc_handle_by_name(path, null);
    }
    if (object == NULL) {
	acc_error_flag = TRUE;
    }
    return (object);
}

/************************************************************************
 * acc_next_scope							*
/***********************************************************************/

handle acc_next_scope(handle module, handle scope)
{

    acc_error_flag = FALSE;
    if (!module)
	return acc_next_topmod(scope);
    else if (!scope)
	return (handle) BLOCK_DOWN((tree) module);
    else
	return (handle) TREE_CHAIN((tree) scope);
}

/************************************************************************
 * acc_reset_buffer							*
/***********************************************************************/

void acc_reset_buffer(void)
{
    acc_error_flag = FALSE;
    pli_info.p = pli_info.stringbuf;
    pli_info.lastCopy = NULL;
}

/************************************************************************
 * acc_set_interactive_scope							*
/***********************************************************************/

handle acc_set_interactive_scope(void)
{
    acc_error_flag = FALSE;
    pli_info.scope = (handle) current_scope;
    return (pli_info.scope);
}

/************************************************************************
 * acc_handle_interactive_scope							*
/***********************************************************************/

handle acc_handle_interactive_scope(void)
{
    acc_error_flag = FALSE;
    return (handle) current_scope;
}

/************************************************************************
 * acc_handle_tfinst						*
/***********************************************************************/

handle acc_handle_tfinst(void)
{
    acc_error_flag = FALSE;
    return (handle) tf_getinstance();
}

/************************************************************************
 * tf_write_save						*
/***********************************************************************/

int tf_write_save(char *blockptr, int blocklen)
{
    tf_error("tf_write_save() is not supported");
    return (0);
}

/************************************************************************
 * tf_read_restart						*
/***********************************************************************/

int tf_read_restart(char *blockptr, int blocklen)
{
    tf_error("tf_read_restart() is not supported");
    return (0);
}

/************************************************************************
 * acc_handle_scope						*
/***********************************************************************/

handle acc_handle_scope(handle object)
{
    return acc_handle_parent(object);
}

/************************************************************************
 * acc_fetch_tfarg						*
/***********************************************************************/

double acc_fetch_tfarg(int argumentNumber)
{
    int type;
    int high;
    int low;
    double result;
    int size;

    acc_error_flag = FALSE;

    if (argumentNumber < 1 || argumentNumber > tf_nump()) {
	tf_warning
	    ("argument number %d is out of range in acc_fetch_tfarg()",
	     argumentNumber);
	return (0.0);
    }

    type = tf_typep(argumentNumber);
    size = tf_sizep(argumentNumber);

    switch (type) {
    case tf_readonly:
    case tf_readwrite:
    case tf_rwbitselect:
    case tf_rwpartselect:
	if (size <= 32) {
	    high = 0;
	    low = tf_getp(argumentNumber);
	    tf_long_to_real(low, high, &result);
	    return (result);
	} else {
	    low = tf_getlongp(&high, argumentNumber);
	    tf_long_to_real(low, high, &result);
	    return (result);
	}
    case tf_readonlyreal:
    case tf_readwritereal:
	return (tf_getrealp(argumentNumber));
    case tf_nullparam:
    case tf_string:
    default:
	tf_warning
	    ("argument number %d in acc_fetch_tfarg() is not representable",
	     argumentNumber);
	return (0.0);
    }

}

/************************************************************************
 * acc_fetch_tfarg_int						*
/***********************************************************************/

int acc_fetch_tfarg_int(int argumentNumber)
{
    int type;
    int high;
    int low;
    double real;
    int size;

    acc_error_flag = FALSE;

    if (argumentNumber < 1 || argumentNumber > tf_nump()) {
	tf_warning
	    ("argument number %d is out of range in acc_fetch_tfarg_int()",
	     argumentNumber);
	return (0);
    }

    type = tf_typep(argumentNumber);
    size = tf_sizep(argumentNumber);

    switch (type) {
    case tf_readonly:
    case tf_readwrite:
    case tf_rwbitselect:
    case tf_rwpartselect:
	return tf_getp(argumentNumber);
    case tf_readonlyreal:
    case tf_readwritereal:
	real = tf_getrealp(argumentNumber);
	tf_real_to_long(real, &low, &high);
	return (low);
    case tf_nullparam:
    case tf_string:
    default:
	tf_warning
	    ("argument number %d in acc_fetch_tfarg_int() is not representable",
	     argumentNumber);
	return (0);
    }

}

/************************************************************************
 * acc_fetch_tfarg_str						*
/***********************************************************************/

char *acc_fetch_tfarg_str(int argumentNumber)
{
    int type;
    char buffer[64];


    acc_error_flag = FALSE;

    if (argumentNumber < 1 || argumentNumber > tf_nump()) {
	tf_warning
	    ("argument number %d is out of range in acc_fetch_tfarg_str()",
	     argumentNumber);
	return (NULL);
    }

    type = tf_typep(argumentNumber);

    switch (type) {
    case tf_string:
	return (tf_getcstringp(argumentNumber));
    case tf_readonly:
    case tf_readwrite:
    case tf_rwbitselect:
    case tf_rwpartselect:
	sprintf(buffer, "%d", tf_getp(argumentNumber));
	return pli_write_string(buffer);
    case tf_readonlyreal:
    case tf_readwritereal:
	sprintf(buffer, "%g", tf_getrealp(argumentNumber));
	return pli_write_string(buffer);
    case tf_nullparam:
    default:
	tf_warning
	    ("argument number %d in acc_fetch_tfarg_str() is not representable",
	     argumentNumber);
	return (NULL);
    }

}

/************************************************************************
 * acc_handle_tfarg							*
/***********************************************************************/

handle acc_handle_tfarg(int argnum)
{
    tree arg;
    int type;

    acc_error_flag = FALSE;

    arg = nth_arg(argnum, (tree) tf_getinstance());
    type = tf_typep(argnum);
    switch (type) {
    case tf_nullparam:
	return (NULL);
    case tf_string:
	return (acc_handle_object(tf_getcstringp(argnum)));
    case tf_readwrite:
    case tf_rwpartselect:
    case tf_rwbitselect:
    case tf_readwritereal:
    case tf_readonlyreal:
	return ((handle) arg);
    }

    switch (TREE_CODE(arg)) {
    case BLOCK_DECL:
	return (acc_handle_object(IDENT(DECL_NAME(arg))));
    case IDENTIFIER_NODE:
	return (acc_handle_object(IDENT(arg)));
    case NET_SCALAR_DECL:
    case NET_VECTOR_DECL:
    case MODULE_BLOCK:
	return ((handle) arg);

    }

    return (handle) NULL;
}

/****************************************************************

	acc_next_parameter

*****************************************************************/
handle acc_next_parameter(handle reference, handle param)
{
    tree next;

    acc_error_flag = FALSE;

    if (param == NULL) {
	next = BLOCK_DECL((tree) reference);
    } else {
	next = TREE_CHAIN((tree) param);
    }

    while (next != NULL && acc_fetch_type((handle) next) != accParameter) {
	next = TREE_CHAIN(next);
    }
    return ((handle) next);
}

/****************************************************************

	acc_next_specparam

*****************************************************************/
handle acc_next_specparam(handle reference, handle param)
{
    tree next;

    acc_error_flag = FALSE;

    if (param == NULL) {
	next = BLOCK_DECL((tree) reference);
    } else {
	next = TREE_CHAIN((tree) param);
    }

    while (next != NULL && acc_fetch_type((handle) next) != accSpecparam) {
	next = TREE_CHAIN(next);
    }
    return ((handle) next);
}

/****************************************************************

	acc_fetch_delay_mode

*****************************************************************/
int acc_fetch_delay_mode(handle module)
{
    acc_error_flag = FALSE;
    return (accDelayModeNone);
}

/****************************************************************

	acc_fetch_paramtype

*****************************************************************/
int acc_fetch_paramtype(handle param)
{
    int type;

    acc_error_flag = FALSE;
    type = acc_fetch_type(param);
    if (type != accParameter && type != accSpecparam) {
	acc_error_flag = TRUE;
	return (0);
    }
    return (acc_fetch_fulltype(param));
}

/****************************************************************

	acc_fetch_paramval

*****************************************************************/
double acc_fetch_paramval(handle param)
{
    int type;
    s_acc_value value;

    acc_error_flag = FALSE;
    type = acc_fetch_paramtype(param);
    switch (type) {
    case accRealParam:
	value.format = accRealVal;
	acc_fetch_value(param, "%%", &value);
	return (value.value.real);
    case accIntegerParam:
	value.format = accIntVal;
	acc_fetch_value(param, "%%", &value);
	return ((double) value.value.integer);
    case accStringParam:
	value.format = accStringVal;
	acc_fetch_value(param, "%%", &value);
	return ((double) (long) value.value.str);
    default:
	acc_error_flag = TRUE;
	return (0);
    }
}

/****************************************************************
	acc_next_portout
*****************************************************************/
handle acc_next_portout(handle reference, handle port)
{
    while (TRUE) {
	port = acc_next_port(reference, port);
	if (port == NULL || acc_fetch_direction(port) != accInput) {
	    return (port);
	}
    }
}

/****************************************************************
	tf_getworkarea	
	tf_igetworkarea	
*****************************************************************/

char *tf_getworkarea(void)
{
    return (tf_igetworkarea(tf_getinstance()));
}

char *tf_igetworkarea(char *instance)
{
    tree node;

    node = (tree) instance;
    switch (TREE_CODE(node)) {
    case SYSTASK_STMT:
	return STMT_SYSTASK_WORKAREA(node);
    case SYSFUNCTION_REF:
	return FUNC_REF_WORKAREA(node);
    default:
	return (NULL);
    }
}

/****************************************************************
	tf_setworkarea	
	tf_isetworkarea	
*****************************************************************/

void tf_setworkarea(char *area)
{
    tf_isetworkarea(area, tf_getinstance());
}

void tf_isetworkarea(char *area, char *instance)
{
    tree node;

    node = (tree) instance;
    switch (TREE_CODE(node)) {
    case SYSTASK_STMT:
	STMT_SYSTASK_WORKAREA(node) = area;
	break;
    case SYSFUNCTION_REF:
	FUNC_REF_WORKAREA(node) = area;
	break;
    }
}

/****************************************************************
	freePliDelayNode
*****************************************************************/

void freePliDelayNode(tree node)
{
    if (PLI_DELAY_STORAGE(node)) {
	free(PLI_DELAY_STORAGE(node));
    }

    PLI_DELAY_NEXT(node) = delayFreeList;
    delayFreeList = node;
}

/****************************************************************
	tf_setlongdelay	
	tf_isetlongdelay	
*****************************************************************/

int tf_setlongdelay(int low, int high)
{
    return (tf_isetlongdelay(low, high, tf_getinstance()));
}

int tf_isetlongdelay(int low, int high, char *instance)
{
    tree delayNode;
    tree t;
    time64 delay;
    time64 newTime;
    tree pliNode;
    struct SCB *scb;
    tree *prev;

    pliNode = (tree) instance;
    tf_scale_longdelay(instance, low, high, (int *) &delay.timel,
		       (int *) &delay.timeh);

    if (pliNode == NULL) {
	return (0);
    }

    switch (TREE_CODE(pliNode)) {
    case SYSTASK_STMT:
	scb = STMT_SYSTASK_DELAYSCB(pliNode);
	break;
    case SYSFUNCTION_REF:
	scb = FUNC_REF_DELAYSCB(pliNode);
	break;
    default:
	return (0);
    }


    if (delayFreeList) {
	delayNode = delayFreeList;
	delayFreeList = PLI_DELAY_NEXT(delayNode);
    } else {
	delayNode = make_node(PLI_DELAY);
    }

    ADDTIME6464(&newTime, &CurrentTime, &delay);
    PLI_DELAY_NODE(delayNode) = pliNode;

    PLI_DELAY_TIME(delayNode) = newTime;
    PLI_DELAY_LVAL(delayNode) = NULL;
    PLI_DELAY_STORAGE(delayNode) = NULL;

    if (scb != NULL) {
	for (prev = &scb->pc, t = scb->pc; t;
	     prev = &PLI_DELAY_NEXT(t), t = PLI_DELAY_NEXT(t)) {
	    if (CMPTIME64(&newTime, &PLI_DELAY_TIME(t)) < 0) {
		break;
	    }
	}
	*prev = delayNode;
	PLI_DELAY_NEXT(delayNode) = t;
    } else {
	PLI_DELAY_NEXT(delayNode) = NULL;
	scb = BuildSCB(delayNode, NOLIST);

	switch (TREE_CODE(pliNode)) {
	case SYSTASK_STMT:
	    STMT_SYSTASK_DELAYSCB(pliNode) = scb;
	    break;
	case SYSFUNCTION_REF:
	    FUNC_REF_DELAYSCB(pliNode) = scb;
	    break;
	default:
	    ASSERT(FALSE);
	}
    }
    if (scb->pc == delayNode && scb != readylist) {
	Schedule64(&newTime, scb, FALSE);
    }

    return (1);
}

/****************************************************************
	tf_setdelay	
	tf_isetdelay	
*****************************************************************/

int tf_setdelay(int delay)
{
    return (tf_isetdelay(delay, tf_getinstance()));
}

int tf_isetdelay(int delay, char *instance)
{
    return (tf_isetlongdelay(delay, 0, instance));
}

/****************************************************************
	tf_setrealdelay	
	tf_isetrealdelay	
*****************************************************************/

int tf_setrealdelay(double delay)
{
    return (tf_isetrealdelay(delay, tf_getinstance()));
}

int tf_isetrealdelay(double delay, char *instance)
{
    int high;
    int low;
    double scaledDelay;

    tf_scale_realdelay(instance, delay, &scaledDelay);
    tf_real_to_long(scaledDelay, &low, &high);
    tf_unscale_longdelay(instance, low, high, &low, &high);
    return (tf_isetlongdelay(low, high, instance));

}

/****************************************************************
	tf_clearalldelays	
	tf_iclearalldelays	
*****************************************************************/

void tf_clearalldelays(void)
{
    tf_clearalldelays();
}

void tf_iclearalldelays(char *instance)
{
    tree delayNode;
    tree pliNode;
    SCB *scb;
    tree prev;

    pliNode = (tree) instance;

    if (pliNode == NULL) {
	return;
    }

    switch (TREE_CODE(pliNode)) {
    case SYSTASK_STMT:
	scb = STMT_SYSTASK_DELAYSCB(pliNode);
	break;
    case SYSFUNCTION_REF:
	scb = FUNC_REF_DELAYSCB(pliNode);
	break;
    default:
	return;
    }


    if (scb == NULL) {
	return;
    }
    delayNode = scb->pc;
    while (delayNode != NULL) {
	prev = delayNode;
	delayNode = PLI_DELAY_NEXT(delayNode);
	freePliDelayNode(prev);
    }
    scb->pc = NULL;
    FreeSCB(scb);
}

/****************************************************************
	tf_spname	
	tf_ispname	
*****************************************************************/
char *tf_spname(void)
{
    return tf_ispname(tf_getinstance());
}

char *tf_ispname(char *instance)
{
    handle object = (handle) instance;

    return (acc_fetch_fullname(acc_handle_parent(object)));
}

/****************************************************************
	acc_fetch_argc
*****************************************************************/
int acc_fetch_argc(void)
{
    acc_error_flag = FALSE;
    return globalArgc;
}

/****************************************************************
	acc_fetch_argv
*****************************************************************/
char **acc_fetch_argv(void)
{
    acc_error_flag = FALSE;
    return globalArgv;
}



/***************************************************
	tf_text
****************************************************/

void tf_text(char *format, ...)
{
    va_list ap;
    int oldOffset;

    // this assumes that the text is less than 512 bytes

    oldOffset = pli_info.nextText - pli_info.text;
    if (pli_info.textSize - oldOffset < 512) {
	pli_info.textSize += 512;
	pli_info.text =
	    (char *) xrealloc(pli_info.text, pli_info.textSize);
	if (pli_info.text == NULL) {
	    tf_error("Out of Memory");
	    return;
	}
	pli_info.nextText = pli_info.text + oldOffset;
    }

    va_start(ap, format);
    pli_info.nextText += vsprintf(pli_info.nextText, format, ap);
    va_end(ap);
    return;
}

/***************************************************
	tf_message
****************************************************/

void tf_message(int level, char *facility, char *code, char *format, ...)
{
    va_list ap;

    switch (level) {
    case ERR_ERROR:
	if (!pli_info.config.accDisplayErrors_) {
	    return;
	}
	printf_V("ERROR!\t");
	count_error(FALSE);
	break;
    case ERR_SYSTEM:
	printf_V("SYSTEM!\t");
	count_error(FALSE);
	break;
    case ERR_INTERNAL:
	count_error(FALSE);
	printf_V("INTERNAL!\t");
	break;
    case ERR_WARNING:
	if (!pli_info.config.accDisplayWarnings_) {
	    return;
	}
	count_error(TRUE);
	printf_V("WARNING!\t");
	break;
    case ERR_MESSAGE:
	printf_V("MESSAGE!\t");
	break;
    }

    *pli_info.nextText = 0;
    printf_V(pli_info.text);
    pli_info.nextText = pli_info.text;

    va_start(ap, format);
    vprintf_V(format, ap);
    va_end(ap);

    return;
}

/***************************************************
	tf_error
****************************************************/


void tf_error(char *format, ...)
{
    va_list ap;

    if (!pli_info.config.accDisplayErrors_) {
	return;
    }
    count_error(FALSE);
    printf_V("ERROR! ");
    va_start(ap, format);
    vprintf_V(format, ap);
    va_end(ap);
    printf_V("\t[User-PLI]\n");
    return;
}

/***************************************************
	tf_warning
****************************************************/

void tf_warning(char *format, ...)
{
    va_list ap;

    if (!pli_info.config.accDisplayWarnings_)
	return;
    count_error(TRUE);
    printf_V("WARNING! ");
    va_start(ap, format);
    vprintf_V(format, ap);
    va_end(ap);
    printf_V("\t[User-PLI]\n");
    return;
}

/****************************************************************

	tf_nodeinfo
	tf_inodeinfo

*****************************************************************/

struct t_tfnodeinfo *tf_nodeinfo(int nparam,
				 struct t_tfnodeinfo *nodeinfo_p)
{
    return (tf_inodeinfo(nparam, nodeinfo_p, tf_getinstance()));
}

struct t_tfnodeinfo *tf_inodeinfo(int nparam,
				  struct t_tfnodeinfo *nodeinfo_p,
				  char *instance)
{
    int type;
    tree arg;
    tree param;
    int bits;

    /* this isn't fully implemented 
       strengths missing 
       memorys missing
     */

    param = nth_parameter(nparam, (tree) instance);
    if (param == NULL) {
	return (NULL);
    }
    arg = TREE_PURPOSE(param);
    type = acc_fetch_type((handle) arg);

    bits = tf_isizep(nparam, instance);

    switch (type) {
    case accRealVar:
	nodeinfo_p->node_type = tf_real_node;
	nodeinfo_p->node_value.real_val_p = (double *) DECL_STORAGE(arg);
	nodeinfo_p->node_ngroups = 0;
	nodeinfo_p->node_vec_size = 0;
	nodeinfo_p->node_sign = (*nodeinfo_p->node_value.real_val_p < 0);
	break;
    case accIntegerVar:
	nodeinfo_p->node_type = tf_integer_node;
	nodeinfo_p->node_value.vecval_p =
	    (struct t_vecval *) DECL_STORAGE(arg);
	nodeinfo_p->node_ngroups = (bits + 31) / 32;
	nodeinfo_p->node_vec_size = bits;
	nodeinfo_p->node_sign = (AVAL(DECL_STORAGE(arg)) < 0);
	break;
    case accTimeVar:
	nodeinfo_p->node_type = tf_time_node;
	nodeinfo_p->node_value.vecval_p =
	    (struct t_vecval *) DECL_STORAGE(arg);
	nodeinfo_p->node_ngroups = (bits + 31) / 32;
	nodeinfo_p->node_vec_size = bits;
	nodeinfo_p->node_sign = 0;
	break;
    case accRegister:
	nodeinfo_p->node_type = tf_reg_node;
	nodeinfo_p->node_value.vecval_p =
	    (struct t_vecval *) DECL_STORAGE(arg);
	nodeinfo_p->node_ngroups = (bits + 31) / 32;
	nodeinfo_p->node_vec_size = bits;
	nodeinfo_p->node_sign = 0;
	break;
    case accNet:
    case accPort:
	nodeinfo_p->node_type = tf_netvector_node;
	nodeinfo_p->node_value.vecval_p =
	    (struct t_vecval *) DECL_STORAGE(arg);
	nodeinfo_p->node_ngroups = (bits + 31) / 32;
	nodeinfo_p->node_vec_size = bits;
	nodeinfo_p->node_sign = 0;
	break;
    default:
	nodeinfo_p->node_type = tf_null_node;
	return nodeinfo_p;
    }

    nodeinfo_p->node_symbol = IDENT(DECL_NAME(arg));
    nodeinfo_p->node_handle = NULL;
    nodeinfo_p->node_mem_size = 0;

    return (nodeinfo_p);
}

/****************************************************************
	tf_strlongdelputp	
	tf_istrlongdelputp	
*****************************************************************/

int
tf_strlongdelputp(int nparam, int bitlength, int format_char,
		  char *value_p, int low, int high, int delaytype)
{
    return (tf_istrlongdelputp(nparam, bitlength, format_char,
			       value_p, low, high, delaytype,
			       tf_getinstance()));
}

int
tf_istrlongdelputp(int nparam, int bitlength, int format_char,
		   char *value_p, int low, int high, int delaytype,
		   char *instance)
{
    handle object;
    s_setval_delay delay;
    s_setval_value value;

    object = (handle) nth_arg(nparam, (tree) instance);
    if (object == NULL) {
	return (0);
    }

    delay.time.type = accTime;
    delay.time.low = low;
    delay.time.high = high;
    switch (delaytype) {
    case 0:
	delay.model = accInertialDelay;
	break;
    case 1:
	delay.model = accTransportDelay;
	break;
    case 2:
	delay.model = accPureTransportDelay;
	break;
    default:
	return (0);
    }

    switch (format_char) {
    case 'b':
    case 'B':
	value.format = accBinStrVal;
	break;
    case 'd':
    case 'D':
	value.format = accDecStrVal;
	break;
    case 'h':
    case 'H':
	value.format = accHexStrVal;
	break;
    case 'o':
    case 'O':
	value.format = accOctStrVal;
	break;
    default:
	return (0);
    }
    value.value.str = value_p;

    return (acc_set_value(object, &value, &delay));
}

/****************************************************************
	tf_strdelputp	
	tf_istrdelputp	
*****************************************************************/

int
tf_strdelputp(int nparam, int bitlength, int format_char,
	      char *value_p, int delay, int delaytype)
{
    return (tf_istrdelputp(nparam, bitlength, format_char,
			   value_p, delay, delaytype, tf_getinstance()));
}

int
tf_istrdelputp(int nparam, int bitlength, int format_char,
	       char *value_p, int delay, int delaytype, char *instance)
{
    return (tf_istrlongdelputp(nparam, bitlength, format_char,
			       value_p, delay, 0, delaytype, instance));
}

/****************************************************************
	tf_strrealdelputp	
	tf_istrrealdelputp	
*****************************************************************/

int
tf_strrealdelputp(int nparam, int bitlength, int format_char,
		  char *value_p, double delay, int delaytype)
{
    return (tf_istrrealdelputp(nparam, bitlength, format_char,
			       value_p, delay, delaytype,
			       tf_getinstance()));
}

int
tf_istrrealdelputp(int nparam, int bitlength, int format_char,
		   char *value_p, double delay, int delaytype,
		   char *instance)
{
    int high;
    int low;
    double scaledDelay;

    tf_scale_realdelay(instance, delay, &scaledDelay);
    tf_real_to_long(scaledDelay, &low, &high);
    tf_unscale_longdelay(instance, low, high, &low, &high);
    return (tf_istrlongdelputp(nparam, bitlength, format_char,
			       value_p, low, high, delaytype, instance));

}

/****************************************************************
	tf_getpchange
	tf_igetpchange
*****************************************************************/
int tf_getpchange(int nparam)
{
    return (tf_igetpchange(nparam, tf_getinstance()));
}

int tf_igetpchange(int nparam, char *instance)
{
    tree param;
    int index;
    PliInfo_t *info;

    index = nparam + 1;
    while (TRUE) {
	param = nth_parameter(index, (tree) instance);
	if (param == NULL) {
	    return (0);
	}
	info = (PliInfo_t *) TREE_PLIINFO(param);
	if (info->saved_pvc) {
	    return (index);
	}
	index++;
    }

}

/****************************************************************
	tf_copypvc_flag
	tf_icopypvc_flag
*****************************************************************/
int tf_copypvc_flag(int nparam)
{
    return (tf_icopypvc_flag(nparam, tf_getinstance()));
}

int tf_icopypvc_flag(int nparam, char *instance)
{
    tree param;
    int index;
    int acc;
    PliInfo_t *info;

    if (nparam == -1) {
	index = 1;
    } else {
	index = nparam;
    }

    acc = FALSE;
    while (TRUE) {
	param = nth_parameter(index, (tree) instance);
	if (param == NULL) {
	    break;
	}
	info = (PliInfo_t *) TREE_PLIINFO(param);
	info->saved_pvc = info->pvc;
	acc |= info->saved_pvc;
	if (nparam != -1) {
	    break;
	}
	index++;
    }

    return (acc);

}

/****************************************************************
	tf_movepvc_flag
	tf_imovepvc_flag
*****************************************************************/
int tf_movepvc_flag(int nparam)
{
    return (tf_imovepvc_flag(nparam, tf_getinstance()));
}

int tf_imovepvc_flag(int nparam, char *instance)
{
    tree param;
    int index;
    int acc;
    PliInfo_t *info;

    if (nparam == -1) {
	index = 1;
    } else {
	index = nparam;
    }

    acc = FALSE;
    while (TRUE) {
	param = nth_parameter(index, (tree) instance);
	if (param == NULL) {
	    break;
	}
	info = (PliInfo_t *) TREE_PLIINFO(param);
	info->saved_pvc = info->pvc;
	info->pvc = FALSE;
	acc |= info->saved_pvc;
	if (nparam != -1) {
	    break;
	}
	index++;
    }

    return (acc);

}

/****************************************************************
	tf_testpvc_flag
	tf_itestpvc_flag
*****************************************************************/
int tf_testpvc_flag(int nparam)
{
    return (tf_itestpvc_flag(nparam, tf_getinstance()));
}

int tf_itestpvc_flag(int nparam, char *instance)
{
    tree param;
    int index;
    int acc;
    PliInfo_t *info;

    if (nparam == -1) {
	index = 1;
    } else {
	index = nparam;
    }

    acc = FALSE;
    while (TRUE) {
	param = nth_parameter(index, (tree) instance);
	if (param == NULL) {
	    break;
	}
	info = (PliInfo_t *) TREE_PLIINFO(param);
	acc |= info->saved_pvc;
	if (nparam != -1) {
	    break;
	}
	index++;
    }

    return (acc);

}

/****************************************************************
	tf_getnextlongtime
*****************************************************************/
int tf_getnextlongtime(int *aof_lowtime, int *aof_hightime)
{
    tree gate;
    int timeh;
    int timel;
    int result;


    if (!roFlag) {
	timel = CurrentTime.timel;
	timeh = CurrentTime.timeh;
	result = 2;
	goto DONE;
    }

    gate = PeekGate();

    if (gate != NULL && timelist != NULL) {
	if (CMPTIME64(&GATE_TIMEQ(gate)->timeIndex, &timelist->time) > 0) {
	    gate = NULL;
	}
    }

    if (gate != NULL) {
	timel = GATE_TIMEQ(gate)->timeIndex.timel;
	timeh = GATE_TIMEQ(gate)->timeIndex.timeh;
	result = 0;
	goto DONE;
    }

    if (timelist != NULL) {
	timel = timelist->time.timel;
	timeh = timelist->time.timeh;
	result = 0;
	goto DONE;
    }



    timeh = 0;
    timel = 0;
    result = 1;
  DONE:

    /*
     * in this call time is unscaled (vcs behavior)
     * ieee 95 contradicts this
     */
    *aof_lowtime = timel;
    *aof_hightime = timeh;

    return result;
}

/****************************************************************

	acc_fetch_direction
		- return port or terminal direction type 

*****************************************************************/
int acc_fetch_direction(handle object)
{
    tree t = (tree) object;
    tree gate;

    acc_error_flag = FALSE;

    if (t == NULL_TREE || TREE_CODE(t) != TREE_LIST) {
	acc_error_flag = TRUE;
	return (0);
    }

    gate = GATE_TERMINAL_GATE(t);
    if (gate == NULL || TREE_CODE(gate) != GATE_INSTANCE) {
	t = TREE_PURPOSE(t);
    }

    if (PORT_INPUT_ATTR(t) && PORT_OUTPUT_ATTR(t)) {
	return (accInout);
    }
    if (PORT_INPUT_ATTR(t)) {
	return (accInput);
    }

    if (PORT_OUTPUT_ATTR(t)) {
	return (accOutput);
    }

    acc_error_flag = TRUE;
    return (0);

}

/****************************************************************

	acc_handle_tchk
		- return specified timing check 

*****************************************************************/

handle
acc_handle_tchk(handle module, int tchk_type, char *net1_name,
		int edge1, char *net2_name, int edge2, ...)
{
    handle tchk;
    handle object;
    char *name1;
    char *name2;
    va_list args;

    name1 = net1_name;
    name2 = net2_name;
    if (pli_info.config.accEnableArgsTchk_) {
	va_start(args, edge2);
	if (net1_name == NULL) {
	    object = va_arg(args, handle);
	    name1 = acc_fetch_name(object);
	}
	if (net2_name == NULL && tchk_type != accWidth &&
	    tchk_type != accPeriod) {
	    object = va_arg(args, handle);
	    name2 = acc_fetch_name(object);
	}
	va_end(args);
    }

    tchk = null;
    while (TRUE) {
	tchk = acc_next_tchk(module, tchk);
	if (tchk == null) {
	    return (null);
	}
	if (acc_fetch_fulltype(tchk) != tchk_type) {
	    continue;
	}
	if (strcmp
	    (name1,
	     acc_fetch_name(acc_handle_conn(acc_handle_tchkarg1(tchk))))
	    || (edge1 != 0
		&& TIMING_CHECK_EDGESPEC((tree) tchk, 0) != edge1)) {
	    continue;
	}
	if (name2 != NULL && name2[0] != 0 &&
	    (strcmp
	     (name2,
	      acc_fetch_name(acc_handle_conn(acc_handle_tchkarg2(tchk))))
	     || (edge2 != 0
		 && TIMING_CHECK_EDGESPEC((tree) tchk, 1) != edge2))) {
	    continue;
	}
	return (tchk);
    }
}

/****************************************************************

	acc_handle_tchkarg1
		- return first net of timing check 

*****************************************************************/

handle acc_handle_tchkarg1(handle tchk)
{
    tree instance;
    tree decl;

    instance = (tree) tchk;
    if (tchk == null || TREE_CODE(instance) != TIMING_CHECK) {
	return (null);
    }

    decl = TIMING_CHECK_EXPRESSION(instance, 0);
    if (decl != NULL && STMT_SURROGATE_ATTR(decl)) {
	decl = TREE_CHAIN(decl);
    }
    return ((handle) decl);
}


/****************************************************************

	acc_handle_tchkarg2
		- return first net of timing check 

*****************************************************************/

handle acc_handle_tchkarg2(handle tchk)
{
    tree instance;
    tree decl;

    instance = (tree) tchk;
    if (tchk == null || TREE_CODE(instance) != TIMING_CHECK) {
	return (null);
    }

    decl = TIMING_CHECK_EXPRESSION(instance, 1);
    if (decl != NULL && STMT_SURROGATE_ATTR(decl)) {
	decl = TREE_CHAIN(decl);
    }
    return ((handle) decl);
}



/****************************************************************

	acc_next_driver
		- return the next driver of net 

*****************************************************************/

handle acc_next_driver(handle net, handle driver)
{
    tree tnet = (tree) net;
    tree tdriver = (tree) driver;
    tree surrogate;
    tree ndriver;


    // make sure the net is not one of the surrogates

    if (STMT_SURROGATE_ATTR(tnet)) {
	tnet = TREE_CHAIN(tnet);
    }
    // if driver is null start with net as home
    // if driver is not null, scan net surrogate until current
    // driver is found.

    if (tdriver == NULL_TREE) {
	surrogate = tnet;
    } else {
	for (surrogate = tnet; surrogate;
	     surrogate = NET_SOURCE(surrogate)) {
	    ndriver = NET_ASSIGNMENT(surrogate);
	    if (ndriver == tdriver) {
		break;
	    }
	}
	ASSERT(surrogate != NULL_TREE);
    }

    // keep getting surrogates until a valid driver is found

    do {
	surrogate = NET_SOURCE(surrogate);
	if (surrogate == NULL) {
	    return (null);
	}

	ndriver = NET_ASSIGNMENT(surrogate);
	if (ndriver != null
	    && acc_fetch_type((handle) ndriver) != accTerminal) {
	    ndriver = NULL;
	}
    }
    while (ndriver == NULL);

    return ((handle) ndriver);
}


/****************************************************************

	acc_handle_loconn
		- return lo net connected to port 

*****************************************************************/
handle acc_handle_loconn(handle terminal)
{
    tree t = (tree) terminal;
    tree decl;

    acc_error_flag = FALSE;

    if (t == NULL) {
	acc_error_flag = TRUE;
	return NULL;
    }

    if (TREE_CODE(t) == TREE_LIST) {
	decl = TREE_PURPOSE(t);
    } else {
	decl = t;
    }

    if (PORT_REDEFINED_ATTR(decl)) {
	decl = DECL_THREAD(decl);
    }
// loconn will have to return registers
/*	if( TREE_CODE(decl) != NET_SCALAR_DECL &&
	    TREE_CODE(decl) != NET_VECTOR_DECL ) {
		return NULL;
	}
*/
    if (decl != NULL && STMT_SURROGATE_ATTR(decl)) {
	decl = TREE_CHAIN(decl);
    }
    return ((handle) decl);
}

/****************************************************************

	acc_fetch_index

*****************************************************************/
int acc_fetch_index(handle object)
{
    tree t = (tree) object;
    tree gate;
    tree decl;
    tree module;
    int pin;
    tree port;

    acc_error_flag = FALSE;

    if (t == NULL_TREE || TREE_CODE(t) != TREE_LIST) {
	acc_error_flag = TRUE;
	return (0);
    }

    gate = GATE_TERMINAL_GATE(t);
    if (gate != NULL && TREE_CODE(gate) == GATE_INSTANCE) {
	return (GATE_TERMINAL_NUMBER(t));
    }

    /* must be a port */

    decl = TREE_PURPOSE(t);
    module = DECL_CONTEXT(decl);
    pin = 0;
    if (acc_fetch_type((handle) module) == accModule) {
	port = MODULE_PORT_LIST(module);
    } else {
	port = BLOCK_PORTS(module);
    }
    for (; port != NULL && port != t; port = TREE_CHAIN(port), pin++);


    if (port == NULL) {
	acc_error_flag = TRUE;
    }

    return (pin);

}

/****************************************************************

	acc_next_terminal
		- return the next terminal of primitive 

*****************************************************************/

handle acc_next_terminal(handle primitive, handle terminal)
{
    tree next;

    acc_error_flag = FALSE;

    if (terminal == NULL) {
	next = GATE_OUTPUT_LIST((tree) primitive);
    } else {

	next = TREE_CHAIN((tree) terminal);
	if (next == NULL_TREE && PORT_OUTPUT_ATTR((tree) terminal)) {
	    next = GATE_INPUT_LIST((tree) primitive);
	}
	// skip inout that are in input list
	while (next != NULL && !PORT_OUTPUT_ATTR(next) &&
	       !PORT_INPUT_ATTR(next)) {
	    next = TREE_CHAIN(next);

	}
    }
    return ((handle) next);


}

/****************************************************************

	acc_handle_path

*****************************************************************/
handle acc_handle_path(handle port_output_handle, handle port_input_handle)
{
    acc_error_flag = FALSE;
    return null;
}

/****************************************************************

	acc_handle_terminal

*****************************************************************/
handle acc_handle_terminal(handle primitive_handle, int terminal_index)
{
    handle terminal;
    int count;

    terminal = null;
    for (count = terminal_index; count >= 0; count--) {
	terminal = acc_next_terminal(primitive_handle, terminal);
	if (terminal == null) {
	    break;
	}
    }
    return (terminal);
}

/****************************************************************

	acc_handle_modpath
		- retrieve a given module path 

*****************************************************************/

handle
acc_handle_modpath(handle module, char *src_name, char *dest_name, ...)
{
    handle path;
    handle src;
    handle dest;
    va_list args;
    handle inpath;
    handle outpath;

    acc_error_flag = FALSE;

    va_start(args, dest_name);
    if (pli_info.config.accEnableArgsModpath_) {
	src = va_arg(args, handle);
	dest = va_arg(args, handle);
    }
    va_end(args);

    if (src_name != NULL) {
	src = acc_handle_by_name(src_name, module);
    }
    if (dest_name != NULL) {
	dest = acc_handle_by_name(dest_name, module);
    }

    if (src == NULL || dest == NULL) {
	acc_error_flag = TRUE;
	return NULL;
    }

    path = NULL;
    for (path = acc_next_modpath(module, path);
	 path != NULL; path = acc_next_modpath(module, path)) {
	inpath = acc_handle_pathin(path);
	outpath = acc_handle_pathout(path);
	if (!acc_compare_handles(src, inpath)) {
	    continue;
	}
	if (acc_compare_handles(dest, outpath)) {
	    return path;
	}
    }

    acc_error_flag = TRUE;
    return NULL;

}


/****************************************************************

	acc_handle_pathin
		- retrieve first input of module path 

*****************************************************************/

handle acc_handle_pathin(handle mod_path)
{
    handle terminal;
    handle net;

    acc_error_flag = FALSE;

    terminal = acc_next_input(mod_path, null);
    net = acc_handle_conn(terminal);
    acc_release_object(terminal);
    return net;

#if 0
    tree pathInstance;
    tree pathDesc;
    tree instance;
    tree constraint;
    tree decl;
    tree module;

    acc_error_flag = FALSE;

    pathInstance = (tree) mod_path;
    if (TREE_CODE(pathInstance) != PATH_INSTANCE) {
	acc_error_flag = TRUE;
	return (null);
    }

    pathDesc = PATH_INSTANCE_PATHDESC(pathInstance);
    ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

    module = PATH_INSTANCE_MODULE(pathInstance);
    ASSERT(TREE_CODE(module) == MODULE_BLOCK);

    for (instance = MODULE_SPECINST(module);
	 instance != NULL_TREE; instance = TREE_CHAIN(instance)) {

	if (TREE_CODE(instance) != PATH_OUTPUT) {
	    continue;
	}


	for (constraint = PATH_OUTPUT_CONSTRAINTS(instance);
	     constraint != NULL_TREE; constraint = TREE_CHAIN(constraint))
	{

	    ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
	    if (PATH_CONSTRAINT_PATHDESC(constraint) == pathDesc) {
		decl =
		    TREE_PURPOSE(PATH_CONSTRAINT_DEPENDENCIES(constraint));
		if (STMT_SURROGATE_ATTR(decl)) {
		    decl = TREE_CHAIN(decl);
		}
		return ((handle) decl);
	    }
	}
    }

    /* this is an error case */

    acc_error_flag = TRUE;
    return (NULL);
#endif
}

/****************************************************************

	acc_handle_pathout
		- retrieve first output of module path 

*****************************************************************/

handle acc_handle_pathout(handle mod_path)
{
    handle terminal;
    handle net;

    acc_error_flag = FALSE;

    terminal = acc_next_output(mod_path, null);
    net = acc_handle_conn(terminal);
    acc_release_object(terminal);
    return net;
#if 0
    tree pathInstance;
    tree pathDesc;
    tree instance;
    tree constraint;
    tree decl;
    tree module;
    tree gate;

    acc_error_flag = FALSE;

    pathInstance = (tree) mod_path;
    if (TREE_CODE(pathInstance) != PATH_INSTANCE) {
	acc_error_flag = TRUE;
	return (null);
    }

    pathDesc = PATH_INSTANCE_PATHDESC(pathInstance);
    ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

    module = PATH_INSTANCE_MODULE(pathInstance);
    ASSERT(TREE_CODE(module) == MODULE_BLOCK);

    for (instance = MODULE_SPECINST(module);
	 instance != NULL_TREE; instance = TREE_CHAIN(instance)) {

	if (TREE_CODE(instance) != PATH_OUTPUT) {
	    continue;
	}


	for (constraint = PATH_OUTPUT_CONSTRAINTS(instance);
	     constraint != NULL_TREE; constraint = TREE_CHAIN(constraint))
	{

	    ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
	    if (PATH_CONSTRAINT_PATHDESC(constraint) == pathDesc) {
		gate = PATH_OUTPUT_GATE(instance);
		ASSERT(TREE_CODE(gate) == GATE_INSTANCE);
		ASSERT(TREE_CODE(GATE_OUTPUT_LIST(gate)) == TREE_LIST);
		decl = TREE_PURPOSE(GATE_OUTPUT_LIST(gate));
		if (STMT_SURROGATE_ATTR(decl)) {
		    decl = TREE_CHAIN(decl);
		}
		return ((handle) decl);
	    }
	}
    }
    /* this is an error case */

    acc_error_flag = TRUE;
    return (NULL);

#endif

}

/****************************************************************

	acc_next_nput

*****************************************************************/

handle acc_next_input(handle mod_path, handle terminal)
{
    tree pathInstance;
    tree pathDesc;
    tree instance;
    tree constraint;
    tree module;

    acc_error_flag = FALSE;

    pathInstance = (tree) mod_path;
    if (TREE_CODE(pathInstance) != PATH_INSTANCE) {
	acc_error_flag = TRUE;
	return (null);
    }

    pathDesc = PATH_INSTANCE_PATHDESC(pathInstance);
    ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

    module = PATH_INSTANCE_MODULE(pathInstance);
    ASSERT(TREE_CODE(module) == MODULE_BLOCK);

    if (terminal == NULL) {
	for (instance = MODULE_SPECINST(module);
	     instance != NULL_TREE; instance = TREE_CHAIN(instance)) {

	    if (TREE_CODE(instance) != PATH_OUTPUT) {
		continue;
	    }

	    for (constraint = PATH_OUTPUT_CONSTRAINTS(instance);
		 constraint != NULL_TREE;
		 constraint = TREE_CHAIN(constraint)) {

		ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
		if (PATH_CONSTRAINT_INSTANCE(constraint) == pathInstance) {
		    return (handle) PATH_CONSTRAINT_DEPENDENCIES((tree)
								 constraint);
		}
	    }
	}
    } else {
	return ((handle) TREE_CHAIN((tree) terminal));
    }

    /* this is an error case */

    acc_error_flag = TRUE;
    return (NULL);
}

/****************************************************************

	acc_next_output

*****************************************************************/

handle acc_next_output(handle mod_path, handle terminal)
{
    tree pathInstance;
    tree pathDesc;
    tree instance;
    tree constraint;
    tree module;

    acc_error_flag = FALSE;

    pathInstance = (tree) mod_path;
    if (TREE_CODE(pathInstance) != PATH_INSTANCE) {
	acc_error_flag = TRUE;
	return (null);
    }

    pathDesc = PATH_INSTANCE_PATHDESC(pathInstance);
    ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

    module = PATH_INSTANCE_MODULE(pathInstance);
    ASSERT(TREE_CODE(module) == MODULE_BLOCK);

    for (instance = MODULE_SPECINST(module);
	 instance != NULL_TREE; instance = TREE_CHAIN(instance)) {

	if (TREE_CODE(instance) != PATH_OUTPUT) {
	    continue;
	}

	for (constraint = PATH_OUTPUT_CONSTRAINTS(instance);
	     constraint != NULL_TREE; constraint = TREE_CHAIN(constraint))
	{

	    ASSERT(TREE_CODE(constraint) == PATH_CONSTRAINT);
	    if (PATH_CONSTRAINT_INSTANCE(constraint) == pathInstance) {
		if (terminal == NULL &&
		    PATH_CONSTRAINT_OUTPUT_NUMBER(constraint) == 0) {
		    return ((handle) constraint);
		} else if (terminal != NULL &&
			   PATH_CONSTRAINT_OUTPUT_NUMBER((tree) terminal) +
			   1 ==
			   PATH_CONSTRAINT_OUTPUT_NUMBER(constraint)) {
		    return ((handle) constraint);
		}
	    }
	}
    }

    /* this is an error case */

    acc_error_flag = TRUE;
    return (NULL);
}

/****************************************************************

	resolve_path	
		- find next item in path 

*****************************************************************/
static int resolveList[] = {
    accIntegerVar,
    accModule,
    accNamedEvent,
    accNet,
    accPrimitive,
    accRealVar,
    accRegister,
    accTimeVar,
    accParameter,
    accSpecparam,
    0
};

static handle resolve_path(handle anchor, char *path)
{
    handle object;
    char *startPtr;
    char *endPtr;
    int length;


    startPtr = path;
    endPtr = strchr(startPtr, '.');
    if (endPtr == NULL) {
	length = strlen(startPtr);
	endPtr = startPtr + length;
    } else {
	length = endPtr - startPtr;
	endPtr++;
    }

    if (length <= 0) {
	return (anchor);
    }

    object = NULL;
    for (object = acc_next_child(anchor, object);
	 object != NULL; object = acc_next_child(anchor, object)) {
	char *name = acc_fetch_name(object);
	if (!strncmp(startPtr, name, length) && strlen(name) == length) {
	    return (resolve_path(object, endPtr));
	}
    }

    if (*endPtr != 0) {
	return (NULL);
    }

    object = NULL;
    for (object = acc_next(resolveList, anchor, object);
	 object != NULL; object = acc_next(resolveList, anchor, object)) {
	char *name = acc_fetch_name(object);
	if (!strncmp(startPtr, name, length) && strlen(name) == length) {
	    return (object);
	}
    }

    return (NULL);

}



/****************************************************************

	acc_handle_by_name	
		- return  the handle to named object in given scope

 ****************************************************************/

handle acc_handle_by_name(char *path, handle scope)
{
    handle object;
    char *startPtr;
    char *endPtr;
    int length;


    acc_error_flag = FALSE;

    /* search scope */

    if (scope != NULL) {
	object = resolve_path(scope, path);
	if (object != NULL) {
	    return (object);
	}
    } else {

	/* start resolution at top level */

	startPtr = path;
	endPtr = strchr(startPtr, '.');
	if (endPtr == NULL) {
	    length = strlen(startPtr);
	    endPtr = startPtr + length;
	} else {
	    length = endPtr - startPtr;
	    endPtr++;
	}

	object = NULL;
	for (object = acc_next_topmod(object);
	     object != NULL; object = acc_next_topmod(object)) {
	    char *name = acc_fetch_name(object);
	    if (!strncmp(startPtr, name, length) && strlen(name) == length) {
		return (resolve_path(object, endPtr));
	    }
	}
    }

    acc_error_flag = TRUE;
    return (NULL);
}

/****************************************************************

	acc_release_object	

 ****************************************************************/
int acc_release_object(handle object)
{
    acc_error_flag = FALSE;
    return TRUE;
}

/****************************************************************

	acc_handle_datapath	

 ****************************************************************/
handle acc_handle_datapath(handle modpath)
{
    // data paths are not supported
    acc_error_flag = FALSE;
    return null;
}

/****************************************************************

	acc_next_loconn	

 ****************************************************************/
handle acc_next_loconn(handle port, handle bit)
{
    // vector net expansion is not supported
    acc_error_flag = TRUE;
    return null;
}

/****************************************************************

	acc_next_hiconn	

 ****************************************************************/
handle acc_next_hiconn(handle port, handle bit)
{
    // vector net expansion is not supported
    acc_error_flag = TRUE;
    return null;
}


/****************************************************************

	acc_fetch_polarity

*****************************************************************/
int acc_fetch_polarity(handle object)
{
    tree pathNode;

    acc_error_flag = FALSE;

    pathNode = PATH_INSTANCE_PATHDESC((tree) object);
    switch (PATH_POLARITY(pathNode)) {
    case 0:
	return accUnknown;
    case 1:
	return accPositive;
    case 2:
	return accNegative;
    }
    acc_error_flag = TRUE;
    return 0;

}

/****************************************************************

	acc_fetch_edge

*****************************************************************/
int acc_fetch_edge(handle object)
{
    tree t = (tree) object;
    tree timingCheck;
    tree instance;
    tree pathNode;
    tree constraint;

    acc_error_flag = FALSE;

    if (t == NULL_TREE ||
	(TREE_CODE(t) != TREE_LIST && TREE_CODE(t) != PATH_CONSTRAINT)) {
	acc_error_flag = TRUE;
	return (0);
    }

    if (TREE_CODE(t) == PATH_CONSTRAINT) {
	instance = PATH_CONSTRAINT_INSTANCE(t);
	pathNode = PATH_INSTANCE_PATHDESC(instance);
	return (PATH_EDGE(pathNode));
    }

    constraint = MODPATH_INPUT_TERMINAL_PARENT((tree) object);
    if (constraint != NULL && TREE_CODE(constraint) == PATH_CONSTRAINT) {
	instance = PATH_CONSTRAINT_INSTANCE(constraint);
	pathNode = PATH_INSTANCE_PATHDESC(instance);
	return (PATH_EDGE(pathNode));
    }


    timingCheck = TIMING_TERMINAL_CHECK(t);
    if (timingCheck != NULL && TREE_CODE(timingCheck) == TIMING_CHECK) {
	if (t == TIMING_CHECK_EXPRESSION(t, 0)) {
	    return (TIMING_CHECK_EDGESPEC(t, 0));
	} else if (t == TIMING_CHECK_EXPRESSION(t, 1)) {
	    return (TIMING_CHECK_EDGESPEC(t, 1));
	}
	acc_error_flag = TRUE;
	return (0);
    }

    acc_error_flag = TRUE;
    return (0);

}

/****************************************************************

	acc_handle_condition

*****************************************************************/
handle acc_handle_condition(handle object)
{
    tree t = (tree) object;
    tree timingCheck;
    tree instance;
    tree pathNode;
    tree constraint;

    acc_error_flag = FALSE;

    if (t == NULL_TREE || (TREE_CODE(t) != TREE_LIST &&
			   TREE_CODE(t) != PATH_CONSTRAINT)) {
	acc_error_flag = TRUE;
	return (NULL);
    }

    if (TREE_CODE(t) == PATH_CONSTRAINT) {
	instance = PATH_CONSTRAINT_INSTANCE(t);
	pathNode = PATH_INSTANCE_PATHDESC(instance);
	return ((handle) PATH_CONDITION(pathNode));
    }

    constraint = MODPATH_INPUT_TERMINAL_PARENT((tree) object);
    if (constraint != NULL && TREE_CODE(constraint) == PATH_CONSTRAINT) {
	instance = PATH_CONSTRAINT_INSTANCE(constraint);
	pathNode = PATH_INSTANCE_PATHDESC(instance);
	return ((handle) PATH_CONDITION(pathNode));
    }


    timingCheck = TIMING_TERMINAL_CHECK(t);
    if (timingCheck != NULL && TREE_CODE(timingCheck) == TIMING_CHECK) {
	if (t == TIMING_CHECK_EXPRESSION(t, 0)) {
	    return ((handle) TIMING_CHECK_CONDITION(t, 0));
	} else if (t == TIMING_CHECK_EXPRESSION(t, 1)) {
	    return ((handle) TIMING_CHECK_CONDITION(t, 1));
	}
	acc_error_flag = TRUE;
	return (NULL);
    }

    acc_error_flag = TRUE;
    return (NULL);

}

/****************************************************************

	acc_handle_notifier

*****************************************************************/
handle acc_handle_notifier(handle object)
{
    tree t = (tree) object;

    acc_error_flag = FALSE;

    if (t == NULL_TREE || TREE_CODE(t) != TIMING_CHECK) {
	acc_error_flag = TRUE;
	return (NULL);
    }

    return ((handle) TIMING_CHECK_NOTIFIER(t));

}

 /****************************************************************
	acc_fetch_location
*****************************************************************/

void acc_fetch_location(p_location loc_p, handle object)
{
    int fulltype;

    switch (acc_fetch_type(object)) {
    case accTask:
    case accFunction:
    case accModule:
	loc_p->line_no = BLOCK_SOURCE_LINE((tree) object);
	loc_p->filename = BLOCK_SOURCE_FILE((tree) object);
	break;
    case accPort:
	acc_fetch_location(loc_p, acc_handle_parent(object));
	break;
    case accPrimitive:
	fulltype = acc_fetch_fulltype(object);
	switch (fulltype) {
	case accCombPrim:
	case accSeqPrim:	// not sure about this
	    loc_p->line_no =
		BLOCK_SOURCE_LINE(GATE_UDP_DEF((tree) object));
	    loc_p->filename =
		BLOCK_SOURCE_FILE(GATE_UDP_DEF((tree) object));
	    break;
	default:
	    loc_p->filename = GATE_FILE((tree) object);
	    loc_p->line_no = GATE_LINE((tree) object);
	    break;
	}
	break;
    case accPath:
	loc_p->filename = PATH_SOURCE_FILE((tree) object);
	loc_p->line_no = PATH_SOURCE_LINE((tree) object);
	break;
    case accNamedEvent:
	loc_p->line_no = STMT_SOURCE_LINE((tree) object);
	loc_p->filename = STMT_SOURCE_FILE((tree) object);
	break;
    case accNet:
    case accNetBit:
    case accRealVar:
    case accRegister:
    case accRegBit:
    case accTimeVar:
    case accIntegerVar:
    case accParameter:
    case accSpecparam:
	loc_p->filename = DECL_SOURCE_FILE((tree) object);
	loc_p->line_no = DECL_SOURCE_LINE((tree) object);
	break;
    case accPortBit:
    case accStatement:
    case accTchkTerminal:
    case accTerminal:
    case accTchk:
    case accPathTerminal:
    case accPartSelect:
    case accDataPath:
    case accConstant:
    case accOperator:
    default:
	break;
    }
}

/****************************************************************
	acc_fetch_attribute	
*****************************************************************/
double acc_fetch_attribute(handle object, char *string, double def)
{
    acc_error_flag = TRUE;
    tf_error("acc_fetch_attribute() is not supported\n");
    return def;
}


/****************************************************************
	acc_fetch_attribute_int	
*****************************************************************/
int acc_fetch_attribute_int(handle object, char *string, int def)
{
    acc_error_flag = TRUE;
    tf_error("acc_fetch_attribute() is not supported\n");
    return def;
}

/****************************************************************
	acc_fetch_attribute_str	
*****************************************************************/
char *acc_fetch_attribute_str(handle object, char *string, char *def)
{
    acc_error_flag = TRUE;
    tf_error("acc_fetch_attribute() is not supported\n");
    return def;
}

/****************************************************************
	handleLcb	
*****************************************************************/
void handleLcb(tree pc)
{
    tree block;
    pli_lcb *lcb;

    switch (*tree_code_type[TREE_CODE(pc)]) {
    case 'd':
	block = DECL_CONTEXT(pc);
	break;
    case 's':
	block = STMT_SCOPE(pc);
	break;
    default:
	return;
    }
    for (; block &&
	 TREE_CODE(block) != MODULE_BLOCK &&
	 TREE_CODE(block) != FUNCTION_BLOCK &&
	 TREE_CODE(block) != TASK_BLOCK; block = BLOCK_UP(block));
    if (block == NULL) {
	return;
    }
    lcb = (pli_lcb *) BLOCK_LCB_LIST(block);
    while (lcb) {
	(*lcb->routine) (STMT_SOURCE_FILE(pc),
			 STMT_SOURCE_LINE(pc), lcb->userdata);
	lcb = lcb->next;
    }
}

 /****************************************************************
	acc_mod_lcb_add	
*****************************************************************/

void
acc_mod_lcb_add(handle module,
		void (*routine) (char *, unsigned int, char *),
		char *userdata)
{
    pli_lcb *lcb;

    acc_error_flag = FALSE;

    for (lcb = (pli_lcb *) BLOCK_LCB_LIST((tree) module); lcb;
	 lcb = lcb->next) {
	if (lcb->module == (tree) module && lcb->routine == routine
	    && lcb->userdata == userdata) {
	    tf_error("lcb already exists in acc_mod_lcb_add()");
	    acc_error_flag = TRUE;
	    return;
	}
    }

    lcb = (pli_lcb *) xmalloc(sizeof(pli_lcb));
    lcb->routine = routine;
    lcb->userdata = userdata;
    lcb->module = (tree) module;
    lcb->next = (pli_lcb *) BLOCK_LCB_LIST((tree) module);
    BLOCK_LCB_LIST((tree) module) = (tree) lcb;
    lcbCount++;
}

 /****************************************************************
	acc_mod_lcb_delete	
*****************************************************************/

void
acc_mod_lcb_delete(handle module,
		   void (*routine) (char *, unsigned int, char *),
		   char *userdata)
{
    pli_lcb *lcb;
    pli_lcb **pptr;


    acc_error_flag = FALSE;

    for (pptr = ((pli_lcb **) & BLOCK_LCB_LIST((tree) module)), lcb =
	 *pptr; lcb; pptr = &lcb->next, lcb = lcb->next) {
	if (lcb->module == (tree) module && lcb->routine == routine
	    && lcb->userdata == userdata) {
	    break;
	}
    }

    if (lcb == NULL) {
	tf_error("lcb in acc_mod_lcb_delete() does not exist");
	acc_error_flag = TRUE;
	return;
    }

    *pptr = lcb->next;
    free(lcb);
    lcbCount--;

}



/************************************************************************
 * acc_fetch_name							*
/***********************************************************************/

char *acc_fetch_name(handle object)
{
    acc_error_flag = FALSE;

    if (object == NULL) {
	acc_error_flag = TRUE;
	tf_error("Illegal object in acc_fetch_name()");
	return (NULL);
    }

    pli_write_string(NULL);	// start a new string
    return (append_name(object));
}

/************************************************************************
 * append_name							
 * 		-	append name to pli string, return
 *			pointer to start of string
/***********************************************************************/

char *append_name(handle object)
{
    char *result;
    char buffer[1024];
    tree t;

    result = NULL;
    switch (TREE_CODE((tree) object)) {
    case PATH_CONSTRAINT:
	return (append_name(acc_handle_conn(object)));

    case IDENTIFIER_NODE:
	return (pli_append_string(IDENT((tree) object)));

    case TREE_LIST:
	object = (handle) TREE_PURPOSE((tree) object);
	break;
//      case    TIMING_CHECK:
//          result = append_name( acc_handle_conn( 
//              acc_handle_tchkarg1(object) ) );    
//          result = pli_append_string( pli_info.config.accPathDelimStr_ );
//          if( acc_handle_tchkarg2(object) != null ) {
//              result = append_name( acc_handle_conn( 
//                  acc_handle_tchkarg2(object) ) );    
//          }
//          return( result );
    case PATH_INSTANCE:
	{
	    tree pathDesc;
	    tree t;
	    tree decl;

	    pathDesc = PATH_INSTANCE_PATHDESC((tree) object);
	    ASSERT(pathDesc != NULL_TREE);
	    ASSERT(TREE_CODE(pathDesc) == PATH_NODE);

	    t = PATH_OUTPUTS(pathDesc);
	    ASSERT(t != NULL_TREE);
	    ASSERT(TREE_CODE(t) == TREE_LIST);

	    decl = TREE_PURPOSE(t);
	    ASSERT(decl != NULL_TREE);

	    result = append_name((handle) decl);
	    result = pli_append_string(pli_info.config.accPathDelimStr_);

	    t = PATH_INPUTS(pathDesc);
	    ASSERT(t != NULL_TREE);
	    ASSERT(TREE_CODE(t) == TREE_LIST);

	    decl = TREE_PURPOSE(t);
	    ASSERT(decl != NULL_TREE);

	    result = append_name((handle) decl);
	}
	return (result);
    }


    t = (tree) object;
    switch (*tree_code_type[TREE_CODE(t)]) {
    case 'd':			/* decl nodes */
	return pli_append_string(IDENT(DECL_NAME((tree) object)));
    case 'b':			/* blocks */
	return pli_append_string(IDENT(BLOCK_NAME((tree) object)));
    case 'g':			/* primitive */
	return pli_append_string(GATE_NAME((tree) object));
    case 'r':			/* storage reference */
	switch (TREE_CODE(t)) {
	case PART_REF:
	    {
		tree name = PART_NAME(t);

		if (TREE_CODE(name) != IDENTIFIER_NODE) {
		    name = DECL_NAME(name);
		}

		sprintf(buffer, "%s[%d:%d]",
			IDENT(name),
			get_range(PART_MSB_(t), IDENT(name)),
			get_range(PART_LSB_(t), IDENT(name)));
	    }
	    return (pli_append_string(buffer));
	case BIT_REF:
	    {
		tree name = BIT_REF_NAME(t);

		if (TREE_CODE(name) != IDENTIFIER_NODE) {
		    name = DECL_NAME(name);
		}

		sprintf(buffer, "%s[%d]",
			IDENT(name), get_range(BIT_EXPR(t), IDENT(name)));
	    }
	    return (pli_append_string(buffer));
	default:
	    return NULL;
	}

    default:
	return NULL;
    }
}

/************************************************************************
 * acc_handle_calling_mod_m
/***********************************************************************/

handle acc_handle_calling_mod_m(void)
{
    return (acc_handle_parent((handle) tf_getinstance()));
}

/*******************************************
 mdh - left off here
********************************************/


/************************************************************************
 * acc_set_value                            *
/***********************************************************************/

int
acc_set_value(handle object, p_setval_value value_p,
	      p_setval_delay delay_p)
{
    time64 delay;
    double scaledDelay;
    Group *g;
    tree constant;
    int nbits;
    int ngroups;
    Group g1;
    tree t = (tree) object;
    int type;
    int fulltype;
    tree stmt;
    SCB *scb;
    time64 newTime;

    acc_error_flag = FALSE;

    // calculate delay first
    switch (delay_p->time.type) {
    case accTime:
	tf_scale_longdelay(tf_getinstance(), delay_p->time.low,
			   delay_p->time.high, (int *) &delay.timel,
			   (int *) &delay.timeh);
	break;
    case accSimTime:
	delay.timeh = delay_p->time.high;
	delay.timel = delay_p->time.low;
	break;
    case accRealTime:
	tf_scale_realdelay(tf_getinstance(), delay_p->time.real,
			   &scaledDelay);
	tf_real_to_long(scaledDelay, (int *) &delay.timel,
			(int *) &delay.timeh);
	break;
    default:
	tf_error("Illegal time type in acc_set_value()");
	acc_error_flag = TRUE;
	return 1;
    }


    fulltype = acc_fetch_fulltype(object);
    type = acc_fetch_type(object);

    // get pointer to storage area

    switch (delay_p->model) {
    case accNoDelay:
	if (type != accRegister && fulltype != accSeqPrim) {
	    tf_error
		("Object must be either a register or sequential UDP in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
	if (fulltype == accSeqPrim) {
	    nbits = 1;
	    ngroups = 1;
	    g = &g1;
	} else {
	    nbits = acc_fetch_size(object);
	    ngroups = (nbits + 31) / 32;
	    g = DECL_STORAGE(t);
	}
	break;
    case accPureTransportDelay:
	if (type != accRegister) {
	    tf_error("Object must be a register in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
	nbits = acc_fetch_size(object);
	ngroups = (nbits + 31) / 32;
	g = malloc_Z(ngroups);
	break;
    case accAssignFlag:
	if (type != accRegister) {
	    tf_error("Object must be a register in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
	nbits = acc_fetch_size(object);
	ngroups = (nbits + 31) / 32;
	constant = build_bit_cst(nbits, BIN);
	g = BIT_CST_GROUP(constant);
	break;
    case accDeassignFlag:
	if (type != accRegister) {
	    tf_error("Object must be a register in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
	nbits = acc_fetch_size(object);
	ngroups = (nbits + 31) / 32;
	g = NULL;
	break;
    case accForceFlag:
	if (type != accRegister && type != accNet) {
	    tf_error
		("Object must be either a register or net in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
	nbits = acc_fetch_size(object);
	ngroups = (nbits + 31) / 32;
	constant = build_bit_cst(nbits, BIN);
	g = BIT_CST_GROUP(constant);
	break;
    case accReleaseFlag:
	if (type != accRegister && type != accNet) {
	    tf_error
		("Object must be either a register or net in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
	nbits = acc_fetch_size(object);
	ngroups = (nbits + 31) / 32;
	g = NULL;
	break;

    case accInertialDelay:
	tf_error("accInertialDelay not supported in acc_set_value()");
	acc_error_flag = TRUE;
	return 1;
    case accTransportDelay:
	tf_error("accTransportDelay not supported in acc_set_value()");
	acc_error_flag = TRUE;
	return 1;
    default:
	tf_error("Illegal time model in acc_set_value()");
	acc_error_flag = TRUE;
	return 1;
    }

    // parse input value

    if (g)
	switch (value_p->format) {
	case accScalarVal:
	    if (ngroups > 1) {
		memset(g, 0, ngroups * sizeof(Group));
	    }
	    switch (value_p->value.scalar) {
	    case acc0:
		AVAL(g) = 0;
		BVAL(g) = 0;
		break;
	    case acc1:
		AVAL(g) = 1;
		BVAL(g) = 0;
		break;
	    case accX:
		AVAL(g) = 1;
		BVAL(g) = 1;
		break;
	    case accZ:
		AVAL(g) = 0;
		BVAL(g) = 1;
		break;
	    }
	    break;
	case accVectorVal:
	    memcpy(g, value_p->value.vector, ngroups * sizeof(Group));
	    break;
	case accIntVal:
	    AVAL(g) = value_p->value.integer;
	    BVAL(g) = 0;
	    break;
	case accRealVal:
	    REAL_(g) = value_p->value.real;
	    break;
	case accStringVal:
	    StringToGroup(value_p->value.str, g, nbits, ngroups);
	    break;
	case accBinStrVal:
	    parse_base_const1g(BIN,
			       value_p->value.str +
			       strlen(value_p->value.str) - 1, nbits,
			       strlen(value_p->value.str), g);
	    break;
	case accOctStrVal:
	    parse_base_const1g(OCT,
			       value_p->value.str +
			       strlen(value_p->value.str) - 1, nbits,
			       strlen(value_p->value.str), g);
	    break;
	case accDecStrVal:
	    parse_base_const1g(DEC,
			       value_p->value.str +
			       strlen(value_p->value.str) - 1, nbits,
			       strlen(value_p->value.str), g);
	    break;
	case accHexStrVal:
	    parse_base_const1g(HEX,
			       value_p->value.str +
			       strlen(value_p->value.str) - 1, nbits,
			       strlen(value_p->value.str), g);
	    break;
	default:
	    tf_error("Illegal format in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	}
    // apply new value


    switch (delay_p->model) {
    case accNoDelay:
	if (fulltype == accSeqPrim) {
	    tf_error("accSeqPrim are not supported in acc_set_value()");
	    acc_error_flag = TRUE;
	    return 1;
	} else {
	    notify(t, DECL_STATE(t), 1, NULL);
	}
	break;
    case accPureTransportDelay:
	if (delayFreeList) {
	    stmt = delayFreeList;
	    delayFreeList = PLI_DELAY_NEXT(stmt);
	} else {
	    stmt = make_node(PLI_DELAY);
	}

	ADDTIME6464(&newTime, &CurrentTime, &delay);
	PLI_DELAY_TIME(stmt) = newTime;

	PLI_DELAY_LVAL(stmt) = t;
	PLI_DELAY_STORAGE(stmt) = g;
	TREE_NBITS(stmt) = nbits;

	PLI_DELAY_NEXT(stmt) = NULL;
	scb = BuildSCB(stmt, NOLIST);

	Schedule64(&newTime, scb, FALSE);
	break;
    case accAssignFlag:
	stmt = make_node(ASSIGN_PROC_STMT);
	TREE_CHAIN(stmt) = dump;
	STMT_ASSIGN_RVAL(stmt) = constant;
	STMT_ASSIGN_LVAL(stmt) = t;
	scb = BuildSCB(stmt, NOLIST);
	pass3_assignment(stmt);
	Schedule64(&CurrentTime, scb, FALSE);
	break;
    case accDeassignFlag:
	stmt = make_node(DEASSIGN_STMT);
	TREE_CHAIN(stmt) = dump;
	STMT_BODY(stmt) = t;
	scb = BuildSCB(stmt, NOLIST);
	Schedule64(&CurrentTime, scb, FALSE);
	break;
    case accForceFlag:
	stmt = make_node(FORCE_STMT);
	TREE_CHAIN(stmt) = dump;
	STMT_ASSIGN_RVAL(stmt) = constant;
	STMT_ASSIGN_LVAL(stmt) = t;
	scb = BuildSCB(stmt, NOLIST);
	pass3_assignment(stmt);
	Schedule64(&CurrentTime, scb, FALSE);
	break;
    case accReleaseFlag:
	stmt = make_node(RELEASE_STMT);
	TREE_CHAIN(stmt) = dump;
	STMT_BODY(stmt) = t;
	scb = BuildSCB(stmt, NOLIST);
	Schedule64(&CurrentTime, scb, FALSE);
	break;
    case accInertialDelay:
	tf_error("accInertialDelay is not supported in acc_set_value()");
	acc_error_flag = TRUE;
	return 1;
    case accTransportDelay:
	tf_error("accTransportDelay is not supported in acc_set_value()");
	acc_error_flag = TRUE;
	return 1;
    }

    return 0;
}


/****************************************************************

	acc_handle_hiconn
		- return hi net connected to port 

*****************************************************************/
handle acc_handle_hiconn(handle terminal)
{
    tree t = (tree) terminal;

    tf_error("acc_handle_hiconn() is not supported");
    return null;
#if 0
    acc_error_flag = FALSE;
    if (TREE_CODE((tree) terminal) != TREE_LIST) {
	acc_error_flag = TRUE;
	return NULL;
    }
    return ((handle) PORT_TERMINAL_HICONN((tree) terminal));
#endif
}



/************************************************************************
 * acc_handle_defn
 * - does nothing for now
 *********************************************************************** *
 */
void *acc_handle_defn()
{
    return 0;
}
