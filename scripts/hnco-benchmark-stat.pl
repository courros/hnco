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
use Statistics::Descriptive;
use List::Util qw(min max);
use List::MoreUtils qw(all);

my @summary_statistics = qw(min q1 median q3 max);
my @pref_max = qw(median q1 q3 min max);
my @pref_min = qw(median q3 q1 max min);

my $plan = "plan.json";
open(FILE, $plan) or die "hnco-benchmark-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) { $json .= $_; }

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $path_graphics       = $obj->{graphics};
my $path_results        = $obj->{results};
my $path_report         = $obj->{report};

my $ranges = {};
my $stat_eval = {};
my $stat_time = {};
my $stat_value = {};
my $stat_value_best = {};

my $rankings = {};
foreach my $a (@$algorithms) {
    my $algorithm_id = $a->{id};
    my @counts = (0) x @$algorithms;
    $rankings->{$algorithm_id} = \@counts;
}

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

compute_statistics();
compute_statistics_time();
compute_best_statistics();
compute_rankings();
reverse_values();
reverse_best_statistics();
compute_ranges();

generate_function_data();
generate_gnuplot_candlesticks();
generate_gnuplot_clouds();
generate_latex();

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $eval = {};
        my $value = {};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $algorithm_num_runs = $num_runs;
            if ($a->{deterministic}) {
                $algorithm_num_runs = 1;
            }

            my $prefix = "$path_results/$function_id/$algorithm_id";
            my $SD_eval = Statistics::Descriptive::Full->new();
            my $SD_value = Statistics::Descriptive::Full->new();

            my $path = "$prefix/$algorithm_id.dat";
            my $file_data = IO::File->new($path, '>')
                or die "hnco-benchmark-stat.pl: compute_statistics: Cannot open '$path': $!\n";
            foreach (1 .. $algorithm_num_runs) {
                $path = "$prefix/$_.out";
                my $file_run = IO::File->new($path, '<')
                    or die "hnco-benchmark-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                my $line = $file_run->getline;
                $file_data->print($line);
                chomp $line;
                my @results = split ' ', $line;
                $SD_eval->add_data($results[0]);
                $SD_value->add_data($results[1]);
                $file_run->close;
            }
            $file_data->close;

            $eval->{$algorithm_id} = { min     => $SD_eval->min(),
                                       q1      => $SD_eval->quantile(1),
                                       median  => $SD_eval->median(),
                                       q3      => $SD_eval->quantile(3),
                                       max     => $SD_eval->max() };

            $value->{$algorithm_id} = { min     => $SD_value->min(),
                                        q1      => $SD_value->quantile(1),
                                        median  => $SD_value->median(),
                                        q3      => $SD_value->quantile(3),
                                        max     => $SD_value->max() };

        }

        $stat_eval->{$function_id} = $eval;
        $stat_value->{$function_id} = $value;

    }

}

sub compute_statistics_time
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $time = {};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $algorithm_num_runs = $num_runs;
            if ($a->{deterministic}) {
                $algorithm_num_runs = 1;
            }

            my $prefix = "$path_results/$function_id/$algorithm_id";
            my $SD_time = Statistics::Descriptive::Full->new();

            foreach (1 .. $algorithm_num_runs) {
                my $path = "$prefix/$_.time";
                my $file = IO::File->new($path, '<')
                    or die "hnco-benchmark-stat.pl: compute_statistics_time: Cannot open '$path': $!\n";
                my $line = $file->getline;
                $SD_time->add_data($line);
                $file->close;
            }

            $time->{$algorithm_id} = { mean     => $SD_time->mean(),
                                       stddev   => $SD_time->standard_deviation() };

        }

        $stat_time->{$function_id} = $time;

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

sub compute_rankings
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $stat = $stat_value->{$function_id};

        # Sort algorithm by decreasing order of performance
        @sorted = sort {
            foreach (@pref_max) {
                if ($stat->{$b}->{$_} != $stat->{$a}->{$_}) {
                    return $stat->{$b}->{$_} <=> $stat->{$a}->{$_};
                }
            }
            return 0;
        } keys %$stat;

        # Set ranks
        for (my $i = 0; $i < @sorted; $i++) {
            $stat->{$sorted[$i]}->{rank} = $i;
        }

        # Handle ex-aequo
        for (my $i = 1; $i < @sorted; $i++) {
            if (all { $stat->{$sorted[$i]}->{$_} == $stat->{$sorted[$i - 1]}->{$_} } @summary_statistics) {
                $stat->{$sorted[$i]}->{rank} = $stat->{$sorted[$i - 1]}->{rank};
            }
        }

        # Update rankings
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            ${ $rankings->{$algorithm_id} }[$stat->{$algorithm_id}->{rank}]++; # Update rankings
        }

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

        $file = IO::File->new($path, '>')
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

