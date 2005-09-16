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

/**********************************************************************
 *
 *		sdf.c
 *			- main pli entry point for sdf annotator
 *
 *			things not implemented:
 *				1)	mipds
 *				2) 	conditions
 *				3)  pulses
 *				4)  module mapping
 *				5)  Hierarchy divider
 **********************************************************************
 */

#define SDF_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "veriuser.h"
#include "acc_user.h"
#include "sdf.h"
#include "vtypes.h"
#include "tree.h"
#include "glue.h"
#include "sdfclex.h"
#include "sdflex.h"
#include "flags.h"
#include "veriwell.h"

static char inputFilename[MAX_FILENAME_LENGTH];
static char configFilename[MAX_FILENAME_LENGTH];
static char logFilename[MAX_FILENAME_LENGTH];
static eDelayType_t mtmSpecification;
static double scaleFactor[3];
static eScaleType_t scaleType;
static eDelayType_t interconnectMipd;
static eDelayType_t turnOffDelay;
static handle rootModule;	// current top of hierarchy
static eVerilogConstruct_t map[ePATHPULSE + 1];
static char cellType[256];
static int absoluteDelay;
static int timeScale;
static handle currentInstance;
static int allInstances;


FILE *sdfLogFile;



/**********************************************************************

	setTimeScale	
		set time scale 

***********************************************************************/

void setTimeScale(int scale)
{
    timeScale = scale;
}

/**********************************************************************

	setAbsoluteDelay	
		set delay mode absolute or incremental	

***********************************************************************/

void setAbsoluteDelay(int flag)
{
    absoluteDelay = flag;
}

/**********************************************************************

	resetInstance

		-	clear the current instance 

 **********************************************************************/

void resetInstance()
{
    currentInstance = null;
    allInstances = FALSE;
}


/**********************************************************************

	findInstance
		- find the next level of hierarchy	

 **********************************************************************/

int findInstance(char *name)
{

    if (!strcmp(name, "*")) {
	allInstances = TRUE;
	return (TRUE);
    }

    if (currentInstance == null) {
	currentInstance = acc_handle_object(name);
    } else {
	currentInstance = acc_handle_by_name(name, currentInstance);
    }

    return (currentInstance != null);

}


/**********************************************************************

	setCellType	
		- change the current cell type	

 **********************************************************************/

void setCellType(char *type)
{
    strcpy(cellType, type);
}

/**********************************************************************

	printPort	
		-	printout an port spec	

***********************************************************************/

void printPort(port_t port)
{
    if (port.scalar) {
	fprintf(sdfLogFile, port.name);
    } else if (port.msb == port.lsb) {
	fprintf(sdfLogFile, "%s[%d]", port.name, port.msb);
    } else {
	fprintf(sdfLogFile, "%s[%d:%d]", port.name, port.msb, port.lsb);
    }


}

/**********************************************************************

	printValue	
		-	printout an sdf value	

***********************************************************************/

void printValue(elist_t value)
{
    int i;
    int j;

    for (i = 0; i < value.argCount; i++) {
	fprintf(sdfLogFile, "(");
	for (j = 0; j < 3; j++) {
	    if (value.values[i].triple[j].valid) {
		if (j != 0) {
		    fprintf(sdfLogFile, ",");
		}
		fprintf(sdfLogFile, "%g", value.values[i].triple[j].value);
	    }
	}
	fprintf(sdfLogFile, ")");
    }
}

/**********************************************************************

	scaleDelay
		-	 converts a delay triple into a value
			 returns TRUE if delay is valid

***********************************************************************/

