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

apis = idna_to_ascii_4i.3 idna_to_unicode_44i.3 idna_to_ascii_4z.3 idna_to_ascii_8z.3 idna_to_ascii_lz.3 idna_to_unicode_4z4z.3 idna_to_unicode_8z4z.3 idna_to_unicode_8z8z.3 idna_to_unicode_8zlz.3 idna_to_unicode_lzlz.3 stringprep_utf8_to_unichar.3 stringprep_unichar_to_utf8.3 stringprep_utf8_to_ucs4.3 stringprep_ucs4_to_utf8.3 stringprep_utf8_nfkc_normalize.3 stringprep_ucs4_nfkc_normalize.3 punycode_encode.3 punycode_decode.3 stringprep.3 stringprep_profile.3 stringprep_locale_charset.3 stringprep_convert.3 stringprep_locale_to_utf8.3 stringprep_utf8_to_locale.3 stringprep_check_version.3

dist_man_MANS = idn.1 $(apis)

MAINTAINERCLEANFILES = $(dist_man_MANS)

idn.1: $(top_srcdir)/src/idn.c
	$(HELP2MAN) \
		--name="Internationalized Domain Names command line tool" \
		--output=$@ $(top_builddir)/src/idn

$(apis):
	(files=`echo $(top_srcdir)/lib/*.c`; \
	for file in $$files; do \
		apis=`$(PERL) $(top_srcdir)/doc/gdoc -listfunc $$file`; \
		for api in $$apis; do \
			echo "-- Creating man page for $$api" ; \
			$(PERL) $(top_srcdir)/doc/gdoc -module $(PACKAGE) \
				-man -function $$api $$file > $$api.3 ; \
		done; \
	done)

# Maintainer targets

.PHONY: update-api
update-api:
	(apis=`$(PERL) $(top_srcdir)/doc/gdoc -listfunc \
			$(top_srcdir)/lib/*.c | \
		sed 's/$$/.3/g'`; \
	apis=`echo $$apis`; \
	perl -pi -e "s/^apis = .*/apis = $$apis/" Makefile.am)
