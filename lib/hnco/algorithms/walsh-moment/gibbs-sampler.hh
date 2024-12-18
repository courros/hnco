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

#ifndef HNCO_ALGORITHMS_WALSH_MOMENT_GIBBS_SAMPLER_H
#define HNCO_ALGORITHMS_WALSH_MOMENT_GIBBS_SAMPLER_H

#include "hnco/bit-vector.hh"
#include "hnco/algorithms/probability-vector/probability-vector.hh"

#include "walsh-moment.hh"


namespace hnco {
namespace algorithm {
namespace walsh_moment {


/// Gibbs sampler with triangular moments
class TriangularMomentGibbsSampler {

  /// Model parameters
  const TriangularMoment& _model_parameters;

  /// State of the Gibbs sampler
  bit_vector_t _state;

  /// Probability vector for synchronous Gibbs sampling
  pv_t _pv;

public:

  /// Walsh moment type
  using Moment = TriangularMoment;

  /// Constructor
  TriangularMomentGibbsSampler(int n, const TriangularMoment& mp):
    _model_parameters(mp),
    _state(n),
    _pv(n) {}

  /// Initialize
  void init();

  /// Update state
  void update(int i);

  /// Update state synchronously
  void update_sync();

  /// Get the state of the Gibbs sampler
  const bit_vector_t& get_state() { return _state; }

};

/// Gibbs sampler with full moments
class FullMomentGibbsSampler {

  /// Model parameters
  const FullMoment& _model_parameters;

  /// State of the Gibbs sampler
  bit_vector_t _state;

  /// Probability vector for synchronous Gibbs sampling
  pv_t _pv;

public:

  /// Walsh moment type
  using Moment = FullMoment;

  /// Constructor
  FullMomentGibbsSampler(int n, const FullMoment& mp):
    _model_parameters(mp),
    _state(n),
    _pv(n) {}

  /// Initialize
  void init();

  /// Update state
  void update(int i);

  /// Update state synchronously
  void update_sync();

  /// Get the state of the Gibbs sampler
  const bit_vector_t& get_state() { return _state; }

};


} // end of namespace walsh_moment
} // end of namespace algorithm
} // end of namespace hnco

#endif
