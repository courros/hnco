#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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
my @preference = qw(median q1 q3 min max);

my $path_results        = "results";
my $path_graphics       = "graphics";
my $path_report         = "report";

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

my $algorithms  = $obj->{algorithms};
my $functions   = $obj->{functions};
my $num_runs    = $obj->{num_runs};
my $graphics    = $obj->{graphics};

my @time_data = ();
my @time_statistics = ();
my @rank_statistics = ();

# hash indexed by function
my $longest_run = {};

# hash indexed by algorithm
my $rank_data = {};

#
# Processing
#

load_results();

compute_time_statistics();
compute_longest_run();
compute_rank_data();
compute_rank_statistics();

generate_function_statistics();
generate_gnuplot_candlesticks();
generate_table_rank();
generate_table_function();
generate_latex();

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

#
# Local functions
#

sub load_results
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $prefix = "$path_results/$function_id/$algorithm_id";
            foreach (1 .. $num_runs) {
                my $obj = from_json(read_file("$prefix/$_.out"));
                push @time_data, { function => $function_id,
                                   algorithm => $algorithm_id,
                                   run => $_,
                                   num_evaluations => $obj->{num_evaluations},
                                   success => ($obj->{success} ? 1 : 0) }
            }
        }
    }
}

sub compute_time_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $sd_eval = Statistics::Descriptive::Full->new();
            my $sd_success = Statistics::Descriptive::Full->new();
            my @rows = grep { ($_->{function} eq $function_id) && ($_->{algorithm} eq $algorithm_id) } @time_data;
            foreach (@rows) {
                $sd_eval->add_data($_->{num_evaluations});
                $sd_success->add_data($_->{success});
            }
            push @time_statistics, { function        => $function_id,
                                     algorithm       => $algorithm_id,
                                     min             => $sd_eval->min(),
                                     q1              => $sd_eval->quantile(1),
                                     median          => $sd_eval->median(),
                                     q3              => $sd_eval->quantile(3),
                                     max             => $sd_eval->max(),
                                     success_count   => $sd_success->sum(),
                                     success_rate    => $sd_success->mean() };
        }
    }
}

