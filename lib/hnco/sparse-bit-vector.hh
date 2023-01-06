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

#ifndef HNCO_SPARSE_BIT_VECTOR_H
#define HNCO_SPARSE_BIT_VECTOR_H

#include <iostream>

#include "bit-vector.hh"


namespace hnco {


/** @name Types and functions related to sparse bit vectors

    Output and input-output function parameters appear at the
    beginning of the parameter list.

    Input object parameters are passed by const reference.
*/
///@{

/** Sparse bit vector.

    A sparse bit vector is represented as an vector containing the
    indices of its non-zero components. The indices must be sorted in
    ascending order.

    A sparse bit vector does not know the dimension of the space it
    belongs to.
*/
using sparse_bit_vector_t = std::vector<int>;

/** Check that a sparse bit vector is valid.

    A sparse bit vector is valid if:
    - Its elements are non negative.
    - Its elements are sorted in non-descending order.
*/
bool sbv_is_valid(const sparse_bit_vector_t& sbv);

/** Check that a sparse bit vector is valid.

    A sparse bit vector is valid if:
    - Its elements are non negative.
    - Its elements are sorted in non-descending order.
    - Its elements are valid indices w.r.t. the given dimension.

    \param sbv Input sparse bit vector
    \param n Dimension
*/
bool sbv_is_valid(const sparse_bit_vector_t& sbv, int n);

/** Flip many bits of a bit vector.

    \param x Input-output bit vector
    \param sbv Bits to flip
*/
void sbv_flip(bit_vector_t& x, const sparse_bit_vector_t& sbv);

/// Display sparse bit vector
void sbv_display(const sparse_bit_vector_t& v, std::ostream& stream);

/// Convert a bit vector to a sparse bit vector
sparse_bit_vector_t sbv_from_bv(const bit_vector_t& bv);

///@}


} // end of namespace hnco


#endif
