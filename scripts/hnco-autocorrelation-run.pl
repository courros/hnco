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
use File::Spec;
use File::HomeDir;
use Cwd;

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan)
    or die "hnco-benchmark-run.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}
my $obj = from_json($json);

my $path_results        = $obj->{results};
my $num_runs            = $obj->{num_runs};
my $functions           = $obj->{functions};
my $algorithms          = $obj->{algorithms};
my $parallel            = $obj->{parallel};
my $servers             = $obj->{servers};

if ($parallel) {
    if ($servers) {
        my $dir = File::Spec->abs2rel(getcwd, File::HomeDir->my_home);
        foreach (@$servers) {
            system("ssh $_->{hostname} \"cd $dir ; hnco-benchmark-skeleton.pl\"\n");
        }
    }
}

my $commands = ();

my $path = $path_results;
unless (-d $path) {
    mkdir $path;
    print "Created $path\n";
}
iterate_functions($path, "$obj->{exec} $obj->{opt}");

if ($parallel) {
    my $path = 'commands.txt';
    my $file = IO::File->new($path, '>')
        or die "hnco-benchmark-run.pl: Cannot open '$path': $!\n";
    $file->print(join("\n", @commands));
    $file->close;
    if ($servers) {
        my $hostnames = join(',', map { $_->{hostname} } @$servers);
        system("parallel --eta --progress --workdir . -S :,$hostnames :::: commands.txt");
        print "Bringing back the files:\n";
        foreach (@$servers) {
            my $src = "$_->{hostname}:" . File::Spec->abs2rel(getcwd, File::HomeDir->my_home);
            system("rsync -avvz $src/$path_results/ $path_results");
        }
    } else {
        system("parallel --eta --progress :::: commands.txt");
    }
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
        iterate_runs($path, "$cmd $f->{opt}", $obj->{num_runs});
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