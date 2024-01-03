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
use POSIX;

use HNCO::Report qw(
    %terminal
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

my $path_results        = "results";
my $path_graphics       = "graphics";
my $path_report         = "report";

#
# Read plan
#

my $filename = "plan.json";
if (@ARGV) {
    $filename = shift @ARGV;
}
print "Using $filename\n";

my $plan = from_json(read_file($filename));

#
# Global variables
#

my $algorithms            = $plan->{algorithms};
my $functions             = $plan->{functions};
my $graphics              = $plan->{graphics};
my $num_runs              = $plan->{num_runs};

my @results               = ();
my @value_statistics      = ();
my @time_statistics       = ();
my @rank_statistics       = ();

# hash indexed by algorithm
my $rank_data             = {};
my $algorithm_from_id     = {};

# hash indexed by function
my $best_value_statistics = {};

#
# Processing
#

add_missing_properties();

load_results();

compute_statistics();
compute_best_value_statistics();
compute_rank_data();
compute_rank_statistics();

generate_scatter_data();
generate_scatter_gnuplot();
generate_candlesticks_data();
generate_candlesticks_gnuplot();
generate_table_rank();
generate_table_value();
generate_table_time();
generate_latex();

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

#
# Local functions
#

sub add_missing_properties
{
    foreach (@$algorithms) {
        my $id = $_->{id};
        $algorithm_from_id->{$id} = $_;
        if (exists $_->{labels}) {
            my $labels = $_->{labels};
            unless (exists $labels->{latex}) {
                $labels->{latex} = $id;
            }
            unless (exists $labels->{gnuplot}) {
                $labels->{gnuplot} = $id;
            }
        } else {
            my $labels = {};
            $labels->{latex} = $id;
            $labels->{gnuplot} = $id;
            $_->{labels} = $labels;
        }
    }
}

sub load_results
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $prefix = "$path_results/$function_id/$algorithm_id";
            foreach (1 .. $num_runs) {
                my $obj = from_json(read_file("$prefix/$_.out"));
                push @results, { function        => $function_id,
                                 algorithm       => $algorithm_id,
                                 run             => $_,
                                 value           => $obj->{value},
                                 num_evaluations => $obj->{num_evaluations},
                                 total_time      => $obj->{total_time},
                                 evaluation_time => $obj->{evaluation_time},
                                 algorithm_time  => $obj->{total_time} - $obj->{evaluation_time} };
            }
        }
    }
}

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my @data = grep { $_->{function} eq $function_id } @results;
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my @rows = grep { $_->{algorithm} eq $algorithm_id } @data;

            my @values = map { $_->{value} } @rows;
            my $sd = Statistics::Descriptive::Full->new();
            $sd->add_data(@values);
            push @value_statistics, { function  => $function_id,
                                      algorithm => $algorithm_id,
                                      min       => $sd->min(),
                                      q1        => $sd->quantile(1),
                                      median    => $sd->median(),
                                      q3        => $sd->quantile(3),
                                      max       => $sd->max() };

            my $item = { function  => $function_id,
                         algorithm => $algorithm_id };

            @values = map { $_->{total_time} } @rows;
            $sd->clear();
            $sd->add_data(@values);
            $item->{total_time_mean} = $sd->mean();
            $item->{total_time_stddev} = $sd->standard_deviation();

            @values = map { $_->{evaluation_time} } @rows;
            $sd->clear();
            $sd->add_data(@values);
            $item->{evaluation_time_mean} = $sd->mean();
            $item->{evaluation_time_stddev} = $sd->standard_deviation();

            @values = map { $_->{algorithm_time} } @rows;
            $sd->clear();
            $sd->add_data(@values);
            $item->{algorithm_time_mean} = $sd->mean();
            $item->{algorithm_time_stddev} = $sd->standard_deviation();

            push @time_statistics, $item;
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
    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};
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
    foreach my $algorithm (@$algorithms) {
        my $id = $algorithm->{id};
        my $sd = Statistics::Descriptive::Full->new();
        $sd->add_data(@{ $rank_data->{$id} });
        my $row = { algorithm => $id,
                    min       => $sd->min(),
                    q1        => $sd->quantile(1),
                    median    => $sd->median(),
                    q3        => $sd->quantile(3),
                    max       => $sd->max() };
        push @rank_statistics, $row;
    }
}

