## Process this file with automake to produce Makefile.in
# Copyright (C) 2002 Simon Josefsson.
#
# This file is part of Libstringprep.
#
# Libstringprep is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# Libstringprep is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with Libstringprep; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

EXTRA_DIST = libstringprep.pc.in iconv.m4 strdup.c memset.c \
	gen-stringprep-tables.pl rfc3454.txt \
	gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt \
	SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt \
	CompositionExclusions-3.2.0.txt

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstringprep.pc

lib_LTLIBRARIES = libstringprep.la

include_HEADERS = stringprep.h stringprep_generic.h stringprep_nameprep.h \
	stringprep_kerberos5.h

libstringprep_la_SOURCES = gunicomp.h gunidecomp.h internal.h \
	stringprep.c nfkc.c version.c toutf8.c \
	stringprep_generic.c profiles.c \
	punycode.h punycode.c
libstringprep_la_LIBADD = @LTLIBOBJS@ @LTLIBICONV@
libstringprep_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

nfkc.c: gunicomp.h gunidecomp.h

gunicomp.h gunidecomp.h: gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt
	$(PERL) $(srcdir)/gen-unicode-tables.pl -decomp 3.2 UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt

stringprep_generic.c: rfc3454.txt gen-stringprep-tables.pl 
	$(PERL) $(srcdir)/gen-stringprep-tables.pl rfc3454.txt

tests = tst_stringprep tst_nfkc tst_punycode

check_PROGRAMS = $(tests)

TESTS = $(tests)

INCLUDES = -I$(top_srcdir)
LDADD = libstringprep.la

noinst_PROGRAMS = $(tests) example example2

ACLOCAL_AMFLAGS = -I .

.PHONY: ChangeLog
ChangeLog:
	cvs2cl --fsf --usermap .cvsusers --separate-header -I \
	"ChangeLog|.cvsignore|.cvsusers|autogen.sh"

indent:
	indent $(SOURCES)

sign: $(distdir).tar.gz
	rm -f $(distdir).tar.gz.asc
	gpg -a --sign --detach $(distdir).tar.gz
	gpg --verify $(distdir).tar.gz.asc
