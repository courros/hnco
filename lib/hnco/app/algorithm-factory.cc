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

#include "hnco/algorithms/all.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/exception.hh"

#include "algorithm-factory.hh"

using namespace hnco::algorithm;
using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


Neighborhood *
make_neighborhood(const HncoOptions& options)
{
  switch(options.get_neighborhood()) {

  case 0:
    return new SingleBitFlip(options.get_bv_size());

  case 1: {
    auto neighborhood = new StandardBitMutation
      (options.get_bv_size(),
       options.get_ea_mutation_rate() / options.get_bv_size());
    neighborhood->set_allow_no_mutation(options.with_ea_allow_no_mutation());
    return neighborhood;
  }

  case 2:
    return new HammingBall(options.get_bv_size(), options.get_radius());

  case 3:
    return new HammingSphere(options.get_bv_size(), options.get_radius());

  default:
    throw std::runtime_error("make_neighborhood: Unknown neighborhood type: " + std::to_string(options.get_neighborhood()));
  }
}


NeighborhoodIterator *
make_neighborhood_iterator(const HncoOptions& options)
{
  switch(options.get_neighborhood_iterator()) {

  case 0:
    return new SingleBitFlipIterator(options.get_bv_size());

  case 1:
    return new HammingSphereIterator(options.get_bv_size(), options.get_radius());

  default:
    throw std::runtime_error("make_neighborhood_iterator: Unknown neighborhood iterator type: " + std::to_string(options.get_neighborhood_iterator()));
  }
}


