AC_PREREQ(2.59)
AC_INIT(veriwell,2.8.7)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

# Check for user supplied arguments
AC_MSG_CHECKING([use 32 bit compile])
AC_ARG_ENABLE([m32],AC_HELP_STRING([--enable-m32],
		                   [compile in 32 bit modes (default is no)]),
	      [ac_cv_enable_m32=yes], [ac_cv_enable_m32=no])
AC_MSG_RESULT([$ac_cv_enable_m32])

AC_MSG_CHECKING([enable lxt support])
AC_ARG_ENABLE([lxt],AC_HELP_STRING([--disable-lxt],
		                   [compile lxt pli routines (default is yes)]),
	      [ac_cv_enable_lxt=no],ac_cv_enable_lxt=yes)
AC_MSG_RESULT([$ac_cv_enable_lxt])

AC_MSG_CHECKING([enable lxt2 support])
AC_ARG_ENABLE([lxt2],AC_HELP_STRING([--disable-lxt2],
		                   [compile lxt2 pli routines (default is yes)]),
	      [ac_cv_enable_lxt2=no],ac_cv_enable_lxt2=yes)
AC_MSG_RESULT([$ac_cv_enable_lxt2])

# Before configuring libtool check for --enable-all-static option
AC_ARG_ENABLE(all-static,
  AC_HELP_STRING([--enable-all-static],
                 [Build completely static (standalone) binaries.]),
  [
    if test "$enableval" = "yes" ; then
      LT_LDFLAGS="-all-static $LT_LDFLAGS"
    fi
])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
LT_INIT
AC_PATH_PROG([HELP2MAN],[help2man])
AM_CONDITIONAL( [USE_HELP2MAN], [test -n "$HELP2MAN"])
if test -z "$HELP2MAN"
then
        echo ""
        echo "*** Warning: No suitable help2man found. ***"
        echo "    The help2man package is essential for rebuilding veriwell "
        echo "    man pages. You can get away without it when simply "
	echo "    building from snapshots or major releases."
        echo ""
fi

if test X"$ac_cv_enable_m32" = Xyes 
then  
    CFLAGS="$CFLAGS -m32"
    CPPFLAGS="$CPPFLAGS -m32"
    LDFLAGS="$LDFLAGS -m32"
fi
AC_PROG_LIBTOOL


AC_ARG_ENABLE(gprof,
AC_HELP_STRING([--enable-gprof],
	       [Produce gprof profiling data in 'gmon.out' (GCC only).]),
[
    if test "$enableval" = "yes" ; then
      dnl Probably other compilers support -pg or something similar;
      dnl feel free to extend this to include them.
      if test "$GCC" = "yes"; then
        if test "$enable_shared" = "yes" ; then
          AC_MSG_ERROR(Can't have --enable-gprof without --disable-shared (we
                  recommend also using --enable-all-static).)
        fi
        if test ! "$enable_all_static" = "yes" ; then
          AC_MSG_WARN(We recommend --enable-all-static with --enable-gprof.)
        fi
        AC_MSG_NOTICE([Enabling gprof profiling data (to gmon.out).])
        CFLAGS="$CFLAGS -pg"
        CXXFLAGS="$CXXFLAGS -pg"
        LDFLAGS="$LDFLAGS -pg"
        LT_LDFLAGS="$LT_LDFLAGS -pg"
      else
        AC_MSG_ERROR(We only support --enable-gprof with GCC right now.)
      fi
    fi
])

AC_CHECK_PROGS([XGPERF],[gperf],[none])
if test "$XGPERF" = "none"
then
        echo ""
        echo "*** Warning: No suitable gperf found. ***"
        echo "    The gperf package is essential for building veriwell from"
        echo "    CVS sources, or modifying the parse engine of veriwell itself."
        echo "    You can get away without it when simply building from"
        echo "    snapshots or major releases."
        echo ""
fi

AC_CHECK_PROGS([YACC],[bison],[none])
if test "$YACC" = "none"
then
        echo ""
        echo "*** Warning: No suitable bison found. ***"
        echo "    The bison package is essential for building veriwell from"
        echo "    CVS sources, or modifying the parse engine of veriwell itself."
        echo "    You can get away without it when simply building from"
        echo "    snapshots or major releases."
        echo ""
fi



# Checks for libraries.
AC_CHECK_LIB([z], [gzwrite], [AC_DEFINE(HAVE_LIBZ,[],
	[Define to 1 if you have the `z' library (-lz).])])
AM_CONDITIONAL( [BUILD_LIBZ], [test "$ac_cv_lib_z_gzwrite" = no])
AC_CHECK_LIB([bz2], [BZ2_bzwrite], [AC_DEFINE(HAVE_LIBBZ2,[],
	[Define to 1 if you have the `bz2' library (-lbz2).])])
AM_CONDITIONAL( [BUILD_LIBBZ2], [test "$ac_cv_lib_bz2_BZ2_bzwrite" = no])


VL_LIB_READLINE
if test ! "$HAVE_LIBREADLINE" = 1; then
  AC_LIBOBJ([readline])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h memory.h stddef.h stdlib.h stdarg.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_C_BIGENDIAN

# Checks for library functions.
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memmove memset pow strchr strdup strrchr])
AC_LANG_PUSH(C++)
MDH_STDIN_VFSCANF
AC_LANG_POP


# optional pli code
AM_CONDITIONAL( [USE_LXT], [test "$ac_cv_enable_lxt" = yes])
if test "$ac_cv_enable_lxt" = yes 
then
    AC_DEFINE([HAVE_LXT],1,[Set to one if lxt library should be used])
fi
AM_CONDITIONAL( [USE_LXT2], [test "$ac_cv_enable_lxt2" = yes])
if test "$ac_cv_enable_lxt2" = yes 
then
    AC_DEFINE([HAVE_LXT2],1,[Set to one if lxt2 library should be used])
fi

AC_CONFIG_FILES([Makefile] [replace/Makefile] [replace/libz/Makefile] [replace/libbz2/Makefile] [src/Makefile] [lxt/Makefile] [doc/Makefile])
AC_OUTPUT
