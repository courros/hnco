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

#ifndef HNCO_ALGORITHMS_PV_PBIL_H
#define HNCO_ALGORITHMS_PV_PBIL_H

#include "hnco/algorithms/population.hh"

#include "pv-algorithm.hh"


namespace hnco {
namespace algorithm {


  /// Population-Based Incremental Learning
  class Pbil:
    public PvAlgorithm {

  protected:

    /// Population
    Population _population;

    /// Mean of selected bit vectors
    pv_t _mean;

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    Pbil(int n, int population_size):
      PvAlgorithm(n),
      _population(population_size, n),
      _mean(n) {}

    /** @name Parameters
     */
    ///@{

    /// Selection size
    int _selection_size = 1;

    /// Learning rate
    double _rate = 1e-3;

    ///@}

    /// Initialization
    void init();

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