Algorithm *
CommandLineAlgorithmFactory::make(int bv_size)
{
  switch(_options.get_algorithm()) {

  case 0:
    return new CompleteSearch(bv_size);

  case 10: {
    auto algo = new RandomSearch(bv_size);

    algo->set_num_iterations(_options.get_num_iterations());

    return algo;
  }

  case 20: {
    auto neighborhood = make_neighborhood(_options);
    auto algo = new RandomWalk(bv_size, neighborhood);
    algo->set_incremental_evaluation(_options.with_incremental_evaluation());
    algo->set_num_iterations(_options.get_num_iterations());
    if (_options.with_rw_log_value())
      algo->set_log_value();
    return algo;
  }

  case 30: {
    auto algo = new Human(bv_size);

    algo->set_num_iterations(_options.get_num_iterations());

    return algo;
  }

  case 100: {
    auto neighborhood = make_neighborhood(_options);
    assert(neighborhood);

    auto algo = new RandomLocalSearch
      (bv_size,
       neighborhood);

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_patience(_options.get_rls_patience());
    algo->set_incremental_evaluation(_options.with_incremental_evaluation());

    if (_options.with_rls_strict())
      algo->set_compare(std::greater<double>());

    return algo;
  }

  case 150: {
    auto neighborhood = make_neighborhood_iterator(_options);
    assert(neighborhood);

    auto algo = new SteepestAscentHillClimbing
      (bv_size,
       neighborhood);

    algo->set_num_iterations(_options.get_num_iterations());

    return algo;
  }

  case 160: {
    auto neighborhood = make_neighborhood_iterator(_options);
    assert(neighborhood);

    auto algo = new FirstAscentHillClimbing
      (bv_size,
       neighborhood);

    algo->set_num_iterations(_options.get_num_iterations());

    return algo;
  }

  case 200: {
    auto neighborhood = make_neighborhood(_options);
    assert(neighborhood);

    auto algo = new SimulatedAnnealing(bv_size, neighborhood);

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_num_transitions(_options.get_sa_num_transitions());
    algo->set_num_trials(_options.get_sa_num_trials());
    algo->set_initial_acceptance_probability(_options.get_sa_initial_acceptance_probability());
    algo->set_beta_ratio(_options.get_sa_beta_ratio());

    return algo;
  }

  case 300: {
    auto algo = new OnePlusOneEa(bv_size);

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_incremental_evaluation(_options.with_incremental_evaluation());
    algo->set_allow_no_mutation(_options.with_ea_allow_no_mutation());

    return algo;
  }

  case 301: {
    auto algo = new SelfAdjustingOnePlusOneEa(bv_size);

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_mutation_rate_min(_options.get_ea_mutation_rate_min());
    algo->set_mutation_rate_max(_options.get_ea_mutation_rate_max());
    algo->set_update_strength(_options.get_ea_update_strength());
    algo->set_success_ratio(_options.get_ea_success_ratio());
    algo->set_allow_no_mutation(_options.with_ea_allow_no_mutation());
    algo->set_incremental_evaluation(_options.with_incremental_evaluation());

    return algo;
  }

  case 310: {
    auto algo = new MuPlusLambdaEa
      (bv_size,
       _options.get_ea_mu(),
       _options.get_ea_lambda());

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_allow_no_mutation(_options.with_ea_allow_no_mutation());

    return algo;
  }

  case 320: {
    auto algo = new MuCommaLambdaEa
      (bv_size,
       _options.get_ea_mu(),
       _options.get_ea_lambda());

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_allow_no_mutation(_options.with_ea_allow_no_mutation());

    return algo;
  }

  case 400: {
    auto algo = new GeneticAlgorithm
      (bv_size,
       _options.get_ea_mu());

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_crossover_probability(_options.get_ea_crossover_probability());
    algo->set_tournament_size(_options.get_ea_tournament_size());
    algo->set_allow_no_mutation(_options.with_ea_allow_no_mutation());

    return algo;
  }

  case 450: {
    auto algo = new OnePlusLambdaCommaLambdaGa
      (bv_size,
       _options.get_ea_mu());

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_mutation_rate(_options.get_ea_mutation_rate() / bv_size);
    algo->set_crossover_bias(_options.get_ea_crossover_bias());

    return algo;
  }

  case 500: {
    auto algo = new Pbil
      (bv_size,
       _options.get_population_size());

    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_entropy(_options.with_pv_log_entropy());
    algo->set_log_num_components(_options.get_pv_log_num_components());
    algo->set_log_pv(_options.with_pv_log_pv());

    return algo;
  }

  case 501: {
    auto algo = new NpsPbil
      (bv_size,
       _options.get_population_size());

    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_entropy(_options.with_pv_log_entropy());
    algo->set_log_num_components(_options.get_pv_log_num_components());
    algo->set_log_pv(_options.with_pv_log_pv());

    return algo;
  }

  case 600: {
    auto algo = new Umda
      (bv_size,
       _options.get_population_size());

    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_entropy(_options.with_pv_log_entropy());
    algo->set_log_num_components(_options.get_pv_log_num_components());
    algo->set_log_pv(_options.with_pv_log_pv());

    return algo;
  }

  case 700: {
    auto algo = new CompactGa(bv_size);

    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_num_iterations(_options.get_num_iterations());

    algo->set_log_entropy(_options.with_pv_log_entropy());
    algo->set_log_num_components(_options.get_pv_log_num_components());
    algo->set_log_pv(_options.with_pv_log_pv());

    return algo;
  }

  case 800: {
    auto algo = new Mmas(bv_size);

    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_num_iterations(_options.get_num_iterations());

    if (_options.with_mmas_strict())
      algo->set_compare(std::greater<double>());

    algo->set_log_entropy(_options.with_pv_log_entropy());
    algo->set_log_num_components(_options.get_pv_log_num_components());
    algo->set_log_pv(_options.with_pv_log_pv());

    return algo;
  }

  case 900: {
    using namespace walsh_moment;

    auto algo = new
      Hea<SymmetricWalshMoment2Herding>(bv_size,
                                        _options.get_population_size());

    algo->set_bound_moment(_options.with_hea_bound_moment());
    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_randomize_bit_order(_options.with_hea_randomize_bit_order());
    algo->set_reset_period(_options.get_hea_reset_period());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_delta_norm(_options.with_hea_log_delta_norm());
    algo->set_log_target_norm(_options.with_hea_log_target_norm());
    algo->set_log_herding_error(_options.with_hea_log_herding_error());
    algo->set_log_target(_options.with_hea_log_target());

    return algo;
  }

  case 901: {
    using namespace walsh_moment;

    auto algo = new
      Hea<LowerTriangularWalshMoment2Herding>(bv_size,
                                              _options.get_population_size());

    algo->set_bound_moment(_options.with_hea_bound_moment());
    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_randomize_bit_order(_options.with_hea_randomize_bit_order());
    algo->set_reset_period(_options.get_hea_reset_period());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_delta_norm(_options.with_hea_log_delta_norm());
    algo->set_log_target_norm(_options.with_hea_log_target_norm());
    algo->set_log_herding_error(_options.with_hea_log_herding_error());
    algo->set_log_target(_options.with_hea_log_target());

    return algo;
  }

  case 1000: {
    using namespace walsh_moment;

    auto algo = new BmPbil<SymmetricWalshMoment2GibbsSampler>
      (bv_size,
       _options.get_population_size());

    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_mc_reset_strategy(_options.get_bm_mc_reset_strategy());
    algo->set_negative_positive_selection(_options.with_bm_negative_positive_selection());
    algo->set_num_gs_cycles(_options.get_bm_num_gs_cycles());
    algo->set_num_gs_steps(_options.get_bm_num_gs_steps());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_sampling(_options.get_bm_sampling());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_norm_1(_options.with_bm_log_norm_1());
    algo->set_log_norm_infinite(_options.with_bm_log_norm_infinite());

    return algo;
  }

  case 1001: {
    using namespace walsh_moment;

    auto algo = new BmPbil<LowerTriangularWalshMoment2GibbsSampler>
      (bv_size,
       _options.get_population_size());

    algo->set_learning_rate(_options.get_learning_rate());
    algo->set_mc_reset_strategy(_options.get_bm_mc_reset_strategy());
    algo->set_negative_positive_selection(_options.with_bm_negative_positive_selection());
    algo->set_num_gs_cycles(_options.get_bm_num_gs_cycles());
    algo->set_num_gs_steps(_options.get_bm_num_gs_steps());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_sampling(_options.get_bm_sampling());
    algo->set_selection_size(_options.get_selection_size());

    algo->set_log_norm_1(_options.with_bm_log_norm_1());
    algo->set_log_norm_infinite(_options.with_bm_log_norm_infinite());

    return algo;
  }

  case 1100: {
    auto algo = new hnco::algorithm::Mimic
      (bv_size,
       _options.get_population_size());
    algo->set_num_iterations(_options.get_num_iterations());
    algo->set_selection_size(_options.get_selection_size());
    return algo;
  }

  case 1110: {
    auto algo = new hnco::algorithm::fast_efficient_p3::Hboa(bv_size);
    algo->set_population_size(_options.get_population_size());
    return algo;
  }

  case 1200: {
    auto algo = new hnco::algorithm::fast_efficient_p3::Ltga(bv_size);
    algo->set_population_size(_options.get_population_size());
    return algo;
  }

  case 1300:
    return new hnco::algorithm::fast_efficient_p3::ParameterLessPopulationPyramid(bv_size);

  default:
    throw std::runtime_error("CommandLineAlgorithmFactory::make: Unknown algorithm type: " + std::to_string(_options.get_algorithm()));
  }
}
