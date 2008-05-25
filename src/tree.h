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

/* TREE.H - Tree structures and routine prototypes 

/* This is here as a placeholder since not all modules that include
   tree.h will also include decl.h
*/

#ifndef TREE_H
#define TREE_H

#include "veriuser.h"

/* codes of tree nodes */

#define DEFTREECODE(SYM, STRING, TYPE, NARGS)   SYM,

enum tree_code {
#include "tree.def"

    LAST_AND_UNUSED_TREE_CODE	/* A convienent way to get a value for
				   NUM_TREE_CODE.  */
};

#undef DEFTREECODE

/* Number of tree codes.  */
#define NUM_TREE_CODES ((int)LAST_AND_UNUSED_TREE_CODE)

/* Indexed by enum tree_code, contains a character which is
   `e' for an expression, `r' for a reference, `c' for a constant,
   `d' for a decl, `s' for a statement, `b' for a block,
   and `x' for anything else (TREE_LIST, IDENTIFIER, etc).  */

extern char *tree_code_type[];

/* Number of argument-words in each kind of tree-node.  */

extern int tree_code_length[];

/* Name of tree node */

extern char *tree_code_name[];

enum tree_type {
    NO_TYPE,
    MODULE_TYPE,
    FUNCTION_TYPE,
    TASK_TYPE,
    REG_TYPE,
    INTEGER_TYPE,
    EVENT_TYPE,
    TIME_TYPE,
    REAL_TYPE,
    NET_TRI_TYPE,
    NET_WIRE_TYPE,
    NET_TRIOR_TYPE,
    NET_WOR_TYPE,
    NET_TRIAND_TYPE,
    NET_WAND_TYPE,
    NET_TRIREG_TYPE,
    NET_TRI0_TYPE,
    NET_TRI1_TYPE,
    NET_SUPPLY0_TYPE,
    NET_SUPPLY1_TYPE,
    NAMED_BLOCK_TYPE,
    GATE_AND_TYPE,
    GATE_NAND_TYPE,
    GATE_OR_TYPE,
    GATE_NOR_TYPE,
    GATE_XOR_TYPE,
    GATE_XNOR_TYPE,
    GATE_BUF_TYPE,
    GATE_NOT_TYPE,
    GATE_BUFIF0_TYPE,
    GATE_BUFIF1_TYPE,
    GATE_NOTIF0_TYPE,
    GATE_NOTIF1_TYPE,
    GATE_NMOS_TYPE,
    GATE_PMOS_TYPE,
    GATE_RNMOS_TYPE,
    GATE_RPMOS_TYPE,
    GATE_CMOS_TYPE,
    GATE_RCMOS_TYPE,
    GATE_PULLUP_TYPE,
    GATE_PULLDN_TYPE,
    GATE_TRAN_TYPE,
    GATE_RTRAN_TYPE,
    GATE_TRANIF0_TYPE,
    GATE_TRANIF1_TYPE,
    GATE_RTRANIF0_TYPE,
    GATE_RTRANIF1_TYPE,
    GATE_UDP_TYPE,
    STRENGTH_SUPPLY0_TYPE,
    STRENGTH_SUPPLY1_TYPE,
    STRENGTH_STRONG0_TYPE,
    STRENGTH_STRONG1_TYPE,
    STRENGTH_PULL0_TYPE,
    STRENGTH_PULL1_TYPE,
    STRENGTH_HIGHZ0_TYPE,
    STRENGTH_HIGHZ1_TYPE,
    STRENGTH_WEAK0_TYPE,
    STRENGTH_WEAK1_TYPE
};

enum delay_type {
    MIN_DELAY,
    TYP_DELAY,
    MAX_DELAY
};

enum lval_type {
    LVAL_REG,
    LVAL_NEW_NET,
    LVAL_NET,
    LVAL_REG_NET,
    LVAL_GATE
};

/* Codes that identify the various built in functions
   so that expand_call can identify them quickly.  */

/* May use this later for $display, etc. */
//enum built_in_function
//{
// DISPLAY,
//  DISPLAYH,
//  DISPLAYO,
//  DISPLAYB
//};

/* The definition of tree nodes fills the next several pages.  */

/* A tree node can represent a variable, an expression
   or a statement.  Each node has a TREE_CODE which says what kind of
   thing it represents.  Some common codes are:
   INTEGER_TYPE -- represents a type of integers.
   VAR_DECL -- represents a declared variable.
   INTEGER_CST -- represents a constant integer value.
   PLUS_EXPR -- represents a sum (an expression).

   As for the contents of a tree node: there are some fields
   that all nodes share.  Each TREE_CODE has various special-purpose
   fields as well.  The fields of a node are never accessed directly,
   always through accessor macros.  */

/* This type is used everywhere to refer to a tree node.  */

typedef union tree_node *tree;

#define NULL_TREE (tree) NULL

/* forward defines for Metrowerks */
struct Marker;
struct timeq_tag;

/* Every kind of tree node starts with this structure,
   so all nodes have these fields.

   See the accessor macros, defined below, for documentation of the fields.  */

struct tree_common {
    union tree_node *chain;
    nbits_t nbits;
    unsigned char type:8;
    unsigned char code:8;
    unsigned int label:8;
    unsigned int sub_label:8;

    unsigned constant:1;
    unsigned integer:1;
//  unsigned signed_attr : 1;
//  unsigned self_determined : 1;
    unsigned port_input:1;
    unsigned port_output:1;
    unsigned port_redefined:1;
    unsigned port_collapsed:1;
    unsigned case_default:1;
    /*unsigned parameter_override : 1; *//* uses case_default */
    /*unsigned lib_module : 1; *//* uses case_default */
    /*unsigned net_assign : 1; *//* uses case_default */
    unsigned net_scalared:1;
    unsigned net_vectored:1;
    unsigned stmt_surrogate:1;
    unsigned vector_direction:1;
    unsigned named_port:1;
    /* unsigned port_immediate : 1; *//* uses named_port */
    unsigned interrupt_flag:1;	/* Used only by CASE */
    /* unsigned real : 1; *//* uses interrupt_flag */
    unsigned hierarchical:1;
    unsigned initialized:1;
    unsigned referenced:1;
/* There is room for 0 more attributes.  */
/* -------------------- */
    unsigned unknown_type:1;
    unsigned udp:1;
    unsigned pli_asynch:1;
    unsigned pad:13;
/* There is room for 14 more attributes.  */

//#ifdef ALIGN_32
//  short pad;          /* align to 32 bits */
//#endif
};

/* Define accessors for the fields that all tree nodes have
   (though some fields are not used for all kinds of nodes).  */

/* The tree-code says what kind of node it is.
   Codes are defined in tree.def.  */
#define TREE_CODE(NODE) ((enum tree_code) (NODE)->common.code)
#define TREE_SET_CODE(NODE, VALUE) ((NODE)->common.code = (int) (VALUE))

/* In all nodes that are expressions, this is the data type of the expression.
   In declarations, this is the declaration data type.
   In NET declarations, this is the NET subtype (WIRE, TRI0, etc.) */
#define TREE_TYPE(NODE) ((NODE)->common.type)
#define TREE_SET_TYPE(NODE, VALUE) ((NODE)->common.type = (int) (VALUE))

/* In all nodes that are expressions, assign a "label" that will determine
   the order of execution and the required depth of the stack for that
   expression (From Aho 1986) */
#define TREE_LABEL(NODE) ((NODE)->common.label)

/* In all nodes that are expressions, assign a "sub-label" that is used
   to determin the depth of the stack for a subexpression using the same
   expression length (reduction, conditional, etc. operations may change
   the expression length and will thus reset this value within an
   expression. */
