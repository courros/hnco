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

use JSON;

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan) || die "hnco-runtime2-skeleton.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}
my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $parallel            = $obj->{parallel};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $servers             = $obj->{servers};

foreach ($parameter1, $parameter2) {
    if ($_->{values_perl}) {
        my @tmp = eval $_->{values_perl};
        $_->{values} = \@tmp;
    }
}

my $path_results = "results";
unless (-d $path_results) {
    mkdir $path_results;
    print "Created $path_results\n";
}
results_iterate_algorithms($path_results);

foreach ("stats", "graphics") {
    unless (-d $_) {
        mkdir $_;
        print "Created $_\n";
    }
    iterate_algorithms($_);
}

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
    foreach (@{ $parameter1->{values} }) {
        my $path = "$prefix/$parameter1->{id}-$_";
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
    foreach (@{ $parameter2->{values} }) {
        my $path = "$prefix/$parameter2->{id}-$_";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
    }
}

sub iterate_algorithms
{
    my ($prefix) = @_;
    foreach (@$algorithms) {
        my $path = "$prefix/$_->{id}";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
    }
}
