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

#ifndef HNCO_ALGORITHMS_PV_COMPACT_GA_H
#define HNCO_ALGORITHMS_PV_COMPACT_GA_H

#include <vector>

#include "pv-algorithm.hh"


namespace hnco {
namespace algorithm {


  /// Compact genetic algorithm
  class CompactGa:
    public PvAlgorithm {
  protected:

    /// Candidates
    std::vector<bit_vector_t> _candidates;

    /// Single iteration
    void iterate();

  public:
    /// Constructor
    CompactGa(int n):
      PvAlgorithm(n),
      _candidates(2, bit_vector_t(n)) {}

    /// Learning rate
    double _rate = 1e-3;

    /// Initialization
    void init();
  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
