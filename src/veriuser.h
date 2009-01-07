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

/* VERIUSER.H - PLI user routine header */

#ifndef VERIUSER_H
#define VERIUSER_H


#ifdef _LP64
#define LP64 long
#else
#define LP64 int
#endif


/* defines for error interception */

#define ERR_MESSAGE  1
#define ERR_WARNING  2
#define ERR_ERROR    3
#define ERR_INTERNAL 4
#define ERR_SYSTEM   5

#ifdef __cplusplus
extern "C" {
#endif


/* VERILOG user tasks and functions C header file */

struct t_call_stats {
    unsigned long long count;
    unsigned long long cycles;
};

typedef struct t_tfcell {
    short type;			/* either usertask or userfunction */
    short data;			/* parameter for the following routines */
    int (*checktf) (int, int);	/* routine for checking parameters */
    int (*sizetf) (int, int);	/* for providing size of function return value */
    int (*calltf) (int, int);	/* routine called during simulation */
    int (*misctf) (int, int);	/* miscellaneous routine (see below) */
    char *tfname;		/* the name of the system task or function */
    int forwref;		/* indicates special parameters allowed */
    char *tfveritool;		/* Which Veritool owns the task */
    char *tferrmessage;		/* An optional special case error message
				   which will be printed if the task is skipped */

    /* these components are for system usage only */
    int hash;
    struct t_tfcell *left_p;
    struct t_tfcell *right_p;
    char *namecell_p;
    int warning_printed;	/* Flag is set when skipping warning is printed */
    struct t_call_stats call_stats[18];
    int statsInit;
} s_tfcell, *p_tfcell;

struct t_tfexprinfo;
struct t_vecval;

/* values for component 'type' in tfcell structure */
#define usertask 1
#define USERTASK 1
#define userfunction 2
#define USERFUNCTION 2
#define userrealfunction 3
#define USERREALFUNCTION 3


/* The routines checktf, sizetf, calltf and misctf are always
passed two arguments: 'data' and 'reason'.
The value for 'data' is defined as the second component
supplied by the user in the above table structure.
The value for 'reason' is defined as follows:

   reason_checktf    for checktf
   reason_sizetf     for sizetf
   reason_calltf     for calltf
   reason_save       for misctf during $save
   reason_restart    for misctf during $restart
   reason_disable    for misctf during disable
   reason_paramvc    for misctf at a parameter value change (when enabled)
   reason_synch      for misctf at end of time slot for synchronisation
   reason_finish     for misctf at finish of simulation
   reason_reactivate for misctf at a delayed reactivation event
   reason_rosynch    for misctf at end of time slot read-only synchronisation
   reason_paramdrc   for misctf at a driver value change (when enabled)

(see below for the #defines on these)

Note that any of the routine pointers maybe 0, in which
case the calls from VERILOG are not made.

For the misctf at a parameter value change, the parameter
number is passed as the 3rd argument.
*/

/* values for 'reason' passed as 2nd argument */
#define reason_checktf 1
#define REASON_CHECKTF 1
#define reason_sizetf 2
#define REASON_SIZETF 2
#define reason_calltf 3
#define REASON_CALLTF 3
#define reason_save 4
#define REASON_SAVE 4
#define reason_restart 5
#define REASON_RESTART 5
#define reason_disable 6
#define REASON_DISABLE 6
#define reason_paramvc 7
#define REASON_PARAMVC 7
#define reason_synch 8
#define REASON_SYNCH 8
#define reason_finish 9
#define REASON_FINISH 9
#define reason_reactivate 10
#define REASON_REACTIVATE 10
#define reason_rosynch 11
#define REASON_ROSYNCH 11
#define reason_paramdrc 12
#define REASON_PARAMDRC 12

#define reason_interactive 14
#define REASON_INTERACTIVE 14
#define reason_endofcompile 15
#define REASON_ENDOFCOMPILE 15
#define reason_force 16
#define REASON_FORCE 16
#define reason_release 17
#define REASON_RELEASE 17
#define reason_scope 18
#define REASON_SCOPE 18


/* user error message string */
    extern char *tf_error_message;

/******** utility routines *********************/
    char *tf_getinstance();

    int tf_sizep(int);
    int tf_isizep(int, char *);

    int tf_nump();
    int tf_inump(char *);

    int tf_typep(int);
    int tf_itypep(int, char *);

    LP64 tf_getp(int);
    LP64 tf_igetp(int, char *);
    void tf_putp(int, int);
    void tf_iputp(int, int, char *);

    int tf_getlongp(int *, int);
    int tf_igetlongp(int *, int, char *);
    void tf_putlongp(int, int, int);
    void tf_iputlongp(int, int, int, char *);

    double tf_getrealp(int);
    double tf_igetrealp(int, char *);
    void tf_putrealp(int, double);
    void tf_iputrealp(int, double, char *);

    char *tf_strgetp(int, char);
    char *tf_istrgetp(int, char, char *);

    struct t_tfexprinfo *tf_exprinfo(int, struct t_tfexprinfo *);
    struct t_tfexprinfo *tf_iexprinfo(int, struct t_tfexprinfo *, char *);
    struct t_tfnodeinfo *tf_nodeinfo(int, struct t_tfnodeinfo *);
    struct t_tfnodeinfo *tf_inodeinfo(int, struct t_tfnodeinfo *, char *);

    void tf_evaluatep(int);
    void tf_ievaluatep(int, char *);

    void tf_propagatep(int);
    void tf_ipropagatep(int, char *);

    void tf_asynchon();
    void tf_iasynchon(char *);
    void tf_asynchoff();
    void tf_iasynchoff(char *);

    void tf_synchronize();
    void tf_isynchronize(char *);
    void tf_rosynchronize();
    void tf_irosynchronize(char *);

    int tf_gettime();
    double tf_getrealtime();
    void io_printf(char *, ...);
    void tf_error(char *, ...);
    void tf_warning(char *, ...);
    void tf_text(char *, ...);
    void tf_message(int, char *, char *, char *, ...);
    void tf_dostop();
    void tf_dofinish();
    char *tf_getcstringp(int);
    char *tf_igetcstringp(int, char *);
    int tf_gettimeprecision(void);
    int tf_igettimeprecision(char *);
    int tf_gettimeunit(void);
    int tf_igettimeunit(char *);
    void tf_finish(void);
    void tf_stop(void);

    extern char *tf_dumpfilename();
    extern int tf_setdelay(int delay);
    extern int tf_isetdelay(int delay, char *instance);
    extern int tf_setlongdelay(int low, int high);
    extern int tf_isetlongdelay(int low, int high, char *instance);
    extern int tf_setrealdelay(double delay);
    extern int tf_isetrealdelay(double delay, char *instance);

    extern void tf_clearalldelays();
    extern void tf_iclearalldelays(char *instance);

    extern int tf_strdelputp(int nparam, int bitlength, int format_char,
			     char *value_p, int delay, int delaytype);
    extern int tf_istrdelputp(int nparam, int bitlength, int format_char,
			      char *value_p, int delay, int delaytype,
			      char *instance);
    extern int tf_strlongdelputp(int nparam, int bitlength,
				 int format_char, char *value_p, int low,
				 int high, int delaytype);
    extern int tf_istrlongdelputp(int nparam, int bitlength,
				  int format_char, char *value_p, int low,
				  int high, int delaytype, char *instance);
    extern int tf_strrealdelputp(int nparam, int bitlength,
				 int format_char, char *value_p,
				 double delay, int delaytype);
    extern int tf_istrrealdelputp(int nparam, int bitlength,
				  int format_char, char *value_p,
				  double delay, int delaytype,
				  char *instance);

    extern int tf_copypvc_flag(int);
    extern int tf_icopypvc_flag(int, char *);
    extern int tf_movepvc_flag(int);
    extern int tf_imovepvc_flag(int, char *);
    extern int tf_testpvc_flag(int);
    extern int tf_itestpvc_flag(int, char *);
    extern int tf_getpchange(int);
    extern int tf_igetpchange(int, char *);

    extern void tf_setworkarea(char *);
    extern void tf_isetworkarea(char *, char *);
    extern char *tf_getworkarea();
    extern char *tf_igetworkarea(char *);
    extern char *(*tf_getroutine()) ();
    extern char *(*tf_igetroutine()) ();
    extern char *tf_gettflist();
    extern char *tf_igettflist();

    extern char *tf_mipname();
    extern char *tf_imipname(char *);
    extern char *tf_spname();
    extern char *tf_ispname(char *);

    extern void io_mcdprintf(int, char *, ...);

    extern char *mc_scan_plusargs(char *startarg);

    extern int tf_getlongtime(int *);
    extern int tf_igetlongtime(int *, char *);
    extern char *tf_strgettime();

    extern int tf_getnextlongtime(int *aof_lowtime, int *aof_hightime);
    extern int tf_getnextrealtime();
    extern char *tf_longtime_tostr(int lowtime, int hightime);

    extern int tf_compare_long(int low1, int high1, int low2, int high2);
    extern void tf_add_long(int *low1p, int *high1p, int low2, int high2);
    extern void tf_subtract_long(int *low1p, int *high1p, int low2,
				 int high2);

    extern void tf_multiply_long(int *low1p, int *high1p, int low2,
				 int high2);
    extern void tf_divide_long(int *low1p, int *high1p, int low2,
			       int high2);

    extern void tf_scale_realdelay(char *instance_p, double realdelay,
				   double *aof_realdelay);
    extern void tf_scale_longdelay(char *, int, int, int *, int *);
    extern void tf_unscale_realdelay(char *instance_p, double realdelay,
				     double *aof_realdelay);
    extern void tf_unscale_longdelay(char *, int, int, int *, int *);

    extern void tf_long_to_real(int low, int high, double *aof_real);
    extern void tf_real_to_long(double realvalue, int *aof_int_lo,
				int *aof_int_hi);
    extern int tf_write_save(char *blockptr, int blocklen);
    extern int tf_read_restart(char *blockptr, int blocklen);


#ifdef __cplusplus
}
#endif
/***********************************************//* data structure for use with tf_exprinfo routine to get
   expression information
*/ typedef struct t_tfexprinfo {
    short expr_type;		/* expression type information, one of:
				   tf_nullparam
				   tf_string
				   tf_readonly
				   tf_readwrite
				   tf_rwbitselect
				   tf_rwpartselect
				   tf_rwmemselect
				   tf_readonlyreal
				   tf_readwritereal
				 */
    struct t_vecval *expr_value_p;	/* pointer to expression value */
    double real_value;		/* expression value for 'real' expressions */
    char *expr_string;		/* pointer to string when parameter is a string */
    int expr_ngroups;		/* no of groups in expression value */
    int expr_vec_size;		/* no of bits in expression value */
    int expr_sign;		/* sign of the expression value:
				   0 for unsigned , non-zero for signed */
    int expr_lhs_select;	/* not used at present
				   result of either:
				   bit-select
				   msb of part-select
				   element selection
				 */
    int expr_rhs_select;	/* not used at present
				   result of either:
				   bit-select (same value as previous component)
				   lsb of part-select
				   element selection (same value as previous component)
				 */
} s_tfexprinfo, *p_tfexprinfo;



