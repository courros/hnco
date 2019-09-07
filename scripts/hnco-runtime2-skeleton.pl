#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

use JSON;

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan) || die "hnco-parameter-run: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}
my $obj = from_json($json);

my $function            = $obj->{function};
my $algorithms          = $obj->{algorithms};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $parallel            = $obj->{parallel};
my $servers             = $obj->{servers};

my $path_results        = "results";

unless (-d $path_results) {
    mkdir $path_results;
    print "Created $path_results\n";
}

iterate_algorithms($path_results);

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
        iterate_parameter1($path);
    }
}

sub iterate_parameter1
{
    my ($prefix) = @_;
    my $parameter1_id = $parameter1->{id};
    my $values;
    if ($parameter1->{values_perl}) {
        my @tmp = eval $parameter1->{values_perl};
        $values = \@tmp;
    } else {
        $values = $parameter1->{values};
    }
    foreach my $value (@$values) {
        my $path = "$prefix/$parameter1_id-$value";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        iterate_parameter2($path);
    }
}

sub iterate_parameter2
{
    my ($prefix) = @_;
    my $parameter2_id = $parameter2->{id};
    my $values;
    if ($parameter2->{values_perl}) {
        my @tmp = eval $parameter2->{values_perl};
        $values = \@tmp;
    } else {
        $values = $parameter2->{values};
    }
    foreach my $value (@$values) {
        my $path = "$prefix/$parameter2_id-$value";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
    }
}
