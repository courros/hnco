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

#ifndef HNCO_RANDOM_NUMBER_H
#define HNCO_RANDOM_NUMBER_H

#include <random>


namespace hnco {
/// Pseudo random numbers
namespace random {


  /// Random numbers
  struct Random {

    /// Mersenne Twister 19937 generator
    static std::mt19937 generator;

    /// Next uniformly distributed sample
    static double uniform() {
      std::uniform_real_distribution<double> dist;
      return dist(generator);
    }

    /// Next normally distributed sample
    static double normal() {
      std::normal_distribution<double> dist;
      return dist(generator);
    }

    /// Next random bit
    static bool bernoulli() {
      std::bernoulli_distribution dist;
      return dist(generator);
    }

  };


} // end of namespace random
} // end of namespace hnco


#endif
