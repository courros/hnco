/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_RANDOM_SELECTION_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_RANDOM_SELECTION_H

#include "hnco/random.hh"       // hnco::random::Generator::engine

#include "candidate-set.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/** Tournament selection.

    The selection is biased towards bit vectors with small Pareto
    front (non dominated ones).
*/
class TournamentSelection {

  /// Candidate set
  CandidateSet& _candidate_set;

  /// Random index
  std::uniform_int_distribution<int> _choose_individual;

  /** @name Parameters
   */
  ///@{

  /// Tournament size
  int _tournament_size = 2;

  ///@}

public:

  /// Constructor
  TournamentSelection(CandidateSet& candidate_set)
    : _candidate_set(candidate_set)
    , _choose_individual(0, candidate_set.size() - 1)
  {}

  /// Initialize
  void init() {}

  /// Select a bit vector
  const bit_vector_t& select() {
    int winner = _choose_individual(random::Generator::engine);
    for (int i = 0; i < _tournament_size; i++) {
      int challenger;
      do {
        challenger = _choose_individual(random::Generator::engine);
      } while (challenger == winner);
      if (_candidate_set.pareto_fronts[challenger] < _candidate_set.pareto_fronts[winner])
        winner = challenger;
    }
    return _candidate_set.bvs[winner];
  }

  /** @name Setters
   */
  ///@{

  /// Set the tournament size
  void set_tournament_size(int n) { _tournament_size = n; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
