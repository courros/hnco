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
use List::MoreUtils qw(all);
use File::Slurp qw(read_file);

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

my $path_graphics       = "graphics";
my $path_report         = "report";
my $path_results        = "results";

my @summary_statistics = qw(median q1 q3 min max);
my @summary_statistics_display = qw(min q1 median q3 max);

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
my $parameter           = $obj->{parameter};
my $graphics            = $obj->{graphics};

my $parameter_id        = $parameter->{id};
my $parameter_name      = $parameter->{name} || $parameter_id;
my $parameter_shortname = $parameter->{shortname} || $parameter_name;
my $boxwidth            = $parameter->{boxwidth};
my $categories          = $parameter->{categories};

my $all_stat = {};
my $all_stat_flat = {};
my $all_best = {};

my $rankings = {};

my @rankings_flat = ();

#
# Parameter values
#

my $num_lines = @$algorithms * @$categories;

foreach my $a (@$algorithms) {
    my $algorithm_id = $a->{id};
    foreach my $category (@$categories) {
        my $value = $category->{value};
        my @counts = (0) x ($num_lines); # Array of size $num_lines initialized to 0
        $rankings->{$algorithm_id}->{$value} = \@counts;
    }
}

#
# Processing
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

add_missing_names($functions);
add_missing_names($algorithms);
compute_statistics();
compute_rankings();
compute_rankings_flat();
compute_best_statistics();
reverse_values();
reverse_best_statistics();
generate_data();
generate_data_histogram_by_category();
generate_data_histogram_by_algorithm();
generate_gnuplot_candlesticks();
generate_gnuplot_histogram();
generate_latex();

#
# Local functions
#

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_stat = {};
        my @items = ();

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $algorithm_stat = {};
            my $algorithm_num_runs = $num_runs;
            if ($a->{deterministic}) {
                $algorithm_num_runs = 1;
            }

            foreach my $category (@$categories) {
                my $value = $category->{value};
                my $prefix = "$path_results/$function_id/$algorithm_id/$parameter_id-$value";
                my $SD_value = Statistics::Descriptive::Full->new();

                foreach (1 .. $algorithm_num_runs) {
                    my $obj = from_json(read_file("$prefix/$_.out"));
                    $SD_value->add_data($obj->{value});
                }

                $algorithm_stat->{$value} = { min         => $SD_value->min(),
                                              q1          => $SD_value->quantile(1),
                                              median      => $SD_value->median(),
                                              q3          => $SD_value->quantile(3),
                                              max         => $SD_value->max(),
                                              mean        => $SD_value->mean(),
                                              stddev      => $SD_value->standard_deviation() };

                my $item = { algorithm  => $algorithm_id,
                             value      => $value,
                             min        => $SD_value->min(),
                             q1         => $SD_value->quantile(1),
                             median     => $SD_value->median(),
                             q3         => $SD_value->quantile(3),
                             max        => $SD_value->max() };
                push @items, $item;

            }

            $function_stat->{$algorithm_id} = $algorithm_stat;

        }

        $all_stat->{$function_id} = $function_stat;
        $all_stat_flat->{$function_id} = \@items;
    }

}

sub generate_data
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $prefix = "$path_results/$function_id/$algorithm_id";

            my $path = "$prefix/mean.dat";
            my $mean = IO::File->new($path, '>')
                or die "hnco-algorithm-category-stat.pl: generate_data: Cannot open '$path': $!\n";

            $path = "$prefix/quartiles.dat";
            my $quartiles = IO::File->new($path, '>')
                or die "hnco-algorithm-category-stat.pl: generate_data: Cannot open '$path': $!\n";

            foreach my $category (@$categories) {
                my $value = $category->{value};
                my $stat = $all_stat->{$function_id}->{$algorithm_id}->{$value};
                $quartiles->printf("%e %e %e %e %e %e\n",
                                   $value,
                                   $stat->{min},
                                   $stat->{q1},
                                   $stat->{median},
                                   $stat->{q3},
                                   $stat->{max});
                $mean->printf("%e %e %e\n",
                              $value,
                              $stat->{mean},
                              $stat->{stddev});
            }

            $quartiles->close();
            $mean->close();

        }

    }

}

