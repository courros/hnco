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

#include "umda.hh"


using namespace hnco::function;
using namespace hnco::algorithm;
using namespace hnco;


void
Umda::init()
{
  set_something_to_log();

  random_solution();
  pv_uniform(_pv);
}


void
Umda::iterate()
{
  for (int i = 0; i < _population.get_size(); i++)
    pv_sample(_population.bvs[i], _pv);

  if (_functions.size() > 1)
    _population.evaluate_in_parallel(_functions);
  else
    _population.evaluate(_function);

  _population.sort();

  update_solution(_population.get_best_bv(),
                  _population.get_best_value());

  pv_init(_pv);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_pv, _population.get_best_bv(i));
  pv_average(_pv, _selection_size);
  pv_bound(_pv, _lower_bound, _upper_bound);

}
