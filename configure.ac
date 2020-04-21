# Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

# This file is part of HNCO.

# HNCO is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# HNCO is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
# Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with HNCO. If not, see
# <http://www.gnu.org/licenses/>.

# Prelude.
AC_INIT([hnco], [0.14])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([app/hnco.cc])

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
if test "x$enable_factorization" = "xyes"
then
  AC_DEFINE([ENABLE_FACTORIZATION], [1], [Define to 1 if factorization has been enabled])
fi
AM_CONDITIONAL([ENABLE_FACTORIZATION], [test "x$enable_factorization" = "xyes"])

AC_ARG_ENABLE([plugin], AS_HELP_STRING([--enable-plugin], [Enable plugin]))
if test "x$enable_plugin" = "xyes"
then
  AC_DEFINE([ENABLE_PLUGIN], [1], [Define to 1 if plugin has been enabled])
fi
AM_CONDITIONAL([ENABLE_PLUGIN], [test "x$enable_plugin" = "xyes"])

# Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile app/Makefile tests/Makefile scripts/Makefile examples/Makefile])
AC_OUTPUT
