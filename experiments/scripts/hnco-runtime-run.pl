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
my $functions           = $obj->{functions};
my $parallel            = $obj->{parallel};
my $parameter           = $obj->{parameter};
my $servers             = $obj->{servers};

my $parameter_id        = $parameter->{id};

#
# Parameter values
#

my $values;
if ($parameter->{values_perl}) {
    my @tmp = eval $parameter->{values_perl};
    $values = \@tmp;
} else {
    $values = $parameter->{values};
}

#
# Processing
#

my @commands = ();

iterate_functions($path_results, "$obj->{opt} -b $budget");

if ($parallel) {
    write_file('commands.txt', map { "$_\n" } @commands);
    gnu_parallel($servers, $path_results, "hnco-runtime-skeleton.pl");
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
    }
}

sub iterate_algorithms
{
    my ($prefix, $cmd) = @_;

    foreach my $algorithm (@$algorithms) {
        my $algorithm_cmd = "$obj->{exec} $cmd";
        if ($algorithm->{exec}) {
            $algorithm_cmd = "$algorithm->{exec} $cmd";
        }
        my $algorithm_id = $algorithm->{id};
        print "$algorithm_id\n\n";
        iterate_values("$prefix/$algorithm_id", "$algorithm_cmd $algorithm->{opt}", $algorithm);
        print "\n";
    }
}

sub iterate_values
{
    my ($prefix, $cmd, $algorithm) = @_;

    my $num_runs = $obj->{num_runs};
    if ($algorithm->{deterministic}) {
        $num_runs = 1;
    }

    foreach my $value (@$values) {
        print "$parameter_id = $value: ";

        my $dep = join " ", map {
            my $v = eval $_->{value};
            "$_->{opt} $v";
        } @{ $algorithm->{opt_perl} };

        iterate_runs("$prefix/$parameter_id-$value", "$cmd --$parameter_id $value $dep", $num_runs);
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
