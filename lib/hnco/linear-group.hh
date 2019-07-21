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

#include "bit-vector.hh"


namespace hnco {

  /** @name Types and functions related to the linear group
   */
  ///@{

  /// Generator of the linear group
  struct GlGenerator {

    /// Types of generator
    enum class Type { Permutation, Transvection };

    /// Type names
    static const string names;

    /// Type of generator
    Type type;

    /// First index
    int first_index;

    /// Second index
    int second_index;

    /// Check validity
    bool is_valid() {
      if (type != Permutation &&
          type != Transvection)
        return false;
      if (first_index < 0)
        return false;
      if (second_index < 0)
        return false;
      return true;
    }

    /// Display generator
    void display(const gl_element_t& M, std::ostream& stream) {
      assert(is_valid());

      stream << names[M.type] << "(" << M.first_index << ", " << M.second_index << ")";
    }

  };

  /** Element of the linear group.

      The linear group on the finite field F_2 (GL for short) is the
      set of invertible n by n bit matrices.

      A GL element is encoded as a sequence of generators.
  */
  typedef std::vector<GlGenerator> gl_element_t;

  /// Display a GL element
  void gl_display(const gl_element_t& M, std::ostream& stream);

  /** Sample a random GL element.

      \param M GL element
      \param t Length of the generator sequence
  */
  void gl_random(gl_element_t& M, int t);

  /** Multiply a GL element and a bit vector.

      The result is y = Mx.
  */
  void gl_multiply(const gl_element_t& M, const bit_vector_t& x, bit_vector_t& y);

  ///@}

} // end of namespace hnco


#endif
