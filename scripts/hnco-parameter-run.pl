#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018 Arnaud Berny

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
my $parallel            = $obj->{parallel};

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

my $commands = ();

iterate_functions($path, "$obj->{exec} $obj->{opt}");

if ($parallel) {
    my $path = 'commands.txt';
    my $file = IO::File->new($path, '>')
        or die "hnco-parameter-run.pl: Cannot open '$path': $!\n";
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
        print "$algorithm_id\n";
        iterate_values($path, "$cmd $a->{opt}", $a);
    }
}

sub iterate_values
{
    my ($prefix, $cmd, $a) = @_;

    my $num_runs = $obj->{num_runs};
    if ($a->{deterministic}) {
        $num_runs = 1;
    }

    foreach my $value (@$values) {
        my $path = "$prefix/$parameter_id-$value";
        unless (-d $path) {
            mkdir "$path";
            print "Created $path\n";
        }
        print "$parameter_id = $value: ";
        my $dep = "";
        if (exists($a->{opt_perl})) {
            foreach (@{ $a->{opt_perl} }) {
                my $value = eval $_->{value};
                $dep = "$dep $_->{opt} $value";
            }
        }
        iterate_runs($path, "$cmd --$parameter_id $value $dep", $num_runs);
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd, $num_runs) = @_;
    if ($parallel) {
        foreach (1 .. $num_runs) {
            push @commands,
                "/usr/bin/time --quiet -f \"\%e\" -o $prefix/$_.time $cmd > $prefix/$_.out 2>> $prefix/$_.err";
        }
        print "added to the job queue";
    } else {
        foreach (1 .. $num_runs) {
            system("/usr/bin/time --quiet -f \"\%e\" -o $prefix/$_.time $cmd > $prefix/$_.out 2>> log.err");
            print ".";
        }
    }
}
