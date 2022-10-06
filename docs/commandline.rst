====================
 Command-line tools
====================

HNCO provides the command-line tools ``hnco``, ``ffgen``, and
``mapgen``.

----
hnco
----

``hnco`` allows you to apply any algorithm to any function in the
library. Its command-line interface offers a complete set of
parameters, which allows systematic experiments through scripts
written in any language. ``hnco`` writes results to the standard
output in text format. It is able to embed all parameters in the
output so as to exactly reproduce the simulation later, including the
same seed for random numbers.

For example, to apply (1+1) EA to the previous Nk landscape instance,
execute the following command::

  hnco -A 300 -F 60 --path nk.txt -b 200000 --print-results

Meaning of the options:

- ``-A 300`` sets the algorithm type, here 300 represents (1+1) EA

- ``-F 60`` sets the function type, here 60 represents Nk landscape

- ``--path nk.txt`` sets the path to the file containing the function

- ``-b 200000`` sets the budget to 200000 evaluations

- ``--print-results`` prints in JSON the maximum, the number of
  evaluations needed to reach it, the total time (clock time), and the
  time spent in evaluating solutions.

As another example, let us apply (1+1) EA to OneMax::

  hnco -A 300 -s 100000 --stop-on-maximum --log-improvement -b 0

Meaning of the options:

- ``-s 100000`` sets the bit vector size to 100000

- ``--stop-on-maximum`` tells ``hnco`` to stop as soon as it has found
  the maximum

- ``--log-improvement``: track progress

- ``-b 0`` means no limit for the budget

-----
ffgen
-----

``ffgen`` generates random instances of fitness functions and saves
them to files which can be later loaded by ``hnco`` and used by
different algorithms. HNCO uses the Boost serialization library with
text format. The files are portable across different platforms.
However they can only be read by programs compiled with the same (or
newer) version of the library.

To get the list of available options, execute the following command::

  ffgen --help

On a unix-like operating system, you can get bash auto-completion by
copying the file `ffgen.sh` under `.bash_completion.d/`. Similarly
with `mapgen.sh` and `hnco.sh`.

For example, let us generate a random instance of Nk landscape and
write it to a file::

  ffgen -F 60 -s 100 --nk-k 4 --path nk.txt

Meaning of the options:

- ``-F 60`` sets the function type, here 60 represents Nk landscape

- ``--path nk.txt`` sets the path to the file containing the function

- ``-s 100`` sets the bit vector size to 100

- ``--nk-k 4`` sets the parameter k to 4

------
mapgen
------

``mapgen`` generates random maps and saves them to files. A map can be
a translation, a permutation, the composition of a permutation and a
translation, a linear map, an affine map, or and affine map whose
linear map is given by a sequence of transvections. A map file can be
loaded by ``hnco`` and used by different algorithms.
