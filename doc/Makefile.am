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

SUBDIRS = specifications man

EXTRA_DIST = libidn.html libidn.ps libidn.pdf lgpl.texi libidn-db.xml	\
	libidn-db.txt libidn-db.pdf libidn-db.ps html/a1470.html	\
	html/a1593.html html/c1088.html html/c1113.html			\
	html/c1123.html html/c1126.html html/c1131.html			\
	html/c1136.html html/c1141.html html/c1220.html html/c16.html	\
	html/c261.html html/c363.html html/c756.html html/c846.html	\
	html/c9.html html/i1225.html html/i1351.html html/index.html	\
	html/x1016.html html/x1098.html html/x1103.html			\
	html/x1108.html html/x1184.html html/x1578.html			\
	html/x1696.html html/x179.html html/x195.html html/x214.html	\
	html/x232.html html/x29.html html/x297.html html/x300.html	\
	html/x318.html html/x344.html html/x421.html html/x435.html	\
	html/x440.html html/x45.html html/x551.html html/x617.html	\
	html/x649.html html/x693.html html/x781.html html/x788.html	\
	html/x898.html html/x910.html html/x917.html html/x972.html

info_TEXINFOS = libidn.texi
pdf_TEXINFOS = libidn.texi
ps_TEXINFOS = libidn.texi

libidn.html: libidn.texi
	$(MAKEINFO) --html --no-split --number-sections $^

libidn_TEXINFOS = fdl.texi \
	example.c.texi example2.c.texi example3.c.texi example4.c.texi

%.c.texi: ../examples/%.c
	sed -e 's/{/@{/g' -e 's/}/@}/g' < $< > $@

libidn-db.xml: libidn.texi
	$(MAKEINFO) --docbook --output $@ $^

libidn-db.html: libidn-db.xml
	$(DOCBOOK2HTML) -o html $^

libidn-db.txt: libidn-db.xml
	$(DOCBOOK2TXT) $^

libidn-db.pdf: libidn-db.xml
	$(DOCBOOK2PDF) $^

libidn-db.ps: libidn-db.xml
	$(DOCBOOK2PS) $^
