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

my $functions           = $obj->{functions};
my $algorithms          = $obj->{algorithms};

#
# Processing
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_graphics();
generate_latex();

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp")
        or die "hnco-maximum-evolution-stat.pl: generate_graphics: cannot open graphics.gp\n";

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"Number of evaluations\"\n",
        "set ylabel \"Function value\"\n",
        "set key outside top center box opaque horizontal\n",
        "set format x ", qq("10^{%L}"), "\n",
        "set logscale x\n",
        "set autoscale fix\n\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        if ($f->{logscale}) {
            my $fmt = qq("10^{\%T}");
            print GRAPHICS
                "set logscale y 10\n",
                "set format y $fmt\n";
        } else {
            print GRAPHICS
                "unset logscale y\n",
                "set format y\n";
        }

        my $quoted_title = qq("$function_id");
        my $quoted_path = qq("$path_graphics/$function_id.eps");

        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "set key title $quoted_title\n";

        print GRAPHICS "plot \\\n";
        print GRAPHICS
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                $quoted_path = qq("$path_results/$function_id/$algorithm_id/1.out");
                $quoted_title = qq("$algorithm_id");
                $f->{reverse} ?
                    "  $quoted_path using 1:(-\$2) with steps lw 2 title $quoted_title" :
                    "  $quoted_path using 1:2 with steps lw 2 title $quoted_title";
             } @$algorithms);
        print GRAPHICS "\n";

        $quoted_path = qq("$path_graphics/$function_id.pdf");
        print GRAPHICS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = qq("$path_graphics/$function_id.png");
        print GRAPHICS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";
    }

    system("chmod a+x graphics.gp");
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-maximum-evolution-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        print LATEX latex_section($function_id);
        print LATEX latex_begin_center();
        print LATEX latex_includegraphics($function_id);
        print LATEX latex_end_center();
    }
}
