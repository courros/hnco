package HNCO 1.14;

use strict;
use warnings;

use Exporter qw(import);

our @EXPORT = qw(
    read_file
    add_missing_names
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

1;
