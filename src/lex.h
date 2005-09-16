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

#ifndef LEX_H
#define LEX_H
// public definitions
void fix_lastchar(void);
void init_lex(void);
void reinit_lex(void);
void eatnum(void);
int read_num_token(char *);
void parse_base_const1(enum radii, char *, nbits_t, nbits_t, tree);
void parse_base_const1g(enum radii, char *, nbits_t, nbits_t, Group *);
int skip_white_space(int);
void yyerror(char *);
int prog_parse(void);		// this really belongs with the parser
int IsReservedWord(char *, int);
int convert_edge_digit(int c);
void init_parse(void);
void init_lex_1(void);
#ifdef LEX_C			// private definitions
static unsigned int hash(register const char *str,
			 register unsigned int len);
#endif				// LEX_C

#endif				// LEX_H
