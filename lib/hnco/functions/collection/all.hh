/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

   This file is part of HNCO.

   HNCO is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   HNCO is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
   Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with HNCO. If not, see
   <http://www.gnu.org/licenses/>.

*/

#ifndef HNCO_FUNCTIONS_COLLECTION_ALL_H
#define HNCO_FUNCTIONS_COLLECTION_ALL_H

#include "cancellation.hh"
#include "equal-products.hh"
#include "four-peaks.hh"
#include "ising/all.hh"
#include "jump.hh"
#include "labs.hh"
#include "linear-function.hh"
#include "long-path.hh"
#include "max-sat.hh"
#include "nk-landscape.hh"
#include "parsed-multivariate-function.hh"
#include "partition.hh"
#include "qubo.hh"
#include "sudoku.hh"
#include "theory.hh"
#include "trap.hh"
#include "tsp.hh"
#include "walsh/all.hh"

#ifdef ENABLE_FACTORIZATION
#include "factorization.hh"
#endif

#ifdef ENABLE_PLUGIN
#include "plugin.hh"
#endif

#ifdef ENABLE_PYTHON
#include "python-function.hh"
#endif

#endif
