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

open(FILE, $plan)
    or die "hnco-dynamics-run.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);

my $functions   = $obj->{functions};
my $algorithms  = $obj->{algorithms};
my $parallel    = $obj->{parallel};

my $path = $obj->{results};
unless (-d $path) {
    mkdir $path;
    print "Created $path\n";
}

my $commands = ();

iterate_functions($path, "$obj->{exec} $obj->{opt}");

if ($parallel) {
    my $path = 'commands.txt';
    my $file = IO::File->new($path, '>')
        or die "hnco-dynamics-run.pl: Cannot open '$path': $!\n";
    $file->print(join("\n", @commands));
    $file->close;
    system("parallel --eta --progress :::: commands.txt");
}

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
        print "$algorithm_id: ";
        iterate_runs($path, "$cmd $a->{opt}");
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd) = @_;
    if ($parallel) {
        push @commands, "$cmd > $prefix/1.out 2>> log.err";
        print "added to the list";
    } else {
        system("$cmd > $prefix/1.out 2>> log.err");
        print "done";
    }
}
