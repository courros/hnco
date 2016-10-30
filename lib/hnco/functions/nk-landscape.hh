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

#ifndef HNCO_FUNCTIONS_NK_LANDSCAPE_H
#define HNCO_FUNCTIONS_NK_LANDSCAPE_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "function.hh"


namespace hnco {
namespace function {


  /** NK landscape.

      Source: Kauffman
  */
  class NkLandscape:
    public Function {

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

  public:

    /// Default constructor
    NkLandscape() {}

    /// Get bit vector size
    size_t get_bv_size() { return _partial_functions.size(); }

    /** Random instance.

        \param n Size of bit vector
        \param k Number of neighbors of each bit
        \param stddev Standard deviation of the values of the partial
        functions
    */
    void random(int n, int k, double stddev);

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
