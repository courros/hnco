/* Copyright (C) 2016 Arnaud Berny

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

#ifndef HNCO_BIT_MATRIX_H
#define HNCO_BIT_MATRIX_H

#include <algorithm>            // std::for_each
#include <vector>

#include "bit-vector.hh"


namespace hnco {

  /** @name Types and functions related to bit matrices
   */
  ///@{

  /// Bit matrix
  typedef std::vector<bit_vector_t> bit_matrix_t;

  /// Display bit matrix
  inline void bm_display(const bit_matrix_t& M, std::ostream& stream)
  {
    for (auto& bv : M) {
      bv_display(bv, stream);
      stream << std::endl;
    }
  }

  /// Check whether a bit matrix is valid
  inline bool bm_is_valid(const bit_matrix_t& M)
  {
    if (M.size() == 0)
      return false;
    std::size_t num_columns = M[0].size();
    if (num_columns == 0)
      return false;
    return all_of(M.begin(), M.end(), [num_columns](const bit_vector_t& row){ return row.size() == num_columns; });
  }

  /// Number of rows
  inline size_t bm_num_rows(const bit_matrix_t& M) { return M.size(); }

  /// Number of columns
  inline size_t bm_num_columns(const bit_matrix_t& M) { return M[0].size(); }

  /// Check whether the matrix is the identity matrix
  bool bm_is_identity(const bit_matrix_t& M);

  /// Resize a bit matrix
  void bm_resize(bit_matrix_t& M, int num_rows, int num_columns);

  /// Clear bit matrix
  inline void bm_clear(bit_matrix_t& M) { std::for_each(M.begin(), M.end(), [](bit_vector_t& row){ bv_clear(row); }); }

  /// Set the matrix to the identity matrix
  void bm_identity(bit_matrix_t& M);

  /// Sample a random bit matrix
  void bm_random(bit_matrix_t& M);

  /// Swap two rows
  void bm_swap_rows(bit_matrix_t& M, std::size_t i, std::size_t j);

  /** Add two rows.

      Row i is added to row j.
  */
  void bm_add_rows(bit_matrix_t& M, std::size_t i, std::size_t j);

  /** Invert a bit matrix.

      \param M input matrix
      \param N inverse matrix

      \return true if M is invertible

      \warning M is modified by the function. At the end, if M is
      invertible, it is changed into the identity matrix.
  */
  bool bm_invert(bit_matrix_t& M, bit_matrix_t& N);

  /** Multiply a bit matrix and a bit vector.

      The result is y = Mx.
  */
  void bm_multiply(const bit_matrix_t& M, const bit_vector_t& x, bit_vector_t& y);

  /// Transpose
  void bm_transpose(const bit_matrix_t& M, bit_matrix_t& N);

  ///@}

} // end of namespace hnco


#endif
