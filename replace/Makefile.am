if BUILD_LIBZ
    LIBZ_DIR = libz
    LIBZ_LIB = $(top_builddir)/replace/libz/libz.la
endif
if BUILD_LIBBZ2
    LIBBZ2_DIR = libbz2
    LIBBZ2_LIB = $(top_builddir)/replace/libbz2/libbz2.la
endif

SUBDIRS = $(LIBZ_DIR) $(LIBBZ2_DIR) .

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

noinst_LTLIBRARIES    = libreplace.la
libreplace_la_SOURCES =
libreplace_la_LIBADD  = $(LTLIBOBJS) $(LIBZ_LIB) $(LIBBZ2_LIB)

MAINTAINERCLEANFILES = Makefile.in 
