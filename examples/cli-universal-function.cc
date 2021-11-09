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

#include "hnco/app/algorithm-factory.hh"
#include "hnco/app/application.hh"
#include "hnco/app/function-factory.hh"
#include "hnco/app/hnco-options.hh"
#include "hnco/bit-vector.hh"
#include "hnco/functions/representations/all.hh"
#include "hnco/functions/universal-function-adapter.hh"
#include "hnco/functions/universal-function.hh"

using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function::representation;
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
    result -= std::abs(square(complex_vars[0]) + complex_vars[0] + 1.0);
    result -= std::abs(square(complex_vars[1]) + 1.0);
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
    using RealRep = DyadicRealRepresentation<double>;
    using IntegerRep = DyadicIntegerRepresentation<int>;
    using ComplexRep = DyadicComplexRepresentation<double>;
    using CategoricalRep = LinearCategoricalRepresentation;
    using PermutationRep = PermutationRepresentation;

    // Integer variables
    std::vector<IntegerRep> integer_reps;
    integer_reps.push_back(IntegerRep(0, 255));
    integer_reps.push_back(IntegerRep(0, 255));

    RealRep real_rep(-2, 2, 8);

    // Real variables
    std::vector<RealRep> real_reps;
    real_reps.push_back(real_rep);
    real_reps.push_back(real_rep);

    // Complex variables
    std::vector<ComplexRep> complex_reps;
    complex_reps.push_back(ComplexRep(real_rep, real_rep));
    complex_reps.push_back(ComplexRep(real_rep, real_rep));

    // Categorical variables
    std::vector<CategoricalRep> categorical_reps;
    categorical_reps.push_back(CategoricalRep(4));
    categorical_reps.push_back(CategoricalRep(3));

    // Permutation variables
    std::vector<PermutationRep> permutation_reps;
    permutation_reps.push_back(PermutationRep(3, 2));
    permutation_reps.push_back(PermutationRep(5, 2));

    return new UniversalFunctionAdapter(new MyFonction, 2, integer_reps, real_reps, complex_reps, categorical_reps, permutation_reps);
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
