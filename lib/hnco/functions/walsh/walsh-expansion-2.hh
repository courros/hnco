/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_2_H
#define HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_2_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


  /** Walsh expansion of degree 2.

      Its expression is of the form

      \f$ f(x) = \sum_i a_i (1 - 2x_i) + \sum_{i < j} a_{ij} (1 - 2x_i) (1 - 2x_j) \f$

      or equivalently

      \f$ f(x) = \sum_i a_i (-1)^{x_i} + \sum_{i < j} a_{ij} (-1)^{x_i + x_j} \f$

      where the sum \f$ x_i + x_j \f$ is mod 2 (xor).

  */
  class WalshExpansion2:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _linear;
      ar & _quadratic;
    }

    /// Linear part
    std::vector<double> _linear;

    /** Quadratic part.

        Represented as a lower triangular matrix (without its
        diagonal).
    */
    std::vector<std::vector<double> > _quadratic;

  public:

    /// Constructor
    WalshExpansion2() {}

    /// Get bit vector size
    size_t get_bv_size() { return _quadratic.size(); }

    /** Random instance.

        \param n                Size of bit vector
        \param stddev_lin       Standard deviation of the coefficients of the linear part
        \param stddev_quad      Standard deviation of the coefficients of the quadratic part
    */
    void random(int n, double stddev_lin, double stddev_quad);

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
