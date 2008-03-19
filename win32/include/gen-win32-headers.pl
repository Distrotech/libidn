#!/usr/bin/perl -w

# Copyright (C) 2008  Free Software Foundation, Inc.
#
# Author: Adam Strzelecki <ono@java.pl>.
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

# This script generates win32 (Visual Studio) include files for libidn
# using configure.ac.
# Usage:
#  gen-win32-headers.pl configure.ac > config.h
#  gen-win32-headers.pl configure.ac stringprep.h.in > stringprep.h

use strict;

my $version = "";
my $package = "";
my $upackage = "";

if(@ARGV < 1) {
	print "#error \"gen-win32-headers.pl requires at least one parameter.\"\n";
	exit;
}

if(!(open CONFIG, "<$ARGV[0]")) {
	print "#error \"Cannot open $ARGV[0] for version information.\"\n";
	exit;
};
while(<CONFIG>) {
	if(m/AC_INIT\s*\(\s*\[([^\]]+)\]\s*,\s*\[([^\]]+)\]\s*,\s*\[([^\]]+)\]\s*\)/gi) {
		$package = $1;
		$version = $2;
		$package =~ s/^GNU\s//;
		$upackage = uc($package);
		last;
	}
}
close CONFIG;

if($version eq "") {
	print "#error \"Cannot find version information in $ARGV[0]\"\n";
	exit;
}

shift @ARGV;

if(@ARGV == 0) {
	print <<CONFIG;
#ifndef _CONFIG_H
#define _CONFIG_H

#define PACKAGE "$package"
#define PACKAGE_VERSION "$version"

#define strcasecmp stricmp
#define strncasecmp strnicmp

#define LOCALEDIR "."

#if _MSC_VER && !__cplusplus
# define inline __inline
#endif

#endif /* _CONFIG_H */
CONFIG
	exit;
}

while(<>) {
	if(! m/^extern "C"/gi) {
		if(s/\@PACKAGE_VERSION@/$version/gi) {
			print;
			print <<EXPORTS;

/* ${package} Windows DLL */
#ifndef ${upackage}_API
# if defined(_WIN32) && !defined(${upackage}_STATIC)
#  ifdef ${upackage}_EXPORTS
#   define ${upackage}_API  __declspec(dllexport)
#  else /* ${upackage}_EXPORTS */
#   define ${upackage}_API  __declspec(dllimport)
#  endif /* ${upackage}_EXPORTS */
# else /* _WIN32 && !${upackage}_STATIC */
#  define ${upackage}_API extern
# endif /* _WIN32 && !${upackage}_STATIC */
#endif /* ${upackage}_API */
EXPORTS
			next;
		}
		s/^(\s*)extern(\s*IDN_DLL_VAR)?/$1${upackage}_API/i;
	}
	print;
}
