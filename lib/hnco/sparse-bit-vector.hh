/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include <vector>
#include <iostream>

#include "bit-vector.hh"


/// top-level HNCO namespace
namespace hnco {


  /** @name Types and functions related to sparse bit vectors
   */
  ///@{

  /** Sparse bit vector.

      A sparse bit vector is represented as an array containing the
      indices of its non-zero components. The indices must be sorted
      in ascending order.

      A sparse bit vector does not know its size (dimension).
  */
  typedef std::vector<int> sparse_bit_vector_t;

  /// Display sparse bit vector
  inline void sbv_display(const sparse_bit_vector_t& v, std::ostream& stream)
  {
    for (auto c : v)
      stream << c << " ";
  }

  /// Convert a bit vector to a sparse bit vector
  inline void bv_to_sbv(const bit_vector_t& bv, sparse_bit_vector_t& sbv)
  {
    sbv = sparse_bit_vector_t(bv_hamming_weight(bv));
    size_t index = 0;
    for (size_t i = 0; i < bv.size(); i++)
      if (bv[i])
        sbv[index++] = i;
    assert(index == sbv.size());
  }

  ///@}


} // end of namespace hnco


#endif
