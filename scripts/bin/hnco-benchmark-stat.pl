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

use HNCO::Report qw(
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

my $path_results        = "results";
my $path_graphics       = "graphics";
my $path_report         = "report";

my @summary_statistics  = qw(min q1 median q3 max);
my @pref_max            = qw(median q1 q3 min max);
my @pref_min            = qw(median q3 q1 max min);

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

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

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $graphics            = $obj->{graphics};

my $ranges = {};
my $ranking = {};
my $stat_eval = {};
my $stat_value = {};
my $stat_total_time = {};
my $stat_evaluation_time = {};
my $stat_algorithm_time = {};
my $stat_value_best = {};
my $hall_of_fame = {};

#
# Processing
#

my $rank_distribution = {};
foreach my $a (@$algorithms) {
    my $algorithm_id = $a->{id};
    my @counts = (0) x @$algorithms;
    $rank_distribution->{$algorithm_id} = \@counts;
}

compute_statistics();
compute_best_statistics();
compute_rank_distribution();

reverse_values();
reverse_best_statistics();
compute_ranges();

generate_function_data();
generate_function_results();

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_gnuplot_candlesticks();
generate_gnuplot_scatter();
generate_table_rank_distribution();
generate_ranking();
generate_table_functions();
generate_latex();

#
# Local functions
#

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $value = {};
        my $eval = {};
        my $total_time = {};
        my $evaluation_time = {};
        my $algorithm_time = {};

        $hall_of_fame->{$function_id} = [];

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $algorithm_num_runs = $num_runs;
            if ($a->{deterministic}) {
                $algorithm_num_runs = 1;
            }

            my $prefix = "$path_results/$function_id/$algorithm_id";

            my $SD_value = Statistics::Descriptive::Full->new();
            my $SD_eval = Statistics::Descriptive::Full->new();
            my $SD_total_time = Statistics::Descriptive::Full->new();
            my $SD_evaluation_time = Statistics::Descriptive::Full->new();
            my $SD_algorithm_time = Statistics::Descriptive::Full->new();

            my $path = "$prefix/$algorithm_id.dat";
            my $file_data = IO::File->new($path, '>')
                or die "hnco-benchmark-stat.pl: compute_statistics: Cannot open '$path': $!\n";
            foreach (1 .. $algorithm_num_runs) {
                my $obj = from_json(read_file("$prefix/$_.out"));
                $file_data->print("$obj->{num_evaluations} $obj->{value}\n");
                $SD_value->add_data($obj->{value});
                $SD_eval->add_data($obj->{num_evaluations});
                $SD_total_time->add_data($obj->{total_time});
                $SD_evaluation_time->add_data($obj->{evaluation_time});
                $SD_algorithm_time->add_data($obj->{total_time} - $obj->{evaluation_time});

                push @{ $hall_of_fame->{$function_id} }, { algorithm => $algorithm_id, run => $_, value => $obj->{value} };

            }
            $file_data->close;

            $value->{$algorithm_id} = { min     => $SD_value->min(),
                                        q1      => $SD_value->quantile(1),
                                        median  => $SD_value->median(),
                                        q3      => $SD_value->quantile(3),
                                        max     => $SD_value->max() };

            $eval->{$algorithm_id} = { min     => $SD_eval->min(),
                                       q1      => $SD_eval->quantile(1),
                                       median  => $SD_eval->median(),
                                       q3      => $SD_eval->quantile(3),
                                       max     => $SD_eval->max() };

            $total_time->{$algorithm_id} = { mean     => $SD_total_time->mean(),
                                             stddev   => $SD_total_time->standard_deviation() };

            $evaluation_time->{$algorithm_id} = { mean     => $SD_evaluation_time->mean(),
                                                  stddev   => $SD_evaluation_time->standard_deviation() };

            $algorithm_time->{$algorithm_id} = { mean     => $SD_algorithm_time->mean(),
                                                 stddev   => $SD_algorithm_time->standard_deviation() };

        }

        $stat_value->{$function_id} = $value;
        $stat_eval->{$function_id} = $eval;
        $stat_total_time->{$function_id} = $total_time;
        $stat_evaluation_time->{$function_id} = $evaluation_time;
        $stat_algorithm_time->{$function_id} = $algorithm_time;

    }

}

