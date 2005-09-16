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
 *		sdf.h
 *			- definitions for sdf.c module 
 *
 **********************************************************************
 */

#ifndef SDF_H
#define SDF_H

// public section

#define SDF_DEBUG
#define MAX_FILENAME_LENGTH 1024

typedef enum {
    eMINIMUM = 0,
    eTYPICAL,
    eMAXIMUM,
    eTOOL_CONTROL,
    eAVERAGE,
    eFROM_FILE
} eDelayType_t;

typedef enum {
    eFROM_MINIMUM = 0,
    eFROM_TYPICAL,
    eFROM_MAXIMUM,
    eFROM_MTM
} eScaleType_t;

typedef enum {
    eIOPATH = 0,
    ePORT,
    eINTERCONNECT,
    eNETDELAY,
    eDEVICE,
    eSETUP,
    eHOLD,
    eSETUPHOLD,
    eRECOVERY,
    eSKEW,
    eWIDTH,
    ePERIOD,
    eNOCHANGE,
    ePATHPULSE
} eSDFConstruct_t;

typedef enum {
    eVIGNORE = 0,
    eVINTERMODPATH,
    eVMIPD,
    eVCELL,
    eVSETUP,
    eVHOLD,
    eVSETUPHOLD,
    eVRECOVERY,
    eVSKEW,
    eVWIDTH,
    eVPERIOD,
    eVNOCHANGE,
    eVPATHPULSE,
    eVUSE
} eVerilogConstruct_t;

typedef struct {
    double value;
    int valid;
} creal_t;

typedef struct {
    creal_t triple[3];
} triple_t;

typedef struct {
    triple_t values[6];
    int argCount;
} elist_t;

typedef struct {
    char *path;
    char *name;
    int scalar;
    int msb;
    int lsb;
    int edge;
} port_t;

int sdf_check(int data, int reason);
int sdf_call(int data, int reason);
int sdf_misc(int data, int reason);
void setScaleFactors(double s1, double s2, double s3);
void setScaleType(eScaleType_t aType);
void setDelayType(eDelayType_t aDelay);
void setInterconnectMipd(eDelayType_t aDelay);
void setTurnOffDelay(eDelayType_t aDelay);
int setMap(eSDFConstruct_t from, eVerilogConstruct_t to);
void setConstraint(eSDFConstruct_t type, port_t * p1, port_t * p2,
		   elist_t value);
void setCellConstraint(handle object, port_t * p1, port_t * p2,
		       elist_t value);
void setInstanceConstraint(handle object, int matchAll,
			   eSDFConstruct_t type, port_t * p1, port_t * p2,
			   elist_t value);
void setCellType(char *type);
void setTimeScale(int scale);
void resetInstance(void);
int findInstance(char *name);
void setPathDelays(handle instance, handle object, elist_t value);
void setTimingConstraint(handle instance, eVerilogConstruct_t type,
			 port_t * p1, port_t * p2, elist_t value);
void initSDFCParse(void);
void initSDFParse(void);
int sdfconfig_parse(void);
int sdf_parse(void);
void setMTMSpec(eDelayType_t);
void setAbsoluteDelay(int);
void printPort(port_t port);
void printValue(elist_t value);
int scaleDelay(handle object, triple_t * value, double *d);
void setNewDelays(handle object, double *d, elist_t * value);
handle getPathHandle(handle instance, port_t * outPort, port_t * inPort);
void setPrimitiveDelays(handle instance, handle driver, elist_t value);

// private section

#ifdef SDF_C

char *sDelayType[] = {
    "MINIMUM",
    "TYPICAL",
    "MAXIMUM",
    "TOOL_CONTROL",
    "AVERAGE",
    "FROM_FILE"
};

char *sScaleType[] = {
    "FROM_MINIMUM",
    "FROM_TYPICAL",
    "FROM_MAXIMUM",
    "FROM_MTM"
};

char *sSDFConstruct[] = {
    "IOPATH",
    "PORT",
    "INTERCONNECT",
    "NETDELAY",
    "DEVICE",
    "SETUP",
    "HOLD",
    "SETUPHOLD",
    "RECOVERY",
    "SKEW",
    "WIDTH",
    "PERIOD",
    "NOCHANGE",
    "PATHPULSE"
};

char *sVerilogConstruct[] = {
    "IGNORE",
    "INTERMODPATH",
    "MIPD",
    "CELL",
    "SETUP",
    "HOLD",
    "SETUPHOLD",
    "RECOVERY",
    "SKEW",
    "WIDTH",
    "PERIOD",
    "NOCHANGE",
    "PATHPULSE",
    "USE"
};

void printDelayGroup(char *title, int delayCount, double d1, double d2,
		     double d3, double d4, double d5, double d6);

#endif				// SDF_C

#endif				// SDF_H
