#!/bin/sh -x
ACLOCAL=${ACLOCAL:-aclocal}; export ACLOCAL
AUTOMAKE=${AUTOMAKE:-automake}; export AUTOMAKE
AUTOCONF=${AUTOCONF:-autoconf}; export AUTOCONF
LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}; export LIBTOOLIZE
AUTOHEADER=${AUTOHEADER:-autoheader}; export AUTOHEADER

rm -vf config.cache &&
rm -rvf autom4te.cache &&
$ACLOCAL
$LIBTOOLIZE --force --automake
$ACLOCAL
$AUTOCONF
$AUTOMAKE --gnits --add-missing
$AUTOHEADER
: 'You can now run CFLAGS=-g ./configure --disable-shared and then make.'
