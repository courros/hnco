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

#ifndef HNCO_FUNCTIONS_FUNCTION_H
#define HNCO_FUNCTIONS_FUNCTION_H

#include "hnco/neighborhoods/neighborhood-iterator.hh"
#include "hnco/bit-vector.hh"
#include "hnco/sparse-bit-vector.hh"

namespace hnco {
/// Functions defined on bit vectors
namespace function {

/// %Function
/**
 * %Function.
 */
class Function {
public:
  /// Destructor
  virtual ~Function() {}
  /**
   * @name Information about the function
   */
  ///@{
  /// Get bit vector size
  virtual int get_bv_size() const = 0;
  /**
   * Get the global maximum.
   * @throw std::runtime_error
   */
  virtual double get_maximum() const { throw std::runtime_error("Function::get_maximum: Unknown maximum"); }
  /// Check for a known maximum.
  virtual bool has_known_maximum() const { return false; }
  /**
   * Check whether the function provides incremental evaluation.
   * @return false
   */
  virtual bool provides_incremental_evaluation() const { return false; }
  ///@}
  /**
   * @name Evaluation
   */
  ///@{
  /**
   * Evaluate a bit vector.
   *
   * This member function is not declared const and is not supposed to
   * be thread-safe. In particular, in order to evaluate a bit vector,
   * it might require some data member to store temporary results. In
   * case of parallel evaluation, there should be a copy of the
   * function per thread, as is done in
   * Population::evaluate_in_parallel.
   */
  virtual double evaluate(const bit_vector_t&) = 0;
  /**
   * Incrementally evaluate a bit vector.
   * @throw std::runtime_error
   */
  virtual double evaluate_incrementally(const bit_vector_t& bv, double value, const sparse_bit_vector_t& flipped_bits) {
    throw std::runtime_error("Function::evaluate_incrementally: This function does not provide incremental evaluation");
  }
  /**
   * Safely evaluate a bit vector.
   * 
   * Must neither throw any exception nor update global states (e.g.
   * maximum) in function controllers. It is used in
   * Population::evaluate_in_parallel inside a OMP parallel for loop.
   * 
   * By default, calls evaluate.
   */
  virtual double evaluate_safely(const bit_vector_t& bv) { return evaluate(bv); }
  /**
   * Update states after a safe evaluation.
   * 
   * By default, does nothing.
   */
  virtual void update(const bit_vector_t& bv, double value) {}
  ///@}
  /**
   * @name Display
   */
  ///@{
  /// Display
  virtual void display(std::ostream& stream) const;
  /**
   * Describe a bit vector.
   *
   * The member function Function::describe is not declared const for
   * the same reason Function::evaluate is not: it might need to
   * decode the given bit vector hence use some pre-allocated memory
   * buffer.
   */
  virtual void describe(const bit_vector_t& bv, std::ostream& stream) {
    bv_display(bv, stream);
    stream << std::endl;
  }
  ///@}
};

/// Check whether a bit vector is locally maximal
bool bv_is_locally_maximal(const bit_vector_t& bv, Function& fn, neighborhood::NeighborhoodIterator& it);

/// Check whether a bit vector is globally maximal
bool bv_is_globally_maximal(const bit_vector_t& bv, Function& fn);

} // end of namespace function
} // end of namespace hnco

#endif
