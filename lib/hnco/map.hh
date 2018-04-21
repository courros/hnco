/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_MAP_H
#define HNCO_MAP_H

#include <fstream>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "bit-matrix.hh"
#include "bit-vector.hh"
#include "permutation.hh"


namespace hnco {

  /// Map
  class Map {

  public:

    /// Destructor
    virtual ~Map() {}

    /// Map
    virtual void map(const bit_vector_t& input, bit_vector_t& output) = 0;

    /// Get input size
    virtual size_t get_input_size() = 0;

    /// Get output size
    virtual size_t get_output_size() = 0;

    /** Check for surjective map.

        \return false
    */
    virtual bool is_surjective() { return false; }

  };


  /** Translation.

      A translation is an affine map f from \f$Z_2^n\f$ to itself
      defined by \f$f(x) = x + b\f$, where b is an n-dimensional bit
      vector.
  */
  class Translation:
    public Map {

  private:

    friend class boost::serialization::access;

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & _bv;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & _bv;

      assert(bv_is_valid(_bv));
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Translation vector
    bit_vector_t _bv;

  public:

    /// Random instance
    void random(int n) {
      assert(n > 0);

      _bv.resize(n);
      bv_random(_bv);
    }

    /// Map
    void map(const bit_vector_t& input, bit_vector_t& output);

    /// Get input size
    size_t get_input_size() { return _bv.size(); }

    /// Get output size
    size_t get_output_size() { return _bv.size(); }

    /** Check for surjective map.

        \return true
    */
    bool is_surjective() { return true; }

  };

  
  /** Permutation.

      A permutation is a linear map f from \f$Z_2^n\f$ to itself
      defined by \f$f(x) = y\f$, where \f$y_i = x_{\sigma_i}\f$ and
      \f$\sigma\f$ is a permutation of 0, 1, ..., n - 1.
  */
  class Permutation:
    public Map {

  private:

    friend class boost::serialization::access;

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & _permutation;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & _permutation;

      assert(perm_is_valid(_permutation));
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Permutation
    permutation_t _permutation;

  public:

    /// Random instance
    void random(int n) {
      assert(n > 0);

      _permutation.resize(n);
      perm_random(_permutation);
    }

    /// Map
    void map(const bit_vector_t& input, bit_vector_t& output);

    /// Get input size
    size_t get_input_size() { return _permutation.size(); }

    /// Get output size
    size_t get_output_size() { return _permutation.size(); }

    /** Check for surjective map.

        \return true
    */
    bool is_surjective() { return true; }

  };


  /** Linear map.

      A linear map f from \f$Z_2^m\f$ to \f$Z_2^n\f$ is defined by
      \f$f(x) = Ax\f$, where A is an n x m bit matrix.
  */
  class LinearMap:
    public Map {

  private:

    friend class boost::serialization::access;

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & _bm;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & _bm;

      assert(bm_is_valid(_bm));
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Bit matrix
    bit_matrix_t _bm;

  public:

    /** Random instance.

        \param rows Number of rows
        \param cols Number of columns
        \param surjective Flag to ensure a surjective map

        \throw Error
    */
    void random(int rows, int cols, bool surjective);

    /// Map
    void map(const bit_vector_t& input, bit_vector_t& output);

    /// Get input size
    size_t get_input_size() { return bm_num_columns(_bm); }

    /// Get output size
    size_t get_output_size() { return bm_num_rows(_bm); }

    /** Check for surjective map.

        \return true if rank(_bm) == bm_num_rows(_bm)
    */
    bool is_surjective();

  };


  /** Affine map.

      An affine map f from \f$Z_2^m\f$ to \f$Z_2^n\f$ is defined by
      \f$f(x) = Ax + b\f$, where A is an n x m bit matrix and b is an
      n-dimensional bit vector.
  */
  class AffineMap:
    public Map {

  private:

    friend class boost::serialization::access;

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & _bm;
      ar & _bv;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & _bm;
      ar & _bv;

      assert(bm_is_valid(_bm));
      assert(bv_is_valid(_bv));
      assert(bm_num_rows(_bm) == _bv.size());
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Bit matrix
    bit_matrix_t _bm;

    /// Translation vector
    bit_vector_t _bv;

  public:

    /** Random instance.

        \param rows Number of rows
        \param cols Number of columns
        \param surjective Flag to ensure a surjective map

        \throw Error
    */
    void random(int rows, int cols, bool surjective);

    /// Map
    void map(const bit_vector_t& input, bit_vector_t& output);

    /// Get input size
    size_t get_input_size() { return bm_num_columns(_bm); }

    /// Get output size
    size_t get_output_size() { return bm_num_rows(_bm); }

    /** Check for surjective map.

        \return true if rank(_bm) == bm_num_rows(_bm)
    */
    bool is_surjective();

  };


  /** Map composition.

      The resulting composition f is defined for all bit vector x by
      f(x) = outer(inner(x)).
   */
  class MapComposition:
    public Map {

    /// Outer map
    Map *_outer;

    /// Inner map
    Map *_inner;

    /// Temporary bit vector
    bit_vector_t _bv;

  public:

    /// Default constructor
    MapComposition() {}

    /** Constructor.

        \param outer outer map
        \param inner inner map

        \pre outer->get_input_size() == inner->get_output_size()
    */
    MapComposition(Map *outer, Map *inner):
      _outer(outer),
      _inner(inner)
    {
      assert(outer->get_input_size() == inner->get_output_size());
      _bv.resize(_inner->get_output_size());
    }

    /// Map
    void map(const bit_vector_t& input, bit_vector_t& output) {
      _inner->map(input, _bv);
      _outer->map(_bv, output);
    }

    /// Get input size
    size_t get_input_size() { return _inner->get_input_size(); }

    /// Get output size
    size_t get_output_size() { return _outer->get_output_size(); }

    /** Check for surjective map.

        \return true if both maps are surjective
    */
    bool is_surjective() { return _outer->is_surjective() && _inner->is_surjective(); }

  };


} // end of namespace hnco

#endif
