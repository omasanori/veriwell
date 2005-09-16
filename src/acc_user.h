/****************************************************************************
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

/* ACC_USER.C - PLI access routine header */

#ifndef ACC_USER_H
#define ACC_USER_H



/**********************************************************************/
/*** General constant definitions ***/


typedef int *handle;

#define bool int
#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

#define global extern
#define exfunc
#define local static
#define null 0L

extern bool acc_error_flag;


/**********************************************************************/
/*** Type and configuration constant definitions ***/

#define    accModule               20
#define    accScope                21
#define    accNet                  25
#define    accReg                  30
#define    accRegister             30
#define    accPort                 35
#define    accTerminal             45
#define    accInputTerminal        46
#define    accOutputTerminal       47
#define    accInoutTerminal        48
#define    accCombPrim            140
#define    accSeqPrim             142
#define    accAndGate             144
#define    accNandGate            146
#define    accNorGate             148
#define    accOrGate              150
#define    accXorGate             152
#define    accXnorGate            154
#define    accBufGate             156
#define    accNotGate             158
#define    accBufif0Gate          160
#define    accBufif1Gate          162
#define    accNotif0Gate          164
#define    accNotif1Gate          166
#define    accNmosGate            168
#define    accPmosGate            170
#define    accCmosGate            172
#define    accRnmosGate           174
#define    accRpmosGate           176
#define    accRcmosGate           178
#define    accRtranGate           180
#define    accRtranif0Gate        182
#define    accRtranif1Gate        184
#define    accTranGate            186
#define    accTranif0Gate         188
#define    accTranif1Gate         190
#define    accPullupGate          192
#define    accPulldownGate        194
#define    accIntegerParam        200
#define    accIntParam            200
#define    accRealParam           202
#define    accStringParam         204
#define    accPath                206
#define    accTchk                208
#define    accPrimitive           210
#define    accBit                 212
#define    accPortBit             214
#define    accNetBit              216
#define    accRegBit              218
#define    accParameter           220
#define    accSpecparam           222
#define    accTopModule           224
#define    accModuleInstance      226
#define    accCellInstance        228
#define    accModPath             230
#define    accPrimPath            232
#define    accWirePath            234
#define    accModNetPath          236	/*alias for accInterModPath */
#define    accInterModPath        236
#define    accTermPath            238
#define    accModTermPath         240
#define    accTermModPath         242
#define    accScalarPort          250
#define    accBitSelectPort       252
#define    accPartSelectPort      254
#define    accVectorPort          256
#define    accConcatPort          258
#define    accWire                260
#define    accWand                261
#define    accWor                 262
#define    accTri                 263
#define    accTriand              264
#define    accTrior               265
#define    accTri0                266
#define    accTri1                267
#define    accTrireg              268
#define    accSupply0             269
#define    accSupply1             270
#define    accNamedEvent          280
#define    accEventVar            280
#define    accIntegerVar          281
#define    accIntVar              281
#define    accRealVar             282
#define    accTimeVar             283
#define    accScalar              300
#define    accVector              302
#define    accCollapsedNet        304
#define    accExpandedVector      306
#define    accUnexpandedVector    307
#define    accProtected           308
#define    accVlogSimPath         310
#define    accExpandedPath        312
#define	   accModPathHasIfnone	  313
#define    accSwXlInvisibleNet    314
#define    accAcceleratedNet      316
#define    accSetup               366
#define    accHold                367
#define    accWidth               368
#define    accPeriod              369
#define    accRecovery            370
#define    accSkew                371
#define    accNochange            376
#define    accNoChange            376
#define    accSetuphold           377
#define    accInput               402
#define    accOutput              404
#define    accInout               406
#define	   accMixedIo		  407
#define    accPositive            408
#define    accNegative            410
#define    accUnknown             412
#define    accPathTerminal        420
#define    accPathInput           422
#define    accPathOutput          424
#define    accDataPath            426
#define    accTchkTerminal        428
#define    accBitSelect           500
#define    accPartSelect          502
#define    accTask                504
#define    accFunction            506
#define    accStatement           508
#define    accTaskCall            510
#define    accFunctionCall        512
#define    accSystemTask          514
#define    accSystemFunction      516
#define    accSystemRealFunction  518
#define    accUserTask            520
#define    accUserFunction        522
#define    accUserRealFunction    524
#define    accAssignmentStat      526
#define    accContAssignStat      527
#define    accNullStat            528
#define    accDelayStat           530
#define    accAssignDelayStat     532
#define    accRtlDelayStat        534
#define    accAssignEventStat     536
#define    accAssignMultiStat     537
#define    accRtlEventStat        538
#define    accRtlMultiStat        539
#define    accGenEventStat        540
#define    accDisableStat         542
#define    accAssignStat          544
#define    accDeassignStat        546
#define    accForceStat           548
#define    accReleaseStat         550
#define    accInitialStat         552
#define    accAlwaysStat          554
#define    accAtEventStat         556
#define    accUnnamedBeginStat    558
#define    accNamedBeginStat      560
#define    accUnnamedForkStat     562
#define    accNamedForkStat       564
#define    accIfStat              566
#define    accCaseStat            568
#define    accCaseZStat           570
#define    accCaseXStat           572
#define    accForeverStat         574
#define    accRepeatStat          576
#define    accWhileStat           578
#define    accForStat             580
#define    accWaitStat            582
#define    accConstant            600
#define    accConcat              610
#define    accOperator            620

