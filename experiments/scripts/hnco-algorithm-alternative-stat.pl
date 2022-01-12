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

my $path_results        = "results";
my $path_graphics       = "graphics";
my $path_report         = "report";

my @summary_statistics = qw(min q1 median q3 max);
my @summary_statistics_reverse = reverse @summary_statistics;

my @preference_max = qw(median q3 q1 max min);
my @preference_min = qw(median q1 q3 min max);

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
my $parameter   = $obj->{parameter};

my $parameter_id        = $parameter->{id};
my $parameter_name      = $parameter->{name} || $parameter_id;
my $parameter_shortname = $parameter->{shortname} || $parameter_name;
my $alternatives        = $parameter->{alternatives};

my @results = ();
my @value_statistics = ();
my @rank_statistics = ();

# hash indexed by algorithm, alternative
my $rank_data = {};

# hash indexed by function, algorithm, alternative
my $mean_data = {};

#
# Processing
#

add_missing_names($functions);
add_missing_names($algorithms);

load_results();

compute_statistics();
compute_rank_data();
compute_rank_statistics();

generate_table_rank();
generate_table_value();
generate_histogram_data();
generate_histogram_gnuplot();
generate_latex();

#
# Make directories
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

#
# Local functions
#

sub load_results
{
    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            foreach my $alternative (@$alternatives) {
                my $alternative_value = $alternative->{value};
                my $prefix = "$path_results/$function_id/$algorithm_id/$parameter_id-$alternative_value";
                foreach (1 .. $num_runs) {
                    my $obj = from_json(read_file("$prefix/$_.out"));
                    push @results, { function           => $function_id,
                                     algorithm          => $algorithm_id,
                                     alternative_value  => $alternative_value,
                                     run                => $_,
                                     value              => $obj->{value},
                                     num_evaluations    => $obj->{num_evaluations},
                                     total_time         => $obj->{total_time},
                                     evaluation_time    => $obj->{evaluation_time},
                                     algorithm_time     => $obj->{total_time} - $obj->{evaluation_time} };
                }
            }
        }
    }
}

sub compute_statistics
{
    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        my @rows1 = grep { $_->{function} eq $function_id } @results;
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my @rows2 = grep { $_->{algorithm} eq $algorithm_id } @rows1;
            foreach my $alternative (@$alternatives) {
                my $alternative_value = $alternative->{value};
                my @rows3 = grep { $_->{alternative_value} eq $alternative_value } @rows2;
                my @values = map { $_->{value} } @rows3;
                my $sd = Statistics::Descriptive::Full->new();
                $sd->add_data(@values);
                push @value_statistics, { function => $function_id,
                                          algorithm => $algorithm_id,
                                          alternative_value => $alternative_value,
                                          alternative_name => $alternative->{name},
                                          min => $sd->min(),
                                          q1 => $sd->quantile(1),
                                          median => $sd->median(),
                                          q3 => $sd->quantile(3),
                                          max => $sd->max() };
                $mean_data->{$function_id}->{$algorithm_id}->{$alternative_value} = $sd->mean();
            }
        }
    }
}

sub compute_rank_data
{
    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};
        foreach my $alternative (@$alternatives) {
            my $alternative_value = $alternative->{value};
            $rank_data->{$algorithm_id}->{$alternative_value} = [];
        }
    }

    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};

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
            my $alternative_value = $_->{alternative_value};
            push @{ $rank_data->{$algorithm_id}->{$alternative_value} }, $_->{rank};
        }
    }
}

sub compute_rank_statistics
{
    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};
        foreach my $alternative (@$alternatives) {
            my $alternative_value = $alternative->{value};
            my $sd = Statistics::Descriptive::Full->new();
            $sd->add_data(@{ $rank_data->{$algorithm_id}->{$alternative_value} });
            push @rank_statistics, { algorithm => $algorithm_id,
                                     alternative_name => $alternative->{name},
                                     min => $sd->min(),
                                     q1 => $sd->quantile(1),
                                     median => $sd->median(),
                                     q3 => $sd->quantile(3),
                                     max => $sd->max() };
        }
    }
}

sub generate_histogram_data
{
    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        my $path = "$path_results/$function_id/histogram.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-algorithm-alternative-stat.pl: generate_histogram_data: Cannot open $path\n";
        $file->print(qq("$parameter_name"), " ");
        $file->print(join " ", map { $_->{name} } @$alternatives);
        $file->print("\n");
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            $file->print("$algorithm_id ");
            foreach my $alternative (@$alternatives) {
                my $alternative_value = $alternative->{value};
                my $mean = $mean_data->{$function_id}->{$algorithm_id}->{$alternative_value};
                if ($fn->{reverse}) {
                    $mean = -$mean;
                }
                $file->print("$mean ");
            }
            $file->print("\n");
        }
        $file->close();
    }
}

