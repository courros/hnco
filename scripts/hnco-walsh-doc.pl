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

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan)
    or die "hnco-walsh-doc.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);

my $path_results        = $obj->{results};
my $path_report         = $obj->{report};
my $path_graphics       = $obj->{graphics};
my $functions           = $obj->{functions};

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_graphics();
generate_latex();

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp")
        or die "hnco-walsh-doc.pl: generate_graphics: cannot open graphics.gp\n";

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"Coefficient rank\"\n",
        "set ylabel \"Amplitude\"\n",
        "set key top right box opaque\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    print GRAPHICS "\n";

    if ($obj->{xlogscale}) {
        my $fmt = quote("10^{\%L}");
        print GRAPHICS
            "set logscale x\n",
            "set format x $fmt\n";
    } else {
        print GRAPHICS
            "unset logscale x\n",
            "set format x\n";
    }

    if ($obj->{ylogscale}) {
        my $fmt = quote("10^{\%L}");
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

        my $quoted_title = quote("$function_id");

        my $quoted_path = quote("$path_graphics/$function_id.eps");
        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n";

        $quoted_path = quote("$path_results/$function_id/1.out");
        print GRAPHICS "plot $quoted_path using 3 with lines lw 2 title $quoted_title\n";

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

    print GRAPHICS
        "unset key\n";

    $quoted_path = quote("$path_graphics/all.eps");
    print GRAPHICS
        $terminal{eps}, "\n",
        "set output $quoted_path\n";

    print GRAPHICS "plot \\\n";
    print GRAPHICS
        join ", \\\n",
        (map {
            my $function_id = $_->{id};
            $quoted_path = quote("$path_results/$function_id/1.out");
            #$quoted_title = quote("$function_id");
            "  $quoted_path using 3 with lines lw 1 notitle";
         } @$functions);

    print GRAPHICS "\n";

    $quoted_path = quote("$path_graphics/all.pdf");
    print GRAPHICS
        $terminal{pdf}, "\n",
        "set output $quoted_path\n",
        "replot\n";

    $quoted_path = quote("$path_graphics/all.png");
    print GRAPHICS
        $terminal{png}, "\n",
        "set output $quoted_path\n",
        "replot\n\n";

    system("chmod a+x graphics.gp");
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-walsh-doc.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        latex_section($function_id);
        latex_begin_center();
        latex_includegraphics($function_id);
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
