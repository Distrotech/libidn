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

DIST_SUBDIRS = doc reference
SUBDIRS = . doc @GTKDOC@

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = FAQ libidn.pc.in \
	m4/iconv.m4 m4/ax_create_stdint_h.m4 m4/ac_compile_check_sizeof.m4 \
	gen-stringprep-tables.pl rfc3454.txt \
	gen-unicode-tables.pl UnicodeData-3.2.0.txt LineBreak-3.2.0.txt \
	SpecialCasing-3.2.0.txt CaseFolding-3.2.0.txt \
	CompositionExclusions-3.2.0.txt \
	libc/README libc/Makefile libc/Versions libc/configure \
	libc/example.c libc/getaddrinfo.c libc/libc.patch libc/netdb.h \
	contrib/README contrib/idn-python/README contrib/idn-python/Makefile \
	contrib/idn-python/idn.c contrib/idn-python/test.py \
	contrib/web/idn.php \
	contrib/java/README contrib/java/autogen.sh \
	contrib/java/configure.ac contrib/java/Makefile.am \
	contrib/java/ExampleIDNA.java contrib/java/IDNA.java \
	contrib/java/IDNA.c \
	contrib/doxygen/Doxyfile.in contrib/doxygen/Doxyfile.orig \
	contrib/doxygen/gdoc2doxygen \
	getopt.h getopt.c getopt1.c strdup.c memset.c

MAINTAINERCLEANFILES = idn_cmd.c idn_cmd.h
DISTCLEANFILES = idn-int.h

# Library

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libidn.pc

lib_LTLIBRARIES = libidn.la

include_HEADERS = stringprep.h stringprep_generic.h stringprep_nameprep.h \
	stringprep_kerberos5.h stringprep_xmpp.h stringprep_plain.h \
	stringprep_iscsi.h idna.h punycode.h idn-int.h

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

idn.c: idn_cmd.c idn_cmd.h

idn_cmd.c idn_cmd.h: idn.ggo
	-gengetopt --input $^ --file-name $(srcdir)/idn_cmd

# Test suite

tests = tst_stringprep tst_punycode tst_idna tst_nfkc

check_PROGRAMS = $(tests)

TESTS = $(tests)

INCLUDES = -I$(top_srcdir)
LDADD = libidn.la

noinst_PROGRAMS = $(tests) example example2 example3 example4

# Lisp interface

dist_lisp_LISP = punycode.el idna.el
ELCFILES=
elc-stamp:

# Maintainer targets

ChangeLog:
	test ! -f .cvsusers || \
	(cvs2cl --FSF --usermap .cvsusers \
	-I "ChangeLog|.cvs|autogen.sh|contrib|draft.*xml" && \
	cat .cvscopying >> ChangeLog)

indent:
	indent $(SOURCES)

htmldir = ../www-$(PACKAGE)
tag = $(PACKAGE)-`echo $(VERSION) | sed 's/\./-/g'`

release:
	cvs upd -p -r HEAD NEWS > /dev/null
	if cvs upd -p -r $(tag) NEWS > /dev/null; then false else true; fi
	rm -f ChangeLog && cvs upd ChangeLog
	cvs commit
	rm -f ChangeLog
	$(MAKE) ChangeLog distcheck
	cvs commit -m "" ChangeLog
	rm -f $(distdir).tar.gz.asc
	gpg -a --sign --detach $(distdir).tar.gz
	gpg --verify $(distdir).tar.gz.asc
	scp $(distdir).tar.gz* gnudist.gnu.org:~alpha/gnu/$(PACKAGE)/
	test -d $(htmldir) && cp -v doc/$(PACKAGE).{ps,pdf,html} $(htmldir)/
	test -d $(htmldir)/reference && \
		cp -v reference/*.html $(htmldir)/reference/
	test -d $(htmldir)/doxygen && test -d contrib/doxygen/html && \
		cp -v contrib/doxygen/html/* $(htmldir)/doxygen/
	test -d $(htmldir) && cd $(htmldir) && cvs commit -m "Update." \
		$(PACKAGE).{ps,pdf,html} reference/*.html
	cvs rtag $(tag) $(PACKAGE)
