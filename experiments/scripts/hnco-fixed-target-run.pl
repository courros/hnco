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
use List::MoreUtils qw(none any);

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
my $only_algorithms     = $obj->{only_algorithms};
my $skip_algorithms     = $obj->{skip_algorithms};
my $budget              = $obj->{budget};
my $functions           = $obj->{functions};
my $parallel            = $obj->{parallel};
my $servers             = $obj->{servers};
my $save_solution       = $obj->{save_solution};
my $num_runs            = $obj->{num_runs};

#
# Processing
#

my @commands = ();

iterate_functions($path_results, "$obj->{exec} $obj->{opt} -b $budget");

if ($parallel) {
    write_file('commands.txt', map { "$_\n" } @commands);
    gnu_parallel($servers, $path_results, "hnco-benchmark-skeleton.pl");
}

#
# Local functions
#

sub iterate_functions
{
    my ($prefix, $cmd) = @_;
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        print "$function_id\n\n";
        iterate_algorithms("$prefix/$function_id", "$cmd $f->{opt}");
        print "\n";
    }
}

sub iterate_algorithms
{
    my ($prefix, $cmd) = @_;
    foreach my $algorithm (@$algorithms) {
        my $id = $algorithm->{id};
        if ($only_algorithms &&
            none { $_ eq $id } @$only_algorithms) { next; }
        if ($skip_algorithms &&
            any { $_ eq $id } @$skip_algorithms) { next; }
        print "$id: ";
        iterate_runs("$prefix/$id", "$cmd $algorithm->{opt}");
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd) = @_;
    if ($parallel) {
        foreach (1 .. $num_runs) {
            if ($save_solution) {
                push @commands, "$cmd --save-solution --solution-path $prefix/$_.sol --save-description --description-path $prefix/$_.des > $prefix/$_.out 2> $prefix/$_.err";
            } else {
                push @commands, "$cmd > $prefix/$_.out 2> $prefix/$_.err";
            }
        }
        print "added to the job queue";
    } else {
        foreach (1 .. $num_runs) {
            if ($save_solution) {
                system("$cmd --save-solution --solution-path $prefix/$_.sol --save-description --description-path $prefix/$_.des > $prefix/$_.out 2>> log.err");
            } else {
                system("$cmd > $prefix/$_.out 2>> log.err");
            }
            print ($? >> 8);
        }
    }
}
