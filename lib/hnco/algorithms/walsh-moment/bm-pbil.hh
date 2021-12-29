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

#ifndef HNCO_ALGORITHMS_WALSH_MOMENT_BM_PBIL_H
#define HNCO_ALGORITHMS_WALSH_MOMENT_BM_PBIL_H

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/exception.hh"
#include "hnco/logging/logger.hh"
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
template<class GibbsSampler>
class BmPbil: public IterativeAlgorithm {

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

protected:

  /// Population
  Population _population;

  /// Model parameters
  typename GibbsSampler::Moment _model_parameters;

  /// Model
  GibbsSampler _gibbs_sampler;

  /// Parameters averaged over all individuals
  typename GibbsSampler::Moment _walsh_moment_all;

  /// Parameters averaged over selected individuals
  typename GibbsSampler::Moment _walsh_moment_best;

  /// Parameters averaged over negatively selected individuals
  typename GibbsSampler::Moment _walsh_moment_worst;

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
  bool _log_norm_1 = false;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override {
    set_something_to_log();

    random_solution();
    _model_parameters.init();
    _gibbs_sampler.init();
  }

  /// Single iteration
  void iterate() override {
    if (_mc_reset_strategy == RESET_ITERATION)
      _gibbs_sampler.init();

    // Sample population
    for (int i = 0; i < _population.size(); i++) {
      if (_mc_reset_strategy == RESET_BIT_VECTOR)
        _gibbs_sampler.init();
      sample(_population.get_bv(i));
    }

    // Evaluate population
    if (_functions.size() > 1)
      _population.evaluate_in_parallel(_functions);
    else
      _population.evaluate(_function);

    _population.sort();

    update_solution(_population.get_best_bv(),
                    _population.get_best_value());

    // Average best individuals
    _walsh_moment_best.init();
    for (int i = 0; i < _selection_size; i++)
      _walsh_moment_best.add(_population.get_best_bv(i));
    _walsh_moment_best.average(_selection_size);

    if (_negative_positive_selection) {
      // Average worst individuals
      _walsh_moment_worst.init();
      for (int i = 0; i < _selection_size; i++)
        _walsh_moment_worst.add(_population.get_worst_bv(i));
      _walsh_moment_worst.average(_selection_size);
      _model_parameters.update(_walsh_moment_best, _walsh_moment_worst, _learning_rate);
    } else {
      // Average all individuals
      _walsh_moment_all.init();
      for (int i = 0; i < _population.size(); i++)
        _walsh_moment_all.add(_population.get_bv(i));
      _walsh_moment_all.average(_population.size());
      _model_parameters.update(_walsh_moment_best, _walsh_moment_all, _learning_rate);
    }

  }

  /// Log
  void log() override {
    assert(_something_to_log);

    logging::Logger l(_log_context);

    if (_log_norm_infinite)
      l.line() << _model_parameters.norm_infinite() << " ";

    if (_log_norm_1)
      l.line() << _model_parameters.norm_1() << " ";

  }

  ///@}

  /// Set flag for something to log
  void set_something_to_log() { _something_to_log = _log_norm_infinite || _log_norm_1; }

  /// Sample a bit vector
  void sample(bit_vector_t& x) {
    switch (_sampling) {
    case SAMPLING_ASYNCHRONOUS:
      sample_asynchronous();
      break;
    case SAMPLING_ASYNCHRONOUS_FULL_SCAN:
      sample_asynchronous_full_scan();
      break;
    case SAMPLING_SYNCHRONOUS:
      sample_synchronous();
      break;
    default:
      std::ostringstream stream;
      stream << _sampling;
      throw std::runtime_error("BmPbil::sample: Unknown _sampling: " + stream.str());
    }
    x = _gibbs_sampler.get_state();
  }

  /// Asynchronous sampling
  void sample_asynchronous() {
    for (int t = 0; t < _num_gs_steps; t++)
      _gibbs_sampler.update(_choose_bit(random::Generator::engine));
  }

  /// Asynchronous sampling with full scan
  void sample_asynchronous_full_scan() {
    for (int t = 0; t < _num_gs_cycles; t++) {
      perm_random(_permutation);
      for (size_t i = 0; i < _permutation.size(); i++)
        _gibbs_sampler.update(_permutation[i]);
    }
  }

  /// Synchronous sampling
  void sample_synchronous() {
    for (int t = 0; t < _num_gs_cycles; t++)
      _gibbs_sampler.update_sync();
  }

public:

  /// Constructor
  BmPbil(int n, int population_size):
    IterativeAlgorithm(n),
    _population(population_size, n),
    _model_parameters(n),
    _gibbs_sampler(n, _model_parameters),
    _walsh_moment_all(n),
    _walsh_moment_best(n),
    _walsh_moment_worst(n),
    _choose_bit(0, n - 1),
    _permutation(n) {}

  /** @name Setters for parameters
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
  void set_log_norm_1(bool x) { _log_norm_1 = x; }

  ///@}

};


} // end of namespace bm_pbil
} // end of namespace algorithm
} // end of namespace hnco


#endif
