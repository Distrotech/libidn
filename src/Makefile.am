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

INCLUDES = -I$(top_srcdir)/lib -I../lib

bin_PROGRAMS = idn

dist_lisp_DATA = punycode.el idna.el

BUILT_SOURCES = idn_cmd.c idn_cmd.h
idn_SOURCES = idn.c $(BUILT_SOURCES)
EXTRA_DIST = getopt.h
DISTCLEANFILES = $(BUILT_SOURCES)
idn_LDADD = ../lib/libidn.la $(LTLIBOBJS)

idn.c: $(BUILT_SOURCES)

$(BUILT_SOURCES): idn.ggo
	gengetopt --input $^ --file-name idn_cmd || \
		cp $(srcdir)/idn_cmd.h $(srcdir)/idn_cmd.h .

# Maintainer targets

indent:
	indent $(SOURCES)
