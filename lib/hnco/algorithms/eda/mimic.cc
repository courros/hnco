/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "mimic.hh"


using namespace hnco::algorithm::eda;
using namespace hnco::function;
using namespace hnco;


void
Mimic::init()
{
  random_solution();
}


void
Mimic::iterate()
{
  // sample

  if (_functions.size() > 1)
    _population.eval(_functions);
  else
    _population.eval(_function);

  _population.sort();

  update_solution(_population.get_best_bv(),
                  _population.get_best_value());

  // update model
}
