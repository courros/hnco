AC_INIT([hnco], [0.4])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/hnco.cc])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT
AC_PROG_CXX

dnl Check for gmp for factorization
AC_CHECK_LIB([gmp], [__gmpz_init])
AC_CHECK_LIB([gmpxx], [abs])
AC_CHECK_HEADERS([gmp.h])
AC_ARG_ENABLE([factorization],
  [AS_HELP_STRING([--enable-factorization], [enable factorization @<:@default=no@:>@])],
  [:],
  [enable_factorization=no])
AM_CONDITIONAL([ENABLE_FACTORIZATION], [test "$enable_factorization" = "yes"])

AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile tests/Makefile scripts/Makefile])
AC_OUTPUT
