AC_INIT([hnco], [0.4])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/hnco.cc])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT

AC_LANG(C++)
AC_PROG_CXX
AC_OPENMP

AC_CHECK_LIB([gmp], [__gmpz_init])
AC_CHECK_LIB([gmpxx], [abs])

AC_CHECK_HEADERS([gmp.h])

AC_ARG_ENABLE([factorization], AS_HELP_STRING([--enable-factorization], [Enable factorization]))
AM_CONDITIONAL([ENABLE_FACTORIZATION], [test "x$enable_factorization" = "xyes"])
if test "x$enable_factorization" = "xyes"
then
  AC_DEFINE([ENABLE_FACTORIZATION], [1], [Define to 1 if factorization has been enabled])
fi

AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([LIBS])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile tests/Makefile scripts/Makefile])
AC_OUTPUT
