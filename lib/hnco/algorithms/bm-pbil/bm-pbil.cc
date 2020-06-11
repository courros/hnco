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

#include "hnco/exception.hh"

#include "bm-pbil.hh"

using namespace hnco::algorithm::bm_pbil;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;


void
BmPbil::init()
{
  set_something_to_log();

  random_solution();
  _model.init();
}


void
BmPbil::sample(bit_vector_t& x)
{
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
    throw Error("BmPbil::sample: Unknown _sampling: " + stream.str());
  }
  x = _model.get_state();
}


void
BmPbil::sample_asynchronous()
{
  for (int t = 0; t < _num_gs_steps; t++)
    _model.gibbs_sampler(_choose_bit(random::Generator::engine));
}


void
BmPbil::sample_asynchronous_full_scan()
{
  for (int t = 0; t < _num_gs_cycles; t++) {
    perm_random(_permutation);
    for (size_t i = 0; i < _permutation.size(); i++)
      _model.gibbs_sampler(_permutation[i]);
  }
}


void
BmPbil::sample_synchronous()
{
  for (int t = 0; t < _num_gs_cycles; t++)
    _model.gibbs_sampler_synchronous();
}


void
BmPbil::iterate()
{
  if (_mc_reset_strategy == RESET_ITERATION)
    _model.reset_mc();

  // Sample population
  for (int i = 0; i < _population.size(); i++) {
    if (_mc_reset_strategy == RESET_BIT_VECTOR)
      _model.reset_mc();
    sample(_population.get_bv(i));
  }

  // Evaluate population
  if (_functions.size() > 1)
    _population.eval(_functions);
  else
    _population.eval(_function);

  _population.sort();

  update_solution(_population.get_best_bv(),
                  _population.get_best_value());

  // Average best individuals
  _parameters_best.init();
  for (int i = 0; i < _selection_size; i++)
    _parameters_best.add(_population.get_best_bv(i));
  _parameters_best.average(_selection_size);

  if (_negative_positive_selection) {
    // Average worst individuals
    _parameters_worst.init();
    for (int i = 0; i < _selection_size; i++)
      _parameters_worst.add(_population.get_worst_bv(i));
    _parameters_worst.average(_selection_size);
    _model.update(_parameters_best, _parameters_worst, _learning_rate);
  } else {
    // Average all individuals
    _parameters_all.init();
    for (int i = 0; i < _population.size(); i++)
      _parameters_all.add(_population.get_bv(i));
    _parameters_all.average(_population.size());
    _model.update(_parameters_best, _parameters_all, _learning_rate);
  }

}


void
BmPbil::log()
{
  assert(_something_to_log);

  if (_log_norm_infinite)
    (*_stream) << _model.norm_infinite() << " ";

  if (_log_norm_l1)
    (*_stream) << _model.norm_l1() << " ";

  (*_stream) << std::endl;

}
