/* Copyright (C) 2016 Arnaud Berny

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

#include "nps-pbil.hh"


using namespace hnco::algorithm;
using namespace hnco;


void
NpsPbil::init()
{
  random_solution();
  pv_uniform(_pv);
}


void
NpsPbil::iterate()
{
  for (size_t i = 0; i < _population.size(); i++)
    pv_sample(_pv, _population.get_bv(i));

  _population.eval(_function);

  _population.sort();

  update_solution(_population.get_nth_bv(0),
                  _population.get_evaluation(0).value);

  pv_init(_mean_best);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_mean_best, _population.get_nth_bv(i));
  pv_average(_mean_best, _selection_size);

  pv_init(_mean_worst);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_mean_worst, _population.get_nth_bv(_last_index - i));
  pv_average(_mean_worst, _selection_size);

  pv_update(_pv, _rate, _mean_best, _mean_worst);
  pv_bound(_pv, _lower_bound, _upper_bound);

}
