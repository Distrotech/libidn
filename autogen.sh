#!/bin/sh -x
cat<<EOF > doc/Makefile.gdoc
gdoc_MANS =
gdoc_TEXINFOS =
EOF
gtkdocize
autoreconf --install --force
: 'Run "./configure --enable-gtk-doc && make" now.'
