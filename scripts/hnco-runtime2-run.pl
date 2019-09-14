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
use File::Spec;
use File::HomeDir;
use Cwd;

my $path_results        = "results";

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan)
    or die "hnco-runtime2-run: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $budget              = $obj->{budget};
my $parallel            = $obj->{parallel};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $servers             = $obj->{servers};

my $parameter1_id       = $parameter1->{id};
my $parameter2_id       = $parameter2->{id};

my $parameter1_values;
if ($parameter1->{values_perl}) {
    my @tmp = eval $parameter1->{values_perl};
    $parameter1_values = \@tmp;
} else {
    $parameter1_values = $parameter1->{values};
}

my $parameter2_values;
if ($parameter2->{values_perl}) {
    my @tmp = eval $parameter2->{values_perl};
    $parameter2_values = \@tmp;
} else {
    $parameter2_values = $parameter2->{values};
}

if ($parallel) {
    if ($servers) {
        my $dir = File::Spec->abs2rel(getcwd, File::HomeDir->my_home);
        foreach (@$servers) {
            system("ssh $_->{hostname} \"cd $dir ; hnco-parameter-skeleton.pl\"\n");
        }
    }
}

my $commands = ();

iterate_algorithms($path_results, "$obj->{exec} $obj->{opt} -b $budget");

if ($parallel) {
    my $path = 'commands.txt';
    my $file = IO::File->new($path, '>')
        or die "hnco-parameter-run.pl: Cannot open '$path': $!\n";
    $file->print(join("\n", @commands));
    $file->close;
    if ($servers) {
        my $hostnames = join(',', map { $_->{hostname} } @$servers);
        system("parallel --joblog log.parallel --eta --progress --workdir . -S :,$hostnames :::: commands.txt");
        print "Bringing back the files:\n";
        foreach (@$servers) {
            my $src = "$_->{hostname}:" . File::Spec->abs2rel(getcwd, File::HomeDir->my_home);
            system("rsync -avvz $src/$path_results/ $path_results");
        }
    } else {
        system("parallel --joblog log.parallel --eta --progress :::: commands.txt");
    }
}

sub iterate_algorithms
{
    my ($prefix, $cmd) = @_;
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        print "$algorithm_id\n\n";
        iterate_parameter1_values("$prefix/$algorithm_id", "$cmd $a->{opt}", $a);
        print "\n";
    }
}

sub iterate_parameter1_values
{
    my ($prefix, $cmd, $a) = @_;
    foreach my $value (@$parameter1_values) {
        print "$parameter1_id = $value\n\n";
        iterate_parameter2_values("$prefix/$parameter1_id-$value", "$cmd --$parameter1_id $value", $a);
        print "\n";
    }
}

sub iterate_parameter2_values
{
    my ($prefix, $cmd, $a) = @_;
    my $num_runs = $obj->{num_runs};
    if ($a->{deterministic}) {
        $num_runs = 1;
    }
    foreach my $value (@$parameter2_values) {
        print "$parameter2_id = $value: ";
        iterate_runs("$prefix/$parameter2_id-$value", "$cmd --$parameter2_id $value", $num_runs);
        print "\n";
    }
}

sub iterate_runs
{
    my ($prefix, $cmd, $num_runs) = @_;
    if ($parallel) {
        foreach (1 .. $num_runs) {
            push @commands, "$cmd > $prefix/$_.out 2> $prefix/$_.err";
        }
        print "added to the job queue";
    } else {
        foreach (1 .. $num_runs) {
            system("$cmd > $prefix/$_.out 2>> log.err");
            print ($? >> 8);
        }
    }
}
