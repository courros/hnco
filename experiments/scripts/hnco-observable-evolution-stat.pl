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

my $path_results        = $obj->{results};
my $path_report         = $obj->{report};
my $path_graphics       = $obj->{graphics};
my $functions           = $obj->{functions};
my $algorithms          = $obj->{algorithms};
my $xcolumn             = $obj->{xcolumn};
my $ycolumn             = $obj->{ycolumn};

#
# Processing
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_graphics();
generate_latex();

#
# Local functions
#

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp")
        or die "hnco-observable-evolution-stat.pl: generate_graphics: cannot open graphics.gp\n";

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"$obj->{xlabel}\"\n",
        "set ylabel \"$obj->{ylabel}\"\n",
        "set key outside top center box opaque horizontal\n",
        "set autoscale fix\n\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    if ($obj->{xlogscale}) {
        my $fmt = qq("10^{\%L}");
        print GRAPHICS
            "set logscale x\n",
            "set format x $fmt\n";
    } else {
        print GRAPHICS
            "unset logscale x\n",
            "set format x\n";
    }

    if ($obj->{ylogscale}) {
        my $fmt = qq("10^{\%L}");
        print GRAPHICS
            "set logscale y\n",
            "set format y $fmt\n";
    } else {
        print GRAPHICS
            "unset logscale y\n",
            "set format y\n";
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_title = qq("$function_id");
        my $quoted_path = qq("$path_graphics/$function_id.eps");

        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "set key title $quoted_title\n";

        print GRAPHICS "plot \\\n";
        my $position = 1;
        print GRAPHICS
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                $quoted_path = qq("$path_results/$function_id/$algorithm_id/1.out");
                $quoted_title = qq("$algorithm_id");
                $position++;
                "  $quoted_path using $xcolumn:$ycolumn with lines lw 1 title $quoted_title";
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
        or die "hnco-observable-evolution-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        print LATEX latex_section($function_id);
        print LATEX latex_begin_center();
        print LATEX latex_includegraphics($function_id);
        print LATEX latex_end_center();
    }
}