#define TREE_SUB_LABEL(NODE) ((NODE)->common.sub_label)

/* In all nodes that are expressions, "nbits" is the number of bits
   in the result taking into account context dependent-ness. */
#define TREE_NBITS(NODE) ((NODE)->common.nbits)

/* Nodes are chained together for many purposes.
   Decls in the same scope are chained together to record the contents
   of the scope.
   Statement nodes for successive statements used to be chained together.
   Often lists of things are represented by TREE_LIST nodes that
   are chained together.  */

#define TREE_CHAIN(NODE) ((NODE)->common.chain)

/* Define many boolean fields that all tree nodes have.  */

/* For expression nodes, this determines if the expression tree represents
   a constant expression. */
#define TREE_CONSTANT_ATTR(NODE) ((NODE)->common.constant)

/* For expression nodes, this determines if the expression tree represents
   an integer expression. */
#define TREE_INTEGER_ATTR(NODE) ((NODE)->common.integer)

/* Also, determine if expression/constant/ref is signed or unsigned */
#define TREE_SIGNED_ATTR(NODE) ((NODE)->common.signed_attr)

/* Certain operators are defined as have self-determined or context-determined
    lengths as a result. */
#define TREE_SELF_DETERMINED_ATTR(NODE) ((NODE)->common.self_determined)


/* For ports and function/task arguments, define it as being an input,
   output or inout. */
#define PORT_INPUT_ATTR(NODE) ((NODE)->common.port_input)
#define PORT_OUTPUT_ATTR(NODE) ((NODE)->common.port_output)
#define PORT_REDEFINED_ATTR(NODE) ((NODE)->common.port_redefined)

/* For NETs (applied to all DECLs, but should always be 0), mark if a
   source to a net is a collapsed port.  If so, it will be scheduled
   immediately. */
#define PORT_COLLAPSED_ATTR(NODE) ((NODE)->common.port_collapsed)

/* For NETS that are collapsed, indicate this fact in the port argument
   in the instance statement.  It cannot be labeled as PORT_COLLAPSE
   because do_net_eval uses that to ignore it for calculating the value
   to be propigated to the output port. */
#define PORT_IMMEDIATE_ATTR(NODE) ((NODE)->common.named_port)

/* For case statements, mark the statement as having seen a default case
     to flag multiple default cases. */
#define CASE_DEFAULT_ATTR(NODE) ((NODE)->common.case_default)

/* For parameter assignments, indicates that this assignment has
   been overridden.  Use the override rval. */
#define PARAMETER_OVERRIDE_ATTR(NODE) ((NODE)->common.case_default)

/* Modules that have been found in the library are tagged as such since
   the same module name might be used for several library modules, each
   a different number of arguments */
#define LIB_MODULE_ATTR(NODE) ((NODE)->common.case_default)

/* Net assignments get evaluated immediately, as opposed to continuous
   assignements which are scheduled.  Use this attribute to distinguish
   the difference. */
#define NET_ASSIGN_ATTR(NODE) ((NODE)->common.case_default)

/* For nets, indicate if declaration is forcing expansion or forcing
    no expansion. */
#define NET_SCALARED_ATTR(NODE) ((NODE)->common.net_scalared)
#define NET_VECTORED_ATTR(NODE) ((NODE)->common.net_vectored)

/* In a FOR statement, there are two nodes generated, on for the initial
   expression/condition, and one for the iteration expression/condition.
   Mark the interation node so that it can be fixed up in pass 3. */
/* In certain statements, entering it for the first time has a different
   effect that re-entering after a loop or returning after an event.
   Such statements include FOR.  */
#define STMT_SURROGATE_ATTR(NODE) ((NODE)->common.stmt_surrogate)

/* In a repeat statement, this it toggled at runtime to determine of the
   expression should be evaluated (first time through), or if the top
   of the stack should be decremented. */
//#define REPEAT_1ST_ATTR(NODE) ((NODE)->common.repeat_1st)

/* In a vectored data type, this is set if the msb is greater than the
   lsb (usual case) */
#define VECTOR_DIRECTION_ATTR(NODE) ((NODE)->common.vector_direction)

/* In module instances and module declarations, NAMED_PORT_ATTR indicates
   that named ports are being used and in the port list, TREE_VALUE
   points to an IDENTIFIER_NODE that is the name of the port. */
#define NAMED_PORT_ATTR(NODE) ((NODE) -> common.named_port)

/* Determines if this decl is a REAL number */
#define TREE_REAL_ATTR(NODE) ((NODE) -> common.interrupt_flag)

/* For case statements (and maybe others in the future), the statement could
   be interrupted in the middle of processing by control C or single step.
   This is intself is OK, but if another statment is executed, then the state
   needs to be staved.  INTERRUPT_FLAG tells exec to redo the command without
   tracing. */
#define INTERRUPT_FLAG_ATTR(NODE) ((NODE) -> common.interrupt_flag)

/* If an IDENTIFIER_NODE (as well as TASK and FUNCTION nodes)
   contains a hierarchical reference (xx.xxx.xxx...)
   then it will be tagged as such in parse.y. */
#define HIERARCHICAL_ATTR(NODE) ((NODE) -> common.hierarchical)

/* When a block node or decl node has been initialized in pass2 (or pass3),
   set INITIALIZED_ATTR for debugging and sanity check and, in the case of
   module blocks, to avoid being initialized twice. */
#define INITIALIZED_ATTR(NODE) ((NODE) -> common.initialized)

/* When a decl node is referenced, mark it as such.  For now, if a port
   is declared, then referenced, then redeclared, we can't handle this,
   so flag it. */
#define REFERENCED_ATTR(NODE) ((NODE) -> common.referenced)

/* If there is a hierarchical name or a function in an expression, then
   compile-time type conversion won't work.  Do some extra work in pass3.c */
#define UNKNOWN_TYPE_ATTR(NODE) ((NODE) -> common.unknown_type)

/* If module definition is really for a udp flag, set this flag */
#define UDP_ATTR(NODE) ((NODE) -> common.udp)

/* If pli async is enabled set this flag */
#define ASYNCH_ATTR(NODE) ((NODE) -> common.pli_asynch)


/* Define additional fields and accessors for nodes representing constants.  */

/* In an INTEGER_CST node. This represents a signed 32-bit constant. */
#define INT_CST_HIGH(node) ((node)->int_cst.high)
#define INT_CST_LOW(node) ((node)->int_cst.low)
#define INT_CST_DATA(NODE) ((NODE)->int_cst.data)

struct tree_int_cst {
    char common[sizeof(struct tree_common)];
    signed_32_t data;
};

#define REAL_CST_DATA(NODE) ((NODE)->real_cst.data)

struct tree_real_cst {
    char common[sizeof(struct tree_common)];
    double data;
};

/* Bit constants are unsigned, of any length.  TREE_BIT_LENGTH is the
   size of the constant in bits; TREE_BITVAL_POINTER points to a bitval
   structure that stores the value. */

enum radii { BIN, HEX, OCT, DEC, STRING_, CHAR_, EXP_, FLOAT_, EXPFLOAT_,
	TIME_ };

#define BIT_CST_GROUP(NODE) ((NODE)->bit_cst.group)
#define BIT_CST_RADIX(NODE) ((NODE)->bit_cst.radix)
#define BIT_CST_SET_RADIX(NODE, VALUE) ((NODE)->bit_cst.radix = (enum radii) (VALUE))
#define BIT_CST_NBITS(NODE) ((NODE)->bit_cst.nbits)
//#define BIT_CST_NGROUPS(NODE) (((NODE)->bit_cst.nbits) >> GROUP_SHIFT_BITS)