/* acc_configure() parameters */
#define    accPathDelayCount        1
#define    accPathDelimStr          2
#define    accDisplayErrors         3
#define    accDefaultAttr0          4
#define    accToHiZDelay            5
#define    accEnableArgs            6
#define    accSpecitemScope         7
#define    accDisplayWarnings       8
#define    accWarnNestedLoconn      9
#define    accWarnNestedHiconn     10
#define    accDevelopmentVersion   11
#define    accMinMultiplier        12
#define    accTypMultiplier        13
#define    accMaxMultiplier        14
#define    accAttrDelimStr         15
#define    accDelayCount           16
#define    accMapToMipd            17
#define    accDelayArrays          18
#define    accMinTypMaxDelays      19
#define    accUserErrorString      20
#define    accTwoLimitChecks       21

/* Edge information used by acc_handle_tchk, etc. */
#define accNoedge         0
#define accNoEdge         0
#define accEdge01         1
#define accEdge10         2
#define accEdge0x         4
#define accEdgex1         8
#define accEdge1x        16
#define accEdgex0        32
#define accPosedge       13	/* accEdge01 & accEdge0x & accEdgex1 */
#define accPosEdge       13	/* accEdge01 & accEdge0x & accEdgex1 */
#define accNegedge       50	/* accEdge10 & accEdge1x & accEdgex0 */
#define accNegEdge       50	/* accEdge10 & accEdge1x & accEdgex0 */

/* Product types */
#define accVerilog        1
#define accVeritime       2
#define accVerifault      3

/* Version defines */
#define accVersion15Beta    1
#define accVersion15a       2
#define accVersion15b       3
#define accVersion15b1      4
#define accVersion15b2Beta  5
#define accVersion15b2      6
#define accVersion15b3      7
#define accVersion15b4      8
#define accVersion15b5      9
#define accVersion15cBeta  12
#define accVersion15c      16
#define accVersion15c03    20
#define accVersion15c04    21
#define accVersion15c10    24
#define accVersion15c30    28
#define accVersion15c40    32
#define accVersion15c41    33
#define accVersion16Beta   36
#define accVersion16Beta2  37
#define accVersion16Beta3  38
#define accVersion16Beta4  39
#define accVersion16       40
#define accVersion161      41
#define accVersion16aBeta  42
#define accVersion16aBeta3 43
#define accVersion16aBeta5 44
#define accVersion16a      45
#define accVersion16a1     46
#define accVersion16a3     47
#define accVersion16a4     48
#define accVersion16a5     49
#define accVersionLatest   accVersion16a5

/* Delay modes */
#define accDelayModeNone     0
#define accDelayModePath     1
#define accDelayModeDistrib  2
#define accDelayModeUnit     3
#define accDelayModeZero     4
#define accDelayModeVeritime 5
#define accDelayModeMTM		6


/*****************************
** typedefs for time structure
*/

typedef struct t_acc_time {
    int type;			/* one of accTime accSimTime accRealTime */
    int low, high;		/* for accTime and accSimTime */
    double real;		/* for accRealTime */
} s_acc_time, *p_acc_time;

