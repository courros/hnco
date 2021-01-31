#! /usr/bin/perl -w

# Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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
use File::Slurp qw(read_file);
use POSIX;

use HNCO::Report qw(
    %terminal
    latex_graphicspath
    latex_section
    latex_subsection
    latex_subsubsection
    latex_begin_center
    latex_end_center
    latex_begin_figure
    latex_includegraphics
    latex_caption
    latex_end_figure
    latex_newpage
    latex_input_file
    latex_tableofcontents
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

my $algorithms          = $obj->{algorithms};
my $functions           = $obj->{functions};
my $num_runs            = $obj->{num_runs};
my $num_targets         = $obj->{num_targets};
my $budget              = $obj->{budget};
my $graphics            = $obj->{graphics};
my $groups              = $graphics->{groups};

#
# Make directories
#

my @directories = ("$path_graphics",
                   "$path_results",
                   "$path_results/ecdf");

foreach my $section ("global", map { $_->{id} } @$functions) {
    push @directories, "$path_results/ecdf/$section";
    foreach ("raw", "all", "groups") {
        push @directories, "$path_results/ecdf/$section/$_";
    }
    foreach (map { $_->{id} } @$groups) {
        push @directories, "$path_results/ecdf/$section/groups/$_";
    }
}

make_directories(@directories);

sub make_directories
{
    foreach (@_) {
        unless (-d "$_") {
            mkdir "$_";
            print "Created $_\n";
        }
    }
}

#
# Processing
#

foreach (@$functions) {
    compute_ranges($_);
    compute_targets($_);
    generate_ecdf_function_raw($_);
    if ($graphics->{all}->{helper}) {
        generate_ecdf_function_all($_->{id});
    }
    generate_ecdf_function_groups($_->{id});
}
generate_ecdf_global_raw();
if ($graphics->{all}->{helper}) {
    generate_ecdf_function_all("global");
}
generate_ecdf_function_groups("global");

generate_gnuplot_global_all();
generate_gnuplot_global_groups();
generate_gnuplot_function_all();
generate_gnuplot_function_groups();
generate_latex();

#
# Local functions
#

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
            my $file = IO::File->new($path, '<')
                or die "hnco-ecdf-stat.pl: compute_ranges: Cannot open '$path': $!\n";
            my @lines = $file->getlines();
            push @algorithm_min, get_value($lines[0]);
            push @algorithm_max, get_value($lines[-1]);
            $file->close;
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

sub collect_events
{
    my ($function_id, $targets, $algorithm_id, $algorithm_num_runs) = @_;
    my @events = ();
    foreach (1 .. $algorithm_num_runs) {
        my $path = "$path_results/$function_id/$algorithm_id/$_.out";
        my $file = IO::File->new($path, '<')
            or die "hnco-ecdf-stat.pl: generate_ecdf_function_raw: Cannot open '$path': $!\n";
        my @lines = $file->getlines();
        $file->close();
      TARGETS: {
          my $index = 0;
          foreach (@lines) {
              my ($time, $value) = get_time_value($_);
              while ($value >= @{ $targets }[$index]) {
                  push @events, $time;
                  $index++;
                  if ($index == $num_targets) {
                      last TARGETS;
                  }
              }
          }
        }
    }
    return @events;
}

sub generate_ecdf_function_raw
{
    my $fn = shift;
    my $function_id = $fn->{id};
    foreach my $algorithm (@$algorithms) {
        my $algorithm_id = $algorithm->{id};
        my $algorithm_num_runs = $num_runs;
        if ($algorithm->{deterministic}) {
            $algorithm_num_runs = 1;
        }
        my $algorithm_num_targets = $num_targets * $algorithm_num_runs;
        my @events = collect_events($function_id, $fn->{targets}, $algorithm_id, $algorithm_num_runs);
        my @sorted_events = sort { $a <=> $b } @events;
        my $path = "$path_results/ecdf/$function_id/raw/$algorithm_id.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-ecdf-stat.pl: generate_ecdf_function_raw: Cannot open '$path': $!\n";
        my $i = 0;
        while ($i < @sorted_events) {
            my $head = $sorted_events[$i];
            my $j = $i;
            while ($j < @sorted_events && $sorted_events[$j] == $head) {
                $j++;
            }
            $file->printf("%d %e\n", $head, $j / $algorithm_num_targets);
            $i = $j;
        }
        $file->close();
    }
}

sub generate_ecdf_global_raw
{
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
            push @events, collect_events($function_id, $fn->{targets}, $algorithm_id, $algorithm_num_runs);
        }
        my @sorted_events = sort { $a <=> $b } @events;
        my $path = "$path_results/ecdf/global/raw/$algorithm_id.dat";
        my $file = IO::File->new($path, '>')
            or die "hnco-ecdf-stat.pl: generate_ecdf_global_raw: Cannot open '$path': $!\n";
        my $i = 0;
        while ($i < @sorted_events) {
            my $head = $sorted_events[$i];
            my $j = $i;
            while ($j < @sorted_events && $sorted_events[$j] == $head) {
                $j++;
            }
            $file->printf("%d %e\n", $head, $j / $algorithm_num_targets);
            $i = $j;
        }
        $file->close();
    }
}

