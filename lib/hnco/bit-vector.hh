/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_BIT_VECTOR_H
#define HNCO_BIT_VECTOR_H

#include <algorithm>            // std::all_of, std::generate, std::fill
#include <iostream>
#include <numeric>              // std::accumulate
#include <vector>
#include <cstdint>              // std::uint8_t

#include "random.hh"

extern "C" {
  /// Dummy function to help autoconf
  void check_libhnco(void);
}

/// top-level HNCO namespace
namespace hnco {


/** @name Types and functions related to bit
 */
///@{

/// Bit
using bit_t = std::uint8_t;

/// Flip bit
inline bit_t bit_flip(bit_t b) { return b ? 0 : 1; }

/// Sample a random bit
inline bit_t bit_random(double p) { return (random::Generator::uniform() < p) ? 1 : 0; }

///@}


/** @name Types and functions related to bit vectors

    Output and input-output function parameters appear at the
    beginning of the parameter list.

    Output and input-output bit_vector_t parameters are passed by
    reference and must have the right size for the considered
    function.

    Input bit_vector_t parameters are passed by const reference.
*/
///@{

/// Bit vector
using bit_vector_t = std::vector<bit_t>;

/// Display bit vector
inline std::string bv_domain(const bit_vector_t& x) { return std::string("bit vector (") + std::to_string(x.size()) + std::string(" bits)"); }

/// Display bit vector
void bv_display(const bit_vector_t& v, std::ostream& stream);

/// Check whether the bit vector is valid
inline bool bv_is_valid(const bit_vector_t& x) { return std::all_of(x.begin(), x.end(), [](bit_t b){ return b == 0 || b == 1; }); }

/// Check whether the bit vector is zero
inline bool bv_is_zero(const bit_vector_t& x) { return std::all_of(x.begin(), x.end(), [](bit_t b){ return b == 0; }); }

/// Hamming weight
inline int bv_hamming_weight(const bit_vector_t& x) { return std::accumulate(x.begin(), x.end(), 0); }

/// Hamming weight
int bv_hamming_weight(const std::vector<bool>& x);

/// Hamming distance between two bit vectors
int bv_hamming_distance(const bit_vector_t& x, const bit_vector_t& y);

/// Dot product
bit_t bv_dot_product(const bit_vector_t& x, const bit_vector_t& y);

/// Dot product
bit_t bv_dot_product(const bit_vector_t& x, const std::vector<bool>& y);

/// Clear bit vector
inline void bv_clear(bit_vector_t& x) { std::fill(x.begin(), x.end(), 0); }

/// Flip a single bit
inline void bv_flip(bit_vector_t& x, int i) { x[i] = bit_flip(x[i]); }

/// Flip many bits
void bv_flip(bit_vector_t& x, const bit_vector_t& mask);

/// Sample a random bit vector
inline void bv_random(bit_vector_t& x) { std::generate(x.begin(), x.end(), []() { return random::Generator::bernoulli(); }); }

/// Sample a random bit vector with given Hamming weight
void bv_random(bit_vector_t& x, int k);

/** Add two bit vectors.

    Equivalent to dest = dest + src.

    \param dest Destination bit vector
    \param src Source bit vector

    \warning Vectors must be of the same size.
*/
void bv_add(bit_vector_t& dest, const bit_vector_t& src);

/** Add two bit vectors.

    Equivalent to dest = x + y.

    \param dest Destination bit vector
    \param x First operand
    \param y Second operand

    \warning Vectors must be of the same size.
*/
void bv_add(bit_vector_t& dest, const bit_vector_t& x, const bit_vector_t& y);

/** Convert a bit vector to a bool vector.

    \warning Vectors must be of the same size.
*/
void bv_to_vector_bool(std::vector<bool>& y, const bit_vector_t& x);

/** Convert a bool vector to a bit vector.

    \warning Vectors must be of the same size.
*/
void bv_from_vector_bool(bit_vector_t& x, const std::vector<bool>& y);

/** Convert a small bit vector to a size_t.

    x[0] is the least significant bit.

    \param x Input bit vector

    \return An unsigned integer representing x

    \pre x.size() <= 8 * sizeof(std::size_t)
*/
std::size_t bv_to_size_type(const bit_vector_t& x);

/** Convert a slice of a small bit vector to a size_t.

    x[start] is the least significant bit.

    x[stop-1] is the most significant bit.

    \param x Input bit vector
    \param start Start bit
    \param stop Stop bit

    \return An unsigned integer representing x[start], ..., x[stop-1]

    \pre start in [0, x.size())
    \pre stop in [start+1, x.size()]
    \pre (stop - start) <= 8 * sizeof(std::size_t)
*/
std::size_t bv_to_size_type(const bit_vector_t& x, int start, int stop);

/** Convert a size_t to a small bit vector.

    \param x Output bit vector
    \param u Unsigned integer representing a bit vector

    \pre x.size() <= 8 * sizeof(std::size_t)

    \warning Depending on the size of the output bit vector, some bits
    might be lost. The original bit vector can be reconstructed only
    if it is small and the unsigned integer u is the result of
    bv_to_size_type.
*/
void bv_from_size_type(bit_vector_t& x, std::size_t u);

/** Read a bit vector from a string.

    \param str Input string
    \return A bit_vector_t
*/
bit_vector_t bv_from_string(const std::string& str);

/** Read a bit vector from a stream.

    \param stream Input stream
    \return A bit_vector_t
*/
bit_vector_t bv_from_stream(std::istream& stream);

///@}


} // end of namespace hnco


#endif
