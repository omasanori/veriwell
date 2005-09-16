/* C++ code produced by gperf version 3.0.1 */
/* Command-line: gperf -L C++ -a -p -j1 -i 1 -g -o -t -N is_reserved_word v.gpe  */
/* Computed positions: -k'1-3,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 19 "v.gpe"
struct keywords {
    char *name;
    short kval;
    short type;
};

#define TOTAL_KEYWORDS 107
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 170
/* maximum key range = 164, duplicates = 0 */

class Perfect_Hash {
  private:
    static inline unsigned int hash(const char *str, unsigned int len);
  public:
    static struct keywords *is_reserved_word(const char *str,
					     unsigned int len);
};

inline unsigned int
 Perfect_Hash::hash(register const char *str, register unsigned int len)
{
    static unsigned char asso_values[] = {
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 82, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 65, 57,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 37, 9, 44,
	1, 1, 36, 77, 14, 8, 5, 21, 47, 9,
	1, 17, 20, 171, 1, 11, 2, 5, 11, 45,
	18, 24, 2, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
	171, 171, 171, 171, 171, 171
    };
    register int hval = len;

    switch (hval) {
    default:
	hval += asso_values[(unsigned char) str[2]];
     /*FALLTHROUGH*/ case 2:
	hval += asso_values[(unsigned char) str[1]];
     /*FALLTHROUGH*/ case 1:
	hval += asso_values[(unsigned char) str[0]];
	break;
    }
    return hval + asso_values[(unsigned char) str[len - 1]];
}

