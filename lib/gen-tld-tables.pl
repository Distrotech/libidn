#!/usr/bin/perl

# Author: Thomas Jacob, Internet24.de

# Copyright (C) 2004  Simon Josefsson.
# Copyright (C) 2004  Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.

# Use this to generate custom TLD restriction structures as well
#  like follows "./gen-tld-tables TLD definition-file"

# I consider the output of this program to be unrestricted.  Use it as
# you will.
#

use strict;
use warnings;

die "Usage: $0 <TLD-FILE [TLD-FILE ...]>" if ($#ARGV == -1);

print "/* This file is automatically generated.  DO NOT EDIT!\n";
print "   Instead, edit gen-tld-tables.pl and re-run.  */\n";
print "\n";
print "#include \"tld.h\"\n";
print "\n";

my @tlds;

foreach my $tldfile (@ARGV)
{
    my ($tld, $version);
    my $data = process_definition ($tldfile, \$tld, \$version);

    push @tlds, $tld;

    optimize($data);

    print_tld_domain($tld, \@$data, $version);
}

print "/* Main array */\n";
print "const Tld_table * tld_tables[] =\n";
print "{\n";
foreach (@tlds)
{
    print "  &_tld_${_},\n";
}
print "  NULL\n";
print "};\n";

# Print tld domain structure.
# void print_tld_domain($tldfile, \$tld, \@intervals, $version);
sub print_tld_domain
{
    my $tld = shift;
    my $intervals = shift;
    my $version = shift;
    my $num_intervals = @$intervals;

    print "/* TLD $tld */\n";
    print "const Tld_table_element _tld_${tld}_valid[] =\n";
    print "{\n";
    if (@$intervals)
    {
	my $last = pop @$intervals;
	foreach (@$intervals)
	{
	    print "  { $_->[0],  $_->[1] },\n";
	}
	print "  { $last->[0],  $last->[1] }\n";
    }
    print "};\n";
    print "\n";

    print "const Tld_table _tld_${tld} =\n";
    print "{\n";
    print "  \"$tld\",\n";
    print "  \"$version\",\n";
    print "  $num_intervals,\n";
    print "  &_tld_${tld}_valid\n";
    print "};\n\n";
}

# process a definition file
# $filename, \$tld, \$version
# return @data on success, undef otherwise
sub process_definition
{
    my $filename = shift;
    my $tld = shift;
    my $version = shift;
    my $incversion;
    my @data;

    local * FILE;

    open(FILE, "<$filename") or die "Cannot open $filename";

    my ($is_int,$have_num,$num,$cnum);
    my $line = 1;
    while(<FILE>)
    {
	chomp;

	if (m/^#include\s+(\S+)\s*$/i)
	{
	    my $incfile = $1;
	    my ($junk, $ver);
	    my $incdata = process_definition($incfile, \$junk, \$ver);
	    $incversion = $incversion . " $incfile ($ver)";
	    push @data, @$incdata;
	    next;
	}

	s/#.*$//;

	if (m/^version "(.*)"/)
	{
	    $$version = $1;
	    next;
	}

	if (m/^tld (.*)/)
	{
	    $$tld = $1;
	    next;
	}

	while ($_ ne "")
	{
	    s/^\s*//;
	    if ( (s/^(0x)([a-f0-9]+)//i) ||
		 (s/^(0)(\d+)//) ||
		 (s/^(\d+)//) )
	    {
		$cnum = $1;
		if ((lc($1) eq "0x") && $2)
		{
		    $cnum = hex($2);
		}
		elsif (($1 eq "0") && $2)
		{
		    $cnum = oct($2);
		}

		if ($have_num)
		{
		    if ($is_int)
		    {
			push @data, [$num, $cnum];
			$have_num = 0;
			$is_int = 0;
		    }
		    else
		    {
			push @data, [$num,$num];
			$num = $cnum;
		    }
		}
		else
		{
		    $have_num = 1;
		    $num = $cnum;
		}
	    }
	    elsif (s/^\-//)
	    {
		$is_int = 1;
	    }
	    else
	    {
		die "Parser error in file $filename  at line $line near $_, "
		    if $_ ne "";
	    }
	}
	$line++;
    }
    close(FILE);

    if ($have_num)
    {
	if ($is_int)
	{
	    push @data, [$num, $cnum];
	}
	else
	{
	    push @data, [$num, $num];
	}
    }

    $$version = $$version . $incversion if $incversion;
    return \@data;
}

# Build minimal set of intervals.
# void optimize(\@intervals)
sub optimize
{
    my $intervals = shift;

    return undef unless (@$intervals);

    my @intervals = sort {$a->[0] <=> $b->[0]} @$intervals;
    @$intervals = ();

    my $cur_int = shift @intervals;

    foreach (@intervals)
    {
	if ($_->[0]>($cur_int->[1]+1))
	{
	    push @$intervals, $cur_int;
	    $cur_int = $_;
	}
	else
	{
	    if ($_->[1] > $cur_int->[1])
	    {
		$cur_int->[1] = $_->[1];
	    }
	}
    }
    push @$intervals, $cur_int;
}
