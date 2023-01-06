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

#ifndef HNCO_FUNCTIONS_COLLECTION_NK_LANDSCAPE_H
#define HNCO_FUNCTIONS_COLLECTION_NK_LANDSCAPE_H

#include <iostream>
#include <vector>

#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"
#include "hnco/serialization.hh"


namespace hnco {
namespace function {


/** NK landscape.

    Reference:

    S. A. Kauffman. 1993. The origins of order: self-organisation
    and selection in evolution. Oxford University Press.

*/
class NkLandscape: public Function {

private:

  friend class boost::serialization::access;

  /// Serialize
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & _neighbors;
    ar & _partial_functions;
  }

  /// Bit neighbors
  std::vector<std::vector<int> > _neighbors;

  /// Partial functions
  std::vector<std::vector<double> > _partial_functions;

  /** Random structue.

      \param n Size of bit vector
      \param k Number of neighbors per bit
  */
  void random_structure(int n, int k);

public:

  /// Default constructor
  NkLandscape() {}


  /** @name Instance generators
   */
  ///@{

  /** Instance generator.

      \param n Size of bit vector
      \param k Number of neighbors per bit
      \param generator Generator for partial function values
  */
  template<class Generator>
  void generate(int n, int k, Generator generator) {
    assert(n > 0);
    assert(k > 0);

    random_structure(n, k);
    for (size_t i = 0; i < _partial_functions.size(); i++)
      for (size_t j = 0; j < _partial_functions[i].size(); j++)
        _partial_functions[i][j] = generator();
  }

  /** Random instance.

      Partial function values are sampled from the normal
      distribution.

      \param n Size of bit vector
      \param k Number of neighbors per bit
  */
  void random(int n, int k) {
    assert(n > 0);
    assert(k > 0);

    generate(n, k, hnco::random::Generator::normal);
  }

  ///@}


  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) { load_from_archive(*this, path, "NkLandscape"); }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const { save_to_archive(*this, path, "NkLandscape"); }

  ///@}



  /// Get bit vector size
  int get_bv_size() const override { return _partial_functions.size(); }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /// Display
  void display(std::ostream& stream) const override;

};


} // end of namespace function
} // end of namespace hnco


#endif
