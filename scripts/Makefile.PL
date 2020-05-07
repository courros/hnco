use ExtUtils::MakeMaker;
 
WriteMakefile(
    NAME                => 'HNCO',
    VERSION_FROM        => 'lib/HNCO.pm',
    NO_META             => true,
    NO_MYMETA           => true,
    EXE_FILES           => [
	'scripts/hnco-autocorrelation-run.pl',
	'scripts/hnco-autocorrelation-stat.pl',
	'scripts/hnco-benchmark-run.pl',
	'scripts/hnco-benchmark-skeleton.pl',
	'scripts/hnco-benchmark-stat.pl',
	'scripts/hnco-dynamics-run.pl',
	'scripts/hnco-dynamics-skeleton.pl',
	'scripts/hnco-dynamics-stat.pl',
	'scripts/hnco-ecdf-stat.pl',
	'scripts/hnco-function-parameter-stat.pl',
	'scripts/hnco-lookup-ratio-stat.pl',
	'scripts/hnco-maximum-stat.pl',
	'scripts/hnco-parameter-run.pl',
	'scripts/hnco-parameter-skeleton.pl',
	'scripts/hnco-parameter-stat.pl',
	'scripts/hnco-runtime-stat.pl',
	'scripts/hnco-runtime2-run.pl',
	'scripts/hnco-runtime2-skeleton.pl',
	'scripts/hnco-runtime2-stat.pl',
	'scripts/hnco-walsh-run.pl',
	'scripts/hnco-walsh-skeleton.pl',
	'scripts/hnco-walsh-stat.pl',
        'scripts/optgen.pl'
    ]
    );
