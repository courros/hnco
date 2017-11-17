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

my $plan = "plan.json";
open(FILE, $plan)
    or die "hnco-ecdf-stat.pl: Cannot open $plan\n";
my $json = "";
while (<FILE>) { $json .= $_; }

my $obj = from_json($json);

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $path_graphics       = $obj->{graphics};
my $path_results        = $obj->{results};
my $path_report         = $obj->{report};

my $ranges = {};
my $stat_eval = {};
my $stat_value = {};
my $stat_value_best = {};

my %terminal = (
    eps => "set term epscairo color enhanced",
    pdf => "set term pdfcairo color enhanced",
    png => "set term png enhanced" );

unless (-d "$path_graphics") { mkdir "$path_graphics"; }

compute_statistics();
compute_best_statistics();
compute_ranges();

generate_function_data();
generate_latex();

sub compute_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $eval = {};
        my $value = {};

        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $algorithm_num_runs = $num_runs;
            if ($a->{deterministic}) {
                $algorithm_num_runs = 1;
            }

            my $prefix = "$path_results/$function_id/$algorithm_id";
            my $SD_eval = Statistics::Descriptive::Full->new();
            my $SD_value = Statistics::Descriptive::Full->new();

            my $path = "$prefix/$algorithm_id.dat";
            my $file_data = IO::File->new($path, '>')
                or die "hnco-benchmark-stat.pl: compute_statistics: Cannot open '$path': $!\n";
            foreach (1 .. $algorithm_num_runs) {
                $path = "$prefix/$_.out";
                my $file_run = IO::File->new($path, '<')
                    or die "hnco-benchmark-stat.pl: compute_statistics: Cannot open '$path': $!\n";
                my $line = $file_run->getline;
                $file_data->print($line);
                chomp $line;
                my @results = split ' ', $line;
                $SD_eval->add_data($results[0]);
                $SD_value->add_data($results[1]);
                $file_run->close;
            }
            $file_data->close;

            $eval->{$algorithm_id} = { min     => $SD_eval->min(),
                                       q1      => $SD_eval->quantile(1),
                                       median  => $SD_eval->median(),
                                       q3      => $SD_eval->quantile(3),
                                       max     => $SD_eval->max() };

            $value->{$algorithm_id} = { min     => $SD_value->min(),
                                        q1      => $SD_value->quantile(1),
                                        median  => $SD_value->median(),
                                        q3      => $SD_value->quantile(3),
                                        max     => $SD_value->max() };

        }

        $stat_eval->{$function_id} = $eval;
        $stat_value->{$function_id} = $value;

    }

}

sub compute_best_statistics
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $stat = $stat_value->{$function_id};
        my $best = {};

        foreach (@summary_statistics) {
            my @values = ();
            foreach my $a (@$algorithms) {
                my $algorithm_id = $a->{id};
                push @values, $stat->{$algorithm_id}->{$_};
            }
            $best->{$_} = max @values;
        }

        $stat_value_best->{$function_id} = $best;

    }

}

sub compute_ranges
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $eval = $stat_eval->{$function_id};
        my $value = $stat_value->{$function_id};
        my $range = {};

        $range->{xmin} = min (map { $eval->{$_}->{min} } keys %$eval);
        $range->{xmax} = max (map { $eval->{$_}->{max} } keys %$eval);
        $range->{ymin} = min (map { $value->{$_}->{min} } keys %$value);
        $range->{ymax} = max (map { $value->{$_}->{max} } keys %$value);

        $ranges->{$function_id} = $range;
    }

}

sub generate_function_data
{
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $path = "$path_results/$function_id/$function_id.dat";

        $file = IO::File->new($path, '>')
            or die "hnco-benchmark-stat.pl: generate_function_data: Cannot open '$path': $!\n";

        my $position = 1;
        foreach my $a (@$algorithms) {
            my $algorithm_id = $a->{id};
            my $stat = $stat_value->{$function_id}->{$algorithm_id};

            $file->printf("%d %e %e %e %e %e %s\n",
                          $position,
                          $stat->{min},
                          $stat->{q1},
                          $stat->{median},
                          $stat->{q3},
                          $stat->{max},
                          $algorithm_id);

            $position++;
        }

        $file->close();

    }

}

sub generate_latex
{
    open(LATEX, ">$path_report/results.tex") or die "hnco-benchmark-stat.pl: generate_latex: Cannot open $path_report/results.tex\n";

    print LATEX "\\graphicspath{{../$path_graphics/}}\n";
    latex_empty_line();

    latex_section("Rankings");
    latex_begin_center();
    latex_rankings_table_begin();
    latex_rankings_table_body();
    latex_rankings_table_end();
    latex_end_center();
    latex_empty_line();

    # latex_section("Algorithm index table");
    # latex_index_table();
    # latex_empty_line();

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $value = $stat_value->{$function_id};
        my $best = $stat_value_best->{$function_id};

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

sub latex_tableofcontents
{
    print LATEX <<EOF;
\\tableofcontents
EOF
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
