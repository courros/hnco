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

#include "hnco/random.hh"

#include "controller.hh"


using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::controller;


double
StopOnMaximum::eval(const bit_vector_t& x)
{
  assert(_function->has_known_maximum());

  double result = _function->eval(x);
  if (result == _function->get_maximum())
    throw MaximumReached(std::make_pair(x, result));
  return result;
}


double
StopOnMaximum::incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  assert(_function->has_known_maximum());

  double result = _function->incremental_eval(x, value, flipped_bits);
  if (result == _function->get_maximum())
    throw MaximumReached(std::make_pair(x, result));
  return result;
}


void
StopOnMaximum::update(const bit_vector_t& x, double value)
{
  assert(_function->has_known_maximum());

  _function->update(x, value);
  if (value == _function->get_maximum())
    throw MaximumReached(std::make_pair(x, value));
}


double
StopOnTarget::eval(const bit_vector_t& x)
{
  double result = _function->eval(x);
  if (result >= _target)
    throw TargetReached(std::make_pair(x, result));
  return result;
}


double
StopOnTarget::incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result = _function->incremental_eval(x, value, flipped_bits);
  if (result >= _target)
    throw TargetReached(std::make_pair(x, result));
  return result;
}


void
StopOnTarget::update(const bit_vector_t& x, double value)
{
  _function->update(x, value);
  if (value >= _target)
    throw TargetReached(std::make_pair(x, value));
}


double
CallCounter::eval(const bit_vector_t& x)
{
  double result = _function->eval(x);
  _num_calls++;
  return result;
}


double
CallCounter::incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result = _function->incremental_eval(x, value, flipped_bits);
  _num_calls++;
  return result;
}


void
CallCounter::update(const bit_vector_t& x, double value)
{
  _function->update(x, value);
  _num_calls++;
}


double
OnBudgetFunction::eval(const bit_vector_t& x)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  double result = _function->eval(x);
  _num_calls++;
  return result;
}


double
OnBudgetFunction::incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  double result = _function->incremental_eval(x, value, flipped_bits);
  _num_calls++;
  return result;
}


void
OnBudgetFunction::update(const bit_vector_t& x, double value)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  _function->update(x, value);
  _num_calls++;
}


double
ProgressTracker::eval(const bit_vector_t& x)
{
  double result;
  _stop_watch.start();
  result = _function->eval(x);
  _stop_watch.stop();
  update_last_improvement(result);
  return result;
}


double
ProgressTracker::incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result;
  result = _function->incremental_eval(x, value, flipped_bits);
  update_last_improvement(result);
  return result;
}


void
ProgressTracker::update(const bit_vector_t& x, double value)
{
  _function->update(x, value);
  update_last_improvement(value);
}


void
ProgressTracker::update_last_improvement(double value)
{
  _num_calls++;

  if (_num_calls == 1) {

    _last_improvement.num_evaluations = _num_calls;
    _last_improvement.value = value;

    if (_log_improvement)
      (*_stream) << _last_improvement << std::endl;

  } else if (value > _last_improvement.value) {
    assert(_num_calls > 1);

    _last_improvement.num_evaluations = _num_calls;
    _last_improvement.value = value;

    if (_log_improvement)
      (*_stream) << _last_improvement << std::endl;
  }
}


std::ostream& hnco::function::controller::operator<<(std::ostream& stream, const ProgressTracker::Event& event)
{
  stream << event.num_evaluations << " " << event.value;
  return stream;
}


double
Cache::eval(const bit_vector_t& x)
{
  assert(x.size() == _key.size());

  _num_evaluations++;

  bv_to_vector_bool(x, _key);
  auto iter = _cache.find(_key);

  if (iter == _cache.end()) {
    double v = _function->eval(x);
    auto pair = std::pair<std::vector<bool>, double>(_key, v);
    _cache.insert(pair);
    return v;
  } else {
    _num_lookups++;
    assert(iter->first == _key);
    return iter->second;
  }
}
