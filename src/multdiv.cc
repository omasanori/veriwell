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

/* MULTDIV.C - Arbitrary-length multiply, divide, and modulo arithmetic
*/

#include <stdio.h>
#include "vtypes.h"
#include "tree.h"
#include "multdiv.h"
#include "veriwell.h"


static unsigned NZGroups(Group * ag, unsigned groups);
static unsigned NZBits(Bit a);
static Bit GroupRShift(Group * a, Bit * b, unsigned c, unsigned groups);
static Bit GroupLShift(Bit * a, Group * bg, unsigned c, unsigned groups);
static int BitCmp(Bit * a, Bit * b, unsigned bits);
static void Mult32x32(Bit a[2], Bit b, Bit c);
static void AllocAccs(unsigned words);

static Bit *cc = 0;
static Bit *dd = 0;
static ngroups_t cc_size = 0;
static ngroups_t dd_size = 0;

/* 
 * a=b+c. Returns carry. 
 */
Bit BitAdd(Bit * a, Bit * b, Bit * c, unsigned bits)
{
    Bit acc;
    Bit carry;

    carry = 0;
    for (int i = 0; i < bits; i++) {
	if ((acc = b[i] + carry) < carry) {
	    acc = c[i];
	} else if ((acc += c[i]) < c[i]) {
	    carry = 1;
	} else {
	    carry = 0;
	}
	a[i] = acc;
    }
    return (carry);
}


/* 
 * a=b-c. Returns borrow
 */
Bit BitSub(Bit * a, Bit * b, Bit * c, unsigned bits)
{
    Bit acc;
    Bit borrow;

    borrow = 0;
    for (int i = 0; i < bits; i++) {
	if ((acc = b[i] - borrow) > (~((Bit) 0) - borrow)) {
	    acc = (~(Bit) 0) - c[i];
	} else if ((acc -= c[i]) > ((~(Bit) 0) - c[i])) {
	    borrow = 1;
	} else {
	    borrow = 0;
	}
	a[i] = acc;
    }
    return (borrow);
}


/* 
 * Returns the number of non-zero groups.
 */
static unsigned NZGroups(Group * ag, unsigned groups)
{
    int i;
    for (i = groups - 1; i >= 0; i--) {
	if (ag[i].bit.aval) {
	    break;
	}
    }
    return (i + 1);
}


/*
 * return the number of significant bits in word
 */
static unsigned NZBits(Bit a)
{
    unsigned i;
    for (i = 0; i < sizeof(Bit) * 8; i++, a >>= 1) {
	if (a == 0) {
	    break;
	}
    }
    return (i);
}

/*
 * a = b >> c
 *  where c is less than number of bits in 'Bit'
 */
static Bit GroupRShift(Group * a, Bit * b, unsigned c, unsigned groups)
{
    Bit bi;
    Bit carry;
    int i;
    unsigned int t;

    if (c >= sizeof(Bit) * 8) {
	return 0;
    }
    t = sizeof(Bit) * 8 - c;
    carry = 0;
    for (i = groups - 1; i >= 0; i--) {
	bi = b[i];
	a[i].bit.aval = (bi >> c) | carry;
	carry = c ? (bi << t) : 0;
    }
    return (carry);
}

/* 
 * a = b << c;
 *  where c is less than number of bits in 'Bit'
 */
static Bit GroupLShift(Bit * a, Group * bg, unsigned c, unsigned groups)
{
    Bit bi;
    Bit carry;
    unsigned int t;

    if (c >= sizeof(Bit) * 8) {
	return (0);
    }
    t = sizeof(Bit) * 8 - c;
    carry = 0;
    for (int i = 0; i < groups; i++) {
	bi = bg[i].bit.aval;
	a[i] = (bi << c) | carry;
	carry = c ? (bi >> t) : 0;
    }
    return (carry);
}

/* 
 * compare a to b. returns:
 * -1 if a < b,
 *  0 if a == b
 *  1 if a > b
 */
static int BitCmp(Bit * a, Bit * b, unsigned bits)
{
    for (int i = bits - 1; i >= 0; i--) {
	if (a[i] > b[i]) {
	    return (1);
	}
	if (a[i] < b[i]) {
	    return (-1);
	}
    }
    return (0);
}

