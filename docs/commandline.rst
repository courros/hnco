====================
 Command-line tools
====================

HNCO provides the command-line tools ``hnco``, ``hnco-mo``, ``ffgen``,
and ``mapgen`` to optimize functions (single objective), optimize
functions (multiobjective), generate functions, and generate maps
respectively.

To get the list of available options of ``hnco``, enter the following
command::

  hnco --help

Help has been split into several sections given at the end of main
help. For example, enter::

  hnco --help-fn

to get help about functions.

On Ubuntu, you can get bash auto-completion by copying the file
``hnco.bash`` under::

  $HOME/.local/share/bash-completion/completions/

This can be done automatically at build time.

``hnco-mo``, ``ffgen`` and ``mapgen`` behave similarly.

-----
ffgen
-----

``ffgen`` generates random instances of fitness functions and saves
them to files which can be later loaded by ``hnco`` and used by
different algorithms. HNCO uses the Boost serialization library with
text format. The files are portable across different platforms.
However they can only be read by programs compiled with the same (or
newer) version of the library.

For example, let us generate a random instance of Nk landscape and
write it to a file::

  ffgen -F 60 --bv-size 100 --nk-k 4 --path nk.txt

Meaning of the options:

- ``-F 60`` sets the function type, here 60 represents Nk landscape;

- ``--bv-size 100`` (``-s`` for short) sets the bit vector size to
  100;

- ``--nk-k 4`` sets the parameter k to 4;

- ``--path nk.txt`` (``-p`` for short) sets the path to the file
  containing the function.

----
hnco
----

``hnco`` allows you to apply any algorithm to any function in the
library. Its command-line interface offers a complete set of
parameters, which lends itself to systematic experiments through
scripts written in any language. ``hnco`` writes results to the
standard output in text format. It is able to embed all parameters in
the output so as to exactly reproduce the simulation later, including
the same seed for random numbers.

For example, to apply (1+1) EA to the previous Nk landscape instance,
enter the following command::

  hnco -A 300 \
       -F 60 --path nk.txt --budget 200000 \
       --record-total-time \
       --record-evaluation-time \
       --print-results \
       --save-solution

Meaning of the options:

- ``-A 300`` sets the algorithm type, here 300 represents (1+1) EA;

- ``-F 60`` sets the function type, here 60 represents Nk landscape;

- ``--path nk.txt`` (``-p`` for short) sets the path to the file
  containing the function;

- ``--budget 200000`` (``-b`` for short) sets the budget to 200000
  evaluations;

- ``--record-total-time`` records the total time;

- ``--record-evaluation-time`` records the time spent evaluating the
  function;

- ``--print-results`` prints the results in JSON format;

- ``--save-solution`` saves the solution (bit vector) in text format.

Here are the results:

.. code-block:: json

   {
     "value": 0.840228,
     "num_evaluations": 33286,
     "total_num_evaluations": 200000,
     "total_time": 1.82004,
     "evaluation_time": 1.37866
   }

The best function value is 0.840228 and has been reached at evaluation
33286 out of 200000 evaluations. The whole run has taken approximately
1.8s. The computation of evaluation time is quite costly so we usually
do not record it.

As another example, let us apply (1+1) EA to OneMax::

  hnco -A 300 -s 10000 --stop-on-maximum --log-improvement -b 0

Meaning of the options:

- ``-s 10000`` sets the bit vector size to 10000;

- ``--stop-on-maximum`` tells ``hnco`` to stop as soon as it has found
  the maximum;

- ``--log-improvement`` turns on the progress tracker;

- ``-b 0`` means no limit for the budget.

------
mapgen
------

``mapgen`` generates random maps and saves them to files. A map can be
a translation, a permutation, the composition of a permutation and a
translation, a linear map, an affine map, or and affine map whose
linear map is given by a sequence of transvections. A map file can be
loaded by ``hnco`` and used by different algorithms.