struct tree_bit_cst {
    char common[sizeof(struct tree_common)];
    enum radii radix;
    nbits_t nbits;
    union group *group;
};

/* In a STRING_CST */
#define TREE_STRING_LENGTH(NODE) ((NODE)->string.length)
#define TREE_STRING_POINTER(NODE) ((NODE)->string.pointer)

struct tree_string {
    char common[sizeof(struct tree_common)];
    int length;
    char *pointer;
};

/* Define fields and accessors for some special-purpose tree nodes.  */

#define IDENTIFIER_LENGTH(NODE) ((NODE)->identifier.length)
#define IDENTIFIER_POINTER(NODE) ((NODE)->identifier.pointer)
#define IDENT(NODE) ((NODE)->identifier.pointer)
#define IDENT_CURRENT_DECL(NODE) ((NODE)->identifier.current_decl)

struct tree_identifier {
    char common[sizeof(struct tree_common)];
    int length;
    char *pointer;
    union tree_node *current_decl;
};

/* In a TREE_LIST node.  */
#define TREE_PURPOSE(NODE) ((NODE)->list.purpose)
#define TREE_VALUE(NODE) ((NODE)->list.value)
#define TREE_3RD(NODE) ((NODE)->list.third.exp)

/* Alternate reference method */
#define TREE_EXPR(NODE) ((NODE)->list.purpose)
#define TREE_STMT(NODE) ((NODE)->list.value)
#define TREE_EXPR_CODE(NODE) ((NODE)->list.third.exp_code)

/* Another reference for tmp vars for PLI arguments */
#define TREE_PLIINFO(NODE) ((NODE)->list.value)

struct tree_list {
    char common[sizeof(struct tree_common)];
    union tree_node *purpose;
    union tree_node *value;
    union {
	union tree_node *exp;
	union tree_node **exp_code;
    } third;
};


/* Define fields and accessors for some nodes that represent expressions.  */

/* In expression and reference nodes.  */
#define TREE_OPERAND(NODE, I) ((NODE)->exp.operands[I].exp)
#define REF_NAME(NODE) ((NODE)->exp.operands[0].exp)

#define TRUTH_EXPR_CODE(NODE) ((NODE)->exp.operands[2].exp_code)

#define BIT_REF_DECL(NODE) ((NODE)->exp.operands[0].exp)
#define BIT_EXPR(NODE) ((NODE)->exp.operands[1].exp)
#define BIT_EXPR_CODE(NODE) ((NODE)->exp.operands[2].exp_code)
#define BIT_REF_NAME(NODE) ((NODE)->exp.operands[3].exp)

#define ARRAY_REF_DECL(NODE) ((NODE)->exp.operands[0].exp)
#define ARRAY_EXPR(NODE) ((NODE)->exp.operands[1].exp)
#define ARRAY_EXPR_CODE(NODE) ((NODE)->exp.operands[2].exp_code)
#define ARRAY_REF_NAME(NODE) ((NODE)->exp.operands[3].exp)

#define FUNC_REF_NAME(NODE) ((NODE)->func.name)
#define FUNC_REF_ARGS(NODE) ((NODE)->func.args)
#define FUNC_REF_FUNC(NODE) ((NODE)->func.type.func)
#define FUNC_REF_INASSIGN(NODE) ((NODE)->func.inassign)
#define FUNC_REF_LINE(NODE) ((lineno_t)(NODE)->func.line)
#define FUNC_REF_FILE(NODE) ((NODE)->func.file)
#define FUNC_REF_CONTEXT(NODE) ((NODE)->func.context)
#define FUNC_REF_SYSTYPE(NODE) ((enum sysfunction_type)(NODE)->func.type.systype)
#define SET_FUNC_REF_SYSTYPE(NODE, VALUE) ((NODE)->func.type.systype = \
						(int)(VALUE))
#define FUNC_REF_USERTF(NODE) ((NODE)->func.usertf)
#define FUNC_REF_WORKAREA(NODE) ((NODE)->func.workArea)
#define FUNC_REF_DELAYSCB(NODE) ((NODE)->func.delaySCB)
#define FUNC_REF_RETURNDECL(NODE) ((NODE)->func.returndecl)
#define FUNC_REF_NEXT(NODE) ((NODE)->func.nextinstance)

struct tree_func {
    char common[sizeof(struct tree_common)];
    char *name;
    union tree_node *args;
    union {
	union tree_node *func;
	int systype;
//    enum sysfunction_type systype;
    } type;
    union tree_node *inassign;
    lineno_t line;
    char *file;
    union tree_node *context;
    p_tfcell usertf;		/* PLI user_tf pointer (0 if not PLI) */
    char *workArea;		/* used by pli routines */
    struct SCB *delaySCB;	/* used for tf_setdelay */
    union tree_node *returndecl;	/* decl holding return value */
    union tree_node *nextinstance;	/* linked list of all pli instances */
};

#define HIERARCHICAL_REF_IDENT(NODE) ((NODE)->exp.operands[0].exp)

#define SHIFT_OPERAND(NODE) ((NODE)->shift.operand)
#define SHIFT_COUNT(NODE) ((NODE)->shift.count)
#define SHIFT_NBITS(NODE) ((NODE)->shift.nbits)

struct tree_shift {
    char common[sizeof(struct tree_common)];
    union tree_node *operand;
    union tree_node *count;
    nbits_t nbits;
};

/* This is used for part-selects and conatenations */
#define PART_RMASK(NODE) ((NODE)->rmask)
#define PART_LMASK1(NODE) ((NODE)->lmask1)
#define PART_LMASK2(NODE) ((NODE)->lmask2)
#define PART_NGROUPS(NODE) ((NODE)->ngroups)
#define PART_SHIFT(NODE) ((NODE)->shift)
#define PART_NEXT(NODE) ((NODE)->next)
#define PART_ALIGNED(NODE) ((NODE)->aligned)
struct part_info {
    Bit rmask;
    Bit lmask1;
    Bit lmask2;
    ngroups_t ngroups;
    int shift;
    ngroups_t next;
    unsigned aligned:1;
};

#define COND_EXPR(NODE) ((NODE)->exp.operands[0].exp)
#define COND_EXPR_CODE(NODE) ((NODE)->exp.operands[1].exp_code)
#define COND_TRUE(NODE) ((NODE)->exp.operands[2].exp)
#define COND_TRUE_CODE(NODE) ((NODE)->exp.operands[3].exp_code)
#define COND_FALSE(NODE) ((NODE)->exp.operands[4].exp)
#define COND_FALSE_CODE(NODE) ((NODE)->exp.operands[5].exp_code)

#define PART_INFO(NODE) ((NODE)->part.info)
#define PART_DECL(NODE) ((NODE)->part.decl)
#define PART_MSB_(NODE) ((NODE)->part.msb_)
#define PART_LSB_(NODE) ((NODE)->part.lsb_)
#define PART_MSB(NODE) ((NODE)->part.msb)
#define PART_LSB(NODE) ((NODE)->part.lsb)
#define PART_STORAGE(NODE) ((NODE)->part.storage)
#define PART_NAME(NODE) ((NODE)->part.name)

struct tree_part {
    char common[sizeof(struct tree_common)];
    struct part_info *info;
    union tree_node *decl;
    union tree_node *msb_;
    union tree_node *lsb_;
    nbits_t msb;
    nbits_t lsb;
    Group *storage;
    union tree_node *name;
};

#define CONCAT_LIST(NODE) ((NODE)->concat.list)
#define CONCAT_NBITS(NODE) ((NODE)->concat.nbits)
#define CONCAT_EXPR(NODE) ((NODE)->concat.expr)
#define CONCAT_COUNT(NODE) ((NODE)->concat.count)

