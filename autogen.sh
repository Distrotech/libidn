#!/bin/sh -x
echo 'BUILT_SOURCES = Makefile.gdoc' > doc/Makefile.gdoc
autoreconf --install --force --verbose
