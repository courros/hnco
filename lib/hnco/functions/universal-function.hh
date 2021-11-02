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

#ifndef HNCO_FUNCTIONS_UNIVERSAL_FUNCTION_H
#define HNCO_FUNCTIONS_UNIVERSAL_FUNCTION_H

#include <iostream>

#include "hnco/bit-vector.hh"
#include "hnco/permutation.hh"


namespace hnco {
namespace function {

/** Universal function.

    A universal function is a function taking parameters of all types
    (boolean, integer, real, complex, categorical, permutation) and
    returning a double.

*/
class UniversalFunction {
public:

  /// Evaluate the function
  virtual double evaluate(const bit_vector_t& boolean_vars,
                          const std::vector<int>& integer_vars,
                          const std::vector<double>& real_vars,
                          const std::vector<std::complex<double>>& complex_vars,
                          const std::vector<int>& categorical_vars,
                          const std::vector<permutation_t> permutation_vars) = 0;

  /// Display the function
  virtual void display(std::ostream& stream) const {}

  /// Describe variables in the context of the function
  virtual void describe(const bit_vector_t& boolean_vars,
                        const std::vector<int>& integer_vars,
                        const std::vector<double>& real_vars,
                        const std::vector<std::complex<double>>& complex_vars,
                        const std::vector<int>& categorical_vars,
                        const std::vector<permutation_t> permutation_vars,
                        std::ostream& stream)
  {}

};


} // end of namespace function
} // end of namespace hnco


#endif
