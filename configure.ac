# Prelude.
AC_INIT([hnco], [0.7])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/hnco.cc])

AM_INIT_AUTOMAKE([foreign subdir-objects])

LT_INIT

# Checks for programs.
AC_PROG_CXX

AC_LANG(C++)

# Checks for libraries.
AC_CHECK_LIB([gmp], [__gmpz_init])
AC_CHECK_LIB([gmpxx], [abs])
AC_CHECK_LIB([dl], [dlopen])
AX_BOOST_BASE
AX_BOOST_SERIALIZATION

# Checks for header files.
AC_CHECK_HEADERS([gmp.h])
AC_CHECK_HEADERS([dlfcn.h])
AC_CHECK_HEADERS([omp.h])

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX_11
AC_OPENMP

# Checks for optional features.
AC_ARG_ENABLE([factorization], AS_HELP_STRING([--enable-factorization], [Enable factorization]))
AM_CONDITIONAL([ENABLE_FACTORIZATION], [test "x$enable_factorization" = "xyes"])
if test "x$enable_factorization" = "xyes"
then
  AC_DEFINE([ENABLE_FACTORIZATION], [1], [Define to 1 if factorization has been enabled])
fi

# Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile tests/Makefile scripts/Makefile examples/Makefile])
AC_OUTPUT