int scaleDelay(handle object, triple_t * value, double *d)
{
    triple_t newValue;
    int result;
    int scale;

    // pick basis for delays
    switch (scaleType) {
    case eFROM_MINIMUM:
	newValue.triple[0] = value->triple[0];
	newValue.triple[1] = value->triple[0];
	newValue.triple[2] = value->triple[0];
	break;
    case eFROM_MAXIMUM:
	newValue.triple[0] = value->triple[2];
	newValue.triple[1] = value->triple[2];
	newValue.triple[2] = value->triple[2];
	break;
    case eFROM_TYPICAL:
	newValue.triple[0] = value->triple[1];
	newValue.triple[1] = value->triple[1];
	newValue.triple[2] = value->triple[1];
	break;
    case eFROM_MTM:
	newValue = *value;
	break;
    default:
	ASSERT(FALSE);
    }

    // apply scaleFactors
    newValue.triple[0].value *= scaleFactor[0];
    newValue.triple[1].value *= scaleFactor[1];
    newValue.triple[2].value *= scaleFactor[2];

    // pick min:typ:max
    switch (mtmSpecification) {
    case eMINIMUM:
	*d = newValue.triple[0].value;
	result = newValue.triple[0].valid;
	break;
    case eTYPICAL:
	*d = newValue.triple[1].value;
	result = newValue.triple[1].valid;
	break;
    case eMAXIMUM:
	*d = newValue.triple[2].value;
	result = newValue.triple[2].valid;
	break;
    case eTOOL_CONTROL:
	// here is currently the only place I don't use pli to access info
	switch (delay_type) {
	case MIN_DELAY:
	    *d = newValue.triple[0].value;
	    result = newValue.triple[0].valid;
	    break;
	case TYP_DELAY:
	    *d = newValue.triple[1].value;
	    result = newValue.triple[1].valid;
	    break;
	case MAX_DELAY:
	    *d = newValue.triple[2].value;
	    result = newValue.triple[2].valid;
	    break;
	default:
	    ASSERT(FALSE);
	}
	break;
    default:
	ASSERT(FALSE);
    }

    scale =
	timeScale - tf_igettimeunit((char *) acc_handle_parent(object));
    *d *= pow(10.0, (double) scale);

    return (result);
}

/**********************************************************************

	setNewDelays
		-	set new delay values 

***********************************************************************/

void setNewDelays(handle object, double *d, elist_t * value)
{
    int i;
    double delay;

    for (i = 0; i < value->argCount; i++) {

	// scale delay
	if (scaleDelay(object, &value->values[i], &delay)) {
	    // if absolute replace delay
	    // otherwise increment delay
	    if (absoluteDelay) {
		d[i] = delay;
	    } else {
		d[i] += delay;
	    }
	}
    }

}

/**********************************************************************

	getPathHandle
		- returns handle for path matching the given ports	

***********************************************************************/

handle getPathHandle(handle instance, port_t * outPort, port_t * inPort)
{
    handle object;
    char outPortName[256];
    char inPortName[256];
    char *fmt;

    object = NULL;


    if (!outPort->scalar) {
	if (outPort->msb == outPort->lsb) {
	    fmt = "%s[%d]";
	} else {
	    fmt = "%s[%d:%d]";
	}
    } else {
	fmt = "%s";
    }
    sprintf(outPortName, fmt, outPort->name, outPort->msb, outPort->lsb);

    if (!inPort->scalar) {
	if (inPort->msb == inPort->lsb) {
	    fmt = "%s[%d]";
	} else {
	    fmt = "%s[%d:%d]";
	}
    } else {
	fmt = "%s";
    }
    sprintf(inPortName, fmt, inPort->name, inPort->msb, inPort->lsb);


    object = acc_handle_modpath(instance, inPortName, outPortName);

    return (object);
}



/**********************************************************************

	setConstraint
		-	set new sdf constraint

***********************************************************************/

void
setConstraint(eSDFConstruct_t type, port_t * p1, port_t * p2,
	      elist_t value)
{
    handle top;

    if (currentInstance != null) {
	setInstanceConstraint(currentInstance, allInstances, type,
			      p1, p2, value);
    } else {
	top = null;
	while (TRUE) {
	    top = acc_next_topmod(top);
	    if (top == null) {
		break;
	    }
	    setInstanceConstraint(top, allInstances, type, p1, p2, value);
	}
    }
}

