==========
 Building
==========

Enter the following commands in the source directory::

  ./configure
  make
  make install

To turn optimization on and install everything under ``$HOME/.local/``::

  CXXFLAGS="-Wall -O3 -DNDEBUG" ./configure --prefix=$HOME/.local/

If you want to locally install Bash completion scripts, you would
rather enter::

  bashcompdir=$HOME/.local/share/bash-completion/completions/ CXXFLAGS="-Wall -O3 -DNDEBUG" ./configure --prefix=$HOME/.local/

Some configure options are relative to hnco only:

- ``--enable-python`` to build the PythonFunction function (requires
  python3)

- ``--enable-plugin`` to build the Plugin function (requires libdl)

- ``--enable-factorization`` to build the Factorization function
  (requires libgmp-dev)

See ``./configure --help`` for other options.

To run the tests::

  make check

To generate the API documentation::

  make doc

Once generated, you will find it under ``doc/api/``.

Python bindings are located under ``bindings/``. To build and install
the python module for hnco, first build and install the library then::

  python3 -m build --wheel --no-isolation
  python3 -m pip install --user --break-system-packages --force-reinstall ./dist/hnco-0.24-cp311-cp311-linux_x86_64.whl

It should be noted that the resulting Python module is for local use
only.

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
