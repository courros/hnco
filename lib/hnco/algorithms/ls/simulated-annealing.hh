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

#ifndef HNCO_ALGORITHMS_LS_SIMULATED_ANNEALING_H
#define HNCO_ALGORITHMS_LS_SIMULATED_ANNEALING_H

#include <assert.h>

#include "hnco/algorithms/algorithm.hh"

#include "hnco/neighborhoods/neighborhood.hh"


namespace hnco {
namespace algorithm {


  /// Simulated annealing
  class SimulatedAnnealing:
    public IterativeAlgorithm {

  private:

    /// Neighborhood
    neighborhood::Neighborhood *_neighborhood;

    /// Inverse temperature
    double _beta;

    /// Current value
    double _current_value;

    /// Number of accepted transitions
    int _transitions;

    /** Set beta.

        Requires (2 * num_trials) evaluations. This should be taken
        into account when using OnBudgetFunction.

    */
    void set_beta();

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    SimulatedAnnealing(int n, neighborhood::Neighborhood *neighborhood):
      IterativeAlgorithm(n),
      _neighborhood(neighborhood)
    {
      assert(neighborhood);
    }

    /** @name Parameters
     */
    ///@{

    /// Number of accepted transitions before annealing
    int _num_transitions = 50;

    /// Number of trials.
    int _num_trials = 100;

    /// Initial acceptance probability
    double _initial_acceptance_probability = 0.6;

    /// Increase rate for inverse temperature
    double _rate = 1.2;

    ///@}

    /// Initialization
    void init();

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
