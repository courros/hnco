/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#include "pv-algorithm.hh"


namespace hnco {
namespace algorithm {


/** Compact genetic algorithm.

    Reference:

    Georges R. Harik, Fernando G. Lobo, and David
    E. Goldberg. 1999. The Compact Genetic %Algorithm. IEEE
    Trans. on Evolutionary Computation 3, 4 (November 1999),
    287–297.

*/
class CompactGa: public PvAlgorithm {

protected:

  /// Candidates
  std::vector<bit_vector_t> _candidates;

  /** @name Parameters
   */
  ///@{

  /// Learning rate
  double _learning_rate = 1e-3;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  ///@}

public:
  /// Constructor
  CompactGa(int n):
    PvAlgorithm(n),
    _candidates(2, bit_vector_t(n)) {}

  /** @name Setters
   */
  ///@{

  /// Set the learning rate
  void set_learning_rate(double x) { _learning_rate = x; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
