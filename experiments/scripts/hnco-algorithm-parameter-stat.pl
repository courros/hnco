#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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
use POSIX;

use HNCO::Report qw(
    %terminal
    add_missing_names
    latex_graphicspath
    latex_section
    latex_begin_center
    latex_end_center
    latex_begin_figure
    latex_includegraphics
    latex_caption
    latex_end_figure
    latex_newpage
    latex_input_file
    latex_tableofcontents
    );

#
# Global constants
#

my @summary_statistics = qw(min q1 median q3 max);
my @summary_statistics_reverse = reverse @summary_statistics;

my @preference_max = qw(median q3 q1 max min);
my @preference_min = qw(median q1 q3 min max);

my $path_graphics       = "graphics";
my $path_report         = "report";
my $path_results        = "results";

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

my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $parameter           = $obj->{parameter};
my $graphics            = $obj->{graphics};

my $parameter_id        = $parameter->{id};
my $parameter_name      = $parameter->{name} || $parameter_id;
my $parameter_shortname = $parameter->{shortname} || $parameter_name;

my @results = ();
my @value_statistics = ();
my @time_statistics = ();
my @rank_statistics = ();

# hash indexed by algorithm
my $rank_data = {};

# hash indexed by function
my $best_value_statistics = {};

#
# Parameter values
#

my $values;
if ($parameter->{values_perl}) {
    my @tmp = eval $parameter->{values_perl};
    $values = \@tmp;
} else {
    $values = $parameter->{values};
}

#
# Processing
#

add_missing_names($functions);

load_results();

compute_statistics();
compute_best_value_statistics();
compute_rank_data();
compute_rank_statistics();

generate_mean_data();
generate_mean_gnuplot();
generate_stddev_gnuplot();
generate_scatter_data();
generate_scatter_gnuplot();
generate_candlesticks_gnuplot();
generate_table_rank();
generate_table_value();
generate_latex();

unless (-d "$path_graphics") { mkdir "$path_graphics"; }
foreach my $f (@$functions) {
    my $function_id = $f->{id};
    unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }
}

#
# Local functions
#

sub load_results
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        foreach my $value (@$values) {
            my $algorithm_id = "$parameter_id-$value";
            my $prefix = "$path_results/$function_id/$algorithm_id";
            foreach (1 .. $num_runs) {
                my $obj = from_json(read_file("$prefix/$_.out"));
                push @results, { function               => $function_id,
                                 algorithm              => $algorithm_id,
                                 parameter              => $value,
                                 run                    => $_,
                                 value                  => $obj->{value},
                                 num_evaluations        => $obj->{num_evaluations},
                                 total_time             => $obj->{total_time},
                                 evaluation_time        => $obj->{evaluation_time},
                                 algorithm_time         => $obj->{total_time} - $obj->{evaluation_time} };
            }
        }
    }
}

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my @rows_function = grep { $_->{function} eq $function_id } @results;
        foreach my $value (@$values) {
            my $algorithm_id = "$parameter_id-$value";
            my @rows = grep { $_->{algorithm} eq $algorithm_id } @rows_function;
            my $sd = Statistics::Descriptive::Full->new();
            $sd->add_data(map { $_->{value} } @rows);
            push @value_statistics, { function  => $function_id,
                                      algorithm => $algorithm_id,
                                      parameter => $value,
                                      mean      => $sd->mean(),
                                      stddev    => $sd->standard_deviation(),
                                      min       => $sd->min(),
                                      q1        => $sd->quantile(1),
                                      median    => $sd->median(),
                                      q3        => $sd->quantile(3),
                                      max       => $sd->max() };
        }
    }
}

sub compute_best_value_statistics
{
    foreach my $fn (@$functions) {
        my $id = $fn->{id};
        my $best = {};
        my @rows = grep { $_->{function} eq $id } @value_statistics;
        foreach my $summary (@summary_statistics) {
            $best->{$summary} = max (map { $_->{$summary} } @rows);
        }
        $best_value_statistics->{$id} = $best;
    }
}

