package HNCO 1.14;

use strict;
use warnings;

use Exporter qw(import);

our @EXPORT = qw(
    read_file
    add_missing_names
    latex_graphicspath
    latex_section
    latex_begin_center
    latex_end_center
    latex_begin_figure
    latex_includegraphics
    latex_caption
    latex_end_figure
    latex_newpage
    );

sub read_file
{
    my $path = shift;
    my $content;
    {
        local $/;
        open my $fh, '<', $path or die "HNCO::read_file: Cannot open '$path': $!\n";
        $content = <$fh>;
        close $fh;
    }
    return $content;
}

sub add_missing_names
{
    my $list = shift;
    foreach my $item (@$list) {
        if (!exists($item->{name})) {
            $item->{name} = $item->{id};
        }
    }
}

#
# Latex
#

sub latex_graphicspath
{
    my ($path) = @_;
    return "\\graphicspath{{../$path/}}\n";
}

sub latex_section
{
    my ($title) = @_;
    return "\\section{$title}\n";
}

sub latex_begin_center
{
    return "\\begin{center}\n";
}

sub latex_end_center
{
    return "\\end{center}\n";
}

sub latex_begin_figure
{
    return
        "\\begin{figure}[h]\n" .
        "\\centering\n";
}

sub latex_includegraphics
{
    my ($path) = @_;
    return "\\includegraphics[width=0.6\\linewidth]{$path}\n";
}

sub latex_caption
{
    my ($caption) = @_;
    return "\\caption{$caption}\n";
}

sub latex_end_figure
{
    return "\\end{figure}\n";
}

sub latex_newpage
{
    return "\\newpage\n";
}

1;
