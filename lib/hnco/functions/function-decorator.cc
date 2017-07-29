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

#include "hnco/random.hh"

#include "function-decorator.hh"


using namespace hnco::exception;
using namespace hnco::function;


double
StopOnMaximum::eval(const bit_vector_t& x)
{
  assert(_function->has_known_maximum());

  double value = _function->eval(x);
  if (value == _function->get_maximum())
    throw MaximumReached(std::make_pair(x, value));
  return value;
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
  double value = _function->eval(x);
  if (value >= _target)
    throw TargetReached(std::make_pair(x, value));
  return value;
}


void
StopOnTarget::update(const bit_vector_t& x, double value)
{
  _function->update(x, value);
  if (value >= _target)
    throw TargetReached(std::make_pair(x, value));
}


double
Negation::eval(const bit_vector_t& x)
{
  return -_function->eval(x);
}


double
FunctionMapComposition::eval(const bit_vector_t& x)
{
  _map->map(x, _bv);
  return _function->eval(_bv);
}


double
AdditiveGaussianNoise::eval(const bit_vector_t& x)
{
  return _function->eval(x) + _dist(random::Random::engine);
}


double
CallCounter::eval(const bit_vector_t& x)
{
  double result = _function->eval(x);
  _num_calls++;
  return result;
}


double
CallCounter::delta(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result = _function->delta(x, value, flipped_bits);
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
OnBudgetFunction::delta(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  if (_num_calls == _budget)
    throw LastEvaluation();
  double result = _function->delta(x, value, flipped_bits);
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
  try { result = _function->eval(x); }
  catch (MaximumReached) {
    assert(_function->has_known_maximum());
    update_last_improvement(_function->get_maximum());
    throw;
  }
  catch (const TargetReached& e) {
    update_last_improvement(e.get_point_value().second);
    throw;
  }
  update_last_improvement(result);
  return result;
}


double
ProgressTracker::delta(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  double result;
  try { result = _function->delta(x, value, flipped_bits); }
  catch (MaximumReached) {
    assert(_function->has_known_maximum());
    update_last_improvement(_function->get_maximum());
    throw;
  }
  catch (const TargetReached& e) {
    update_last_improvement(e.get_point_value().second);
    throw;
  }
  update_last_improvement(value + result);
  return result;
}


void
ProgressTracker::update(const bit_vector_t& x, double value)
{
  try { _function->update(x, value); }
  catch (MaximumReached) {
    assert(_function->has_known_maximum());
    update_last_improvement(_function->get_maximum());
    throw;
  }
  catch (const TargetReached& e) {
    update_last_improvement(e.get_point_value().second);
    throw;
  }
  update_last_improvement(value);
}


void
ProgressTracker::update_last_improvement(double value)
{
  _num_calls++;

  if (_num_calls == 1) {

    _last_improvement.time = _num_calls;
    _last_improvement.value = value;

    if (_log_improvement)
      _stream << _last_improvement << std::endl;

  } else if (value > _last_improvement.value) {
    assert(_num_calls > 1);

    // Last point of the plateau
    if (_log_improvement) {
      if (_num_calls > _last_improvement.time + 1)
        _stream << (_num_calls - 1) << " " << _last_improvement.value << std::endl;
    }

    _last_improvement.time = _num_calls;
    _last_improvement.value = value;

    if (_log_improvement)
      _stream << _last_improvement << std::endl;
  }
}


std::ostream& hnco::function::operator<<(std::ostream& stream, const ProgressTracker::Event& event)
{
  stream << event.time << " " << event.value;
  return stream;
}


double
Cache::eval(const bit_vector_t& x)
{
  _num_evaluations++;

  assert(x.size() == _x.size());
  for (size_t i = 0; i < _x.size(); i++)
    _x[i] = x[i];

  std::unordered_map<std::vector<bool>, double>::iterator iter = _cache.find(_x);

  if (iter == _cache.end()) {
    double v = _function->eval(x);
    auto pair = std::pair<std::vector<bool>, double>(_x, v);
    _cache.insert(pair);
    return v;
  } else {
    _num_lookups++;
    return iter->second;
  }
}


void
Cache::display(std::ostream& stream)
{
  stream
    << "Cache size = " << _cache.size() << std::endl
    << "Number of lookups = " << _num_lookups << std::endl
    << "Number of evaluations = " << _num_evaluations << std::endl
    << "Ratio = " << double(_num_lookups) / double(_num_evaluations) << std::endl;
}
