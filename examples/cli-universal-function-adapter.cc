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

using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;

class MyFonction {
public:
  double evaluate(const bit_vector_t& boolean_vars,
                  const std::vector<int>& integer_vars,
                  const std::vector<double>& real_vars,
                  const std::vector<std::complex<double>>& complex_vars,
                  const std::vector<int>& categorical_vars,
                  const std::vector<permutation_t> permutation_vars)
  {
    double result = 0;
    return result;
  }
  void display(std::ostream& stream) const {
  }
  void describe(const bit_vector_t& boolean_vars,
                const std::vector<int>& integer_vars,
                const std::vector<double>& real_vars,
                const std::vector<std::complex<double>>& complex_vars,
                const std::vector<int>& categorical_vars,
                const std::vector<permutation_t> permutation_vars,
                std::ostream& stream)
  {
  }
};

class MyFunctionFactory:
  public FunctionFactory {
public:
  Function *make() {
    using namespace hnco::function::representation;
    std::vector<DyadicIntegerRepresentation<int>> integer_reps(2);
    std::vector<DyadicRealRepresentation<double>> real_reps(2);
    std::vector<DyadicComplexRepresentation<double>> complex_reps(2);
    std::vector<LinearCategoricalRepresentation> categorical_reps;
    categorical_reps.push_back(LinearCategoricalRepresentation(4));
    categorical_reps.push_back(LinearCategoricalRepresentation(3));
    std::vector<PermutationRepresentation> permutation_reps;
    permutation_reps.push_back(PermutationRepresentation(3, 2));
    permutation_reps.push_back(PermutationRepresentation(5, 2));
    return new UniversalFunctionAdapter(new MyFonction, 2, integer_reps, real_reps, complex_reps, categorical_reps, permutation_reps);
    // Replace reps with {} is there is no corresponding variable as in :
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
