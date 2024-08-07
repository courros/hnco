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

#ifndef HNCO_FUNCTIONS_CONVERTER_H
#define HNCO_FUNCTIONS_CONVERTER_H

#include <cmath>                // std::fabs
#include <complex>              // std::complex, std::norm

namespace hnco {
namespace function {

/// Convert a scalar to a double
template<class T>
struct ScalarToDouble {
  /// Codomain type
  using codomain_type = T;
  /// Convert to double
  double operator()(T x) { return x; }
};

/// Convert a scalar to a double
template<class T>
struct EquationScalarToDouble {
  /// Codomain type
  using codomain_type = T;
  /**
   * Convert to double.
   * @return The opposite of the absolute value of its argument.
   */
  double operator()(T x) { return -std::fabs(x); }
};

/// Convert a complex to a double
template<class T>
struct ComplexToDouble {
  /// Codomain type
  using codomain_type = std::complex<T>;
  /**
   * Convert to double.
   * @return The squared magnitude of its argument.
   */
  double operator()(std::complex<T> z) { return std::norm(z); }
};

/// Convert a complex to a double
template<class T>
struct EquationComplexToDouble {
  /// Codomain type
  using codomain_type = std::complex<T>;
  /**
   * Convert to double.
   * @return The opposite of the squared magnitude of its argument.
   */
  double operator()(std::complex<T> z) { return -std::norm(z); }
};

} // end of namespace function
} // end of namespace hnco

#endif
