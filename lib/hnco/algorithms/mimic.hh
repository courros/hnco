/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_MIMIC_H
#define HNCO_ALGORITHMS_MIMIC_H

#include <array>
#include <vector>

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/exception.hh"
#include "hnco/permutation.hh"


namespace hnco {
namespace algorithm {

/** Mutual information maximizing input clustering.

    This implementation differs from the algorithm described in the
    reference below in that it constrains all probabilities
    (marginal and conditional) to stay away from the values 0 and 1
    by a fixed margin equal to 1 / n, as usually done in algorithms
    such as Pbil or Umda.

    Reference:

    Jeremy S. De Bonet and Charles L. Isbell and Jr. and Paul Viola,
    MIMIC: Finding Optima by Estimating Probability Densities, in
    Advances in Neural Information Processing Systems, 1996, MIT
    Press.

*/
class Mimic: public IterativeAlgorithm {

protected:

  /// Population
  Population _population;

  /// Permutation
  permutation_t _permutation;

  /// Model parameters
  std::array<pv_t, 2> _parameters;

  /// Mean of selected bit vectors
  pv_t _mean;

  /// Conditional entropies
  std::vector<double> _entropies;

  /// Contingency table
  std::array<std::array<int, 2>, 2> _table;

  /// Lower bound of probability
  double _lower_bound;

  /// Upper bound of probability
  double _upper_bound;

  ///
  /** @name Parameters
   */
  ///@{

  /// Selection size
  int _selection_size;

  ///@}


  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  ///@}


  /// Sample a bit vector
  void sample(bit_vector_t& bv);

  /// Compute conditional entropy
  void compute_conditional_entropy(int index);

  /// Update model
  void update_model();

public:

  /// Constructor
  Mimic(int n, int population_size):
    IterativeAlgorithm(n),
    _population(population_size, n),
    _permutation(n),
    _mean(n),
    _entropies(n),
    _lower_bound(1 / double(n)),
    _upper_bound(1 - 1 / double(n)),
    _selection_size(population_size / 2)
  {
    if (!(population_size > 1))
      throw std::runtime_error("Mimic::Mimic: population size must be > 1");

    for (auto& p: _parameters)
      p = pv_t(n);
  }

  /** @name Setters
   */
  ///@{

  /// Set the selection size
  void set_selection_size(int selection_size) { _selection_size = selection_size; }

  ///@}

};

} // end of namespace algorithm
} // end of namespace hnco


#endif
