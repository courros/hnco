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
    auto neighborhood = new StandardBitMutation
      (options.get_bv_size(),
       options.get_mutation_rate() / options.get_bv_size());
    neighborhood->set_allow_no_mutation(options.with_allow_no_mutation());
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
    return new HammingSphereIterator(options.get_bv_size(), options.get_radius());

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

  case 0:
    return new CompleteSearch(options.get_bv_size());

  case 10: {
    auto algo = new RandomSearch(options.get_bv_size());

    algo->set_num_iterations(options.get_num_iterations());

    return algo;
  }

  case 20: {
    auto neighborhood = make_neighborhood(options);
    auto algo = new RandomWalk(options.get_bv_size(), neighborhood);
    algo->set_incremental_evaluation(options.with_incremental_evaluation());
    algo->set_num_iterations(options.get_num_iterations());
    if (options.with_rw_log_value())
      algo->set_log_value();
    return algo;
  }

  case 30: {
    auto algo = new Human(options.get_bv_size());

    algo->set_num_iterations(options.get_num_iterations());

    return algo;
  }

  case 100: {
    auto neighborhood = make_neighborhood(options);
    assert(neighborhood);

    auto algo = new RandomLocalSearch
      (options.get_bv_size(),
       neighborhood);

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

    algo->set_num_iterations(options.get_num_iterations());

    return algo;
  }

  case 160: {
    auto neighborhood = make_neighborhood_iterator(options);
    assert(neighborhood);

    auto algo = new FirstAscentHillClimbing
      (options.get_bv_size(),
       neighborhood);

    algo->set_num_iterations(options.get_num_iterations());

    return algo;
  }

  case 200: {
    auto neighborhood = make_neighborhood(options);
    assert(neighborhood);

    auto algo = new SimulatedAnnealing(options.get_bv_size(), neighborhood);

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_num_transitions(options.get_sa_num_transitions());
    algo->set_num_trials(options.get_sa_num_trials());
    algo->set_initial_acceptance_probability(options.get_sa_initial_acceptance_probability());
    algo->set_beta_ratio(options.get_sa_beta_ratio());

    return algo;
  }

  case 300: {
    auto algo = new OnePlusOneEa(options.get_bv_size());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_mutation_rate(options.get_mutation_rate() / options.get_bv_size());
    algo->set_incremental_evaluation(options.with_incremental_evaluation());
    algo->set_allow_no_mutation(options.with_allow_no_mutation());

    return algo;
  }

  case 310: {
    auto algo = new MuPlusLambdaEa
      (options.get_bv_size(),
       options.get_ea_mu(),
       options.get_ea_lambda());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_mutation_rate(options.get_mutation_rate() / options.get_bv_size());
    algo->set_allow_no_mutation(options.with_allow_no_mutation());

    return algo;
  }

  case 320: {
    auto algo = new MuCommaLambdaEa
      (options.get_bv_size(),
       options.get_ea_mu(),
       options.get_ea_lambda());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_mutation_rate(options.get_mutation_rate() / options.get_bv_size());
    algo->set_allow_no_mutation(options.with_allow_no_mutation());

    return algo;
  }

  case 400: {
    auto algo = new GeneticAlgorithm
      (options.get_bv_size(),
       options.get_ea_mu());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_mutation_rate(options.get_mutation_rate() / options.get_bv_size());
    algo->set_crossover_probability(options.get_ga_crossover_probability());
    algo->set_tournament_size(options.get_ga_tournament_size());
    algo->set_allow_no_mutation(options.with_allow_no_mutation());

    return algo;
  }

  case 450: {
    auto algo = new OnePlusLambdaCommaLambdaGa
      (options.get_bv_size(),
       options.get_ea_mu());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_mutation_rate(options.get_mutation_rate() / options.get_bv_size());
    algo->set_crossover_bias(options.get_ga_crossover_bias());

    return algo;
  }

  case 500: {
    auto algo = new Pbil
      (options.get_bv_size(),
       options.get_population_size());

    algo->set_learning_rate(options.get_learning_rate());
    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());

    algo->set_log_entropy(options.with_pv_log_entropy());
    algo->set_log_num_components(options.get_pv_log_num_components());
    algo->set_log_pv(options.with_pv_log_pv());

    return algo;
  }

  case 501: {
    auto algo = new NpsPbil
      (options.get_bv_size(),
       options.get_population_size());

    algo->set_learning_rate(options.get_learning_rate());
    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());

    algo->set_log_entropy(options.with_pv_log_entropy());
    algo->set_log_num_components(options.get_pv_log_num_components());
    algo->set_log_pv(options.with_pv_log_pv());

    return algo;
  }

  case 600: {
    auto algo = new Umda
      (options.get_bv_size(),
       options.get_population_size());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());

    algo->set_log_entropy(options.with_pv_log_entropy());
    algo->set_log_num_components(options.get_pv_log_num_components());
    algo->set_log_pv(options.with_pv_log_pv());

    return algo;
  }

  case 700: {
    auto algo = new CompactGa(options.get_bv_size());

    algo->set_learning_rate(options.get_learning_rate());
    algo->set_num_iterations(options.get_num_iterations());

    algo->set_log_entropy(options.with_pv_log_entropy());
    algo->set_log_num_components(options.get_pv_log_num_components());
    algo->set_log_pv(options.with_pv_log_pv());

    return algo;
  }

  case 800: {
    auto algo = new Mmas(options.get_bv_size());

    algo->set_learning_rate(options.get_learning_rate());
    algo->set_num_iterations(options.get_num_iterations());

    if (options.with_mmas_strict())
      algo->set_compare(std::greater<double>());

    algo->set_log_entropy(options.with_pv_log_entropy());
    algo->set_log_num_components(options.get_pv_log_num_components());
    algo->set_log_pv(options.with_pv_log_pv());

    return algo;
  }

  case 900: {
    using namespace hea;

    auto herding = new BitHerding(options.get_bv_size());

    herding->set_randomize_bit_order(options.with_hea_randomize_bit_order());
    herding->set_dynamics(options.get_hea_bit_herding());
    herding->set_weight(options.get_hea_weight());

    auto algo = new
      Hea<BitMoment, BitHerding>(options.get_bv_size(),
                                 options.get_population_size());

    algo->set_herding(herding);
    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());
    algo->set_learning_rate(options.get_learning_rate());
    algo->set_reset_period(options.get_hea_reset_period());
    algo->set_bound_moment(options.with_hea_bound_moment());
    algo->set_weight(options.get_hea_weight());

    Hea<BitMoment, BitHerding>::log_flags_t lf = {};
    if (options.with_hea_log_error())
      lf.set(Hea<BitMoment, BitHerding>::LOG_ERROR);
    if (options.with_hea_log_dtu())
      lf.set(Hea<BitMoment, BitHerding>::LOG_DTU);
    if (options.with_hea_log_delta())
      lf.set(Hea<BitMoment, BitHerding>::LOG_DELTA);
    if (options.with_hea_log_selection())
      lf.set(Hea<BitMoment, BitHerding>::LOG_SELECTION);
    if (options.with_hea_log_moment_matrix())
      lf.set(Hea<BitMoment, BitHerding>::LOG_MOMENT_MATRIX);
    algo->set_log_flags(lf);

    return algo;
  }

  case 901: {
    using namespace hea;

    auto herding = new SpinHerding(options.get_bv_size());

    herding->set_randomize_bit_order(options.with_hea_randomize_bit_order());
    herding->set_weight(options.get_hea_weight());
    herding->set_num_seq_updates(options.get_hea_num_seq_updates());
    herding->set_sampling_method(options.get_hea_sampling_method());

    auto algo = new
      Hea<SpinMoment, SpinHerding>(options.get_bv_size(),
                                   options.get_population_size());

    algo->set_herding(herding);
    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());
    algo->set_learning_rate(options.get_learning_rate());
    algo->set_reset_period(options.get_hea_reset_period());
    algo->set_bound_moment(options.with_hea_bound_moment());
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
    if (options.with_hea_log_moment_matrix())
      lf.set(Hea<SpinMoment, SpinHerding>::LOG_MOMENT_MATRIX);
    algo->set_log_flags(lf);

    return algo;
  }

  case 1000: {
    using namespace bm_pbil;
    auto algo = new BmPbil
      (options.get_bv_size(),
       options.get_population_size());

    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());
    algo->set_learning_rate(options.get_learning_rate());
    algo->set_num_gs_steps(options.get_bm_num_gs_steps());
    algo->set_num_gs_cycles(options.get_bm_num_gs_cycles());
    algo->set_negative_positive_selection(options.with_bm_negative_positive_selection());
    algo->set_sampling(options.get_bm_sampling());
    algo->set_mc_reset_strategy(options.get_bm_mc_reset_strategy());

    algo->set_log_norm_infinite(options.with_bm_log_norm_infinite());
    algo->set_log_norm_l1(options.with_bm_log_norm_l1());

    return algo;
  }

  case 1100: {
    auto algo = new hnco::algorithm::Mimic
      (options.get_bv_size(),
       options.get_population_size());
    algo->set_num_iterations(options.get_num_iterations());
    algo->set_selection_size(options.get_selection_size());
    return algo;
  }

  case 1110: {
    auto algo = new hnco::algorithm::fast_efficient_p3::Hboa(options.get_bv_size());
    algo->set_population_size(options.get_population_size());
    return algo;
  }

  case 1200: {
    auto algo = new hnco::algorithm::fast_efficient_p3::Ltga(options.get_bv_size());
    algo->set_population_size(options.get_population_size());
    return algo;
  }

  case 1300:
    return new hnco::algorithm::fast_efficient_p3::ParameterLessPopulationPyramid(options.get_bv_size());

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