/* t_acc_time types */
#define accTime            1	/* timescaled time */
#define accSimTime         2	/* internal simulation time */
#define accRealTime        3	/* timescaled real time */

/* ****************************************
** typedef for fetch_timescale_info
*/

typedef struct t_timescale_info {
    short unit;
    short precision;
} s_timescale_info, *p_timescale_info;


/******************************************
** typedefs and defines for acc_set_value()
*/

typedef struct t_setval_delay {
    s_acc_time time;
    int model;
    /* accNoDelay */
    /* accInertialDelay */
    /* accTransportDelay */
    /* accPureTransportDelay */
} s_setval_delay, *p_setval_delay;


/* t_setval_delay types */
#define accNoDelay               0
#define accInertialDelay         1
#define accTransportDelay        2
#define accPureTransportDelay    3
#define accAssignFlag		4
#define accDeassignFlag		5
#define	accForceFlag		6
#define	accReleaseFlag		7

typedef struct t_acc_vecval {
    int aval;			/* bit-coding for each bit of vector: */
    int bval;			/* ab: 00=0, 10=1, 11=X, 01=Z */
} s_acc_vecval, *p_acc_vecval;

typedef struct t_setval_value {
    int format;			/* acc[[Bin,Oct,Dec,Hex]Str,Scalar,Int,Real,Vector]Val */
    union {
	char *str;
	int scalar;		/* acc[0,1,X,Z] */
	int integer;
	double real;
	p_acc_vecval vector;
    } value;
} s_setval_value, *p_setval_value, s_acc_value, *p_acc_value;

/* t_setval_value fromats */
#define accBinStrVal             1
#define accOctStrVal             2
#define accDecStrVal             3
#define accHexStrVal             4
#define accScalarVal             5
#define accIntVal                6
#define accRealVal               7
#define accStringVal             8
#define accCompactVal            9
#define accVectorVal            10

/* scalar values */
#define acc0    0
#define acc1    1
#define accX    2
#define accZ    3


/**********************************************************************/
/*
 * includes for Value Change Link
 */

#define logic_value_change       1
#define strength_value_change    2
#define real_value_change        3
#define vector_value_change      4
#define event_value_change       5
#define integer_value_change     6
#define time_value_change        7
#define sregister_value_change   8
#define vregister_value_change   9
#define realtime_value_change   10
#define compact_value_change    11


typedef void (*consumer_function) ();

/* structure that stores strengths */
typedef struct t_strengths {
    unsigned char logic_value;
    unsigned char strength1;
    unsigned char strength2;
} s_strengths, *p_strengths;


typedef struct t_vc_record {
    int vc_reason;
    unsigned int vc_hightime;
    unsigned int vc_lowtime;
    char *user_data;
    union {
	unsigned char logic_value;
	double real_value;
	handle vector_handle;
	s_strengths strengths_s;
    } out_value;
} s_vc_record, *p_vc_record;

/* logic values */
#define vcl0    acc0
#define vcl1    acc1
#define vclX    accX
#define vclZ    accZ

/* VCL strength values */
#define vclSupply            7
#define vclStrong            6
#define vclPull              5
#define vclLarge             4
#define vclWeak              3
#define vclMedium            2
#define vclSmall             1
#define vclHighZ             0

/* vcl bit flag definitions */
#define vcl_strength_flag    1
#define vcl_verilog_flag     2
#define vcl_veritime_flag    4
#define vcl_compact_flag     8


/* flags used with acc_vcl_add */
#define vcl_verilog_logic           (vcl_verilog_flag)
#define VCL_VERILOG_LOGIC           (vcl_verilog_flag)

#define vcl_verilog_strength        (vcl_verilog_flag + vcl_strength_flag)
#define VCL_VERILOG_STRENGTH        (vcl_verilog_flag + vcl_strength_flag)

/* flags used with acc_vcl_delete */
#define vcl_verilog             (vcl_verilog_flag)
#define VCL_VERILOG             (vcl_verilog_flag)

/* test whether strength information is requested for vcl */
#define vcl_setstr_m(flags_) ( flags_ |= vcl_strength_flag )
#define vcl_clearstr_m(flags_) ( flags_ &= ~vcl_strength_flag )
#define vcl_isstr_m(flags_) ( flags_ & vcl_strength_flag )

