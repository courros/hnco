/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_WALSH_H
#define HNCO_FUNCTIONS_WALSH_H

#include "hnco/functions/function.hh"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace hnco {
namespace function {

/// Walsh transform term
struct WalshTerm {
  /**
   * Feature.
   *
   * Implemented <with a vector bool instead of a bit_vector_t to
   * reduce the memory consumption.
   */
  std::vector<bool> feature;
  /// Coefficient
  double coefficient;
  /// Serialize
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & feature;
    ar & coefficient;
  }
};

/**
 * Compute the Walsh transform of the function.
 * 
 * Let \f$f\f$ be a fitness function defined on the hypercube \f$\{0,
 * 1\}^n\f$. Then it can be expressed as \f$\sum_u c_u \chi_u\f$ where
 * \f$c_u = \langle f, \chi_u\rangle\f$, \f$\langle f, g\rangle =
 * \frac1{2^n} \sum_x f(x) g(x)\f$, \f$\chi_u(x) = (-1)^{x\cdot u}\f$,
 * and \f$x\cdot u = \sum_i x_i u_i\f$ (mod 2). In the respective
 * sums, we have \f$x\f$ and \f$u\f$ in the hypercube and \f$i\f$ in
 * \f$\{1, \ldots, n\}\f$.
 * 
 * We have dropped the normalizing constant \f$2^n\f$ since we are
 * mostly interested in ratios \f$|c_u/c_{\max}|\f$, where
 * \f$c_{\max}\f$ is the coefficient with the largest amplitude. It is
 * also helpful to achieve exact computations in the case of functions
 * taking only integer values.
 * 
 * @param function Function the Walsh transform of which to compute
 * @param terms Vector of non zero terms of the Walsh transform
 * 
 * @warning The time complexity is exponential in the dimension n. The
 * computation is done with two nested loops over the hypercube. It
 * requires \f$2^n\f$ function evaluations and \f$2^{2n}\f$ dot
 * products and additions.
 * 
 * @warning The size of the Walsh transform is potentially exponential
 * in the dimension n. For example, if n = 10 then the number of terms
 * is at most 1024.
 */
void compute_walsh_transform(function::Function *function, std::vector<function::WalshTerm>& terms);

/**
 * Compute the Walsh transform of the function using a fast Walsh
 * transform.
 * 
 * Let \f$f\f$ be a fitness function defined on the hypercube \f$\{0,
 * 1\}^n\f$. Then it can be expressed as \f$\sum_u c_u \chi_u\f$ where
 * \f$c_u = \langle f, \chi_u\rangle\f$, \f$\langle f, g\rangle =
 * \frac1{2^n} \sum_x f(x) g(x)\f$, \f$\chi_u(x) = (-1)^{x\cdot u}\f$,
 * and \f$x\cdot u = \sum_i x_i u_i\f$ (mod 2). In the respective
 * sums, we have \f$x\f$ and \f$u\f$ in the hypercube and \f$i\f$ in
 * \f$\{1, \ldots, n\}\f$.
 * 
 * We have dropped the normalizing constant \f$2^n\f$ since we are
 * mostly interested in ratios \f$|c_u/c_{\max}|\f$, where
 * \f$c_{\max}\f$ is the coefficient with the largest amplitude. It is
 * also helpful to achieve exact computations in the case of functions
 * taking only integer values.
 * 
 * @param function Function the Walsh transform of which to compute
 * @param terms Vector of non zero terms of the Walsh transform
 * 
 * @warning The time complexity is exponential in the dimension n. It
 * requires \f$2^n\f$ function evaluations and \f$n2^{n}\f$ additions,
 * which is faster than compute_walsh_transform.
 * 
 * @warning The size of the Walsh transform is potentially exponential
 * in the dimension n. For example, if n = 10 then the number of terms
 * is at most 1024.
 */
void compute_fast_walsh_transform(function::Function *function, std::vector<function::WalshTerm>& terms);

} // end of namespace function
} // end of namespace hnco

#endif