sub compute_best_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $stat = $stat_value->{$function_id};
        my $best = {};

        foreach (@summary_statistics) {
            my @values = ();
            foreach my $a (@$algorithms) {
                my $algorithm_id = $a->{id};
                push @values, $stat->{$algorithm_id}->{$_};
            }
            $best->{$_} = max @values;
        }

        $stat_value_best->{$function_id} = $best;

    }

}

sub compute_rank_distribution
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $stat = $stat_value->{$function_id};

        # Sort algorithms by decreasing order of performance
        my @sorted = sort {
            foreach (@pref_max) {
                if ($stat->{$b}->{$_} != $stat->{$a}->{$_}) {
                    return $stat->{$b}->{$_} <=> $stat->{$a}->{$_};
                }
            }
            return 0;
        } keys %$stat;

        # Set ranks
        for (my $i = 0; $i < @sorted; $i++) {
            my $current = $sorted[$i];
            $stat->{$current}->{rank} = $i;
        }

        # Handle ex-aequo
        my $rk = [];
        my $ex_aequo = [$sorted[0]];
        for (my $i = 1; $i < @sorted; $i++) {
            my $current = $sorted[$i];
            my $previous = $sorted[$i - 1];
            if (all { $stat->{$current}->{$_} == $stat->{$previous}->{$_} } @summary_statistics) {
                $stat->{$current}->{rank} = $stat->{$previous}->{rank};
                push @{ $ex_aequo }, $current;
            } else {
                push @{ $rk }, $ex_aequo;
                $ex_aequo = [$current];
            }
        }
        push @{ $rk }, $ex_aequo;

        # Update rank distributions
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            ${ $rank_distribution->{$algorithm_id} }[$stat->{$algorithm_id}->{rank}]++;
        }

        $ranking->{$function_id} = $rk;

    }

}

sub reverse_values
{
    foreach my $f (@$functions) {
        if ($f->{reverse}) {
            foreach my $a (@$algorithms) {
                my $stat = $stat_value->{$f->{id}}->{$a->{id}};
                ($stat->{min}, $stat->{max}) = ($stat->{max}, $stat->{min});
                ($stat->{q1}, $stat->{q3}) = ($stat->{q3}, $stat->{q1});
                foreach (@summary_statistics) {
                    $stat->{$_} = -$stat->{$_};
                }
            }
        }
    }
}

sub reverse_best_statistics
{
    foreach my $f (@$functions) {
        if ($f->{reverse}) {
            my $function_id = $f->{id};
            my $function_best = $stat_value_best->{$function_id};
            ($function_best->{min}, $function_best->{max}) = ($function_best->{max}, $function_best->{min});
            ($function_best->{q1}, $function_best->{q3}) = ($function_best->{q3}, $function_best->{q1});
            foreach (@summary_statistics) {
                $function_best->{$_} = -$function_best->{$_};
            }
        }
    }
}

sub compute_ranges
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $eval = $stat_eval->{$function_id};
        my $value = $stat_value->{$function_id};
        my $range = {};

        $range->{xmin} = min (map { $eval->{$_}->{min} } keys %$eval);
        $range->{xmax} = max (map { $eval->{$_}->{max} } keys %$eval);
        $range->{ymin} = min (map { $value->{$_}->{min} } keys %$value);
        $range->{ymax} = max (map { $value->{$_}->{max} } keys %$value);

        $ranges->{$function_id} = $range;
    }

}

sub generate_function_data
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$path_results/$function_id/$function_id.dat";

        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_function_data: Cannot open '$path': $!\n";

        my $position = 1;
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $stat = $stat_value->{$function_id}->{$algorithm_id};

            $file->printf("%d %e %e %e %e %e %s\n",
                          $position,
                          $stat->{min},
                          $stat->{q1},
                          $stat->{median},
                          $stat->{q3},
                          $stat->{max},
                          $algorithm_id);

            $position++;
        }

        $file->close();

    }

}

