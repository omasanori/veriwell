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

/* EVAL.H - Macros and prototypes for Eval */

#ifndef EVAL_H
#define EVAL_H

// public methods

/* This macro determines if the value point to by G is 0, 1, or X/Z */
/* (THIS MACRO ALSO APPEARS IN EXEC.C) */

#define DETERMINE_TRUE_COND(G)                          \
        {G = *--R;                                      \
        cond = ZERO;                                    \
        for (i = 0; i < R_ngroups; i++, (G)++) {        \
          if (BVAL (G)) {                               \
            cond = X;                                   \
            G = *R + R_ngroups;                         \
            break;                                      \
          }                                             \
          else if (AVAL (G))                            \
            cond = ONE;                                 \
        }                                               \
        if (BVAL (G) & R_mask)                          \
          cond = X;                                     \
        else if (AVAL (G) & R_mask)                     \
          cond = ONE;}

#define SET_STATE(G)                                    \
                {                                       \
                  if (DECL_STATE (lval) != X) {         \
                    if (BVAL (G) & AVAL (G))            \
                      SET_DECL_STATE (lval, X);         \
                    else if (DECL_STATE (lval) != Z) {  \
                      if (BVAL (G))                     \
                        SET_DECL_STATE (lval, Z);       \
                      else if (AVAL (G))                \
                        SET_DECL_STATE (lval, ONE);     \
                    }                                   \
                  }                                     \
                }

#define TEST(EXPR, G) eval (EXPR); DETERMINE_TRUE_COND (G)

int part_lref(Group * g1, Group * g2, struct part_info *info);
ngroups_t part_rref(Group * g1, Group * g2, struct part_info *info);
Group *eval_(tree * pc, nbits_t * nbits);
void eval_1(tree decl);
void eval(tree * pc);
delay_t eval_delay(tree delay_node, enum logical_value state);
void do_net_eval(tree node, int exclude_inout, tree exclude_tree);
int R_alloc(int number, int area);
nbits_t eval_range(tree * inst, char *ident);
Bit eval_const(tree * inst);
void eval_nbits(tree, nbits_t);
enum logical_value test(tree * expr);
void init_eval();
double reg_to_real(Group * g, nbits_t nbits);
void real_to_reg(double x, Group * g, ngroups_t ngroups);

#endif				// EVAL_H
