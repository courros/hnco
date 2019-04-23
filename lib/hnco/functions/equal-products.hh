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

#ifndef HNCO_FUNCTIONS_EQUAL_PRODUCTS_H
#define HNCO_FUNCTIONS_EQUAL_PRODUCTS_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "function.hh"


namespace hnco {
namespace function {


  /** Equal products.

      Partition a finite set of positive numbers into two subsets such
      that the product of numbers in the first subset is the closest
      to the product of numbers in the second subset. This is
      equivalent to the partition problem applied to the logarithms of
      the given numbers.

      The function computes the negation of the distance between the
      product of numbers corresponding to ones in the bit vector and
      the product of those corresponding to zeros. The negation is a
      consequence of the fact that algorithms in HNCO maximize rather
      than minimize a function.

      Reference:

      S. Baluja and S. Davies. 1997. Using optimal dependency-trees
      for combinatorial optimization: learning the structure of the
      search space. Technical Report CMU- CS-97-107. Carnegie-Mellon
      University.

  */
  class EqualProducts:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _numbers;
    }

    /// Numbers
    std::vector<double> _numbers;

  public:

    /// Constructor
    EqualProducts() {}


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

        The weights are sampled from the uniform distribution on
        [0,1).

        \param n Size of bit vector
    */
    void random(int n) {
      assert(n > 0);

      generate(n, hnco::random::Random::uniform);
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