sub generate_gnuplot_candlesticks
{
    open(CANDLESTICKS, ">candlesticks.gp") or die "hnco-benchmark-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

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

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = quote("10^{\%T}");
            print CANDLESTICKS
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print CANDLESTICKS
                "unset logscale\n",
                "set format y\n";
        }

        my $quoted_path = quote("$path_graphics/$function_id.pdf");
        print CANDLESTICKS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n";
        $quoted_path = quote("$path_results/$function_id/$function_id.dat");
        print CANDLESTICKS
            "plot $quoted_path using 1:3:2:6:5:xticlabels(7) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
            "     $quoted_path using 1:4:4:4:4 with candlesticks lw 3 lt 1 notitle\n";

        $quoted_path = quote("$path_graphics/$function_id.eps");
        print CANDLESTICKS
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id.png");
        print CANDLESTICKS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    system("chmod a+x candlesticks.gp");

}

sub generate_gnuplot_clouds
{
    open(CLOUDS, ">clouds.gp")
        or die "hnco-benchmark-stat.pl: generate_gnuplot_clouds: Cannot open clouds.gp\n";

    print CLOUDS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"Number of evaluations (x10^3)\"\n",
        "set ylabel \"Function value\"\n",
        "set format x ", quote("%.0s"), "\n",
        "set key outside top center box opaque horizontal\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # One function one file
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = quote("10^{\%T}");
            print CLOUDS
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print CLOUDS
                "unset logscale y\n",
                "set format y\n";
        }

        my $quoted_path;
        my $quoted_title;

        $quoted_path = quote("$path_graphics/$function_id+all.eps");
        print CLOUDS
            $terminal{eps}, "\n",
            "set output $quoted_path\n";

        print CLOUDS "plot \\\n";
        print CLOUDS
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $value = $stat_value->{$function_id}->{$algorithm_id};
                $quoted_path = quote("$path_results/$function_id/$algorithm_id/$algorithm_id.dat");
                $quoted_title = quote("$algorithm_id");
                $f->{reverse} ?
                    "  $quoted_path using 1:(-\$2) with points title $quoted_title" :
                    "  $quoted_path using 1:2 with points title $quoted_title";
             } @$algorithms);

        print CLOUDS "\n";

        $quoted_path = quote("$path_graphics/$function_id+all.pdf");
        print CLOUDS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id+all.png");
        print CLOUDS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            $fmt = quote("10^{\%T}");
            print CLOUDS
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print CLOUDS
                "unset logscale y\n",
                "set format y\n";
        }

        my $range = $ranges->{$function_id};
        my $xmin = $range->{xmin};
        my $xmax = $range->{xmax};
        my $ymin = $range->{ymin};
        my $ymax = $range->{ymax};

        print CLOUDS
            "set xrange [$xmin:$xmax]\n",
            ($ymin == $ymax) ? "set autoscale y\n" : "set yrange [$ymin:$ymax]\n";

        print CLOUDS "\n";

        my $quoted_path;
        my $quoted_title;

        # One function many files
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $value = $stat_value->{$function_id}->{$algorithm_id};

            $quoted_path = quote("$path_graphics/$function_id+$algorithm_id.eps");
            print CLOUDS
                $terminal{eps}, "\n",
                "set output $quoted_path\n";

            $quoted_path = quote("$path_results/$function_id/$algorithm_id/$algorithm_id.dat");
            if ($f->{reverse}) {
                print CLOUDS "plot $quoted_path using 1:(-\$2) with points notitle\n";
            } else {
                print CLOUDS "plot $quoted_path using 1:2 with points notitle\n";
            }

            $quoted_path = quote("$path_graphics/$function_id+$algorithm_id.pdf");
            print CLOUDS
                $terminal{pdf}, "\n",
                "set output $quoted_path\n",
                "replot\n";

            $quoted_path = quote("$path_graphics/$function_id+$algorithm_id.png");
            print CLOUDS
                $terminal{png}, "\n",
                "set output $quoted_path\n",
                "replot\n";

        }

        print CLOUDS "\n";

    }

    system("chmod a+x clouds.gp");

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex") or die "hnco-benchmark-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    latex_section("Rankings");
    latex_begin_center();
    latex_rankings_table_begin();
    latex_rankings_table_body();
    latex_rankings_table_end();
    latex_end_center();
    latex_empty_line();

    # latex_section("Algorithm index table");
    # latex_index_table();
    # latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $value = $stat_value->{$function_id};
        my $best = $stat_value_best->{$function_id};
        my $time = $stat_time->{$function_id};

        latex_newpage();

        latex_section("Function $function_id");

        latex_begin_center();
        if (exists($f->{col})) {
            latex_function_table_begin($f->{col});
        } else {
            latex_function_table_begin("N{2}{3}");
        }
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            latex_function_table_add_line($algorithm_id,
                                          $value->{$algorithm_id},
                                          $best,
                                          $f->{logscale},
                                          $time->{$algorithm_id});
        }
        latex_funtion_table_end();
        latex_end_center();

        latex_empty_line();

        latex_begin_center();
        latex_begin_figure();
        latex_includegraphics("$function_id");
        latex_caption("$function_id");
        latex_end_figure();
        latex_end_center();

        latex_empty_line();

        latex_begin_center();
        latex_begin_figure();
        latex_includegraphics("$function_id+all");
        latex_caption("$function_id");
        latex_end_figure();
        latex_end_center();

        latex_empty_line();

    }

    # latex_newpage();
    # latex_section("Graphics");
    # latex_graphics();

}

