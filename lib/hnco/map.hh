/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#include <iostream>             // std::ostream
#include <fstream>
#include <vector>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "bit-matrix.hh"
#include "bit-vector.hh"
#include "permutation.hh"
#include "transvection.hh"
#include "exception.hh"


namespace hnco {

/// %Map
class Map {

public:

  /// Destructor
  virtual ~Map() {}

  /// %Map
  virtual void map(const bit_vector_t& input, bit_vector_t& output) = 0;

  /// Get input size
  virtual int get_input_size() = 0;

  /// Get output size
  virtual int get_output_size() = 0;

  /** Check for surjective map.

      \return false
  */
  virtual bool is_surjective() { return false; }

  /// Display
  virtual void display(std::ostream& stream) {}

  /// Display to standard output
  virtual void display() { display(std::cout); }

};


/** %Translation.

    A translation is an affine map f from \f$F_2y^n\f$ to itself
    defined by \f$f(x) = x + b\f$, where b is an n-dimensional bit
    vector.
*/
class Translation: public Map {

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

  /// %Translation vector
  bit_vector_t _bv;

public:

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output) override;

  /// Get input size
  int get_input_size() override { return _bv.size(); }

  /// Get output size
  int get_output_size() override { return _bv.size(); }

  /** Check for surjective map.

      \return true
  */
  bool is_surjective() override { return true; }

  /// Display
  void display(std::ostream& stream) override {
    const int length = 5;
    stream << "Translation(" << _bv.size() << "): ";
    int bound = std::min(length, int(_bv.size()));
    for (int i = 0; i < bound; i++)
      if (_bv[i])
        stream << 1;
      else
        stream << 0;
    if (length < int(_bv.size()))
      stream << "...";
    stream << std::endl;
  }

  /// Random instance
  void random(int n) {
    assert(n > 0);

    _bv.resize(n);
    bv_random(_bv);
  }

  /// Set the translation vector
  void set_bv(const bit_vector_t& bv) { _bv = bv; }

  /** @name Load and save map
   */
  ///@{

  /** Load map.

      \param path Path of the map to load
      \throw std::runtime_error
  */
  void load(std::string path) {
    std::ifstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Translation::load: Cannot open " + path);
    try {
      boost::archive::text_iarchive archive(stream);
      archive >> (*this);
    }
    catch (boost::archive::archive_exception& e) {
      throw std::runtime_error("Translation::load: " + std::string(e.what()));
    }
  }

  /** Save map.

      \param path Path of the map to save
      \throw std::runtime_error
  */
  void save(std::string path) const {
    std::ofstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Translation::save: Cannot open " + path);
    try {
      boost::archive::text_oarchive archive(stream);
      archive << (*this);
    }
    catch (boost::archive::archive_exception& e) {
      throw std::runtime_error("Translation::save: " + std::string(e.what()));
    }
  }

  ///@}

};

  
/** %Permutation.

    A permutation is a linear map f from \f$F_2^n\f$ to itself
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

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output);

  /// Get input size
  int get_input_size() { return _permutation.size(); }

  /// Get output size
  int get_output_size() { return _permutation.size(); }

  /** Check for surjective map.

      \return true
  */
  bool is_surjective() { return true; }

};


/** Linear map.

    A linear map f from \f$F_2^m\f$ to \f$F_2^n\f$ is defined by
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

      \throw std::runtime_error
  */
  void random(int rows, int cols, bool surjective);

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output);

  /// Get input size
  int get_input_size() { return bm_num_columns(_bm); }

  /// Get output size
  int get_output_size() { return bm_num_rows(_bm); }

  /** Check for surjective map.

      \return true if rank(_bm) == bm_num_rows(_bm)
  */
  bool is_surjective();

};


/** Affine map.

    An affine map f from \f$F_2^m\f$ to \f$F_2^n\f$ is defined by
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
    assert(bm_num_rows(_bm) == int(_bv.size()));
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()

  /// Bit matrix
  bit_matrix_t _bm;

  /// %Translation vector
  bit_vector_t _bv;

public:

  /** Random instance.

      \param rows Number of rows
      \param cols Number of columns
      \param surjective Flag to ensure a surjective map

      \throw std::runtime_error
  */
  void random(int rows, int cols, bool surjective);

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output);

  /// Get input size
  int get_input_size() { return bm_num_columns(_bm); }

  /// Get output size
  int get_output_size() { return bm_num_rows(_bm); }

  /** Check for surjective map.

      \return true if rank(_bm) == bm_num_rows(_bm)
  */
  bool is_surjective();

};


/** %Map composition.

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

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output) {
    _inner->map(input, _bv);
    _outer->map(_bv, output);
  }

  /// Get input size
  int get_input_size() { return _inner->get_input_size(); }

  /// Get output size
  int get_output_size() { return _outer->get_output_size(); }

  /** Check for surjective map.

      \return true if both maps are surjective
  */
  bool is_surjective() { return _outer->is_surjective() && _inner->is_surjective(); }

};


