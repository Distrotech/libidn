dnl Process this file with autoconf to produce a configure script.

dnl Copyright (C) 2002, 2003 Simon Josefsson.
dnl
dnl This file is part of GNU Libidn.
dnl
dnl GNU Libidn is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU Lesser General Public License as
dnl published by the Free Software Foundation; either version 2.1 of
dnl the License, or (at your option) any later version.
dnl
dnl GNU Libidn is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with GNU Libidn; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

AC_INIT(libidn, 0.3.3, bug-libidn@gnu.org)

# Library code modified:                              REVISION++
# Interfaces changed/added/removed:   CURRENT++       REVISION=0
# Interfaces added:                             AGE++
# Interfaces removed:                           AGE=0
LT_CURRENT=11
LT_REVISION=3
LT_AGE=0
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AM_INIT_AUTOMAKE(gnits)
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AM_MISSING_PROG(PERL, perl, $missing_dir)
AM_MISSING_PROG(HELP2MAN, help2man, $missing_dir)
AM_ICONV
AM_PATH_LISPDIR

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h errno.h)

# Checks for typedefs, structures, and compiler characteristics.
AX_CREATE_STDINT_H(lib/idn-int.h)

# Need getopt.
AC_CHECK_FUNCS(getopt_long,, [AC_LIBOBJ(getopt) AC_LIBOBJ(getopt1)])

# Check if locale stuff works
AC_CACHE_CHECK([if setlocale() and nl_langinfo() works],
	ac_cv_locale_works, [
	AC_TRY_RUN([
#include <locale.h>
#include <langinfo.h>
int main ()
{
  char* cs;
  setlocale(LC_CTYPE, "");
  cs = nl_langinfo(CODESET);
  return !cs;
}
],
		ac_cv_locale_works=yes,
		ac_cv_locale_works=no,
		ac_cv_locale_works=no)
	])
if test "$ac_cv_locale_works" = "yes"; then
  AC_DEFINE(LOCALE_WORKS, 1, [Define if setlocale() and nl_langinfo() works.])
fi

# Check for gtk-doc.
AC_ARG_WITH(html-dir, AC_HELP_STRING([--with-html-dir=PATH], [path to installed docs]))
if test "x$with_html_dir" = "x" ; then
  HTML_DIR='${datadir}/gtk-doc/html'
else
  HTML_DIR=$with_html_dir
fi
AC_SUBST(HTML_DIR)
AC_ARG_ENABLE(gtk-doc,
	AC_HELP_STRING([--enable-gtk-doc], [Build API reference using GTK-DOC]),
	enable_gtk_doc="$enableval")
if test "$enable_gtk_doc" = "yes"; then
	GTKDOC=reference
fi
AC_SUBST(GTKDOC)
AM_MISSING_PROG(GTKDOC_SCAN, gtkdoc-scan, $missing_dir)
AM_MISSING_PROG(GTKDOC_MKTMPL, gtkdoc-mktmpl, $missing_dir)
AM_MISSING_PROG(GTKDOC_MKDB, gtkdoc-mkdb, $missing_dir)
AM_MISSING_PROG(GTKDOC_MKHTML, gtkdoc-mkhtml, $missing_dir)
AM_MISSING_PROG(GTKDOC_FIXXREF, gtkdoc-fixxref, $missing_dir)

AC_CONFIG_FILES([Makefile libidn.pc reference/Makefile \
		lib/stringprep.h src/idn.ggo \
		contrib/doxygen/Doxyfile m4/Makefile \
		doc/Makefile doc/specifications/Makefile doc/man/Makefile \
		lib/Makefile src/Makefile tests/Makefile examples/Makefile])
AC_OUTPUT
