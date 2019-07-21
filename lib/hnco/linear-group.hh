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
    static const std::string names[];

    /// Type of generator
    Type type;

    /// First index
    int first_index;

    /// Second index
    int second_index;

    /// Check validity
    bool is_valid() const;

    /// Display generator
    void display(std::ostream& stream) const {
      assert(is_valid());

      stream << names[static_cast<int>(type)] << "(" << first_index << ", " << second_index << ")";
    }

    /** Sample a random generator.

        \param n Dimension
    */
    void random(int n);

    /** Apply the generator to a bit vector.

        This function modifies the given bit vector.

        \param x Bit vector
    */
    void apply(bit_vector_t& x) const;

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
      \param n Dimension
      \param t Length of the generator sequence
  */
  void gl_random(gl_element_t& M, int n, int t);

  /** Apply a GL element to a bit vector.

      This function modifies the given bit vector.

      \param M GL element
      \param x Bit vector
  */
  void gl_apply(const gl_element_t& M, bit_vector_t& x);

  ///@}

} // end of namespace hnco


#endif