struct tree_concat {
    char common[sizeof(struct tree_common)];
    union tree_node *list;
    nbits_t nbits;
    union tree_node *expr;	/* for repeat counts */
    unsigned_32_t count;	/* ditto */
};

struct tree_exp {
    char common[sizeof(struct tree_common)];
    union {
	int value;
	union tree_node *exp;
	union tree_node **exp_code;
    } operands[1];
};

/* Define fields and accessors for nodes representing declared names.  */

#define DECL_NAME(NODE) ((NODE)->decl_common.name)
#define DECL_CONTEXT(NODE) ((NODE)->decl_common.context)
#define DECL_RESULT(NODE) ((NODE)->decl_common.result)
#define DECL_INITIAL(NODE) ((NODE)->decl_common.initial)
#define DECL_SOURCE_FILE(NODE) ((NODE)->decl_common.filename)
#define DECL_SOURCE_LINE(NODE) ((NODE)->decl_common.linenum)
#define DECL_SIZE(NODE) ((NODE)->decl_common.size)
//#define DECL_ACTIVE(NODE) ((NODE)->decl_common.active)
#define DECL_UNUSED_FLAG(NODE) ((NODE)->common.parameter1_attr)
#define DECL_STORAGE(NODE) ((NODE)->decl_common.storage)
//#define DECL_ARRAY(NODE) ((NODE)->common.flag2)
#define DECL_MSB(NODE) ((NODE)->decl_common.msb_)
#define DECL_LSB(NODE) ((NODE)->decl_common.lsb_)
//#define DECL_NBITS(NODE) ((NODE)->decl_common.nbits)
#define DECL_STORAGE(NODE) ((NODE)->decl_common.storage)
#define DECL_EVENT_CHAIN(NODE) ((NODE)->decl_common.event_chain)
#define DECL_EVENT_NOCAST(NODE) ((NODE)->decl_common.event_chain)
#define DECL_STATE(NODE) ((enum logical_value) (NODE)->decl_common.state)
#define SET_DECL_STATE(NODE, VALUE) ((NODE)->decl_common.state = (enum logical_value)(VALUE))
#define DECL_THREAD(NODE) ((NODE)->decl_common.thread)
#define DECL_UPDATE_FILE(NODE) ((NODE)->decl_common.update_file)
#define DECL_UPDATE_LINE(NODE) ((NODE)->decl_common.update_line)
#define DECL_UPDATE_TIME(NODE) ((NODE)->decl_common.update_time)

#define MSB(NODE) ((NODE)->decl_common.msb)
#define LSB(NODE) ((NODE)->decl_common.lsb)

struct tree_decl_common {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    int size;
    union tree_node *name;
    union tree_node *context;
    union tree_node *msb_;
    union tree_node *lsb_;
    nbits_t msb;
    nbits_t lsb;
//  nbits_t nbits;
    union group *storage;
    struct Marker *event_chain;
//  void *event_chain;
    int state;
//  enum logical_value state;
    union tree_node *thread;
    char *update_file;
    lineno_t update_line;
    time64 update_time;
};

/* In a PLI_DELAY node */
#define PLI_DELAY_NEXT(NODE) ((NODE)->pli_delay.next)
#define PLI_DELAY_NODE(NODE) ((NODE)->pli_delay.node)
#define PLI_DELAY_TIME(NODE) ((NODE)->pli_delay.time)
#define PLI_DELAY_STORAGE(NODE) ((NODE)->pli_delay.storage)
#define PLI_DELAY_LVAL(NODE) ((NODE)->pli_delay.lval)

struct tree_pli_delay {
    char common[sizeof(struct tree_common)];
    union tree_node *next;
    union tree_node *node;
    time64 time;
    union group *storage;
    union tree_node *lval;
};




/* In a PATH_INPUT_TERMINAL (TREE_LIST) */
#define MODPATH_INPUT_TERMINAL_PARENT(NODE) TREE_STMT(NODE)

/* In a PATH_NODE node.  */
#define PATH_SOURCE_FILE(NODE) ((NODE)->path.filename)
#define PATH_SOURCE_LINE(NODE) ((NODE)->path.linenum)
#define PATH_DELAYS(NODE) ((NODE)->path.delays)
#define PATH_INPUTS(NODE) ((NODE)->path.inputs)
#define PATH_OUTPUTS(NODE) ((NODE)->path.outputs)
#define PATH_PARALLEL(NODE) ((NODE)->path.parallel)
#define PATH_SOURCE(NODE) ((NODE)->path.source)
#define PATH_CONDITION(NODE) ((NODE)->path.condition)
#define PATH_EDGE(NODE) ((NODE)->path.edge)
#define PATH_POLARITY(NODE) ((NODE)->path.polarity)

struct tree_path {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *delays;
    union tree_node *inputs;
    union tree_node *outputs;
    union tree_node *source;
    union tree_node *condition;
    int parallel;
    int edge;
    int polarity;
};



/* in PATH_INSTANCE */
#define PATH_INSTANCE_MODULE(NODE) ((NODE)->path_instance.module)
#define PATH_INSTANCE_PATHDESC(NODE) ((NODE)->path_instance.pathdesc)

struct tree_path_instance {
    char common[sizeof(struct tree_common)];
    union tree_node *module;	/* points to containing module */
    union tree_node *pathdesc;	/* path description */
};



/* in PATH_OUTPUT */
#define PATH_OUTPUT_GATE(NODE) ((NODE)->path_output.gate)
#define PATH_OUTPUT_VALUE(NODE) ((NODE)->path_output.value)
#define PATH_OUTPUT_PATHDESC(NODE) ((NODE)->path_output.pathdesc)
#define PATH_OUTPUT_CONSTRAINTS(NODE) ((NODE)->path_output.constraints)
#define PATH_OUTPUT_SCB(NODE) ((NODE)->path_output.scb)
#define PATH_OUTPUT_CURRENT_VALUE(NODE) ((NODE)->path_output.current_value)

struct tree_path_output {
    char common[sizeof(struct tree_common)];
    union tree_node *gate;	/* points to associated gate */
    enum logical_value value;	/* keeps track of value to be propagated */
    union tree_node *pathdesc;	/* list of path descriptions */
    union tree_node *constraints;	/* list of path constraints */
    struct SCB *scb;		/* scb for specify path */
    enum logical_value current_value;	/* keeps track of current value */
};



#define PATH_CONSTRAINT_DELAY(NODE,FROM,TO) ((NODE)->path_constraint.delays[((FROM)<<2)|(TO)])
#define PATH_CONSTRAINT_DEPENDENCIES(NODE) ((NODE)->path_constraint.dependencies)
#define PATH_CONSTRAINT_PATHDESC(NODE) ((NODE)->path_constraint.pathdesc)
#define PATH_CONSTRAINT_INSTANCE(NODE) ((NODE)->path_constraint.instance)
#define PATH_CONSTRAINT_OUTPUT_NUMBER(NODE) ((NODE)->common.sub_label)

struct tree_path_constraint {
    char common[sizeof(struct tree_common)];
    union tree_node *dependencies;
    union tree_node *pathdesc;
    union tree_node *instance;
    unsigned         delays[16];
};

/* timing check specification */

#define CHECK_SPEC_EVENT1(NODE)       ((NODE)->check_spec.event1)
#define CHECK_SPEC_EVENT2(NODE)       ((NODE)->check_spec.event2)
#define CHECK_SPEC_PARAM1(NODE)       ((NODE)->check_spec.param1)
#define CHECK_SPEC_PARAM2(NODE)       ((NODE)->check_spec.param2)
#define CHECK_SPEC_NOTIFIER(NODE) ((NODE)->check_spec.notifier)
#define CHECK_SPEC_CHECKTYPE(NODE)    ((NODE)->check_spec.checkType)