sub generate_function_results
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$path_results/$function_id/results.txt";
        my $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_function_results: Cannot open '$path': $!\n";

        my @sorted = sort { $b->{value} <=> $a->{value} } @{ $hall_of_fame->{$function_id} };
        foreach (@sorted) {
            $file->print("$_->{algorithm} $_->{run} $_->{value}\n")
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
        "set xlabel \"Algorithm\"\n",
        "set xtics rotate by -45\n",
        "set ylabel \"Function value\"\n",
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

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print CANDLESTICKS
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print CANDLESTICKS
                "unset logscale\n",
                "set format y\n";
        }

        my $quoted_path = qq("$path_graphics/$function_id.pdf");
        print CANDLESTICKS
            "$terminal{pdf} $font\n",
            "set output $quoted_path\n";
        $quoted_path = qq("$path_results/$function_id/$function_id.dat");
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

sub generate_gnuplot_scatter
{
    open(SCATTER, ">scatter.gp")
        or die "hnco-benchmark-stat.pl: generate_gnuplot_scatter: Cannot open scatter.gp\n";

    print SCATTER
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"Number of evaluations (x10^3)\"\n",
        "set ylabel \"Function value\"\n",
        "set format x ", qq("%.0s"), "\n",
        "set key outside top center box opaque horizontal\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # Font face and size
    my $font = "";
    if ($graphics->{scatter}->{font_face}) {
        $font = $graphics->{scatter}->{font_face};
    }
    if ($graphics->{scatter}->{font_size}) {
        $font = "$font,$graphics->{scatter}->{font_size}";
    }
    $font = qq(font "$font");

    # One function one file
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print SCATTER
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print SCATTER
                "unset logscale y\n",
                "set format y\n";
        }

        my $quoted_path;
        my $quoted_title;

        $quoted_path = qq("$path_graphics/$function_id+all.eps");
        print SCATTER
            "$terminal{eps} $font\n",
            "set output $quoted_path\n";

        print SCATTER "plot \\\n";
        print SCATTER
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $value = $stat_value->{$function_id}->{$algorithm_id};
                $quoted_path = qq("$path_results/$function_id/$algorithm_id/$algorithm_id.dat");
                $quoted_title = qq("$algorithm_id");
                $f->{reverse} ?
                    "  $quoted_path using 1:(-\$2) with points title $quoted_title" :
                    "  $quoted_path using 1:2 with points title $quoted_title";
             } @$algorithms);

        print SCATTER "\n";

        $quoted_path = qq("$path_graphics/$function_id+all.pdf");
        print SCATTER
            "$terminal{pdf} $font\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = qq("$path_graphics/$function_id+all.png");
        print SCATTER
            "$terminal{png} $font\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print SCATTER
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print SCATTER
                "unset logscale y\n",
                "set format y\n";
        }

        my $range = $ranges->{$function_id};
        my $xmin = $range->{xmin};
        my $xmax = $range->{xmax};
        my $ymin = $range->{ymin};
        my $ymax = $range->{ymax};

        print SCATTER
            "set xrange [$xmin:$xmax]\n",
            ($ymin == $ymax) ? "set autoscale y\n" : "set yrange [$ymin:$ymax]\n";

        print SCATTER "\n";

        my $quoted_path;
        my $quoted_title;

        # One function many files
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $value = $stat_value->{$function_id}->{$algorithm_id};

            $quoted_path = qq("$path_graphics/$function_id+$algorithm_id.eps");
            print SCATTER
                "$terminal{eps} $font\n",
                "set output $quoted_path\n";

            $quoted_path = qq("$path_results/$function_id/$algorithm_id/$algorithm_id.dat");
            if ($f->{reverse}) {
                print SCATTER "plot $quoted_path using 1:(-\$2) with points notitle\n";
            } else {
                print SCATTER "plot $quoted_path using 1:2 with points notitle\n";
            }

            $quoted_path = qq("$path_graphics/$function_id+$algorithm_id.pdf");
            print SCATTER
                "$terminal{pdf} $font\n",
                "set output $quoted_path\n",
                "replot\n";

            $quoted_path = qq("$path_graphics/$function_id+$algorithm_id.png");
            print SCATTER
                "$terminal{png} $font\n",
                "set output $quoted_path\n",
                "replot\n";

        }

        print SCATTER "\n";

    }

    system("chmod a+x scatter.gp");

}

