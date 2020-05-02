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

#
# Global constants
#

my $path_results        = "results";
my $path_graphics       = "graphics";
my $path_report         = "report";

my @summary_statistics          = qw(median q1 q3 min max);
my @summary_statistics_display  = qw(min q1 median q3 max);

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

open(FILE, $plan) or die "hnco-function-parameter-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}

#
# Global variables
#

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $parameter           = $obj->{parameter};
my $graphics            = $obj->{graphics};

my $parameter_id        = $parameter->{id};

my $values;
if ($parameter->{values_perl}) {
    my @tmp = eval $parameter->{values_perl};
    $values = \@tmp;
} else {
    $values = $parameter->{values};
}

my $all_stat = {};

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
        "set xlabel " . quote($graphics->{candlesticks}->{xlabel} || $parameter_id) . "\n",
        "set ylabel \"Function value\"\n",
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
    $font = quote($font);
    $font = "font $font";

    # boxwidth
    my $boxwidth = 10;
    if ($graphics->{candlesticks}->{boxwidth}) {
        $boxwidth = $graphics->{candlesticks}->{boxwidth};
    }

    if ($graphics->{logscale}) {
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

            my $quoted_string;

            if ($graphics->{candlesticks}->{title}) {
                $quoted_string = quote("$algorithm_id on $function_id");
                print CANDLESTICKS "set title $quoted_string\n";
            }

            $quoted_string = quote("$path_graphics/$function_id/$algorithm_id.pdf");
            print CANDLESTICKS
                "$terminal{pdf} $font\n",
                "set output $quoted_string\n";
            $quoted_string = quote("$path_results/$function_id/$algorithm_id/quartiles.dat");
            print CANDLESTICKS
                "plot $quoted_string using 1:3:2:6:5:($boxwidth) with candlesticks lw 2 lt 3 notitle whiskerbars, \\\n",
                "     $quoted_string using 1:4:4:4:4:($boxwidth) with candlesticks lw 2 lt 1 notitle\n";

            $quoted_string = quote("$path_graphics/$function_id/$algorithm_id.eps");
            print CANDLESTICKS
                "$terminal{eps} $font\n",
                "set output $quoted_string\n",
                "replot\n";

            $quoted_string = quote("$path_graphics/$function_id/$algorithm_id.png");
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

    print MEAN
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Mean value\"\n",
        "set key bottom right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    # Font face and size
    my $font = "";
    if ($graphics->{mean}->{font_face}) {
        $font = $graphics->{mean}->{font_face};
    }
    if ($graphics->{mean}->{font_size}) {
        $font = "$font,$graphics->{mean}->{font_size}";
    }
    $font = quote($font);
    $font = "font $font";

    if ($graphics->{logscale}) {
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
            "$terminal{pdf} $font\n",
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
            "$terminal{eps} $font\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id/mean.png");
        print MEAN
            "$terminal{png} $font\n",
            "set output $quoted_path\n",
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
        "set xlabel \"$parameter_id\"\n",
        "set ylabel \"Standard deviation of value\"\n",
        "set key bottom right box opaque\n",
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
    $font = quote($font);
    $font = "font $font";

    if ($graphics->{logscale}) {
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
            "$terminal{pdf} $font\n",
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
            "$terminal{eps} $font\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id/stddev.png");
        print STDDEV
            "$terminal{png} $font\n",
            "set output $quoted_path\n",
            "replot\n\n";

    }

    system("chmod a+x stddev.gp");

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-function-parameter-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    foreach my $fn (@$functions) {
        my $function_id = $fn->{id};

        latex_section("Function $function_id");

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

sub read_file
{
    my $path = shift;
    my $json;
    {
        local $/;
        open my $fh, '<', $path or die "hnco-function-parameter-stat.pl: read_file: Cannot open '$path': $!\n";
        $json = <$fh>;
    }
    return $json;
}
