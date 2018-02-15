/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_LINEAR_FUNCTION_H
#define HNCO_FUNCTIONS_LINEAR_FUNCTION_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "function.hh"


namespace hnco {
namespace function {


  /// Linear function
  class LinearFunction:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _weights;
    }

    /// Weights
    std::vector<double> _weights;

  public:

    /// Constructor
    LinearFunction() {}

    /** Random instance.

        \param n Size of bit vectors
    */
    void random(int n);

    /// Get bit vector size
    size_t get_bv_size() { return _weights.size(); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum();

  };


} // end of namespace function
} // end of namespace hnco


#endif