struct keywords *Perfect_Hash::is_reserved_word(register const char *str,
						register unsigned int len)
{
    static struct keywords wordlist[] = {
	{""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 68 "v.gpe"
	{"end", END, NO_TYPE},
	{""}, {""},
#line 100 "v.gpe"
	{"rtran", TRAN, GATE_RTRAN_TYPE},
#line 56 "v.gpe"
	{"endcase", ENDCASE, NO_TYPE},
#line 32 "v.gpe"
	{"endtable", ENDTABLE, NO_TYPE},
#line 25 "v.gpe"
	{"endmodule", ENDMODULE, NO_TYPE},
	{""},
#line 36 "v.gpe"
	{"endfunction", ENDFUNCTION, NO_TYPE},
#line 29 "v.gpe"
	{"endprimitive", ENDPRIMITIVE, NO_TYPE},
#line 110 "v.gpe"
	{"trior", NETTYPE, NET_TRIOR_TYPE},
#line 108 "v.gpe"
	{"triand", NETTYPE, NET_TRIAND_TYPE},
#line 45 "v.gpe"
	{"integer", INTEGER, NO_TYPE},
#line 47 "v.gpe"
	{"event", EVENT, NO_TYPE},
#line 73 "v.gpe"
	{"or", OR, NO_TYPE},
#line 106 "v.gpe"
	{"tri", NETTYPE, NET_TRI_TYPE},
#line 79 "v.gpe"
	{"nor", LOGIC, GATE_NOR_TYPE},
#line 44 "v.gpe"
	{"time", TIME, NO_TYPE},
#line 84 "v.gpe"
	{"not", BUFFER, GATE_NOT_TYPE},
#line 75 "v.gpe"
	{"medium", MEDIUM, NO_TYPE},
#line 91 "v.gpe"
	{"rnmos", BUFIF, GATE_RNMOS_TYPE},
#line 62 "v.gpe"
	{"disable", DISABLE, NO_TYPE},
	{""},
#line 58 "v.gpe"
	{"repeat", REPEAT, NO_TYPE},
#line 34 "v.gpe"
	{"endtask", ENDTASK, NO_TYPE},
#line 39 "v.gpe"
	{"output", OUTPUT, NO_TYPE},
#line 40 "v.gpe"
	{"inout", INOUT, NO_TYPE},
#line 23 "v.gpe"
	{"module", MODULE, NO_TYPE},
#line 70 "v.gpe"
	{"join", JOIN, NO_TYPE},
#line 38 "v.gpe"
	{"input", INPUT, NO_TYPE},
#line 27 "v.gpe"
	{"endspecify", ENDSPECIFY, NO_TYPE},
	{""},
#line 28 "v.gpe"
	{"primitive", PRIMITIVE, NO_TYPE},
#line 81 "v.gpe"
	{"xor", LOGIC, GATE_XOR_TYPE},
#line 82 "v.gpe"
	{"xnor", LOGIC, GATE_XNOR_TYPE},
#line 89 "v.gpe"
	{"nmos", BUFIF, GATE_NMOS_TYPE},
#line 77 "v.gpe"
	{"and", LOGIC, GATE_AND_TYPE},
#line 78 "v.gpe"
	{"nand", LOGIC, GATE_NAND_TYPE},
#line 97 "v.gpe"
	{"tran", TRAN, GATE_TRAN_TYPE},
#line 92 "v.gpe"
	{"rpmos", BUFIF, GATE_RPMOS_TYPE},
#line 66 "v.gpe"
	{"default", DEFAULT, NO_TYPE},
#line 63 "v.gpe"
	{"deassign", DEASSIGN, NO_TYPE},
	{""},
#line 128 "v.gpe"
	{"specparam", SPECPARAM, NO_TYPE},
#line 35 "v.gpe"
	{"function", FUNCTION, NO_TYPE},
	{""}, {""},
#line 31 "v.gpe"
	{"table", TABLE, NO_TYPE},
#line 48 "v.gpe"
	{"defparam", DEFPARAM, NO_TYPE},
#line 71 "v.gpe"
	{"posedge", POSEDGE, NO_TYPE},
#line 65 "v.gpe"
	{"release", RELEASE, NO_TYPE},
#line 60 "v.gpe"
	{"for", FOR, NO_TYPE},
#line 105 "v.gpe"
	{"wire", NETTYPE, NET_WIRE_TYPE},
#line 64 "v.gpe"
	{"force", FORCE, NO_TYPE},
#line 90 "v.gpe"
	{"pmos", BUFIF, GATE_PMOS_TYPE},
#line 57 "v.gpe"
	{"forever", FOREVER, NO_TYPE},
#line 26 "v.gpe"
	{"specify", SPECIFY, NO_TYPE},
#line 52 "v.gpe"
	{"else", ELSE, NO_TYPE},
#line 42 "v.gpe"
	{"vectored", VECTORED, NO_TYPE},
#line 30 "v.gpe"
	{"assign", ASSIGN, NO_TYPE},
#line 109 "v.gpe"
	{"wor", NETTYPE, NET_WOR_TYPE},
#line 37 "v.gpe"
	{"parameter", PARAMETER, NO_TYPE},
#line 102 "v.gpe"
	{"rtranif1", BUFIF, GATE_RTRANIF1_TYPE},
#line 94 "v.gpe"
	{"rcmos", CMOS, GATE_RCMOS_TYPE},
#line 49 "v.gpe"
	{"initial", INITIAL, NO_TYPE},
#line 111 "v.gpe"
	{"tri1", NETTYPE, NET_TRI1_TYPE},
#line 59 "v.gpe"
	{"while", WHILE, NO_TYPE},
	{""},
#line 33 "v.gpe"
	{"task", TASK, NO_TYPE},
	{""},
#line 101 "v.gpe"
	{"rtranif0", BUFIF, GATE_RTRANIF0_TYPE},
#line 124 "v.gpe"
	{"strong1", STRENGTH1, STRENGTH_STRONG1_TYPE},
#line 69 "v.gpe"
	{"fork", FORK, NO_TYPE},
#line 112 "v.gpe"
	{"tri0", NETTYPE, NET_TRI0_TYPE},
#line 96 "v.gpe"
	{"pulldown", PULL, GATE_PULLDN_TYPE},
#line 51 "v.gpe"
	{"if", IF, NO_TYPE},
#line 88 "v.gpe"
	{"notif1", BUFIF, GATE_NOTIF1_TYPE},
#line 129 "v.gpe"
	{"edge", EDGE, NO_TYPE},
#line 93 "v.gpe"
	{"cmos", CMOS, GATE_CMOS_TYPE},
#line 119 "v.gpe"
	{"strong0", STRENGTH0, STRENGTH_STRONG0_TYPE},
#line 72 "v.gpe"
	{"negedge", NEGEDGE, NO_TYPE},
#line 107 "v.gpe"
	{"wand", NETTYPE, NET_WAND_TYPE},
#line 83 "v.gpe"
	{"buf", BUFFER, GATE_BUF_TYPE},
#line 46 "v.gpe"
	{"real", REAL, NO_TYPE},
#line 87 "v.gpe"
	{"notif0", BUFIF, GATE_NOTIF0_TYPE},
	{""},
#line 67 "v.gpe"
	{"begin", BEGIN, NO_TYPE},
#line 115 "v.gpe"
	{"trireg", TRIREG, NET_TRIREG_TYPE},
	{""},
#line 61 "v.gpe"
	{"wait", WAIT_, NO_TYPE},
#line 53 "v.gpe"
	{"case", CASE, NO_TYPE},
#line 95 "v.gpe"
	{"pullup", PULL, GATE_PULLUP_TYPE},
#line 54 "v.gpe"
	{"casez", CASEZ, NO_TYPE},
#line 113 "v.gpe"
	{"supply1", SUPPLY1, NET_SUPPLY1_TYPE},
#line 41 "v.gpe"
	{"scalared", SCALARED, NO_TYPE},
#line 24 "v.gpe"
	{"macromodule", MACROMODULE, NO_TYPE},
	{""},
#line 99 "v.gpe"
	{"tranif1", BUFIF, GATE_TRANIF1_TYPE},
#line 136 "v.gpe"
	{"$setuphold", SETUPHOLD, NO_TYPE},
	{""}, {""},
#line 114 "v.gpe"
	{"supply0", SUPPLY0, NET_SUPPLY0_TYPE},
#line 74 "v.gpe"
	{"small", SMALL, NO_TYPE},
#line 137 "v.gpe"
	{"$nochange", NOCHANGE, NO_TYPE},
#line 132 "v.gpe"
	{"$period", PERIOD, NO_TYPE},
#line 98 "v.gpe"
	{"tranif0", BUFIF, GATE_TRANIF0_TYPE},
#line 86 "v.gpe"
	{"bufif1", BUFIF, GATE_BUFIF1_TYPE},
	{""},
#line 55 "v.gpe"
	{"casex", CASEX, NO_TYPE},
	{""},
#line 135 "v.gpe"
	{"$recovery", RECOVERY, NO_TYPE},
	{""},
#line 131 "v.gpe"
	{"$hold", HOLD, NO_TYPE},
#line 130 "v.gpe"
	{"$setup", SETUP, NO_TYPE},
#line 85 "v.gpe"
	{"bufif0", BUFIF, GATE_BUFIF0_TYPE},
	{""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
	{""}, {""}, {""},
#line 125 "v.gpe"
	{"pull1", STRENGTH1, STRENGTH_PULL1_TYPE},
	{""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 120 "v.gpe"
	{"pull0", STRENGTH0, STRENGTH_PULL0_TYPE},
	{""}, {""},
#line 126 "v.gpe"
	{"weak1", STRENGTH1, STRENGTH_WEAK1_TYPE},
#line 50 "v.gpe"
	{"always", ALWAYS, NO_TYPE},
	{""}, {""}, {""}, {""}, {""}, {""},
#line 121 "v.gpe"
	{"weak0", STRENGTH0, STRENGTH_WEAK0_TYPE},
	{""},
#line 133 "v.gpe"
	{"$width", WIDTH, NO_TYPE},
	{""}, {""}, {""},
#line 43 "v.gpe"
	{"reg", REG, NO_TYPE},
	{""}, {""},
#line 127 "v.gpe"
	{"highz1", STRENGTH1, STRENGTH_HIGHZ1_TYPE},
	{""},
#line 134 "v.gpe"
	{"$skew", SKEW, NO_TYPE},
	{""}, {""}, {""}, {""}, {""},
#line 122 "v.gpe"
	{"highz0", STRENGTH0, STRENGTH_HIGHZ0_TYPE}
    };

    if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH) {
	register int key = hash(str, len);

	if (key <= MAX_HASH_VALUE && key >= 0) {
	    register const char *s = wordlist[key].name;

	    if (*str == *s && !strcmp(str + 1, s + 1))
		return &wordlist[key];
	}
    }
    return 0;
}
