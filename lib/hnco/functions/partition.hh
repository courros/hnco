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

#ifndef HNCO_FUNCTIONS_PARTITION_H
#define HNCO_FUNCTIONS_PARTITION_H

#include <assert.h>
#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "function.hh"


namespace hnco {
namespace function {


  /** Partition.

      Partition a finite multiset of positive integers into two
      subsets such that the sum of numbers in the first subset is the
      closest to the sum of numbers in the second subset.

      The function computes the negation of the distance between the
      sum of numbers corresponding to ones in the bit vector and the
      sum of those corresponding to zeros. The negation is a
      consequence of the fact that algorithms in HNCO maximize rather
      than minimize a function.

  */
  class Partition:
    public Function {

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
        [1..bound].

        \param n Size of bit vector
        \param upper_bound Upper bound of positive integers
    */
    void random(int n, int bound) {
      assert(n > 0);
      assert(bound > 0);

      std::uniform_int_distribution<int> dist(1, bound);
      generate(n, [&dist]() { return dist(hnco::random::Random::generator); });
    }

    ///@}


    /// Get bit vector size
    int get_bv_size() { return _numbers.size(); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
