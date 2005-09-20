#ifndef BISON_SDFPARSE_H
# define BISON_SDFPARSE_H

#ifndef YYSTYPE
typedef union {
	int				integer;
	double			floating;
	char			*ptr;
	eScaleType_t	etype;
	eDelayType_t  	dtype;
	eSDFConstruct_t	stype;
	eVerilogConstruct_t vtype;
	char			character;
	creal_t			creal;
	triple_t		triple;
	elist_t			elist;
	port_t			port;
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
# define	BLOB	281
# define	POSEDGE	282
# define	NEGEDGE	283
# define	DELAYFILE	284
# define	SDFVERSION	285
# define	DESIGN	286
# define	DATE	287
# define	VENDOR	288
# define	PROGRAM	289
# define	VERSION_	290
# define	DIVIDER	291
# define	VOLTAGE	292
# define	PROCESS	293
# define	TEMPERATURE	294
# define	TIMESCALE	295
# define	CELL	296
# define	CELLTYPE	297
# define	INSTANCE	298
# define	DELAY	299
# define	TIMINGCHECK	300
# define	INCLUDE	301
# define	ABSOLUTE	302
# define	INCREMENT	303
# define	USERDEF	304
# define	PATHPULSE	305
# define	GLOBALPATHPULSE	306
# define	IOPATH	307
# define	COND	308
# define	PORT	309
# define	INTERCONNECT	310
# define	NETDELAY	311
# define	DEVICE	312
# define	SETUP	313
# define	HOLD	314
# define	SETUPHOLD	315
# define	RECOVERY	316
# define	SKEW	317
# define	WIDTH	318
# define	PERIOD	319
# define	NOCHANGE	320
# define	PATHCONSTRAINT	321
# define	SUM	322
# define	DIFF	323
# define	SKEWCONSTRAINT	324
# define	MICROSECOND	325
# define	NANOSECOND	326
# define	PICOSECOND	327
# define	EDGE_01	328
# define	EDGE_10	329
# define	EDGE_0z	330
# define	EDGE_1z	331
# define	EDGE_z0	332
# define	EDGE_z1	333
# define	EOF_	334


#endif /* not BISON_SDFPARSE_H */
