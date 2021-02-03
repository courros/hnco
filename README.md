# In Hypercubo Nigrae Capsulae Optimum (HNCO)

## Table of content

1. [Introduction](#introduction)
2. [Library](#library)
3. [Command-line tools](#cli)
4. [Experiments](#experiments)
5. [Requirements](#requirements)
6. [Building and installing](#building)
7. [Contributing](#contributing)
8. [About the name](#name)
9. [Acknowledgments](#ackowledgements)
10. [References](#references)

## Introduction

HNCO is a C++ library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

Library features:

- 29 concrete functions
- 25 concrete algorithms
- 78 unit tests
- Maps from bit vectors to bit vectors
- Function decorators to control evaluation
- Function decorators to modify functions
- Maps and functions can be serialized to text files
- Neighborhoods and neighborhood iterators for local search
- Population with parallel evaluation
- Support for incremental evaluation

Purposes:

- Provide a collection of standard functions
- Provide a collection of standard algorithms
- Favor reproducible research

Use cases:

- Apply algorithms in the library to practical problems
- Automate simulations
- Tune algorithms
- Compare algorithms
- Make conjectures about runtimes
- Illustrate theoretical results
- Generate graphics and tables

## Library

The library declares the top-level namespace `hnco` and nested
namespaces `hnco::algorithm`, `hnco::exception`, `hnco::function`,
`hnco::neighborhood`, and `hnco::random`.

The library is organized around the following base classes:
`Exception`, `Map`, `Function`, `Algorithm`, `Neighborhood`,
`Iterator`, `Population`.

Classes are documented with Doxygen. You might want to generate the
html documentation as it is neither version controlled nor
distributed. However the pdf documentation `doc/api/latex/refman.pdf`
is version controlled and distributed.

The source files of the command-line tools in `app/` provide a
starting point to learn how to use the library, in particular
`make-function.cc`, `make-algorithm.cc`, and `hnco.cc`.

Bit vectors are implemented as `std::vector<char>`.

The library offers basic support for linear algebra on bit vectors.

### Algorithms

Here is the list of currently available algorithms:

- Complete search
- Random search
- Random walk
- Human
- Random local search (strict and non strict)
- Steepest ascent hill climbing
- First ascent hill climbing
- Simulated annealing
- (1+1) evolutionary algorithm
- (mu+lambda) evolutionary algorithm
- (mu,lambda) evolutionary algorithm
- Genetic algorithm
- (1+(lambda,lambda)) genetic algorithm
- Population-based incremental learning
- PBIL with positive and negative selection
- Univariate marginal distribution algorithm
- Compact genetic algorithm
- Max-min ant system (strict and non strict)
- Herding evolutionary algorithm, herding with binary variables
- Herding evolutionary algorithm, herding with spin variables
- Boltzmann machine PBIL
- Mutual information maximizing input clustering
- Hierarchical Bayesian Optimization Algorithm
- Linkage Tree Genetic Algorithm
- Parameter-less Population Pyramid

For population-based algorithms, bit vectors in a population (class
`hnco::algorithm::Population`) can be evaluated in parallel as the
library uses OpenMP. It should be noted that only sufficiently complex
functions should lead to significant speed-up.

### Functions

Here is the list of currently available functions (`ffgen` means that
instances can be generated by `ffgen`).

- OneMax
- Linear function (`ffgen`)
- BinVal
- Leading ones
- Ridge
- Needle
- Jump
- Deceptive jump
- Four peaks
- Six peaks
- Quadratic unconstrained binary optimization (qubo input file format)
- Nk landscape (`ffgen`)
- Max-SAT (`ffgen`, dimacs cnf input file format)
- Max not-all-equal 3SAT (`ffgen`, dimacs cnf input file format)
- Low autocorrelation binary sequence
- Low autocorrelation binary sequence merit factor
- Equal products (`ffgen`)
- Partition (`ffgen`)
- Cancellation
- Cancellation with sinus
- Trap
- Hierarchical if and only if
- Plateau
- Long path
- Factorization
- Walsh expansion (`ffgen`)
- Walsh expansion of degree 1 (`ffgen`)
- Walsh expansion of degree 2 (`ffgen`)
- Nearest neighbor Ising model in one dimension (`ffgen`)
- Nearest neighbor Ising model in two dimensions (`ffgen`)
- Real multivariate function
- Integer multivariate function
- Complex multivariate function (squared magnitude of)
- Sudoku
- Plugin (load any C function)

### Function decorators

Available function controllers: Cache, CallCounter, OnBudgetFunction,
ProgressTracker, StopOnMaximum, StopOnTarget.

Available function modifiers: AdditiveGaussianNoise,
FunctionMapComposition, Negation, PriorNoise, ParsedModifier.

### Neighborhoods

Random local search algorithms, including simulated algorithm, can use
the following neighborhoods:

- Single bit flip
- Bernoulli process
- Hamming ball
- Hamming sphere

Every neighborhood can act as a mutation operator.

Steepest ascent hill climbing can use the following neighborhood
iterators:

- Single bit flip iterator
- Hamming sphere iterator

### Limitations

The library only handles fixed-sized bit vectors.

The library should include more algorithms, in particular Tabu search
algorithms and recent EDAs (Estimation of Distribution
Algorithms). These are considered for future releases.

## Command-line tools <a name="cli"></a>

HNCO provides the command-line tools `ffgen`, `hnco`, and `mapgen`.

### ffgen

`ffgen` generates random instances of fitness functions and saves them
to files which can be later loaded by `hnco` and used by different
algorithms. HNCO uses the Boost serialization library with text
format. The files are portable across different platforms. However
they can only be read by programs compiled with the same (or newer)
version of the library.

To get the list of available options, execute the following command:

    ffgen --help

On a unix-like operating system, you can get bash auto-completion by
copying the file `ffgen.sh` under `.bash_completion.d/`. Similarly
with `mapgen.sh` and `hnco.sh`.

For example, let us generate a random instance of Nk landscape and
write it to a file:

    ffgen -F 60 -s 100 --nk-k 4 --path nk.txt

Meaning of the options:

- `-F 60` sets the function type, here 60 represents Nk landscape
- `--path nk.txt` sets the path to the file containing the function
- `-s 100` sets the bit vector size to 100
- `--nk-k 4` sets the parameter k to 4

### hnco

`hnco` allows you to apply any algorithm to any function in the
library. Its command-line interface offers a complete set of
parameters, which allows systematic experiments through scripts
written in any language. `hnco` writes results to the standard output in
text format. It is able to embed all parameters in the output so as to
exactly reproduce the simulation later, including the same seed for
random numbers.

For example, to apply (1+1) EA to the previous Nk landscape instance,
execute the following command:

    hnco -A 300 -F 60 --path nk.txt -b 200000 --print-results

Meaning of the options:

- `-A 300` sets the algorithm type, here 300 represents (1+1) EA
- `-F 60` sets the function type, here 60 represents Nk landscape
- `--path nk.txt` sets the path to the file containing the function
- `-b 200000` sets the budget to 200000 evaluations
- `--print-results` prints in JSON the maximum, the number of
  evaluations needed to reach it, the total time (clock time), and the
  time spent in evaluating solutions.

As another example, let us apply (1+1) EA to OneMax:

    hnco -A 300 -s 100000 --stop-on-maximum --log-improvement -b 0

Meaning of the options:

- `-s 100000` sets the bit vector size to 100000
- `--stop-on-maximum` tells `hnco` to stop as soon as it has found the
  maximum
- `--log-improvement`: track progress
- `-b 0` means no limit for the budget

### mapgen

`mapgen` generates random maps and saves them to files. A map can be a
translation, a permutation, the composition of a permutation and a
translation, a linear map, an affine map, or and affine map whose
linear map is given by a sequence of transvections. A map file can be
loaded by `hnco` and used by different algorithms.

## Experiments

HNCO is distributed with the following experiments:

- `experiments/examples/algorithm-parameter/`
- `experiments/examples/autocorrelation/`
- `experiments/examples/benchmark/`
- `experiments/examples/ecdf/`
- `experiments/examples/fixed-target/`
- `experiments/examples/function-parameter/`
- `experiments/examples/lookup-ratio/`
- `experiments/examples/maximum-evolution/`
- `experiments/examples/observable-evolution/`
- `experiments/examples/runtime/`
- `experiments/examples/walsh-transforms/`

In each directory, a Makefile runs the simulations and generates the
report. The experiment itself is described in a json file called
`plan.json`.

All experiments can use GNU parallel to run the simulations in
parallel hence take advantage of multicore architectures. To use GNU
parallel, simply set the field `parallel` to `true`.

There is also limited support for remote execution. A list of remote
hosts can be specified in `plan.json` under the name `servers`. For
each server, a hostname (or ip address) must be given. The relative
working directories must be the same on each server. GNU parallel
connects to servers with ssh.

### Algorithm parameter

This experiment is similar to the benchmark experiment but the same
algorithm is run with some parameter taking values in a given set. In
the example, the influence of the learning rate on the performance of
PBIL is studied.

### Autocorrelation

The purpose of this experiment is to visualize empirical
autocorrelation functions of time series generated by random walks on
the hypercube and various fitness functions.

### Benchmark

The purpose of this experiment is to compare the performance of a set
of algorithms applied to a set of functions with a fixed budget. Each
algorithm is run 20 times on each function. Algorithms are ranked
according to their median performance (quartiles are also
considered). They are ranked first per function then globally.

### ECDF

The purpose of this experiment is to compute and plot the empirical
cumulative distribution function of the runtime for each
algorithm-function couple in a given set. Results are computed per
function then globally. This experiment partly follows the
experimental procedure of the COCO framework (see references).

### Function parameter

The purpose of this experiment is to study the influence of a function
parameter on the performance of search algorithms.

### Lookup ratio

The purpose of this experiment is to compare the cache lookup ratio of
a set of algorithms applied to a set of functions with a fixed
budget. It is designed after the benchmark experiment. A high lookup
ratio indicates that an algorithm often resamples already sampled bit
vectors.

### Maximum evolution

The purpose of this experiment is to study the evolution of the
maximum found so far. Each algorithm is run only once on each
function. Time is expressed in terms of number of evaluations.

### Observable evolution

The purpose of this experiment is to study the evolution of some
quantity available through the log function of a given set of
algorithms. In the example, the evolution of entropy in UMDA and PBIL
is studied. Each algorithm is run only once on each function. Time is
expressed in terms of number of iterations.

### Runtime

The purpose of this experiment is to study the influence of some given
parameter (e.g. bit vector size) on the runtime of a set of algorithms
applied to a set of functions. The functions must have a known maximum
and the algorithms must be able to find it in finite time.

### Walsh transforms

The purpose of this experiment is to visualize the Walsh transforms of
various functions in the library.

## Requirements

`libhnco` itself only depends on the Boost serialization
library. Optionally it can be built with the following libraries:

- `libdl` for plugin
- GNU GMP (libgmp and libgmpxx) for factorization

The Perl scripts depend on the following modules:

- `JSON`
- `Statistics::Descriptive`

They also depend on GNU parallel.

You will need gnuplot and latex to generate reports, which finally
requires the `minted` latex package and the executable `pygmentize`.

Under Ubuntu 18.04, execute the following commands to install the
necessary packages:
```
sudo apt-get install build-essential autoconf-archive libboost-serialization-dev
sudo apt-get install libgmp-dev
sudo apt-get install parallel
sudo apt-get install libfile-slurp-perl libfile-homedir-perl libjson-perl libstatistics-descriptive-perl
sudo apt-get install gnuplot
sudo apt-get install texlive-base texlive-latex-extra python-pygments python3-pygments
sudo apt-get install ssh
```

## Building and installing <a name="building"></a>

The library and applications require a C++14 compiler.

Execute the following commands in the source directory:

    ./configure
    make
    make install

To turn optimization on and install everything under `$HOME/.local/`:

    CXXFLAGS="-Wall -O3 -DNDEBUG" ./configure --prefix=$HOME/.local/

The factorization function is disabled by default. To enable it, add
the option `--enable-factorization` to `./configure`. Similarly add
the option `--enable-plugin` to enable the plugin function. See
`./configure --help` for other options.

To run the tests:

    make check

To generate the API documentation:

    make doc

Once generated, you will find it under `doc/api/`.

Installation of Perl scripts is taken care of by autotools. However,
Perl modules are managed separately by `Makefile.PL`. If you want to
install them under `$HOME/.local/`, do the following:

    cd experiments/modules
    perl Makefile.PL INSTALL_BASE=$HOME/.local/
    make
    make install

Modules will be installed under `$HOME/.local/lib/perl5/`. If you want
perl to find them, add this line to the appropriate startup file of
your shell:

    export PERLLIB="$HOME/.local/lib/perl5"

To verify that the path has been added to the search list:

    perl -E 'say for @INC'

## Contributing

Feedbacks and contributions are welcome. Contributions could take the
form of an algorithm or a function. Algorithms from published work
would be most appreciated.

## About the name <a name="name"></a>

### Latin

"in Hypercubo Nigrae Capsulae Optimum" is the (naive) translation of
the english phrase "black box optimization in the hypercube".

### Chemistry

Quoting Wikipedia:

>Isocyanic acid is an organic compound with the formula HNCO,
discovered in 1830 by Liebig and Wöhler. This colourless substance is
volatile and poisonous, with a boiling point of 23.5 °C. Isocyanic
acid is the simplest stable chemical compound that contains carbon,
hydrogen, nitrogen, and oxygen, the four most commonly found elements
in organic chemistry and biology.

## Ackowledgements

Maxim Buzdalov has drawn my attention to the interest of incremental
evaluation.

Dirk Sudholt has pointed the discrepancy between theoretical and
experimental optimization times of (1+1) EA on OneMax, which has led
to the addition of the option `--allow-no-mutation` to allow to stay
on the same bit vector when no mutation occurs.

The library embeds code from Brian W. Goldman's project
FastEfficientP3 to provide Hboa, Ltga, and P3 algorithms.

The library is distributed with fparser. Quoting the preface of
fparser:

>This C++ library offers a class which can be used to parse and
>evaluate a mathematical function from a string (which might be
>eg. requested from the user). The syntax of the function string is
>similar to mathematical expressions written in C/C++ (the exact
>syntax is specified later in this document). The function can then be
>evaluated with different values of variables.

## References

- Berny A. (2017). "In Hypercubo Nigrae Capsulae Optimum". In:
  Proceedings of the Genetic and Evolutionary Computation Conference
  Companion, GECCO '17,
  [doi:10.1145/3067695.3082472](http://dx.doi.org/10.1145/3067695.3082472)
- O. Tange (2011): GNU Parallel - The Command-Line Power Tool, ;login:
  The USENIX Magazine, February 2011:42-47.
- Nikolaus Hansen, Anne Auger, Dimo Brockhoff, Dejan Tusar, Tea
  Tusar (2016). "COCO: Performance Assessment",
  [abs/1605.03560](http://arxiv.org/abs/1605.03560).
- Brian W. Goldman,
  [FastEfficientP3](https://github.com/brianwgoldman/FastEfficientP3)
- Juha Nieminen, Joel Yliluoma,
  [fparser](http://warp.povusers.org/FunctionParser/fparser.html)
