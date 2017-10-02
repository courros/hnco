#! /usr/bin/perl -w

# Copyright (C) 2016, 2017 Arnaud Berny

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

my $functions           = $obj->{functions};
my $algorithms          = $obj->{algorithms};
my $parameter           = $obj->{parameter};
my $parameter_id        = $parameter->{id};

my $values;
if ($parameter->{values_perl}) {
    my @tmp = eval $parameter->{values_perl};
    $values = \@tmp;
} else {
    $values = $parameter->{values};
}

my $path = $obj->{results};
unless (-d $path) {
    mkdir $path;
    print "Created $path\n";
}
iterate_functions($path, "$obj->{exec} $obj->{opt}");

sub iterate_functions
{
    my ($prefix, $cmd) = @_;
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$prefix/$function_id";
        unless (-d $path) {
            mkdir $path;
            print "Created $path\n";
        }
        print "$function_id\n";
        iterate_algorithms($path, "$cmd $f->{opt}");
    }
}

sub iterate_algorithms
{
    my ($prefix, $cmd) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $path = "$prefix/$algorithm_id";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        print "$algorithm_id\n";
        if ($a->{deterministic}) {
            iterate_values($path, "$cmd $a->{opt}", 1);
        } else {
            iterate_values($path, "$cmd $a->{opt}", $obj->{num_runs});
        }
    }
}

sub iterate_values
{
    my ($prefix, $cmd, $num_runs) = @_;
    foreach my $value (@$values) {
        my $path = "$prefix/$parameter_id-$value";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        print "$parameter_id = $value: ";
        iterate_runs($path, "$cmd --$parameter_id $value", $num_runs);
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd, $num_runs) = @_;
    foreach (1 .. $num_runs) {
        system("/usr/bin/time -f \"\%e\" -o $prefix/$_.time $cmd > $prefix/$_.out 2>> log.err");
        print ".";
    }
}
