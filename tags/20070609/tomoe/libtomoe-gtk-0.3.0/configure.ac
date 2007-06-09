#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([libtomoe-gtk], 0.3.0, [daih@users.sourceforge.jp])
AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION, no-define)

# libtool
AM_PROG_LIBTOOL

AC_CONFIG_SRCDIR([src/tomoe_canvas.c],[src/tomoe_candidates_view.c], [src/tomoe_window.c])

# config.h
AC_CONFIG_HEADER(src/config.h)

# Checks for programs.
AC_PROG_CC

# pkg-config
PKG_CHECK_MODULES(GTK2, [gtk+-2.0 >= 2.4.0])
AC_SUBST(GTK2_CFLAGS)
AC_SUBST(GTK2_LIBS)
#
PKG_CHECK_MODULES(TOMOE, [tomoe >= 0.3.0])
AC_SUBST(TOMOE_CFLAGS)
AC_SUBST(TOMOE_LIBS)

GETTEXT_PACKAGE=libtomoe-gtk
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext package])

ALL_LINGUAS="ja"
AM_GLIB_GNU_GETTEXT
AM_GLIB_DEFINE_LOCALEDIR(LIBTOMOE_GTK_LOCALEDIR)

AC_CONFIG_FILES([
Makefile
po/Makefile.in
data/Makefile
src/Makefile
test/Makefile
libtomoe-gtk.pc
])

AC_OUTPUT
