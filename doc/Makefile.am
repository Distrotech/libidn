## Process this file with automake to produce Makefile.in
# Copyright (C) 2002, 2003 Simon Josefsson.
#
# This file is part of GNU Libidn.
#
# GNU Libidn is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# GNU Libidn is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with GNU Libidn; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

SUBDIRS = specifications
if ENABLE_GTK_DOC
SUBDIRS += reference
endif

EXTRA_DIST = gdoc libidn.html libidn.ps libidn.pdf lgpl.texi	\
	libidn-db.xml libidn-db.txt libidn-db.pdf libidn-db.ps	\
	$(libidn_html_auto_deps)

info_TEXINFOS = libidn.texi
libidn_TEXINFOS = fdl.texi $(gdoc_TEXINFOS)

AM_MAKEINFOHTMLFLAGS = --no-split --number-sections

# Man pages.

dist_man_MANS = idn.1 $(gdoc_MANS)
MAINTAINERCLEANFILES = $(dist_man_MANS)

idn.1: $(top_srcdir)/src/idn.c
	$(HELP2MAN) \
		--name="Internationalized Domain Names command line tool" \
		--output=$@ $(top_builddir)/src/idn

# GDOC

GDOC_SRC = $(top_srcdir)/lib/*.c
GDOC_TEXI_PREFIX = texi/
GDOC_MAN_PREFIX = man/
GDOC_MAN_EXTRA_ARGS = -module $(PACKAGE) -sourceversion $(VERSION) \
	-bugsto $(PACKAGE_BUGREPORT) -includefuncprefix -seeinfo $(PACKAGE) \
	-copyright "2002, 2003 Simon Josefsson" -verbatimcopying
include $(srcdir)/Makefile.gdoci

# DocBook

libidn_html_auto_deps = html/a1559.html html/a1682.html			\
	html/c1176.html html/c1197.html html/c1207.html			\
	html/c1210.html html/c1215.html html/c1220.html			\
	html/c1225.html html/c1304.html html/c16.html html/c261.html	\
	html/c364.html html/c541.html html/c803.html html/c9.html	\
	html/c917.html html/i1309.html html/i1440.html html/index.html	\
	html/x1052.html html/x1099.html html/x1185.html			\
	html/x1189.html html/x1193.html html/x1268.html			\
	html/x1667.html html/x1785.html html/x179.html html/x195.html	\
	html/x214.html html/x232.html html/x29.html html/x297.html	\
	html/x300.html html/x319.html html/x345.html html/x376.html	\
	html/x45.html html/x456.html html/x490.html html/x553.html	\
	html/x558.html html/x611.html html/x625.html html/x740.html	\
	html/x815.html html/x835.html html/x842.html html/x929.html	\
	html/x976.html html/x988.html html/x995.html

libidn-db.xml: libidn.texi $(libidn_TEXINFOS)
	$(MAKEINFO) --docbook --output $@ libidn.texi

$(libidn_html_auto_deps): libidn-db.xml
	$(DOCBOOK2HTML) -o html $^

libidn-db.txt: libidn-db.xml
	$(DOCBOOK2TXT) $^

libidn-db.pdf: libidn-db.xml
	$(DOCBOOK2PDF) $^

libidn-db.ps: libidn-db.xml
	$(DOCBOOK2PS) $^