struct tree_check_spec {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *event1;
    union tree_node *event2;
    union tree_node *param1;
    union tree_node *param2;
    union tree_node *notifier;
    unsigned         checkType;
};

/*    specify timing event specification */

#define TIMING_EVENT_EXPRESSION(NODE) ((NODE)->timing_event.expression)
#define TIMING_EVENT_CONDITION(NODE)  ((NODE)->timing_event.condition)
#define TIMING_EVENT_EDGESPEC(NODE)   ((NODE)->timing_event.edgeSpec)
struct tree_timing_event {
    char common[sizeof(struct tree_common)];
    union tree_node *expression;
    union tree_node *condition;
    unsigned         edgeSpec;
};

/* timing check terminal */

#define TIMING_TERMINAL_CHECK(NODE) (TREE_STMT(NODE))

/* timing check node */

#define       TIMING_CHECK_PARAM1(NODE) ((NODE)->timing_check.param1)
#define       TIMING_CHECK_PARAM2(NODE) ((NODE)->timing_check.param2)
#define       TIMING_CHECK_EXPRESSIONCODE(NODE,I) ((NODE)->timing_check.expressionCode[I])
#define       TIMING_CHECK_EXPRESSION(NODE,I) ((NODE)->timing_check.expression[I])
#define       TIMING_CHECK_NOTIFIER(NODE) ((NODE)->timing_check.notifier)
#define       TIMING_CHECK_CURRENTVALUE(NODE,I) ((NODE)->timing_check.currentValue[I])
#define       TIMING_CHECK_CHANGETIME(NODE,I) ((NODE)->timing_check.changeTime[I])
#define       TIMING_CHECK_CONDITION(NODE,I) ((NODE)->timing_check.condition[I])
#define       TIMING_CHECK_CONDITIONCODE(NODE,I) ((NODE)->timing_check.conditionCode[I])
#define       TIMING_CHECK_EDGESPEC(NODE,I) ((NODE)->timing_check.edgeSpec[I])
#define       TIMING_CHECK_CHECKSPEC(NODE) ((NODE)->timing_check.checkSpec)
#define       TIMING_CHECK_MODULE(NODE) ((NODE)->timing_check.module)

struct tree_timing_check {
    char common[sizeof(struct tree_common)];
    int param1;
    int param2;
    union tree_node *expression[2];
    union tree_node **expressionCode[2];
    union tree_node *notifier;
    int currentValue[2];
    time64 changeTime[2];
    union tree_node *condition[2];
    union tree_node **conditionCode[2];
    unsigned int edgeSpec[2];
    union tree_node *checkSpec;
    union tree_node *module;
};

#define NOTIFIER_DECL(NODE)  ((NODE)->common.chain)
#define NOTIFIER_TIMESTAMP(NODE) ((NODE)->notifier.timeStamp)

struct tree_notifier {
    char common[sizeof(struct tree_common)];
    time64 timeStamp;
};

#define UDP_STRING_STRING(NODE) ((NODE)->udp_string.string)
#define UDP_STRING_FILE(NODE) ((NODE)->udp_string.filename)
#define UDP_STRING_LINE(NODE) ((NODE)->udp_string.linenum)

struct tree_udp_string {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    char string[28];
};

#define UDP_TABLE_ARRAY(NODE,I) ((NODE)->udp_table.array[I])

struct tree_udp_table {
    char common[sizeof(struct tree_common)];
    char *array[10];
};

/* Array is used as a modifier; noun points to decl node.  Array is used
   for regs (scalar and vector), int, time and real (eventually) */
//#define ARRAY_DECL(NODE) ((NODE)->decl_array.decl)
#define ARRAY_CODE(NODE) ((NODE)->decl_array.code)
#define ARRAY_HI_EXPR(NODE) ((NODE)->decl_array.hi_expr)
#define ARRAY_LO_EXPR(NODE) ((NODE)->decl_array.lo_expr)
#define ARRAY_HI(NODE) ((NODE)->decl_array.hi)
#define ARRAY_LO(NODE) ((NODE)->decl_array.lo)
/* Recast storage to be array of pointers */
//#define ARRAY_STORAGE(NODE) ((Group **)(NODE)->decl_common.storage)

struct tree_decl_array {
    char decl_common[sizeof(struct tree_decl_common)];
//  union tree_node *decl;
    enum tree_code code;
    union tree_node *lo_expr;
    union tree_node *hi_expr;
    nbits_t lo;
    nbits_t hi;
};

/* For net declarations.  STMT_SURROGATE_ATTR indicates that the net is one
   of many sources to the net and holds the value for that source. */

#define NET_DELAY(NODE) ((NODE)->decl_net.delay)
#define NET_SOURCE(NODE) ((NODE)->decl_net.source)
#define NET_ASSIGNMENT(NODE) ((NODE)->decl_net.assignment)

struct tree_decl_net {
    char decl_common[sizeof(struct tree_decl_common)];
    union tree_node *delay;	/* points to delay expression for the net */
    union tree_node *source;	/* points to other sources to this net */
    union tree_node *assignment;	/* points to con't ass't for this net */
};

/* This is used for port terminal connections */

#define PORT_TERMINAL_HICONN(NODE) (TREE_VALUE(NODE))

/* This is used for module ports and function/task arguments */

#define DECL_PORT_NOUN(NODE) ((NODE)->tree_decl_port.noun)
#define DECL_TASK_NOUN(NODE) ((NODE)->tree_decl_port.noun)
#define DECL_PORT_MSB(NODE) ((NODE)->tree_decl_port_vector.msb)
#define DECL_PORT_LSB(NODE) ((NODE)->tree_decl_port_vector.lsb)
struct tree_decl_port {
    char decl_common[sizeof(struct tree_decl_common)];
    union tree_node *noun;
};

struct tree_decl_port_vector {
    char decl_port[sizeof(struct tree_decl_port)];
    union tree_node *msb;
    union tree_node *lsb;
};

/* Parameter declaration.  Points to constant expression that represents
    default value. */

//#define DECL_PARAM_INIT(NODE) ((NODE)->decl_parameter.initial)
#define DECL_PARAM_RVAL(NODE) ((NODE)->decl_parameter.rval)
#define DECL_PARAM_RVAL_CODE(NODE) ((NODE)->decl_parameter.rval_code)
#define DECL_PARAM_REDIRECT(NODE) ((NODE)->decl_parameter.redirect)

struct tree_decl_parameter {
    char decl_common[sizeof(struct tree_decl_common)];
    union tree_node *rval;
    union tree_node **rval_code;
    union tree_node *redirect;
};



/* This is used at runtime to keep track of which SCB is in which scope.
   It is needed for disable and for future $list */

struct context_member {
    struct context_member *next;
    struct context_member **prev;
    struct context_member *shadow;	/* context_member of previous context */
    struct SCB *scb;		/* Owner */
    union tree_node *pc;	/* PC to which to return upon leaving */
    union tree_node *scope;	/* Pointer to scoping block */
};

/* Define scoping block nodes */

#define BLOCK_SOURCE_LINE(NODE) ((NODE)->block_common.linenum)
#define BLOCK_SOURCE_FILE(NODE) ((NODE)->block_common.filename)
#define BLOCK_NAME(NODE) ((NODE)->block_common.name)
#define BLOCK_DECL(NODE) ((NODE)->block_common.decl)
#define BLOCK_PORTS(NODE) ((NODE)->block_common.ports)
#define BLOCK_ARGS(NODE) ((NODE)->block_common.ports)
#define BLOCK_PARAM(NODE) ((NODE)->block_common.param)
#define BLOCK_DOWN(NODE) ((NODE)->block_common.down)
#define BLOCK_UP(NODE) ((NODE)->block_common.up)
#define BLOCK_BODY(NODE) ((NODE)->block_common.body)
#define BLOCK_CONTEXT_LIST(NODE) ((NODE)->block_common.context_list)
#define BLOCK_LCB_LIST(NODE) ((NODE)->block_common.lcb)

