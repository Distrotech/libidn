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

AC_INIT(libidn, 0.2.2, bug-libidn@gnu.org)
AC_CONFIG_SRCDIR(stringprep.h.in)

# Interfaces removed:    CURRENT++, AGE=0, REVISION=0
# Interfaces added:      CURRENT++, AGE++, REVISION=0
# No interfaces changed:                   REVISION++
LT_CURRENT=10
LT_AGE=0
LT_REVISION=2
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_AGE)
AC_SUBST(LT_REVISION)

AM_INIT_AUTOMAKE(gnits)
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AM_MISSING_PROG(PERL, perl, $missing_dir)
AM_MISSING_PROG(HELP2MAN, help2man, $missing_dir)
AM_ICONV
AM_WITH_DMALLOC
AM_PATH_LISPDIR

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/types.h string.h strings.h memory.h unistd.h errno.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AX_CREATE_STDINT_H(idn-int.h)

# For gnulib stuff.
gl_FUNC_MEMSET
gl_FUNC_STRDUP

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

# Check for getopt in standard library.
AC_CHECK_FUNCS(getopt_long,, [AC_LIBOBJ(getopt) AC_LIBOBJ(getopt1)])

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

AC_CONFIG_FILES([Makefile libidn.pc stringprep.h reference/Makefile idn.ggo \
		contrib/doxygen/Doxyfile doc/Makefile m4/Makefile])
AC_OUTPUT
