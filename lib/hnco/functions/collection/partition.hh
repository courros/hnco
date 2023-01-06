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

#ifndef HNCO_FUNCTIONS_COLLECTION_PARTITION_H
#define HNCO_FUNCTIONS_COLLECTION_PARTITION_H

#include <assert.h>

#include <iostream>
#include <vector>

#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"
#include "hnco/serialization.hh"


namespace hnco {
namespace function {


/** %Partition.

    %Partition a finite multiset of positive integers into two
    subsets such that the sum of numbers in the first subset is the
    closest to the sum of numbers in the second subset.

    The function computes the negation of the distance between the
    sum of numbers corresponding to ones in the bit vector and the
    sum of those corresponding to zeros. The negation is a
    consequence of the fact that algorithms in HNCO maximize rather
    than minimize a function.

*/
class Partition: public Function {

private:

  friend class boost::serialization::access;

  /// Serialize
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & _numbers;
  }

  /// Multiset of positive integers
  std::vector<int> _numbers;

public:

  /// Constructor
  Partition() {}


  /** @name Instance generators
   */
  ///@{

  /** Instance generator.

      \param n Size of bit vectors
      \param generator Number generator
  */
  template<class Generator>
  void generate(int n, Generator generator) {
    assert(n > 0);

    _numbers.resize(n);
    for (size_t i = 0; i < _numbers.size(); i++)
      _numbers[i] = generator();
  }

  /** Random instance.

      The numbers are sampled from the uniform distribution on
      [1..upper_bound].

      \param n Size of bit vector
      \param upper_bound Upper bound of positive integers
  */
  void random(int n, int upper_bound) {
    assert(n > 0);
    assert(upper_bound > 0);

    std::uniform_int_distribution<int> dist(1, upper_bound);
    generate(n, [&dist]() { return dist(hnco::random::Generator::engine); });
  }

  ///@}


  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) { load_from_archive(*this, path, "Partition"); }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const { save_to_archive(*this, path, "Partition"); }

  ///@}


  /// Get bit vector size
  int get_bv_size() const override { return _numbers.size(); }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;


  /** @name Display
   */
  ///@{

  /// Display
  void display(std::ostream& stream) const override;

  /// Describe a bit vector
  void describe(const bit_vector_t& x, std::ostream& stream) override;

  ///@}

};


} // end of namespace function
} // end of namespace hnco


#endif