sub compute_rank_data
{
    foreach my $value (@$values) {
        my $algorithm_id = "$parameter_id-$value";
        $rank_data->{$algorithm_id} = [];
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        # Sort algorithms by decreasing value
        my @rows = grep { $_->{function} eq $function_id } @value_statistics;
        my @sorted = sort {
            foreach (@preference_max) {
                if ($a->{$_} != $b->{$_}) {
                    return $b->{$_} <=> $a->{$_};
                }
            }
            return 0;
        } @rows;

        # Set rank
        for (my $i = 0; $i < @sorted; $i++) {
            $sorted[$i]->{rank} = $i + 1;
        }

        # Handle ex-aequo
        for (my $i = 1; $i < @sorted; $i++) {
            my $current = $sorted[$i];
            my $previous = $sorted[$i - 1];
            if (all { $current->{$_} == $previous->{$_} } @summary_statistics) {
                $current->{rank} = $previous->{rank};
            }
        }

        # Update ranks
        foreach (@sorted) {
            my $algorithm_id = $_->{algorithm};
            push @{ $rank_data->{$algorithm_id} }, $_->{rank};
        }
    }
}

sub compute_rank_statistics
{
    foreach my $value (@$values) {
        my $id = "$parameter_id-$value";
        my $sd = Statistics::Descriptive::Full->new();
        $sd->add_data(@{ $rank_data->{$id} });
        my $row = { algorithm   => $id,
                    parameter   => $value,
                    min         => $sd->min(),
                    q1          => $sd->quantile(1),
                    median      => $sd->median(),
                    q3          => $sd->quantile(3),
                    max         => $sd->max() };
        push @rank_statistics, $row;
    }
}

sub generate_mean_data
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $prefix = "$path_results/$function_id";
        my $path = "$prefix/mean.dat";
        my $mean = IO::File->new($path, '>')
            or die "hnco-algorithm-parameter-stat.pl: generate_mean_data: Cannot open '$path': $!\n";
        $path = "$prefix/quartiles.dat";
        my $quartiles = IO::File->new($path, '>')
            or die "hnco-algorithm-parameter-stat.pl: generate_mean_data: Cannot open '$path': $!\n";
        my @rows = grep { $_->{function} eq $function_id } @value_statistics;
        foreach (@rows) {
            $quartiles->printf("%e %e %e %e %e %e\n",
                               $_->{parameter},
                               $_->{min},
                               $_->{q1},
                               $_->{median},
                               $_->{q3},
                               $_->{max});
            $mean->printf("%e %e %e\n",
                          $_->{parameter},
                          $_->{mean},
                          $_->{stddev});
        }
        $quartiles->close();
        $mean->close();
    }
}

sub generate_mean_gnuplot
{
    my $path = "mean.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-parameter-stat.pl: generate_mean_gnuplot: Cannot open $path\n";

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 qq(set xlabel "$parameter_name"\n),
                 qq(set ylabel "Function value"\n),
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n");

    # Logscale
    if ($graphics->{logscale}) {
        my $fmt = qq("10^{\%T}");
        $file->print("set logscale x\n",
                     "set format x $fmt\n");
    }

    $file->print("\n");

    # Font face and size
    my $font = "";
    if ($graphics->{mean}->{font_face}) {
        $font = $graphics->{mean}->{font_face};
    }
    if ($graphics->{mean}->{font_size}) {
        $font = "$font,$graphics->{mean}->{font_size}";
    }
    $font = qq(font "$font");

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $title = qq("$function_id: Mean value as a function of $parameter_name");
        $file->print("set title $title\n");

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            $file->print("set logscale y\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale y\n",
                         "set format y\n");
        }

        $path = qq("$path_graphics/$function_id/mean.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $path\n");
        if ($f->{reverse}) {
            $file->print(qq(plot "$path_results/$function_id/mean.dat" using 1:(-\$2) with l lw 2 notitle));
        } else {
            $file->print(qq(plot "$path_results/$function_id/mean.dat" using 1:2 with l lw 2 notitle));
        }
        $file->print("\n");

        $path = qq("$path_graphics/$function_id/mean.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $path\n",
                     "replot\n");

        $path = qq("$path_graphics/$function_id/mean.png");
        $file->print("$terminal{png} $font\n",
                     "set output $path\n",
                     "replot\n\n");

    }

    $file->close();
    system("chmod a+x mean.gp");
}

