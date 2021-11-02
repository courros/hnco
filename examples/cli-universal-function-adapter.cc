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
#include "hnco/functions/representations/all.hh"
#include "hnco/functions/universal-function.hh"

using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;

class MyFonction: public UniversalFunction {
public:
  double evaluate(const bit_vector_t& boolean_vars,
                  const std::vector<int>& integer_vars,
                  const std::vector<double>& real_vars,
                  const std::vector<std::complex<double>>& complex_vars,
                  const std::vector<int>& categorical_vars,
                  const std::vector<permutation_t> permutation_vars) override
  {
    double result = 0;
    result += bv_hamming_weight(boolean_vars);
    result -= std::abs(square(integer_vars[0]) + square(integer_vars[1]) - 2500);
    result -= square(real_vars[0] - 0.5) + square(real_vars[1] - 0.5);
    result -= std::abs(complex_vars[0] - 0.5) + std::abs(complex_vars[1] - 0.5);
    result += categorical_vars[0] == 3 ? 1 : 0;
    result += categorical_vars[1] == 2 ? 1 : 0;
    result += (permutation_vars[0][0] > permutation_vars[0][1]) ? 1 : 0;
    result += (permutation_vars[0][1] > permutation_vars[0][2]) ? 1 : 0;
    result += (permutation_vars[1][0] < permutation_vars[1][1]) ? 1 : 0;
    result += (permutation_vars[1][1] < permutation_vars[1][2]) ? 1 : 0;
    result += (permutation_vars[1][2] < permutation_vars[1][3]) ? 1 : 0;
    result += (permutation_vars[1][3] < permutation_vars[1][4]) ? 1 : 0;
    return result;
  }
};

class MyFunctionFactory: public FunctionFactory {
public:
  Function *make() {
    using namespace hnco::function::representation;

    // Integer variables
    std::vector<DyadicIntegerRepresentation<int>> integer_reps;
    integer_reps.push_back(DyadicIntegerRepresentation<int>(0, 255));
    integer_reps.push_back(DyadicIntegerRepresentation<int>(0, 255));

    // Real variables
    std::vector<DyadicRealRepresentation<double>> real_reps;
    real_reps.push_back(DyadicRealRepresentation<double>(0, 1, 7));
    real_reps.push_back(DyadicRealRepresentation<double>(0, 1, 7));

    // Complex variables
    std::vector<DyadicComplexRepresentation<double>> complex_reps;
    complex_reps.push_back(DyadicComplexRepresentation<double>(0, 1, 7));
    complex_reps.push_back(DyadicComplexRepresentation<double>(0, 1, 7));

    // Categoricla variables
    std::vector<LinearCategoricalRepresentation> categorical_reps;
    categorical_reps.push_back(LinearCategoricalRepresentation(4));
    categorical_reps.push_back(LinearCategoricalRepresentation(3));

    // Permutation variables
    std::vector<PermutationRepresentation> permutation_reps;
    permutation_reps.push_back(PermutationRepresentation(3, 2));
    permutation_reps.push_back(PermutationRepresentation(5, 2));

    return new UniversalFunctionAdapter(new MyFonction, 2, integer_reps, real_reps, complex_reps, categorical_reps, permutation_reps);
    // Replace reps with {} if there is no corresponding variable as in:
    // return new UniversalFunctionAdapter(new MyFonction, 2, integer_reps, real_reps, complex_reps, {}, permutation_reps);
  }
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
  catch (const std::runtime_error& e) {
    std::cerr << "main: std::runtime_error: " << e.what() << std::endl;
    return 1;
  }
}
