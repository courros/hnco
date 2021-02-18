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

#include "hnco/bit-vector.hh"
#include "hnco/app/hnco-options.hh"
#include "hnco/app/algorithm-factory.hh"
#include "hnco/app/function-factory.hh"
#include "hnco/app/application.hh"

using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;

// OneMax in dimension 8
class MyFunction:
  public Function {
public:
  int get_bv_size() { return 8; }
  double evaluate(const bit_vector_t& x) {
    int result = 0;
    for (int i = 0; i < 8; i++)
      if (x[i])
        result++;
    return result;
  }
};

class MyFunctionFactory:
  public FunctionFactory {
public:
  Function *make() { return new MyFunction(); }
};

int main(int argc, char *argv[])
{
  HncoOptions options(argc, argv);
  MyFunctionFactory function_factory;
  CommandLineAlgorithmFactory algorithm_factory(options);
  CommandLineApplication application(options, function_factory, algorithm_factory);
  try {
    application.run();
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
