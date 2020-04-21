/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_PV_PV_H
#define HNCO_ALGORITHMS_PV_PV_H

#include <vector>
#include <algorithm>            // fill

#include "hnco/bit-vector.hh"


namespace hnco {
namespace algorithm {

  /** @name Type and functions related to probability vectors
   */
  ///@{

  /// Probability vector type
  typedef std::vector<double> pv_t;

  /// Entropy of a probability vector
  double pv_entropy(const pv_t& pv);

  /// Sample a bit vector
  void pv_sample(const pv_t& pv, bit_vector_t& x);

  /// Probability vector of the uniform distribution
  inline void pv_uniform(pv_t& pv) { fill(pv.begin(), pv.end(), 0.5); }

  /// Initialize
  inline void pv_init(pv_t& pv) { fill(pv.begin(), pv.end(), 0.0); }

  /// Accumulate a bit vector
  void pv_add(pv_t& pv, const bit_vector_t& x);

  /// Accumulate a bit vector
  void pv_add(pv_t& pv, const bit_vector_t& x, double weight);

  /// Average
  void pv_average(pv_t& pv, int count);

  /// Update a probability vector toward a bit vector
  void pv_update(pv_t& pv, double rate, const bit_vector_t& x);

  /// Update a probability vector toward a probability vector
  void pv_update(pv_t& pv, double rate, const std::vector<double>& x);

  /// Update a probability vector toward a probability vector and away from another one
  void pv_update(pv_t& pv, double rate, const std::vector<double>& x, const std::vector<double>& y);

  /// Bound the components of a probability vector
  void pv_bound(pv_t& pv, double lower_bound, double upper_bound);

  ///@}

} // end of namespace algorithm
} // end of namespace hnco


#endif
