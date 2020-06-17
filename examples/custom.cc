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

#include <chrono>
#include <iostream>

#include <hnco/algorithms/all.hh>
#include <hnco/functions/all.hh>

using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


// OneMax
class MyFunction:
  public Function {

  int _bv_size;

public:

  MyFunction(int bv_size):
    _bv_size(bv_size) {}

  int get_bv_size() { return _bv_size; }

  double evaluate(const bit_vector_t& x) {
    int result = 0;
    for (int i = 0; i < _bv_size; i++)
      if (x[i])
        result++;
    return result;
  }

};


// Random search
class MyAlgorithm:
  public IterativeAlgorithm {

  bit_vector_t _candidate;

protected:

  void iterate() {
    bv_random(_candidate);
    update_solution(_candidate);
  }

public:

  MyAlgorithm(int n):
    IterativeAlgorithm(n),
    _candidate(n) {}

  void init() { random_solution(); }

};


int main()
{
  Generator::set_seed();

  const int bv_size = 50;
  MyFunction fn(bv_size);

  MyAlgorithm algo(bv_size);
  algo.set_num_iterations(100);
  algo.set_function(&fn);

  algo.init();
  algo.maximize();

  // finalize not necessary
  solution_t solution = algo.get_solution();

  bv_display(solution.first, std::cout);
  std::cout << std::endl;
  std::cout << solution.second << std::endl;

  return 0;
}
