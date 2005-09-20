#ifndef BISON_PARSE_H
# define BISON_PARSE_H

#ifndef YYSTYPE
typedef union {
	tree	ttype;	/* pointer to a tree node */
	int	integer;	/* used for integers */
//	inst	ctype;	/* pointer to an instruction node (code) */
//	Datum	*dtype; /* pointer to a bit string for returning NUMBERs */
	lineno_t ltype;	/* for passing line numbers */
	enum tree_code	code;	/* for operators */
	enum tree_type	gtype;	/* for gates */
	enum tree_type	ntype;	/* for nets (same as gtype, buy more readable) */
	char	double_digit[2]; /* structure for parsing edges */
	char	c;		/* char used in udp parsing */
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	LOWEST	257
# define	IF	258
# define	ELSE	259
# define	OROR	260
# define	ANDAND	261
# define	ANDANDAND	262
# define	XNOR	263
# define	LOGICAL_EQUALITY	264
# define	LOGICAL_INEQUALITY	265
# define	CASE_EQUALITY	266
# define	CASE_INEQUALITY	267
# define	LT	268
# define	LE	269
# define	GT	270
# define	GE	271
# define	LEFT_SHIFT	272
# define	RIGHT_SHIFT	273
# define	REDUCTION_NOR	274
# define	REDUCTION_NAND	275
# define	UNARY	276
# define	IDENTIFIER	277
# define	HIERARCHY_IDENTIFIER	278
# define	STARARROW	279
# define	EQUALARROW	280
# define	SPECPARAM	281
# define	SETUP	282
# define	HOLD	283
# define	PERIOD	284
# define	WIDTH	285
# define	SKEW	286
# define	RECOVERY	287
# define	SETUPHOLD	288
# define	NOCHANGE	289
# define	MODULE	290
# define	MACROMODULE	291
# define	ENDMODULE	292
# define	SPECIFY	293
# define	ENDSPECIFY	294
# define	PRIMITIVE	295
# define	ENDPRIMITIVE	296
# define	TABLE	297
# define	ENDTABLE	298
# define	TASK	299
# define	ENDTASK	300
# define	FUNCTION	301
# define	ENDFUNCTION	302
# define	PARAMETER	303
# define	INPUT	304
# define	OUTPUT	305
# define	INOUT	306
# define	NETTYPE	307
# define	TRIREG	308
# define	SCALARED	309
# define	VECTORED	310
# define	REG	311
# define	TIME	312
# define	INTEGER	313
# define	REAL	314
# define	EVENT	315
# define	ASSIGN	316
# define	DEFPARAM	317
# define	STRENGTH0	318
# define	STRENGTH1	319
# define	SUPPLY0	320
# define	SUPPLY1	321
# define	LOGIC	322
# define	BUFFER	323
# define	BUFIF	324
# define	CMOS	325
# define	PULL	326
# define	TRAN	327
# define	INITIAL	328
# define	ALWAYS	329
# define	CASE	330
# define	CASEZ	331
# define	CASEX	332
# define	ENDCASE	333
# define	FOREVER	334
# define	REPEAT	335
# define	WHILE	336
# define	FOR	337
# define	WAIT_	338
# define	ARROW_	339
# define	DISABLE	340
# define	DEASSIGN	341
# define	FORCE	342
# define	RELEASE	343
# define	DEFAULT	344
# define	BEGIN	345
# define	END	346
# define	FORK	347
# define	JOIN	348
# define	SYSTEM_IDENTIFIER	349
# define	STRING	350
# define	NUMBER	351
# define	BASE_NUMBER	352
# define	POSEDGE	353
# define	NEGEDGE	354
# define	OR	355
# define	SMALL	356
# define	MEDIUM	357
# define	LARGE	358
# define	EDGE	359
# define	DOUBLE_DIGIT	360
# define	UDP_CHAR	361
# define	UDP_ONE	362
# define	UDP_ZERO	363
# define	UDP_X	364
# define	UDP_B	365
# define	UDP_HOOK	366
# define	EOF_	367


#endif /* not BISON_PARSE_H */
