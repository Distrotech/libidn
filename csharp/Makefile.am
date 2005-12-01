## Process this file with automake to produce Makefile.in
# Copyright (C) 2005 Simon Josefsson.
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.

CSHARPCOMP = $(SHELL) ./csharpcomp.sh
CSHARPCOMPFLAGS = -O -g

SOURCES = AssemblyInfo.cs IDNA.cs RFC3454.cs CombiningClass.cs	\
	IDNAException.cs Stringprep.cs Composition.cs NFKC.cs	\
	StringprepException.cs DecompositionKeys.cs Punycode.cs	\
	DecompositionMappings.cs PunycodeException.cs

EXTRA_DIST = $(SOURCES)

CLEANFILES = Libidn.dll

all-local: Libidn.dll

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) Libidn.dll $(DESTDIR)$(libdir)/Libidn.dll

installdirs-local:
	$(mkinstalldirs) $(DESTDIR)$(libdir)

uninstall-local:
	rm -f $(DESTDIR)$(libdir)/Libidn.dll

Libidn.dll: $(SOURCES)
	$(CSHARPCOMP) $(CSHARPCOMPFLAGS) -o $@ \
		`for src in $(SOURCES); do echo $(srcdir)/$$src; done`
