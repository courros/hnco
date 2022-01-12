==========
 Building
==========


The library and applications require a C++14 compiler.

Execute the following commands in the source directory::

  ./configure
  make
  make install

To turn optimization on and install everything under ``$HOME/.local/``::

  CXXFLAGS="-Wall -O3 -DNDEBUG" ./configure --prefix=$HOME/.local/

If you want to locally install Bash completion scripts, you would
rather execute::

  bashcompdir=$HOME/.local/share/bash-completion/completions/ CXXFLAGS="-Wall -O3 -DNDEBUG" ./configure --prefix=$HOME/.local/

Some configure options are relative to hnco only:

- ``--enable-factorization`` to build the Factorization function
  (requires libgmp-dev)

- ``--enable-plugin`` to build the Plugin function (requires libdl)

- ``--enable-python`` to build the PythonFunction function (requires
  python3)

See ``./configure --help`` for other options.

To run the tests::

  make check

To generate the API documentation::

  make doc

Once generated, you will find it under ``doc/api/``.

Python bindings are located under ``bindings/``. To build and install
the python module for hnco, first build and install the library then::

  cd bindings
  python3 setup.py build
  python3 setup.py install --prefix '~/.local'

Installation of Perl scripts is taken care of by autotools. However,
Perl modules are managed separately by ``Makefile.PL``. If you want to
install them under ``$HOME/.local/``, do the following::

  cd experiments/modules
  perl Makefile.PL INSTALL_BASE=$HOME/.local/
  make
  make install

Modules will be installed under ``$HOME/.local/lib/perl5/``. If you
want perl to find them, add this line to the appropriate startup file
of your shell::

  export PERLLIB="$HOME/.local/lib/perl5"

To verify that the path has been added to the search list::

  perl -E 'say for @INC'
