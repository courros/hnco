/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include "iterative-algorithm.hh"


using namespace hnco::multiobjective::algorithm;


void
IterativeAlgorithm::loop()
{
  init();
  _last_iteration = false;
  for (_iteration = 0; _iteration < _num_iterations; _iteration++) {
    if (_last_iteration)
      break;
    iterate();
    if (_something_to_log)
      log();
  }
  finalize();
}

void
IterativeAlgorithm::minimize(const std::vector<Function *>& functions)
{
  set_functions(functions);
  loop();
}
