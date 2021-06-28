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

#ifndef HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_1_H
#define HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_1_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** Walsh expansion of degree 1.

    Its expression is of the form

    \f$ f(x) = \sum_i a_i (1 - 2x_i) \f$

    or equivalently

    \f$ f(x) = \sum_i a_i (-1)^{x_i} \f$

*/
class WalshExpansion1: public Function {

private:

  friend class boost::serialization::access;

  /// Serialize
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & _linear;
  }

  /// Linear part
  std::vector<double> _linear;

public:

  /// Constructor
  WalshExpansion1() {}


  /** @name Instance generators
   */
  ///@{

  /** Instance generator.

      \param n Size of bit vectors
      \param generator Weight generator
  */
  template<class Generator>
  void generate(int n, Generator generator) {
    assert(n > 0);

    _linear.resize(n);
    for (size_t i = 0; i < _linear.size(); i++)
      _linear[i] = generator();
  }

  /** Random instance.

      The weights are sampled from the normal distribution.

      \param n Size of bit vectors
  */
  void random(int n) {
    assert(n > 0);

    generate(n, hnco::random::Generator::normal);
  }

  ///@}


  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) {
    std::ifstream stream(path);
    if (!stream.good())
      throw std::runtime_error("WalshExpansion1::load: Cannot open " + path);
    try {
      boost::archive::text_iarchive archive(stream);
      archive >> (*this);
    }
    catch (boost::archive::archive_exception& e) {
      throw std::runtime_error("WalshExpansion1::load: " + std::string(e.what()));
    }
  }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const {
    std::ofstream stream(path);
    if (!stream.good())
      throw std::runtime_error("WalshExpansion1::save: Cannot open " + path);
    try {
      boost::archive::text_oarchive archive(stream);
      archive << (*this);
    }
    catch (boost::archive::archive_exception& e) {
      throw std::runtime_error("WalshExpansion1::save: " + std::string(e.what()));
    }
  }

  ///@}


  /** @name Evaluation
   */
  ///@{

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /// Incrementally evaluate a bit vector
  double evaluate_incrementally(const bit_vector_t& x, double v, const hnco::sparse_bit_vector_t& flipped_bits) override;

  ///@}

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() override { return _linear.size(); }

  /// Get the global maximum
  double get_maximum() override;

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() override { return true; }

  /** Check whether the function provides incremental evaluation.
      \return true
  */
  bool provides_incremental_evaluation() override { return true; }

  ///@}

};


} // end of namespace function
} // end of namespace hnco


#endif
