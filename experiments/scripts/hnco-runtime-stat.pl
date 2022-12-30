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
my @pref_max = qw(median q1 q3 min max);
my @pref_min = qw(median q3 q1 max min);

my $path_graphics       = "graphics";
my $path_report         = "report";
my $path_results        = "results";

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

my $algorithms          = $plan->{algorithms};
my $functions           = $plan->{functions};
my $num_runs            = $plan->{num_runs};
my $parameter           = $plan->{parameter};
my $graphics            = $plan->{graphics};

my $parameter_id        = $parameter->{id};
my $parameter_label     = $parameter->{label} || $parameter_id;

my $all_stat = {};

#
# Parameter values
#

my $values;
if ($parameter->{values_perl}) {
    my @tmp = eval $parameter->{values_perl};
    $values = \@tmp;
} else {
    $values = $parameter->{values};
}

#
# Processing
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

add_missing_labels($functions);
add_missing_labels($algorithms);
compute_statistics();
generate_data();
generate_gnuplot_candlesticks();
generate_gnuplot_mean();
generate_gnuplot_stddev();
generate_latex();

#
# Local functions
#

sub add_missing_labels
{
    my $list = shift;
    foreach my $item (@$list) {
        if (!exists($item->{label})) {
            $item->{label} = $item->{id};
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
                    if (-f $path) {
                        my $obj = from_json(read_file($path));
                        $SD->add_data($obj->{total_num_evaluations});
                    } else {
                        die "hnco-runtime-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                    }
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

    my $context = $graphics->{candlesticks};

    print CANDLESTICKS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        qq(set xlabel "$parameter_label"\n),
        qq(set ylabel "Number of evaluations"\n),
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    if ($parameter->{logscale}) {
        my $fmt = qq("10^{\%T}");
        print CANDLESTICKS
            "set logscale x\n",
            "set format x $fmt\n";
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

    # boxwidth
    my $boxwidth = 10;
    if ($context->{boxwidth}) {
        $boxwidth = $context->{boxwidth};
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        unless (-d "$path_graphics/$function_id") {
            mkdir "$path_graphics/$function_id";
        }

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

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            if ($context->{title}) {
                print CANDLESTICKS qq(set title "Runtime of $a->{label} on $f->{label}"\n);
            }

            my $quoted_string = qq("$path_graphics/$function_id/$algorithm_id.pdf");
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

    close(CANDLESTICKS);
    system("chmod a+x candlesticks.gp");

}

sub generate_gnuplot_mean
{
    open(MEAN, ">mean.gp")
        or die "hnco-runtime-stat.pl: generate_gnuplot_mean: Cannot open mean.gp\n";

    my $context = $graphics->{mean};

    print MEAN
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        qq(set xlabel "$parameter_label"\n),
        qq(set ylabel "Number of evaluations"\n),
        "set key " . ($context->{key} || "bottom right box opaque") . "\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # Font face and size
    my $font = "";
    if ($context->{font_face}) {
        $font = $context->{font_face};
    }
    if ($context->{font_size}) {
        $font = "$font,$context->{font_size}";
    }
    $font = qq(font "$font");

    my $xmin = min(@$values);
    my $xmax = max(@$values);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        unless (-d "$path_graphics/$function_id") {
            mkdir "$path_graphics/$function_id";
        }

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print MEAN
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print MEAN
                "unset logscale y\n",
                "set format y\n";
        }

        if ($context->{title}) {
            print MEAN qq(set title "Mean runtime on $f->{label}"\n);
        }

        # Additional functions
        foreach my $gnuplot (@{$f->{mean_gnuplot}}) {
            print MEAN $gnuplot->{expression}, "\n";
        }

        print MEAN
            "$terminal{pdf} $font\n",
            qq(set output "$path_graphics/$function_id/mean.pdf"\n);

        print MEAN "plot \\\n";
        print MEAN
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $label = qq("$_->{label}");
                my $quoted_path = qq("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $quoted_path using 1:2 with l lw 2 title $label";
             } @$algorithms);

        foreach my $gnuplot (@{ $f->{mean_gnuplot}}) {
            my ($before, $after) = split /\s*=\s*/, $gnuplot->{expression};
            my $label = qq("$gnuplot->{label}");
            print MEAN
                ", \\\n",
                "  [$xmin:$xmax] $before w l lw 2 title $label";
        }
        print MEAN "\n";

        foreach('eps', 'png') {
            print MEAN
                "$terminal{$_} $font\n",
                qq(set output "$path_graphics/$function_id/mean.$_"\n),
                "replot\n";
        }

        print MEAN "\n";
    }

    close(MEAN);
    system("chmod a+x mean.gp");

}

sub generate_gnuplot_stddev
{
    open(STDDEV, ">stddev.gp")
        or die "hnco-runtime-stat.pl: generate_gnuplot_stddev: Cannot open stddev.gp\n";

    my $context = $graphics->{stddev};

    print STDDEV
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        qq(set xlabel "$parameter_label"\n),
        qq(set ylabel "Number of evaluations"\n),
        "set key bottom right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # Font face and size
    my $font = "";
    if ($context->{font_face}) {
        $font = $context->{font_face};
    }
    if ($context->{font_size}) {
        $font = "$font,$context->{font_size}";
    }
    $font = qq(font "$font");

    my $xmin = min(@$values);
    my $xmax = max(@$values);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        unless (-d "$path_graphics/$function_id") {
            mkdir "$path_graphics/$function_id";
        }

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print STDDEV
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print STDDEV
                "unset logscale y\n",
                "set format y\n";
        }

        if ($context->{title}) {
            print STDDEV qq(set title "Standard deviation of runtime on $f->{label}"\n);
        }

        # Additional functions
        foreach my $gnuplot (@{$f->{stddev_gnuplot}}) {
            print STDDEV $gnuplot->{expression}, "\n";
        }

        print STDDEV
            "$terminal{pdf} $font\n",
            qq(set output "$path_graphics/$function_id/stddev.pdf"\n);

        print STDDEV "plot \\\n";
        print STDDEV
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $label = qq("$_->{label}");
                my $quoted_path = qq("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $quoted_path using 1:3 with l lw 2 title $label";
             } @$algorithms);

        foreach my $gnuplot (@{ $f->{stddev_gnuplot}}) {
            my ($before, $after) = split /\s*=\s*/, $gnuplot->{expression};
            my $label = qq("$gnuplot->{label}");
            print STDDEV
                ", \\\n",
                "  [$xmin:$xmax] $before w l lw 2 title $label";
        }
        print STDDEV "\n";

        foreach('eps', 'png') {
            print STDDEV
                "$terminal{$_} $font\n",
                qq(set output "$path_graphics/$function_id/stddev.$_"\n),
                "replot\n";
        }

        print STDDEV "\n";
    }

    close(STDDEV);
    system("chmod a+x stddev.gp");
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-runtime-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        print LATEX latex_section("Function $f->{label}");

        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id/mean");
        print LATEX latex_end_center();

        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id/stddev");
        print LATEX latex_end_center();

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            print LATEX latex_begin_center();
            print LATEX latex_includegraphics("$function_id/$algorithm_id");
            print LATEX latex_end_center();

        }

    }

}
