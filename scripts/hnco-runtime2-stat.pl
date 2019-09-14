#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

my @summary_statistics = qw(min q1 median q3 max);

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

my $plan = "plan.json";
if (@ARGV) {
    $plan = shift @ARGV;
}
print "Using $plan\n";

open(FILE, $plan)
    or die "hnco-runtime2-stat.pl: Cannot open '$plan': $!\n";
my $json = "";
while (<FILE>) { $json .= $_; }

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $budget              = $obj->{budget};
my $num_runs            = $obj->{num_runs};
my $parallel            = $obj->{parallel};
my $parameter1          = $obj->{parameter1};
my $parameter2          = $obj->{parameter2};
my $servers             = $obj->{servers};

foreach ($parameter1, $parameter2) {
    if ($_->{values_perl}) {
        my @tmp = eval $_->{values_perl};
        $_->{values} = \@tmp;
    }
}

my $root_graphics       = "graphics";
my $root_report         = "report";
my $root_results        = "results";
my $root_stats          = "stats";

my $data = {};

add_missing_names($algorithms);
compute_statistics();
foreach (@summary_statistics) {
    generate_data($_);
}
generate_gnuplot();
generate_latex();

sub add_missing_names
{
    my $list = shift;
    foreach (@$list) {
        if (!exists($_->{name})) {
            $_->{name} = $_->{id};
        }
    }
}

sub compute_statistics
{
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        $data->{$algorithm_id} = {};

        my $algorithm_num_runs = $num_runs;
        if ($a->{deterministic}) {
            $algorithm_num_runs = 1;
        }

        foreach my $v1 (@{ $parameter1->{values} }) {
            my $key1 = "$parameter1->{id}-$v1";
            $data->{$algorithm_id}->{$key1} = {};
            foreach my $v2 (@{ $parameter2->{values} }) {
                my $key2 = "$parameter2->{id}-$v2";
                my $prefix = "$root_results/$algorithm_id/$key1/$key2";
                my $SD = Statistics::Descriptive::Full->new();
                foreach (1 .. $algorithm_num_runs) {
                    my $path = "$prefix/$_.out";
                    if (-f $path) {
                        my $obj = from_json(read_file($path));
                        $SD->add_data($obj->{total_num_evaluations});
                    } else {
                        die "hnco-runtime2-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                    }
                }
                $data->{$algorithm_id}->{$key1}->{$key2} = { min       => $SD->min(),
                                                             q1        => $SD->quantile(1),
                                                             median    => $SD->median(),
                                                             q3        => $SD->quantile(3),
                                                             max       => $SD->max(),
                                                             mean      => $SD->mean(),
                                                             stddev    => $SD->standard_deviation() };
            }
        }
    }
}

sub generate_data
{
    my $measure = shift;

    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        my $prefix = "$root_stats/$algorithm_id";

        foreach my $v1 (@{ $parameter1->{values} }) {
            my $key1 = "$parameter1->{id}-$v1";
            my $path = "$prefix/$measure-$key1.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!\n";
            foreach my $v2 (@{ $parameter2->{values} }) {
                my $key2 = "$parameter2->{id}-$v2";
                $file->printf("%e %e\n", $v2, $data->{$algorithm_id}->{$key1}->{$key2}->{$measure});
            }
            $file->close();
        }

        foreach my $v2 (@{ $parameter2->{values} }) {
            my $key2 = "$parameter2->{id}-$v2";
            my $path = "$prefix/$measure-$key2.dat";
            my $file = IO::File->new($path, '>')
                or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!\n";
            foreach my $v1 (@{ $parameter1->{values} }) {
                my $key1 = "$parameter1->{id}-$v1";
                $file->printf("%e %e\n", $v1, $data->{$algorithm_id}->{$key1}->{$key2}->{$measure});
            }
            $file->close();
        }

        my $path = "$prefix/$measure.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-runtime2-stat.pl: generate_data: Cannot open '$path': $!\n";
        foreach my $v1 (@{ $parameter1->{values} }) {
            my $key1 = "$parameter1->{id}-$v1";
            foreach my $v2 (@{ $parameter2->{values} }) {
                my $key2 = "$parameter2->{id}-$v2";
                $file->printf("%e %e %e\n", $v1, $v2, $data->{$algorithm_id}->{$key1}->{$key2}->{$measure});
            }
        }
        $file->close();

    }
}

