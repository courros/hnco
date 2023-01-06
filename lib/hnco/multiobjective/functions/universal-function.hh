/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_MULTIOBJECTIVE_FUNCTIONS_UNIVERSAL_FUNCTION_H
#define HNCO_MULTIOBJECTIVE_FUNCTIONS_UNIVERSAL_FUNCTION_H

#include <iostream>

#include "hnco/bit-vector.hh"
#include "hnco/permutation.hh"

#include "value.hh"


namespace hnco {
namespace multiobjective {
namespace function {

/** Universal function.

    A universal function is a function taking parameters of all types
    (boolean, integer, float, complex, categorical, permutation) and
    returning a double.

*/
class UniversalFunction {
public:

  /// Destructor
  virtual ~UniversalFunction() {}

  /// Get output size (number of objectives)
  virtual int get_output_size() const = 0;

  /// Evaluate the function
  virtual void evaluate(const bit_vector_t& boolean_vars,
                        const std::vector<int>& integer_vars,
                        const std::vector<double>& float_vars,
                        const std::vector<std::complex<double>>& complex_vars,
                        const std::vector<int>& categorical_vars,
                        const std::vector<permutation_t> permutation_vars,
                        value_t& value) = 0;

  /// Display the function
  virtual void display(std::ostream& stream) const {}

  /// Describe variables in the context of the function
  virtual void describe(const bit_vector_t& boolean_vars,
                        const std::vector<int>& integer_vars,
                        const std::vector<double>& float_vars,
                        const std::vector<std::complex<double>>& complex_vars,
                        const std::vector<int>& categorical_vars,
                        const std::vector<permutation_t> permutation_vars,
                        std::ostream& stream)
  {}

};

} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif
