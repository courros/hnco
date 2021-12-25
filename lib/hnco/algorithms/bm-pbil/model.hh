/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_BM_PBIL_MODEL_H
#define HNCO_ALGORITHMS_BM_PBIL_MODEL_H

#include <vector>

#include "hnco/bit-vector.hh"
#include "hnco/algorithms/pv/probability-vector.hh"


namespace hnco {
namespace algorithm {
namespace bm_pbil {


/// Parameters of a Boltzmann machine
class ModelParameters
{
  /** Weights.

      _weight is a full square matrix of order n, where n is the
      dimension of the search space.
  */
  std::vector<std::vector<double> > _weight;

  /// Bias
  std::vector<double> _bias;

  friend class Model;

public:

  /// Constructor
  ModelParameters(int n):
    _weight(n, std::vector<double>(n)),
    _bias(n) {}

  /** Initialize.

      All entries of _weight are set to 0.
  */
  void init();

  /** Add a bit vector.

      Only the upper triangular part of _weight is updated with the
      equation:

      \f$ w_{ij} = w_{ij} + (-1)^{x_i + x_j} \f$

      where i < j.

  */
  void add(const bit_vector_t& x);

  /** Compute averages.

      Only the upper triangular part of _weight is averaged.

  */
  void average(int count);

  /** Update parameters in the direction of p and away from q.

      First, the upper triangular part of _weight is updated.

      Second, _weight is made symmetrical.

      \post _weight is symmetrical.
  */
  void update(const ModelParameters& p, const ModelParameters& q, double rate);

  /// Infinite norm of the parameters
  double norm_infinite();

  /// l1 norm of the parameters
  double norm_l1();

};


/// %Model of a Boltzmann machine
class Model
{
  /// Model parameters
  ModelParameters _model_parameters;

  /// State of the Gibbs sampler
  bit_vector_t _state;

  /// Probability vector for synchronous Gibbs sampling
  pv_t _pv;

public:

  /// Constructor
  Model(int n):
    _model_parameters(n),
    _state(n),
    _pv(n) {}

  /// Initialize
  void init();

  /// Reset Markov chain
  void reset_mc();

  /// A Gibbs sampler cycle
  void gibbs_sampler(int i);

  /// A synchronous Gibbs sampler
  void gibbs_sampler_synchronous();

  /// Get the state of the Gibbs sampler
  const bit_vector_t& get_state() { return _state; }

  /// Update parameters in the direction of p and away from q
  void update(const ModelParameters& p, const ModelParameters& q, double rate) {
    _model_parameters.update(p, q, rate);
  }

  /// Infinite norm of the parameters
  double norm_infinite() { return _model_parameters.norm_infinite(); }

  /// l1 norm of the parameters
  double norm_l1() { return _model_parameters.norm_l1(); }

};


}
}
}

#endif
