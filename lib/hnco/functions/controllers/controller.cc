/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#include "hnco/logging/logger.hh"

#include "controller.hh"


using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::controller;


double
StopOnTarget::evaluate(const bit_vector_t& bv)
{
  double value = _function->evaluate(bv);
  if (value >= _target) {
    _trigger.first = bv;
    _trigger.second = value;
    throw TargetReached();
  }
  return value;
}


double
StopOnTarget::evaluate_incrementally(const bit_vector_t& bv, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result = _function->evaluate_incrementally(bv, value, flipped_bits);
  if (result >= _target) {
    _trigger.first = bv;
    _trigger.second = result;
    throw TargetReached();
  }
  return result;
}


void
StopOnTarget::update(const bit_vector_t& bv, double value)
{
  _function->update(bv, value);
  if (value >= _target) {
    _trigger.first = bv;
    _trigger.second = value;
    throw TargetReached();
  }
}


double
CallCounter::evaluate(const bit_vector_t& bv)
{
  double value = _function->evaluate(bv);
  _num_calls++;
  return value;
}


double
CallCounter::evaluate_incrementally(const bit_vector_t& bv, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result = _function->evaluate_incrementally(bv, value, flipped_bits);
  _num_calls++;
  return result;
}


void
CallCounter::update(const bit_vector_t& bv, double value)
{
  _function->update(bv, value);
  _num_calls++;
}


double
OnBudgetFunction::evaluate(const bit_vector_t& bv)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  return CallCounter::evaluate(bv);
}


double
OnBudgetFunction::evaluate_incrementally(const bit_vector_t& bv, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  return CallCounter::evaluate_incrementally(bv, value, flipped_bits);
}


void
OnBudgetFunction::update(const bit_vector_t& bv, double value)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  CallCounter::update(bv, value);
}


double
ProgressTracker::evaluate(const bit_vector_t& bv)
{
  double value;
  if (_record_evaluation_time) {
    _stop_watch.start();
    value = CallCounter::evaluate(bv);
    _stop_watch.stop();
  } else {
    value = CallCounter::evaluate(bv);
  }
  update_last_improvement(bv, value);
  return value;
}


double
ProgressTracker::evaluate_incrementally(const bit_vector_t& bv, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result = CallCounter::evaluate_incrementally(bv, value, flipped_bits);
  update_last_improvement(bv, result);
  return result;
}


void
ProgressTracker::update(const bit_vector_t& bv, double value)
{
  CallCounter::update(bv, value);
  update_last_improvement(bv, value);
}


void
ProgressTracker::update_last_improvement(const bit_vector_t& bv, double value)
{
  if (_num_calls == 1) {
    update_last_improvement_details(bv, value);
  } else if (value > _last_improvement.solution.second) {
    assert(_num_calls > 1);
    update_last_improvement_details(bv, value);
  }
}


void
ProgressTracker::update_last_improvement_details(const bit_vector_t& bv, double value)
{
  _last_improvement.num_evaluations = _num_calls;
  if (_record_bit_vector)
    _last_improvement.solution.first = bv;
  _last_improvement.solution.second = value;
  if (_log_improvement)
    HNCO_LOG() << _last_improvement;
}


std::ostream& hnco::function::controller::operator<<(std::ostream& stream, const ProgressTracker::Event& event)
{
  stream << event.num_evaluations << " " << event.solution.second;
  return stream;
}


double
Cache::evaluate(const bit_vector_t& bv)
{
  assert(bv.size() == _key.size());

  _num_evaluations++;

  bv_to_vector_bool(_key, bv);
  auto iter = _cache.find(_key);

  if (iter == _cache.end()) {
    double v = _function->evaluate(bv);
    auto pair = std::pair<std::vector<bool>, double>(_key, v);
    _cache.insert(pair);
    return v;
  } else {
    _num_lookups++;
    assert(iter->first == _key);
    return iter->second;
  }
}
