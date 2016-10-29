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

#ifndef HNCO_ALGORITHMS_UTIL_H
#define HNCO_ALGORITHMS_UTIL_H

#include <math.h>               // exp


namespace hnco {
namespace algorithm {

  /// Generic square function
  template<class T>
  T square(T x) { return x * x; }

  /// Logistic function (sigmoid)
  inline double logistic(double x)
  {
    if (x > 0)
      return 1 / (1 + exp(-x));
    else {
      double tmp = exp(x);
      return tmp / (1 + tmp);
    }
  }

} // end of namespace algorithm
} // end of namespace hnco


#endif
