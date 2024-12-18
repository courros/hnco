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

#ifndef HNCO_ALGORITHMS_PROBABILITY_VECTOR_PROBABILITY_VECTOR_H
#define HNCO_ALGORITHMS_PROBABILITY_VECTOR_PROBABILITY_VECTOR_H

#include <vector>
#include <algorithm>            // std::fill

#include "hnco/bit-vector.hh"

namespace hnco {
namespace algorithm {

/**
 * @name Type and functions related to probability vectors
 */
///@{

/// Probability vector type
using pv_t = std::vector<double>;

/// Entropy of a probability vector
double pv_entropy(const pv_t& pv);

/**
 * Sample a bit vector.
 * @param pv Probability vector
 * @param bv Sampled bit vector
 */
void pv_sample(const pv_t& pv, bit_vector_t& bv);

/**
 * Probability vector of the uniform distribution.
 * All the elements of the probability vector are set to 1/2.
 * @param pv Probability vector
 */
inline void pv_uniform(pv_t& pv) { std::fill(pv.begin(), pv.end(), 0.5); }

/**
 * Initialize.
 * All the elements of the probability vector are set to 0.
 * @param pv Probability vector
 */
inline void pv_init(pv_t& pv) { std::fill(pv.begin(), pv.end(), 0.0); }

/**
 * Accumulate a bit vector into a probability vector.
 * Equivalent to pv += x
 * @param pv Probability vector
 * @param bv Bit vector
 */
void pv_add(pv_t& pv, const bit_vector_t& bv);

/**
 * Accumulate a weighted bit vector into a probability vector.
 * Equivalent to pv += weight * bv
 * @param pv Probability vector
 * @param bv Bit vector
 * @param weight Weight
 */
void pv_add(pv_t& pv, const bit_vector_t& bv, double weight);

/**
 * Average.
 * Equivalent to pv = pv / count.
 * @param pv Probability vector
 * @param count Number of accumulated bit vectors
 */
void pv_average(pv_t& pv, int count);

/**
 * Update a probability vector.
 * Equivalent to pv += rate * (x - pv)
 * @param pv Probability vector
 * @param x Attractor bit vector
 * @param rate Rate
 */
template<class T>
void pv_update(pv_t& pv, const T& x, double rate)
{
  assert(x.size() == pv.size());

  for (size_t i = 0; i < pv.size(); i++)
    pv[i] += rate * (x[i] - pv[i]);
}

/**
 * Update a probability vector.
 * Equivalent to pv += rate(x - y)
 * @param pv Probability vector
 * @param x Attractor probability vector
 * @param y Repulsor probability vector
 * @param rate Rate
 */
void pv_update(pv_t& pv, const pv_t& x, const pv_t& y, double rate);

/**
 * Bound the elements of a probability vector.
 * @param pv Probability vector
 * @param lower_bound Lower bound
 * @param upper_bound Upper bound
 */
void pv_bound(pv_t& pv, double lower_bound, double upper_bound);

///@}

} // end of namespace algorithm
} // end of namespace hnco

#endif
