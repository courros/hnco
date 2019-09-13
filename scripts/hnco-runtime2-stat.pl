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
use Statistics::Descriptive;
use List::Util qw(min max);
use List::MoreUtils qw(all);

my @summary_statistics = qw(min q1 median q3 max);
my @pref_max = qw(median q1 q3 min max);
my @pref_min = qw(median q3 q1 max min);

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan)
    or die "hnco-runtime2-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) { $json .= $_; }

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $budget              = $obj->{budget};
my $function            = $obj->{function};
my $num_runs            = $obj->{num_runs};
my $parallel            = $obj->{parallel};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $servers             = $obj->{servers};

my $parameter1_id       = $parameter1->{id};
my $boxwidth1           = $parameter1->{boxwidth};

my $parameter2_id       = $parameter2->{id};
my $boxwidth2           = $parameter2->{boxwidth};

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

my $path_graphics       = "graphics";
my $path_report         = "report";
my $path_results        = "results";
my $path_stats          = "stats";

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

my $data = {};

add_missing_names($algorithms);
compute_statistics();
generate_data();
generate_latex();

sub add_missing_names
{
    my $list = shift;
    foreach my $item (@$list) {
        if (!exists($item->{name})) {
            $item->{name} = $item->{id};
        }
    }
}

sub compute_statistics
{
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $algorithm_num_runs = $num_runs;
        if ($a->{deterministic}) {
            $algorithm_num_runs = 1;
        }

        foreach my $v1 (@$parameter1_values) {
            my $key1 = "$parameter1_id-$v1";
            $data->{$key1} = {};
            foreach my $v2 (@$parameter2_values) {
                my $key2 = "$parameter2_id-$v2";
                my $prefix = "$path_results/$algorithm_id/$key1/$key2";
                my $SD = Statistics::Descriptive::Full->new();
                foreach (1 .. $algorithm_num_runs) {
                    my $path = "$prefix/$_.out";
                    if (-f $path) {
                        my $obj = from_json(read_file($path));
                        $SD->add_data($obj->{total_num_evaluations});
                    } else {
                        die "hnco-runtime2-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                    }
                }
                $data->{$key1}->{$key2} = { min       => $SD->min(),
                                            q1        => $SD->quantile(1),
                                            median    => $SD->median(),
                                            q3        => $SD->quantile(3),
                                            max       => $SD->max(),
                                            mean      => $SD->mean(),
                                            stddev    => $SD->standard_deviation() };
            }
        }
    }
}

sub generate_data
{
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $prefix = "$path_stats/$algorithm_id";

        foreach my $v1 (@$parameter1_values) {
            my $key1 = "$parameter1_id-$v1";
            my $path = "$prefix/mean-$key1.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!\n";
            foreach my $v2 (@$parameter2_values) {
                my $key2 = "$parameter2_id-$v2";
                $file->printf("%e %e\n", $v2, $data->{$key1}->{$key2}->{mean});
            }
            $file->close();
        }

        foreach my $v2 (@$parameter2_values) {
            my $key2 = "$parameter2_id-$v2";
            my $path = "$prefix/mean-$key2.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!\n";
            foreach my $v1 (@$parameter1_values) {
                my $key1 = "$parameter1_id-$v1";
                $file->printf("%e %e\n", $v1, $data->{$key1}->{$key2}->{mean});
            }
            $file->close();
        }

        my $path = "$prefix/mean.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!\n";
        foreach my $v1 (@$parameter1_values) {
            my $key1 = "$parameter1_id-$v1";
            foreach my $v2 (@$parameter2_values) {
                my $key2 = "$parameter2_id-$v2";
                $file->printf("%e %e %e\n", $v1, $v2, $data->{$key1}->{$key2}->{mean});
            }
        }
        $file->close();

    }
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-runtime2-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        latex_section("Algorithm $a->{name}");
    }
}

sub latex_section
{
    my ($title) = @_;
    print LATEX <<EOF;
\\section{$title}
EOF
}

sub latex_begin_center
{
    print LATEX <<EOF;
\\begin{center}
EOF
}

sub latex_end_center
{
    print LATEX <<EOF;
\\end{center}
EOF
}

sub latex_begin_figure
{
    print LATEX <<EOF;
\\begin{figure}[h]
\\centering
EOF
}

sub latex_includegraphics
{
    my ($path) = @_;
    print LATEX <<EOF
\\includegraphics[width=0.6\\linewidth]{$path}
EOF
}

sub latex_caption
{
    my ($caption) = @_;
    print LATEX <<EOF;
\\caption{$caption}
EOF
}

sub latex_end_figure
{
    print LATEX <<EOF;
\\end{figure}
EOF
}

sub latex_newpage
{
    print LATEX <<EOF;
\\newpage

EOF
}

sub latex_empty_line
{
    print LATEX "\n";
}

sub quote
{
    my $s = shift;
    return "\"$s\"";
}

sub read_file
{
    my $path = shift;
    my $json;
    {
        local $/;
        open my $fh, '<', $path or die "hnco-runtime2-stat.pl: compute_statistics: Cannot open '$path': $!\n";
        $json = <$fh>;
    }
    return $json;
}
