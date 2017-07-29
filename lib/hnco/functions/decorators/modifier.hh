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

#ifndef HNCO_FUNCTIONS_DECORATORS_MODIFIER_H
#define HNCO_FUNCTIONS_DECORATORS_MODIFIER_H

#include <assert.h>

#include <iostream>
#include <unordered_map>

#include "hnco/map.hh"

#include "hnco/functions/decorators/function-decorator.hh"


namespace hnco::function {


  /** Negation.

      Use cases:

      - for algorithms which minimize rather than maximize a function
      - for functions one wishes to minimize
      - when minimization is needed inside an algorithm

  */
  class Negation:
    public FunctionDecorator {

  public:

    /// Constructor
    Negation(Function *function):
      FunctionDecorator(function) {}

    /// Constructor
    Negation() {}

    /** Check for a known maximum.
        \return false */
    bool has_known_maximum() { return false; }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() { throw exception::Error("Unknown maximum"); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Incremental evaluation
    double delta(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits);

  };


  /// Composition of a function and a map
  class FunctionMapComposition:
    public FunctionDecorator {

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
      FunctionDecorator(function),
      _map(map)
    {
      assert(map);

      if (map->get_output_size() != function->get_bv_size())
        throw exception::Error("FunctionMapComposition::FunctionMapComposition: _function and _map must be compatible");
      _bv.resize(function->get_bv_size());
    }

    /// Get bit vector size
    size_t get_bv_size() { return _map->get_input_size(); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true if the function has a known maximum and the map
        is bijective. */
    bool has_known_maximum() {
      return
        _function->has_known_maximum() &&
        _map->is_surjective();
    }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() {
      if (has_known_maximum())
        return _function->get_maximum();
      else
        throw exception::Error("Unknown maximum");
    }

  };


  /// Additive Gaussian Noise
  class AdditiveGaussianNoise:
    public FunctionDecorator {

    /// Normal distribution
    std::normal_distribution<double> _dist;

  public:

    /// Constructor
    AdditiveGaussianNoise(Function *function, double stddev):
      FunctionDecorator(function),
      _dist(0, stddev) {}

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return false */
    bool has_known_maximum() { return false; }

    /** Get the global maximum.
        \throw Error */
    double get_maximum() { throw exception::Error("Unknown maximum"); }

  };


} // end of namespace hnco::function


#endif
