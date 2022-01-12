============
Requirements
============

``libhnco`` itself only depends on the Boost serialization library.
Optionally it can be built with the following libraries:

- ``libdl`` for plugin

- GNU GMP (``libgmp`` and ``libgmpxx``) for factorization

The Perl scripts depend on the following modules:

- ``JSON``

- ``Statistics::Descriptive``

They also depend on GNU parallel.

You will need gnuplot and latex to generate reports, which finally
requires the ``minted`` latex package and the executable
``pygmentize``.

Under Ubuntu 20.04 LTS, execute the following commands to install the
necessary packages::

  sudo apt-get install build-essential autoconf-archive libboost-serialization-dev
  sudo apt-get install libgmp-dev
  sudo apt-get install parallel
  sudo apt-get install libfile-slurp-perl libfile-homedir-perl libjson-perl libstatistics-descriptive-perl
  sudo apt-get install gnuplot
  sudo apt-get install texlive-base texlive-latex-extra python3-pygments
  sudo apt-get install ssh
  sudo apt-get install python3-dev pybind11-dev python3-pybind11
