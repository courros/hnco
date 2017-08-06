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

#ifndef HNCO_FUNCTIONS_DECORATORS_FUNCTION_MODIFIER_H
#define HNCO_FUNCTIONS_DECORATORS_FUNCTION_MODIFIER_H

#include <assert.h>

#include <iostream>

#include "hnco/map.hh"

#include "function-decorator.hh"


namespace hnco::function {


  /// Function modifier
  class FunctionModifier:
    public FunctionDecorator {

  public:

    /// Constructor
    FunctionModifier(Function *function):
      FunctionDecorator(function) {}

  };


  /** Negation.

      Use cases:

      - for algorithms which minimize rather than maximize a function
      - for functions one wishes to minimize
      - when minimization is needed inside an algorithm

  */
  class Negation:
    public FunctionModifier {

  public:

    /// Constructor
    Negation(Function *function):
      FunctionModifier(function) {}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _function->get_bv_size(); }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() { throw exception::Error("Unknown maximum"); }

    /** Check for a known maximum.
        \return false */
    bool has_known_maximum() { return false; }

    /** Check whether the function provides incremental evaluation.
        \return true
    */
    bool provides_incremental_evaluation() { return true; }

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Incremental evaluation
    double eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

    ///@}

  };


  /// Composition of a function and a map
  class FunctionMapComposition:
    public FunctionModifier {

    /// Map
    Map *_map;

    /// _bv
    bit_vector_t _bv;

  public:

    /** Constructor.
        \pre map->get_output_size() == function->get_bv_size()
        \throw Error
    */
    FunctionMapComposition(Function *function, Map *map):
      FunctionModifier(function),
      _map(map)
    {
      assert(map);

      if (map->get_output_size() != function->get_bv_size())
        throw exception::Error("FunctionMapComposition::FunctionMapComposition: _function and _map must be compatible");
      _bv.resize(function->get_bv_size());
    }

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _map->get_input_size(); }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() {
      if (has_known_maximum())
        return _function->get_maximum();
      else
        throw exception::Error("Unknown maximum");
    }

    /** Check for a known maximum.
        \return true if the function has a known maximum and the map
        is bijective. */
    bool has_known_maximum() {
      return
        _function->has_known_maximum() &&
        _map->is_surjective();
    }

    ///@}

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


  /// Additive Gaussian Noise
  class AdditiveGaussianNoise:
    public FunctionModifier {

    /// Normal distribution
    std::normal_distribution<double> _dist;

  public:

    /// Constructor
    AdditiveGaussianNoise(Function *function, double stddev):
      FunctionModifier(function),
      _dist(0, stddev) {}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _function->get_bv_size(); }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() { throw exception::Error("Unknown maximum"); }

    /** Check for a known maximum.
        \return false */
    bool has_known_maximum() { return false; }

    ///@}


    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace hnco::function


#endif
