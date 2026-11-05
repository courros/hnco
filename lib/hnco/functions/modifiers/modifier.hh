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

#ifndef HNCO_FUNCTIONS_MODIFIERS_MODIFIER_H
#define HNCO_FUNCTIONS_MODIFIERS_MODIFIER_H

#include "hnco/functions/function.hh"
#include "hnco/maps/map.hh"

#include <cassert>

namespace hnco {
namespace function {
/// Modifiers
namespace modifier {

/// %Function modifier
class Modifier: public Function {
protected:
  /// Decorated function
  Function *_function;

public:
  /// Constructor
  Modifier(Function *function)
    : _function(function)
  {
    assert(function);
  }
  /// Get bit vector size
  int get_bv_size() const                                     override { return _function->get_bv_size(); }
  /// Display
  void display(std::ostream& stream) const                    override { _function->display(stream); }
  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override { _function->describe(bv, stream); }
};

/**
 * Opposite function.
 * To minimize rather than maximize a function.
 */
class OppositeFunction: public Modifier {
public:
  /// Constructor
  OppositeFunction(Function *function) : Modifier(function) {}

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t& bv)                                override { return -_function->evaluate(bv); }
  /// Check whether the function provides incremental evaluation
  bool provides_incremental_evaluation() const                           override { return _function->provides_incremental_evaluation(); }
  /// Incrementally evaluate a bit vector
  double evaluate_incrementally(const bit_vector_t& bv, double value,
                                const sparse_bit_vector_t& flipped_bits) override { return -_function->evaluate_incrementally(bv, -value, flipped_bits); }
};

/// Composition of a function and a map
class FunctionMapComposition: public Modifier {
  /// Map
  map::Map *_map;
  /// Map output
  bit_vector_t _output;

public:
  /**
   * Constructor.
   * @pre map->get_output_size() == function->get_bv_size()
   * @throw std::runtime_error
   */
  FunctionMapComposition(Function *function, map::Map *map);
  int get_bv_size() const override { return _map->get_input_size(); }
  /**
   * Get the global maximum.
   * @throw std::runtime_error
   */
  double get_maximum() const override;
  /**
   * Check for a known maximum.
   * @return true if the function has a known maximum and the map is
   * bijective.
   */
  bool has_known_maximum() const override;
  /// Evaluate a bit vector
  double evaluate(const bit_vector_t& bv) override;
  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override;
};

/// Additive Gaussian Noise
class AdditiveGaussianNoise: public Modifier {
  /// Normal distribution
  std::normal_distribution<double> _dist;

public:
  /// Constructor
  AdditiveGaussianNoise(Function *function, double stddev)
    : Modifier(function)
    , _dist(0, stddev)
  {}
  /// Evaluate a bit vector
  double evaluate(const bit_vector_t& bv) override;
};

} // end of namespace modifier
} // end of namespace function
} // end of namespace hnco

#endif
