EXTRA_DIST = veriwell.1
man_MANS   = veriwell.1

if USE_HELP2MAN
veriwell.1: $(top_builddir)/src/veriwell
	help2man -N $< > $@
endif

DISTCLEANFILES = veriwell.1
MAINTAINERCLEANFILES = Makefile.in 
