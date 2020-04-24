/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_BM_PBIL_BM_PBIL_H
#define HNCO_ALGORITHMS_BM_PBIL_BM_PBIL_H

#include "hnco/algorithms/algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/permutation.hh"

#include "model.hh"


namespace hnco {
namespace algorithm {
/// Boltzmann machine PBIL
namespace bm_pbil {

  /** Boltzmann machine PBIL.

      The BM model is slightly different from the one given in the
      reference below. More precisely, 0/1 variables are mapped to
      -1/+1 variables as in Walsh analysis.

      Reference:

      Arnaud Berny. 2002. Boltzmann machine for population-based
      incremental learning. In ECAI 2002. IOS Press, Lyon.

  */
  class BmPbil:
    public IterativeAlgorithm {

  public:

    enum {

      /** Asynchronous sampling.

          A single component of the internal state is randomly
          selected then updated by Gibbs sampling. This step is
          repeated _num_gs_steps times.
      */
      SAMPLING_ASYNCHRONOUS,

      /** Asynchronous sampling with full scan.

          To sample a new bit vector, a random permutation is sampled
          and all components of the internal state are updated by
          Gibbs sampling in the order defined by the permutation.
      */
      SAMPLING_ASYNCHRONOUS_FULL_SCAN,

      /** Synchronous sampling.

          The full internal state is updated in one step from the
          probability vector made of the very marginal probabilities
          used in Gibbs sampling.
      */
      SAMPLING_SYNCHRONOUS

    };

    enum {

      /** No reset.

       */
      RESET_NO_RESET,

      /** Reset MC at the beginning of each iteration.

       */
      RESET_ITERATION,

      /** Reset MC before sampling each bit vector.

       */
      RESET_BIT_VECTOR

    };

  private:

    /// Population
    Population _population;

    /// Model
    Model _model;

    /// Parameters averaged over all individuals
    ModelParameters _parameters_all;

    /// Parameters averaged over selected individuals
    ModelParameters _parameters_best;

    /// Parameters averaged over negatively selected individuals
    ModelParameters _parameters_worst;

    /// Uniform distribution on bit_vector_t components
    std::uniform_int_distribution<int> _choose_bit;

    /// Permutation
    permutation_t _permutation;

    /** @name Parameters
     */
    ///@{

    /// Selection size (number of selected individuals in the population)
    int _selection_size = 1;

    /// Learning rate
    double _learning_rate = 1e-3;

    /// Number of gibbs sampler steps
    int _num_gs_steps = 100;

    /// Number of gibbs sampler cycles
    int _num_gs_cycles = 1;

    /// Negative and positive selection
    bool _negative_positive_selection = false;

    /// Sampling mode
    int _sampling = SAMPLING_ASYNCHRONOUS;

    /// MC reset strategy
    int _mc_reset_strategy = RESET_NO_RESET;

    ///@}

    /** @name Logging
     */
    ///@{

    /// Log infinite norm of the model parameters
    bool _log_norm_infinite = false;

    /// Log 1-norm of the model parameters
    bool _log_norm_l1 = false;

    ///@}

    /// Single iteration
    void iterate();

    /// Log
    void log();

    /// Sample a bit vector
    void sample(bit_vector_t& x);

    /// Asynchronous sampling
    void sample_asynchronous();

    /// Asynchronous sampling with full scan
    void sample_asynchronous_full_scan();

    /// Synchronous sampling
    void sample_synchronous();

  public:

    /// Constructor
    BmPbil(int n,
           int population_size):
      IterativeAlgorithm(n),
      _population(population_size, n),
      _model(n),
      _parameters_all(n),
      _parameters_best(n),
      _parameters_worst(n),
      _choose_bit(0, n - 1),
      _permutation(n) {}

    /// Initialization
    void init();

    /** @name Parameters
     */
    ///@{

    /** Set the selection size.

        The selection size is the number of selected individuals in
        the population.
    */
    void set_selection_size(int x) { _selection_size = x; }

    /// Set the learning rate
    void set_learning_rate(double x) { _learning_rate = x; }

    /// Set the number of gibbs sampler steps
    void set_num_gs_steps(int x) { _num_gs_steps = x; }

    /// Set the number of gibbs sampler cycles
    void set_num_gs_cycles(int x) { _num_gs_cycles = x; }

    /// Set negative and positive selection
    void set_negative_positive_selection(bool x) { _negative_positive_selection = x; }

    /// Set the sampling mode
    void set_sampling(int x) { _sampling = x; }

    /// Set the MC reset strategy
    void set_mc_reset_strategy(int x) { _mc_reset_strategy = x; }

    ///@}

    /** @name Setters for logging
     */
    ///@{

    /// Log infinite norm of the model parameters
    void set_log_norm_infinite(bool x) { _log_norm_infinite = x; }

    /// Log 1-norm of the model parameters
    void set_log_norm_l1(bool x) { _log_norm_l1 = x; }

    /// Set flag for something to log
    void set_something_to_log() { _something_to_log = _log_norm_infinite || _log_norm_l1; }

    ///@}

  };

} // end of namespace bm_pbil
} // end of namespace algorithm
} // end of namespace hnco


#endif
