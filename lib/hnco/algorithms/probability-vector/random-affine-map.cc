/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include <random>               // std::uniform_int_distribution,
                                // std::geometric_distribution

#include <hnco/algorithms/evolutionary-algorithms/crossover.hh> // hnco::algorithm::UniformCrossover

#include "random-affine-map.hh"

using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::map;
using namespace hnco::random;
using namespace hnco;

void
RamUmda::init()
{
  set_something_to_log();
  random_solution();
  _population_x.random();
}

void
RamUmda::iterate()
{
  if (_functions.size() > 1)
    _population_x.evaluate_in_parallel(_functions);
  else
    _population_x.evaluate(_function);
  _population_x.sort();
  update_solution(_population_x.get_best_bv(),
                  _population_x.get_best_value());

  // Maps
  compute_ts_length();
  _map.random(get_bv_size(), _ts_length, TsAffineMap::SamplingMode::unconstrained);
  _inverse_map = _map;
  _inverse_map.invert();

  // Send selected individuals to the latent space
  for (int i = 0; i < _selection_size; i++)
    _map.map(_population_x.get_best_bv(i), _population_y.bvs[i]);

  // Compute probability vector in the latent space
  pv_init(_pv);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_pv, _population_y.bvs[i]);
  pv_average(_pv, _selection_size);
  pv_bound(_pv, _lower_bound, _upper_bound);

  // Generate new population
  for (int i = 0; i < _population_x.get_size(); i++) {
    pv_sample(_pv, _a);
    _inverse_map.map(_a, _population_x.bvs[i]);
  }
}

void
RamUmda::compute_ts_length()
{
  switch (_ts_length_mode) {

  case TsLengthMode::constant:
    break;

  case TsLengthMode::increasing:
    if ((_iteration + 1) % _ts_length_increment_period == 0)
      _ts_length++;
    _ts_length = std::min(_ts_length, _ts_length_upper_bound);
    break;

  case TsLengthMode::decreasing:
    if ((_iteration + 1) % _ts_length_increment_period == 0)
      _ts_length--;
    _ts_length = std::max(_ts_length, _ts_length_lower_bound);
    break;

  case TsLengthMode::uniform: {
    std::uniform_int_distribution dist(_ts_length_lower_bound, _ts_length_upper_bound);
    _ts_length = dist(Generator::engine);
    break;
  }

  case TsLengthMode::geometric: {
    std::geometric_distribution dist(_ts_length_distribution_parameter);
    _ts_length = dist(Generator::engine);
    _ts_length = std::min(_ts_length, _ts_length_upper_bound);
    break;
  }

  case TsLengthMode::reverse_geometric: {
    std::geometric_distribution dist(_ts_length_distribution_parameter);
    _ts_length = _ts_length_upper_bound - dist(Generator::engine);
    _ts_length = std::max(_ts_length, _ts_length_lower_bound);
    break;
  }

  default:
    std::ostringstream stream;
    stream << _ts_length_mode;
    throw std::runtime_error("RamUmda::compute_ts_length: Unknown _ts_length_mode: " + stream.str());

  }
}

void
RamUmda2::init()
{
  set_something_to_log();
  random_solution();
  _population_x.random();
}

void
RamUmda2::iterate()
{
  if (_functions.size() > 1)
    _population_x.evaluate_in_parallel(_functions);
  else
    _population_x.evaluate(_function);
  _population_x.sort();
  update_solution(_population_x.get_best_bv(),
                  _population_x.get_best_value());

  // Compute probability vector in the solution space
  pv_init(_pv);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_pv, _population_x.bvs[i]);
  pv_average(_pv, _selection_size);
  pv_bound(_pv, _lower_bound, _upper_bound);

  // Maps
  compute_ts_length();
  _map.random(get_bv_size(), _ts_length, TsAffineMap::SamplingMode::unconstrained);
  _inverse_map = _map;
  _inverse_map.invert();

  // Send selected individuals to the latent space
  for (int i = 0; i < _selection_size; i++)
    _map.map(_population_x.get_best_bv(i), _population_y.bvs[i]);

  // Compute probability vector in the latent space
  pv_init(_pv_y);
  for (int i = 0; i < _selection_size; i++)
    pv_add(_pv_y, _population_y.bvs[i]);
  pv_average(_pv_y, _selection_size);
  pv_bound(_pv_y, _lower_bound, _upper_bound);

  // Generate new population
  for (int i = 0; i < _population_x.get_size(); i++) {
    if (Generator::uniform() < _crossover_probability) {
      pv_sample(_pv_y, _a);
      _inverse_map.map(_a, _b);
      pv_sample(_pv, _c);
      UniformCrossover crossover;
      crossover.recombine(_b, _c, _population_x.bvs[i]);
    } else {
      if (Generator::uniform() < _latent_space_probability) {
        pv_sample(_pv_y, _a);
        _inverse_map.map(_a, _population_x.bvs[i]);
      } else {
        pv_sample(_pv, _population_x.bvs[i]);
      }
    }
  }
}
