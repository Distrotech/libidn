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

DIST_SUBDIRS = doc m4 reference
SUBDIRS = . doc m4 @GTKDOC@ gl lib src tests examples

ACLOCAL_AMFLAGS = -I m4 -I gl/m4

EXTRA_DIST = FAQ libidn.pc.in \
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
	contrib/doxygen/gdoc2doxygen

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libidn.pc

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
	-cd contrib/doxygen && doxygen && cp -v html/* $(htmldir)/doxygen/
	-cp -v doc/$(PACKAGE).{ps,pdf,html} $(htmldir)/ && \
		cp -v reference/*.html $(htmldir)/reference/ && \
		cd $(htmldir) && cvs commit -m "Update." \
			$(PACKAGE).{ps,pdf,html} reference/*.html doxygen/*
	cvs rtag $(tag) $(PACKAGE)
