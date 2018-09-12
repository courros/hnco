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

use strict;
use warnings;
use JSON;
use List::Util qw(min max);

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
my $path_report         = $obj->{report};
my $path_results        = $obj->{results};

unless (-d "$path_graphics") {
    mkdir "$path_graphics";
    print "Created $path_graphics\n";
}

foreach (@$functions) {
    compute_ranges($_);
    compute_targets($_);
    generate_ecdf($_);
}
generate_ecdf_all();
generate_graphics();
generate_graphics_all();
generate_latex();

sub compute_ranges
{
    my $fn = shift;
    my $function_id = $fn->{id};

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
            my $path = "$prefix/$_.out";
            my $fh = IO::File->new($path, '<')
                or die "hnco-ecdf-stat.pl: compute_ranges: Cannot open '$path': $!\n";
            my @lines = $fh->getlines;
            push @algorithm_min, get_value($lines[0]);
            push @algorithm_max, get_value($lines[-1]);
            $fh->close;
        }

        if (exists($fn->{min})) {
            $fn->{min} = min($fn->{min}, min(@algorithm_min));
        } else {
            $fn->{min} = min(@algorithm_min);
        }

        if (exists($fn->{max})) {
            $fn->{max} = max($fn->{max}, max(@algorithm_max));
        } else {
            $fn->{max} = max(@algorithm_max);
        }
    }
}

sub compute_targets
{
    my $fn = shift;
    my $function_id = $fn->{id};
    my @targets = map { $fn->{min} + $_ * ($fn->{max} - $fn->{min}) / $num_targets} (1 .. $num_targets);
    $fn->{targets} = \@targets;
}

sub generate_ecdf
{
    my $fn = shift;
    my $function_id = $fn->{id};

    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};
        my $prefix = "$path_results/$function_id/$algorithm_id";

        my $algorithm_num_runs = $num_runs;
        if ($algorithm->{deterministic}) {
            $algorithm_num_runs = 1;
        }
        my $algorithm_num_targets = $num_targets * $algorithm_num_runs;

        my @events = ();
        foreach (1 .. $algorithm_num_runs) {
            my $path = "$prefix/$_.out";
            my $fh = IO::File->new($path, '<')
                or die "hnco-ecdf-stat.pl: generate_ecdf: Cannot open '$path': $!\n";
            my @lines = $fh->getlines;
            $fh->close;

          TARGETS: {
              my $index = 0;
              foreach (@lines) {
                  my ($evaluation, $value) = get_evaluation_value($_);
                  while ($value >= @{ $fn->{targets} }[$index]) {
                      push @events, $evaluation;
                      $index++;
                      if ($index == $num_targets) {
                          last TARGETS;
                      }
                  }
              }
            }

        }

        # Sort @events by increasing order of number of evaluations
        my @sorted_events = sort { $a <=> $b } @events;

        my $path_ecdf = "$prefix/ecdf.txt";
        my $fh_ecdf = IO::File->new($path_ecdf, '>')
            or die "hnco-ecdf-stat.pl: generate_ecdf: Cannot open '$path_ecdf': $!\n";

        my $i = 0;
        while ($i < @sorted_events) {
            my $head = $sorted_events[$i];
            my $j = $i;
            while ($j < @sorted_events && $sorted_events[$j] == $head) {
                $j++;
            }
            $fh_ecdf->printf("%d %e\n", $head, $j / $algorithm_num_targets);
            $i = $j;
        }

        $fh_ecdf->close;
    }
}

