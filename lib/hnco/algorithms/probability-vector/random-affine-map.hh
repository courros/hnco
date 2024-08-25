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

#ifndef HNCO_ALGORITHMS_PROBABILITY_VECTOR_RANDOM_AFFINE_MAP_H
#define HNCO_ALGORITHMS_PROBABILITY_VECTOR_RANDOM_AFFINE_MAP_H

#include <hnco/maps/map.hh>
#include <hnco/algorithms/population.hh>
#include <hnco/algorithms/probability-vector/pv-algorithm.hh>

namespace hnco {
namespace algorithm {

/**
 * UMDA with random affine maps.
 * _pv is the probability vector in the latent space.
 */
class RamUmda: public PvAlgorithm {
public:
  /// Transvection sequence length mode
  struct TsLengthMode {
    enum {
      /// Constant
      constant = 0,
      /// Increasing
      increasing = 1,
      /// Decreasing
      decreasing = 2,
      /// Random with uniform distribution
      uniform = 3,
      /// Random with geometric distribution
      geometric = 4,
      /// Random with reverse geometric distribution
      reverse_geometric = 5
    };
  };
  /// Constructor
  RamUmda(int n, int population_size)
    : PvAlgorithm(n)
    , _population_x(population_size, n)
    , _population_y(population_size, n) // Can be resized to _selection_size
    , _a(n)
    , _b(n)
    , _c(n)
  {}

  /**
   * @name Setters
   */
  ///@{
  /// Set the selection size
  void set_selection_size(int n) { _selection_size = n; }
  /// Set the transvection sequence length mode
  void set_ts_length_mode(int n) { _ts_length_mode = n; }
  /// Set the transvection sequence length
  void set_ts_length(int n) { _ts_length = n; }
  /// Set the lower bound for the transvection sequence length
  void set_ts_length_lower_bound(int n) { _ts_length_lower_bound = n; }
  /// Set the upper bound for the transvection sequence length
  void set_ts_length_upper_bound(int n) { _ts_length_upper_bound = n; }
  /// Set the transvection sequence length increment (or decrement) period
  void set_ts_length_increment_period(int n) { _ts_length_increment_period = n; }
  /// Set the parameter of the geometric distribution of the transvection sequence length
  void set_ts_length_distribution_parameter(double f) { _ts_length_distribution_parameter = f; }
  ///@}

protected:
  /// Population in the x space
  Population _population_x;
  /// Population in the y space
  Population _population_y;
  /// Transvection sequence affine map
  map::TsAffineMap _map;
  /// Inverse transvection sequence affine map
  map::TsAffineMap _inverse_map;
  /// Temporary bit vector
  bit_vector_t _a;
  /// Temporary bit vector
  bit_vector_t _b;
  /// Temporary bit vector
  bit_vector_t _c;
  /**
   * @name Parameters
   */
  ///@{
  /// Selection size
  int _selection_size = 10;
  /// Transvection sequence length mode
  int _ts_length_mode = TsLengthMode::constant;
  /// Transvection sequence length
  int _ts_length = 10;
  /// Lower bound for the transvection sequence length
  int _ts_length_lower_bound = 0;
  /// Upper bound for the transvection sequence length
  int _ts_length_upper_bound = 20;
  /// Transvection sequence length increment (or decrement) period
  int _ts_length_increment_period = 10000;
  /// Parameter of the geometric distribution of the transvection sequence length
  double _ts_length_distribution_parameter = 0.1;
  ///@}
  /**
   * @name Loop
   */
  ///@{
  /// Initialize
  void init() override;
  /// Single iteration
  void iterate() override;
  ///@}
  /// Compute the transvection sequence length
  void compute_ts_length();
};

/**
 * UMDA with random affine maps and two probability vectors.
 * _pv is the probability vector in the solution space.
 * _pv_y is the probability vector in the latent space.
 */
class RamUmda2: public RamUmda {
public:
  /// Constructor
  RamUmda2(int n, int population_size)
    : RamUmda(n, population_size)
    , _pv_y(n)
  {}
  /**
   * @name Setters
   */
  ///@{
  /// Set the crossover probability
  void set_crossover_probability(double p) { _crossover_probability = p; }
  /// Set the probability of sampling from the latent space
  void set_latent_space_probability(double p) { _latent_space_probability = p; }
  ///@}

private:
  /// Probability vector in the latent space
  pv_t _pv_y;
  /**
   * @name Parameters
   */
  ///@{
  /// Crossover probability
  double _crossover_probability = 0.1;
  /// Probability of sampling from the latent space
  double _latent_space_probability = 0.5;
  ///@}
  /**
   * @name Loop
   */
  ///@{
  /// Initialize
  void init() override;
  /// Single iteration
  void iterate() override;
  ///@}
};

} // end of namespace algorithm
} // end of namespace hnco

#endif
