#
# readline lib detection
#
AC_DEFUN([VL_LIB_READLINE], [
  AC_CACHE_CHECK([for a readline compatible library],
                 vl_cv_lib_readline, [
    ORIG_LIBS="$LIBS"
    for readline_lib in readline edit editline; do
      for termcap_lib in "" termcap curses ncurses; do
        if test -z "$termcap_lib"; then
          TRY_LIB="-l$readline_lib"
        else
          TRY_LIB="-l$readline_lib -l$termcap_lib"
        fi
        LIBS="$ORIG_LIBS $TRY_LIB"
        AC_TRY_LINK_FUNC(readline, vl_cv_lib_readline="$TRY_LIB")
        if test -n "$vl_cv_lib_readline"; then
          break
        fi
      done
      if test -n "$vl_cv_lib_readline"; then
        break
      fi
    done
    if test -z "$vl_cv_lib_readline"; then
      vl_cv_lib_readline="no"
      LIBS="$ORIG_LIBS"
    fi
  ])

  if test "$vl_cv_lib_readline" != "no"; then
    AC_DEFINE(HAVE_LIBREADLINE, 1,
              [Define if you have a readline compatible library])
    AC_CHECK_HEADERS(readline/readline.h)
    AC_CHECK_HEADERS(readline.h)
    AC_CACHE_CHECK([whether readline supports history],
                   vl_cv_lib_readline_history, [
      vl_cv_lib_readline_history="no"
      AC_TRY_LINK_FUNC(add_history, vl_cv_lib_readline_history="yes")
    ])
    if test "$vl_cv_lib_readline_history" = "yes"; then
      AC_DEFINE(HAVE_READLINE_HISTORY, 1,
                [Define if your readline library has \`add_history'])
      AC_CHECK_HEADERS(history.h readline/history.h)
    fi
  fi
])dnl


#
# stdin vfscanf detection
#
AC_DEFUN([MDH_STDIN_VFSCANF], [
    AC_CACHE_CHECK(
        [for vfscanf in stdio.h],
        ac_cv_stdin_vfscanf, [
            AC_TRY_LINK(
                [ #include <stdio.h>
                  #include <stdarg.h>],
                [ FILE* f;
                  va_list ap;
                  vfscanf(f, "format", ap); ],
                ac_cv_stdin_vfscanf=yes,
                ac_cv_stdin_vfscanf=no)
        ]
    )

    if test x"$ac_cv_stdin_vfscanf" = "xyes"
    then
        AC_DEFINE(HAVE_VFSCANF, 1,
            [ Define if your system has vfscanf() in stdin.h ])
    else
        AC_LIBOBJ([vfscanf])
    fi
]) dnl MDH_STDIN_VFSCANF