sub generate_table_rank_distribution
{
    my $path = "$path_report/table-rank-distribution.tex";
    open(LATEX, ">$path")
        or die "hnco-benchmark-stat.pl: generate_table_rank_distribution: Cannot open $path\n";
    latex_table_rank_distribution_begin();
    latex_table_rank_distribution_body();
    latex_table_rank_distribution_end();
    close LATEX;
}

sub generate_table_functions
{
    foreach my $fn (@$functions) {
        generate_table_value($fn);
        generate_table_time($fn);
    }
}

sub generate_table_value
{
    my $fn = shift;

    my $function_id = $fn->{id};

    my $rounding_value_before = $fn->{rounding}->{value}->{before} || 3;
    my $rounding_value_after = $fn->{rounding}->{value}->{after} || 0;

    my $value = $stat_value->{$function_id};
    my $best = $stat_value_best->{$function_id};

    my $path = "$path_report/table-value-$function_id.tex";
    open(LATEX, ">$path")
        or die "hnco-benchmark-stat.pl: generate_table_value: Cannot open $path\n";

    latex_table_value_begin(">{{\\nprounddigits{$rounding_value_after}}}N{$rounding_value_before}{$rounding_value_after}");

    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        latex_table_value_add_line($algorithm_id,
                                   $value->{$algorithm_id},
                                   $best,
                                   $fn->{logscale});
    }
    latex_table_end();

    close LATEX;
}

sub generate_table_time
{
    my $fn = shift;

    my $function_id = $fn->{id};
    my $rounding_time_before = $fn->{rounding}->{time}->{before} || 1;
    my $rounding_time_after = $fn->{rounding}->{time}->{after} || 2;
    my $total_time = $stat_total_time->{$function_id};
    my $evaluation_time = $stat_evaluation_time->{$function_id};
    my $algorithm_time = $stat_algorithm_time->{$function_id};

    my $path = "$path_report/table-time-$function_id.tex";
    open(LATEX, ">$path")
        or die "hnco-benchmark-stat.pl: generate_table_time: Cannot open $path\n";

    latex_table_time_begin(">{{\\nprounddigits{$rounding_time_after}}}N{$rounding_time_before}{$rounding_time_after}");
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        latex_table_time_add_line($algorithm_id,
                                  $total_time->{$algorithm_id},
                                  $evaluation_time->{$algorithm_id},
                                  $algorithm_time->{$algorithm_id});
    }
    latex_table_end();

    close LATEX;
}

sub generate_ranking
{
    my $path = "$path_report/ranking.tex";
    open(LATEX, ">$path")
        or die "hnco-benchmark-stat.pl: generate_ranking: Cannot open $path\n";

    print LATEX "Per function rankings (ex-eaquo are grouped in parentheses):\n";
    print LATEX "\\begin{description}\n";
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        print LATEX "\\item[$function_id]\n";
        print LATEX join (", ",
                          map {
                              if (@{ $_ } == 1) {
                                  join(", ", @{ $_ });
                              } else {
                                  "(" . join(", ", @{ $_ }) . ")";
                              }
                          } @{ $ranking->{$function_id} });
        print LATEX "\n\n";
    }
    print LATEX "\\end{description}\n";

    close LATEX;
}

