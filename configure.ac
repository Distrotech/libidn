dnl Process this file with autoconf to produce a configure script.
AC_INIT(libstringprep, 0.0.2, bug-libstringprep@josefsson.org)
AC_CONFIG_SRCDIR(stringprep.h.in)

# Interfaces removed:    CURRENT++, AGE=0, REVISION=0
# Interfaces added:      CURRENT++, AGE++, REVISION=0
# No interfaces changed:                   REVISION++
LT_CURRENT=1
LT_AGE=1
LT_REVISION=0
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_AGE)
AC_SUBST(LT_REVISION)

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_AWK
AM_MISSING_PROG(PERL, perl, $missing_dir)
AM_ICONV

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(stdio.h stdarg.h sys/types.h stdlib.h string.h unistd.h \
		 errno.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strchr strdup])

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

AC_CONFIG_FILES([Makefile libstringprep.pc stringprep.h])
AC_OUTPUT
