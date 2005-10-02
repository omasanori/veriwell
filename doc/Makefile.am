EXTRA_DIST = veriwell.1
man_MANS   = veriwell.1

if USE_HELP2MAN
veriwell.1: $(top_srcdir)/src/veriwell.cc
	help2man -N $(top_builddir)/src/veriwell > $@
endif

#DISTCLEANFILES = veriwell.1
MAINTAINERCLEANFILES = Makefile.in 
