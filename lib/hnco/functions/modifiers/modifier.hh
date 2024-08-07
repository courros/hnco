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

#ifndef HNCO_FUNCTIONS_MODIFIERS_MODIFIER_H
#define HNCO_FUNCTIONS_MODIFIERS_MODIFIER_H

#include <cassert>
#include <iosfwd>

#include "hnco/functions/decorator.hh"
#include "hnco/maps/map.hh"

namespace hnco {
namespace function {
/// Modifiers
namespace modifier {

/// Function modifier
class Modifier: public Decorator {
public:
  /// Constructor
  Modifier(Function *function)
    : Decorator(function)
  {}
};

/**
 * Negation.
 * Use cases:
 * - for algorithms which minimize rather than maximize a function
 * - for functions one wishes to minimize
 * - when minimization is needed inside an algorithm
 */
class Negation: public Modifier {
public:
  /// Constructor
  Negation(Function *function)
    : Modifier(function)
  {}

  /**
   * @name Properties
   */
  ///@{
  /// Get bit vector size
  int get_bv_size() const override { return _function->get_bv_size(); }
  /**
   * Check whether the function provides incremental evaluation.
   * @return true
   */
  bool provides_incremental_evaluation() const override { return true; }
  ///@}

  /**
   * @name Evaluation
   */
  ///@{
  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;
  /// Incrementally evaluate a bit vector
  double evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits) override;
  ///@}
};

/// Composition of a function and a map
class FunctionMapComposition: public Modifier {
  /// Map
  hnco::map::Map *_map;
  /// Image of bit vectors under the map
  bit_vector_t _bv;

public:
  /**
   * Constructor.
   * @pre map->get_output_size() == function->get_bv_size()
   * @throw std::runtime_error
   */
  FunctionMapComposition(Function *function, hnco::map::Map *map)
    : Modifier(function)
    , _map(map)
  {
    assert(map);
    if (map->get_output_size() != function->get_bv_size())
      throw std::runtime_error("FunctionMapComposition::FunctionMapComposition: _function and _map must be compatible");
    _bv.resize(function->get_bv_size());
  }

  /**
   * @name Properties
   */
  ///@{
  /// Get bit vector size
  int get_bv_size() const override { return _map->get_input_size(); }
  /**
   * Get the global maximum.
   * @throw std::runtime_error
   */
  double get_maximum() const override {
    if (has_known_maximum())
      return _function->get_maximum();
    else
      throw std::runtime_error("Unknown maximum");
  }
  /**
   * Check for a known maximum.
   * @return true if the function has a known maximum and the map is
   * bijective. */
  bool has_known_maximum() const override {
    return
      _function->has_known_maximum() &&
      _map->is_surjective();
  }
  ///@}

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /**
   * @name Display
   */
  ///@{
  /// Describe a bit vector
  void describe(const bit_vector_t& x, std::ostream& stream) override;
  ///@}
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

  /**
   * @name Properties
   */
  ///@{
  /// Get bit vector size
  int get_bv_size() const override { return _function->get_bv_size(); }
  ///@}

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;
};

} // end of namespace modifier
} // end of namespace function
} // end of namespace hnco

#endif