sub generate_histogram_gnuplot
{
    my $path = "histogram.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-alternative-stat.pl: generate_histogram_gnuplot: Cannot open histogram.gp\n";

    my $context = $graphics->{histogram};
    # Orientation of labels
    my $angle = "-45";
    if (exists($context->{angle})) {
        $angle = $context->{angle};
    }
    # Font face and size
    my $font = "";
    if ($context->{font_face}) {
        $font = $context->{font_face};
    }
    if ($context->{font_size}) {
        $font = "$font,$context->{font_size}";
    }
    $font = qq(font "$font");

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set style data histograms\n",
                 "set style histogram clustered gap 1\n",
                 "set style fill solid 1.00 border lt -1\n",
                 "set xtic rotate by $angle scale 0\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n",
                 qq(set key font ",12" right top box vertical title "$parameter_name" font ",12"\n\n));

    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};

        if ($context->{title}) {
            $file->print(qq(set title "$function_id: Mean value as a function of $parameter_name"\n));
        }
        if ($fn->{logscale}) {
            my $fmt = qq("10^{\%T}");
            $file->print("set logscale y 10\n",
                         "set format y $fmt\n");
        } else {
            $file->print("unset logscale y\n",
                         "set format y\n");
        }
        $file->print("\n");

        my $quoted_string = qq("$path_graphics/histogram.$function_id.pdf");
        $file->print("$terminal{pdf} $font\n",
                     "set output $quoted_string\n");

        $quoted_string = qq("$path_results/$function_id/histogram.dat");
        $file->print("plot for [COL=2:*] $quoted_string using COL:xticlabels(1) title columnhead\n");

        $quoted_string = qq("$path_graphics/histogram.$function_id.eps");
        $file->print("$terminal{eps} $font\n",
                     "set output $quoted_string\n",
                     "replot\n");

        $quoted_string = qq("$path_graphics/histogram.$function_id.png");
        $file->print("$terminal{png} $font\n",
                     "set output $quoted_string\n",
                     "replot\n\n");
    }
    $file->close();
    system("chmod a+x histogram.gp");
}

sub generate_table_rank
{
    my $path = "$path_report/table.rank.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-algorithm-alternative-stat.pl: generate_table_rank: Cannot open $path\n";

    # Header
    my $width = int(ceil(log(1 + @{ $algorithms } * @{ $alternatives }) / log(10)));
    my $quantiles = "";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{1}}}n{$width}{1}";
    $quantiles .= ">{{\\nprounddigits{2}}}n{$width}{2}";
    $quantiles .= ">{{\\nprounddigits{0}}}n{$width}{0}";

    $file->print("\\begin{tabular}{\@{} ll $quantiles \@{}}\n",
                 "\\toprule\n",
                 "{Algo.} & {$parameter_shortname} & \\multicolumn{5}{l}{{Rank}} \\\\\n",
                 "\\midrule\n",
                 "&& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} \\\\\n",
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
        $file->print("$row->{algorithm} & $row->{alternative_name} & ");
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
            or die "hnco-algorithm-alternative-stat.pl: generate_table_value: Cannot open $path\n";

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
        $quantiles .= ">{{\\nprounddigits{$after_min_max}}}n{$before}{0}";
        $quantiles .= ">{{\\nprounddigits{$after_quantiles}}}n{$before}{$after_quantiles}";
        $quantiles .= ">{{\\nprounddigits{$after_median}}}n{$before}{$after_median}";
        $quantiles .= ">{{\\nprounddigits{$after_quantiles}}}n{$before}{$after_quantiles}";
        $quantiles .= ">{{\\nprounddigits{$after_min_max}}}n{$before}{0}";

        $file->print("\\begin{tabular}{\@{} ll $quantiles \@{}}\n",
                     "\\toprule\n",
                     "{Algo.} & {$parameter_shortname} & \\multicolumn{5}{l}{{Value}} \\\\\n",
                     "\\midrule\n",
                     "&& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} \\\\\n",
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
        foreach my $row (@sorted) {
            $file->print("$row->{algorithm} & $row->{alternative_name} & ");
            if ($fn->{reverse}) {
                my $format = join " & ", map { "$conversion" } @summary_statistics_reverse;
                $file->printf($format,
                              -$row->{max},
                              -$row->{q3},
                              -$row->{median},
                              -$row->{q1},
                              -$row->{min});
            } else {
                my $format = join " & ", map { "$conversion" } @summary_statistics;
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
        or die "hnco-algorithm-alternative-stat.pl: generate_latex: Cannot open $path_report/content.tex\n";
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
                     latex_includegraphics("histogram.$id", 0.6),
                     latex_end_center());
    }
    $file->close();
}
