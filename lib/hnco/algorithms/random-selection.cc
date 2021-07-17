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

#include "random-selection.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


const bit_vector_t&
UniformSelection::select()
{
  return get_best_bv(_choose_individual(Generator::engine));
}


const bit_vector_t&
TournamentSelection::select()
{
  int winner = _choose_individual(Generator::engine);
  for (int i = 0; i < _tournament_size; i++) {
    int challenger;
    do {
      challenger = _choose_individual(Generator::engine);
    } while (challenger == winner);
    if (_compare_index_value(_lookup[challenger], _lookup[winner]))
      winner = challenger;
  }
  return get_best_bv(winner);
}
