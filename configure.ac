dnl Process this file with autoconf to produce a configure script.
AC_INIT(libidn, 0.1.0, bug-libidn@josefsson.org)
AC_CONFIG_SRCDIR(stringprep.h.in)

# Interfaces removed:    CURRENT++, AGE=0, REVISION=0
# Interfaces added:      CURRENT++, AGE++, REVISION=0
# No interfaces changed:                   REVISION++
LT_CURRENT=5
LT_AGE=0
LT_REVISION=0
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_AGE)
AC_SUBST(LT_REVISION)

AM_INIT_AUTOMAKE(gnits)
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AM_MISSING_PROG(PERL, perl, $missing_dir)
AM_ICONV
AM_WITH_DMALLOC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(sys/types.h string.h strings.h memory.h unistd.h errno.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_REPLACE_FUNCS(memset strdup)

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

AC_CONFIG_FILES([Makefile libidn.pc stringprep.h])
AC_OUTPUT