sub generate_data_histogram_by_category
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$path_results/$function_id/by-category.dat";
        open(DATA, ">$path")
            or die "hnco-algorithm-category-stat.pl: generate_data_histogram_by_category: Cannot open '$path': $!\n";
        print DATA "algorithm ";
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            print DATA "$algorithm_id ";
        }
        print DATA "\n";
        foreach my $category (@$categories) {
            my $value = $category->{value};
            my $name = $category->{name};
            print DATA "$name ";
            foreach my $a (@$algorithms) {
                my $algorithm_id = $a->{id};
                my $stat = $all_stat->{$function_id}->{$algorithm_id}->{$value};
                print DATA "$stat->{mean} ";
            }
            print DATA "\n";
        }
        close(DATA);
    }
}

sub generate_data_histogram_by_algorithm
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$path_results/$function_id/by-algorithm.dat";
        open(DATA, ">$path")
            or die "hnco-algorithm-category-stat.pl: generate_data_histogram_by_algorithm: Cannot open '$path': $!\n";
        print DATA qq("$parameter_name");
        print DATA " ";
        foreach my $category (@$categories) {
            my $value = $category->{value};
            my $name = $category->{name};
            print DATA "$name ";
        }
        print DATA "\n";
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            print DATA "$algorithm_id ";
            foreach my $category (@$categories) {
                my $value = $category->{value};
                my $name = $category->{name};
                my $stat = $all_stat->{$function_id}->{$algorithm_id}->{$value};
                print DATA "$stat->{mean} ";
            }
            print DATA "\n";
        }
        close(DATA);
    }
}

sub compute_rankings
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        # Sort results
        my @sorted = sort {
            foreach (@summary_statistics) {
                # Decreasing order
                if ($b->{$_} != $a->{$_}) {
                    return $b->{$_} <=> $a->{$_};
                }
            }
            return 0;
        } @{ $all_stat_flat->{$function_id} };
        $all_stat_flat->{$function_id} = \@sorted;

        # Set ranks
        for (my $i = 0; $i < @sorted; $i++) {
            $sorted[$i]->{rank} = $i;
        }

        # Handle ex-aequo
        for (my $i = 1; $i < @sorted; $i++) {
            if (all { $sorted[$i]->{$_} == $sorted[$i - 1]->{$_} } @summary_statistics) {
                $sorted[$i]->{rank} = $sorted[$i - 1]->{rank};
            }
        }

        # Update rank_counts and store rank
        foreach (@sorted) {
            ${ $rankings->{$_->{algorithm}}->{$_->{value}} }[$_->{rank}]++;
            $all_stat->{$function_id}->{$_->{algorithm}}->{$_->{value}}->{rank} = $_->{rank};
        }

    }

}

sub compute_rankings_flat
{
    @rankings_flat = ();
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        foreach my $category (@$categories) {
            my $value = $category->{value};
            my $item = {
                algorithm       => $algorithm_id,
                value           => $value,
                rank_counts     => $rankings->{$algorithm_id}->{$value}
            };
            push @rankings_flat, $item;
        }
    }

    my @sorted = sort {
        foreach (0 .. $num_lines - 1) {
            if (${ $b->{rank_counts} }[$_] != ${ $a->{rank_counts} }[$_]) {
                return ${ $b->{rank_counts} }[$_] <=> ${ $a->{rank_counts} }[$_];
            }
        }
        return 0;
    } @rankings_flat;

    @rankings_flat = @sorted;
}

sub compute_best_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_best = {};

        foreach (@summary_statistics) {
            my @sorted = sort { $b->{$_} <=> $a->{$_} } @{ $all_stat_flat->{$function_id} };
            $function_best->{$_} = $sorted[0]->{$_};
        }
        $all_best->{$function_id} = $function_best;

    }

}