sub latex_index_table
{
    latex_begin_center();

    print LATEX
        "\\begin{tabular}{\@{}lr\@{}}\n",
        "\\toprule\n",
        "{algorithm} & {index} \\\\\n",
        "\\midrule\n";

    my $position = 1;
    foreach my $a (@$algorithms) {
        my $id = $a->{id};
        print LATEX "$id & $position \\\\\n";
        $position++;
    }

    print LATEX
        "\\bottomrule\n",
        "\\end{tabular}\n";

    latex_end_center();

}

sub latex_graphics
{
    my $counter = 0;
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($counter == 0) {
            latex_begin_figure();
            print LATEX "\\ContinuedFloat\n";
        }

        latex_subfigure_includegraphics("$function_id", "$function_id");
        if ($counter % 2 == 1) {
            print LATEX "\\\\\n";
        }

        $counter++;
        if ($counter == 8) {
            latex_end_figure();
            $counter = 0;
        }

    }

    if ($counter > 0) {
        latex_end_figure();
    }

}

sub latex_tableofcontents
{
    print LATEX <<EOF;
\\tableofcontents
EOF
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

sub latex_function_table_begin
{
    my $col = shift;

    print LATEX
        "\\begin{tabular}{\@{}l*{5}{$col}>{{\\nprounddigits{0}}}N{2}{0}N{1}{3}N{1}{3}\@{}}\n",
        "\\toprule\n",
        "{algorithm} & \\multicolumn{6}{l}{{function value}} & \\multicolumn{2}{l}{{time (s)}} \\\\\n",
        "\\midrule\n",
        "& {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & {rk} & {mean} & {dev.} \\\\\n",
        "\\midrule\n";
}

sub latex_function_table_add_line
{
    my ($algo, $perf, $best, $logscale, $time) = @_;

    print LATEX "$algo & ";

    my $conversion = "%f";
    if ($logscale) {
        $conversion = "%e";
    }

    my $format = join " & ", map { $perf->{$_} == $best->{$_} ? "{\\color{blue}} $conversion" : "$conversion" } @summary_statistics;

    printf LATEX ($format, $perf->{min}, $perf->{q1}, $perf->{median}, $perf->{q3}, $perf->{max});

    printf LATEX (" & %d", $perf->{rank} + 1);
    printf LATEX (" & %f & %f \\\\\n ", $time->{mean}, $time->{stddev});

}

sub latex_funtion_table_end
{
    print LATEX <<EOF;
\\bottomrule
\\end{tabular}
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

sub latex_subfigure_includegraphics
{
    my ($id, $path) = @_;
    print LATEX
        "\\begin{subfigure}[t]{0.45\\textwidth}\n",
        "\\centering\n",
        "\\includegraphics[width=\\textwidth]{$path}\n",
        "\\caption{$id}\n",
        "\\end{subfigure}\n";
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

sub latex_rankings_table_begin
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

sub latex_rankings_table_body
{
    my @order = sort {
        foreach (0 .. @$algorithms - 1) {
            if (${ $rankings->{$b} }[$_] != ${ $rankings->{$a} }[$_]) {
                return ${ $rankings->{$b} }[$_] <=> ${ $rankings->{$a} }[$_];
            }
        }
        return 0;
    } keys %$rankings;

    foreach (@order) {
        print LATEX "$_ & ", join(" & ", @{ $rankings->{$_} }), "\\\\\n";
    }

}

sub latex_rankings_table_end
{
    print LATEX
        "\\bottomrule\n",
        "\\end{tabular}\n";
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
