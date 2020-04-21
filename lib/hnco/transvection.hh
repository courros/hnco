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

#ifndef HNCO_TRANSVECTION_H
#define HNCO_TRANSVECTION_H

#include <assert.h>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "bit-vector.hh"
#include "bit-matrix.hh"


namespace hnco {


  /** @name Types and functions related to transvections
   */
  ///@{

  /** %Transvection.

      We only consider transvections defined by matrices \f$\tau_{ij}
      = I_n + B_{ij}\f$, where \f$I_n\f$ is the \f$n\times n\f$
      identity matrix and \f$B_{ij}\f$ is the matrix whose \f$(i,
      j)\f$ entry is 1 and other entries are zero. Such a matrix is
      also sometimes called a shear matrix.

      Transvections generate invertible matrices over the finite field
      \f$F_2\f$.

  */
  struct Transvection {

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & row_index;
      ar & column_index;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & row_index;
      ar & column_index;

      assert(is_valid());
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Row index
    int row_index;

    /// Column index
    int column_index;

    /// Check validity
    bool is_valid() const;

    /** Check validity.

        \param n Dimension
    */
    bool is_valid(int n) const;

    /// Display transvection
    void display(std::ostream& stream) const { stream << "(" << row_index << ", " << column_index << ")"; }

    /** Sample a random transvection.

        \param n Dimension

        \pre n > 1
    */
    void random(int n);

    /** Sample a random transvection.

        This member function ensures that the sampled transvection
        does not commute with some given one.

        \param n Dimension
        \param a Given transvection

        \pre n > 1
    */
    void random_non_commuting(int n, const Transvection& a);

    /** Multiply a bit vector from the left.

        \param x Bit vector

        \pre is_valid()
        \pre is_valid(x.size())

        \warning This function modifies the given bit vector.
    */
    void multiply(bit_vector_t& x) const;

    /** Multiply a bit matrix from the left.

        \param M Bit matrix

        \pre is_valid()
        \pre is_valid(bm_num_rows(M))

        \warning This function modifies the given bit vector.
    */
    void multiply(bit_matrix_t& M) const;

    /** Multiply a bit matrix from the right.

        \param M Bit matrix

        \pre is_valid()
        \pre is_valid(bm_num_rows(M))

        \warning This function modifies the given bit vector.
    */
    void multiply_right(bit_matrix_t& M) const;

  };

  /// Check whether two transvections commute
  bool transvections_commute(const Transvection& a, const Transvection& b);

  /// Check whether two transvections are disjoint
  bool transvections_are_disjoint(const Transvection& a, const Transvection& b);


  /** %Transvection sequence.

      The general linear group of a linear space of dimension n over
      the finite field F_2 is the group of invertible n by n bit
      matrices.

      Any invertible bit matrix can be expressed as a finite product
      of transvections.

      Finite transvection sequences can then represent all invertible
      bit matrices.
  */
  typedef std::vector<Transvection> transvection_sequence_t;


  /** Check validity.

      \param ts %Transvection sequence
  */
  bool ts_is_valid(const transvection_sequence_t& ts);

  /** Check validity.

      \param ts %Transvection sequence
      \param n Dimension
  */
  bool ts_is_valid(const transvection_sequence_t& ts, int n);

  /// Display a transvection sequence
  void ts_display(const transvection_sequence_t& ts, std::ostream& stream);


  /** Sample a random transvection sequence.

      \param ts %Transvection sequence
      \param n Dimension
      \param t Length of the sequence

      \pre n > 1
      \pre t >= 0
  */
  void ts_random(transvection_sequence_t& ts, int n, int t);

  /** Sample a random sequence of commuting transvections.

      This function ensures that all transvections in the sequence
      commute.

      \param ts %Transvection sequence
      \param n Dimension
      \param t Length of the sequence

      \pre n > 1
      \pre t >= 0

      \warning If t > floor(n / 2) then t is set to floor(n / 2).

      \warning If t = floor(n / 2) then the space and time complexity
      of ts_random_commuting is quadratic in the dimension n.
  */
  void ts_random_commuting(transvection_sequence_t& ts, int n, int t);

  /** Sample a random sequence of transvections with unique source.

      A transvection sequence with unique source is such that, for
      each destination, there is a unique source.

      \param ts %Transvection sequence
      \param n Dimension
      \param t Length of the sequence

      \pre n > 1
      \pre t >= 0
  */
  void ts_random_unique_source(transvection_sequence_t& ts, int n, int t);

  /** Sample a random sequence of transvections with unique destination.

      A transvection sequence with unique destination is such that,
      for each source, there is a unique destination.

      \param ts %Transvection sequence
      \param n Dimension
      \param t Length of the sequence

      \pre n > 1
      \pre t >= 0
  */
  void ts_random_unique_destination(transvection_sequence_t& ts, int n, int t);

  /** Sample a random sequence of disjoint transvections.

      Two transvections \f$\tau_{ij}\f$ and \f$\tau_{kl}\f$ are said
      to be disjoint if the pairs {i,j} and {k,l} are disjoint.

      If 2t>n then the sequence length is set to the largest t such
      that 2t<=n.

      \param ts %Transvection sequence
      \param n Dimension
      \param t Length of the sequence

      \pre n > 1
      \pre t >= 0
  */
  void ts_random_disjoint(transvection_sequence_t& ts, int n, int t);

  /** Sample a random sequence of non commuting transvections.

      This function ensures that two consecutive transvections do not
      commute.

      \param ts %Transvection sequence
      \param n Dimension
      \param t Length of the sequence

      \pre n > 1
      \pre t >= 0
  */
  void ts_random_non_commuting(transvection_sequence_t& ts, int n, int t);


  /** Multiply a vector by a transvection sequence from the left.

      \param ts %Transvection sequence
      \param x Bit vector

      \pre ts_is_valid(ts)
      \pre ts_is_valid(ts, x.size())

      \warning This function modifies the given bit vector.
  */
  void ts_multiply(const transvection_sequence_t& ts, bit_vector_t& x);


  /** Multiply a matrix by a transvection sequence from the left.

      \param ts %Transvection sequence
      \param M Bit matrix

      \pre ts_is_valid(ts)
      \pre ts_is_valid(ts, bm_num_rows(M))

      \warning This function modifies the given bit vector.
  */
  void ts_multiply(const transvection_sequence_t& ts, bit_matrix_t& M);

  ///@}

} // end of namespace hnco


#endif