/**********************************************************************

	setInstanceConstraint
		-	set new sdf constraint and walk instances

***********************************************************************/

void
setInstanceConstraint(handle instance, int matchAll,
		      eSDFConstruct_t type, port_t * p1, port_t * p2,
		      elist_t value)
{
    int match;
    handle child;


    match = !strcmp(cellType, acc_fetch_defname(instance));
    if (matchAll && !match) {
	child = null;
	while (TRUE) {
	    child = acc_next_child(instance, child);
	    if (child == null) {
		break;
	    }
	    setInstanceConstraint(child, matchAll, type, p1, p2, value);
	}
    } else if (match) {

	switch (map[type]) {
	case eVCELL:
	    setCellConstraint(instance, p1, p2, value);
	    break;
	case eVSETUP:
	case eVHOLD:
	case eVSKEW:
	case eVRECOVERY:
	case eVPERIOD:
	case eVWIDTH:
	case eVSETUPHOLD:
	    setTimingConstraint(instance, map[type], p1, p2, value);
	    break;
	case eVIGNORE:
	    break;
	default:

	    {
		char buffer[255];

		sprintf(buffer, "%s contraints are not supported",
			sVerilogConstruct[map[type]]);
		sdf_error(buffer);
	    }
	    break;

	}

    }
}


/**********************************************************************

	printDelayGroup
		- print delay group values to logfile	

***********************************************************************/
void
printDelayGroup(char *title, int delayCount, double d1,
		double d2, double d3, double d4, double d5, double d6)
{
    int count = delayCount;

    fprintf(sdfLogFile, "\t%s\n", title);
    fprintf(sdfLogFile, "\t\t01\t%g\n", d1);
    count--;
    if (count > 0) {
	fprintf(sdfLogFile, "\t\t10\t%g\n", d2);
	count--;
    }
    if (count > 0) {
	fprintf(sdfLogFile, "\t\t0z\t%g\n", d3);
	count--;
    }
    if (count > 0) {
	fprintf(sdfLogFile, "\t\tz1\t%g\n", d4);
	count--;
    }
    if (count > 0) {
	fprintf(sdfLogFile, "\t\t1z\t%g\n", d5);
	count--;
    }
    if (count > 0) {
	fprintf(sdfLogFile, "\t\tz0\t%g\n", d6);
	count--;
    }
}

/**********************************************************************

	setPathDelays
		- set delays for path instance	

***********************************************************************/

