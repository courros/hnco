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

#ifndef HNCO_ALGORITHMS_EDA_P3_H
#define HNCO_ALGORITHMS_EDA_P3_H

#include <memory>               // std::shared_ptr

#include "hnco/algorithms/algorithm.hh"


namespace hnco {
namespace algorithm {
namespace fast_efficient_p3 {

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
  class ParameterLessPopulationPyramid:
    public Algorithm {

    /// Implementation structure
    struct Implementation;

    /// Pointer to implementation
    std::unique_ptr<Implementation> _pimpl;;

  public:

    /// Constructor
    ParameterLessPopulationPyramid(int n);

    /// Initialization
    void init();

    /// Maximize
    void maximize();

    /// Finalize
    void finalize();

  };

} // end of namespace eda
} // end of namespace algorithm
} // end of namespace hnco


#endif