/*
 * 32 bit by 32 bit multiply
 */
static void Mult32x32(Bit a[2], Bit b, Bit c)
{
    Bit t;
    Bit u;
    Bit bHigh = b >> 16;
    Bit bLow = b & 0xffff;
    Bit cHigh = c >> 16;
    Bit cLow = c & 0xffff;

    a[0] = bLow * cLow;
    t = bLow * cHigh;
    u = bHigh * cLow;
    a[1] = bHigh * cHigh;

    if ((t += u) < u) {
	a[1] += 0x10000;
    }
    u = t << 16;

    if ((a[0] += u) < u) {
	a[1]++;
    }
    a[1] += t >> 16;
}



/*  
 * a=b+c*d, where c is a single word
 * return carry word
 */
static Bit Mult32Acc(Bit * a, Bit * b, Bit c, Group * dg, unsigned words)
{
    Bit carry;
    Bit t[2];
    unsigned i;

    carry = 0;
    for (i = 0; i < words; i++) {
	Mult32x32(t, c, dg[i].bit.aval);
	if ((a[i] = b[i] + carry) < carry) {
	    carry = 1;
	} else {
	    carry = 0;
	}
	if ((a[i] += t[0]) < t[0]) {
	    carry++;
	}
	carry += t[1];
    }
    return (carry);
}


/* 
 * a=b*c
 */
void GroupMult(Group * ag, Group * bg, Group * cg, unsigned groups)
{
    unsigned bGroups;
    unsigned cGroups;

    AllocAccs(groups);
    /*
     * set cc to 0
     */
    for (int i = 0; i < 2 * groups; i++) {
	cc[i] = 0;
    }

    /*
     * calculate number if significant words in each group
     */
    bGroups = NZGroups(bg, groups);
    cGroups = NZGroups(cg, groups);

    /*
     * multiply and accumulate a group at a time
     */
    for (int i = 0; i < bGroups; i++) {
	cc[i + cGroups] += Mult32Acc(&cc[i], &cc[i],
				     bg[i].bit.aval, cg, cGroups);
    }
    /*
     * copy result to destination
     */
    for (int i = 0; i < groups; i++) {
	ag[i].bit.aval = cc[i];
	ag[i].bit.bval = 0;
    }
}

/* 
 * a=b-c*d, where c is a single word. 
 *          returns borrow
 */
Bit Mult32Sub(Bit * a, Bit * b, Bit c, Bit * d, unsigned words)
{
    Bit borrow;
    Bit t[2];

    borrow = 0;
    for (int i = 0; i < words; i++) {
	Mult32x32(t, c, d[i]);
	if ((a[i] = b[i] - borrow) > ((~(Bit) 0) - borrow)) {
	    borrow = 1;
	} else {
	    borrow = 0;
	}
	if ((a[i] -= t[0]) > (~((Bit) 0) - t[0])) {
	    borrow++;
	}
	borrow += t[1];
    }
    return (borrow);
}


/* 
 * a=b/c, a and c are single words, b is a double word. 
 *	assumptions: b[1] < c and c>>16 > 0.
 */
