/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_ALL_H
#define HNCO_FUNCTIONS_ALL_H

#include "hnco/functions/cancellation.hh"
#include "hnco/functions/controllers/all.hh"
#include "hnco/functions/equal-products.hh"
#include "hnco/functions/four-peaks.hh"
#include "hnco/functions/decorator.hh"
#include "hnco/functions/function.hh"
#include "hnco/functions/ising/all.hh"
#include "hnco/functions/jump.hh"
#include "hnco/functions/labs.hh"
#include "hnco/functions/linear-function.hh"
#include "hnco/functions/long-path.hh"
#include "hnco/functions/max-sat.hh"
#include "hnco/functions/modifiers/all.hh"
#include "hnco/functions/nk-landscape.hh"
#include "hnco/functions/partition.hh"
#include "hnco/functions/qubo.hh"
#include "hnco/functions/representations/all.hh"
#include "hnco/functions/theory.hh"
#include "hnco/functions/trap.hh"
#include "hnco/functions/walsh/all.hh"

#ifdef ENABLE_FACTORIZATION
#include "hnco/functions/factorization.hh"
#endif

#ifdef ENABLE_PLUGIN
#include "hnco/functions/plugin.hh"
#endif

#endif