sub generate_stddev_gnuplot
{
    my $path = "stddev.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-parameter-stat.pl: generate_stddev_gnuplot: Cannot open $path\n";

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 qq(set xlabel "$parameter_name"\n),
                 qq(set ylabel "Function value"\n),
                 "set autoscale fix\n",
                 qq(set format x "%.0h"\n),
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n");

    # Logscale
    if ($graphics->{logscale}) {
        my $fmt = qq("10^{\%T}");
        $file->print("set logscale x\n",
                     "set format x $fmt\n");
    }

    $file->print("\n");

    # Font face and size
    my $font = "";
    if ($graphics->{stddev}->{font_face}) {
        $font = $graphics->{stddev}->{font_face};
    }
    if ($graphics->{stddev}->{font_size}) {
        $font = "$font,$graphics->{stddev}->{font_size}";
    }
    $font = qq(font "$font");

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $title = qq("$function_id: Standard deviation of value as a function of $parameter_name");
        $file->print("set title $title\n");

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            $file->print("set logscale y\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale y\n",
                         "set format y\n");
        }

        $path = qq("$path_graphics/$function_id/stddev.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $path\n");
        $file->print(qq(plot "$path_results/$function_id/mean.dat" using 1:3 with l lw 2 notitle));
        $file->print("\n");

        $path = qq("$path_graphics/$function_id/stddev.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $path\n",
                     "replot\n");

        $path = qq("$path_graphics/$function_id/stddev.png");
        $file->print("$terminal{png} $font\n",
                     "set output $path\n",
                     "replot\n\n");

    }

    $file->close();
    system("chmod a+x stddev.gp");
}

sub generate_scatter_data
{
    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        my @rows = grep { $_->{function} eq $function_id } @results;
        foreach my $value (@$values) {
            my $algorithm_id = "$parameter_id-$value";
            my $path = "$path_results/$function_id/$algorithm_id/scatter.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-algorithm-parameter-stat.pl: generate_scatter_data: Cannot open '$path': $!\n";
            foreach (grep { $_->{algorithm} eq $algorithm_id } @rows) {
                $file->print("$_->{num_evaluations} $_->{value}\n");
            }
            $file->close();
        }
    }
}

sub generate_scatter_gnuplot
{
    my $path = "scatter.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-parameter-stat.pl: generate_scatter_gnuplot: Cannot open $path\n";
    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 "set xlabel \"Number of evaluations\"\n",
                 "set ylabel \"Function value\"\n",
                 qq(set format x "%.0h"\n),
                 "set key outside top center box opaque horizontal\n",
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n");

    # Font face and size
    my $font = "";
    if ($graphics->{scatter}->{font_face}) {
        $font = $graphics->{scatter}->{font_face};
    }
    if ($graphics->{scatter}->{font_size}) {
        $font = "$font,$graphics->{scatter}->{font_size}";
    }
    $font = qq(font "$font");

    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};

        if ($fn->{logscale}) {
            my $fmt = qq("10^{\%T}");
            $file->print("set logscale y\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale\n",
                         "set format y\n");
        }

        $path = qq("$path_graphics/$function_id/scatter.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $path\n");
        $file->print("plot \\\n");
        $file->print(join ", \\\n",
                     (map {
                         my $algorithm_id = "$parameter_id-$_";
                         $path = qq("$path_results/$function_id/$algorithm_id/scatter.dat");
                         my $title = qq("$_");
                         $fn->{reverse} ?
                             "  $path using 1:(-\$2) with points title $title" :
                             "  $path using 1:2 with points title $title";
                      } @$values));
        $file->print("\n");

        $path = qq("$path_graphics/$function_id/scatter.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $path\n",
                     "replot\n");

        $path = qq("$path_graphics/$function_id/scatter.png");
        $file->print("$terminal{png} $font\n",
                     "set output $path\n",
                     "replot\n\n");
    }

    $file->close();
    system("chmod a+x scatter.gp");
}

