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

EXTRA_DIST = gen-stringprep-tables.pl gen-unicode-tables.pl 

lib_LTLIBRARIES = libidn.la

idn_int = idn-int.h

BUILT_SOURCES = $(idn_int) \
	gunibreak.h gunicomp.h gunidecomp.h \
	stringprep_generic.c

DISTCLEANFILES = $(idn_int)
include_HEADERS = stringprep.h stringprep_generic.h stringprep_nameprep.h \
	stringprep_kerberos5.h stringprep_xmpp.h stringprep_plain.h \
	stringprep_iscsi.h idna.h punycode.h
nodist_include_HEADERS = $(idn_int)

libidn_la_SOURCES = gunicomp.h gunidecomp.h internal.h \
	stringprep.c nfkc.c version.c toutf8.c \
	stringprep_generic.c obsolete.c profiles.c \
	punycode.h punycode.c \
	idna.h idna.c
libidn_la_LIBADD = @LTLIBICONV@
libidn_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

nfkc.c: gunibreak.h gunicomp.h gunidecomp.h

SPEC = ../doc/specifications

gunibreak.h gunicomp.h gunidecomp.h: gen-unicode-tables.pl $(SPEC)/UnicodeData-3.2.0.txt $(SPEC)/LineBreak-3.2.0.txt $(SPEC)/SpecialCasing-3.2.0.txt $(SPEC)/CaseFolding-3.2.0.txt $(SPEC)/CompositionExclusions-3.2.0.txt
	$(PERL) $(srcdir)/gen-unicode-tables.pl -decomp 3.2 $(SPEC)/UnicodeData-3.2.0.txt $(SPEC)/LineBreak-3.2.0.txt $(SPEC)/SpecialCasing-3.2.0.txt $(SPEC)/CaseFolding-3.2.0.txt $(SPEC)/CompositionExclusions-3.2.0.txt

stringprep_generic.c: gen-stringprep-tables.pl
	$(PERL) $(srcdir)/gen-stringprep-tables.pl $(SPEC)/rfc3454.txt