sub generate_latex
{
    open(LATEX, ">$path_report/content.tex")
        or die "hnco-benchmark-stat.pl: generate_latex: Cannot open $path_report/content.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    print LATEX latex_section("Ranking");

    print LATEX latex_begin_center();
    print LATEX latex_input_file("table-rank-distribution.tex");
    print LATEX latex_end_center();

    print LATEX latex_input_file("ranking.tex");

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $value = $stat_value->{$function_id};
        my $best = $stat_value_best->{$function_id};
        my $total_time = $stat_total_time->{$function_id};

        print LATEX latex_newpage();

        print LATEX latex_section("Function $function_id");

        print LATEX latex_begin_center();
        print LATEX latex_input_file("table-value-$function_id.tex");
        print LATEX latex_end_center();

        print LATEX latex_begin_center();
        print LATEX latex_input_file("table-time-$function_id.tex");
        print LATEX latex_end_center();

        print LATEX latex_begin_figure();
        print LATEX latex_includegraphics("$function_id");
        print LATEX latex_caption("$function_id");
        print LATEX latex_end_figure();

        print LATEX latex_begin_figure();
        print LATEX latex_includegraphics("$function_id+all");
        print LATEX latex_caption("$function_id");
        print LATEX latex_end_figure();
    }

    close LATEX;
}

sub latex_table_value_begin
{
    my $rounding_value = shift;

    print LATEX
        "\\begin{tabular}{\@{}l*{5}{$rounding_value}>{{\\nprounddigits{0}}}N{2}{0}\@{}}\n",
        "\\toprule\n",
        "{algorithm} & \\multicolumn{6}{l}{{function value}} \\\\\n",
        "\\midrule\n",
        "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & {rk}\\\\\n",
        "\\midrule\n";
}

sub latex_table_value_add_line
{
    my ($algo, $perf, $best, $logscale) = @_;

    print LATEX "$algo & ";

    my $conversion = "%f";
    if ($logscale) {
        $conversion = "%e";
    }

    my $format = join " & ", map { $perf->{$_} == $best->{$_} ? "{\\color{blue}} $conversion" : "$conversion" } @summary_statistics;

    printf LATEX ($format, $perf->{min}, $perf->{q1}, $perf->{median}, $perf->{q3}, $perf->{max});

    printf LATEX (" & %d\\\\\n", $perf->{rank} + 1);
}

sub latex_table_time_begin
{
    my $rounding_time = shift;

    print LATEX
        "\\begin{tabular}{\@{}l*{6}{$rounding_time}\@{}}\n",
        "\\toprule\n",
        "{algorithm} & \\multicolumn{2}{l}{{algo. time (s)}} & \\multicolumn{2}{l}{{eval. time (s)}} & \\multicolumn{2}{l}{{total time (s)}} \\\\\n",
        "\\midrule\n",
        "& {mean} & {dev.} & {mean} & {dev.} & {mean} & {dev.} \\\\\n",
        "\\midrule\n";
}

sub latex_table_time_add_line
{
    my ($algo, $total_time, $evaluation_time, $algorithm_time) = @_;

    print LATEX "$algo";
    printf LATEX (" & %f & %f & %f & %f & %f & %f \\\\\n ",
                  $algorithm_time->{mean}, $algorithm_time->{stddev},
                  $evaluation_time->{mean}, $evaluation_time->{stddev},
                  $total_time->{mean}, $total_time->{stddev});
}

sub latex_table_end
{
    print LATEX <<EOF;
\\bottomrule
\\end{tabular}
EOF
}

sub latex_table_rank_distribution_begin
{
    my $num_algo = @$algorithms;
    print LATEX
        "\\begin{tabular}{\@{}l*{$num_algo}{r}\@{}}\n",
        "\\toprule\n",
        "algorithm & \\multicolumn{$num_algo}{l}{{rank distribution}}\\\\\n",
        "\\midrule\n",
        "& ", join(" & ", 1 .. $num_algo), "\\\\\n",
        "\\midrule\n";
}

sub latex_table_rank_distribution_body
{
    my @order = sort {
        foreach (0 .. @$algorithms - 1) {
            if (${ $rank_distribution->{$b} }[$_] != ${ $rank_distribution->{$a} }[$_]) {
                return ${ $rank_distribution->{$b} }[$_] <=> ${ $rank_distribution->{$a} }[$_];
            }
        }
        return 0;
    } keys %$rank_distribution;

    foreach (@order) {
        print LATEX "$_ & ", join(" & ", @{ $rank_distribution->{$_} }), "\\\\\n";
    }

}

sub latex_table_rank_distribution_end
{
    print LATEX
        "\\bottomrule\n",
        "\\end{tabular}\n";
}
