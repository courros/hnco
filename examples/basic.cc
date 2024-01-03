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

#include <iostream>

#include <hnco/algorithms/all.hh>
#include <hnco/functions/all.hh>

using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;

int main()
{
  Generator::set_seed();

  const int bv_size = 50;
  OneMax fn(bv_size);

  OnePlusOneEa ea(bv_size);
  ea.set_num_iterations(100);

  ea.maximize({&fn});
  ea.finalize();
  auto solution = ea.get_solution();

  bv_display(solution.first, std::cout);
  std::cout << std::endl;
  std::cout << solution.second << std::endl;

  return 0;
}