sub reverse_values
{
    foreach my $f (@$functions) {
        if ($f->{reverse}) {
            foreach my $a (@$algorithms) {
                foreach my $category (@$categories) {
                    my $value = $category->{value};
                    my $stat = $all_stat->{$f->{id}}->{$a->{id}}->{$value};
                    ($stat->{min}, $stat->{max}) = ($stat->{max}, $stat->{min});
                    ($stat->{q1}, $stat->{q3}) = ($stat->{q3}, $stat->{q1});
                    foreach (@summary_statistics) {
                        $stat->{$_} = -$stat->{$_};
                    }
                    $stat->{mean} = -$stat->{mean};
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
            my $function_best = $all_best->{$function_id};
            ($function_best->{min}, $function_best->{max}) = ($function_best->{max}, $function_best->{min});
            ($function_best->{q1}, $function_best->{q3}) = ($function_best->{q3}, $function_best->{q1});
            foreach (@summary_statistics) {
                $function_best->{$_} = -$function_best->{$_};
            }
        }
    }
}

sub generate_gnuplot_candlesticks
{
    open(CANDLESTICKS, ">candlesticks.gp")
        or die "hnco-algorithm-category-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

    print CANDLESTICKS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        qq(set xlabel "$parameter_name"\n),
        qq(set ylabel "Function value"\n),
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

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
            print CANDLESTICKS
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print CANDLESTICKS
                "unset logscale y\n",
                "set format y\n";
        }

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            my $quoted_string;

            if ($graphics->{candlesticks}->{title}) {
                $quoted_string = qq("$algorithm_id on $function_id");
                print CANDLESTICKS "set title $quoted_string\n";
            }

            $quoted_string = qq("$path_graphics/$function_id/$algorithm_id.pdf");
            print CANDLESTICKS
                "$terminal{pdf} $font\n",
                "set output $quoted_string\n";
            $quoted_string = qq("$path_results/$function_id/$algorithm_id/quartiles.dat");
            print CANDLESTICKS
                "plot $quoted_string using 1:3:2:6:5:($boxwidth) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                "     $quoted_string using 1:4:4:4:4:($boxwidth) with candlesticks lw 2 lt 1 notitle\n";

            $quoted_string = qq("$path_graphics/$function_id/$algorithm_id.eps");
            print CANDLESTICKS
                "$terminal{eps} $font\n",
                "set output $quoted_string\n",
                "replot\n";

            $quoted_string = qq("$path_graphics/$function_id/$algorithm_id.png");
            print CANDLESTICKS
                "$terminal{png} $font\n",
                "set output $quoted_string\n",
                "replot\n\n";

        }
    }

    system("chmod a+x candlesticks.gp");

}