/* data structure for use with tf_nodeinfo routine to get
   node information
*/
typedef struct t_tfnodeinfo {
    short node_type;		/* expression type information, one of:
				   tf_null_node
				   tf_reg_node
				   tf_integer_node
				   tf_time_node
				   tf_netvector_node
				   tf_netscalar_node
				   tf_memory_node
				   tf_real_node
				 */
    union {
	struct t_vecval *vecval_p;	/* reg, integer, time, net vector value */
	struct t_strengthval *strengthval_p;	/* net scalar value */
	char *memoryval_p;	/* memory value */
	double *real_val_p;	/* type 'real' value */
    } node_value;
    char *node_symbol;		/* identifying string of node */
    int node_ngroups;		/* no of groups in node value */
    int node_vec_size;		/* no of bits in node value */
    int node_sign;		/* sign of the node value
				   0 for unsigned , non-zero for signed */
    int node_ms_index;		/* not used at present
				   most significant index of node value */
    int node_ls_index;		/* not used at present
				   least significant index of node value */
    int node_mem_size;		/* no of elements in memory structure */
    int node_lhs_element;	/* not used at present
				   lhs memory element address */
    int node_rhs_element;	/* not used at present
				   rhs memory element address */
    int *node_handle;		/*jjp052389 add handle field for access integration */
} s_tfnodeinfo, *p_tfnodeinfo;



