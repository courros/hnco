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

#include <hnco/algorithms/iterative-algorithm.hh>
#include "hnco/app/hnco-options.hh"
#include "hnco/app/algorithm-factory.hh"
#include "hnco/app/function-factory.hh"
#include "hnco/app/application.hh"

using namespace hnco::algorithm;
using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;

// Random search
class MyAlgorithm:
  public IterativeAlgorithm {
protected:
  bit_vector_t _candidate;
  void init() override { random_solution(); }
  void iterate() override {
    bv_random(_candidate);
    update_solution(_candidate);
  }
public:
  MyAlgorithm(int n):
    IterativeAlgorithm(n),
    _candidate(n) {}
};

class MyAlgorithmFactory:
  public AlgorithmFactory {
public:
  Algorithm *make(int bv_size) { return new MyAlgorithm(bv_size); }
};

int main(int argc, char *argv[])
{
  HncoOptions options(argc, argv);
  CommandLineFunctionFactory function_factory(options);
  MyAlgorithmFactory algorithm_factory;
  CommandLineApplication application(options, function_factory, algorithm_factory);
  try {
    application.run();
  }
  catch (const std::runtime_error& e) {
    std::cerr << "main: std::runtime_error: " << e.what() << std::endl;
    return 1;
  }
}
