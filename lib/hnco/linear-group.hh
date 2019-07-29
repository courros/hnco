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

#ifndef HNCO_LINEAR_GROUP_H
#define HNCO_LINEAR_GROUP_H

#include <assert.h>

#include "bit-vector.hh"
#include "bit-matrix.hh"


namespace hnco {


  /** @name Types and functions related to the linear group
   */
  ///@{

  /** Transvection.

      When applied to a matrix M, a transvection (i, j) adds the i-th
      row of M to its j-th row modulo 2.

      Transvections generate invertible matrices over the finite field
      F_2.

  */
  struct Transvection {

    /// First index
    int first_index;

    /// Second index
    int second_index;

    /// Check validity
    bool is_valid() const;

    /// Display generator
    void display(std::ostream& stream) const { stream << "(" << first_index << ", " << second_index << ")"; }

    /** Sample a random generator.

        \param n Dimension
    */
    void random(int n);

    /** Apply the generator to a bit vector.

        This function modifies the given bit vector.

        \param x Bit vector
    */
    void apply(bit_vector_t& x) const;

    /** Apply the generator to a bit matrix.

        This function modifies the given bit matrix.

        \param M Bit matrix
    */
    void apply(bit_matrix_t& M) const;

  };


  /** Element of the linear group.

      The general linear (GL for short) group of a linear space of
      dimension n over the finite field F_2 is the group of invertible
      n by n bit matrices.

      Any element of GL is a finite product of transvections. Elements
      of GL are represented as sequences of transvections.
  */
  typedef std::vector<Transvection> gl_element_t;


  /// Display a GL element
  void gl_display(const gl_element_t& A, std::ostream& stream);


  /** Sample a random GL element.

      \param M GL element
      \param n Dimension
      \param t Length of the generator sequence
  */
  void gl_random(gl_element_t& A, int n, int t);


  /** Apply a GL element to a bit vector.

      This function modifies the given bit vector.

      \param M GL element
      \param x Bit vector
  */
  void gl_apply(const gl_element_t& A, bit_vector_t& x);


  /** Apply a GL element to a bit matrix.

      This function modifies the given bit matrix.

      \param A GL element
      \param M Bit matrix
  */
  void gl_apply(const gl_element_t& A, bit_matrix_t& M);


  ///@}

} // end of namespace hnco


#endif