sub compute_longest_run
{
    foreach my $function (@$functions) {
        my $id = $function->{id};
        my @rows = grep { ($_->{function} eq $id) } @time_data;
        $longest_run->{$id} = max (map { $_->{num_evaluations} } @rows);
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

        # Sort algorithms by success_count, num_evaluations
        my @rows = grep { $_->{function} eq $function_id } @time_statistics;
        my @sorted = sort {
            if ($a->{success_count} != $b->{success_count}) {
                return $b->{success_count} <=> $a->{success_count};
            }
            foreach (@preference) {
                if ($a->{$_} != $b->{$_}) {
                    return $a->{$_} <=> $b->{$_};
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
            if ((all { $current->{$_} == $previous->{$_} } @summary_statistics) and ($current->{success_rate} == $previous->{success_rate})) {
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
        my $row = { algorithm   => $id,
                    min         => $sd->min(),
                    q1          => $sd->quantile(1),
                    median      => $sd->median(),
                    q3          => $sd->quantile(3),
                    max         => $sd->max() };
        $sd->clear();
        $sd->add_data(map { $_->{success} } (grep { ($_->{algorithm} eq $id) } @time_data));
        $row->{success_rate} = $sd->mean();
        push @rank_statistics, $row;
    }
}

sub generate_function_statistics
{
    foreach my $f (@$functions) {
        my $id = $f->{id};
        my $path = "$path_results/$id/statistics.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_function_statistics: Cannot open '$path': $!\n";
        my $position = 1;
        my @rows = grep { $_->{function} eq $id } @time_statistics;
        foreach (@rows) {
            $file->printf("%d %e %e %e %e %e %s\n",
                          $position,
                          $_->{min},
                          $_->{q1},
                          $_->{median},
                          $_->{q3},
                          $_->{max},
                          $_->{algorithm});
            $position++;
        }
        $file->close();
    }
}

sub generate_gnuplot_candlesticks
{
    my $path = "candlesticks.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-benchmark-stat.pl: generate_gnuplot_candlesticks: Cannot open $path\n";

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "unset grid\n",
                 "set grid y\n",
                 "set xtics rotate by -45\n",
                 "set ylabel \"Number of evaluations\"\n",
                 "set autoscale y\n",
                 "set boxwidth 0.4\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, 0\n");

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

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_path = qq("$path_graphics/$function_id.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $quoted_path\n");
        $quoted_path = qq("$path_results/$function_id/statistics.dat");
        $file->print("plot $quoted_path using 1:3:2:6:5:xticlabels(7) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                     "     $quoted_path using 1:4:4:4:4 with candlesticks lw 3 lt 1 notitle\n");

        $quoted_path = qq("$path_graphics/$function_id.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $quoted_path\n",
                     "replot\n");

        $quoted_path = qq("$path_graphics/$function_id.png");
        $file->print("$terminal{png} $font\n",
                     "set output $quoted_path\n",
                     "replot\n\n");
    }

    $file->close();
    system("chmod a+x candlesticks.gp");
}

sub generate_table_rank
{
    my $path = "$path_report/table-rank.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-benchmark-stat.pl: generate_table_rank: Cannot open $path\n";

    # Header
    my $width = int(ceil(log(@{ $algorithms }) / log(10)));
    my $quantiles = "";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{1}}}n{$width}{1}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";

    $file->print("\\begin{tabular}{\@{} l $quantiles >{{\\nprounddigits{1} \\npunit{\\%}}}n{3}{1} \@{}}\n",
                 "\\toprule\n",
                 "{Algorithm} & \\multicolumn{5}{l}{{Rank}} & {Success} \\\\\n",
                 "\\midrule\n",
                 "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & \\\\\n",
                 "\\midrule\n");

    # Rows
    my @sorted = sort {
        if ($a->{success_rate} != $b->{success_rate}) {
            return $b->{success_rate} <=> $a->{success_rate};
        }
        foreach (@preference) {
            if ($a->{$_} != $b->{$_}) {
                return $a->{$_} <=> $b->{$_};
            }
        }
        return 0;
    } @rank_statistics;
    my $format = join " & ", map { "%f" } @summary_statistics;
    foreach my $row (@sorted) {
        $file->print("$row->{algorithm} & ");
        $file->printf($format, $row->{min}, $row->{q1}, $row->{median}, $row->{q3}, $row->{max});
        $file->printf(" & %d\\\\\n", 100 * $row->{success_rate});
    }

    $file->print(latex_table_end());
    $file->close();
}

sub generate_table_function
{
    foreach my $fn (@$functions) {
        my $id = $fn->{id};
        my $path = "$path_report/table-$id.tex";
        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_table_function: Cannot open $path\n";

        # Header
        my $width = int(ceil(log($longest_run->{$id}) / log(10)));
        my $quantiles = "";
        $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";
        $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
        $quantiles .= ">{{\\nprounddigits{1}}}n{$width}{1}";
        $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
        $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";

        $file->print("\\begin{tabular}{\@{} l $quantiles >{{\\nprounddigits{1} \\npunit{\\%}}}n{3}{1} \@{}}\n",
                     "\\toprule\n",
                     "{Algorithm} & \\multicolumn{5}{l}{{Number of evaluations}} & {Success} \\\\\n",
                     "\\midrule\n",
                     "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & \\\\\n",
                     "\\midrule\n");

        # Rows
        my @rows = grep { $_->{function} eq $id } @time_statistics;
        my @sorted = sort {
            if ($a->{success_rate} != $b->{success_rate}) {
                return $b->{success_rate} <=> $a->{success_rate};
            }
            foreach (@preference) {
                if ($a->{$_} != $b->{$_}) {
                    return $a->{$_} <=> $b->{$_};
                }
            }
            return 0;
        } @rows;
        foreach my $row (@sorted) {
            my $format = join " & ", map { "%f" } @summary_statistics;
            $file->print("$row->{algorithm} & ");
            $file->printf($format, $row->{min}, $row->{q1}, $row->{median}, $row->{q3}, $row->{max});
            $file->printf(" & %d\\\\\n", 100 * $row->{success_rate});
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
                 latex_input_file("table-rank.tex"),
                 latex_end_center());
    foreach my $f (@$functions) {
        my $id = $f->{id};
        $file->print(latex_section("Function $id"),
                     latex_begin_center(),
                     latex_input_file("table-$id.tex"),
                     latex_end_center(),
                     latex_begin_center(),
                     latex_includegraphics("$id", 0.6),
                     latex_end_center());
    }

    $file->close();
}