/* data structure of vector values */
typedef struct t_vecval {
    int avalbits;		/* bit-coding for each bit of vector: */
    int bvalbits;		/* ab: 00=0, 10=1, 11=X, 01=Z */
} s_vecval, *p_vecval;



/* data structure of scalar net strength values */
typedef struct t_strengthval {
    int strength0;		/* 0 strength component */
    int strength1;		/* 1 strength component */
} s_strengthval, *p_strengthval;



/*
   data structure of memory values
   byte array organized in format:

        struct
        {
            char avalbits[ngroups];    bit-coding for each element:
            char bvalbits[ngroups];    ab: 00=0, 10=1, 11=X, 01=Z
        } memval[mem_size];

   with element addressed by left-hand-side index in
   declaration as the first word.
   N.B. this data structure cannot be represented in C, thus it
   it is declared as an array of char's.
*/




/* types used by tf_typep() and expr_type in tf_exprinfo structure */
#define tf_nullparam 0
#define TF_NULLPARAM 0
#define tf_string 1
#define TF_STRING 1
#define tf_specialparam 2
#define TF_SPECIALPARAM 2
#define tf_readonly 10
#define TF_READONLY 10
#define tf_readwrite 11
#define TF_READWRITE 11
#define tf_rwbitselect 12
#define TF_RWBITSELECT 12
#define tf_rwpartselect 13
#define TF_RWPARTSELECT 13
#define tf_rwmemselect 14
#define TF_RWMEMSELECT 14
#define tf_readonlyreal 15
#define TF_READONLYREAL 15
#define tf_readwritereal 16
#define TF_READWRITEREAL 16

/* types used by node_type in tf_nodeinfo structure */
#define tf_null_node 100
#define TF_NULL_NODE 100
#define tf_reg_node 101
#define TF_REG_NODE 101
#define tf_integer_node 102
#define TF_INTEGER_NODE 102
#define tf_time_node 103
#define TF_TIME_NODE 103
#define tf_netvector_node 104
#define TF_NETVECTOR_NODE 104
#define tf_netscalar_node 105
#define TF_NETSCALAR_NODE 105
#define tf_memory_node 106
#define TF_MEMORY_NODE 106
#define tf_real_node 107
#define TF_REAL_NODE 107

#endif
