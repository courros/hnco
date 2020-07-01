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

#ifndef HNCO_SPARSE_BIT_VECTOR_H
#define HNCO_SPARSE_BIT_VECTOR_H

#include <iostream>

#include "bit-vector.hh"


namespace hnco {


/** @name Types and functions related to sparse bit vectors

    Output and input-output function parameters appear at the
    beginning of the parameter list.

    Input sparse_bit_vector_t parameters are passed by const
    reference.
*/
///@{

/** Sparse bit vector.

    A sparse bit vector is represented as an vector containing the
    indices of its non-zero components. The indices must be sorted in
    ascending order.

    A sparse bit vector does not know the dimension of the space it
    belongs to.
*/
typedef std::vector<int> sparse_bit_vector_t;

bool sbv_is_valid(const sparse_bit_vector_t& sbv);
bool sbv_is_valid(const sparse_bit_vector_t& sbv, int n);

/** Flip many bits of a bit vector.

    \param x Bit vector to update
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
