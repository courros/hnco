#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

# This file is part of HNCO.

# HNCO is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# HNCO is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
# Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with HNCO. If not, see <http://www.gnu.org/licenses/>.

use strict;
use warnings;

use JSON;
use File::Slurp qw(read_file write_file);
use List::MoreUtils qw(none any);

use HNCO::Run qw(gnu_parallel);

#
# Read plan
#

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

my $obj = from_json(read_file($plan));

#
# Global variables
#

my $functions           = $obj->{functions};
my $parallel            = $obj->{parallel};
my $servers             = $obj->{servers};

my $path_results        = $obj->{results};

#
# Processing
#

my @commands = ();

iterate_functions($path_results, "$obj->{exec} $obj->{opt}");

if ($parallel) {
    write_file('commands.txt', map { "$_\n" } @commands);
    gnu_parallel($servers, $path_results, "hnco-observable-evolution-skeleton.pl");
}

#
# Local functions
#

sub iterate_functions
{
    my ($prefix, $cmd) = @_;
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        print "$function_id: ";
        run("$prefix/$function_id", "$cmd $f->{opt}");
    }
}

sub run
{
    my ($prefix, $cmd) = @_;
    if ($parallel) {
        push @commands, "$cmd > $prefix/1.out 2> $prefix/1.err";
        print "added to the job queue\n";
    } else {
        system("$cmd > $prefix/1.out 2>> log.err");
        print "done\n";
    }
}