sub sort_algorithms
{
    my ($prefix, $ids) = @_;
    my %levels = ();
    foreach my $id (@$ids) {
        my $path = "$prefix/$id.dat";
        my $file = IO::File->new($path, '<')
            or die "hnco-ecdf-stat.pl: sort_algorithms: Cannot open '$path': $!\n";
        my @lines = $file->getlines();
        $file->close();
        $levels{$id} = get_value($lines[-1]);
    }
    return sort { $levels{$b} <=> $levels{$a} } keys %levels;
}

sub compute_ecdf_ranges
{
    my ($prefix, $ids) = @_;
    my @lows = ();
    my @highs = ();
    foreach my $id (@$ids) {
        my $path = "$prefix/$id.dat";
        my $file = IO::File->new($path, '<')
            or die "hnco-ecdf-stat.pl: compute_ecdf_ranges: Cannot open '$path': $!\n";
        my @lines = $file->getlines();
        $file->close();
        push @lows, get_value($lines[0]);
        push @highs, get_value($lines[-1]);
    }
    return min(@lows), max(@highs);
}

sub generate_ecdf_function_all
{
    my $function_id = shift;
    my @ids = map { $_->{id} } @$algorithms;
    my $prefix = "$path_results/ecdf/$function_id";
    my @sorted = reverse(sort_algorithms("$prefix/raw", \@ids));
    my ($low, $high) = compute_ecdf_ranges("$prefix/raw", \@ids);
    my $i = 0;
    foreach (@sorted) {
        my $path = "$prefix/raw/$_.dat";
        my $file = IO::File->new($path, '<')
            or die "hnco-ecdf-stat.pl: generate_ecdf_function_all: Cannot open '$path': $!\n";
        my @lines = $file->getlines();
        $file->close();
        $path = "$prefix/all/$_.dat";
        $file = IO::File->new($path, '>')
            or die "hnco-ecdf-stat.pl: generate_ecdf_function_all: Cannot open '$path': $!\n";
        $file->print(@lines);
        $file->printf("%d %e\n", $budget, get_value($lines[-1]));
        $file->printf("%d %e\n", 10 * $budget, $low + ($i + 0.5) * ($high - $low) / @sorted);
        $file->close();
        $i++;
    }
}

sub generate_ecdf_function_groups
{
    my $function_id = shift;
    my $prefix = "$path_results/ecdf/$function_id";
    foreach my $group (@$groups) {
        next unless ($group->{helper});
        my $group_id = $group->{id};
        my @sorted = reverse(sort_algorithms("$prefix/raw", $group->{algorithms}));
        my ($low, $high) = compute_ecdf_ranges("$prefix/raw", $group->{algorithms});
        my $i = 0;
        foreach (@sorted) {
            my $path = "$prefix/raw/$_.dat";
            my $file = IO::File->new($path, '<')
                or die "hnco-ecdf-stat.pl: generate_ecdf_function_all: Cannot open '$path': $!\n";
            my @lines = $file->getlines();
            $file->close();
            $path = "$prefix/groups/$group_id/$_.dat";
            $file = IO::File->new($path, '>')
                or die "hnco-ecdf-stat.pl: generate_ecdf_function_groups: Cannot open '$path': $!\n";
            $file->print(@lines);
            $file->printf("%d %e\n", $budget, get_value($lines[-1]));
            $file->printf("%d %e\n", 10 * $budget, $low + ($i + 0.5) * ($high - $low) / @sorted);
            $file->close();
            $i++;
        }
    }
}

