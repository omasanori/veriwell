if USE_LXT
   LXTSOURCE=lxt.c lxt_write.c lxt_write.h
   LXTLIB = liblxt.la
endif
if USE_LXT2
   LXT2SOURCE=lxt2.c lxt2_write.c lxt2_write.h
   LXTLIB = liblxt.la
endif
if BUILD_LIBZ
    LIBZ_INCLUDE = -I $(top_srcdir)/replace/libz
endif
if BUILD_LIBBZ2
    LIBBZ2_INCLUDE = -I $(top_srcdir)/replace/libbz2
endif
noinst_LTLIBRARIES = $(LXTLIB)

liblxt_la_SOURCES = $(LXTSOURCE) $(LXT2SOURCE)
liblxt_la_LDFLAGS = -version-info 0:0:0

AM_CFLAGS = -I $(top_srcdir)/src $(LIBZ_INCLUDE) $(LIBBZ2_INCLUDE)


EXTRA_DIST = LXT_Explained.html test.v lxt.tab

MAINTAINERCLEANFILES = Makefile.in 
