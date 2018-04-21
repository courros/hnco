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

#ifndef HNCO_ALGORITHMS_EA_TOURNAMENT_SELECTION_H
#define HNCO_ALGORITHMS_EA_TOURNAMENT_SELECTION_H

#include "hnco/functions/function.hh"
#include "hnco/random.hh"
#include "hnco/algorithms/population.hh"


namespace hnco {
namespace algorithm {


  /// Population with tournament selection
  class TournamentSelection:
    public Population
  {

    /// Random index
    std::uniform_int_distribution<int> _choose_individual;

    /** @name Parameters
     */
    ///@{

    /// Tournament size
    int _tournament_size = 10;

    ///@}

  public:

    /// Constructor
    TournamentSelection(int population_size, int n):
      Population(population_size, n),
      _choose_individual(0, population_size - 1) {}

    /** Selection.

        The selection only requires that the population be evaluated,
        not necessarily sorted.

        \pre The population must be evaluated.
    */
    const bit_vector_t& select();

    /** @name Setters
     */
    ///@{

    /// Set the tournament size
    void set_tournament_size(int x) { _tournament_size = x; }

    ///@}

  };


}
}


#endif
