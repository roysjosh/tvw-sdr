AC_INIT([tvw-sdr], [2012.08.16], [roysjosh@gmail.com])
AC_PREREQ([2.61])
AM_INIT_AUTOMAKE([foreign dist-bzip2])
LT_INIT([disable-static])
AC_CONFIG_SRCDIR([src/libtvwsdr.c])
AC_CONFIG_HEADERS([config.h])

dnl kernel style compile messages
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC

AC_LANG([C])

# Checks for libraries.
PKG_CHECK_MODULES(LIBUSB, libusb-1.0 >= 1.0)
LIBS="$LIBS $LIBUSB_LIBS"
CFLAGS="$CFLAGS $LIBUSB_CFLAGS"

# Checks for headers.
AC_CHECK_HEADERS([endian.h machine/endian.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.

# All done!
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
