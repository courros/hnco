/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_SPARSE_BIT_MATRIX_H
#define HNCO_SPARSE_BIT_MATRIX_H

#include <iostream>
#include <vector>

#include "bit-vector.hh"
#include "bit-matrix.hh"
#include "sparse-bit-vector.hh"


/// top-level HNCO namespace
namespace hnco {


  /** @name Types and functions related to sparse bit matrices
   */
  ///@{

  /** Sparse bit matrix.

      A sparse bit matrix is represented as an array of sparse bit
      vectors. It knows its number of row, not its number of columns.
  */
  typedef std::vector<sparse_bit_vector_t> sparse_bit_matrix_t;

  /// Display sparse bit matrix
  void sbm_display(const sparse_bit_matrix_t& sbm, std::ostream& stream);

  /// Convert a bit matrix to a sparse bit matrix
  void bm_to_sbm(const bit_matrix_t& bm, sparse_bit_matrix_t& sbm);

  /** Multiply a sparse bit matrix and a bit vector.

      The result is y = Mx.
  */
  void sbm_multiply(const sparse_bit_matrix_t& M, const bit_vector_t& x, bit_vector_t& y);

  ///@}


} // end of namespace hnco


#endif
