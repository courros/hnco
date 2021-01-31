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
use File::Slurp qw(read_file);

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

my $functions   = $obj->{functions};
my $algorithms  = $obj->{algorithms};
my $path        = $obj->{results};

#
# Processing
#

unless (-d $path) {
    mkdir $path;
    print "Created $path\n";
}

iterate_functions($path);

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
    }
}
