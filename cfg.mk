# Copyright (C) 2006, 2007, 2008 Simon Josefsson.
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

CFGFLAGS ?= --enable-java --enable-gtk-doc \
	WARN_CFLAGS="-Wall -Werror"

ifeq ($(.DEFAULT_GOAL),abort-due-to-no-makefile)
.DEFAULT_GOAL := bootstrap
endif

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
	git-add po/*.po.in
	git-commit -m "Sync with TP." po/LINGUAS po/*.po.in

bootstrap: autoreconf
	./configure $(CFGFLAGS)

web-coverage:
	rm -fv `find $(htmldir)/coverage -type f | grep -v CVS`
	cp -rv $(COVERAGE_OUT)/* $(htmldir)/coverage/

upload-web-coverage:
	cd $(htmldir) && \
		cvs commit -m "Update." coverage

W32ROOT ?= $(HOME)/gnutls4win/inst

mingw32: autoreconf 
	./configure --enable-gtk-doc --host=i586-mingw32msvc --build=`build-aux/config.guess` --prefix=$(W32ROOT)

ChangeLog:
	git2cl > ChangeLog
	cat .clcopying >> ChangeLog

tag = $(PACKAGE)-`echo $(VERSION) | sed 's/\./-/g'`
htmldir = ../www-$(PACKAGE)

release: prepare upload web upload-web

prepare:
	! git-tag -l $(tag) | grep $(PACKAGE) > /dev/null
	rm -f ChangeLog
	$(MAKE) ChangeLog distcheck
	git commit -m Generated. ChangeLog
	git-tag -u b565716f! -m $(VERSION) $(tag)

upload:
	git-push
	git-push --tags
	build-aux/gnupload --to ftp.gnu.org:$(PACKAGE) --to alpha.gnu.org:$(PACKAGE) $(distdir).tar.gz
	cp $(distdir).tar.gz $(distdir).tar.gz.sig ../releases/$(PACKAGE)/

web:
	cd doc && env MAKEINFO="makeinfo -I ../examples" \
		      TEXI2DVI="texi2dvi -I ../examples" \
		../build-aux/gendocs.sh --html "--css-include=texinfo.css" \
		-o ../$(htmldir)/manual/ $(PACKAGE) "$(PACKAGE_NAME)"
	cd contrib/doxygen && doxygen && cd ../.. && cp -v contrib/doxygen/html/* $(htmldir)/doxygen/ && cd contrib/doxygen/latex && make refman.pdf && cd ../../../ && cp contrib/doxygen/latex/refman.pdf $(htmldir)/doxygen/$(PACKAGE).pdf
	cp -v doc/reference/html/*.html doc/reference/html/*.png doc/reference/html/*.devhelp doc/reference/html/*.css $(htmldir)/reference/
	cp -rv doc/java/* $(htmldir)/javadoc/

upload-web:
	cd $(htmldir) && \
		cvs commit -m "Update." manual/ javadoc/ reference/ doxygen/
