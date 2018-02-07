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

#include "hnco/algorithms/all.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/exception.hh"

#include "make-algorithm.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


Neighborhood *
make_neighborhood(const Options& options)
{
  switch(options.get_neighborhood()) {

  case 0:
    return new SingleBitFlip(options.get_bv_size());

  case 1: {
    auto neighborhood = new BernoulliProcess
      (options.get_bv_size(),
       options.get_mutation() / options.get_bv_size());
    neighborhood->_allow_stay = options.with_allow_stay();
    return neighborhood;
  }

  case 2:
    return new HammingBall(options.get_bv_size(), options.get_radius());

  case 3:
    return new HammingSphere(options.get_bv_size(), options.get_radius());

  default:
    std::ostringstream stream;
    stream << options.get_neighborhood();
    throw Error("make_neighborhood: Unknown neighborhood type: " + stream.str());
  }
}


NeighborhoodIterator *
make_neighborhood_iterator(const Options& options)
{
  switch(options.get_neighborhood_iterator()) {

  case 0:
    return new SingleBitFlipIterator(options.get_bv_size());

  case 1:
    return new HammingBallIterator(options.get_bv_size(), options.get_radius());

  default:
    std::ostringstream stream;
    stream << options.get_neighborhood_iterator();
    throw Error("make_neighborhood_iterator: Unknown neighborhood iterator type: " + stream.str());
  }
}


