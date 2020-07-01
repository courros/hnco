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

#ifndef HNCO_BIT_MATRIX_H
#define HNCO_BIT_MATRIX_H

#include <algorithm>            // std::for_each, std::all_of

#include "bit-vector.hh"


namespace hnco {


/** @name Types and functions related to bit matrices

    Output and input-output function parameters appear at the
    beginning of the parameter list.

    Output and input-output bit_matrix_t parameters are passed by
    reference and must have the right size for the considered
    function.
*/
///@{

/// Bit matrix
typedef std::vector<bit_vector_t> bit_matrix_t;


/// Make a rectangular bit matrix
bit_matrix_t bm_rectangular(int nrows, int ncols);

/// Make a square bit matrix
inline bit_matrix_t bm_square(int n) { return bm_rectangular(n, n); }

/** Set a matrix to the identity matrix.

    \pre bm_is_square(M)
*/
void bm_identity(bit_matrix_t& M);

/** Make an identity bit matrix.

    \param n Dimension
    \return An order n identity matrix
*/
bit_matrix_t bm_identity(int n);

/** Transpose a bit matrix.

    \pre bm_num_columns(N) == bm_num_rows(M)
    \pre bm_num_rows(N) == bm_num_columns(M)
*/
void bm_transpose(bit_matrix_t& N, const bit_matrix_t& M);

/** Transpose a bit matrix.

    \param M Bit matrix
    \return Transposed bit matrix
*/
bit_matrix_t bm_transpose(const bit_matrix_t& M);


/// Display bit matrix
void bm_display(const bit_matrix_t& M, std::ostream& stream);

/// Check whether a bit matrix is valid
bool bm_is_valid(const bit_matrix_t& M);

/// Number of rows
inline int bm_num_rows(const bit_matrix_t& M) { return M.size(); }

/// Number of columns
inline int bm_num_columns(const bit_matrix_t& M) { return M[0].size(); }

/// Check whether the matrix is a square matrix
inline bool bm_is_square(const bit_matrix_t& M) { return bm_num_rows(M) == bm_num_columns(M); }

/// Check whether the matrix is the identity matrix
bool bm_is_identity(const bit_matrix_t& M);

/// Check whether the matrix is upper triangular
bool bm_is_upper_triangular(const bit_matrix_t& M);


/// Resize a bit matrix
void bm_resize(bit_matrix_t& M, int nrows, int ncols);

/// Resize a bit matrix and make it a square matrix
inline void bm_resize(bit_matrix_t& M, int nrows) { bm_resize(M, nrows, nrows); }

/// Clear bit matrix
inline void bm_clear(bit_matrix_t& M) { std::for_each(M.begin(), M.end(), [](bit_vector_t& row){ bv_clear(row); }); }

/// Sample a random bit matrix
void bm_random(bit_matrix_t& M);

/// Swap two rows
void bm_swap_rows(bit_matrix_t& M, int i, int j);

/** Add two rows.

    Equivalent to dest = dest + src.

    \param M Bit matrix
    \param dest Destination row
    \param src Source row
*/
void bm_add_rows(bit_matrix_t& M, int dest, int src);

/** Add two columns.

    Equivalent to dest = dest + src.

    \param M Bit matrix
    \param dest Destination column
    \param src Source column

    \warning M is modified by the function.
*/
void bm_add_columns(bit_matrix_t& M, int dest, int src);


/** Compute a row echelon form of a matrix.

    \warning A is modified by the function.
*/
void bm_row_echelon_form(bit_matrix_t& A);

/** Compute the rank of a matrix.

    \pre A must be in row echelon form.
*/
int bm_rank(const bit_matrix_t& A);

/** Solve a linear system.

    Solve the linear equation Ax = b.

    \param A Matrix
    \param b Right hand side

    \pre bm_is_square(A)
    \pre bm_num_rows(A) == b.size()

    \return true if the system has a unique solution

    \warning Both A and b are modified by the function. Provided
    that A is invertible, after returning from the function, A is
    the identity matrix and b is the unique solution to the linear
    equation.
*/
bool bm_solve(bit_matrix_t& A, bit_vector_t& b);

/** Solve a linear system in upper triangular form.

    Solve the linear equation Ax = b.

    \param A Upper triangular matrix
    \param b Right hand side

    \pre bm_is_square(A)
    \pre bm_num_rows(A) == b.size()
    \pre bm_is_upper_triangular(A)

    \return true if the system has a unique solution

    \warning Both A and b are modified by the function. Provided
    that A is invertible, after returning from the function, A is
    the identity matrix and b is the unique solution to the linear
    equation.
*/
bool bm_solve_upper_triangular(bit_matrix_t& A, bit_vector_t& b);

/** Invert a bit matrix.

    \param M Bit matrix
    \param N Inverse bit matrix

    \pre bm_is_square(M)
    \pre bm_is_square(N)
    \pre bm_num_rows(M) == bm_num_rows(N)

    \return true if M is invertible

    \warning M is modified by the function. Provided that M is
    invertible, after returning from the function, M is the identity
    matrix and N is the computed inverse matrix.
*/
bool bm_invert(bit_matrix_t& M, bit_matrix_t& N);

/** Multiply a bit matrix and a bit vector.

    Computes y = Mx.

    \param y Output bit vector
    \param M Bit matrix
    \param x Bit vector
*/
void bm_multiply(bit_vector_t& y, const bit_matrix_t& M, const bit_vector_t& x);

///@}


} // end of namespace hnco


#endif
