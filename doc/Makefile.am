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

EXTRA_DIST = gdoc libidn.html libidn.ps libidn.pdf lgpl.texi	\
	libidn-db.xml libidn-db.txt libidn-db.pdf libidn-db.ps	\
	$(libidn_html_auto_deps)

info_TEXINFOS = libidn.texi
pdf_TEXINFOS = libidn.texi
ps_TEXINFOS = libidn.texi

libidn_TEXINFOS = fdl.texi $(gdoc_TEXINFOS) \
	example.c.texi example2.c.texi example3.c.texi example4.c.texi

libidn.html: libidn.texi $(libidn_TEXINFOS)
	$(MAKEINFO) --html --no-split --number-sections libidn.texi

%.c.texi: ../examples/%.c
	sed -e 's/{/@{/g' -e 's/}/@}/g' < $< > $@

# Man pages.

dist_man_MANS = idn.1 $(gdoc_MANS)
MAINTAINERCLEANFILES = $(dist_man_MANS)

idn.1: $(top_srcdir)/src/idn.c
	$(HELP2MAN) \
		--name="Internationalized Domain Names command line tool" \
		--output=$@ $(top_builddir)/src/idn

# GDOC

GDOC_SRC = $(top_srcdir)/lib/version.c \
		$(top_srcdir)/lib/toutf8.c \
		$(top_srcdir)/lib/nfkc.c \
		$(top_srcdir)/lib/punycode.c \
		$(top_srcdir)/lib/stringprep.c \
		$(top_srcdir)/lib/idna.c
GDOC_TEXI = libidn
GDOC_TEXI_PREFIX = texi/
GDOC_MAN = dist_man
GDOC_MAN_PREFIX = man/
include $(srcdir)/Makefile.gdoci

# DocBook

libidn_html_auto_deps = html/a1553.html html/a1676.html			\
	html/c1166.html html/c1191.html html/c1201.html			\
	html/c1204.html html/c1209.html html/c1214.html			\
	html/c1219.html html/c1298.html html/c16.html html/c261.html	\
	html/c364.html html/c531.html html/c793.html html/c9.html	\
	html/c907.html html/i1303.html html/i1434.html html/index.html	\
	html/x1042.html html/x1089.html html/x1176.html			\
	html/x1181.html html/x1186.html html/x1262.html			\
	html/x1661.html html/x1779.html html/x179.html html/x195.html	\
	html/x214.html html/x232.html html/x29.html html/x297.html	\
	html/x300.html html/x319.html html/x345.html html/x376.html	\
	html/x446.html html/x45.html html/x480.html html/x543.html	\
	html/x548.html html/x601.html html/x615.html html/x730.html	\
	html/x805.html html/x825.html html/x832.html html/x919.html	\
	html/x966.html html/x978.html html/x985.html

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
