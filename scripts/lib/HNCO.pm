package HNCO 1.14;

use strict;
use warnings;

use Exporter qw(import);

our @EXPORT = qw(read_file);

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

1;
