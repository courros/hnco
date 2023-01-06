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

#ifndef HNCO_FUNCTIONS_FUNCTION_H
#define HNCO_FUNCTIONS_FUNCTION_H

#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/bit-vector.hh"
#include "hnco/exception.hh"
#include "hnco/neighborhoods/neighborhood-iterator.hh"
#include "hnco/sparse-bit-vector.hh"

#include "walsh-term.hh"


namespace hnco {

/// Functions defined on bit vectors
namespace function {


/// %Function
class Function {

public:

  /// Destructor
  virtual ~Function() {}

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  virtual int get_bv_size() const = 0;

  /** Get the global maximum.
      \throw std::runtime_error */
  virtual double get_maximum() const {
    throw std::runtime_error("Function::get_maximum: Unknown maximum");
  }

  /// Check for a known maximum.
  virtual bool has_known_maximum() const { return false; }

  /** Check whether the function provides incremental evaluation.
      \return false
  */
  virtual bool provides_incremental_evaluation() const { return false; }

  ///@}


  /** @name Evaluation
   */
  ///@{

  /** Evaluate a bit vector.

      This member function is not declared const and is not supposed
      to be thread-safe. In particular, in order to evaluate a bit
      vector, it might require some data member to store temporary
      results. In case of parallel evaluation, there should be a copy
      of the function per thread, as is done in
      Population::evaluate_in_parallel.
  */
  virtual double evaluate(const bit_vector_t&) = 0;

  /** Incrementally evaluate a bit vector.

      \throw std::runtime_error
  */
  virtual double evaluate_incrementally(const bit_vector_t& x, double value,
                                        const sparse_bit_vector_t& flipped_bits)
  {
    throw std::runtime_error("Function::evaluate_incrementally: This function does not provide incremental evaluation");
  }

  /** Safely evaluate a bit vector.

      Must neither throw any exception nor update global states (e.g.
      maximum) in function controllers. It is used in
      Population::evaluate_in_parallel inside a OMP parallel for loop.

      By default, calls evaluate.
  */
  virtual double evaluate_safely(const bit_vector_t& x) { return evaluate(x); }

  /** Update states after a safe evaluation.

      By default, does nothing.
  */
  virtual void update(const bit_vector_t& x, double value) {}

  ///@}


  /** @name Display
   */
  ///@{

  /// Display
  virtual void display(std::ostream& stream) const { stream << "Function" << std::endl; }

  /** Describe a bit vector.

      The member function Function::describe is not declared const for
      the same reason Function::evaluate is not: it might need to
      decode the given bit vector hence use some pre-allocated memory
      buffer.
  */
  virtual void describe(const bit_vector_t& x, std::ostream& stream) {
    bv_display(x, stream);
    stream << std::endl;
  }

  ///@}

};

/** Compute the Walsh transform of the function.

    Let \f$f\f$ be a fitness function defined on the hypercube \f$\{0,
    1\}^n\f$. Then it can be expressed as \f$\sum_u c_u \chi_u\f$
    where \f$c_u = \langle f, \chi_u\rangle\f$, \f$\langle f, g\rangle
    = \frac1{2^n} \sum_x f(x) g(x)\f$, \f$\chi_u(x) = (-1)^{x\cdot
    u}\f$, and \f$x\cdot u = \sum_i x_i u_i\f$ (mod 2). In the
    respective sums, we have \f$x\f$ and \f$u\f$ in the hypercube and
    \f$i\f$ in \f$\{1, \ldots, n\}\f$.

    We have dropped the normalizing constant \f$2^n\f$ since we are
    mostly interested in ratios \f$|c_u/c_{\max}|\f$, where
    \f$c_{\max}\f$ is the coefficient with the largest amplitude. It
    is also helpful to achieve exact computations in the case of
    functions taking only integer values.

    \param function Function the Walsh transform of which to compute
    \param terms Vector of non zero terms of the Walsh transform

    \warning The time complexity is exponential in the dimension n.
    The computation is done with two nested loops over the hypercube.
    It requires \f$2^n\f$ function evaluations and \f$2^{2n}\f$ dot
    products and additions.

    \warning The size of the Walsh transform is potentially
    exponential in the dimension n. For example, if n = 10 then the
    number of terms is at most 1024.

*/
void compute_walsh_transform(function::Function *function,
                             std::vector<function::WalshTerm>& terms);


/** Compute the Walsh transform of the function using a fast Walsh
    transform.

    Let \f$f\f$ be a fitness function defined on the hypercube \f$\{0,
    1\}^n\f$. Then it can be expressed as \f$\sum_u c_u \chi_u\f$
    where \f$c_u = \langle f, \chi_u\rangle\f$, \f$\langle f, g\rangle
    = \frac1{2^n} \sum_x f(x) g(x)\f$, \f$\chi_u(x) = (-1)^{x\cdot
    u}\f$, and \f$x\cdot u = \sum_i x_i u_i\f$ (mod 2). In the
    respective sums, we have \f$x\f$ and \f$u\f$ in the hypercube and
    \f$i\f$ in \f$\{1, \ldots, n\}\f$.

    We have dropped the normalizing constant \f$2^n\f$ since we are
    mostly interested in ratios \f$|c_u/c_{\max}|\f$, where
    \f$c_{\max}\f$ is the coefficient with the largest amplitude. It
    is also helpful to achieve exact computations in the case of
    functions taking only integer values.

    \param function Function the Walsh transform of which to compute
    \param terms Vector of non zero terms of the Walsh transform

    \warning The time complexity is exponential in the dimension n. It
    requires \f$2^n\f$ function evaluations and \f$n2^{n}\f$
    additions, which is faster than compute_walsh_transform.

    \warning The size of the Walsh transform is potentially
    exponential in the dimension n. For example, if n = 10 then the
    number of terms is at most 1024.

*/
void compute_fast_walsh_transform(function::Function *function,
                                  std::vector<function::WalshTerm>& terms);

/// Check whether a bit vector is locally maximal
bool bv_is_locally_maximal(const bit_vector_t& bv, Function& fn,
                           neighborhood::NeighborhoodIterator& it);

/// Check whether a bit vector is globally maximal
bool bv_is_globally_maximal(const bit_vector_t& bv, Function& fn);

} // end of namespace function
} // end of namespace hnco


#endif
