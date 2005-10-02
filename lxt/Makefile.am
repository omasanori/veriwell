if USE_LXT
   LXTSOURCE=lxt.c lxt_write.c lxt_write.h
   LXTLIB = liblxt.la
endif
if USE_LXT2
   LXT2SOURCE=lxt2.c lxt2_write.c lxt2_write.h
   LXTLIB = liblxt.la
endif
noinst_LTLIBRARIES = $(LXTLIB)

liblxt_la_SOURCES = $(LXTSOURCE) $(LXT2SOURCE)
liblxt_la_LDFLAGS = -version-info 0:0:0

AM_CFLAGS = -I $(top_srcdir)/src


EXTRA_DIST = LXT_Explained.html test.v lxt.tab

MAINTAINERCLEANFILES = Makefile.in 
