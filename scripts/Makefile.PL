# Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

use ExtUtils::MakeMaker;
 
WriteMakefile(
    NAME                => 'HNCO::Run',
    AUTHOR              => q{Arnaud Berny},
    VERSION_FROM        => 'lib/HNCO/Run.pm',
    NO_META             => true,
    NO_MYMETA           => true,
    EXE_FILES           => [
	'bin/hnco-autocorrelation-run.pl',
	'bin/hnco-autocorrelation-stat.pl',
	'bin/hnco-benchmark-run.pl',
	'bin/hnco-benchmark-skeleton.pl',
	'bin/hnco-benchmark-stat.pl',
	'bin/hnco-dynamics-run.pl',
	'bin/hnco-dynamics-skeleton.pl',
	'bin/hnco-dynamics-stat.pl',
	'bin/hnco-ecdf-stat.pl',
	'bin/hnco-function-parameter-stat.pl',
	'bin/hnco-lookup-ratio-stat.pl',
	'bin/hnco-maximum-stat.pl',
	'bin/hnco-parameter-run.pl',
	'bin/hnco-parameter-skeleton.pl',
	'bin/hnco-parameter-stat.pl',
	'bin/hnco-runtime-stat.pl',
	'bin/hnco-runtime2-run.pl',
	'bin/hnco-runtime2-skeleton.pl',
	'bin/hnco-runtime2-stat.pl',
	'bin/hnco-walsh-run.pl',
	'bin/hnco-walsh-skeleton.pl',
	'bin/hnco-walsh-stat.pl',
        'bin/optgen.pl'
    ]
    );
