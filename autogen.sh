#!/bin/sh -x
cat<<EOF > doc/Makefile.gdoc
gdoc_MANS =
gdoc_TEXINFOS =
EOF
gtkdocize
autoreconf --install --force --verbose
