if BUILD_LIBZ
else
   ZLIB=-lz
endif
if BUILD_LIBBZ2
else
   BZ2LIB=-lbz2
endif
if USE_LXT
   LIBLXT = $(top_builddir)/lxt/liblxt.la
endif
if USE_LXT2
   LIBLXT = $(top_builddir)/lxt/liblxt.la
endif
nobase_include_HEADERS = veriuser.h acc_user.h veriuser.c

bin_PROGRAMS = veriwell
veriwell_SOURCES = defaultveriuser.cc
veriwell_LDADD =  $(vl_cv_lib_readline) libveriwell.la $(ZLIB) $(BZ2LIB)

AM_GPEFLAGS= -L C++ -a -p -j1 -i 1 -g -o -t -N is_reserved_word 
EXTRA_DIST = v.h
GPE = gperf
.gpe.h:
	$(GPE) $(GPEFLAGS) $(AM_GPEFLAGS) $< > $@


lib_LTLIBRARIES = libveriwell.la 
libveriwell_la_SOURCES = parse.yy sdfparse.yy sdfcpars.yy multdiv.cc \
		   print.cc usertask.cc flags.cc store.cc \
		   nsched.cc verisys.cc check.cc gates.cc schedule.cc \
		   strobe.cc glue.cc obstack.cc scope.cc timescal.cc \
		   systask.cc copy.cc veriwell.cc pass2.cc sdf.cc \
		   sdfclex.cc decl.cc io.cc pass3.cc trace.cc dumpvar.cc \
		   lex.cc pli.cc tree.cc sdflex.cc eval.cc macro.cc \
		   udp.cc exec.cc  specify.cc v.gpe plihacks.cc \
		   random.cc \
		   acc_user.h check.h copy.h decl.h dumpvar.h eval.h \
		   exec.h flags.h gates.h glue.h io.h lex.h macro.h \
		   multdiv.h nsched.h obstack.h parse.h pass2.h pass3.h \
		   pli.h plihacks.h print.h runtime.h schedule.h \
		   scope.h sdfclex.h sdfcpars.h \
		   sdf.h sdflex.h sdfparse.h specify.h store.h strobe.h \
		   systask.h timescal.h trace.h tree.h udp.h usertask.h \
		   verisys.h veriuser.h veriwell.h v.h vtypes.h tree.def \
		   random.h
libveriwell_la_LDFLAGS = -version-info 0:0:0
libveriwell_la_LIBADD =  $(LIBLXT) $(top_builddir)/replace/libreplace.la

AM_YFLAGS = -y -d

BUILT_SOURCES = build.h
CLEANFILES    = build.h
build.h: Makefile
	echo '#define BINDIR "$(bindir)"' >$@
	echo '#define LIBDIR "$(libdir)"' >>$@
	echo '#define INCLUDEDIR "$(includedir)"' >>$@
	echo '#define CFLAGS "$(CFLAGS)"' >>$@
	echo '#define LDFLAGS "$(LDFLAGS)"' >>$@
	echo '#define LIBS "$(LIBS) $(ZLIB) $(BZ2LIB)"' >>$@


MAINTAINERCLEANFILES = Makefile.in 
