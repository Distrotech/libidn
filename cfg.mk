# Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011 Simon Josefsson
#
# This file is part of GNU Libidn.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

WFLAGS ?= --enable-gcc-warnings
ADDFLAGS ?=
CFGFLAGS ?= --enable-java --enable-gtk-doc --enable-gtk-doc-pdf \
	$(ADDFLAGS) $(WFLAGS)

ifeq ($(.DEFAULT_GOAL),abort-due-to-no-makefile)
.DEFAULT_GOAL := bootstrap
endif

local-checks-to-skip = sc_prohibit_strcmp sc_prohibit_have_config_h	\
	sc_require_config_h sc_require_config_h_first			\
	sc_prohibit_HAVE_MBRTOWC sc_program_name sc_trailing_blank	\
	sc_GPL_version sc_immutable_NEWS
VC_LIST_ALWAYS_EXCLUDE_REGEX = \
	^(maint.mk|gtk-doc.make|m4/pkg.m4|doc/specifications|contrib/doxygen/Doxyfile|doc/fdl-1.3.texi|csharp/libidn.*suo|(lib/)?(gl|gltests|build-aux)/)
update-copyright-env = UPDATE_COPYRIGHT_HOLDER="Simon Josefsson"

# Explicit syntax-check exceptions.
exclude_file_name_regexp--sc_bindtextdomain = ^examples/|libc/|tests/
exclude_file_name_regexp--sc_prohibit_atoi_atof = ^examples/example2.c$$
exclude_file_name_regexp--sc_copyright_check = ^doc/libidn.texi
exclude_file_name_regexp--sc_useless_cpp_parens = ^lib/nfkc.c$$

doc/Makefile.gdoc:
	printf "gdoc_MANS =\ngdoc_TEXINFOS =\n" > doc/Makefile.gdoc

autoreconf: doc/Makefile.gdoc
	for f in po/*.po.in; do \
		cp $$f `echo $$f | sed 's/.in//'`; \
	done
	mv build-aux/config.rpath build-aux/config.rpath-
	test -f ./configure || autoreconf --install
	mv build-aux/config.rpath- build-aux/config.rpath

update-po: refresh-po
	for f in `ls po/*.po | grep -v quot.po`; do \
		cp $$f $$f.in; \
	done
	git add po/*.po.in
	git commit -m "Sync with TP." po/LINGUAS po/*.po.in

bootstrap: autoreconf
	./configure $(CFGFLAGS)

# Coverage

coverage-web:
	rm -fv `find $(htmldir)/coverage -type f | grep -v CVS`
	cp -rv $(COVERAGE_OUT)/* $(htmldir)/coverage/

coverage-web-upload:
	cd $(htmldir) && \
		cvs commit -m "Update." coverage

# Release

ChangeLog:
	git2cl > ChangeLog
	cat .clcopying >> ChangeLog

tag = $(PACKAGE)-`echo $(VERSION) | sed 's/\./-/g'`
htmldir = ../www-$(PACKAGE)

release: prepare upload web upload-web

prepare:
	! git tag -l $(tag) | grep $(PACKAGE) > /dev/null
	rm -f ChangeLog
	$(MAKE) ChangeLog distcheck
	git commit -m Generated. ChangeLog
	git tag -u b565716f! -m $(VERSION) $(tag)

upload:
	git push
	git push --tags
	build-aux/gnupload --to ftp.gnu.org:$(PACKAGE) $(distdir).tar.gz
	cp $(distdir).tar.gz $(distdir).tar.gz.sig ../releases/$(PACKAGE)/

web:
	cd doc && env MAKEINFO="makeinfo -I ../examples" \
		      TEXI2DVI="texi2dvi -I ../examples" \
		../build-aux/gendocs.sh --html "--css-include=texinfo.css" \
		-o ../$(htmldir)/manual/ $(PACKAGE) "$(PACKAGE_NAME)"
	cd contrib/doxygen && doxygen && cd ../.. && cp -v contrib/doxygen/html/* $(htmldir)/doxygen/ && cd contrib/doxygen/latex && make refman.pdf && cd ../../../ && cp contrib/doxygen/latex/refman.pdf $(htmldir)/doxygen/$(PACKAGE).pdf
	cp -v doc/reference/$(PACKAGE).pdf doc/reference/html/*.html doc/reference/html/*.png doc/reference/html/*.devhelp doc/reference/html/*.css $(htmldir)/reference/
	cp -rv doc/java/* $(htmldir)/javadoc/
	cp -v doc/cyclo/cyclo-$(PACKAGE).html $(htmldir)/cyclo/

upload-web:
	cd $(htmldir) && \
		cvs commit -m "Update." \
			manual/ javadoc/ reference/ doxygen/ cyclo/

review-diff:
	git diff `git describe --abbrev=0`.. \
	| grep -v -e ^index -e '^diff --git' \
	| filterdiff -p 1 -x 'build-aux/*' -x 'gl/*' -x 'gltests/*' -x 'lib/gl/*' -x 'lib/gltests/*' -x 'po/*' -x 'maint.mk' -x '.gitignore' -x '.x-sc*' -x ChangeLog -x GNUmakefile \
	| less
