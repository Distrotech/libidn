# ansi-c++.m4 serial 2
dnl Copyright (C) 2002-2003, 2010 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl From Bruno Haible.

# gl_PROG_ANSI_CXX([ANSICXX_VARIABLE], [ANSICXX_CONDITIONAL])
# Sets ANSICXX_VARIABLE to the name of a sufficiently ANSI C++ compliant
# compiler, or to ":" if none is found.
# Defines the Automake condition ANSICXX_CONDITIONAL to true if such a compiler
# was found, or to false if not.

AC_DEFUN([gl_PROG_ANSI_CXX],
[
  m4_if([$1], [CXX], [],
    [gl_save_CXX="$CXX"])
  if test -z "$CXX"; then
    if test -n "$CCC"; then
      CXX="$CCC"
    else
      AC_CHECK_PROGS([CXX], [g++ c++ gpp aCC CC cxx cc++ cl FCC KCC RCC xlC_r xlC], [:])
    fi
  fi
  if test "$CXX" != ":"; then
    dnl Use a modified version of AC_PROG_CXX_WORKS that does not exit
    dnl upon failure.
    AC_MSG_CHECKING([whether the C++ compiler ($CXX $CXXFLAGS $LDFLAGS) works])
    AC_LANG_PUSH([C++])
    AC_ARG_VAR([CXX], [C++ compiler command])
    AC_ARG_VAR([CXXFLAGS], [C++ compiler flags])
    echo 'int main () { return 0; }' > conftest.$ac_ext
    if AC_TRY_EVAL([ac_link]) && test -s conftest$ac_exeext; then
      gl_cv_prog_ansicxx_works=yes
      if (./conftest; exit) 2>/dev/null; then
        gl_cv_prog_ansicxx_cross=no
      else
        gl_cv_prog_ansicxx_cross=yes
      fi
    else
      gl_cv_prog_ansicxx_works=no
    fi
    rm -fr conftest*
    AC_LANG_POP([C++])
    AC_MSG_RESULT([$gl_cv_prog_ansicxx_works])
    if test $gl_cv_prog_ansicxx_works = no; then
      CXX=:
    else
      dnl Test for namespaces.
      dnl We don't bother supporting pre-ANSI-C++ compilers.
      AC_MSG_CHECKING([whether the C++ compiler supports namespaces])
      AC_LANG_PUSH([C++])
      cat <<EOF > conftest.$ac_ext
#include <iostream>
namespace test { using namespace std; }
std::ostream* ptr;
int main () { return 0; }
EOF
      if AC_TRY_EVAL([ac_link]) && test -s conftest$ac_exeext; then
        gl_cv_prog_ansicxx_namespaces=yes
      else
        gl_cv_prog_ansicxx_namespaces=no
      fi
      rm -fr conftest*
      AC_LANG_POP([C++])
      AC_MSG_RESULT([$gl_cv_prog_ansicxx_namespaces])
      if test $gl_cv_prog_ansicxx_namespaces = no; then
        CXX=:
      fi
    fi
  fi
  m4_if([$1], [CXX], [],
    [$1="$CXX"
     CXX="$gl_save_CXX"])
  AC_SUBST([$1])

  AM_CONDITIONAL([$2], [test "$$1" != ":"])

  dnl This macro invocation resolves an automake error:
  dnl /usr/local/share/automake-1.11/am/depend2.am: am__fastdepCXX does not appear in AM_CONDITIONAL
  dnl /usr/local/share/automake-1.11/am/depend2.am:   The usual way to define `am__fastdepCXX' is to add `AC_PROG_CXX'
  dnl /usr/local/share/automake-1.11/am/depend2.am:   to `configure.ac' and run `aclocal' and `autoconf' again.
  _AM_DEPENDENCIES([CXX])
])