sub generate_candlesticks_gnuplot
{
    my $path = "candlesticks.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-parameter-stat.pl: generate_candlesticks_gnuplot: Cannot open $path\n";

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 qq(set xlabel "$parameter_name"\n),
                 qq(set ylabel "Function value"\n),
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n");

    # Logscale
    if ($graphics->{logscale}) {
        my $fmt = qq("10^{\%T}");
        $file->print("set logscale x\n",
                     "set format x $fmt\n");
    }

    $file->print("\n");

    # Font face and size
    my $font = "";
    if ($graphics->{candlesticks}->{font_face}) {
        $font = $graphics->{candlesticks}->{font_face};
    }
    if ($graphics->{candlesticks}->{font_size}) {
        $font = "$font,$graphics->{candlesticks}->{font_size}";
    }
    $font = qq(font "$font");

    # boxwidth
    my $boxwidth = 10;
    if ($graphics->{candlesticks}->{boxwidth}) {
        $boxwidth = $graphics->{candlesticks}->{boxwidth};
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            $file->print("set logscale y\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale y\n",
                         "set format y\n");
        }

        $path = qq("$path_graphics/$function_id/candlesticks.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $path\n");
        $path = qq("$path_results/$function_id/quartiles.dat");
        if ($f->{reverse}) {
            $file->print("plot $path using 1:(-\$5):(-\$6):(-\$2):(-\$3):($boxwidth) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                         "     $path using 1:(-\$4):(-\$4):(-\$4):(-\$4):($boxwidth) with candlesticks lw 2 lt 1 notitle\n");
        } else {
            $file->print("plot $path using 1:3:2:6:5:($boxwidth) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                         "     $path using 1:4:4:4:4:($boxwidth) with candlesticks lw 2 lt 1 notitle\n");
        }

        $path = qq("$path_graphics/$function_id/candlesticks.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $path\n",
                     "replot\n");

        $path = qq("$path_graphics/$function_id/candlesticks.png");
        $file->print("$terminal{png} $font\n",
                     "set output $path\n",
                     "replot\n\n");

    }

    $file->close();
    system("chmod a+x candlesticks.gp");
}

sub latex_table_end
{
    return
        "\\bottomrule\n" .
        "\\end{tabular}\n";
}

sub generate_table_rank
{
    my $path = "$path_report/table.rank.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-parameter-stat.pl: generate_table_rank: Cannot open $path\n";

    # Header
    my $width = int(ceil(log(1 + @{ $values }) / log(10)));
    my $quantiles = "";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{1}}}n{$width}{1}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";

    $file->print("\\begin{tabular}{\@{} l $quantiles \@{}}\n",
                 "\\toprule\n",
                 "{$parameter_shortname} & \\multicolumn{5}{l}{{Rank}} \\\\\n",
                 "\\midrule\n",
                 "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} \\\\\n",
                 "\\midrule\n");

    # Sort algorithms by increasing rank
    my @sorted = sort {
        foreach (@preference_min) {
            if ($a->{$_} != $b->{$_}) {
                return $a->{$_} <=> $b->{$_};
            }
        }
        return 0;
    } @rank_statistics;
    my $format = join " & ", map { "%f" } @summary_statistics;
    foreach my $row (@sorted) {
        $file->print("$row->{parameter} & ");
        $file->printf($format, $row->{min}, $row->{q1}, $row->{median}, $row->{q3}, $row->{max});
        $file->print(" \\\\\n");
    }

    $file->print(latex_table_end());
    $file->close();
}