/* test whether Verilog information is requested for vcl */
#define vcl_setvl_m(flags_) ( flags_ |= vcl_verilog_flag )
#define vcl_clearvl_m(flags_) ( flags_ &= ~vcl_verilog_flag )
#define vcl_isvl_m(flags_) ( flags_ & vcl_verilog_flag )

/* test whether Veritime information is requested for vcl */
#define vcl_setvt_m(flags_) ( flags_ |= vcl_veritime_flag )
#define vcl_clearvt_m(flags_) ( flags_ &= ~vcl_veritime_flag )
#define vcl_isvt_m(flags_) ( flags_ & vcl_veritime_flag )

/* test whether vcl trigger is compact or normal */
#define vcl_setcompact_m(flags_) ( flags_ |= vcl_compact_flag )
#define vcl_clearcompact_m(flags_) ( flags_ &= ~vcl_compact_flag )
#define vcl_iscompact_m(flags_) ( flags_ & vcl_compact_flag )

/* constants for acc_product_type */
#define accSimulator		1
#define accTimingAnalyser	2
#define accFaultSimulator	3
#define accOther			4


/**********************************************************************/
/*** includes for the location structure ***/
/* structure that stores location */
typedef struct t_location {
    int line_no;
    char *filename;
} s_location, *p_location;

/**********************************************************************/
/*** includes for the time callbacks ***/
#define reason_begin_of_simtime     1
#define reason_end_of_simtime       2

/**********************************************************************/
/*
 * include information for stability checks
 */
#define accTaskFuncStable       0x0001
#define accSystfStable          0x0002
#define accPrimStable           0x0004
#define accContAssignStable     0x0008
#define accBehavStable          0x0010
#define accNetRegVarStable      0x0020
#define acc_taskfunc_stable     accTaskFuncStable
#define acc_systf_stable        accSystfStable
#define acc_primitive_stable    accPrimStable
#define acc_contassign_stable   accContAssignStable
#define acc_behav_stable        accBehavStable
#define acc_netreg_stable       accNetRegVarStable
#define acc_setstabflags_m(_flags,_pos)   (_flags |= _pos)
#define acc_clearstabflags_m(_flags,_pos) (_flags &= ~_pos)
#define acc_isstabflags_m(_flags,_pos)    (_flags & _pos)


/**********************************************************************/
/*** Routine declarations ***/


