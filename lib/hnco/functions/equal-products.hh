/* Copyright (C) 2016 Arnaud Berny

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

      Source: technical report by Larranaga et al.
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

    /// Get bit vector size
    size_t get_bv_size() { return _numbers.size(); }

    /** Random instance.

        \param n Size of bit vector
        \param upper_bound Upper bound of numbers
    */
    void random(int n, double upper_bound);

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