sub generate_gnuplot
{
    my $path = "mean.gp";
    open(GRAPHICS, ">$path")
        or die "hnco-runtime2-stat.pl: generate_gnuplot: Cannot open '$path': $!\n";

    print GRAPHICS
        "#!/usr/bin/gnuplot -persist\n",
        "set grid\n",
        "set ylabel \"Runtime\"\n",
        "set logscale y\n",
        "set format y", quote("10^{\%T}"), "\n",
        "set autoscale fix\n",
        "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n";

    foreach my $a (@$algorithms) {
        foreach my $measure (@summary_statistics) {
            generate_gnuplot_section($a, $parameter1, $parameter2, $measure);
            generate_gnuplot_section($a, $parameter2, $parameter1, $measure);
        }
    }

    close(GRAPHICS);
    system("chmod a+x mean.gp");
}

sub generate_gnuplot_section
{
    my ($a, $variable, $parameter, $measure) = @_;

    my $algorithm_id = $a->{id};
    my $prefix_stats = "$root_stats/$algorithm_id";
    my $prefix_graphics = "$root_graphics/$algorithm_id";

    my $quoted_string = quote("$variable->{name}");
    print GRAPHICS "set xlabel $quoted_string\n";

    $quoted_string = quote("$parameter->{name}");
    print GRAPHICS "set key font \",10\" reverse Left outside right center box vertical title $quoted_string font \",10\"\n";

    my $path = "$prefix_graphics/$measure-$parameter->{id}";
    $quoted_string = quote("$path.pdf");
    print GRAPHICS
        $terminal{pdf}, "\n",
        "set output $quoted_string\n";
    print GRAPHICS "plot \\\n";
    print GRAPHICS
        join ", \\\n",
        (map {
            my $key = "$parameter->{id}-$_";
            my $path = quote("$prefix_stats/$measure-$key.dat");
            my $title = quote("$parameter->{key} = $_");
            "  $path using 1:2 with l lw 2 title $title";
         } reverse(@{ $parameter->{values} }));
    print GRAPHICS "\n";

    $quoted_string = quote("$path.eps");
    print GRAPHICS
        $terminal{eps}, "\n",
        "set output $quoted_string\n",
        "replot\n";

    $quoted_string = quote("$path.png");
    print GRAPHICS
        $terminal{png}, "\n",
        "set output $quoted_string\n",
        "replot\n\n";
}

sub generate_latex
{
    open(LATEX, ">$root_report/results.tex")
        or die "hnco-runtime2-stat.pl: generate_latex: Cannot open '$root_report/results.tex': $!\n";

    print LATEX "\\graphicspath{{../$root_graphics/}}\n";
    latex_empty_line();
    foreach my $a (@$algorithms) {
        my $algorithm_id = $a->{id};
        latex_section("$a->{name}");
        foreach ($parameter1, $parameter2) {
            latex_subsection("$_->{name}");
            foreach my $measure (@summary_statistics) {
                latex_subsubsection("$measure");
                latex_begin_center();
                latex_includegraphics("$algorithm_id/$measure-$_->{id}");
                latex_end_center();
            }
        }
    }

    close(LATEX);
}

sub latex_section
{
    my ($title) = @_;
    print LATEX <<EOF;
\\section{$title}
EOF
}

sub latex_subsection
{
    my ($title) = @_;
    print LATEX <<EOF;
\\subsection{$title}
EOF
}

sub latex_subsubsection
{
    my ($title) = @_;
    print LATEX <<EOF;
\\subsubsection{$title}
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
        open my $fh, '<', $path
            or die "hnco-runtime2-stat.pl: compute_statistics: Cannot open '$path': $!\n";
        $json = <$fh>;
    }
    return $json;
}
