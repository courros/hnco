#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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
    );

#
# Global constants
#

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

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $parameter           = $obj->{parameter};
my $graphics            = $obj->{graphics};

my $parameter_id        = $parameter->{id};
my $parameter_name      = $parameter->{name} || $parameter_id;

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

add_missing_names($functions);
add_missing_names($algorithms);
compute_statistics();
generate_data();

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_gnuplot_candlesticks();
generate_gnuplot_mean();
generate_gnuplot_stddev();
generate_latex();

#
# Local functions
#

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
                or die "hnco-function-parameter-stat.pl: generate_data: Cannot open '$path': $!\n";

            $path = "$prefix/quartiles.dat";
            my $quartiles = IO::File->new($path, '>')
                or die "hnco-function-parameter-stat.pl: generate_data: Cannot open '$path': $!\n";

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
        or die "hnco-function-parameter-stat.pl: generate_gnuplot_candlesticks: Cannot open candlesticks.gp\n";

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

    if ($graphics->{logscale}) {
        my $fmt = qq("10^{\%T}");
        print CANDLESTICKS
            "set logscale x\n",
            "set format x $fmt\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_name = $f->{name};

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
            my $algorithm_name = $a->{name};

            my $quoted_string;

            if ($graphics->{candlesticks}->{title}) {
                $quoted_string = qq("$algorithm_name on $function_name");
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

sub generate_gnuplot_mean
{
    open(MEAN, ">mean.gp")
        or die "hnco-function-parameter-stat.pl: generate_gnuplot_mean: Cannot open mean.gp\n";

    # Font face and size
    my $font = "";
    if ($graphics->{mean}->{font_face}) {
        $font = $graphics->{mean}->{font_face};
    }
    if ($graphics->{mean}->{font_size}) {
        $font = "$font,$graphics->{mean}->{font_size}";
    }
    $font = qq("$font");

    my $key = qq(font $font notitle);
    if ($graphics->{mean}->{key}) {
        $key = "$key $graphics->{mean}->{key}";
    } else {
        # opaque vertical reverse Left outside right center box
        $key = "$key reverse Left outside right center box vertical";
    }

    print MEAN
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        qq(set xlabel "$parameter_name"\n),
        qq(set ylabel "Mean value"\n),
        "unset key\n",
        "set key $key\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    if ($graphics->{logscale}) {
        my $fmt = qq("10^{\%T}");
        print MEAN
            "set logscale x\n",
            "set format x $fmt\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_name = $f->{name};

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }

        if ($graphics->{mean}->{title}) {
            print MEAN qq(set title "$function_name: Mean value as a function of $parameter_name"\n);
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

        print MEAN
            "$terminal{pdf} font $font\n",
            qq(set output "$path_graphics/$function_id/mean.pdf"\n);

        print MEAN "plot \\\n";
        print MEAN
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $title = qq("$algorithm_id");
                my $path = qq("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $path using 1:2 with lp ps 0.5 title $title";
             } @$algorithms);
        print MEAN "\n";

        print MEAN
            "$terminal{eps} font $font\n",
            qq(set output "$path_graphics/$function_id/mean.eps"\n),
            "replot\n";

        print MEAN
            "$terminal{png} font $font\n",
            qq(set output "$path_graphics/$function_id/mean.png"\n),
            "replot\n\n";

    }

    system("chmod a+x mean.gp");

}

sub generate_gnuplot_stddev
{
    open(STDDEV, ">stddev.gp")
        or die "hnco-function-parameter-stat.pl: generate_gnuplot_stddev: Cannot open stddev.gp\n";

    print STDDEV
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        qq(set xlabel "$parameter_name"\n),
        qq(set ylabel "Standard deviation"\n),
        qq(set key font ",10" reverse Left outside right center box vertical\n),
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # Font face and size
    my $font = "";
    if ($graphics->{stddev}->{font_face}) {
        $font = $graphics->{stddev}->{font_face};
    }
    if ($graphics->{stddev}->{font_size}) {
        $font = "$font,$graphics->{stddev}->{font_size}";
    }
    $font = qq(font "$font");

    if ($graphics->{logscale}) {
        my $fmt = qq("10^{\%T}");
        print STDDEV
            "set logscale x\n",
            "set format x $fmt\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $function_name = $f->{name};

        unless (-d "$path_graphics/$function_id") { mkdir "$path_graphics/$function_id"; }


        if ($graphics->{stddev}->{title}) {
            print STDDEV qq(set title "$function_name: standard deviation value as a function of $parameter_name"\n);
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

        print STDDEV
            "$terminal{pdf} $font\n",
            qq(set output "$path_graphics/$function_id/stddev.pdf"\n);

        print STDDEV "plot \\\n";
        print STDDEV
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $title = qq("$algorithm_id");
                my $path = qq("$path_results/$function_id/$algorithm_id/mean.dat");
                "  $path using 1:3 with lp ps 0.5 title $title";
             } @$algorithms);
        print STDDEV "\n";

        print STDDEV
            "$terminal{eps} $font\n",
            qq(set output "$path_graphics/$function_id/stddev.eps"\n),
            "replot\n";

        print STDDEV
            "$terminal{png} $font\n",
            qq(set output "$path_graphics/$function_id/stddev.png"\n),
            "replot\n\n";

    }

    system("chmod a+x stddev.gp");

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-function-parameter-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};
        my $function_name = $fn->{name};

        print LATEX latex_section("Function $function_name");

        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id/mean");
        print LATEX latex_end_center();

        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id/stddev");
        print LATEX latex_end_center();

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};

            print LATEX latex_begin_center();
            print LATEX latex_includegraphics("$function_id/$algorithm_id", 0.6);
            print LATEX latex_end_center();

        }

    }

    close(LATEX);
}
