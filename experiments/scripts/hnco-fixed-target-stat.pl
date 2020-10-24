#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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
my @preference = qw(median q3 q1 max min);

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
my $longest_run = $obj->{budget};

my @results = ();
my @statistics = ();
my $best_statistics = {};
my $rank_distributions = {};
my $global_statistics = {};
#
# Processing
#

load_results();
compute_global_statistics();
compute_statistics();
compute_best_statistics();
compute_rank_distributions();
generate_data_statistics();
generate_gnuplot_candlesticks();
generate_latex_rank_distributions();
generate_latex_statistics();
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
                push @results, { function => $function_id,
                                 algorithm => $algorithm_id,
                                 run => $_,
                                 num_evaluations => $obj->{num_evaluations},
                                 success => ($obj->{success} ? 1 : 0) }
            }
        }
    }
}

sub compute_global_statistics
{
    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};
        my $sd = Statistics::Descriptive::Full->new();
        my @rows = grep { ($_->{algorithm} eq $algorithm_id) } @results;
        foreach (@rows) {
            $sd->add_data($_->{success});
        }
        $global_statistics->{$algorithm_id} = $sd->mean();
    }
    $longest_run = max (map { $_->{num_evaluations} } @results);
}

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $sd_eval = Statistics::Descriptive::Full->new();
            my $sd_success = Statistics::Descriptive::Full->new();
            my @rows = grep { ($_->{function} eq $function_id) && ($_->{algorithm} eq $algorithm_id) } @results;
            foreach (@rows) {
                $sd_eval->add_data($_->{num_evaluations});
                $sd_success->add_data($_->{success});
            }
            push @statistics, { function        => $function_id,
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

sub compute_best_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $best = {};
        my @rows = grep { ($_->{function} eq $function_id) && ($_->{success_count} == $num_runs) } @statistics;
        foreach my $summary (@summary_statistics) {
            $best->{$summary} = min (map { $_->{$summary} } @rows);
        }
        $best_statistics->{$function_id} = $best;
    }
}

sub compute_rank_distributions
{
    # Initialize rank distributions
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my @counts = (0) x @$algorithms;
        $rank_distributions->{$algorithm_id} = \@counts;
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        # Sort algorithms by increasing order of num_evaluations
        my @rows = grep { $_->{function} eq $function_id } @statistics;
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

        # Set ranks
        for (my $i = 0; $i < @sorted; $i++) {
            $sorted[$i]->{rank} = $i;
        }

        # Handle ex-aequo
        my $rk = [];
        my $ex_aequo = [$sorted[0]->{algorithm}];
        for (my $i = 1; $i < @sorted; $i++) {
            my $current = $sorted[$i];
            my $previous = $sorted[$i - 1];
            if (all { $current->{$_} == $previous->{$_} } @preference) {
                $current->{rank} = $previous->{rank};
                push @{ $ex_aequo }, $current->{algorithm};
            } else {
                push @{ $rk }, $ex_aequo;
                $ex_aequo = [$current->{algorithm}];
            }
        }
        push @{ $rk }, $ex_aequo;

        # Update rank distributions
        foreach (@sorted) {
            my $algorithm_id = $_->{algorithm};
            ${ $rank_distributions->{$algorithm_id} }[$_->{rank}]++;
        }
    }
}

sub generate_data_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $path = "$path_results/$function_id/statistics.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_data_statistics: Cannot open '$path': $!\n";

        my $position = 1;
        my @rows = grep { $_->{function} eq $function_id } @statistics;
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
    open(CANDLESTICKS, ">candlesticks.gp")
        or die "hnco-benchmark-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

    print CANDLESTICKS
        "#!/usr/bin/gnuplot -persist\n",
        "unset grid\n",
        "set grid y\n",
        "set xtics rotate by -45\n",
        "set ylabel \"Number of evaluations\"\n",
        "set autoscale y\n",
        "set boxwidth 0.4\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    # x range
    my $xrange = @$algorithms + 1;
    print CANDLESTICKS "set xrange [0:$xrange]\n\n";

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
        print CANDLESTICKS
            "$terminal{pdf} $font\n",
            "set output $quoted_path\n";
        $quoted_path = qq("$path_results/$function_id/statistics.dat");
        print CANDLESTICKS
            "plot $quoted_path using 1:3:2:6:5:xticlabels(7) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
            "     $quoted_path using 1:4:4:4:4 with candlesticks lw 3 lt 1 notitle\n";

        $quoted_path = qq("$path_graphics/$function_id.eps");
        print CANDLESTICKS
            "$terminal{eps} $font\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = qq("$path_graphics/$function_id.png");
        print CANDLESTICKS
            "$terminal{png} $font\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    system("chmod a+x candlesticks.gp");
}

