#! /usr/bin/perl -w

# Copyright (C) 2016, 2017 Arnaud Berny

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

open(FILE, $plan) or die "hnco-parameter-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) { $json .= $_; }

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $path_graphics       = $obj->{graphics};
my $path_results        = $obj->{results};
my $path_report         = $obj->{report};

my $parameter           = $obj->{parameter};
my $parameter_id        = $parameter->{id};
my $values              = $parameter->{values};
my $boxwidth            = $parameter->{boxwidth};

my $all_stat = {};
my $all_best = {};

my $num_lines = @$algorithms * @$values;

my $rankings = {};
foreach my $a (@$algorithms) {
    my $algorithm_id = $a->{id};
    foreach my $value (@$values) {
        my $id = "$algorithm_id-$value";
        my @counts = (0) x ($num_lines); # Array of size $num_lines initialized to 0
        $rankings->{$id} = \@counts;
    }
}

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

compute_statistics();
compute_rankings();
compute_best_statistics();

generate_data();
generate_gnuplot_candlesticks();
generate_gnuplot_mean();
generate_gnuplot_stddev();
generate_latex();

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_stat = {};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            foreach my $value (@$values) {
                my $id = "$algorithm_id-$value";

                my $path = "$path_results/$function_id/$algorithm_id/$parameter_id-$value.dat";
                my $input = IO::File->new($path, '<') or die "hnco-parameter-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                my $SD = Statistics::Descriptive::Full->new();

                while (my $line = $input->getline) {
                    chomp $line;
                    my @results = split ' ', $line;
                    $SD->add_data($results[1]);
                }

                $input->close();

                if ($f->{reverse}) {
                    $function_stat->{$id} = { min         => -$SD->max(),
                                              q1          => -$SD->quantile(3),
                                              median      => -$SD->median(),
                                              q3          => -$SD->quantile(1),
                                              max         => -$SD->min(),
                                              mean        => -$SD->mean(),
                                              stddev      => $SD->standard_deviation() };
                } else {
                    $function_stat->{$id} = { min         => $SD->min(),
                                              q1          => $SD->quantile(1),
                                              median      => $SD->median(),
                                              q3          => $SD->quantile(3),
                                              max         => $SD->max(),
                                              mean        => $SD->mean(),
                                              stddev      => $SD->standard_deviation() };
                }

            }

        }

        $all_stat->{$function_id} = $function_stat;

    }

}

sub generate_data
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            $path = "$path_results/$function_id/$algorithm_id/mean.dat";
            my $mean = IO::File->new($path, '>') or die "hnco-parameter-stat.pl: generate_data: Cannot open '$path': $!\n";

            my $path = "$path_results/$function_id/$algorithm_id/quartiles.dat";
            my $quartiles = IO::File->new($path, '>') or die "hnco-parameter-stat.pl: generate_data: Cannot open '$path': $!\n";

            foreach my $value (@$values) {
                my $id = "$algorithm_id-$value";
                my $stat = $all_stat->{$function_id}->{$id};
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

sub compute_rankings
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_stat = $all_stat->{$function_id};

        if ($f->{reverse}) {
            @order = sort {
                foreach (@pref_min) {
                    if ($function_stat->{$a}->{$_} != $function_stat->{$b}->{$_}) {
                        return $function_stat->{$a}->{$_} <=> $function_stat->{$b}->{$_};
                    }
                }
                return 0;
            } keys %$function_stat;
        } else {
            @order = sort {
                foreach (@pref_max) {
                    if ($function_stat->{$b}->{$_} != $function_stat->{$a}->{$_}) {
                        return $function_stat->{$b}->{$_} <=> $function_stat->{$a}->{$_};
                    }
                }
                return 0;
            } keys %$function_stat;
        }

        for (my $i = 0; $i < @order; $i++) {
            my $cur = $order[$i];
            my $rank;
            if ($i == 0) {
                $rank = 0;
            } else {            # Handle Ex aequo
                my $prev = $order[$i - 1];
                if (all { $function_stat->{$cur}->{$_} == $function_stat->{$prev}->{$_} } @summary_statistics) {
                    $rank = $function_stat->{$prev}->{rank};
                } else {
                    $rank = $i;
                }
            }

            $function_stat->{$cur}->{rank} = $rank;  # Save rank
            ${ $rankings->{$cur} }[$rank]++; # Update rankings
        }

    }

}

