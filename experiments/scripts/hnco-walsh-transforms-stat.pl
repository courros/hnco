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

my $functions           = $obj->{functions};

my $path_results        = $obj->{results};
my $path_report         = $obj->{report};
my $path_graphics       = $obj->{graphics};

#
# Processing
#

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_spectrum();
generate_graphics();
generate_latex();

#
# Local functions
#

sub generate_spectrum
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $path = "$path_results/$function_id/1.out";
        my $file = IO::File->new($path, '<')
            or die "hnco-walsh-transforms-stat.pl: generate_spectrum: Cannot open '$path': $!\n";
        my @walsh_transform = ();
        while (defined(my $line = $file->getline)) {
            chomp $line;
            my ($order, $coefficient) = split ' ', $line;
            push @walsh_transform, { order => $order, coefficient => $coefficient };
        }
        $file->close;

        my @sorted_walsh_transform = sort { $a->{order} <=> $b->{order} } @walsh_transform;
        my %spectrum = ();
        my $i = 0;
        while ($i < @sorted_walsh_transform) {
            my $energy = ($sorted_walsh_transform[$i]->{coefficient})**2;
            my $order = $sorted_walsh_transform[$i]->{order};
            my $j = $i + 1;
            while ($j < @sorted_walsh_transform && $sorted_walsh_transform[$j]->{order} == $order) {
                $energy += ($sorted_walsh_transform[$j]->{coefficient})**2;
                $j++;
            }
            $spectrum{$order} = $energy;
            $i = $j;
        }

        $path = "$path_results/$function_id/spectrum.dat";
        $file = IO::File->new($path, '>')
            or die "hnco-walsh-transforms-stat.pl: generate_spectrum: Cannot open '$path': $!\n";
        while (my ($order, $energy) = each %spectrum) {
            $file->print("$order $energy\n");
        }
        $file->close;

    }
}

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp")
        or die "hnco-walsh-transforms-stat.pl: generate_graphics: cannot open graphics.gp\n";
    print GRAPHICS "#!/usr/bin/gnuplot -persist\n";
    generate_graphics_coefficients();
    generate_graphics_spectrum();
    close(GRAPHICS);
    system("chmod a+x graphics.gp");
}

sub generate_graphics_coefficients
{
    print GRAPHICS
        "set grid\n",
        "set xlabel \"Coefficient rank\"\n",
        "set ylabel \"Amplitude\"\n",
        "set key top right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    my $fmt = qq("10^{\%T}");
    print GRAPHICS
        "set logscale y\n",
        "set format y $fmt\n";

    print GRAPHICS "\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_title = qq("$function_id");

        my $quoted_path = qq("$path_graphics/$function_id-coef.eps");
        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n";

        $quoted_path = qq("$path_results/$function_id/1.out");
        print GRAPHICS "plot $quoted_path using 2 with lines lw 2 title $quoted_title\n";

        $quoted_path = qq("$path_graphics/$function_id-coef.pdf");
        print GRAPHICS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = qq("$path_graphics/$function_id-coef.png");
        print GRAPHICS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";
    }

    #
    # All functions on the same graphics
    #

    generate_group("all", $functions);

    if ($obj->{groups}) {
        foreach(@{$obj->{groups}}) {
            generate_group($_->{id}, $_->{functions});
        }
    }

}

sub generate_group
{
    my ($id, $ref) = @_;

    print GRAPHICS "unset key\n";

    my $quoted_path = qq("$path_graphics/$id.eps");
    print GRAPHICS
        $terminal{eps}, "\n",
        "set output $quoted_path\n";

    print GRAPHICS "plot \\\n";
    print GRAPHICS
        join ", \\\n",
        (map {
            my $function_id = $_->{id};
            $quoted_path = qq("$path_results/$function_id/1.out");
            "  $quoted_path using 2 with lines lw 1 notitle";
         } @$ref);

    print GRAPHICS "\n";

    $quoted_path = qq("$path_graphics/$id.pdf");
    print GRAPHICS
        $terminal{pdf}, "\n",
        "set output $quoted_path\n",
        "replot\n";

    $quoted_path = qq("$path_graphics/$id.png");
    print GRAPHICS
        $terminal{png}, "\n",
        "set output $quoted_path\n",
        "replot\n\n";
}

sub generate_graphics_spectrum
{
    print GRAPHICS
        "set grid\n",
        "set xlabel \"Feature Hamming weight\"\n",
        "set ylabel \"Energy\"\n",
        "set key top right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    my $fmt = qq("10^{\%T}");
    print GRAPHICS
        "set logscale y\n",
        "set format y $fmt\n";

    print GRAPHICS "\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_title = qq("$function_id");

        my $quoted_path = qq("$path_graphics/$function_id-spectrum.eps");
        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n";

        $quoted_path = qq("$path_results/$function_id/spectrum.dat");
        print GRAPHICS
            "plot $quoted_path using 1:2 w impulses lw 2 title $quoted_title\n";

        $quoted_path = qq("$path_graphics/$function_id-spectrum.pdf");
        print GRAPHICS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = qq("$path_graphics/$function_id-spectrum.png");
        print GRAPHICS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";
    }

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-walsh-transforms-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX latex_graphicspath($path_graphics);

    print LATEX latex_section("All functions");
    print LATEX latex_begin_center();
    print LATEX latex_includegraphics("all");
    print LATEX latex_end_center();

    if ($obj->{groups}) {
        foreach(@{$obj->{groups}}) {
            print LATEX latex_section($_->{id});
            print LATEX latex_begin_center();
            print LATEX latex_includegraphics($_->{id});
            print LATEX latex_end_center();
        }
    }

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        print LATEX latex_section($function_id);
        print LATEX latex_begin_center();
        print LATEX latex_includegraphics("$function_id-coef", 0.6);
        print LATEX latex_includegraphics("$function_id-spectrum", 0.6);
        print LATEX latex_end_center();
    }
}
