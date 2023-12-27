/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_H
#define HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_H

#include <assert.h>

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/serialization/vector.hpp>

#include "hnco/bit-matrix.hh"
#include "hnco/functions/function.hh"
#include "hnco/serialization.hh"


namespace hnco {
namespace function {


/** Walsh expansion.

    Its expression is of the form

    \f$ f(x) = \sum_u a_u (-1)^{x \cdot u} \f$

    where the sum is over a subset of \f$ \{0, 1\}^n \f$ and \f$ x
    \cdot u = \sum_i x_i u_i \f$ is mod 2. The real numbers \f$ a_u
    \f$ are the coefficients of the expansion and the bit vectors
    \f$ u \f$ are its feature vectors.
*/
class WalshExpansion: public Function {

private:

  friend class boost::serialization::access;

  /// Save
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & _terms;
  }

  /// Terms
  std::vector<function::WalshTerm> _terms;

public:

  /// Constructor
  WalshExpansion() {}


  /** @name Instance generators
   */
  ///@{

  /** Instance generator.

      \param n Size of bit vectors
      \param num_features Number of feature vectors
      \param generator Coefficient generator
  */
  template<class Generator>
  void generate(int n, int num_features, Generator generator) {
    assert(n > 0);
    assert(num_features > 0);

    bit_vector_t bv(n);
    std::vector<bool> feature(n);

    _terms.resize(num_features);

    for (size_t i = 0; i < _terms.size(); i++) {
      WalshTerm& t = _terms[i];
      bv_random(bv);
      bv_to_vector_bool(feature, bv);
      t.feature = feature;
      t.coefficient = generator();
    }

  }

  /** Random instance.

      The coefficients are sampled from the normal distribution.

      \param n Size of bit vector
      \param num_features Number of feature vectors
  */
  void random(int n, int num_features) {
    assert(n > 0);
    assert(num_features > 0);

    generate(n, num_features, hnco::random::Generator::normal);
  }

  ///@}


  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) { load_from_archive(*this, path, "WalshExpansion"); }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const { save_to_archive(*this, path, "WalshExpansion"); }

  ///@}


  /// Get bit vector size
  int get_bv_size() const override {
    assert(_terms.size() > 0);
    return _terms[0].feature.size();
  }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /// Display
  void display(std::ostream& stream) const override;

  /// Set terms
  void set_terms(const std::vector<function::WalshTerm> terms) { _terms = terms; }

};


} // end of namespace function
} // end of namespace hnco


#endif