sub compute_best_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $stat = $all_stat->{$function_id};
        my $best = {};

        my @order;
        foreach (@summary_statistics) {
            if ($f->{reverse}) {
                @order = sort { $stat->{$a}->{$_} <=> $stat->{$b}->{$_} } keys %$stat;
            } else {
                @order = sort { $stat->{$b}->{$_} <=> $stat->{$a}->{$_} } keys %$stat;
            }
            $best->{$_} = $stat->{$order[0]}->{$_};
        }

        $all_best->{$function_id} = $best;

    }

}

sub generate_gnuplot_candlesticks
{
    open(CANDLESTICKS, ">candlesticks.gp") or die "hnco-parameter-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

    print CANDLESTICKS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Performance\"\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    if ($parameter->{logscale}) {
        my $fmt = quote("10^{\%T}");
        print CANDLESTICKS
            "set logscale x\n",
            "set format x $fmt\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = quote("10^{\%T}");
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

            my $quoted_string = quote("$algorithm_id on $function_id");

            print CANDLESTICKS
                "set title $quoted_string\n";

            $quoted_string = quote("$path_graphics/$function_id/$algorithm_id.pdf");
            print CANDLESTICKS
                $terminal{pdf}, "\n",
                "set output $quoted_string\n";
            $quoted_string = quote("$path_results/$function_id/$algorithm_id/quartiles.dat");
            print CANDLESTICKS
                "plot $quoted_string using 1:3:2:6:5:$boxwidth with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                "     $quoted_string using 1:4:4:4:4:$boxwidth with candlesticks lw 2 lt 1 notitle\n";

            $quoted_string = quote("$path_graphics/$function_id/$algorithm_id.eps");
            print CANDLESTICKS
                $terminal{eps}, "\n",
                "set output $quoted_string\n",
                "replot\n";

            $quoted_string = quote("$path_graphics/$function_id/$algorithm_id.png");
            print CANDLESTICKS
                $terminal{png}, "\n",
                "set output $quoted_string\n",
                "replot\n\n";

        }
    }

    system("chmod a+x candlesticks.gp");

}

sub generate_gnuplot_mean
{
    open(MEAN, ">mean.gp") or die "hnco-parameter-stat.pl: generate_gnuplot_mean: Cannot open mean.gp\n";

    print MEAN
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Mean performance\"\n",
        "set key bottom right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    if ($parameter->{logscale}) {
        my $fmt = quote("10^{\%T}");
        print MEAN
            "set logscale x\n",
            "set format x $fmt\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = quote("10^{\%T}");
            print MEAN
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print MEAN
                "unset logscale y\n",
                "set format y\n";
        }

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }

        my $quoted_string = quote("$path_graphics/$function_id/mean.pdf");
        print MEAN
            $terminal{pdf}, "\n",
            "set output $quoted_string\n";

        $quoted_string = quote("$function_id: Mean performance as a function of $parameter_id");
        print MEAN
            "set title $quoted_string\n";

        print MEAN "plot \\\n";
        print MEAN
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $quoted_title = quote("$algorithm_id");
                my $quoted_path = quote("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $quoted_path using 1:2 with l lw 2 title $quoted_title";
             } @$algorithms);
        print MEAN "\n";

        $quoted_path = quote("$path_graphics/$function_id/mean.eps");
        print MEAN
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id/mean.png");
        print MEAN
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    system("chmod a+x mean.gp");

}