/** %Injection.

    An injection copies the bits of input x to given positions of
    output y.

    Let \f$I = \{i_1, i_2, \ldots, i_m\}\f$ be a subset of \f$\{1,
    2, \ldots, n\}\f$.

    An injection f from \f$F_2^m\f$ to \f$F_2^n\f$, where \f$n \ge
    m\f$, is defined by \f$f(x) = y\f$, where, for all \f$j\in \{1,
    2, \ldots, m\}\f$, \f$y_{i_j} = x_j\f$.

    If f is a projection and g is an injection with the same bit
    positions then their composition \f$f\circ g\f$ is the identity.

*/
class Injection:
    public Map {

  /// Bit positions
  std::vector<int> _bit_positions;

  /// Output size
  int _output_size;

public:

  /** Constructor.

      The input size of the map is given by the size of
      bit_positions.

      \param bit_positions Bit positions in the output to where
      input bits are copied

      \param output_size Output size

      \pre output_size >= bit_positions.size()
  */
  Injection(const std::vector<int>& bit_positions, int output_size);

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output);

  /// Get input size
  int get_input_size() { return _bit_positions.size(); }

  /// Get output size
  int get_output_size() { return _output_size; }

  /// Check for surjective map
  bool is_surjective() { return get_output_size() == get_input_size(); }

};


/** %Projection.

    The projection y of a bit vector x is x where we have dropped a
    given set of components.

    Let \f$I = \{i_1, i_2, \ldots, i_m\}\f$ be a subset of \f$\{1,
    2, \ldots, n\}\f$.

    A projection f from \f$F_2^n\f$ to \f$F_2^m\f$, where \f$n \ge
    m\f$, is defined by \f$f(x) = y\f$, where, for all \f$j\in \{1,
    2, \ldots, m\}\f$, \f$y_j = x_{i_j}\f$.

    If f is a projection and g is an injection with the same bit
    positions then their composition \f$f\circ g\f$ is the identity.

*/
class Projection:
    public Map {

  /// Bit positions
  std::vector<int> _bit_positions;

  /// Input size
  int _input_size;

public:

  /** Constructor.

      The output size of the map is given by the size of
      bit_positions.

      \param bit_positions Bit positions in the input from where
      output bits are copied

      \param input_size Input size

      \pre input_size >= bit_positions.size()
  */
  Projection(const std::vector<int>& bit_positions, int input_size);

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output);

  /// Get input size
  int get_input_size() { return _input_size; }

  /// Get output size
  int get_output_size() { return _bit_positions.size(); }

  /** Check for surjective map.

      \return true
  */
  bool is_surjective() { return true; }

};


/** %Transvection sequence affine map.

    An affine map f from \f$F_2^m\f$ to \f$F_2^n\f$ is defined by
    \f$f(x) = Ax + b\f$, where A is an n x m bit matrix and b is an
    n-dimensional bit vector.

    In TsAffineMap, A is a finite product of transvections
    represented by a transvection_sequence_t.
*/
class TsAffineMap: public Map {

private:

  friend class boost::serialization::access;

  /// Save
  template<class Archive>
  void save(Archive& ar, const unsigned int version) const
  {
    ar & _bv;
    ar & _ts;
  }

  /// Load
  template<class Archive>
  void load(Archive& ar, const unsigned int version)
  {
    ar & _bv;
    ar & _ts;

    assert(bv_is_valid(_bv));
    assert(ts_is_valid(_ts, _bv.size()));
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()

  /// %Transvection sequence
  transvection_sequence_t _ts;

  /// %Translation vector
  bit_vector_t _bv;

public:

  /// Sampling mode
  enum SamplingMode {

                     /// Unconstrained
                     Unconstrained,

                     /// Commuting transvections
                     CommutingTransvections,

                     /// %Transvection sequence with unique source
                     UniqueSource,

                     /// %Transvection sequence with unique destination
                     UniqueDestination,

                     /// Disjoint transvections
                     DisjointTransvections,

                     /// Non commuting transvections
                     NonCommutingTransvections

  };

  /** Random instance.

      \param n Dimension
      \param t Length of sequence of transvections
      \param mode Sampling mode
  */
  void random(int n, int t, SamplingMode mode);

  /// %Map
  void map(const bit_vector_t& input, bit_vector_t& output);

  /// Get input size
  int get_input_size() { return _bv.size(); }

  /// Get output size
  int get_output_size() { return _bv.size(); }

  /** Check for surjective map.

      \return true
  */
  bool is_surjective() { return true; }

  /// Display
  void display(std::ostream& stream);

};


} // end of namespace hnco

#endif
