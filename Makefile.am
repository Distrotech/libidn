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

DIST_SUBDIRS = reference
SUBDIRS = @GTKDOC@

ACLOCAL_AMFLAGS = -I .

EXTRA_DIST = libidn.pc.in iconv.m4 strdup.c memset.c \
	gen-stringprep-tables.pl rfc3454.txt \
	gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt \
	SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt \
	CompositionExclusions-3.2.0.txt \
	gdoc libidn.html libidn.ps libidn.pdf lgpl.texi \
	libc/README libc/Makefile libc/Versions libc/configure \
	libc/example.c libc/getaddrinfo.c libc/libc.patch libc/netdb.h \
	contrib/README \
	contrib/idn-python/README contrib/idn-python/Makefile \
	contrib/idn-python/idn.c contrib/idn-python/test.py \
	getopt.h getopt.c getopt1.c \
	idn.1 punycode.el idna.el

clean-local:
	rm -f idn_cmd.c idn_cmd.h punycode.elc idna.elc

# Library

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libidn.pc

lib_LTLIBRARIES = libidn.la

include_HEADERS = stringprep.h stringprep_generic.h stringprep_nameprep.h \
	stringprep_kerberos5.h stringprep_xmpp.h stringprep_plain.h \
	stringprep_iscsi.h idna.h punycode.h

libidn_la_SOURCES = gunicomp.h gunidecomp.h internal.h \
	stringprep.c nfkc.c version.c toutf8.c \
	stringprep_generic.c profiles.c \
	punycode.h punycode.c \
	idna.h idna.c
libidn_la_LIBADD = @LTLIBOBJS@ @LTLIBICONV@
libidn_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

nfkc.c: gunicomp.h gunidecomp.h

gunicomp.h gunidecomp.h: gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt
	$(PERL) $(srcdir)/gen-unicode-tables.pl -decomp 3.2 UnicodeData-3.2.0.txt LineBreak-3.2.0.txt SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt CompositionExclusions-3.2.0.txt

stringprep_generic.c: rfc3454.txt gen-stringprep-tables.pl
	$(PERL) $(srcdir)/gen-stringprep-tables.pl rfc3454.txt

# Applications

bin_PROGRAMS = idn

idn_SOURCES = idn.c idn_cmd.c idn_cmd.h
idn_LDADD = @LIBOBJS@ libidn.la

idn_cmd.c idn_cmd.h: idn.ggo
	gengetopt --input $< --file-name idn_cmd

man_MANS = idn.1

idn.1: idn.c
	$(HELP2MAN) --name="Internationalized Domain Names command line tool" \
		--output=$@ $(top_builddir)/idn

# Test suite

tests = tst_stringprep tst_nfkc tst_punycode tst_idna

check_PROGRAMS = $(tests)

TESTS = $(tests)

INCLUDES = -I$(top_srcdir)
LDADD = libidn.la

noinst_PROGRAMS = $(tests) example example2 example3 example4

# Lisp interface

lisp_LISP = punycode.el idna.el
ELCFILES =

# Documentation

info_TEXINFOS = libidn.texi
pdf_TEXINFOS = libidn.texi
ps_TEXINFOS = libidn.texi

libidn.html: libidn.texi
	$(MAKEINFO) --html --no-split --number-sections $<

libidn_TEXINFOS = fdl.texi \
	example.c.texi example2.c.texi example3.c.texi example4.c.texi \
	libidn-api-version.texi libidn-api-stringprep.texi \
	libidn-api-punycode.texi libidn-api-idna.texi

%.c.texi: %.c
	sed -e 's/{/@{/g' -e 's/}/@}/g' < $< > $@

libidn-api-version.texi: version.c
	$(PERL) $(srcdir)/gdoc -texinfo $^ > $@

libidn-api-stringprep.texi: stringprep.c nfkc.c toutf8.c reference/dummy.c
	$(PERL) $(srcdir)/gdoc -texinfo $^ > $@

libidn-api-punycode.texi: punycode.c
	$(PERL) $(srcdir)/gdoc -texinfo $^ > $@

libidn-api-idna.texi: idna.c
	$(PERL) $(srcdir)/gdoc -texinfo $^ > $@

# Maintainer targets

.PHONY: ChangeLog
ChangeLog:
	test ! -f .cvsusers || \
	cvs2cl --fsf --usermap .cvsusers --no-times --no-common-dir --prune \
	-I "ChangeLog|.cvsignore|.cvsusers|autogen.sh"

indent:
	indent $(SOURCES)

htmldir = ../www-$(PACKAGE)
tag = $(PACKAGE)-`echo $(VERSION) | sed 's/\./-/g'`

release:
	cvs upd -p -r HEAD NEWS > /dev/null
	if cvs upd -p -r $(tag) NEWS > /dev/null; then false else true; fi
	rm -f ChangeLog && cvs upd ChangeLog
	cvs commit
	$(MAKE) distcheck
	cvs commit -m "" ChangeLog
	rm -f $(distdir).tar.gz.asc
	gpg -a --sign --detach $(distdir).tar.gz
	gpg --verify $(distdir).tar.gz.asc
	scp $(distdir).tar.gz* gnudist.gnu.org:~alpha/gnu/$(PACKAGE)/
	test -d $(htmldir) && cp -v $(PACKAGE).{ps,pdf,html} $(htmldir)/
	test -d $(htmldir)/reference && \
		cp -v reference/*.html $(htmldir)/reference/
	test -d $(htmldir) && cd $(htmldir) && cvs commit -m "Update." \
		$(PACKAGE).{ps,pdf,html} reference/*.html
	cvs rtag $(tag) $(PACKAGE)
