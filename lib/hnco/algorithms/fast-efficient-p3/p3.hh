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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_P3_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_P3_H

#include "hnco/algorithms/algorithm.hh"


namespace hnco::algorithm::fast_efficient_p3 {

  /** Parameter-less %Population Pyramid.

      Implemention of the Parameter-less %Population Pyramid (P3 for
      short).

      Author: Brian W. Goldman

      Reference:

      "Fast and Efficient Black Box Optimization using the
      Parameter-less Population Pyramid" by B. W. Goldman and
      W. F. Punch

      Integrated into HNCO by Arnaud Berny
  */
  class ParameterlessPopulationPyramid:
    public Algorithm {

  public:

    /// Constructor
    ParameterlessPopulationPyramid(int n):
      Algorithm(n) {}

    /// Maximize
    void maximize();

  };

} // end of namespace hnco::algorithm::fast_efficient_p3


#endif
