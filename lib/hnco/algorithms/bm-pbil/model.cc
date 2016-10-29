/* Copyright (C) 2016 Arnaud Berny

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

#include <math.h>

#include <algorithm>

#include "hnco/algorithms/matrix.hh"
#include "hnco/algorithms/util.hh"
#include "hnco/random.hh"

#include "model.hh"


using namespace std;
using namespace hnco;
using namespace hnco::random;
using namespace hnco::algorithm::bm_pbil;


void
ModelParameters::init()
{
  fill(_bias.begin(), _bias.end(), 0.0);
  for (auto& ws : _weight)
    fill(ws.begin(), ws.end(), 0.0);
}


void
ModelParameters::add(const bit_vector_t& x)
{
  const size_t dimension = x.size();
  for (size_t i = 0; i < dimension; i++) {
    vector<double>& ws = _weight[i];
    if (x[i]) {
      _bias[i] -= 1;
      for (size_t j = i + 1; j < dimension; j++)
        if (x[j])
          ws[j] += 1;
        else
          ws[j] -= 1;
    } else {
      _bias[i] += 1;
      for (size_t j = i + 1; j < dimension; j++)
        if (x[j])
          ws[j] -= 1;
        else
          ws[j] += 1;
    }
  }
}


void
ModelParameters::average(int count)
{
  const size_t dimension = _bias.size();
  for (size_t i = 0; i < dimension; i++) {
    _bias[i] /= count;
    for (size_t j = i + 1; j < dimension; j++)
      _weight[i][j] /= count;
  }
}


void
ModelParameters::update(const ModelParameters& p, const ModelParameters& q, double rate)
{
  const size_t dimension = _bias.size();
  for (size_t i = 0; i < dimension; i++) {
    _bias[i] += rate * (p._bias[i] - q._bias[i]);
    for (size_t j = i + 1; j < dimension; j++) {
      _weight[i][j] += rate * (p._weight[i][j] - q._weight[i][j]);
      _weight[j][i] = _weight[i][j];
    }
  }
}


double
ModelParameters::norm_infinite()
{
  double norm = 0;

  const size_t dimension = _bias.size();
  for (size_t i = 0; i < dimension; i++)
    norm = max(norm, fabs(_bias[i]));

  for (size_t i = 0; i < dimension; i++)
    for (size_t j = i + 1; j < dimension; j++)
      norm = max(norm, fabs(_weight[i][j]));

  return norm;
}


double
ModelParameters::norm_l1()
{
  double norm = 0;

  const size_t dimension = _bias.size();
  for (size_t i = 0; i < dimension; i++)
    norm += fabs(_bias[i]);

  for (size_t i = 0; i < dimension; i++)
    for (size_t j = i + 1; j < dimension; j++)
      norm += fabs(_weight[i][j]);

  return norm;
}


void
Model::init()
{
  _model_parameters.init();
  bv_random(_state);
}


void
Model::reset_mc()
{
  bv_random(_state);
}


void
Model::gibbs_sampler(size_t i)
{
  assert(i < _state.size());
  assert(matrix_is_symmetric(_model_parameters._weight));

  double delta = _model_parameters._bias[i];
  const vector<double>& ws = _model_parameters._weight[i];
  for (size_t j = 0; j < _state.size(); j++) {
    if (j == i)
      continue;
    if (_state[j])
      delta -= ws[j];
    else
      delta += ws[j];
  }
  delta *= -2;

  if (Random::uniform() < logistic(delta))
    _state[i] = 1;
  else
    _state[i] = 0;
}


void
Model::gibbs_sampler_synchronous()
{
  assert(matrix_is_symmetric(_model_parameters._weight));

  for (size_t i = 0; i < _pv.size(); i++) {
    double delta = _model_parameters._bias[i];
    const vector<double>& ws = _model_parameters._weight[i];
    for (size_t j = 0; j < _state.size(); j++) {
      if (j == i)
        continue;
      if (_state[j])
        delta -= ws[j];
      else
        delta += ws[j];
    }
    delta *= -2;

    _pv[i] = logistic(delta);
  }
  pv_sample(_pv, _state);
}
