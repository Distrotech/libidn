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

EXTRA_DIST = libstringprep.pc.in \
	gen-stringprep-tables.pl draft-hoffman-stringprep-07.txt \
	gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstringprep.pc

lib_LTLIBRARIES = libstringprep.la

include_HEADERS = stringprep.h stringprep_generic.h stringprep_nameprep.h \
	stringprep_kerberos5.h

libstringprep_la_SOURCES = gunicomp.h gunidecomp.h stringprep.c nfkc.c \
	stringprep_generic.c version.c 
libstringprep_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

gunicomp.h gunidecomp.h: gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt
	$(PERL) $(srcdir)/gen-unicode-tables.pl -decomp 3.2 UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt

stringprep_generic.c: draft-hoffman-stringprep-07.txt gen-stringprep-tables.pl 
	$(PERL) $(srcdir)/gen-stringprep-tables.pl $<

check_PROGRAMS = tst_stringprep

TESTS = $(check_PROGRAMS)

INCLUDES = -I$(top_srcdir)
LDADD = libstringprep.la

noinst_PROGRAMS = $(check_PROGRAMS)

ChangeLog:
	cvs2cl --fsf --usermap .cvsusers --separate-header -I \
	"ChangeLog|.cvsignore|.cvsusers|autogen.sh"

indent:
	indent $(libstringprep_la_SOURCES)
