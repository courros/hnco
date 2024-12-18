#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

use strict;
use warnings;
use JSON;
use Statistics::Descriptive;
use List::Util qw(min max);
use List::MoreUtils qw(all);
use File::Slurp qw(read_file);

use HNCO::Report qw(
    %terminal
    add_missing_names
    latex_graphicspath
    latex_section
    latex_subsection
    latex_subsubsection
    latex_begin_center
    latex_end_center
    latex_begin_figure
    latex_includegraphics
    latex_caption
    latex_end_figure
    );

#
# Global constants
#

my @summary_statistics = qw(min q1 median q3 max mean stddev);

#
# Read plan
#

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

my $obj = from_json(read_file($plan));

#
# Global variables
#

my $path_graphics = "graphics";
my $path_report   = "report";
my $path_results  = "results";
my $path_stats    = "stats";

my $algorithms    = $obj->{algorithms};
my $graphics      = $obj->{graphics};
my $num_runs      = $obj->{num_runs};
my $parameter1    = $obj->{parameter1};
my $parameter2    = $obj->{parameter2};

my $data = {};

#
# Parameter values
#

foreach ($parameter1, $parameter2) {
    if ($_->{values_perl}) {
        my @tmp = eval $_->{values_perl};
        $_->{values} = \@tmp;
    }
}

#
# Processing
#

add_missing_names($algorithms);
compute_statistics();
foreach (@summary_statistics) {
    generate_data($_);
}
generate_gnuplot();
generate_latex();

#
# Local functions
#

sub compute_statistics
{
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        $data->{$algorithm_id} = {};

        my $algorithm_num_runs = $num_runs;
        if ($a->{deterministic}) {
            $algorithm_num_runs = 1;
        }

        foreach my $v1 (@{ $parameter1->{values} }) {
            my $key1 = "$parameter1->{id}-$v1";
            $data->{$algorithm_id}->{$key1} = {};
            foreach my $v2 (@{ $parameter2->{values} }) {
                my $key2 = "$parameter2->{id}-$v2";
                my $prefix = "$path_results/$algorithm_id/$key1/$key2";
                my $SD = Statistics::Descriptive::Full->new();
                foreach (1 .. $algorithm_num_runs) {
                    my $path = "$prefix/$_.out";
                    if (-f $path) {
                        my $obj = from_json(read_file($path));
                        $SD->add_data($obj->{total_num_evaluations});
                    } else {
                        die "hnco-runtime2-stat.pl: compute_statistics: Cannot open '$path': $!";
                    }
                }
                $data->{$algorithm_id}->{$key1}->{$key2} = { min       => $SD->min(),
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
    my $measure = shift;

    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $prefix = "$path_stats/$algorithm_id";

        foreach my $v1 (@{ $parameter1->{values} }) {
            my $key1 = "$parameter1->{id}-$v1";
            my $path = "$prefix/$measure-$key1.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!";
            foreach my $v2 (@{ $parameter2->{values} }) {
                my $key2 = "$parameter2->{id}-$v2";
                $file->printf("%e %e\n", $v2, $data->{$algorithm_id}->{$key1}->{$key2}->{$measure});
            }
            $file->close();
        }

        foreach my $v2 (@{ $parameter2->{values} }) {
            my $key2 = "$parameter2->{id}-$v2";
            my $path = "$prefix/$measure-$key2.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!";
            foreach my $v1 (@{ $parameter1->{values} }) {
                my $key1 = "$parameter1->{id}-$v1";
                $file->printf("%e %e\n", $v1, $data->{$algorithm_id}->{$key1}->{$key2}->{$measure});
            }
            $file->close();
        }

        my $path = "$prefix/$measure.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!";
        foreach my $v1 (@{ $parameter1->{values} }) {
            my $key1 = "$parameter1->{id}-$v1";
            foreach my $v2 (@{ $parameter2->{values} }) {
                my $key2 = "$parameter2->{id}-$v2";
                $file->printf("%e %e %e\n", $v1, $v2, $data->{$algorithm_id}->{$key1}->{$key2}->{$measure});
            }
        }
        $file->close();

    }
}

sub generate_gnuplot
{
    my $path = "graphics.gp";
    open(my $fh, ">$path")
        or die "hnco-runtime2-stat.pl: generate_gnuplot: Cannot open '$path': $!";

    print $fh
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set ylabel \"Runtime\"\n",
        qq(set format y "10^{\%T}"\n),
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    if ($graphics->{all}->{logscale}) {
        print $fh
            "set logscale y 10\n",
            qq(set format y "10^{\%T}"\n);
    } else {
        print $fh
            "unset logscale y\n",
            "set format y\n";
    }
    print $fh "\n";

    foreach my $a (@$algorithms) {
        foreach my $measure (@summary_statistics) {
            generate_gnuplot_section($a, $parameter1, $parameter2, $measure, $fh);
            generate_gnuplot_section($a, $parameter2, $parameter1, $measure, $fh);
        }
    }

    close($fh);
    system("chmod a+x graphics.gp");
}

sub generate_gnuplot_section
{
    my ($a, $variable, $parameter, $measure, $fh) = @_;

    my $algorithm_id = $a->{id};
    my $prefix_stats = "$path_stats/$algorithm_id";
    my $prefix_graphics = "$path_graphics/$algorithm_id";

    # Font face and size
    my $font = "";
    if ($variable->{font_face}) {
        $font = $variable->{font_face};
    }
    if ($variable->{font_size}) {
        $font = "$font,$variable->{font_size}";
    }
    $font = qq(font "$font");

    my $key = $font;
    if ($graphics->{all}->{key}) {
        $key = "$key $graphics->{all}->{key}";
    } else {
        $key = "$key outside right vertical reverse Left top box";
    }

    print $fh
        "unset key\n",
        "set key $key\n",
        qq(set xlabel "$variable->{label}"\n);

    my $basename = "$prefix_graphics/$measure-$parameter->{id}";
    print $fh
        "$terminal{pdf} $font\n",
        qq(set output "$basename.pdf"\n);
    print $fh "plot \\\n";
    print $fh
        join ", \\\n",
        (map {
            my $key = "$parameter->{id}-$_";
            my $path = qq("$prefix_stats/$measure-$key.dat");
            my $title = qq("$parameter->{key} = $_");
            "  $path using 1:2 with l lw 2 title $title";
         } reverse(@{ $parameter->{values} }));
    print $fh "\n";

    print $fh
        "$terminal{eps} $font\n",
        qq(set output "$basename.eps"\n),
        "replot\n";

    print $fh
        "$terminal{png} $font\n",
        qq(set output "$basename.png"\n),
        "replot\n\n";
}

sub generate_latex
{
    open(my $fh, ">$path_report/results.tex")
        or die "hnco-runtime2-stat.pl: generate_latex: Cannot open '$path_report/results.t': $!";

    print $fh latex_graphicspath($path_graphics);
    foreach my $a (@$algorithms) {
        print $fh latex_section("$a->{label}");
        foreach my $p ($parameter2, $parameter1) {
            print $fh latex_subsection("$p->{label}");
            foreach my $measure (@summary_statistics) {
                print $fh latex_subsubsection("$measure");
                print $fh latex_begin_center();
                print $fh latex_includegraphics("$a->{id}/$measure-$p->{id}");
                print $fh latex_end_center();
            }
        }
    }

    close($fh);
}
