============
Requirements
============

The library and applications require a C++17 compiler.

``libhnco`` itself only depends on the Boost serialization library.
Optionally it can be built with the following libraries:

- ``libdl`` for plugin

- GNU GMP (``libgmp`` and ``libgmpxx``) for factorization

The Perl scripts running the experiments depend on the following
modules:

- ``JSON``

- ``Statistics::Descriptive``

- ``File::Slurp``

- ``File::HomeDir``

They also execute GNU parallel in case of distributed experiments. The
results of such experiments are collected with ssh.

You will need gnuplot and latex to generate reports, which finally
requires the ``minted`` latex package and a syntax highlighter called
``pygmentize``.

The library pybind11 is used to create Python bindings of ``libhnco``.

Under Ubuntu 22.04 LTS and later, enter the following commands to
install the necessary packages::

  sudo apt-get install build-essential libboost-serialization-dev
  sudo apt-get install libgmp-dev
  sudo apt-get install parallel
  sudo apt-get install libfile-slurp-perl libfile-homedir-perl libjson-perl libstatistics-descriptive-perl
  sudo apt-get install gnuplot
  sudo apt-get install texlive-base texlive-latex-extra python3-pygments
  sudo apt-get install ssh
  sudo apt-get install python3-dev pybind11-dev python3-pybind11
