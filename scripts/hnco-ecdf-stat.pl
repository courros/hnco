#! /usr/bin/perl -w

# Copyright (C) 2016, 2017 Arnaud Berny

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
use List::Util qw(min max);
use List::MoreUtils qw(all);

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

my $plan = "plan.json";
open(FILE, $plan)
    or die "hnco-ecdf-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) { $json .= $_; }

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $num_targets         = $obj->{num_targets};
my $path_graphics       = $obj->{graphics};
my $path_results        = $obj->{results};
my $path_report         = $obj->{report};

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

compute_ranges();

sub compute_ranges
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $prefix = "$path_results/$function_id/$algorithm_id";

            my $algorithm_num_runs = $num_runs;
            if ($a->{deterministic}) {
                $algorithm_num_runs = 1;
            }

            my @algorithm_min = ();
            my @algorithm_max = ();

            foreach (1 .. $algorithm_num_runs) {
                $path = "$prefix/$_.out";
                my $fh = IO::File->new($path, '<')
                    or die "hnco-ecdf-stat.pl: compute_ranges: Cannot open '$path': $!\n";
                my @lines = $fh->getlines;
                push @algorithm_min, get_value($lines[0]);
                push @algorithm_max, get_value($lines[-1]);
                $fh->close;
            }

            if (exists($f->{min})) {
                $f->{min} = min($f->{min}, min(@algorithm_min));
            } else {
                $f->{min} = min(@algorithm_min);
            }

            if (exists($f->{max})) {
                $f->{max} = max($f->{max}, max(@algorithm_max));
            } else {
                $f->{max} = max(@algorithm_max);
            }
        }
    }
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex") or die "hnco-benchmark-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        latex_newpage();

        latex_section("Function $function_id");

        latex_empty_line();

        latex_begin_center();
        latex_begin_figure();
        latex_includegraphics("$function_id");
        latex_caption("$function_id");
        latex_end_figure();
        latex_end_center();

        latex_empty_line();

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

sub get_value
{
    my $line = shift;
    chomp $line;
    my @results = split ' ', $line;
    return $results[1];
}
