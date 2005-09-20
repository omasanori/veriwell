#ifndef BISON_SDFCPARS_H
# define BISON_SDFCPARS_H

#ifndef YYSTYPE
typedef union {
	int				integer;
	double			floating;
	char			*ptr;
	eScaleType_t	etype;
	eDelayType_t  	dtype;
	eSDFConstruct_t	stype;
	eVerilogConstruct_t vtype;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	IF	257
# define	ELSE	258
# define	OROR	259
# define	ANDAND	260
# define	ANDANDAND	261
# define	XNOR	262
# define	LOGICAL_EQUALITY	263
# define	LOGICAL_INEQUALITY	264
# define	CASE_EQUALITY	265
# define	CASE_INEQUALITY	266
# define	LT	267
# define	LE	268
# define	GT	269
# define	GE	270
# define	LEFT_SHIFT	271
# define	RIGHT_SHIFT	272
# define	REDUCTION_NOR	273
# define	REDUCTION_NAND	274
# define	UNARY	275
# define	INTEGER	276
# define	SCALAR_CONSTANT	277
# define	FLOATING	278
# define	QSTRING	279
# define	IDENTIFIER	280
# define	IOPATH	281
# define	PORT	282
# define	INTERCONNECT	283
# define	NETDELAY	284
# define	DEVICE	285
# define	SETUP	286
# define	HOLD	287
# define	SETUPHOLD	288
# define	RECOVERY	289
# define	SKEW	290
# define	WIDTH	291
# define	PERIOD	292
# define	NOCHANGE	293
# define	PATHPULSE	294
# define	IGNORE	295
# define	INTERMOD_PATH	296
# define	MIPD	297
# define	CELL	298
# define	USE	299
# define	INTERCONNECT_MIPD	300
# define	MINIMUM	301
# define	MAXIMUM	302
# define	AVERAGE	303
# define	MTM	304
# define	TOOL_CONTROL	305
# define	TYPICAL	306
# define	SCALE_FACTORS	307
# define	SCALE_TYPE	308
# define	FROM_MINIMUM	309
# define	FROM_TYPICAL	310
# define	FROM_MAXIMUM	311
# define	FROM_MTM	312
# define	FROM_FILE	313
# define	TURNOFF_DELAY	314
# define	TURNON_DELAY	315
# define	MODULE	316
# define	MAP_INNER	317
# define	ADD	318
# define	OVERRIDE	319
# define	EOF_	320


#endif /* not BISON_SDFCPARS_H */