sub generate_scatter_data
{
    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        my @rows = grep { $_->{function} eq $function_id } @results;
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $path = "$path_results/$function_id/$algorithm_id/scatter.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-benchmark-stat.pl: generate_scatter_data: Cannot open '$path': $!\n";
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
        or die "hnco-benchmark-stat.pl: generate_scatter_gnuplot: Cannot open $path\n";
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
            $file->print("set logscale y 10\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale\n",
                         "set format y\n");
        }

        $path = qq("$path_graphics/scatter.$function_id.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $path\n");

        $file->print("plot \\\n");
        $file->print(join ", \\\n",
                     (map {
                         my $algorithm_id = $_->{id};
                         my $path = qq("$path_results/$function_id/$algorithm_id/scatter.dat");
                         my $title = qq("$_->{labels}->{gnuplot}");
                         $fn->{reverse} ?
                             "  $path using 1:(-\$2) with points title $title" :
                             "  $path using 1:2 with points title $title";
                      } @$algorithms));
        $file->print("\n");

        $path = qq("$path_graphics/scatter.$function_id.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $path\n",
                     "replot\n");

        $path = qq("$path_graphics/scatter.$function_id.png");
        $file->print("$terminal{png} $font\n",
                     "set output $path\n",
                     "replot\n\n");
    }
    $file->close();
    system("chmod a+x scatter.gp");
}

sub generate_candlesticks_data
{
    foreach my $fn (@$functions) {
        my $id = $fn->{id};
        my $path = "$path_results/$id/candlesticks.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_candlesticks_data: Cannot open '$path': $!\n";
        my $position = 1;
        my @rows = grep { $_->{function} eq $id } @value_statistics;
        foreach (@rows) {
            my $label = qq("$algorithm_from_id->{$_->{algorithm}}->{labels}->{gnuplot}");
            if ($fn->{reverse}) {
                $file->printf("%d %e %e %e %e %e %s\n",
                              $position,
                              -$_->{max},
                              -$_->{q3},
                              -$_->{median},
                              -$_->{q1},
                              -$_->{min},
                              $label);
            } else {
                $file->printf("%d %e %e %e %e %e %s\n",
                              $position,
                              $_->{min},
                              $_->{q1},
                              $_->{median},
                              $_->{q3},
                              $_->{max},
                              $label);
            }
            $position++;
        }
        $file->close();
    }
}

sub generate_candlesticks_gnuplot
{
    my $path = "candlesticks.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-benchmark-stat.pl: generate_candlesticks_gnuplot: Cannot open $path\n";

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "unset grid\n",
                 "set grid y\n",
                 "set xtics rotate by -45\n",
                 "set ylabel \"Value\"\n",
                 "set autoscale y\n",
                 "set boxwidth 0.4\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n");

    # x range
    my $xrange = @$algorithms + 1;
    $file->print("set xrange [0:$xrange]\n\n");

    # Font face and size
    my $font = "";
    if ($graphics->{candlesticks}->{font_face}) {
        $font = $graphics->{candlesticks}->{font_face};
    }
    if ($graphics->{candlesticks}->{font_size}) {
        $font = "$font,$graphics->{candlesticks}->{font_size}";
    }
    $font = qq(font "$font");

    foreach my $fn (@$functions) {
        my $id = $fn->{id};

        if ($fn->{logscale}) {
            my $fmt = qq("10^{\%T}");
            $file->print("set logscale y 10\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale\n",
                         "set format y\n");
        }

        my $path = qq("$path_graphics/candlesticks.$id.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $path\n");
        $path = qq("$path_results/$id/candlesticks.dat");
        $file->print("plot $path using 1:3:2:6:5:xticlabels(7) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                     "     $path using 1:4:4:4:4 with candlesticks lw 3 lt 1 notitle\n");

        $path = qq("$path_graphics/candlesticks.$id.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $path\n",
                     "replot\n");

        $path = qq("$path_graphics/candlesticks.$id.png");
        $file->print("$terminal{png} $font\n",
                     "set output $path\n",
                     "replot\n\n");
    }
    $file->close();
    system("chmod a+x candlesticks.gp");
}

