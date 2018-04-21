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
use List::MoreUtils qw(all);

my @summary_statistics = qw(median q1 q3 min max);
my @summary_statistics_display = qw(min q1 median q3 max);

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan)
    or die "hnco-parameter-stat.pl: Cannot open $plan\n";
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
my $boxwidth            = $parameter->{boxwidth};

my $values;
if ($parameter->{values_perl}) {
    my @tmp = eval $parameter->{values_perl};
    $values = \@tmp;
} else {
    $values = $parameter->{values};
}

my $all_stat = {};
my $all_stat_flat = {};
my $all_best = {};

my $num_lines = @$algorithms * @$values;

my $rankings = {};
foreach my $a (@$algorithms) {
    my $algorithm_id = $a->{id};
    foreach my $value (@$values) {
        my @counts = (0) x ($num_lines); # Array of size $num_lines initialized to 0
        $rankings->{$algorithm_id}->{$value} = \@counts;
    }
}
my @rankings_flat = ();

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
generate_gnuplot_candlesticks();
generate_gnuplot_mean();
generate_gnuplot_stddev();
generate_latex();

sub add_missing_names
{
    my $list = shift;
    foreach my $item (@$list) {
        if (!exists($item->{name})) {
            $item->{name} = $item->{id};
        }
    }
}

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

            foreach my $value (@$values) {
                my $prefix = "$path_results/$function_id/$algorithm_id/$parameter_id-$value";
                my $SD = Statistics::Descriptive::Full->new();

                foreach (1 .. $algorithm_num_runs) {
                    my $path = "$prefix/$_.out";
                    my $file = IO::File->new($path, '<')
                        or die "hnco-parameter-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                    my $line = $file->getline;
                    chomp $line;
                    my @results = split ' ', $line;
                    $SD->add_data($results[1]);
                    $file->close;
                }

                $algorithm_stat->{$value} = { min         => $SD->min(),
                                              q1          => $SD->quantile(1),
                                              median      => $SD->median(),
                                              q3          => $SD->quantile(3),
                                              max         => $SD->max(),
                                              mean        => $SD->mean(),
                                              stddev      => $SD->standard_deviation() };

                my $item = { algorithm  => $algorithm_id,
                             value      => $value,
                             min        => $SD->min(),
                             q1         => $SD->quantile(1),
                             median     => $SD->median(),
                             q3         => $SD->quantile(3),
                             max        => $SD->max() };
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
                or die "hnco-parameter-stat.pl: generate_data: Cannot open '$path': $!\n";

            $path = "$prefix/quartiles.dat";
            my $quartiles = IO::File->new($path, '>')
                or die "hnco-parameter-stat.pl: generate_data: Cannot open '$path': $!\n";

            foreach my $value (@$values) {
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
        foreach my $value (@$values) {
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
                foreach my $value (@$values) {
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
        or die "hnco-parameter-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

    print CANDLESTICKS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Function value\"\n",
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
                "plot $quoted_string using 1:3:2:6:5:($boxwidth) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                "     $quoted_string using 1:4:4:4:4:($boxwidth) with candlesticks lw 2 lt 1 notitle\n";

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
    open(MEAN, ">mean.gp")
        or die "hnco-parameter-stat.pl: generate_gnuplot_mean: Cannot open mean.gp\n";

    print MEAN
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Mean value\"\n",
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

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }

        my $quoted_string = quote("$function_id: Mean value as a function of $parameter_id");
        print MEAN "set title $quoted_string\n";
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

        $quoted_string = quote("$path_graphics/$function_id/mean.pdf");
        print MEAN
            $terminal{pdf}, "\n",
            "set output $quoted_string\n";

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
    open(STDDEV, ">stddev.gp")
        or die "hnco-parameter-stat.pl: generate_gnuplot_stddev: Cannot open stddev.gp\n";

    print STDDEV
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Standard deviation of value\"\n",
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

        $quoted_string = quote("$function_id: Standard deviation of value as a function of $parameter_id");
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
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-parameter-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    latex_section("Rankings");
    latex_begin_center();
    latex_rankings_table();
    latex_end_center();
    latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_best = $all_best->{$function_id};

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
                latex_function_table_add_line($algorithm_id,
                                              $value,
                                              $all_stat->{$function_id}->{$algorithm_id}->{$value},
                                              $function_best,
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

sub latex_rankings_table
{
    print LATEX
        "\\begin{tabular}{\@{}ll*{$num_lines}{r}\@{}}\n",
        "\\toprule\n",
        "algorithm & $parameter_id & \\multicolumn{$num_lines}{l}{{rank distribution}}\\\\\n",
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
        "{algorithm} & {$parameter_id} & \\multicolumn{6}{l}{{function value}} \\\\\n",
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
