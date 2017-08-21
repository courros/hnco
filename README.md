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
9. [Acknowledgment](#ackowledgement)
10. [References](#references)

## Introduction

HNCO is a C++11 library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

Library features:

- 25 concrete functions
- 17 concrete algorithms
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
distributed.

The source files of the command-line tools in `src` provide a starting
point to learn how to use the library, in particular
`make-function.cc`, `make-algorithm.cc`, and `hnco.cc`.

Bit vectors are implemented as `std::vector<char>`.

The library offers basic support for linear algebra on bit vectors.

### Algorithms

Here is the list of currently available algorithms:

- Complete search
- Random search
- Random local search (strict and non strict)
- Steepest ascent hill climbing
- Simulated annealing
- (1+1) evolutionary algorithm
- (mu+lambda) evolutionary algorithm
- (mu,lambda) evolutionary algorithm
- Genetic algorithm
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
- Plugin

The plugin function can load any C function declared as

    double bar(const char[], size_t)

Suppose `bar` is defined in `bar.c` which is part of `libfoo`. Then,
to use it with `hnco`, you would do:

    gcc -fPIC -c bar.c
    gcc -shared -Wl,-soname,libfoo.so -o libfoo.so bar.o
    hnco -F 1000 --path ./libfoo.so --plugin-function-name bar

### Neighborhoods

Random local search algorithms, including simulated algorithm, can use
the following neighborhoods:

- One bit flip
- Bernoulli process
- Hamming ball
- Hamming sphere

Steepest ascent hill climbing can use the following neighborhood
iterators:

- One bit flip iterator
- Hamming ball iterator

### Limitations

The library only handles fixed-sized bit vectors.

The scripts currently do not run `hnco` in parallel. However, it is
possible to split a `plan.json` in two or four and run the simulations
in parallel. We might use GNU Parallel in future releases.

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

    hnco -A 300 -s 100000 --stop-on-maximum --print-performance -b 0

Meaning of the options:

- `-s 100000` sets the bit vector size to 100000
- `--stop-on-maximum` tells `hnco` to stop as soon as it has found the
  maximum
- `-b 0` means no limit for the budget

Other notable command-line options include:

- `--seed`: seed for random number generator
- `--log-improvement`: track progress
- `--map`: type of map
- `--map-random`: sample a random map
- `--restart`: restart an algorithm until the budget is consumed
- `--target`: set a target
- `--stop-on-target`: stop as soon as the target has been reached
- `--num-threads`: number of threads
- `--cache`: cache function evaluations (beware memory consumption!)

### mapgen

`mapgen` generates random instances of maps and saves them to
files. The map can be a translation, a permutation, a linear map, or
an affine map in the space of bit vectors. The same map instance can
be loaded by `hnco` and used by different algorithms.

## Experiments

HNCO is distributed with four experiments under `experiments/`:
`benchmark`, `dynamics`, `parameter`, and `runtimes`. In each
directory, a Makefile runs the simulations and generates the
report. The experiment itself is described in a json file called
`plan.json`.

To run the simulations, compute the statistics, and generate the
report, execute the following command:

    make

To compute the statistics and generate the report, execute the
following command:

    make doc

If you are only interested in compiling the latex document, execute
the following commands:

    cd report
    make doc

### Benchmark

The purpose is to study the relative performances of a set of
algorithms applied to a set of functions. Each algorithm is run 20
times on each function. Algorithms are ranked according to their
median performance (quartiles are also considered). They are ranked
first per function then globally.

- `hnco-benchmark-run.pl`: run the simulations
- `hnco-benchmark-stat.pl`: collect the results and generate the report

You will find an example of such an experiment under
`experiments/benchmark/`.

### Dynamics

The purpose is to study the dynamics of some quantity such as the
maximum found so far. Each algorithm is run only once.

- `hnco-dynamics-run.pl`: run the simulations
- `hnco-dynamics-stat-maximum.pl`: collect the results and generate
  the report, for studying the maximum only
- `hnco-dynamics-stat.pl`: collect the results and generate the
  report, for studying other quantities

You will find an example of such an experiment under
`experiments/dynamics/`.

### Parameter

The purpose is to study the influence of some parameter on the
performances of a set of algorithms applied to a set of functions.

- `hnco-parameter-run.pl`: run the simulations
- `hnco-parameter-stat.pl`: collect the results and generate the
  report

You will find an example of such an experiment under
`experiments/parameter/`.

### Runtime

The purpose is to study the runtime as a function of some given
parameter of a set of algorithms applied to a set of functions. The
functions must have a known maximum and the algorithms must be able to
find it in finite time.

- `hnco-parameter-run.pl`: run the simulations
- `hnco-runtime-stat.pl`: collect the results and generate the report

You will find an example of such an experiment under
`experiments/runtime/`.

## Requirements

`hnco`, `ffgen`, and `mapgen` depend on the following libraries:

- Boost serialization library
- libdl for plugin
- GNU GMP (libgmp and libgmpxx) for factorization

The Perl scripts depend on the following modules:

- `JSON`
- `Statistics::Descriptive`

You also need gnuplot and latex to generate reports. In particular,
reports require the `minted` latex package which in turn require
`pygmentize`.

Under Ubuntu 16.04, execute the following commands to install the
necessary packages:
```
sudo apt-get install build-essential libboost-serialization1.58-dev
sudo apt-get install libgmp-dev
sudo apt-get install libjson-perl libstatistics-descriptive-perl gnuplot
sudo apt-get install texlive-base texlive-latex-extra python-pygments
```

Under Ubuntu 16.10, replace `libboost-serialization1.58-dev` with
`libboost-serialization1.61-dev`.

## Building and installing <a name="building"></a>

Execute the following commands in the source directory:

    ./configure
    make
    make install

The factorization function is disabled by default. To enable it, use
`./configure --enable-factorization` instead of `./configure`. See
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

## Ackowledgement

I would like to thank Maxim Buzdalov for having drawn my attention to
the interest of incremental evaluation.

## References

- Berny A. (2017). "In Hypercubo Nigrae Capsulae Optimum". In:
  Proceedings of the Genetic and Evolutionary Computation Conference
  Companion, GECCO
  '17,
  [doi:10.1145/3067695.3082472](http://dx.doi.org/10.1145/3067695.3082472)
