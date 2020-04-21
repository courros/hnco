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

#ifndef HNCO_ALGORITHMS_PV_PBIL_H
#define HNCO_ALGORITHMS_PV_PBIL_H

#include "hnco/algorithms/population.hh"

#include "pv-algorithm.hh"


namespace hnco {
namespace algorithm {

  /** Population-based incremental learning.

      Reference:

      S. Baluja and R. Caruana. 1995. Removing the genetics from the
      standard genetic algorithm. In Proceedings of the 12th Annual
      Conference on Machine Learning.  38–46.

  */
  class Pbil:
    public PvAlgorithm {

  protected:

    /// Population
    Population _population;

    /// Mean of selected bit vectors
    pv_t _mean;

    /** @name Parameters
     */
    ///@{

    /// Selection size
    int _selection_size = 1;

    /// Learning rate
    double _learning_rate = 1e-3;

    ///@}

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    Pbil(int n, int population_size):
      PvAlgorithm(n),
      _population(population_size, n),
      _mean(n) {}

    /// Initialization
    void init();

    /** @name Setters
     */
    ///@{

    /// Set the selection size
    void set_selection_size(int x) { _selection_size = x; }

    /// Set the learning rate
    void set_learning_rate(double x) { _learning_rate = x; }

    ///@}

  };

} // end of namespace algorithm
} // end of namespace hnco


#endif
