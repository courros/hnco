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
use File::Slurp qw(read_file);

#
# Global constants
#

my $path_graphics = "graphics";
my $path_results  = "results";
my $path_stats    = "stats";

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

make_path($path_results);
results_iterate_algorithms($path_results);

make_path($path_stats);
iterate_algorithms($path_stats);

make_path($path_graphics);
iterate_algorithms($path_graphics);

#
# Local functions
#

sub make_path
{
    my ($path) = @_;
    if (-d $path) {
        die qq(hnco-runtime2-skeleton.pl: make_path: "$path" already exists\n)
    } else {
        mkdir $path;
        print "Created $path\n";
    }
}

sub results_iterate_algorithms
{
    my ($prefix) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $path = "$prefix/$algorithm_id";
        make_path($path);
        results_iterate_parameter1($path);
    }
}

sub results_iterate_parameter1
{
    my ($prefix) = @_;
    foreach (@{ $parameter1->{values} }) {
        my $path = "$prefix/$parameter1->{id}-$_";
        make_path($path);
        results_iterate_parameter2($path);
    }
}

sub results_iterate_parameter2
{
    my ($prefix) = @_;
    foreach (@{ $parameter2->{values} }) {
        my $path = "$prefix/$parameter2->{id}-$_";
        make_path($path);
    }
}

sub iterate_algorithms
{
    my ($prefix) = @_;
    foreach (@$algorithms) {
        my $path = "$prefix/$_->{id}";
        make_path($path);
    }
}
