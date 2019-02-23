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
  class WalshExpansion1:
    public Function {

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

    /// Get bit vector size
    size_t get_bv_size() { return _linear.size(); }

    /** Random instance.

        \param n                Size of bit vector
        \param stddev           Standard deviation of the coefficients
    */
    void random(int n, double stddev);

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
