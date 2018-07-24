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

#ifndef HNCO_ALGORITHMS_EDA_MIMIC_H
#define HNCO_ALGORITHMS_EDA_MIMIC_H

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/population.hh"


namespace hnco::algorithm::eda {

  /** Mutual information maximizing input clustering.

      Reference:

  */
  class Mimic:
    public IterativeAlgorithm {

  protected:

    /// Population
    Population _population;

    /** @name Parameters
     */
    ///@{

    /// Selection size
    int _selection_size = 1;

    ///@}

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    Mimic(int n, int population_size):
      IterativeAlgorithm(n),
      _population(population_size, n) {}

    /// Initialization
    void init();

    /** @name Setters
     */
    ///@{

    /// Set the selection size
    void set_selection_size(int x) { _selection_size = x; }

    ///@}

  };

} // end of namespace hnco::algorithm


#endif
