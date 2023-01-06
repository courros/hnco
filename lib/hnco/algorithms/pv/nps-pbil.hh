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

#ifndef HNCO_ALGORITHMS_PV_PBIL_NPS_H
#define HNCO_ALGORITHMS_PV_PBIL_NPS_H

#include "hnco/algorithms/population.hh"

#include "pv-algorithm.hh"


namespace hnco {
namespace algorithm {

/** Population-based incremental learning with negative and positive selection.

    Reference:

    Arnaud Berny. 2001. Extending selection learning toward
    fixed-length d-ary strings. In Artificial Evolution (Lecture
    Notes in Computer Science), P. Collet and others
    (Eds.). Springer, Le Creusot.

*/
class NpsPbil:
    public PvAlgorithm {

protected:

  /// Population
  Population _population;

  /// Mean of best individuals
  pv_t _mean_best;

  /// Mean of worst individuals
  pv_t _mean_worst;

  /** @name Parameters
   */
  ///@{

  /// Selection size
  int _selection_size = 1;

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
  NpsPbil(int n, int population_size):
    PvAlgorithm(n),
    _population(population_size, n),
    _mean_best(n),
    _mean_worst(n) {}

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
