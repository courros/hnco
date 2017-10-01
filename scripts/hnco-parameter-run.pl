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

my $exec                = $obj->{exec};
my $path_results        = $obj->{results};
my $num_runs            = $obj->{num_runs};
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

unless (-d $path_results) {
    mkdir $path_results;
    print "Created $path_results\n";
}

iterate_functions();

sub iterate_functions
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        print "$function_id\n";
        my $path = "$path_results/$function_id";
        unless (-d $path) {
            mkdir $path;
            print "Created $path\n";
        }
        iterate_algorithms($path, "$obj->{opt} $f->{opt}");
    }
}

sub iterate_algorithms
{
    my ($prefix, $opt) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        print "$algorithm_id\n";
        my $path = "$prefix/$algorithm_id";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        iterate_values($path, "$opt $a->{opt}", $a);
    }
}

sub iterate_values
{
    my ($prefix, $opt, $a) = @_;
    foreach my $value (@$values) {
        print "$parameter_id = $value: ";
        my $output = "$prefix/$parameter_id-$value.dat";
        system("rm -rf $output");
        my $command = "$exec $opt --$parameter_id $value >> $output 2>> log.err";
        if ($a->{deterministic}) {
            system("$command");
            print ".";
        } else {
            foreach (1..$num_runs) {
                system("$command");
                print ".";
            }
        }
        print "\n";
    }
}
