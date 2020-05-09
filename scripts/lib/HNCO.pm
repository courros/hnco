package HNCO 1.14;

use strict;
use warnings;

use File::Spec;
use File::HomeDir;
use Cwd;

use Exporter qw(import);

our @EXPORT = qw(
    gnu_parallel
    );

sub gnu_parallel
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

1;
