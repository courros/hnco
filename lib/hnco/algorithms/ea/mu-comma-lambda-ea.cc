/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include <assert.h>

#include <iostream>

#include "hnco/bit-vector.hh"
#include "hnco/functions/function.hh"

#include "mu-comma-lambda-ea.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;
using namespace std;


void
MuCommaLambdaEa::init()
{
  _parents.random();
  _parents.eval(_function);
  _parents.sort();

  set_solution(_parents.get_best_bv(),
               _parents.get_best_value());
}


void
MuCommaLambdaEa::iterate()
{
  for (size_t i = 0; i < _offsprings.size(); i++) {
    bit_vector_t& offspring = _offsprings.get_bv(i);
    offspring = _parents.get_bv(_select_parent(Random::engine));
    for (size_t i = 0; i < offspring.size(); i++)
      if (_do_mutation(Random::engine))
        bv_flip(offspring, i);
  }

  if (_functions.size() > 1)
    _offsprings.eval(_functions);
  else
    _offsprings.eval(_function);

  _offsprings.sort();

  _parents.comma_selection(_offsprings);

  update_solution(_parents.get_best_bv(),
                  _parents.get_best_value());
}
