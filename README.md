# In Hypercubo Nigrae Capsulae Optimum (HNCO)

## Introduction

HNCO is a C++11 library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

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

## Command-line tools

HNCO provides the command-line tools `ffgen`, `mapgen`, and `hnco`.

### ffgen

`ffgen` generates random instances of fitness functions and saves them
to files which can be later loaded by `hnco` and used by different
algorithms. HNCO uses the boost serialization library with text
format. The files are portable across different platforms.

To get the list of available options, execute the following command:

    ffgen --help

On a unix-like operating system, you can get bash auto-completion by
copying the file `ffgen.sh` under `.bash_completion.d`. Similarly with
`mapgen.sh` and `hnco.sh`.

For example, to generate a random instance of NK landscape with n =
100 and k = 4 and write it to the archive `nk.100.4`, execute the
following command:

    ffgen -F 60 -s 100 --nk-k 4 --path nk.100.4

### mapgen

`mapgen` generates random instances of maps and saves them to files. A
map is a translation, a permutation, a linear map, or an affine map in
the space of bit vectors. It can be composed with any fitness
function. Random maps can be used to check that an algorithm is
invariant under the action of those maps. `hnco` can generate random
maps but subsequent executions will get different maps. When comparing
different algorithms, it is necessary that all algorithms share the
same random map instance. Hence the need for `mapgen`.

### hnco

`hnco` allows you to apply any algorithm to any function in the
library. Its command-line interface offers a complete set of
parameters, which allows systematic experiments through scripts
written in any language. `hnco` writes results to the standard output in
text format. It is able to embed all parameters in the output so as to
exactly reproduce the simulation later, including the same seed for
random numbers.

For example, to apply (1+1) EA to the previous NK landscape instance,
execute the following command:

    hnco -A 300 -F 60 --path nk.100.4 -b 200000 --print-performances

where we have set the budget to 200000 evaluations of the fitness
function.

Other notable command-line options include:

- `--stop-on-maximum`: Stop as soon as the function maximum has been
  reached. This options requires the function to have a known maximum.
- `--log-improvement`: Track progress.
- `--cache`: Cache function evaluations (beware memory consumption!)
- `--restart`: Restart an algorithm until the budget is consumed.
- `--negation`: Maximize the negation of the function hence minimize
  it.
- `--additive-gaussian-noise`: Add Gaussian noise to function
  evaluations.

With the following command:

    hnco -A 300 -s 100000 --stop-on-maximum --print-performances -b 0

(1+1) EA found the solution to OneMax with n = 100000 after less than
80 minutes on a laptop equipped with a i5-2520M processor running
at 2.50 GHz. It required 2236703 function evaluations.

## Scripts for experimental studies

HNCO contains scripts which cover simple use cases. The scripts are
written in Perl. They are independent from each other and contain some
duplicated code. You can write more elaborate scripts to meet your
particular needs.

For each use case, there is a script for running the simulation and
another one for computing the statistics. The latter also generates
gnuplot scripts and a latex file. The gnuplot scripts generate images
in eps, pdf, and png formats.

HNCO is distributed with four experiments under `experiments`:
`benchmark`, `dynamics`, `parameter`, and `runtimes`. In each
directory, a Makefile runs the simulation and generates the
report. The simulation itself is described in a json file called
`plan.json` which is loaded by the Perl scripts.

To run the simulation, compute the statistics, and generate the
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
algorithms applied to a set of functions. Each algorithm is run
`num_runs = 20` times on each function. Algorithms are ranked
according to their median performance (other quartiles are also
considered). They are ranked first per function then globally.

- `hnco-benchmark-run.pl`: run the simulation
- `hnco-benchmark-stat.pl`: collect the results and generate the report

You will find an example of such an experiment under
`experiments/benchmark`.

### Dynamics

The purpose is to study the dynamics of some quantity such as the
maximum found so far. Each algorithm is run only once.

- `hnco-dynamics-run.pl`: run the simulation
- `hnco-dynamics-stat-maximum.pl`: collect the results and generate
  the report, for studying the maximum only
- `hnco-dynamics-stat.pl`: collect the results and generate the
  report, for studying other quantities

You will find an example of such an experiment under
`experiments/dynamics`.

### Parameter

The purpose is to study the influence of some parameter on the
performances of a set of algorithms applied to a set of functions.

- `hnco-parameter-run.pl`: run the simulation
- `hnco-parameter-stat.pl`: collect the results and generate the
  report

You will find an example of such an experiment under
`experiments/parameter`.

### Runtime

The purpose is to study the runtime as a function of some given
parameter of a set of algorithms applied to a set of functions. The
functions must have a known maximum and the algorithms must be able to
find them in finite time.

- `hnco-parameter-run.pl`: run the simulation
- `hnco-runtime-stat.pl`: collect the results and generate the report

