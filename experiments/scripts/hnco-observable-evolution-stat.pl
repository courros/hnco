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
my $graphics            = $plan->{graphics};

my $xcolumn             = $graphics->{xcolumn};
my $xlabel              = $graphics->{xlabel};
my $xlogscale           = $graphics->{xlogscale};
my $ycolumn             = $graphics->{ycolumn};
my $ylabel              = $graphics->{ylabel};
my $ylogscale           = $graphics->{ylogscale};

#
# Processing
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

add_missing_labels($functions);
add_missing_labels($algorithms);
generate_graphics();
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

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp")
        or die "hnco-observable-evolution-stat.pl: generate_graphics: cannot open graphics.gp\n";

    my $key;
    if ($graphics->{key}) {
        $key = $graphics->{key};
    } else {
        $key = qq($key inside top right opaque vertical reverse Left title "Algorithm" left);
    }

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$xlabel\"\n",
        "set ylabel \"$ylabel\"\n",
        "set key $key\n",
        "set autoscale fix\n\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    if ($xlogscale) {
        my $fmt = qq("10^{\%L}");
        print GRAPHICS
            "set logscale x\n",
            "set format x $fmt\n";
    } else {
        print GRAPHICS
            "unset logscale x\n",
            "set format x\n";
    }

    if ($ylogscale) {
        my $fmt = qq("10^{\%L}");
        print GRAPHICS
            "set logscale y\n",
            "set format y $fmt\n";
    } else {
        print GRAPHICS
            "unset logscale y\n",
            "set format y\n";
    }

    my $data = "using $xcolumn:$ycolumn";
    if ($graphics->{transform}) {
        my $transform = $graphics->{transform};
        print GRAPHICS "$transform->{function}($transform->{variable}) = $transform->{expression}\n";
        $data = "using $xcolumn:($transform->{function}(\$$ycolumn))";
    }
    print GRAPHICS "\n";

    # Font face and size
    my $font = "";
    if ($graphics->{font_face}) {
        $font = $graphics->{font_face};
    }
    if ($graphics->{font_size}) {
        $font = "$font,$graphics->{font_size}";
    }
    $font = qq(font "$font");

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $path = qq("$path_graphics/$function_id.eps");
        print GRAPHICS
            "$terminal{eps} $font\n",
            "set output $path\n";

        print GRAPHICS "plot \\\n";
        print GRAPHICS
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                my $title = qq("$_->{label}");
                $path = qq("$path_results/$function_id/$algorithm_id/1.out");
                "  $path $data with lines lw 1 title $title";
             } @$algorithms);
        print GRAPHICS "\n";

        $path = qq("$path_graphics/$function_id.pdf");
        print GRAPHICS
            "$terminal{pdf} $font\n",
            "set output $path\n",
            "replot\n";

        $path = qq("$path_graphics/$function_id.png");
        print GRAPHICS
            "$terminal{png} $font\n",
            "set output $path\n",
            "replot\n\n";
    }

    system("chmod a+x graphics.gp");
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-observable-evolution-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        print LATEX latex_section($f->{label});
        print LATEX latex_begin_center();
        print LATEX latex_includegraphics($function_id);
        print LATEX latex_end_center();
    }
}