void setPathDelays(handle instance, handle object, elist_t value)
{
    double d[6];

    fprintf(sdfLogFile, "modifying path %s => ",
	    acc_fetch_fullname(object));
    printValue(value);
    fprintf(sdfLogFile, "\n");

    acc_fetch_delays(object, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
    printDelayGroup("Current delays", 6, d[0], d[1], d[2], d[3], d[4],
		    d[5]);
    setNewDelays(instance, d, &value);
    acc_replace_delays(object, d[0], d[1], d[2], d[3], d[4], d[5]);

    acc_fetch_delays(object, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
    printDelayGroup("New delays", 6, d[0], d[1], d[2], d[3], d[4], d[5]);
}

/**********************************************************************

	setPrimitiveDelays
		- set delays for primitive instance	

***********************************************************************/

void setPrimitiveDelays(handle instance, handle driver, elist_t value)
{
    double d[6];

    fprintf(sdfLogFile, "modifying primitive %s => ",
	    acc_fetch_fullname(driver));
    printValue(value);
    fprintf(sdfLogFile, "\n");

    acc_fetch_delays(driver, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
    printDelayGroup("Current delays", 3, d[0], d[1], d[2], d[3], d[4],
		    d[5]);
    setNewDelays(instance, d, &value);
    acc_replace_delays(driver, d[0], d[1], d[2], d[3], d[4], d[5]);

    acc_fetch_delays(driver, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
    printDelayGroup("New delays", 3, d[0], d[1], d[2], d[3], d[4], d[5]);
}

/**********************************************************************

	setTimingConstraint	
		-	set timing constraint on given instance 

 **********************************************************************/
void
setTimingConstraint(handle instance, eVerilogConstruct_t type,
		    port_t * p1, port_t * p2, elist_t value)
{
    int timingCheckType;
    int edge2;
    handle check;
    double d[6];
    char name1[256];
    char name2[256];

    // map to pli timing check type
    switch (type) {
    case eVSETUP:
	timingCheckType = accSetup;
	break;
    case eVHOLD:
	timingCheckType = accHold;
	break;
    case eVSKEW:
	timingCheckType = accSkew;
	break;
    case eVRECOVERY:
	timingCheckType = accRecovery;
	break;
    case eVPERIOD:
	timingCheckType = accPeriod;
	break;
    case eVWIDTH:
	timingCheckType = accWidth;
	break;
    case eVSETUPHOLD:
	timingCheckType = accSetuphold;
	break;
    default:
	ASSERT(FALSE);
    }

    // find timing check

    if (p1->scalar) {
	sprintf(name1, "%s", p1->name);
    } else if (p1->lsb == p1->msb) {
	sprintf(name1, "%s[%d]", p1->name, p1->msb);
    } else {
	sprintf(name1, "%s[%d:%d]", p1->name, p1->msb, p1->lsb);
    }

    if (p2 == NULL) {
	name2[0] = 0;
    } else if (p2->scalar) {
	sprintf(name2, "%s", p2->name);
    } else if (p2->lsb == p2->msb) {
	sprintf(name2, "%s[%d]", p2->name, p2->msb);
    } else {
	sprintf(name2, "%s[%d:%d]", p2->name, p2->msb, p2->lsb);
    }

    if (p2 == NULL) {
	edge2 = 0;
    } else {
	edge2 = p2->edge;
    }

    check = acc_handle_tchk(instance, timingCheckType, name1, p1->edge,
			    name2, edge2);

    if (check == null) {
	return;
    }
    // update delays

    fprintf(sdfLogFile, "modifying %s check %s => ",
	    acc_fetch_type_str(acc_fetch_fulltype(check)),
	    acc_fetch_fullname(check));
    printValue(value);
    fprintf(sdfLogFile, "\n");

    acc_fetch_delays(check, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
    printDelayGroup("Current delays", 1, d[0], d[1], d[2], d[3], d[4],
		    d[5]);
    setNewDelays(instance, d, &value);
    acc_replace_delays(check, d[0], d[1], d[2], d[3], d[4], d[5]);

    acc_fetch_delays(check, &d[0], &d[1], &d[2], &d[3], &d[4], &d[5]);
    printDelayGroup("New delays", 1, d[0], d[1], d[2], d[3], d[4], d[5]);
}

/**********************************************************************

	setCellConstraint	
		-	set cell constraint on given instance 

 **********************************************************************/
void
setCellConstraint(handle instance, port_t * p1, port_t * p2, elist_t value)
{
    handle object;
    handle port;
    handle net;
    handle driver;
    handle primitive;
    char *ptr;
    int position;


    // if p1 and p2 is specified locate path

    ASSERT(p1 != NULL || p2 == NULL);
    object = null;
    if (p2 != NULL) {		// this is a path specification
	object = getPathHandle(instance, p1, p2);

	// if path is found set delays and return
	if (object != null) {
	    setPathDelays(instance, object, value);
	    return;
	}
    }

    // if p1 is specified locate port
    // apply delay to each driving primitive with same parent

    // if p1 is unspecified iterate through each output port
    // apply delay to each driving primitive with same parent

    port = null;
    while (TRUE) {
	port = acc_next_port(instance, port);
	if (port == null) {
	    break;
	}
	if (acc_fetch_direction(port) == accInput) {
	    continue;
	}
	if (p1 != NULL && strcmp(acc_fetch_name(port), p1->name) != 0) {
	    break;
	}
	net = acc_handle_loconn(port);
	if (net == NULL) {
	    continue;
	}
	driver = null;
	while (TRUE) {
	    driver = acc_next_driver(net, driver);
//          if( driver == null || 
//              acc_fetch_type( driver ) != accPrimitive ||
//              acc_handle_parent( driver ) != instance ) {
//              break;
//          }
//          terminal = acc_next_terminal( driver, null );
	    if (driver == null || acc_fetch_type(driver) != accTerminal) {
		break;
	    }
	    primitive = acc_handle_parent(driver);
	    if (acc_handle_parent(primitive) != instance) {
		break;
	    }
	    ptr = strchr(acc_fetch_name(acc_handle_conn(driver)), '[');
	    if (ptr != null) {
		position = atoi(ptr + 1);
	    }

	    if (p1 != NULL &&
		!p1->scalar &&
		ptr != NULL &&
		!(p1->lsb <= position && position <= p1->msb ||
		  p1->msb <= position && position <= p1->lsb)) {
		continue;
	    }

	    setPrimitiveDelays(instance, primitive, value);

	}
    }

}

/**********************************************************************

	setMap
		-	set construct mapping, return TRUE if mapping is valid

 **********************************************************************/

int setMap(eSDFConstruct_t sdfConstruct, eVerilogConstruct_t vConstruct)
{
    switch (vConstruct) {
    case eVINTERMODPATH:
	if (sdfConstruct != eINTERCONNECT && sdfConstruct != eNETDELAY) {
	    return (FALSE);
	}
	break;
    case eVMIPD:
	if (sdfConstruct != eINTERCONNECT &&
	    sdfConstruct != ePORT && sdfConstruct != eNETDELAY) {
	    return (FALSE);
	}
	break;
    case eVCELL:
	if (sdfConstruct != eDEVICE && sdfConstruct != eIOPATH) {
	    return (FALSE);
	}
	break;
    case eVUSE:
	return (TRUE);		// use defaults
    case eVIGNORE:
	break;
    default:
	ASSERT(FALSE);
    }
    map[sdfConstruct] = vConstruct;
    return (TRUE);
}

/**********************************************************************
	
	setInterconnectMipd
		- set global interconnectMipd spec 

***********************************************************************/
void setInterconnectMipd(eDelayType_t aSpec)
{
    interconnectMipd = aSpec;
}

/**********************************************************************
	
	setTurnOffDelay
		- set global turn off delay specification

***********************************************************************/
void setTurnOffDelay(eDelayType_t aSpec)
{
    char *ptr;

    turnOffDelay = aSpec;
    switch (aSpec) {
    case eMINIMUM:
	ptr = "min";
	break;
    case eMAXIMUM:
	ptr = "max";
	break;
    case eAVERAGE:
	ptr = "average";
	break;
    case eFROM_FILE:
	ptr = "from_user";
	break;
    default:
	ASSERT(FALSE);
	return;
    }
    acc_configure(accToHiZDelay, ptr);
}

/**********************************************************************
	
	setMTMSpec
		- set global mtm specification

***********************************************************************/
void setMTMSpec(eDelayType_t aSpec)
{
    mtmSpecification = aSpec;
}

/**********************************************************************
	
	setScaleFactors
		- set global scale factors 

***********************************************************************/
void setScaleFactors(double sf1, double sf2, double sf3)
{
    scaleFactor[0] = sf1;
    scaleFactor[1] = sf2;
    scaleFactor[2] = sf3;
}

/**********************************************************************
	
	setScaleType
		- set global mtm specification

***********************************************************************/
void setScaleType(eScaleType_t aSpec)
{
    scaleType = aSpec;
}

/**********************************************************************

		sdf_check
			-	validate arguments for $sdf_annotate call

***********************************************************************/

int sdf_check(int data, int reason)
{
    int numberOfArgs;
    int argNumber;
    char *stringPtr;


    acc_initialize();

    // validate arguments

    numberOfArgs = tf_nump();
    if (numberOfArgs < 1) {
	tf_error("Not enough arguments");
	goto fail;
    }
    if (numberOfArgs > 7) {
	tf_error("Too many arguments");
	goto fail;
    }
    // argument 1 - sdf filename

    stringPtr = tf_getcstringp(1);
    if (stringPtr == NULL) {
	tf_error("argument 1 must be a string");
	goto fail;
    }
    strcpy(inputFilename, stringPtr);

    // argument 2 - module instance
    // validate later

    // arguments 3-7 

    for (argNumber = 3; argNumber <= numberOfArgs; argNumber++) {
	if (tf_typep(argNumber) != tf_string) {
	    tf_error("argument %d must be a string", argNumber);
	    goto fail;
	}
    }



  fail:
    acc_close();
    return (0);
}

/**********************************************************************

		sdf_call
			-	execute $sdf_annotate call

***********************************************************************/

int sdf_call(int data, int reason)
{
    int numberOfArgs;
    int argNumber;
    char *stringPtr;
    char *ptr;
    int i;

    acc_initialize();



    // parse arguments

    numberOfArgs = tf_nump();

    // argument 1 - sdf filename

    strcpy(inputFilename, tf_getcstringp(1));

    // argument 2 - module instance

    if (numberOfArgs > 1) {
	rootModule = acc_handle_tfarg(2);
    } else {
	rootModule = NULL;
    }

    if (rootModule == NULL) {
	rootModule = acc_handle_object(tf_mipname());
    }
//  setup defaults

    strcpy(logFilename, "sdf.log");
    scaleFactor[0] = 1;
    scaleFactor[1] = 1;
    scaleFactor[2] = 1;
    scaleType = eFROM_MTM;
    timeScale = -9;		// default is one ns
    mtmSpecification = eTOOL_CONTROL;
    configFilename[0] = 0;
    turnOffDelay = eFROM_FILE;
    interconnectMipd = eMAXIMUM;

    // setup default mappings

    map[eIOPATH] = eVCELL;
    map[ePORT] = eVMIPD;
    map[eINTERCONNECT] = eVMIPD;
    map[eNETDELAY] = eVMIPD;
    map[eDEVICE] = eVCELL;
    map[eSETUP] = eVSETUP;
    map[eHOLD] = eVHOLD;
    map[eSETUPHOLD] = eVSETUPHOLD;
    map[eRECOVERY] = eVRECOVERY;
    map[eSKEW] = eVSKEW;
    map[eWIDTH] = eVWIDTH;
    map[ePERIOD] = eVPERIOD;
    map[eNOCHANGE] = eVNOCHANGE;
    map[ePATHPULSE] = eVPATHPULSE;


    // argument 3

    if (numberOfArgs > 2 && tf_typep(3) == tf_string) {
	strcpy(configFilename, tf_getcstringp(3));
	initSDFCParse();
	if (sdfclexOpenFile(configFilename) != NULL) {
	    sdfconfig_parse();
	}
    }

    // arguments 4-7 

    for (argNumber = 4; argNumber <= numberOfArgs; argNumber++) {
	if (tf_typep(argNumber) == tf_nullparam) {
	    continue;
	}

	stringPtr = tf_getcstringp(argNumber);

	switch (argNumber) {
	case 4:		// log file
	    strcpy(logFilename, stringPtr);
	    break;
	case 5:		// mtm specification
	    if (!strcmp(stringPtr, "MINIMUM")) {
		mtmSpecification = eMINIMUM;
	    } else if (!strcmp(stringPtr, "TYPICAL")) {
		mtmSpecification = eTYPICAL;
	    } else if (!strcmp(stringPtr, "MAXIMUM")) {
		mtmSpecification = eMAXIMUM;
	    } else if (!strcmp(stringPtr, "TOOL_CONTROL")) {
		mtmSpecification = eTOOL_CONTROL;
	    } else {
		tf_error("argument %d is not recognized", argNumber);
		goto fail;
	    }
	    break;
	case 6:		// scale factor

	    // parse first value

	    ptr = strchr(stringPtr, ':');
	    if (ptr == NULL) {
		tf_error("argument %d is an illegal scale factor",
			 argNumber);
		goto fail;
	    }

	    if (ptr == stringPtr) {
		scaleFactor[0] = 1;
	    } else {
		scaleFactor[0] = atof(stringPtr);
	    }
	    stringPtr = ptr + 1;

	    // parse second value

	    ptr = strchr(stringPtr, ':');
	    if (ptr == NULL) {
		tf_error("argument %d is an illegal scale factor",
			 argNumber);
		goto fail;
	    }

	    if (ptr == stringPtr) {
		scaleFactor[1] = 1;
	    } else {
		scaleFactor[1] = atof(stringPtr);
	    }
	    stringPtr = ptr + 1;

	    // parse third value


	    if (*stringPtr == 0) {
		scaleFactor[2] = 1;
	    } else {
		scaleFactor[2] = atof(stringPtr);
	    }

	    break;
	case 7:		// scale type
	    if (!strcmp(stringPtr, "FROM_MINIMUM")) {
		scaleType = eFROM_MINIMUM;
	    } else if (!strcmp(stringPtr, "FROM_TYPICAL")) {
		scaleType = eFROM_TYPICAL;
	    } else if (!strcmp(stringPtr, "FROM_MAXIMUM")) {
		scaleType = eFROM_MAXIMUM;
	    } else if (!strcmp(stringPtr, "FROM_MTM")) {
		scaleType = eFROM_MTM;
	    } else {
		tf_error("argument %d is not recognized", argNumber);
		goto fail;
	    }
	    break;
	default:
	    ASSERT(FALSE);
	}
    }


// open logfile

    sdfLogFile = shell_fopen(logFilename, "w");
    if (sdfLogFile == NULL) {
	tf_error("Could not open logfile '%s'\n", logFilename);
	goto fail;
    }


#ifdef SDF_DEBUG
    fprintf(sdfLogFile, "input file = %s\n", inputFilename);
    fprintf(sdfLogFile, "root module<fullname> = %s\n",
	    acc_fetch_fullname(rootModule));
    fprintf(sdfLogFile, "config file = %s\n", configFilename);
    fprintf(sdfLogFile, "log file = %s\n", logFilename);
    fprintf(sdfLogFile, "mtm spec = %s\n", sDelayType[mtmSpecification]);
    fprintf(sdfLogFile, "scale factor = %g:%g:%g\n", scaleFactor[0],
	    scaleFactor[1], scaleFactor[2]);
    fprintf(sdfLogFile, "scale type = %s\n", sScaleType[scaleType]);
    fprintf(sdfLogFile, "interconnect midp = %s\n",
	    sScaleType[interconnectMipd]);
    fprintf(sdfLogFile, "turn off delay = %s\n", sDelayType[turnOffDelay]);
    for (i = 0; i < sizeof(map) / sizeof(eVerilogConstruct_t); i++) {
	fprintf(sdfLogFile, "mapping %s => %s\n", sSDFConstruct[i],
		sVerilogConstruct[map[i]]);
    }
#endif				// SDF_DEBUG



    // parse input file

    initSDFParse();
    if (sdflexOpenFile(inputFilename) != NULL) {
	sdf_parse();
    }


  fail:
    shell_fclose(sdfLogFile);
    acc_close();
    return (0);
}

/**********************************************************************

		sdf_misc
			-	misc pli maintanance calls 

***********************************************************************/

int sdf_misc(int data, int reason)
{
    acc_initialize();

    acc_close();
    return (0);
}
