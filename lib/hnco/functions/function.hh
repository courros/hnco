/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include "hnco/bit-vector.hh"
#include "hnco/exception.hh"
#include "hnco/sparse-bit-vector.hh"


/// Functions to be maximized
namespace hnco::function {

  /// Function
  class Function {

  protected:

  public:

    /// Destructor
    virtual ~Function() {}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    virtual size_t get_bv_size() = 0;

    /// Check for a known maximum.
    virtual bool has_known_maximum() { return false; }

    /** Get the global maximum.
        \throw Error */
    virtual double get_maximum() { throw exception::Error("Unknown maximum"); }

    /** Check whether the function provides incremental evaluation.
        \return false
    */
    virtual bool provides_incremental_evaluation() { return false; }

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    virtual double eval(const bit_vector_t&) = 0;

    /** Incremental evaluation.

        If \f$x \rightarrow x' = x + \delta x\f$ then \f$f(x)
        \rightarrow f(x') = f(x) + \delta f\f$.

        This function computes \f$f(x')\f$.

        \throw Error
    */
    virtual double eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits) {
      throw exception::Error("Incremental evaluation not implemented for this function");
    }

    /** Incremental evaluation.

        If \f$x \rightarrow x' = x + \delta x\f$ then \f$f(x)
        \rightarrow f(x') = f(x) + \delta f\f$.

        This function computes \f$\delta f\f$.

        \throw Error
    */
    virtual double delta(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits) {
      throw exception::Error("Incremental evaluation not implemented for this function");
    }

    /** Safely evaluate a bit vector.

        Must be thread-safe, that is must avoid throwing exceptions
        and updating global states (e.g. maximum) in function
        decorators.
    */
    virtual double safe_eval(const bit_vector_t& x) { return eval(x); }

    /// Update after a safe evaluation
    virtual void update(const bit_vector_t& x, double value) {}

    ///@}


    /** @name Display
     */
    ///@{

    /// Display
    virtual void display(std::ostream& stream) {}

    /// Describe a bit vector
    virtual void describe(const bit_vector_t& x, std::ostream& stream) {
      bv_display(x, stream);
      stream << std::endl;
    }

    ///@}

  };

} // end of namespace hnco::function


#endif
