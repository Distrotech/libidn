## Process this file with automake to produce Makefile.in
# Copyright (C) 2003 Simon Josefsson.
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

EXTRA_DIST = dummy.c libidn-docs.tmpl $(HTML_OBJECTS) $(IMAGES)

HTML_OBJECTS = index.html c4.html \
	libidn-idna.html libidn-punycode.html libidn-stringprep.html
IMAGES = home.png left.png right.png up.png

MODULE = libidn
HIGNORE = config.h gunibreak.h gunicomp.h gunidecomp.h idn-int.h \
	netdb.h getopt.h internal.h stringprep_*.h idn_cmd.h

scan.stamp: ../lib/*.c ../lib/*.h
	$(GTKDOC_SCAN) --module=$(MODULE) --source-dir=$(top_srcdir) \
		--ignore-headers="$(HIGNORE)"
	touch scan.stamp

tmpl.stamp: scan.stamp
	$(GTKDOC_MKTMPL) --module=$(MODULE) --output-dir=.

sgml.stamp: tmpl.stamp
	$(GTKDOC_MKDB) --module=$(MODULE) --source-dir=$(top_srcdir) \
		--tmpl-dir=. --output-dir=. --main-sgml-file=$(srcdir)/$(MODULE)-docs.tmpl

index.html: sgml.stamp
	cp $(srcdir)/$(MODULE)-docs.tmpl $(MODULE)-docs.sgml
	$(GTKDOC_MKHTML) $(MODULE) $(MODULE)-docs.sgml
	rm -f $(MODULE)-docs.sgml ../html.stamp
	$(GTKDOC_FIXXREF) --module-dir=. --html-dir=$(HTML_DIR)

all: index.html

clean-local:
	rm -f *.bak scan.stamp tmpl.stamp sgml.stamp *.txt *.sgml \
		$(MODULE)-doc.bottom $(MODULE)-doc.top \
		$(HTML_OBJECTS) $(IMAGES) 
