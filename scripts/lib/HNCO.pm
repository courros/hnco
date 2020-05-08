package HNCO 1.14;

use strict;
use warnings;

use File::Spec;
use File::HomeDir;
use Cwd;

use Exporter qw(import);

our @EXPORT = qw(
    read_file
    gnu_parallel
    add_missing_names
    );

sub read_file
{
    my $path = shift;
    my $content;
    {
        local $/;
        open my $fh, '<', $path
            or die "HNCO::read_file: Cannot open '$path': $!\n";
        $content = <$fh>;
        close $fh;
    }
    return $content;
}

sub write_commands()
{
    my ($comaands) = @_;

    my $path = 'commands.txt';
    my $file = IO::File->new($path, '>')
        or die "HNCO::gnu_parallel: Cannot open '$path': $!\n";
    $file->print(join("\n", @$commands));
    $file->close;
}

sub gnu_parallel()
{
    my ($servers, $path_results, $skeleton) = @_;

    if ($servers) {
        my $dir = File::Spec->abs2rel(getcwd, File::HomeDir->my_home);
        foreach (@$servers) {
            system(qq(ssh $_->{hostname} "cd $dir ; $skeleton"\n));
        }
        my $hostnames = join(',', map { $_->{hostname} } @$servers);
        system("parallel --joblog log.parallel --eta --progress --workdir . -S :,$hostnames :::: commands.txt");
        print "Bringing back the files:\n";
        foreach (@$servers) {
            system("rsync -avvz $_->{hostname}:$dir/$path_results/ $path_results");
        }
    } else {
        system("parallel --joblog log.parallel --eta --progress :::: commands.txt");
    }
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
