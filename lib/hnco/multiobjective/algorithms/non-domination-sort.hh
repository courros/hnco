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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_NON_DOMINATION_SORT_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_NON_DOMINATION_SORT_H

#include "candidate-set.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/** Non domination sort from the NSGA-II paper.

 */
class Nsga2NonDominationSort {

  /// Candidate set
  CandidateSet& _candidate_set;

public:

  /// Constructor
  Nsga2NonDominationSort(CandidateSet& candidate_set)
    : _candidate_set(candidate_set)
  {}

  /// Sort
  void sort() {
  }

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