sub generate_table_value
{
    foreach my $fn (@$functions) {
        my $id = $fn->{id};
        my $path = "$path_report/table.value.$id.tex";
        my $file = IO::File->new($path, '>')
            or die "hnco-algorithm-parameter-stat.pl: generate_table_value: Cannot open $path\n";

        # Header
        my $before = $fn->{rounding}->{value}->{before} || 3;

        # Precision for min/max
        my $after_min_max = $fn->{rounding}->{value}->{after} || 0;

        # Precision for median
        my $after_median = 1;
        if ($fn->{rounding}->{value}->{after}) {
            $after_median = $fn->{rounding}->{value}->{after} + 1;
        }

        # Precision for quantiles
        my $after_quantiles = 2;
        if ($fn->{rounding}->{value}->{after}) {
            $after_quantiles = $fn->{rounding}->{value}->{after} + 2;
        }

        my $quantiles = "";
        $quantiles .= ">{{\\nprounddigits{$after_min_max}}}n{$before}{$after_min_max}";
        $quantiles .= ">{{\\nprounddigits{$after_quantiles}}}n{$before}{$after_quantiles}";
        $quantiles .= ">{{\\nprounddigits{$after_median}}}n{$before}{$after_median}";
        $quantiles .= ">{{\\nprounddigits{$after_quantiles}}}n{$before}{$after_quantiles}";
        $quantiles .= ">{{\\nprounddigits{$after_min_max}}}n{$before}{$after_min_max}";

        $file->print("\\begin{tabular}{\@{} l $quantiles \@{}}\n",
                     "\\toprule\n",
                     "{$parameter_shortname} & \\multicolumn{5}{l}{{Function value}} \\\\\n",
                     "\\midrule\n",
                     "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} \\\\\n",
                     "\\midrule\n");

        # Sort algorithms by decreasing value
        my @rows = grep { $_->{function} eq $id } @value_statistics;
        my @sorted = sort {
            foreach (@preference_max) {
                if ($a->{$_} != $b->{$_}) {
                    return $b->{$_} <=> $a->{$_};
                }
            }
            return 0;
        } @rows;
        my $conversion = $fn->{logscale} ? "%e" : "%f";
        my $best = $best_value_statistics->{$id};
        foreach my $row (@sorted) {
            $file->print("$row->{parameter} & ");
            if ($fn->{reverse}) {
                my $format = join " & ",
                    map { $row->{$_} == $best->{$_} ? "{\\npboldmath} $conversion" : "$conversion" } @summary_statistics_reverse;
                $file->printf($format,
                              -$row->{max},
                              -$row->{q3},
                              -$row->{median},
                              -$row->{q1},
                              -$row->{min});
            } else {
                my $format = join " & ",
                    map { $row->{$_} == $best->{$_} ? "{\\npboldmath} $conversion" : "$conversion" } @summary_statistics;
                $file->printf($format,
                              $row->{min},
                              $row->{q1},
                              $row->{median},
                              $row->{q3},
                              $row->{max});
            }
            $file->print(" \\\\\n");
        }
        $file->print(latex_table_end());
        $file->close();
    }
}

sub generate_latex
{
    my $path = "$path_report/content.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-parameter-stat.pl: generate_latex: Cannot open $path_report/content.tex\n";
    $file->print(latex_graphicspath($path_graphics),
                 latex_section("Global results"),
                 latex_begin_center(),
                 latex_input_file("table.rank.tex"),
                 latex_end_center());
    foreach my $f (@$functions) {
        my $id = $f->{id};
        $file->print(latex_newpage(),
                     latex_section("Function $id"),

                     latex_begin_center(),
                     latex_includegraphics("$id/scatter", 0.6),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_includegraphics("$id/candlesticks", 0.6),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_input_file("table.value.$id.tex"),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_includegraphics("$id/mean", 0.6),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_includegraphics("$id/stddev", 0.6),
                     latex_end_center());

            }

    $file->close();
}
