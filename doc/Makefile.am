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

EXTRA_DIST = FAQ libidn.html libidn.ps libidn.pdf lgpl.texi

dist_man_MANS = idn.1

idn.1: ../idn.c
	$(HELP2MAN) --name="Internationalized Domain Names command line tool" \
		--output=$@ $(top_builddir)/idn

info_TEXINFOS = libidn.texi
pdf_TEXINFOS = libidn.texi
ps_TEXINFOS = libidn.texi

libidn.html: libidn.texi
	$(MAKEINFO) --html --no-split --number-sections $^

libidn_TEXINFOS = fdl.texi \
	example.c.texi example2.c.texi example3.c.texi example4.c.texi

%.c.texi: ../%.c
	sed -e 's/{/@{/g' -e 's/}/@}/g' < $< > $@