sub generate_gnuplot_histogram
{
    open(HISTOGRAM, ">histogram.gp")
        or die "hnco-algorithm-category-stat.pl: generate_gnuplot_histogram: Cannot open histogram.gp\n";

    print HISTOGRAM
        "#!/usr/bin/gnuplot -persist\n",
        "set style data histograms\n",
        "set style histogram clustered\n",
        "set style fill solid 1.00 border lt -1\n",
        "set xtic rotate by -45 scale 0\n",
        "set boxwidth 0.8 relative\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # Font face and size
    my $font = "";
    if ($graphics->{histogram}->{font_face}) {
        $font = $graphics->{histogram}->{font_face};
    }
    if ($graphics->{histogram}->{font_size}) {
        $font = "$font,$graphics->{histogram}->{font_size}";
    }
    $font = qq(font "$font");

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }

        my $quoted_string = qq("$function_id: Mean value as a function of $parameter_name");
        print HISTOGRAM "set title $quoted_string\n";
        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print HISTOGRAM
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print HISTOGRAM
                "unset logscale y\n",
                "set format y\n";
        }

        # By category
        $quoted_string = qq("$path_graphics/$function_id/histogram-by-category.pdf");
        print HISTOGRAM
            "$terminal{pdf} $font\n",
            "set output $quoted_string\n";

        $quoted_string = qq("$path_results/$function_id/by-category.dat");
        print HISTOGRAM qq(set key font ",10" outside right top box vertical title "algorithm" font ",10"\n);
        print HISTOGRAM "plot for [COL=2:*] $quoted_string using COL:xticlabels(1) title columnhead\n";

        $quoted_string = qq("$path_graphics/$function_id/histogram-by-category.eps");
        print HISTOGRAM
            "$terminal{eps} $font\n",
            "set output $quoted_string\n",
            "replot\n";

        $quoted_string = qq("$path_graphics/$function_id/histogram-by-category.png");
        print HISTOGRAM
            "$terminal{png} $font\n",
            "set output $quoted_string\n",
            "replot\n\n";

        # By algorithm
        $quoted_string = qq("$path_graphics/$function_id/histogram-by-algorithm.pdf");
        print HISTOGRAM
            "$terminal{pdf} $font\n",
            "set output $quoted_string\n";

        $quoted_string = qq("$path_results/$function_id/by-algorithm.dat");
        print HISTOGRAM qq(set key font ",10" outside right top box vertical title "$parameter_name" font ",10"\n);
        print HISTOGRAM "plot for [COL=2:*] $quoted_string using COL:xticlabels(1) title columnhead\n";

        $quoted_string = qq("$path_graphics/$function_id/histogram-by-algorithm.eps");
        print HISTOGRAM
            "$terminal{eps} $font\n",
            "set output $quoted_string\n",
            "replot\n";

        $quoted_string = qq("$path_graphics/$function_id/histogram-by-algorithm.png");
        print HISTOGRAM
            "$terminal{png} $font\n",
            "set output $quoted_string\n",
            "replot\n\n";

    }

    system("chmod a+x histogram.gp");
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-algorithm-category-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    print LATEX latex_section("Rankings");
    print LATEX latex_begin_center();
    latex_rankings_table();
    print LATEX latex_end_center();

    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        my $function_best = $all_best->{$function_id};

        my $rounding_value_before = $fn->{rounding}->{value}->{before} || 3;
        my $rounding_value_after = $fn->{rounding}->{value}->{after} || 0;
        my $rounding_time_before = $fn->{rounding}->{time}->{before} || 1;
        my $rounding_time_after = $fn->{rounding}->{time}->{after} || 2;

        print LATEX latex_section("Function $function_id");

        print LATEX latex_begin_center();
        latex_function_table_begin(">{{\\nprounddigits{$rounding_value_after}}}N{$rounding_value_before}{$rounding_value_after}");
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            foreach my $category (@$categories) {
                my $value = $category->{value};
                latex_function_table_add_line($algorithm_id,
                                              $value,
                                              $all_stat->{$function_id}->{$algorithm_id}->{$value},
                                              $function_best,
                                              $fn->{logscale});
            }
        }
        latex_funtion_table_end();
        print LATEX latex_end_center();

        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id/histogram-by-category", 0.6);
        print LATEX latex_end_center();

        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id/histogram-by-algorithm", 0.6);
        print LATEX latex_end_center();

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            print LATEX latex_begin_center();
            print LATEX latex_includegraphics("$function_id/$algorithm_id", 0.6);
            print LATEX latex_end_center();

        }

    }

}

sub latex_rankings_table
{
    print LATEX
        "\\begin{tabular}{\@{}ll*{$num_lines}{r}\@{}}\n",
        "\\toprule\n",
        "algorithm & $parameter_shortname & \\multicolumn{$num_lines}{l}{{rank distribution}}\\\\\n",
        "\\midrule\n",
        "&& ", join(" & ", 1 .. $num_lines), "\\\\\n",
        "\\midrule\n";

    foreach (@rankings_flat) {
        print LATEX "$_->{algorithm} & $_->{value} & ", join(" & ", @{ $_->{rank_counts} }), "\\\\\n";
    }

    print LATEX
        "\\bottomrule\n",
        "\\end{tabular}\n";
}

sub latex_function_table_begin
{
    my $col = shift;

    print LATEX
        "\\begin{tabular}{\@{}ll*{5}{$col}>{{\\nprounddigits{0}}}N{2}{0}\@{}}\n",
        "\\toprule\n",
        "{algorithm} & {$parameter_shortname} & \\multicolumn{6}{l}{{function value}} \\\\\n",
        "\\midrule\n",
        "& & {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & {rk} \\\\\n",
        "\\midrule\n";
}

sub latex_function_table_add_line
{
    my ($algo, $value, $stat, $best, $logscale) = @_;

    my $conversion = $logscale ? "%e" : "%f";
    my $format = join " & ",
        map { $stat->{$_} == $best->{$_} ? "{\\color{blue}} $conversion" : "$conversion" } @summary_statistics_display;

    printf LATEX "$algo & $value & ";
    printf LATEX ($format, $stat->{min}, $stat->{q1}, $stat->{median}, $stat->{q3}, $stat->{max});
    printf LATEX (" & %d \\\\\n", $stat->{rank} + 1);

}

sub latex_funtion_table_end
{
    print LATEX <<EOF;
\\bottomrule
\\end{tabular}
EOF
}
