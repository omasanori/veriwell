if USE_LXT
   LXTLIB=../lxt/liblxt.la
   ZLIB=-lz
endif
libveriwell_la_CPPFLAGS = -DBINDIR=\"$(bindir)\" \
			 -DLIBDIR=\"$(libdir)\" \
			 -DINCLUDEDIR=\"$(includedir)\" \
			 -DCFLAGS="\"$(CFLAGS)\"" \
			 -DLDFLAGS="\"$(LDFLAGS)\"" \
			 -DLIBS="\"$(LIBS) $(ZLIB)\""
nobase_include_HEADERS = veriuser.h acc_user.h veriuser.c

bin_PROGRAMS = veriwell
veriwell_SOURCES = defaultveriuser.cc
veriwell_LDADD =  $(vl_cv_lib_readline) libveriwell.la $(ZLIB)

AM_GPEFLAGS= -L C++ -a -p -j1 -i 1 -g -o -t -N is_reserved_word 
EXTRA_DIST = v.h
GPE = gperf
.gpe.h:
	$(GPE) $(GPEFLAGS) $(AM_GPEFLAGS) $< > $@


lib_LTLIBRARIES = libveriwell.la 
libveriwell_la_SOURCES = parse.yy multdiv.cc print.cc usertask.cc flags.cc store.cc \
		   nsched.cc verisys.cc check.cc gates.cc schedule.cc \
		   strobe.cc glue.cc obstack.cc scope.cc \
		   systask.cc copy.cc veriwell.cc pass2.cc sdf.cc timescal.cc \
		   sdfclex.cc decl.cc io.cc pass3.cc trace.cc dumpvar.cc \
		   lex.cc pli.cc tree.cc sdflex.cc eval.cc macro.cc plihacks.cc \
		   udp.cc exec.cc  specify.cc  sdfcpars.yy sdfparse.yy v.gpe \
		   acc_user.h check.h copy.h decl.h dumpvar.h eval.h \
		   exec.h flags.h gates.h glue.h io.h lex.h macro.h multdiv.h \
		   nsched.h obstack.h parse.h pass2.h pass3.h pli.h plihacks.h \
		   print.h runtime.h schedule.h scope.h sdfclex.h sdfcpars.h \
		   sdf.h sdflex.h sdfparse.h specify.h store.h strobe.h \
		   systask.h timescal.h trace.h tree.h udp.h usertask.h \
		   verisys.h veriuser.h veriwell.h v.h vtypes.h tree.def
libveriwell_la_LDFLAGS = -version-info 0:0:0
libveriwell_la_LIBADD =  $(LXTLIB) ../replace/libreplace.la

AM_YFLAGS = -y -d


