/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_WALSH_TERM_H
#define HNCO_FUNCTIONS_WALSH_TERM_H

#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>


namespace hnco {
namespace function {

  /// Walsh transform term
  struct WalshTerm {

    /** Feature.

        Implemented with a vector bool instead of a bit_vector_t to
        reduce the memory consumption.
    */
    std::vector<bool> feature;

    /// Coefficient
    double coefficient;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & feature;
      ar & coefficient;
    }

  };

} // end of namespace function
} // end of namespace hnco


#endif
