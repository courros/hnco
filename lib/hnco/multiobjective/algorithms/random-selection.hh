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

#include "candidate-set.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


class TournamentSelection {
  CandidateSet& _candidate_set;
public:
  TournamentSelection(CandidateSet& candidate_set)
    : _candidate_set(candidate_set)
  {}
  void init() {}
  const bit_vector_t& select() {
    int winner = 0;
    return _candidate_set.bvs[winner];
  }
};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