You will find an example of such an experiment under
`experiments/runtime`.

## Library

The library is organized around two class hierarchies, one for
functions and one for algorithms. Classes are documented with
Doxygen. You might want to generate the html documentation as it is
neither version controlled nor distributed (see below). The source
files of the command-line tools in `src` provide a starting point to
learn how to use the library, in particular `make-function.cc`,
`make-algorithm.cc`, and `hnco.cc`.

### Algorithms

Here is the list of currently available algorithms:

- Complete search
- Random search
- Non strict (>=) random local search
- Strict (>) random local search
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
- Non strict (>=) max-min ant system
- Strict (>) max-min ant system
- Herding evolutionary algorithm, herding with binary variables
- Herding evolutionary algorithm, herding with spin variables
- Boltzmann machine PBIL

### Functions

Here is the list of currently available functions (those whoose
instances are generated are indicated by `ffgen`):

- OneMax
- Linear function (`ffgen`)
- Leading ones
- Ridge
- Needle
- Jump
- Deceptive jump
- Four peaks
- Six peaks
- Quadratic function (`ffgen`)
- NK landscape (`ffgen`)
- Max-SAT (`ffgen`)
- Low autocorrelation binary sequence
- Equal products (`ffgen`)
- Cancellation
- Cancellation with sinus
- Trap
- Hierarchical if and only if
- Plateau
- Long path
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
any neighborhood among:

- One bit flip
- Binomial
- Hamming ball
- Hamming sphere

Steepest ascent hill climbing can use any neighborhood iterator among:

- One bit flip iterator
- Hamming ball iterator

### Implementation

Bit vectors are implemented as `std::vector<char>`. Other options
include `std::vector<bool>` and `boost::dynamic_bitset`. We have
compared their runtime with sizes from 100 to 100k, various functions,
and two algorithms, (1+1) EA (linear time and space per iteration) and
BM PBIL (quadratic time and space per iteration). It appears that
`std::vector<char>` is always the fastest implementation. The other
two implementations are 5% to 70% slower, with the majority of
overheads between 10% and 30%. It should be noted that
`std::vector<bool>` is not always slower than `boost::dynamic_bitset`.

Apart from the fact that `std::vector<char>` uses more memory than the
other two implementations, it is clear that some algorithms could
benefit from `std::vector<bool>` or `boost::dynamic_bitset`, in
particular those intensively using bit-wise operations. In the absence
of such algorithms in the library at the moment, the decision is in
favor of `std::vector<char>`.

The library also offers basic support for linear algebra on bit
vectors. It defines the types `bit_matrix_t`, `sparse_bit_vector_t`
and `sparse_bit_matrix_t`.

### Limitations

The library only handles fixed-sized bit vectors. Size is set at
runtime.

The library code is not thread-safe. However, there is no dynamic
memory allocation with `new` in the library.

The code does not take advantage of multicore architectures or
GPUs. Evaluating a population of bit vectors could benefit from such
architectures.

The scripts do not run `hnco` in parallel although a map/reduce
strategy can clearly be applied. However, it is possible to split a
`plan.json` in two or four and run the simulation in parallel.

## Requirements

`hnco`, `ffgen`, and `mapgen` depend on the boost serialization
library.

The Perl scripts depend on the following modules:

- `JSON`
- `Statistics::Descriptive`

You also need gnuplot and latex to generate reports. In particular,
reports require the `minted` latex package which in turn require
`pygmentize`.

Under Ubuntu 16.04, execute the following commands to install the
necessary packages:
```
apt-get install build-essential libboost-serialization1.58-dev
apt-get libjson-perl libstatistics-descriptive-perl gnuplot
apt-get texlive-base texlive-latex-extra python-pygments
```

## Building and installing

For a debug version, execute the following commands in the source
directory:

    CXXFLAGS="-Wall -ggdb -std=c++11" ./configure
    make
    make install

For a production version:

    CXXFLAGS="-Wall -O3 -DNDEBUG -std=c++11" ./configure
    make
    make instal

To run the tests:

    make check

To generate the API documentation:

    make doc

Once generated, you will find it under `doc/api`.

## Contributing

Feedbacks and contributions are welcome. Contributions could take the
form of an algorithm or a function. Algorithms from published work
would be most appreciated.

## About the name

### Latin

"in Hypercubo Nigrae Capsulae Optimum" is the (naive) translation of
the english phrase "black box optimization in the hypercube".

### Chemistry

Quoting Wikipedia:

Isocyanic acid is an organic compound with the formula HNCO,
discovered in 1830 by Liebig and Wöhler. This colourless substance is
volatile and poisonous, with a boiling point of 23.5 °C. Isocyanic
acid is the simplest stable chemical compound that contains carbon,
hydrogen, nitrogen, and oxygen, the four most commonly found elements
in organic chemistry and biology.
