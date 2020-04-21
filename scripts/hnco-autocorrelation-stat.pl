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

use JSON;
use Statistics::Descriptive;

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan) or die "hnco-autocorrelation-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);

my $path_results        = $obj->{results};
my $path_report         = $obj->{report};
my $path_graphics       = $obj->{graphics};
my $functions           = $obj->{functions};
my $lag_max             = $obj->{lag_max};

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_autocorrelation();
generate_graphics();
generate_latex();

sub generate_autocorrelation
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        print "$function_id: ";

        my $path = "$path_results/$function_id/1.out";
        my $file = IO::File->new($path, '<') or die "hnco-autocorrelation-stat.pl: generate_autocorrelation: Cannot open '$path': $!\n";
        my @rw = ();
        while (defined(my $line = $file->getline)) {
            chomp $line;
            push @rw, $line;
        }
        $file->close;

        my $SD = Statistics::Descriptive::Full->new();
        $SD->add_data(@rw);
        my $mean = $SD->mean();
        my $standard_deviation = $SD->standard_deviation();

        my @autocorrelation = (0.0) x ($lag_max);
        for (my $tau = 0; $tau < $lag_max; $tau++) {
            for (my $t = 0; $t < @rw - $tau; $t++) {
                $autocorrelation[$tau] += ($rw[$t] - $mean) * ($rw[$t + $tau] - $mean);
            }
        }
        my $norm = $autocorrelation[0];
        for (my $tau = 0; $tau < $lag_max; $tau++) {
            $autocorrelation[$tau] /= $norm;
        }

        $path = "$path_results/$function_id/autocorrelation.txt";
        $file = IO::File->new($path, '>') or die "hnco-autocorrelation-stat.pl: generate_autocorrelation: Cannot open '$path': $!\n";
        foreach (@autocorrelation) {
            $file->print("$_\n");
        }
        $file->close;

        print "done\n";
    }
}

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp") or die "hnco-autocorrelation-stat.pl: generate_graphics: cannot open graphics.gp\n";
    print GRAPHICS "#!/usr/bin/gnuplot -persist\n";

    print GRAPHICS
        "set grid\n",
        "set xlabel \"Time lag\"\n",
        "set ylabel \"Autocorrelation\"\n",
        "set key top right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    my $fmt = quote("10^{\%T}");
    #print GRAPHICS
    #"set logscale y\n",
    #"set format y $fmt\n";

    print GRAPHICS "\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_title = quote("$function_id");

        my $quoted_path = quote("$path_graphics/$function_id.eps");
        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n";

        $quoted_path = quote("$path_results/$function_id/autocorrelation.txt");
        print GRAPHICS "plot $quoted_path with lines lw 2 title $quoted_title\n";

        $quoted_path = quote("$path_graphics/$function_id.pdf");
        print GRAPHICS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id.png");
        print GRAPHICS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";
    }

    #
    # All functions on the same graphics
    #

    generate_group("all", $functions);

    close(GRAPHICS);
    system("chmod a+x graphics.gp");
}

sub generate_group
{
    my ($id, $ref) = @_;

    my $quoted_title = quote("All functions");
    print GRAPHICS
        "set key font \",10\" outside right top box vertical title $quoted_title font \",10\"\n";

    my $quoted_path = quote("$path_graphics/$id.eps");
    print GRAPHICS
        $terminal{eps}, "\n",
        "set output $quoted_path\n";

    print GRAPHICS "plot \\\n";
    print GRAPHICS
        join ", \\\n",
        (map {
            my $function_id = $_->{id};
            $quoted_title = quote("$function_id");
            $quoted_path = quote("$path_results/$function_id/autocorrelation.txt");
            "  $quoted_path with lines lw 1 title $quoted_title";
         } @$ref);

    print GRAPHICS "\n";

    $quoted_path = quote("$path_graphics/$id.pdf");
    print GRAPHICS
        $terminal{pdf}, "\n",
        "set output $quoted_path\n",
        "replot\n";

    $quoted_path = quote("$path_graphics/$id.png");
    print GRAPHICS
        $terminal{png}, "\n",
        "set output $quoted_path\n",
        "replot\n\n";
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-autocorrelation-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";

    latex_section("All functions");
    latex_begin_center();
    latex_includegraphics("all");
    latex_end_center();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        latex_section($function_id);
        latex_begin_center();
        latex_includegraphics("$function_id");
        latex_end_center();
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

sub latex_includegraphics
{
    my $id = shift;
    print LATEX <<EOF
\\includegraphics[width=\\linewidth]{$id}
EOF
}

sub quote
{
    my $s = shift;
    return "\"$s\"";
}