void Div64by32(Bit * a, Bit b[2], Bit c)
{
    Bit t[2];
    Bit u;
    Bit v;
    Bit aHigh;
    Bit aLow;
    Bit cHigh = c >> 16;
    Bit cLow = c & 0xffff;

    t[0] = b[0];
    t[1] = b[1];

    /* 
     * Underestimate high half of quotient and 
     * subtract product of estimate and divisor 
     * from dividend. 
     */
    if (cHigh == 0xffff) {
	aHigh = t[1] >> 16;
    } else {
	aHigh = (t[1] / (cHigh + 1)) & 0xffff;
    }
    u = aHigh * cLow;
    v = aHigh * cHigh;
    if ((t[0] -= (u << 16)) > ((~(Bit) 0) - (u << 16))) {
	t[1]--;
    }
    t[1] -= u >> 16;
    t[1] -= v;

    /* Correct estimate. */
    while ((t[1] > cHigh) || ((t[1] == cHigh) && (t[0] >= (cLow << 16)))) {
	if ((t[0] -= (cLow << 16)) > (~(Bit) 0) - (cLow << 16)) {
	    t[1]--;
	}
	t[1] -= cHigh;
	aHigh++;
    }
    /* 
     * Underestimate low half of quotient and 
     * subtract product of estimate and divisor 
     * from what remains of dividend. 
     */
    if (cHigh == 0xffff) {
	aLow = t[1] >> 16;
    } else {
	aLow = (((t[1] << 16) + (t[0] >> 16)) / (cHigh + 1)) & 0xffff;
    }
    u = aLow * cLow;
    v = aLow * cHigh;
    if ((t[0] -= u) > ((~(Bit) 0) - u)) {
	t[1]--;
    }
    if ((t[0] -= (v << 16)) > ((~(Bit) 0) - (v << 16))) {
	t[1]--;
    }
    t[1] -= v >> 16;

    /* Correct estimate. */
    while ((t[1] > 0) || ((t[1] == 0) && t[0] >= c)) {
	if ((t[0] -= c) > ((~(Bit) 0) - c)) {
	    t[1]--;
	}
	aLow++;
    }

    *a = (aHigh << 16) + aLow;
}


/*  
 * a=c/d and b=c%d. 
 *	d must be > 0
 */
void
GroupDiv(Group * ag, Group * bg, Group * cg, Group * dg,
	 unsigned words, int do_mod)
{
    Bit ai;
    Bit t;
    unsigned dWords;
    unsigned shift;

    /*
     * calculate number of significant words in divisor
     */
    dWords = NZGroups(dg, words);
    if (dWords == 0) {
	return;
    }

    AllocAccs(words);

    /* Normalize operands. */
    shift = sizeof(Bit) * 8 - NZBits(dg[dWords - 1].bit.aval);
    /*
     * set cc to 0
     */
    for (int i = 0; i < dWords; i++) {
	cc[i] = 0;
    }
    cc[words] = GroupLShift(cc, cg, shift, words);
    GroupLShift(dd, dg, shift, dWords);
    t = dd[dWords - 1];

    /* 
     * set ag to 0
     */
    for (int i = 0; i < words; i++) {
	ag[i].bit.aval = 0;
	ag[i].bit.bval = 0;
    }

    for (int i = words - dWords; i >= 0; i--) {
	/* Underestimate quotient digit and subtract. */
	if (t == (~(Bit) 0)) {
	    ai = cc[i + dWords];
	} else {
	    Div64by32(&ai, &cc[i + dWords - 1], t + 1);
	}
	cc[i + dWords] -= Mult32Sub(&cc[i], &cc[i], ai, dd, dWords);
	/* Correct estimate. */
	while (cc[i + dWords] || (BitCmp(&cc[i], dd, dWords) >= 0)) {
	    ai++;
	    cc[i + dWords] -= BitSub(&cc[i], &cc[i], dd, dWords);
	}
	ag[i].bit.aval = ai;
    }
    /* Restore result. */
    if (do_mod) {
	/*
	 * zero out bg
	 */
	for (int i = 0; i < words; i++) {
	    bg[i].bit.aval = 0;
	    bg[i].bit.bval = 0;
	}
	GroupRShift(bg, cc, shift, dWords);
    }
}

static void AllocAccs(unsigned words)
{
    /*
     * used shared accumulators when calculating results
     */
    if (!cc || cc_size <= words) {
	if (cc) {
	    cc = (Bit *) xrealloc((char *) cc,
				  (2 * (words + 1) + 1) * sizeof(Bit));
	} else {
	    cc = (Bit *) xmalloc((2 * (words + 1) + 1) * sizeof(Bit));
	}
	cc_size = words + 1;
    }
    if (!dd || dd_size <= words) {
	if (dd) {
	    dd = (Bit *) xrealloc((char *) dd, (words + 1) * sizeof(Bit));
	} else {
	    dd = (Bit *) xmalloc((words + 1) * sizeof(Bit));
	}
	dd_size = words + 1;
    }
}

void init_multdiv()
{
    cc = 0;
    dd = 0;
    cc_size = 0;
    dd_size = 0;
}