sub generate_latex_rank_distributions
{
    my $path = "$path_report/table-rank-distribution.tex";
    open(LATEX, ">$path")
        or die "hnco-benchmark-stat.pl: generate_latex_rank_distributions: Cannot open $path\n";
    latex_table_rank_distributions_begin();
    latex_table_rank_distributions_body();
    latex_table_rank_distributions_end();
    close LATEX;
}

sub generate_latex_statistics
{
    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};

        my $best = $best_statistics->{$function_id};
        my $path = "$path_report/table-$function_id.tex";

        open(LATEX, ">$path")
            or die "hnco-benchmark-stat.pl: generate_latex_statistics: Cannot open $path\n";
        latex_table_statistics_begin();
        my @rows = grep { $_->{function} eq $function_id } @statistics;
        foreach (@rows) {
            latex_table_statistics_add_row($_, $best);
        }
        latex_table_end();
        close LATEX;
    }
}

sub generate_latex
{
    open(LATEX, ">$path_report/content.tex")
        or die "hnco-benchmark-stat.pl: generate_latex: Cannot open $path_report/content.tex\n";

    print LATEX latex_graphicspath($path_graphics);
    print LATEX latex_section("Global results");
    print LATEX latex_begin_center();
    print LATEX latex_input_file("table-rank-distribution.tex");
    print LATEX latex_end_center();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        print LATEX latex_section("Function $function_id");
        print LATEX latex_begin_center();
        print LATEX latex_input_file("table-$function_id.tex");
        print LATEX latex_end_center();
        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id", 0.6);
        print LATEX latex_end_center();
    }

    close LATEX;
}

sub latex_table_statistics_begin
{
    my $width = int(ceil(log($longest_run) / log(10)));
    print LATEX
        "\\begin{tabular}{\@{}l*{5}{>{{\\nprounddigits{0}}}n{$width}{0}}>{{\\nprounddigits{1} \\npunit{\\%}}}n{3}{1}>{{\\nprounddigits{0}}}n{2}{0}\@{}}\n",
        "\\toprule\n",
        "{Algorithm} & \\multicolumn{7}{l}{{Number of evaluations}} \\\\\n",
        "\\midrule\n",
        "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & {success} & {rk}\\\\\n",
        "\\midrule\n";
}

sub latex_table_statistics_add_row
{
    my ($row, $best) = @_;

    my $format = join " & ", map { (defined($best->{$_}) && $row->{$_} == $best->{$_}) ? "{\\npboldmath} %f" : "%f" } @summary_statistics;

    print LATEX "$row->{algorithm} & ";
    printf LATEX ($format, $row->{min}, $row->{q1}, $row->{median}, $row->{q3}, $row->{max});
    printf LATEX (" & %d", 100 * $row->{success_rate});
    printf LATEX (" & %d\\\\\n", $row->{rank} + 1);
}

sub latex_table_end
{
    print LATEX
        "\\bottomrule\n",
        "\\end{tabular}\n";
}

sub latex_table_rank_distributions_begin
{
    my $num_algo = @$algorithms;
    print LATEX
        "\\begin{tabular}{\@{}l*{$num_algo}{r}>{{\\nprounddigits{1} \\npunit{\\%}}}n{3}{1}\@{}}\n",
        "\\toprule\n",
        "Algorithm & \\multicolumn{$num_algo}{l}{{Rank distribution}} & {Success} \\\\\n",
        "\\midrule\n",
        "& ", join(" & ", 1 .. $num_algo), "\\\\\n",
        "\\midrule\n";
}

sub latex_table_rank_distributions_body
{
    my @order = sort {
        if ($global_statistics->{$a} != $global_statistics->{$b}) {
            return $global_statistics->{$b} <=> $global_statistics->{$a};
        }
        foreach (0 .. @$algorithms - 1) {
            if (${ $rank_distributions->{$b} }[$_] != ${ $rank_distributions->{$a} }[$_]) {
                return ${ $rank_distributions->{$b} }[$_] <=> ${ $rank_distributions->{$a} }[$_];
            }
        }
        return 0;
    } keys %$rank_distributions;
    foreach (@order) {
        print LATEX "$_ & ", join(" & ", @{ $rank_distributions->{$_} });
        printf LATEX (" & %d", 100 * $global_statistics->{$_});
        print LATEX "\\\\\n";
    }
}

sub latex_table_rank_distributions_end
{
    print LATEX
        "\\bottomrule\n",
        "\\end{tabular}\n";
}