struct tree_block_common {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *name;
    union tree_node *decl;
    union tree_node *param;
    union tree_node *ports;	// Maybe merge w/decl later?
    union tree_node *down;
    union tree_node *up;
    union tree_node *body;
    struct context_member *context_list;
    union tree_node *lcb;	// line execution call back list
//  union tree_node *scb_list;
};

#define MODULE_PORT_LIST(NODE) ((NODE)->block_module.port_list)
//#define MODULE_INSTANCES(NODE) ((NODE)->block_module.common.module.instances)
#define MODULE_INSTANCES(NODE) ((NODE)->block_common.body)
#define MODULE_ASSIGNMENTS(NODE) ((NODE)->block_module.common.module.assignments)
#define MODULE_NAME(NODE) ((NODE)->block_module.name)
#define MODULE_DEFPARAMS(NODE) ((NODE)->block_module.common.module.defparams)
#define MODULE_SPECDEFS(NODE) ((NODE)->block_module.common.module.specdefs)
#define MODULE_SPECINST(NODE) ((NODE)->block_module.common.module.specinst)

#define MODULE_TIME_UNIT(NODE) ((NODE)->block_module.time_unit)
#define MODULE_TIME_PREC(NODE) ((NODE)->block_module.time_prec)

/* UDP definitions overlay the same structure and type as modules
   UDP will have the UDP_ATTR flag set */

#define UDP_PORT_LIST(NODE) ((NODE)->block_module.port_list)
#define UDP_NAME(NODE) ((NODE)->block_module.name)
#define UDP_REG_NAME(NODE) ((NODE)->block_module.common.udp.reg_name)
#define UDP_INITIAL_VALUE(NODE) ((NODE)->block_module.common.udp.initial_value)
#define UDP_STRING_LIST(NODE) ((NODE)->block_module.common.udp.string_list)
#define UDP_TABLE(NODE) ((NODE)->block_module.common.udp.udp_table)

enum module_code { INITIAL_CODE, ALWAYS_CODE };

struct tree_block_module {
    char block_common[sizeof(struct tree_block_common)];
    char *name;
    union tree_node *port_list;
    union tree_node *instances;
    union {
	struct {
	    union tree_node *assignments;
	    union tree_node *defparams;
	    union tree_node *specinst;
	    union tree_node *specdefs;
	} module;
	struct {
	    union tree_node *reg_name;
	    union tree_node *initial_value;
	    union tree_node *string_list;
	    union tree_node *udp_table;
	} udp;
    } common;
    unsigned char time_unit:8;
    unsigned char time_prec:8;
};

/* Block structure for functions (and tasks). decl is the decl node for
   the return var */

#define FUNCT_INPUT(NODE) ((NODE)->block_funct.input)
#define FUNCT_OUTPUT(NODE) ((NODE)->block_funct.output)
#define FUNCT_DECL(NODE) ((NODE)->block_funct.decl)
//#define FUNCT_DECL_CODE(NODE) ((NODE)->block_funct.code)
#define FUNCT_AREA(NODE) ((NODE)->block_funct.area)

#define TASK_INPUT(NODE) ((NODE)->block_funct.input)
#define TASK_OUTPUT(NODE) ((NODE)->block_funct.output)

struct tree_block_funct {
    char block_common[sizeof(struct tree_block_common)];
    union tree_node *input;
    union tree_node *output;
    union tree_node *decl;
//  union tree_node **code;
    ngroups_t area;
};

#define GATE(NODE) (&((NODE)->gate_instance))
#define GATE_FILE(NODE) ((NODE)->gate_instance.filename)
#define GATE_LINE(NODE) ((NODE)->gate_instance.linenum)
#define GATE_SCOPE(NODE) ((NODE)->gate_instance.scope)
#define GATE_NAME(NODE) ((NODE)->gate_instance.name)
#define GATE_ROUTINE(NODE) ((NODE)->gate_instance.routine)
#define GATE_PROPAGATE_ROUTINE(NODE) ((NODE)->gate_instance.propagate_routine)
#define GATE_DELAY(NODE) ((NODE)->gate_instance.delay)
#define GATE_MARKERS(NODE) ((NODE)->gate_instance.markers)
#define GATE_TIME(NODE) ((NODE)->gate_instance.time)
#define GATE_OUTPUT(NODE) ((NODE)->gate_instance.output)
#define GATE_STATE(NODE) ((NODE)->gate_instance.state)
#define GATE_INPUTS(NODE) ((NODE)->gate_instance.total_inputs)
#define GATE_INPUT_LIST(NODE) ((NODE)->gate_instance.input_ports)
#define GATE_OUTPUT_LIST(NODE) ((NODE)->gate_instance.output_ports)
#define GATE_FIRED_INPUTS(NODE) ((NODE)->gate_instance.shared.gate.fired_inputs)
#define GATE_CONTROL(NODE) ((NODE)->gate_instance.shared.gate.fired_inputs)
#define GATE_ZEROS(NODE) ((NODE)->gate_instance.shared.gate.fired_inputs)
#define GATE_ONES(NODE) ((NODE)->gate_instance.shared.gate.fired_inputs)
#define GATE_X(NODE) ((NODE)->gate_instance.shared.gate.any_Xs)
#define GATE_NEXT(NODE) ((NODE)->gate_instance.next)
#define GATE_PREV(NODE) ((NODE)->gate_instance.prev)
#define GATE_INPUT_NUMBER(NODE) ((NODE)->common.sub_label)
#define GATE_TIMEQ(NODE) ((NODE)->gate_instance.timeq)

/* Holds/sets the current value of the gate input */
#define GATE_IN(LIST) ((enum logical_value)(LIST)->common.nbits)
#define SET_GATE_IN(LIST, NEW) ((LIST)->common.nbits=(NEW))
#define GATE_PATH_OUTPUT(NODE) ((NODE)->gate_instance.path_output)
#define GATE_TERMINAL_GATE(NODE) (TREE_STMT(NODE))
#define GATE_TERMINAL_NUMBER(NODE) ((NODE)->common.label)

/* special members for udp gates */
#define GATE_UDP_DEF(NODE) ((NODE)->gate_instance.shared.udp.udp_def)


struct tree_gate_instance {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *scope;
    char *name;			/* name of the instance */
    void (*routine) (struct Marker *);	/* routine to call to process gate */
    void (*propagate_routine) (union tree_node *);	/* routine to call to propage gate output */
    union tree_node *input_ports;	/* TREE_LIST of inputs */
    union tree_node *output_ports;	/* TREE_LIST of outputs */
    union tree_node *delay;	/* delay structure */
    struct Marker *markers;	/* Pointer to markers */
//  void *markers;
//  time64 time;                          /* time to trigger if delayed */
    enum logical_value output;	/* scheduled output */
    int state;			/* State of gate */
    int total_inputs;		/* total number of inputs */
//  struct tree_gate_instance *next;      /* For threading on the wait list */
//  struct tree_gate_instance **prev;     /*  (ok to use ADD_LIST, etc.) */
    tree next;			/* next gate for given time */
    tree prev;			/*  prev gate for given time */
    struct timeq_tag *timeq;	/* pointer to index node */
    union tree_node *path_output;	/* pointer to specify path */
    union {
	struct {
	    union tree_node *udp_def;	/* points to udp_def node */
	    int dummy;		/* not used */
	} udp;
	struct {
	    int fired_inputs;	/* number of inputs at 1 (or 0) */
	    int any_Xs;		/* Are any inputs X (or Z)? */
	} gate;
    } shared;


//  union tree_node *next;      /* For threading on the wait list */
//  union tree_node **prev;     /*  (ok to use ADD_LIST, etc.) */
};


