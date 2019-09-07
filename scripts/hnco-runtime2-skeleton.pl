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

my $algorithms          = $obj->{algorithms};
my $function            = $obj->{function};
my $parallel            = $obj->{parallel};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $servers             = $obj->{servers};

my $parameter1_id       = $parameter1->{id};
my $parameter2_id       = $parameter2->{id};

my $path_results = "results";
unless (-d $path_results) {
    mkdir $path_results;
    print "Created $path_results\n";
}
results_iterate_algorithms($path_results);

my $path_stats = "stats";
unless (-d $path_stats) {
    mkdir $path_stats;
    print "Created $path_stats\n";
}
stats_iterate_algorithms($path_stats);

sub results_iterate_algorithms
{
    my ($prefix) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $path = "$prefix/$algorithm_id";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        results_iterate_parameter1($path);
    }
}

sub results_iterate_parameter1
{
    my ($prefix) = @_;
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
        results_iterate_parameter2($path);
    }
}

sub results_iterate_parameter2
{
    my ($prefix) = @_;
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

sub stats_iterate_algorithms
{
    my ($prefix) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        foreach ("$prefix/$algorithm_id", "$prefix/$algorithm_id/$parameter1_id", "$prefix/$algorithm_id/$parameter2_id") {
            unless (-d $_) {
                mkdir "$_";
                print "Created $_\n";
            }
        }
    }
}
