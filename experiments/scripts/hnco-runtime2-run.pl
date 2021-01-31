#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

use HNCO::Run qw(gnu_parallel);

#
# Global constants
#

my $path_results = "results";

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

my $algorithms          = $obj->{algorithms};
my $budget              = $obj->{budget};
my $parallel            = $obj->{parallel};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $servers             = $obj->{servers};

#
# Parameter values
#

foreach ($parameter1, $parameter2) {
    if ($_->{values_perl}) {
        my @tmp = eval $_->{values_perl};
        $_->{values} = \@tmp;
    }
}

#
# Processing
#

my @commands = ();

iterate_algorithms($path_results, "$obj->{exec} $obj->{opt} -b $budget");

if ($parallel) {
    write_file('commands.txt', map { "$_\n" } @commands);
    gnu_parallel($servers, $path_results, "hnco-runtime2-skeleton.pl");
}

#
# Local functions
#

sub iterate_algorithms
{
    my ($prefix, $cmd) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        print "$algorithm_id\n\n";
        iterate_parameter1_values("$prefix/$algorithm_id", "$cmd $a->{opt}", $a);
        print "\n";
    }
}

sub iterate_parameter1_values
{
    my ($prefix, $cmd, $a) = @_;
    foreach my $value (@{ $parameter1->{values} }) {
        print "$parameter1->{id} = $value\n\n";
        iterate_parameter2_values("$prefix/$parameter1->{id}-$value", "$cmd --$parameter1->{id} $value", $a);
        print "\n";
    }
}

sub iterate_parameter2_values
{
    my ($prefix, $cmd, $a) = @_;
    my $num_runs = $obj->{num_runs};
    if ($a->{deterministic}) {
        $num_runs = 1;
    }
    foreach my $value (@{ $parameter2->{values} }) {
        print "$parameter2->{id} = $value: ";
        iterate_runs("$prefix/$parameter2->{id}-$value", "$cmd --$parameter2->{id} $value", $num_runs);
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd, $num_runs) = @_;
    if ($parallel) {
        foreach (1 .. $num_runs) {
            push @commands, "$cmd > $prefix/$_.out 2> $prefix/$_.err";
        }
        print "added to the job queue";
    } else {
        foreach (1 .. $num_runs) {
            system("$cmd > $prefix/$_.out 2>> log.err");
            print ($? >> 8);
        }
    }
}
