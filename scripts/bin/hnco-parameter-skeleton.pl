#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

my $functions           = $obj->{functions};
my $algorithms          = $obj->{algorithms};
my $parameter           = $obj->{parameter};
my $parallel            = $obj->{parallel};
my $servers             = $obj->{servers};

#
# Processing
#

unless (-d $path_results) {
    mkdir $path_results;
    print "Created $path_results\n";
}

iterate_functions($path_results);

#
# Local functions
#

sub iterate_functions
{
    my ($prefix) = @_;
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$prefix/$function_id";
        unless (-d $path) {
            mkdir $path;
            print "Created $path\n";
        }
        iterate_algorithms($path);
    }
}

sub iterate_algorithms
{
    my ($prefix) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $path = "$prefix/$algorithm_id";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        iterate_values($path);
    }
}

sub iterate_values
{
    my ($prefix) = @_;

    my $parameter_id = $parameter->{id};

    my $values;
    if ($parameter->{values_perl}) {
        my @tmp = eval $parameter->{values_perl};
        $values = \@tmp;
    } else {
        $values = $parameter->{values};
    }

    foreach my $value (@$values) {
        my $path = "$prefix/$parameter_id-$value";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
    }
}