sub generate_ecdf_all
{
    unless (-d "$path_results/ecdf") {
        mkdir "$path_results/ecdf";
        print "Created $path_results/ecdf\n";
    }

    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};

        my $algorithm_num_runs = $num_runs;
        if ($algorithm->{deterministic}) {
            $algorithm_num_runs = 1;
        }
        my $algorithm_num_targets = @$functions * $num_targets * $algorithm_num_runs;

        my @events = ();

        foreach my $fn (@$functions) {
            my $function_id = $fn->{id};
            my $prefix = "$path_results/$function_id/$algorithm_id";

            foreach (1 .. $algorithm_num_runs) {
                my $path = "$prefix/$_.out";
                my $fh = IO::File->new($path, '<')
                    or die "hnco-ecdf-stat.pl: generate_ecdf_all: Cannot open '$path': $!\n";
                my @lines = $fh->getlines;
                $fh->close;

              TARGETS: {
                  my $index = 0;
                  foreach (@lines) {
                      my ($evaluation, $value) = get_evaluation_value($_);
                      while ($value >= @{ $fn->{targets} }[$index]) {
                          push @events, $evaluation;
                          $index++;
                          if ($index == $num_targets) {
                              last TARGETS;
                          }
                      }
                  }
                }

            }
        }

        # Sort @events by increasing order of number of evaluations
        my @sorted_events = sort { $a <=> $b } @events;

        my $path_ecdf = "$path_results/ecdf/$algorithm_id.txt";
        my $fh_ecdf = IO::File->new($path_ecdf, '>')
            or die "hnco-ecdf-stat.pl: generate_ecdf_all: Cannot open '$path_ecdf': $!\n";

        my $i = 0;
        while ($i < @sorted_events) {
            my $head = $sorted_events[$i];
            my $j = $i;
            while ($j < @sorted_events && $sorted_events[$j] == $head) {
                $j++;
            }
            $fh_ecdf->printf("%d %e\n", $head, $j / $algorithm_num_targets);
            $i = $j;
        }

        $fh_ecdf->close;
    }
}

sub generate_graphics
{
    open(GRAPHICS, ">graphics.gp")
        or die "hnco-ecdf-stat.pl: generate_graphics: cannot open graphics.gp\n";

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"Number of evaluations\"\n",
        "set ylabel \"Proportion of targets\"\n",
        "set key outside top center box opaque horizontal\n",
        "set format x ", quote("10^{%L}"), "\n",
        "set logscale x\n",
        "set autoscale fix\n\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        my $quoted_title = quote("$function_id");
        my $quoted_path = quote("$path_graphics/$function_id.eps");

        print GRAPHICS
            $terminal{eps}, "\n",
            "set output $quoted_path\n",
            "set key title $quoted_title\n";

        print GRAPHICS "plot \\\n";
        print GRAPHICS
            join ", \\\n",
            (map {
                my $algorithm_id = $_->{id};
                $quoted_path = quote("$path_results/$function_id/$algorithm_id/ecdf.txt");
                $quoted_title = quote("$algorithm_id");
                "  $quoted_path using 1:2 with steps title $quoted_title";
             } @$algorithms);
        print GRAPHICS "\n";

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

    system("chmod a+x graphics.gp");
}

sub generate_graphics_all
{
    open(GRAPHICS, ">graphics-all.gp")
        or die "hnco-ecdf-stat.pl: generate_graphics_all: cannot open graphics.gp\n";

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set xlabel \"Number of evaluations\"\n",
        "set ylabel \"Proportion of targets\"\n",
        "set key outside top center box opaque horizontal\n",
        "set format x ", quote("10^{%L}"), "\n",
        "set logscale x\n",
        "set autoscale fix\n\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n";

    my $quoted_title = quote("All functions");
    my $quoted_path = quote("$path_graphics/all.eps");

    print GRAPHICS
        $terminal{eps}, "\n",
        "set output $quoted_path\n",
        "set key title $quoted_title\n";

    print GRAPHICS "plot \\\n";
    print GRAPHICS
        join ", \\\n",
        (map {
            my $algorithm_id = $_->{id};
            $quoted_path = quote("$path_results/ecdf/$algorithm_id.txt");
            $quoted_title = quote("$algorithm_id");
            "  $quoted_path using 1:2 with steps title $quoted_title";
         } @$algorithms);
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

    system("chmod a+x graphics-all.gp");
}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex")
        or die "hnco-ecdf-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    latex_section("All Functions");
    latex_empty_line();
    latex_begin_center();
    latex_includegraphics("all");
    latex_end_center();
    latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};

        latex_section("Function $function_id");
        latex_empty_line();
        latex_begin_center();
        latex_includegraphics("$function_id");
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
\\includegraphics[width=\\linewidth]{$path}
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

sub get_evaluation_value
{
    my $line = shift;
    chomp $line;
    return split ' ', $line;
}