sub generate_gnuplot_global_all
{
    my $path = "global-all.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-ecdf-stat.pl: generate_gnuplot_function_all: cannot open $path\n";

    # Font face and size
    my $font = "";
    if ($graphics->{all}->{font_face}) {
        $font = $graphics->{all}->{font_face};
    }
    if ($graphics->{all}->{font_size}) {
        $font = "$font,$graphics->{all}->{font_size}";
    }
    $font = qq("$font");

    my $key = qq(font $font notitle);
    if ($graphics->{all}->{key}) {
        $key = "$key $graphics->{all}->{key}";
    } else {
        # opaque vertical reverse Left outside right center box
        $key = "$key opaque vertical noreverse Right top inside left box";
    }

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 "set xlabel \"Number of evaluations\"\n",
                 "set ylabel \"Proportion of targets\"\n",
                 "unset key\n",
                 "set key $key\n",
                 "set format x ", qq("10^{%L}"), "\n",
                 "set logscale x\n",
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n");

    my $quoted_path = qq("$path_graphics/global.eps");
    $file->print("$terminal{eps} font $font\n",
                 "set output $quoted_path\n");
    my @ids = map { $_->{id} } @$algorithms;
    my $type = $graphics->{all}->{helper} ? "all" : "raw";
    $file->print("plot \\\n",
                 join ", \\\n",
                 (map { qq(  "$path_results/ecdf/global/$type/$_.dat" using 1:2 with lines title "$_"); }
                  sort_algorithms("$path_results/ecdf/global/raw", \@ids)));
    $file->print("\n");
    $quoted_path = qq("$path_graphics/global.pdf");
    $file->print("$terminal{pdf} font $font\n",
                 "set output $quoted_path\n",
                 "replot\n");
    $quoted_path = qq("$path_graphics/global.png");
    $file->print("$terminal{png} font $font\n",
                 "set output $quoted_path\n",
                 "replot\n");
    $file->close();
    system("chmod a+x global-all.gp");
}

sub generate_gnuplot_global_groups
{
    my $path = "global-groups.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-ecdf-stat.pl: generate_gnuplot_global_groups: cannot open $path\n";

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 "set xlabel \"Number of evaluations\"\n",
                 "set ylabel \"Proportion of targets\"\n",
                 "set format x ", qq("10^{%L}"), "\n",
                 "set logscale x\n",
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n");

    foreach my $group (@$groups) {
        my $group_id = $group->{id};

        # Font face and size
        my $font = "";
        if ($group->{font_face}) {
            $font = $group->{font_face};
        }
        if ($group->{font_size}) {
            $font = "$font,$group->{font_size}";
        }
        $font = qq("$font");

        my $key = qq(font $font notitle);
        if ($group->{key}) {
            $key = "$key $group->{key}";
        } else {
            # opaque vertical reverse Left outside right center box
            $key = "$key opaque vertical noreverse Right top inside left box";
        }

        $file->print("unset key\n",
                     "set key $key\n");

        my $quoted_path = qq("$path_graphics/global-$group_id.eps");
        $file->print("$terminal{eps} font $font\n",
                     "set output $quoted_path\n");
        my $type = $group->{helper} ? "groups/$group_id" : "raw";
        $file->print("plot \\\n",
                     join ", \\\n",
                     (map { qq(  "$path_results/ecdf/global/$type/$_.dat" using 1:2 with lines title "$_"); }
                      sort_algorithms("$path_results/ecdf/global/raw", $group->{algorithms})));
        $file->print("\n");
        $quoted_path = qq("$path_graphics/global-$group_id.pdf");
        $file->print("$terminal{pdf} font $font\n",
                     "set output $quoted_path\n",
                     "replot\n");
        $quoted_path = qq("$path_graphics/global-$group_id.png");
        $file->print("$terminal{png} font $font\n",
                     "set output $quoted_path\n",
                     "replot\n\n");
    }
    $file->close();
    system("chmod a+x global-groups.gp");
}

sub generate_gnuplot_function_all
{
    my $path = "function-all.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-ecdf-stat.pl: generate_gnuplot_function_all: cannot open graphics.gp\n";

    # Font face and size
    my $font = "";
    if ($graphics->{all}->{font_face}) {
        $font = $graphics->{all}->{font_face};
    }
    if ($graphics->{all}->{font_size}) {
        $font = "$font,$graphics->{all}->{font_size}";
    }
    $font = qq("$font");

    my $key = qq(font $font notitle);
    if ($graphics->{all}->{key}) {
        $key = "$key $graphics->{all}->{key}";
    } else {
        # opaque vertical reverse Left outside right center box
        $key = "$key opaque vertical noreverse Right top inside left box";
    }

    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 "set xlabel \"Number of evaluations\"\n",
                 "set ylabel \"Proportion of targets\"\n",
                 "unset key\n",
                 "set key $key\n",
                 "set format x ", qq("10^{%L}"), "\n",
                 "set logscale x\n",
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n");

    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        my $quoted = qq("$path_graphics/$function_id.eps");
        $file->print("$terminal{eps} font $font\n",
                     "set output $quoted\n");
        my @ids = map { $_->{id} } @$algorithms;
        my $type = $graphics->{all}->{helper} ? "all" : "raw";
        $file->print("plot \\\n",
                     join ", \\\n", (map { qq(  "$path_results/ecdf/$function_id/$type/$_.dat" using 1:2 with lines title "$_"); }
                                     sort_algorithms("$path_results/ecdf/$function_id/raw", \@ids)));
        $file->print("\n");
        $quoted = qq("$path_graphics/$function_id.pdf");
        $file->print("$terminal{pdf} font $font\n",
                     "set output $quoted\n",
                     "replot\n");
        $quoted = qq("$path_graphics/$function_id.png");
        $file->print("$terminal{png} font $font\n",
                     "set output $quoted\n",
                     "replot\n\n");
    }

    $file->close();
    system("chmod a+x function-all.gp");
}