/* Define fields and accessors for nodes representing statements. */

/* Common for most statements */
#define STMT_SOURCE_LINE(NODE) ((NODE)->stmt.linenum)
#define STMT_SOURCE_FILE(NODE) ((NODE)->stmt.filename)
#define STMT_SCOPE(NODE) ((NODE)->stmt.scope)
#define STMT_BODY(NODE) ((NODE)->stmt.operand[0].exp)
#define STMT_OPERAND(NODE,I) ((NODE)->stmt.operand[I].exp)

/* For IF_STMT.  */
#define STMT_COND(NODE) ((NODE)->stmt.operand[0].exp)
#define STMT_THEN(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_ELSE(NODE) ((NODE)->stmt.operand[2].exp)
#define STMT_COND_CODE(NODE) ((NODE)->stmt.operand[3].exp_code)

/* For CASE_STMT.  */
#define STMT_CASE_EXPR(NODE) ((NODE)->stmt.operand[0].exp)
#define STMT_CASE_LIST(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_CASE_DEFAULT(NODE) ((NODE)->stmt.operand[2].exp)
#define STMT_CASE_EXPR_CODE(NODE) ((NODE)->stmt.operand[3].exp_code)
//#define STMT_CASE_DEFAULT_ATTR(NODE) ((NODE)->common.general1_attr)

/* For REPEAT_STMT */
#define STMT_REPEAT_EXPR(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_REPEAT_EXPR_CODE(NODE) ((NODE)->stmt.operand[2].exp_code)
#define STMT_REPEAT_VAR(NODE) ((NODE)->stmt.operand[3].group)

/* For WHILE_STMT */
#define STMT_WHILE_COND(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_WHILE_COND_CODE(NODE) ((NODE)->stmt.operand[2].exp_code)

/* For FOR_STMT */
#define STMT_FOR_ASSIGN(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_FOR_COND(NODE) ((NODE)->stmt.operand[2].exp)
#define STMT_FOR_COND_CODE(NODE) ((NODE)->stmt.operand[3].exp_code)

/* Delay control */
#define STMT_DELAY_EXPR(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_DELAY_EXPR_CODE(NODE) ((NODE)->stmt.operand[2].exp_code)

/* Named BEGIN or FORK */
#define STMT_BLOCK(NODE) ((NODE)->stmt.operand[1].exp)
/* points to stmt after end (points to end at parse; fixed up in pass 3) */
//#define STMT_BEGIN_NAMED_END(NODE) ((NODE)->stmt.operand[2].exp)

/* Fork stmts */
#define STMT_FORK_SCB(NODE) ((NODE)->stmt.operand[2].scb)
/* Join stmt */
//#define STMT_JOIN_FORK(NODE) ((void *)(NODE)->stmt.operand[0].exp)
#define STMT_JOIN_FORK(NODE) ((NODE)->stmt.operand[0].exp)

/* Tasks and system tasks */
#define STMT_TASK_NAME(NODE) ((NODE)->stmt.operand[0].name)
#define STMT_TASK_ARGS(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_TASK_TASK(NODE) ((NODE)->stmt.operand[2].exp)
#define STMT_TASK_INASSIGN(NODE) ((NODE)->stmt.operand[3].exp)
#define STMT_TASK_OUTASSIGN(NODE) ((NODE)->stmt.operand[4].exp)

#define STMT_SYSTASK_TYPE(NODE) ((NODE)->stmt.operand[2].systask)
#define SET_STMT_SYSTASK_TYPE(NODE, VALUE) ((NODE)->stmt.operand[2].systask = \
							(int)(VALUE))
#define STMT_SYSTASK_USERTF(NODE) ((NODE)->stmt.operand[5].p_tf)
#define STMT_SYSTASK_WORKAREA(NODE) ((NODE)->stmt.operand[6].workArea)
#define STMT_SYSTASK_DELAYSCB(NODE) ((NODE)->stmt.operand[7].delaySCB)
#define STMT_SYSTASK_NEXT(NODE) ((NODE)->stmt.operand[8].nextInstance)

/* DISABLE_STMT */
#define STMT_DISABLE_NAME(NODE) ((NODE)->stmt.operand[0].exp)
#define STMT_DISABLE_EXPR(NODE) ((NODE)->stmt.operand[0].exp)
#define STMT_DISABLE_SCOPE(NODE) ((NODE)->stmt.operand[1].exp)


/* WAIT_STMT */
#define STMT_WAIT_EXPR(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_WAIT_EXPR_CODE(NODE) ((NODE)->stmt.operand[2].exp_code)
#define STMT_WAIT_MARKER(NODE) ((NODE)->stmt.operand[3].marker)

/* For event triggereing */
#define STMT_EVENT_EXPR_LIST(NODE) ((NODE)->stmt.operand[1].exp)
#define STMT_EVENT_MARKER(NODE) ((NODE)->stmt.operand[2].marker)

/* -> foo: NAME is used for trace, EVENT is used for exec; both are same
   unless NAME is hierachical. */
#define STMT_ARROW_NAME(NODE) ((NODE)->stmt.operand[0].exp)
#define STMT_ARROW_EVENT(NODE) ((NODE)->stmt.operand[1].exp)

/* Undo ASSIGN and FORCE */
#define STMT_DEASSIGN_LVAL(NODE) ((NODE)->stmt.operand[0].exp)

/* Note: any changes here must also be reflected in assign and instance */
struct tree_stmt {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *scope;
    union {
	union tree_node *exp;
	union tree_node **exp_code;
	char *name;
	struct Marker *marker;
//  void *marker;
	struct SCB *scb;
//  void *scb;
	Group *group;
	int systask;
	char *workArea;
	struct SCB *delaySCB;
/*    enum systask_type systask;	/* system task type, if internal */
	p_tfcell p_tf;		/* points to tf structure for pli tasks */
	union tree_node *nextInstance;
    } operand[1];
};

/* All assignments use all or part of this structure.  Simple procedural
   assignments use lval, rval, and rval code.  Continuous assignments use
   the whole structure.  See tree.def for a full listing. */

#define STMT_ASSIGN_LVAL(NODE) ((NODE)->assign.lval)
#define STMT_ASSIGN_RVAL(NODE) ((NODE)->assign.rval)
#define STMT_ASSIGN_RVAL_CODE(NODE) ((NODE)->assign.rval_code)

#define STMT_ASSIGN_EVENT_LIST(NODE) ((NODE)->assign.event_list)
#define STMT_ASSIGN_DELAY(NODE) ((NODE)->assign.arg1.delay)
#define STMT_ASSIGN_EVENT(NODE) ((NODE)->assign.arg1.delay)

#define STMT_ASSIGN_TMP(NODE) ((NODE)->assign.tmp)
#define STMT_ASSIGN_TMPCODE(NODE) ((NODE)->assign.tmp_code)

#define STMT_ASSIGN_SCB(NODE) ((NODE)->assign.scb)
#define STMT_FORCE_TMP_DECL(NODE) ((NODE)->assign.arg1.tmp)
/* If the following changes, change all calls to build_stmt(ASSIGN...) */

