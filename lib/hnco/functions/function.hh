/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

  public:

    /// Walsh transform term
    struct WalshTransformTerm {

      /** Feature.

          Implemented with a vector bool instead of a bit_vector_t to
          reduce the memory consumption.
      */
      std::vector<bool> feature;

      /// Coefficient
      double coefficient;

    };

    /// Destructor
    virtual ~Function() {}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    virtual size_t get_bv_size() = 0;

    /** Get the global maximum.
        \throw Error */
    virtual double get_maximum() { throw exception::Error("Unknown maximum"); }

    /// Check for a known maximum.
    virtual bool has_known_maximum() { return false; }

    /** Check whether the function provides incremental evaluation.
        \return false
    */
    virtual bool provides_incremental_evaluation() { return false; }

    /** Compute the Walsh transform.

        Let \f$f\f$ be a fitness function defined on \f$\{0,
        1\}^n\f$. Then it can be expressed as \f$\sum_u c_u \chi_u\f$
        where \f$c_u = \langle f, \chi_u\rangle\f$, \f$\langle f,
        g\rangle = \sum_x f(x) g(x)\f$, \f$\chi_u(x) = (-1)^{x\cdot
        u}\f$, and \f$x\cdot u = \sum_i x_i u_i\f$ (mod 2). All sums
        run over the hypercube. We have dropped the normalizing
        constant \f$2^n\f$ or \f$2^{n/2}\f$.

        \param terms Vector of non zero terms of the Walsh transform

        \warning The size of the Walsh transform is potentially
        exponential in the dimension \f$n\f$. For example if \f$n =
        10\f$ then the size of terms is at most 1024.

    */
    virtual void compute_walsh_transform(std::vector<WalshTransformTerm>& terms);

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    virtual double eval(const bit_vector_t&) = 0;

    /** Incremental evaluation.

        \throw Error
    */
    virtual double incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits) {
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
