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

#include "restart.hh"


using namespace hnco::algorithm;
using namespace hnco::exception;


void
Restart::iterate(bool first_iteration)
{
  _algorithm->maximize(_functions);
  _algorithm->finalize();
  if (first_iteration)
    _solution = _algorithm->get_solution();
  else
    update_solution(_algorithm->get_solution());
}

void
Restart::maximize(const std::vector<function::Function *>& functions)
{
  set_functions(functions);
  if (_num_iterations > 0) {
    for (int i = 0; i < _num_iterations; i++) {
      iterate(i == 0);
    }
  } else {
    for (int i = 0;; i++) {
      iterate(i == 0);
    }
  }
}