sub generate_gnuplot_function_groups
{
    my $path = "function-groups.gp";
    my $file = IO::File->new($path, '>')
        or die "hnco-ecdf-stat.pl: generate_gnuplot_function_groups: cannot open graphics.gp\n";
    $file->print("#!/usr/bin/gnuplot -persist\n",
                 "set grid\n",
                 "set xlabel \"Number of evaluations\"\n",
                 "set ylabel \"Proportion of targets\"\n",
                 "set format x ", qq("10^{%L}"), "\n",
                 "set logscale x\n",
                 "set autoscale fix\n",
                 "set offsets graph 0.05, graph 0.05, graph 0.05, graph 0.05\n\n");
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        foreach my $group (@$groups) {
            my $group_id = $group->{id};

            # Font face and size
            my $font = "";
            if ($group->{font_face}) {
                $font = $group->{font_face};
            }
            if ($group->{font_size}) {
                $font = "$font,$group->{font_size}";
            }
            $font = qq("$font");

            my $key = qq(font $font notitle);
            if ($group->{key}) {
                $key = "$key $group->{key}";
            } else {
                # opaque vertical reverse Left outside right center box
                $key = "$key opaque vertical noreverse Right top inside left box";
            }

            $file->print("unset key\n",
                         "set key $key\n");

            my $quoted_path = qq("$path_graphics/$function_id-$group_id.eps");
            $file->print("$terminal{eps} font $font\n",
                         "set output $quoted_path\n");
            my $type = $group->{helper} ? "groups/$group_id" : "raw";
            $file->print("plot \\\n",
                         join ", \\\n", (map { qq(  "$path_results/ecdf/$function_id/$type/$_.dat" using 1:2 with lines title "$_"); }
                                         sort_algorithms("$path_results/ecdf/$function_id/raw", $group->{algorithms})));
            $file->print("\n");
            $quoted_path = qq("$path_graphics/$function_id-$group_id.pdf");
            $file->print("$terminal{pdf} font $font\n",
                         "set output $quoted_path\n",
                         "replot\n");
            $quoted_path = qq("$path_graphics/$function_id-$group_id.png");
            $file->print("$terminal{png} font $font\n",
                         "set output $quoted_path\n",
                         "replot\n\n");
        }
    }
    $file->close();
    system("chmod a+x function-groups.gp");
}

sub generate_latex
{
    my $path = "$path_report/results.tex";
    my $file = IO::File->new($path, '>')
        or die "hnco-ecdf-stat.pl: generate_latex: Cannot open $path\n";
    $file->print(latex_graphicspath($path_graphics),
                 latex_section("Global results"),
                 latex_subsection("All algorithms"),
                 latex_begin_center(),
                 latex_includegraphics("global"),
                 latex_end_center(),
                 latex_subsection("Groups"));
    foreach my $group (@$groups) {
        my $group_id = $group->{id};
        $file->print(latex_subsubsection("$group_id"),
                     latex_begin_center(),
                     latex_includegraphics("global-$group_id"),
                     latex_end_center());
    }
    foreach my $f (@$functions) {
        my $function_id = $f->{id};
        $file->print(latex_section("Results for $function_id"),
                     latex_subsection("All algorithms"),
                     latex_begin_center(),
                     latex_includegraphics("$function_id"),
                     latex_end_center(),
                     latex_subsection("Groups"));
        foreach my $group (@$groups) {
            my $group_id = $group->{id};
            $file->print(latex_subsubsection("$group_id"),
                         latex_begin_center(),
                         latex_includegraphics("$function_id-$group_id"),
                         latex_end_center());
        }
    }
}

#
# Helper functions
#

sub get_value
{
    my $line = shift;
    chomp $line;
    my @results = split ' ', $line;
    return $results[1];
}

sub get_time_value
{
    my $line = shift;
    chomp $line;
    return split ' ', $line;
}
