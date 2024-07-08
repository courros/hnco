#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

my $algorithms = $obj->{algorithms};
my $budget     = $obj->{budget};
my $num_runs   = $obj->{num_runs};
my $parallel   = $obj->{parallel};
my $parameter1 = $obj->{parameter1};
my $parameter2 = $obj->{parameter2};
my $servers    = $obj->{servers};

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
    foreach my $algorithm (@$algorithms) {
        my $id = $algorithm->{id};
        print "$id\n\n";
        iterate_values("$prefix/$id", "$cmd $algorithm->{opt}");
        print "\n";
    }
}

sub iterate_values
{
    my ($prefix, $cmd) = @_;
    my $id1 = $parameter1->{id};
    my $id2 = $parameter2->{id};
    foreach my $value1 (@{ $parameter1->{values} }) {
        print "$id1 = $value1\n\n";
        foreach my $value2 (@{ $parameter2->{values} }) {
            print "$id2 = $value2: ";
            my $v1 = $value1;
            if (exists $parameter1->{expression}) {
                $v1 = eval  $parameter1->{expression};
                print "with $id1 = $v1: ";
            }
            iterate_runs("$prefix/$id1-$value1/$id2-$value2", "$cmd --$id1 $v1 --$id2 $value2");
            print "\n";
        }
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd) = @_;
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
