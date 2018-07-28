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

#include <array>
#include <utility>              // std::pair
#include <vector>

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/exception.hh"
#include "hnco/permutation.hh"


namespace hnco::algorithm::eda {

  /** Mutual information maximizing input clustering.

      Reference:

  */
  class Mimic:
    public IterativeAlgorithm {

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

    /// Single iteration
    void iterate();

    /// Sample a bit vector
    void sample(bit_vector_t& bv);

    /// Compute conditional entropy
    void compute_conditional_entropy(std::size_t index);

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
        throw hnco::exception::Error("Mimic::Mimic: population size must be > 1");

      for (auto& p: _parameters)
        p = pv_t(n);
    }

    /// Initialization
    void init();

    /** @name Setters
     */
    ///@{

    /// Set the selection size
    void set_selection_size(int x) {

      if (!(x > 0))
        throw hnco::exception::Error("Mimic::set_selection_size: selection size must be positive");

      if (!(size_t(x) < _population.size()))
        throw hnco::exception::Error("Mimic::set_selection_size: selection size must be lower than population size");

      _selection_size = x;
    }

    ///@}

  };

} // end of namespace hnco::algorithm


#endif
