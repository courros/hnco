#! /usr/bin/perl -w

# Copyright (C) 2016 Arnaud Berny

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

open(FILE, $plan) or die "Error: cannot open $plan\n";

my $json = "";
while (<FILE>) {
    $json .= $_;
}

my $obj = from_json($json);
my $path_results = $obj->{results};
my $path_report = $obj->{report};
my $path_graphics = $obj->{graphics};
my $functions = $obj->{functions};
my $algorithms = $obj->{algorithms};

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

generate_graphics();
generate_latex();

sub generate_graphics
{
    open(PARTS, ">graphics-parts.gp") or die "Error: cannot open graphics-parts.gp\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_title = quote("$function_id");
        my $quoted_path = quote("$path_graphics/$function_id.eps");

        print PARTS
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "set key title $quoted_title\n";

        print PARTS "plot \\\n";
        print PARTS
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                $quoted_path = quote("$path_results/$function_id/$algorithm_id.log");
                $quoted_title = quote("$algorithm_id");
                "  $quoted_path with lines title $quoted_title";
             } @$algorithms);

        print PARTS "\n";

        $quoted_path = quote("$path_graphics/$function_id.pdf");
        print PARTS
            $terminal{pdf}, "\n",
            "set output $quoted_path\n",
            "replot\n";

        $quoted_path = quote("$path_graphics/$function_id.png");
        print PARTS
            $terminal{png}, "\n",
            "set output $quoted_path\n",
            "replot\n\n";
    }
}

sub generate_latex
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        latex_begin_figure();
        latex_includegraphics("$function_id");
        latex_end_figure();
    }
}

sub quote
{
    my $s = shift;
    return "\"$s\"";
}

sub latex_begin_figure()
{
    print <<EOF;
\\begin{figure}[h]
\\centering
EOF
}

sub latex_includegraphics
{
    my $id = shift;
    print <<EOF
\\includegraphics[width=\\linewidth]{$id}
EOF
}

sub latex_end_figure()
{
    print <<EOF;
\\end{figure}
EOF
}