#ifdef __cplusplus
extern "C" {
#endif

/* Handle routines */
    void acc_vcl_add(handle, int (*)(s_vc_record *), char *, int);
    void acc_vcl_delete(handle, int (*)(s_vc_record *), char *, int);
    handle acc_next_topmod(handle);
    handle acc_handle_tfarg(int);
    handle acc_next_scope(handle, handle);
    int acc_fetch_size(handle);
    char *acc_fetch_value(handle, char *, ...);
    char *acc_fetch_name(handle);
    int acc_set_value(handle, p_setval_value, p_setval_delay);
    int acc_fetch_type(handle);
    int acc_configure(int, char *);
    int acc_initialize(void);
    void acc_close(void);
    handle acc_handle_object(char *);
    handle acc_handle_tchkarg1(handle);
    handle acc_handle_tchkarg2(handle);
    handle acc_handle_parent(handle);
    char *acc_fetch_fullname(handle);
    int acc_fetch_fulltype(handle);
    handle acc_handle_by_name(char *, handle);
    handle acc_next_child(handle, handle);
    handle acc_next_port(handle, handle);
    char *acc_fetch_defname(handle);
    handle acc_next_modpath(handle, handle);
    handle acc_handle_pathin(handle);
    handle acc_handle_pathout(handle);
    handle acc_handle_modpath(handle, char *, char *, ...);
    handle acc_next_primitive(handle, handle);
    bool acc_fetch_delays(handle object, ...);
    bool acc_replace_delays(handle object, ...);
    bool acc_append_delays(handle object, ...);

    handle acc_handle_simulated_net(handle collapsed_net_handle);


    bool acc_append_pulsere(handle path, double f1, double e1, double f2,
			    double e2, double f3, double e3, double f4,
			    double e4, double f5, double e5, double f6,
			    double e6);
    bool acc_replace_pulsere(handle path, double f1, double e1, double f2,
			     double e2, double f3, double e3, double f4,
			     double e4, double f5, double e5, double f6,
			     double e6);
    bool acc_set_pulsere(handle path, double reject_percentage,
			 double e_percentage);
    bool acc_fetch_pulsere(handle path, double *f1, double *e1, double *f2,
			   double *e2, double *f3, double *e3, double *f4,
			   double *e4, double *f5, double *e5, double *f6,
			   double *e6);

    int acc_fetch_direction(handle);
    handle acc_handle_conn(handle);
    handle acc_next_terminal(handle, handle);
    handle acc_next_driver(handle, handle);
    handle acc_next_tchk(handle, handle);
    handle acc_handle_tchkarg1(handle);
    handle acc_handle_tchkarg2(handle);
    handle acc_handle_tchk(handle, int, char *, int, char *, int, ...);
    char *acc_fetch_type_str(int);
    handle *acc_collect(handle(*nextFunction) (handle, handle),
			handle reference, int *numberOfObjects);
    bool acc_object_in_typelist(handle object, int *typeList);
    int acc_product_type(void);
    char *acc_product_version(void);
    char *acc_version(void);
    handle acc_next_cell(handle reference, handle cell);
    handle acc_next_cell_load(handle reference, handle load);
    handle acc_next_load(handle reference, handle load);
    handle acc_next_bit(handle reference, handle bit);
    handle acc_handle_port(handle module, int argumentNumber);
    handle acc_handle_interactive_scope(void);
    handle acc_handle_tfinst(void);
    handle acc_handle_scope(handle object);
    char *acc_set_scope(handle module, char *name);
    void acc_reset_buffer(void);
    handle acc_set_interactive_scope(void);
    double acc_fetch_tfarg(int argumentNumber);
    int acc_fetch_tfarg_int(int argumentNumber);
    char *acc_fetch_tfarg_str(int argumentNumber);
    handle acc_next_parameter(handle reference, handle param);
    handle acc_next_specparam(handle reference, handle param);
    int acc_fetch_delay_mode(handle module);
    int acc_fetch_paramtype(handle param);
    double acc_fetch_paramval(handle param);
    handle acc_next_portout(handle reference, handle port);
    int acc_fetch_argc(void);
    char **acc_fetch_argv(void);

    void acc_free(handle * list);
    int acc_count(handle(*nextFunction) (handle, handle),
		  handle reference);

    void acc_fetch_timescale_info(handle, p_timescale_info);
    int acc_fetch_precision(void);
    int acc_release_object(handle);

    handle acc_next_net(handle, handle);
    handle acc_next(int *, handle, handle);
    int acc_fetch_range(handle, int *, int *);
    handle acc_handle_loconn(handle);
    bool acc_object_of_type(handle object, int type);
    handle acc_next_input(handle, handle);
    handle acc_next_output(handle, handle);
    void acc_mod_lcb_add(handle, void (*)(char *, unsigned int, char *),
			 char *);
    void acc_mod_lcb_delete(handle, void (*)(char *, unsigned int, char *),
			    char *);
    void acc_fetch_location(p_location loc_p, handle object);
    bool acc_compare_handles(handle object1, handle object2);
    int acc_fetch_index(handle object);
    handle acc_handle_path(handle port_output_handle,
			   handle port_input_handle);
    handle acc_handle_terminal(handle primitive_handle,
			       int terminal_index);
    handle acc_handle_datapath(handle modpath);
    handle acc_next_loconn(handle port, handle bit);
    int acc_fetch_polarity(handle object);
    handle acc_next_hiconn(handle port, handle bit);
    int acc_fetch_edge(handle object);
    handle acc_handle_index(handle object);
    handle acc_handle_notifier(handle object);
    double acc_fetch_attribute(handle object, char *string, double def);
    handle acc_handle_hiconn(handle terminal);
    handle acc_handle_calling_mod_m(void);
    handle acc_handle_condition(handle);
    int acc_fetch_attribute_int(handle object, char *string, int def);
    char *acc_fetch_attribute_str(handle object, char *string, char *def);

#ifdef __cplusplus
}
#endif
#endif