sub generate_gnuplot_stddev
{
    open(STDDEV, ">stddev.gp") or die "hnco-parameter-stat.pl: generate_gnuplot_stddev: Cannot open stddev.gp\n";

    print STDDEV
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Standard deviation of performance\"\n",
        "set key bottom right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    if ($parameter->{logscale}) {
        my $fmt = quote("10^{\%T}");
        print STDDEV
            "set logscale x\n",
            "set format x $fmt\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = quote("10^{\%T}");
            print STDDEV
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print STDDEV
                "unset logscale y\n",
                "set format y\n";
        }

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }

        my $quoted_string = quote("$path_graphics/$function_id/stddev.pdf");
        print STDDEV
            $terminal{pdf}, "\n",
            "set output $quoted_string\n";

        $quoted_string = quote("$function_id: Standard deviation of performance as a function of $parameter_id");
        print STDDEV
            "set title $quoted_string\n";

        print STDDEV "plot \\\n";
        print STDDEV
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $quoted_title = quote("$algorithm_id");
                my $quoted_path = quote("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $quoted_path using 1:3 with l lw 2 title $quoted_title";
             } @$algorithms);
        print STDDEV "\n";

        $quoted_path = quote("$path_graphics/$function_id/stddev.eps");
        print STDDEV
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id/stddev.png");
        print STDDEV
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    system("chmod a+x stddev.gp");

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex") or die "hnco-parameter-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    latex_section("Rankings");
    latex_begin_center();
    latex_rankings_table_begin();
    latex_rankings_table_body();
    latex_rankings_table_end();
    latex_end_center();
    latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $best = $all_best->{$function_id};

        latex_section("Function $function_id");

        latex_begin_center();
        if (exists($f->{col})) {
            latex_function_table_begin($f->{col});
        } else {
            latex_function_table_begin("N{2}{3}");
        }
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            foreach my $value (@$values) {
                my $id = "$algorithm_id-$value";
                my $stat = $all_stat->{$function_id}->{$id};
                latex_function_table_add_line($id,
                                              $stat,
                                              $best,
                                              $f->{logscale});
            }
        }
        latex_funtion_table_end();
        latex_end_center();

        latex_begin_center();
        latex_includegraphics("$function_id/mean");
        latex_end_center();

        latex_begin_center();
        latex_includegraphics("$function_id/stddev");
        latex_end_center();

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            latex_begin_center();
            latex_includegraphics("$function_id/$algorithm_id");
            latex_end_center();

        }

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

sub latex_rankings_table_begin
{
    print LATEX
        "\\begin{tabular}{\@{}l*{$num_lines}{r}\@{}}\n",
        "\\toprule\n",
        "algorithm & \\multicolumn{$num_lines}{l}{{rank distribution}}\\\\\n",
        "\\midrule\n",
        "& ", join(" & ", 1 .. $num_lines), "\\\\\n",
        "\\midrule\n";
}

sub latex_rankings_table_body
{
    my @order = sort {
        foreach (0 .. $num_lines - 1) {
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

sub latex_function_table_begin
{
    my $col = shift;

    print LATEX
        "\\begin{tabular}{\@{}l*{5}{$col}>{{\\nprounddigits{0}}}N{2}{0}\@{}}\n",
        "\\toprule\n",
        "{algorithm} & {min} & {\$Q_1\$} & {med.} & {\$Q_3\$} & {max} & {rk} \\\\\n",
        "\\midrule\n";
}

sub latex_function_table_add_line
{
    my ($algo, $perf, $best, $logscale) = @_;

    my $conversion = $logscale ? "%e" : "%f";
    my $format = join " & ", map { $perf->{$_} == $best->{$_} ? "{\\color{blue}} $conversion" : "$conversion" } @summary_statistics;

    printf LATEX ("\\verb\|%s\| & ", $algo);
    printf LATEX ($format, $perf->{min}, $perf->{q1}, $perf->{median}, $perf->{q3}, $perf->{max});
    printf LATEX (" & %d \\\\\n", $perf->{rank} + 1);

}

sub latex_funtion_table_end
{
    print LATEX <<EOF;
\\bottomrule
\\end{tabular}
EOF
}
