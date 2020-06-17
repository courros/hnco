/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include "mu-comma-lambda-ea.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


void
MuCommaLambdaEa::init()
{
  _mutation.set_probability(_mutation_probability);
  _mutation.set_allow_stay(_allow_stay);

  _parents.random();

  // There is no need to evaluate them as they are going to be
  // replaced by offsprings.

  random_solution();
}


void
MuCommaLambdaEa::iterate()
{
  for (int i = 0; i < _offsprings.size(); i++) {
    bit_vector_t& offspring = _offsprings.get_bv(i);
    offspring = _parents.get_bv(_select_parent(Generator::engine));
    _mutation.mutate(offspring);
  }

  if (_functions.size() > 1)
    _offsprings.evaluate(_functions);
  else
    _offsprings.evaluate(_function);

  _offsprings.shuffle();
  _offsprings.partial_sort(_parents.size());
  _parents.comma_selection(_offsprings);

  update_solution(_parents.get_best_bv(),
                  _parents.get_best_value());
}
