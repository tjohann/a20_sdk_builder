lf# configure.ac

AC_INIT([A20 SDK builder], [trunk], [thorsten.johannvorderbrueggen@t-online.de])
AC_COPYRIGHT([Copyright 2016, Thorsten Johannvorderbrueggen])

AC_CONFIG_AUX_DIR([build-aux/autoconf])
AC_CONFIG_MACRO_DIR([build-aux/m4])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile lib/Makefile po/Makefile.in pixmaps/Makefile])

AM_GNU_GETTEXT_VERSION([0.19.4])
AM_GNU_GETTEXT([external])

AC_PROG_CC
CFLAGS="${CFLAGS} -g -std=gnu11 -O2 -Wall -Wextra -pedantic -Wstrict-overflow -fno-strict-aliasing"

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2])

# libservice
AM_PROG_AR
AC_PROG_RANLIB

# -> ./configure --enable-silent-rules to get quite build behaviour
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([no])])

# ------

# Checks for programs.
PKG_PROG_PKG_CONFIG

# Checks for libraries.
PKG_CHECK_MODULES([LIBGTK], [gtk+-2.0])
PKG_CHECK_MODULES([LIBCONFIG], [libconfig])
PKG_CHECK_MODULES([LIBGIT2], [libgit2])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