Algorithm *
make_concrete_algorithm(const Options& options)
{
  switch(options.get_algorithm()) {

  case 0: {
    auto algo = new CompleteSearch(options.get_bv_size());
    assert(algo);
    return algo;
  }

  case 10: {
    auto algo = new RandomSearch(options.get_bv_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());

    return algo;
  }

  case 100: {
    auto neighborhood = make_neighborhood(options);
    assert(neighborhood);

    auto algo = new RandomLocalSearch
      (options.get_bv_size(),
       neighborhood);
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_patience(options.get_rls_patience());
    algo->set_incremental_evaluation(options.with_incremental_evaluation());

    if (options.with_rls_strict())
      algo->set_compare(std::greater<double>());

    return algo;
  }

  case 150: {
    auto neighborhood = make_neighborhood_iterator(options);
    assert(neighborhood);

    auto algo = new SteepestAscentHillClimbing
      (options.get_bv_size(),
       neighborhood);
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());

    return algo;
  }

  case 200: {
    auto neighborhood = make_neighborhood(options);
    assert(neighborhood);

    auto algo = new SimulatedAnnealing(options.get_bv_size(), neighborhood);
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_num_transitions                      = options.get_sa_num_transitions();
    algo->_num_trials                           = options.get_sa_num_trials();
    algo->_initial_acceptance_probability       = options.get_sa_initial_acceptance_probability();
    algo->_rate                                 = options.get_sa_rate();

    return algo;
  }

  case 300: {
    auto algo = new OnePlusOneEa(options.get_bv_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_mutation_probability         = options.get_mutation() / options.get_bv_size();
    algo->_incremental_evaluation       = options.with_incremental_evaluation();
    algo->_allow_stay                   = options.with_allow_stay();

    return algo;
  }

  case 310: {
    auto algo = new MuPlusLambdaEa
      (options.get_bv_size(),
       options.get_ea_mu(),
       options.get_ea_lambda());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_mutation_probability         = options.get_mutation() / options.get_bv_size();
    algo->_allow_stay                   = options.with_allow_stay();

    return algo;
  }

  case 320: {
    auto algo = new MuCommaLambdaEa
      (options.get_bv_size(),
       options.get_ea_mu(),
       options.get_ea_lambda());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_mutation_probability         = options.get_mutation() / options.get_bv_size();
    algo->_allow_stay                   = options.with_allow_stay();

    return algo;
  }

  case 400: {
    auto algo = new GeneticAlgorithm
      (options.get_bv_size(),
       options.get_ea_mu());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_mutation_probability         = options.get_mutation() / options.get_bv_size();
    algo->_crossover_probability        = options.get_ga_crossover_probability();
    algo->_tournament_size              = options.get_ga_tournament_size();
    algo->_allow_stay                   = options.with_allow_stay();

    return algo;
  }

  case 450: {
    auto algo = new OnePlusLambdaCommaLambdaGa
      (options.get_bv_size(),
       options.get_ea_mu());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_mutation_probability         = options.get_mutation() / options.get_bv_size();
    algo->_crossover_bias               = options.get_ga_crossover_bias();

    return algo;
  }

  case 500: {
    auto algo = new Pbil
      (options.get_bv_size(),
       options.get_population_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_selection_size       = options.get_selection_size();
    algo->_rate                 = options.get_learning_rate();
    algo->_log_num_components   = options.get_pv_log_num_components();

    PvAlgorithm::log_flags_t lf = {};
    if (options.with_pv_log_pv())
      lf.set(PvAlgorithm::LOG_PV);
    if (options.with_pv_log_entropy())
      lf.set(PvAlgorithm::LOG_ENTROPY);
    algo->set_log_flags(lf);

    return algo;
  }

  case 501: {
    auto algo = new NpsPbil
      (options.get_bv_size(),
       options.get_population_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_selection_size       = options.get_selection_size();
    algo->_rate                 = options.get_learning_rate();
    algo->_log_num_components   = options.get_pv_log_num_components();

    PvAlgorithm::log_flags_t lf = {};
    if (options.with_pv_log_pv())
      lf.set(PvAlgorithm::LOG_PV);
    if (options.with_pv_log_entropy())
      lf.set(PvAlgorithm::LOG_ENTROPY);
    algo->set_log_flags(lf);

    return algo;
  }

  case 600: {
    auto algo = new Umda
      (options.get_bv_size(),
       options.get_population_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_selection_size       = options.get_selection_size();
    algo->_log_num_components   = options.get_pv_log_num_components();

    PvAlgorithm::log_flags_t lf = {};
    if (options.with_pv_log_pv())
      lf.set(PvAlgorithm::LOG_PV);
    if (options.with_pv_log_entropy())
      lf.set(PvAlgorithm::LOG_ENTROPY);
    algo->set_log_flags(lf);

    return algo;
  }

  case 700: {
    auto algo = new CompactGa(options.get_bv_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_rate                 = options.get_learning_rate();
    algo->_log_num_components   = options.get_pv_log_num_components();

    PvAlgorithm::log_flags_t lf = {};
    if (options.with_pv_log_pv())
      lf.set(PvAlgorithm::LOG_PV);
    if (options.with_pv_log_entropy())
      lf.set(PvAlgorithm::LOG_ENTROPY);
    algo->set_log_flags(lf);

    return algo;
  }

  case 800: {
    auto algo = new Mmas(options.get_bv_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_rate                 = options.get_learning_rate();
    algo->_log_num_components   = options.get_pv_log_num_components();

    if (options.with_mmas_strict())
      algo->_compare = std::greater<double>();

    PvAlgorithm::log_flags_t lf = {};
    if (options.with_pv_log_pv())
      lf.set(PvAlgorithm::LOG_PV);
    if (options.with_pv_log_entropy())
      lf.set(PvAlgorithm::LOG_ENTROPY);
    algo->set_log_flags(lf);

    return algo;
  }

  case 900: {
    using namespace hea;

    auto algo = new
      Hea<BinaryMoment, BinaryHerding>(options.get_bv_size(),
                                       options.get_population_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_selection_size               = options.get_selection_size();
    algo->_rate_strategy                = options.get_hea_rate_strategy();
    algo->_initial_rate                 = options.get_learning_rate();
    algo->_time_constant                = options.get_hea_time_constant();
    algo->_reset_period                 = options.get_hea_reset_period();
    algo->_delay                        = options.get_hea_delay();
    algo->_bound_moment                 = options.with_hea_bound_moment();

    auto& herding = algo->_herding;
    herding._randomize_bit_order        = options.with_hea_randomize_bit_order();
    herding._dynamics                   = options.get_hea_binary_dynamics();
    herding._weight                     = options.get_hea_weight();

    algo->set_weight(options.get_hea_weight());

    Hea<BinaryMoment, BinaryHerding>::log_flags_t lf = {};
    if (options.with_hea_log_error())
      lf.set(Hea<BinaryMoment, BinaryHerding>::LOG_ERROR);
    if (options.with_hea_log_dtu())
      lf.set(Hea<BinaryMoment, BinaryHerding>::LOG_DTU);
    if (options.with_hea_log_delta())
      lf.set(Hea<BinaryMoment, BinaryHerding>::LOG_DELTA);
    if (options.with_hea_log_selection())
      lf.set(Hea<BinaryMoment, BinaryHerding>::LOG_SELECTION);
    algo->set_log_flags(lf);

    return algo;
  }

  case 901: {
    using namespace hea;

    auto algo = new
      Hea<SpinMoment, SpinHerding>(options.get_bv_size(),
                                   options.get_population_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_selection_size               = options.get_selection_size();
    algo->_rate_strategy                = options.get_hea_rate_strategy();
    algo->_initial_rate                 = options.get_learning_rate();
    algo->_time_constant                = options.get_hea_time_constant();
    algo->_reset_period                 = options.get_hea_reset_period();
    algo->_delay                        = options.get_hea_delay();
    algo->_bound_moment                 = options.with_hea_bound_moment();

    auto& herding = algo->_herding;
    herding._randomize_bit_order        = options.with_hea_randomize_bit_order();
    herding._weight                     = options.get_hea_weight();
    herding._num_par_updates            = options.get_hea_num_par_updates();
    herding._num_seq_updates            = options.get_hea_num_seq_updates();
    herding._sampling_method            = options.get_hea_sampling_method();

    algo->set_weight(options.get_hea_weight());

    Hea<SpinMoment, SpinHerding>::log_flags_t lf = {};
    if (options.with_hea_log_error())
      lf.set(Hea<SpinMoment, SpinHerding>::LOG_ERROR);
    if (options.with_hea_log_dtu())
      lf.set(Hea<SpinMoment, SpinHerding>::LOG_DTU);
    if (options.with_hea_log_delta())
      lf.set(Hea<SpinMoment, SpinHerding>::LOG_DELTA);
    if (options.with_hea_log_selection())
      lf.set(Hea<SpinMoment, SpinHerding>::LOG_SELECTION);
    algo->set_log_flags(lf);

    return algo;
  }

  case 1000: {
    using namespace bm_pbil;
    auto algo = new BmPbil
      (options.get_bv_size(),
       options.get_population_size());
    assert(algo);

    algo->set_num_iterations(options.get_num_iterations());
    algo->_selection_size               = options.get_selection_size();
    algo->_rate                         = options.get_learning_rate();
    algo->_num_gs_steps                 = options.get_bm_num_gs_steps();
    algo->_num_gs_cycles                = options.get_bm_num_gs_cycles();
    algo->_negative_positive_selection  = options.with_bm_negative_positive_selection();
    algo->_sampling                     = options.get_bm_sampling();
    algo->_mc_reset_strategy            = options.get_bm_mc_reset_strategy();

    BmPbil::log_flags_t lf = {};
    if (options.with_bm_log_norm_infinite())
      lf.set(BmPbil::LOG_NORM_INFINITE);
    if (options.with_bm_log_norm_l1())
      lf.set(BmPbil::LOG_NORM_L1);
    algo->set_log_flags(lf);

    return algo;
  }

  default:
    std::ostringstream stream;
    stream << options.get_algorithm();
    throw Error("make_algorithm: Unknown algorithm type: " + stream.str());
  }
}


Algorithm *
make_managed_algorithm(const Options& options, Algorithm *algo)
{
  assert(algo);

  if (options.with_restart()) {
    algo = new Restart(options.get_bv_size(), algo);
    assert(algo);
  }

  return algo;
}


Algorithm *
make_algorithm(const Options& options)
{
  auto algo = make_concrete_algorithm(options);
  assert(algo);

  algo = make_managed_algorithm(options, algo);
  assert(algo);

  return algo;
}
