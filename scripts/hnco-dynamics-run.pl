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

open(FILE, $plan) || die "hnco-dynamics-run.pl: Cannot open $plan\n";

my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);

my $path_results        = $obj->{results};
my $functions           = $obj->{functions};
my $algorithms          = $obj->{algorithms};

unless (-d $path_results) {
    mkdir $path_results;
    print "Created $path_results\n";
}

foreach my $f (@$functions) {
    my $function_id = $f->{id};

    print "$function_id:\n";

    unless (-d "$path_results/$function_id") {
        mkdir "$path_results/$function_id";
        print "Created $path_results/$function_id\n";
    }

    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};

        print "$algorithm_id... ";

        my $exec = $obj->{exec};
        if (exists($a->{exec})) {
            $exec = $a->{exec};
        }
        my $command = $exec . " " . $obj->{opt} . " " . $f->{opt} . " " . $a->{opt};
        system("$command > $path_results/$function_id/$algorithm_id.log");

        print "done\n";
    }

    print "\n";
}
