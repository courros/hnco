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
    or die "hnco-runtime-stat.pl: Cannot open $plan\n";
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

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

add_missing_names($functions);
add_missing_names($algorithms);
compute_statistics();
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
                        or die "hnco-runtime-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                    my $line = $file->getline;
                    chomp $line;
                    my @results = split ' ', $line;
                    $SD->add_data($results[0]);
                    $file->close;
                }

                $algorithm_stat->{$value} = { min       => $SD->min(),
                                              q1        => $SD->quantile(1),
                                              median    => $SD->median(),
                                              q3        => $SD->quantile(3),
                                              max       => $SD->max(),
                                              mean      => $SD->mean(),
                                              stddev    => $SD->standard_deviation() };

            }

            $function_stat->{$algorithm_id} = $algorithm_stat;
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
            my $prefix = "$path_results/$function_id/$algorithm_id";

            my $path = "$prefix/mean.dat";
            my $mean = IO::File->new($path, '>')
                or die "hnco-runtime-stat.pl: generate_data: Cannot open '$path': $!\n";

            $path = "$prefix/quartiles.dat";
            my $quartiles = IO::File->new($path, '>')
                or die "hnco-runtime-stat.pl: generate_data: Cannot open '$path': $!\n";

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

sub generate_gnuplot_candlesticks
{
    open(CANDLESTICKS, ">candlesticks.gp")
        or die "hnco-runtime-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

    print CANDLESTICKS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Runtime\"\n",
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
        unless (-d "$path_graphics/$function_id") {
            mkdir "$path_graphics/$function_id";
        }

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $quoted_string = quote("$a->{name} on $f->{name}");
            print CANDLESTICKS "set title $quoted_string\n";

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
        or die "hnco-runtime-stat.pl: generate_gnuplot_mean: Cannot open mean.gp\n";

    print MEAN
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Mean runtime\"\n",
        "set logscale y\n",
        "set format y", quote("10^{\%T}"), "\n",
        "set key bottom right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    my $xmin = min(@$values);
    my $xmax = max(@$values);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }
        $quoted_string = quote("$f->{name}: Mean runtime as a function of $parameter_id");
        print MEAN "set title $quoted_string\n";
        foreach my $gnuplot (@{$f->{mean_gnuplot}}) {
            print MEAN $gnuplot->{expression}, "\n";
        }

        my $quoted_string = quote("$path_graphics/$function_id/mean.pdf");
        print MEAN
            $terminal{pdf}, "\n",
            "set output $quoted_string\n";

        print MEAN "plot \\\n";
        print MEAN
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $name = quote($_->{name});
                my $quoted_path = quote("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $quoted_path using 1:2 with l lw 2 title $name";
             } @$algorithms);

        foreach my $gnuplot (@{ $f->{mean_gnuplot}}) {
            my ($before, $after) = split /\s*=\s*/, $gnuplot->{expression};
            my $title = quote($gnuplot->{title});
            print MEAN
                ", \\\n",
                "  [$xmin:$xmax] $before w l lw 2 title $title";
        }
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

    close(MEAN);

    system("chmod a+x mean.gp");

}

sub generate_gnuplot_stddev
{
    open(STDDEV, ">stddev.gp")
        or die "hnco-runtime-stat.pl: generate_gnuplot_stddev: Cannot open stddev.gp\n";

    print STDDEV
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Standard deviation of runtime\"\n",
        "set logscale y\n",
        "set format y", quote("10^{\%T}"), "\n",
        "set key bottom right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    my $xmin = min(@$values);
    my $xmax = max(@$values);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }
        $quoted_string = quote("$f->{name}: Standard deviation of runtime as a function of $parameter_id");
        print STDDEV "set title $quoted_string\n";
        foreach my $gnuplot (@{$f->{stddev_gnuplot}}) {
            print STDDEV $gnuplot->{expression}, "\n";
        }

        my $quoted_string = quote("$path_graphics/$function_id/stddev.pdf");
        print STDDEV
            $terminal{pdf}, "\n",
            "set output $quoted_string\n";

        print STDDEV "plot \\\n";
        print STDDEV
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $name = quote($_->{name});
                my $quoted_path = quote("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $quoted_path using 1:3 with l lw 2 title $name";
             } @$algorithms);

        foreach my $gnuplot (@{ $f->{stddev_gnuplot}}) {
            my ($before, $after) = split /\s*=\s*/, $gnuplot->{expression};
            my $title = quote($gnuplot->{title});
            print STDDEV
                ", \\\n",
                "  [$xmin:$xmax] $before w l lw 2 title $title";
        }
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

    close(STDDEV);
    system("chmod a+x stddev.gp");

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-runtime-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        latex_section("Function $f->{name}");

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