sub generate_table_rank
{
    my $path = "$path_report/table.rank.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-benchmark-stat.pl: generate_table_rank: Cannot open $path\n";

    # Header
    my $width = int(ceil(log(1 + @{ $algorithms }) / log(10)));
    my $quantiles = "";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{1}}}n{$width}{1}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";

    $file->print("\\begin{tabular}{\@{} l $quantiles \@{}}\n",
                 "\\toprule\n",
                 "{Algorithm} & \\multicolumn{5}{l}{{Rank}} \\\\\n",
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
        my $label = $algorithm_from_id->{$row->{algorithm}}->{labels}->{latex};
        $file->print("$label & ");
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
            or die "hnco-benchmark-stat.pl: generate_table_value: Cannot open $path\n";

        # Header
        my $before = $fn->{rounding}->{value}->{before} || 3;
        my $after = $fn->{rounding}->{value}->{after} || 0;
        $file->print("\\begin{tabular}{\@{} l *{5}{>{{\\nprounddigits{$after}}}n{$before}{$after}} \@{}}\n",
                     "\\toprule\n",
                     "{Algorithm} & \\multicolumn{5}{l}{{Value}} \\\\\n",
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
            my $label = $algorithm_from_id->{$row->{algorithm}}->{labels}->{latex};
            $file->print("$label & ");
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

sub generate_table_time
{
    foreach my $fn (@$functions) {
        my $id = $fn->{id};
        my $path = "$path_report/table.time.$id.tex";
        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_table_time: Cannot open $path\n";

        # Header
        my $before = $fn->{rounding}->{time}->{before} || 1;
        my $after = $fn->{rounding}->{time}->{after} || 2;
        $file->print("\\begin{tabular}{\@{} l *{6}{>{{\\nprounddigits{$after}}}N{$before}{$after}} \@{}}\n",
                     "\\toprule\n",
                     "{Algorithm} & \\multicolumn{6}{l}{{Time (s)}} \\\\\n",
                     "\\midrule\n",
                     "& \\multicolumn{2}{l}{{algorithm}} & \\multicolumn{2}{l}{{evaluation}} & \\multicolumn{2}{l}{{total}} \\\\\n",
                     "\\midrule\n",
                     "& {mean} & {dev.} & {mean} & {dev.} & {mean} & {dev.} \\\\\n",
                     "\\midrule\n");

        # Rows
        my @rows = grep { $_->{function} eq $id } @time_statistics;
        my @sorted = sort { $a->{algorithm_time_mean} <=> $b->{algorithm_time_mean} } @rows;
        foreach my $row (@sorted) {
            my $label = $algorithm_from_id->{$row->{algorithm}}->{labels}->{latex};
            $file->printf("$label & %f & %f & %f & %f & %f & %f \\\\\n",
                          $row->{algorithm_time_mean},
                          $row->{algorithm_time_stddev},
                          $row->{evaluation_time_mean},
                          $row->{evaluation_time_stddev},
                          $row->{total_time_mean},
                          $row->{total_time_stddev});
        }
        $file->print(latex_table_end());
        $file->close();
    }
}

sub latex_table_end
{
    return
        "\\bottomrule\n" .
        "\\end{tabular}\n";
}

sub generate_latex
{
    my $path = "$path_report/content.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-benchmark-stat.pl: generate_latex: Cannot open $path_report/content.tex\n";
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
                     latex_input_file("table.value.$id.tex"),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_includegraphics("candlesticks.$id", 0.6),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_includegraphics("scatter.$id", 0.6),
                     latex_end_center(),

                     latex_begin_center(),
                     latex_input_file("table.time.$id.tex"),
                     latex_end_center());
    }
    $file->close();
}
