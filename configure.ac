#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([CM0304 Coursework], [0.1], [alexduller@gmail.com])
AC_CONFIG_SRCDIR([src/coursework.cpp])
AC_CONFIG_AUX_DIR([scripts])

AM_INIT_AUTOMAKE([coursework], [0.1])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
AX_CHECK_GLUT
AC_CHECK_LIB([m], [cos])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
