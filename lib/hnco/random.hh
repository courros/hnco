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

#ifndef HNCO_RANDOM_H
#define HNCO_RANDOM_H

#include <random>


namespace hnco {
/// Random numbers
namespace random {


  /// Random number generator
  struct Generator {

    /// Mersenne Twister engine
    static std::mt19937 engine;

    static unsigned seed;
    static void set_seed(unsigned n);
    static void set_seed();
    static void reset();

    /// Sample random number with uniform distribution
    static double uniform();

    /// Sample random number with normal distribution
    static double normal();

    /// Sample random number with Bernoulli distribution
    static bool bernoulli();

  };


} // end of namespace random
} // end of namespace hnco


#endif
