==========
 Building
==========

Enter the following commands in the source directory::

  ./configure
  make
  make install

To turn optimization on and install all executables in
``$HOME/.local/``, instead of ``./configure``, enter the command::

  CXXFLAGS="-Wall -O3 -DNDEBUG" ./configure --prefix=$HOME/.local/

Additionally, if you also want to install Bash completion scripts in
``$HOME/.local/``, instead of ``./configure``, enter the command::

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

Once generated, you will find it in ``doc/api/``.

The source file for the Python bindings is located in ``bindings/``.
To build and install the Python module for hnco, enter the commands::

  python3 -m build --wheel --no-isolation
  python3 -m pip install --user --break-system-packages --force-reinstall ./dist/hnco-0.24-cp311-cp311-linux_x86_64.whl

To ensure that Python will find the module, add this line to the
appropriate startup file of your shell::

  export PYTHONPATH=$HOME/.local/lib/python3.11/site-packages

Installation of Perl scripts is taken care of by the Makefile.
However, Perl modules are managed separately by ``Makefile.PL``. If
you want to install them in ``$HOME/.local/``, do the following::

  cd experiments/modules
  perl Makefile.PL INSTALL_BASE=$HOME/.local/
  make
  make install

Modules will be installed in ``$HOME/.local/lib/perl5/``. If you want
perl to find them, add this line to the appropriate startup file of
your shell::

  export PERLLIB="$HOME/.local/lib/perl5"

To verify that the path has been added to the search list::

  perl -E 'say for @INC'
