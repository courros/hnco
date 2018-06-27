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

HNCO is a C++11 library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

Library features:

- 25 concrete functions
- 18 concrete algorithms
- Maps from bit vectors to bit vectors (affine maps)
- Function decorators to control evaluation (budget)
- Function decorators to modify functions (noise)
- Maps and functions can be serialized to text files
- Neighborhoods and neighborhood iterators for local search
- Population with parallel evaluation (OpenMP)
- Support for incremental evaluation (OneMax and RLS)
- Uses a unique instance of random number generator `std::mt19937`
- 37 unit tests

Purposes:

- Provide a collection of standard functions
- Provide a collection of standard algorithms
- Favor reproducible research
- Provide an implementation of the author's algorithms (BM-PBIL, HEA)

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
distributed. However the pdf documentation can be found under
`doc/api/latex/refman.pdf`.

The source files of the command-line tools in `src/` provide a
starting point to learn how to use the library, in particular
`make-function.cc`, `make-algorithm.cc`, and `hnco.cc`.

Bit vectors are implemented as `std::vector<char>`.

The library offers basic support for linear algebra on bit vectors.

### Algorithms

Here is the list of currently available algorithms:

- Complete search
- Random search
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

For population-based algorithms, bit vectors in the population can be
evaluated in parallel as the library uses OpenMP. It should be noted
that only sufficiently complex functions should lead to significant
speed-up.

### Functions

Here is the list of currently available functions (`ffgen` means that
instances can be generated by `ffgen`).

- OneMax
- Linear function (`ffgen`)
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
- Low autocorrelation binary sequence
- Equal products (`ffgen`)
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
- Plugin (load any C function)

### Function decorators

Available function controllers: Cache, CallCounter, OnBudgetFunction,
ProgressTracker, StopOnMaximum, StopOnTarget.

Available function modifiers: AdditiveGaussianNoise,
FunctionMapComposition, Negation, PriorNoise.

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
- Hamming ball iterator

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
format. The files are portable across different platforms.

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

    hnco -A 300 -F 60 --path nk.txt -b 200000 --print-performance

Meaning of the options:

- `-A 300` sets the algorithm type, here 300 represents (1+1) EA
- `-F 60` sets the function type, here 60 represents Nk landscape
- `--path nk.txt` sets the path to the file containing the function
- `-b 200000` sets the budget to 200000 evaluations
- `--print-performance` tells `hnco` to print performance (maximum and
  number of evaluations needed to reach it) at the end

As another example, let us apply (1+1) EA to OneMax:

    hnco -A 300 -s 100000 --stop-on-maximum --log-improvement -b 0

Meaning of the options:

- `-s 100000` sets the bit vector size to 100000
- `--stop-on-maximum` tells `hnco` to stop as soon as it has found the
  maximum
- `--log-improvement`: track progress
- `-b 0` means no limit for the budget

### mapgen

`mapgen` generates random instances of maps and saves them to
files. The map can be a translation, a permutation, a linear map, or
an affine map in the space of bit vectors. The same map instance can
be loaded by `hnco` and used by different algorithms.

## Experiments

HNCO is distributed with the following experiments:

- `experiments/benchmark/`
- `experiments/dynamics/`
- `experiments/ecdf/`
- `experiments/maximum/`
- `experiments/parameter/`
- `experiments/runtime/`
- `experiments/walsh-transforms/`

In each directory, a Makefile runs the simulations and generates the
report. The experiment itself is described in a json file called
`plan.json`.

All experiments can use GNU parallel to run the simulations in
parallel hence take advantage of multicore architectures. To use GNU
parallel, simply set the field `parallel` to `true`.

There is also experimental support for remote execution at the moment
limited to the following experiments: benchmark, maximum, dynamics. A
list of remote hosts can be specified in `plan.json` under the name
`servers`. For each server, a hostname (or ip address) and a username
must be given. The path to the respective working directories must be
the same. GNU parallel connects to servers with ssh and ssh login must
not require a password.

### Benchmark

The purpose of this experiment is to compare the performance of a set
of algorithms applied to a set of functions with a fixed budget. Each
algorithm is run 20 times on each function. Algorithms are ranked
according to their median performance (quartiles are also
considered). They are ranked first per function then globally.

### Dynamics

The purpose of this experiment is to study the evolution of some
quantity available through the log function of a given set of
algorithms. In the example, the evolution of entropy in UMDA and PBIL
is studied. Each algorithm is run only once on each function. Time is
expressed in terms of number of iterations.

### ECDF

The purpose of this experiment is to compute and plot the empirical
cumulative distribution function of the runtime for each
algorithm-function couple in a given set. Results are computed per
function then globally. This experiment partly follows the
experimental procedure of the COCO framework (see references).

### Maximum

The purpose of this experiment is to study the evolution of the
maximum found so far. Each algorithm is run only once on each
function. Time is expressed in terms of number of evaluations.

### Parameter

This experiment is similar to the benchmark experiment but the same
algorithm is run with some parameter taking values in a given set. In
the example, the influence of the learning rate on the performance of
PBIL is studied.

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

They also depend on the following executables:

- `/usr/bin/time`
- GNU parallel

You will need gnuplot and latex to generate reports. Moreover reports
require the `minted` latex package which in turn requires
`pygmentize`.

Under Ubuntu 18.04, execute the following commands to install the
necessary packages:
```
sudo apt-get install build-essential libboost-serialization-dev
sudo apt-get install libgmp-dev
sudo apt-get install libjson-perl libstatistics-descriptive-perl gnuplot
sudo apt-get install texlive-base texlive-latex-extra python-pygments
sudo apt-get install parallel
```

## Building and installing <a name="building"></a>

Execute the following commands in the source directory:

    ./configure
    make
    make install

The factorization function is disabled by default. To enable it, add
the option `--enable-factorization` to `./configure`. Similarly add
the option `--enable-plugin` to enable the plugin function. See
`./configure --help` for other options.

To run the tests:

    make check

To generate the API documentation:

    make doc

Once generated, you will find it under `doc/api/`.

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
to the addition of the option `--allow-stay` to allow to stay on the
same bit vector when no mutation occurs.

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