struct tree_assign {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *scope;
    union tree_node *lval;
    union tree_node *rval;
    union tree_node **rval_code;
    struct Marker *event_list;
//  void *event_list;
    union {
       union tree_node *delay;	/* used for cont assign and intra-asgn timing */
       union tree_node *tmp;    /* used for temp storage */
    } arg1;
    union tree_node *tmp;	/* saved rval for intra-asgn and non blk */
    union tree_node **tmp_code;
    union tree_node *repeat_count;
    struct SCB *scb;		/* list of scbs in non blk event assigns */
//  void *scb;
};

/* This is a placeholder for module instantiations within a module.  If
    NAMED_PORT_ATTR is set, then TREE_VALUE of the port list points to
    and IDENTIFIER_NODE that is the name of the port. */

#define INSTANCE_FILE(NODE) ((NODE)->instance.filename)
#define INSTANCE_LINE(NODE) ((NODE)->instance.linenum)
#define INSTANCE_NAME(NODE) ((NODE)->instance.name)
#define INSTANCE_MODULE_NAME(NODE) ((NODE)->instance.module_name)
#define INSTANCE_PORTS(NODE) ((NODE)->instance.ports)
#define INSTANCE_PARAMS(NODE) ((NODE)->instance.params)
#define INSTANCE_BLOCK(NODE) ((NODE)->instance.block)

struct tree_instance {
    char common[sizeof(struct tree_common)];
    char *filename;
    lineno_t linenum;
    union tree_node *scope;
    union tree_node *name;	/* name of the instance */
    union tree_node *module_name;	/* name of the referenced module */
    union tree_node *ports;	/* TREE_LIST of arguments */
    union tree_node *params;	/* TREE_LIST or param overrides */
    union tree_node *block;	/* Points to block after pass2 */
};

/* Define the overall contents of a tree node.
   It may be any of the structures declared above
   for various types of node.  */

union tree_node {
    struct tree_common common;
    struct tree_int_cst int_cst;
    struct tree_real_cst real_cst;
    struct tree_string string;
    struct tree_identifier identifier;
    struct tree_bit_cst bit_cst;
    struct tree_list list;
    struct tree_part part;
    struct tree_concat concat;
    struct tree_exp exp;
    struct tree_func func;
    struct tree_shift shift;
    struct tree_stmt stmt;
    struct tree_decl_common decl_common;
    struct tree_decl_net decl_net;
    struct tree_decl_array decl_array;
    struct tree_decl_parameter decl_parameter;
    struct tree_block_common block_common;
    struct tree_block_module block_module;
    struct tree_block_funct block_funct;
    struct tree_instance instance;
    struct tree_assign assign;
    struct tree_path path;
    struct tree_path_constraint path_constraint;
    struct tree_timing_event timing_event;
    struct tree_check_spec check_spec;
    struct tree_timing_check timing_check;
    struct tree_notifier notifier;
    struct tree_gate_instance gate_instance;
    struct tree_path_output path_output;
    struct tree_udp_string udp_string;
    struct tree_udp_table udp_table;
    struct tree_path_instance path_instance;
    struct tree_pli_delay pli_delay;
};
char *linkalloc (long);

/* Lowest level primitive for allocating a node.
   The TREE_CODE is the only argument.  Contents are initialized
   to zero except for a few of the common fields.  */

tree make_node(enum tree_code);

/* Make a copy of a node, with all the same contents except
   for TREE_UID and TREE_PERMANENT.  (The copy is permanent
   iff nodes being made now are permanent.)  */

tree copy_node(tree);

/* Return the (unique) IDENTIFIER_NODE node for a given name.
   The name is supplied as a char *.  */

tree get_identifier(char *);

/* Construct various types of nodes.  */

tree build(enum tree_code, enum tree_type, ...);
tree build_nt(enum tree_code, ...);
tree build_tree_list(tree, tree);
tree build_decl(enum tree_code, tree, enum tree_type);
tree build_string(int, char *);
tree build_array(tree, tree, tree, tree);
tree build_module(tree);
void end_module(tree);
tree build_task(tree);
tree build_function(tree);
tree build_stmt(enum tree_code, ...);
void add_cont_assign_list(tree);
tree build_cont_assign(tree, tree, lineno_t, tree, int);

/* Construct expressions, performing type checking.  */

tree build_binary_op(enum tree_code, tree, tree);
tree build_unary_op(enum tree_code, tree);
tree build_cond_expr(tree, tree, tree);

tree build_int_cst(signed_32_t);
tree build_real_cst(char *);
tree build_bit_cst(nbits_t, enum radii);

/* Concatenate two lists (chains of TREE_LIST nodes) X and Y
   by making the last node in X point to Y.
   Returns X, except if X is 0 returns Y.  */

tree chainon(tree, tree);

/* Make a new TREE_LIST node from specified PURPOSE, VALUE and CHAIN.  */

tree tree_cons(tree, tree, tree);
tree perm_tree_cons(tree, tree, tree);
tree link_tree_cons(tree, tree, tree);
void free_link_list(tree);

/* Reverse the order of elements in a chain, and return the new head.  */

tree nreverse(tree);


/* Returns the length of a chain of nodes
   (number of chain pointers to follow before reaching a null pointer).  */

int list_length(tree);
int lvalue_or_else();
int is_proc_lval(tree);
tree the_lval(tree);

/* Declare commonly used variables for tree structure.  */

/* Node to pass lsb/msb expressions during parse */
extern tree range_handle;

/* Points to a TREE_LIST of parsed modules (defined in parse.y) */
extern tree module_list;

/* Points to a chain of top level modules */
extern tree top_level;

/* List of all port connections */
extern tree port_connections;

/* List of all continuous assignments for initializing */
extern tree continuous_assignments;

/* An integer constant with value 0 */
//extern tree integer_zero_node;

/* An integer constant with value 1 */
//extern tree integer_one_node;

/* A single-bit constant 0 (1'b0) */
//extern tree bit_zero_node;

/* a single-bit constant 1 (1'b1) */
//extern tree bit_one_node;

/* A node of type ERROR_MARK.  */
extern tree error_mark_node;

/* Points to the name of the input file from which the current input
   being parsed originally came (before it went into cpp).  */
extern char *input_filename;

/* Current source line number in that file.  */
extern lineno_t lineno;
extern lineno_t stmt_lineno;

/* Points to the FUNCTION_DECL of the function whose body we are reading. */

extern tree current_function_decl;


void clear_temporary();

void concat_labels(tree);

tree check_lval(tree, enum lval_type, tree);
tree check_lval_nocheck(tree, enum lval_type, tree);
tree check_lval_port(tree);
tree check_rval(tree);
tree check_rval_nocheck(tree);
tree check_block(tree);
tree check_task(tree);
tree check_function(tree);
tree check_reg(tree);
tree check_non_reg(tree);
tree check_net(tree);
tree check_port(tree);
tree check_named_event(tree);
tree build_bit_ref(tree, tree);
tree build_part_ref(tree, tree, tree);
tree build_function_call(tree, tree);
tree make_net_source(tree);

tree implicit_conversion(tree, tree);

tree double_check_block(tree);
tree double_check_named_event(tree);

int is_net_code(enum tree_code);
int is_all_net(tree);
int is_array(tree);
int is_string(tree);
int is_var(tree, tree *);
int is_block(tree, tree *);
char *decl_type(tree);
char *ident_name(tree);
void init_tree(void);

void switch_to_linkage_obstack(void);
void switch_to_permanent_obstack(void);
void start_identifier_warnings(void);
/* Set all of the values of a list of TREE_LIST nodes to the same value. */

void set_all_list_values(tree, tree);
void init_tree_1(void);

// private methods

#ifdef TREE_C
#endif				// TREE_C

#endif				// TREE_H
