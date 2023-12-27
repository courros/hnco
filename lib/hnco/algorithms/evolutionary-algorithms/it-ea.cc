#include <algorithm>            // std::clamp

#include <hnco/util.hh>         // hnco::is_in_range
#include <hnco/logging/logger.hh>

#include "it-ea.hh"


using namespace hnco::function;
using namespace hnco::algorithm;
using namespace hnco;


template<typename Fn>
int sample_from_arg(const std::vector<double>& values, Fn fn)
{
  const int n = values.size();
  int index = std::distance(values.begin(), fn(begin(values), end(values)));
  assert(is_in_range(index, n));
  double value = values[index];
  std::vector<int> candidates;
  for (int i = 0; i < n; i++)
    if (values[i] == value)
      candidates.push_back(i);
  assert(candidates.size() >= 1);
  if (candidates.size() > 1) {
    std::uniform_int_distribution<int> dist(0, candidates.size() - 1);
    index = dist(random::Generator::engine);
    assert(is_in_range(index, candidates.size()));
    return candidates[index];
  }
  return index;
}


void
InformationTheoreticEa::init()
{
  if (_replacement == Replacement::no_replacement) {
    bv_random(_solution.first, _initial_hamming_weight);
    _solution.second = _function->evaluate(_solution.first);
  } else {
    random_solution();
  }
  _center = _solution;
  _mutation_rate = _mutation_rate_init;
  _mutation_operator.set_allow_no_mutation(_allow_no_mutation);
  set_something_to_log();
}


void
InformationTheoreticEa::compute_masks(bool equivalent_individuals, std::pair<int, int> range, double c)
{
  if (equivalent_individuals) {
    for (int k = 0; k < range.second; k++)
      bv_add(_masks[k], _center.first, _population.get_best_bv(k));
  } else {
    for (int k = 0; k < _selection_size; k++)
      bv_add(_masks[k], _center.first, _population.get_best_bv(k));
  }
}


void
InformationTheoreticEa::ml_update(bool equivalent_individuals, std::pair<int, int> range, double c)
{
  const int n = get_bv_size();

  std::fill(begin(_likelihoods), end(_likelihoods), 0.0);
  if (equivalent_individuals) {
    for (int k = 0; k < range.first; k++) {
      auto& bv = _population.get_best_bv(k);
      for (int i = 0; i < n; i++)
        _likelihoods[i] += bv[i];
    }
    for (int k = range.first; k < range.second; k++) {
      auto& bv = _population.get_best_bv(k);
      for (int i = 0; i < n; i++)
        _likelihoods[i] += c * bv[i];
    }
  } else {
    for (int k = 0; k < _selection_size; k++) {
      auto& bv = _population.get_best_bv(k);
      for (int i = 0; i < n; i++)
        _likelihoods[i] += bv[i];
    }
  }

  if (_mutation_rate < 0.5) {
    for (int i = 0; i < n; i++) {
      if (2 * _likelihoods[i] > _selection_size) {
        _center.first[i] = 1;
      } else if (2 * _likelihoods[i] < _selection_size) {
        _center.first[i] = 0;
      } else {
        _center.first[i] = bit_random(0.5);
      }
    }
  } else {
    for (int i = 0; i < n; i++) {
      if (2 * _likelihoods[i] > _selection_size) {
        _center.first[i] = 0;
      } else if (2 * _likelihoods[i] < _selection_size) {
        _center.first[i] = 1;
      } else {
        _center.first[i] = bit_random(0.5);
      }
    }
  }

  _center.second = _function->evaluate(_center.first);
  update_solution(_center);
}


void
InformationTheoreticEa::incremental_ml_update(bool equivalent_individuals, std::pair<int, int> range, double c)
{
  const int n = get_bv_size();

  std::fill(begin(_likelihoods), end(_likelihoods), 0.0);
  if (equivalent_individuals) {
    for (int k = 0; k < range.first; k++)
      for (int i = 0; i < n; i++)
        _likelihoods[i] += _masks[k][i];
    for (int k = range.first; k < range.second; k++)
      for (int i = 0; i < n; i++)
        _likelihoods[i] += c * _masks[k][i];
  } else {
    for (int k = 0; k < _selection_size; k++)
      for (int i = 0; i < n; i++)
        _likelihoods[i] += _masks[k][i];
  }

  if (_mutation_rate < 0.5) {
    auto op = std::max_element<typename std::vector<double>::const_iterator>;
    int i = sample_from_arg(_likelihoods, op);
    assert(is_in_range(i, _likelihoods.size()));
    if (2 * _likelihoods[i] > _selection_size) {
      bv_flip(_center.first, i);
      _center.second = _function->evaluate(_center.first);
      update_solution(_center);
    } else if (2 * _likelihoods[i] == _selection_size) {
      if (random::Generator::bernoulli()) {
        bv_flip(_center.first, i);
        _center.second = _function->evaluate(_center.first);
        update_solution(_center);
      }
    }
  } else {
    auto op = std::min_element<typename std::vector<double>::const_iterator>;
    int i = sample_from_arg(_likelihoods, op);
    assert(is_in_range(i, _likelihoods.size()));
    if (2 * _likelihoods[i] < _selection_size) {
      bv_flip(_center.first, i);
      _center.second = _function->evaluate(_center.first);
      update_solution(_center);
    } else if (2 * _likelihoods[i] == _selection_size) {
      if (random::Generator::bernoulli()) {
        bv_flip(_center.first, i);
        _center.second = _function->evaluate(_center.first);
        update_solution(_center);
      }
    }
  }
}


void
InformationTheoreticEa::igo_update(bool equivalent_individuals, std::pair<int, int> range, double c)
{
  // Compute target mutation rate.
  double target = 0;
  if (equivalent_individuals) {
    for (int k = 0; k < range.first; k++)
      target += bv_hamming_weight(_masks[k]);
    for (int k = range.first; k < range.second; k++)
      target += c * bv_hamming_weight(_masks[k]);
  } else {
    for (int k = 0; k < _selection_size; k++)
      target += bv_hamming_weight(_masks[k]);
  }
  const int n = get_bv_size();
  target /= _selection_size * n;

  // Update mutation rate.
  _mutation_rate += _learning_rate * (target - _mutation_rate);
  _mutation_rate = std::clamp(_mutation_rate, _mutation_rate_min, _mutation_rate_max);
}


void
InformationTheoreticEa::iterate()
{
  assert(_solution.second >= _center.second);

  // Generate population.
  _mutation_operator.set_mutation_rate(_mutation_rate);
  for (int k = 0; k < _population.get_size(); k++) {
    _mutation_operator.map(_center.first, _population.bvs[k]);
    // We could do _mutation_operator.get_flipped_bits() to set
    // _masks (see later).
  }

  // Evaluate and sort.
  if (_functions.size() > 1)
    _population.evaluate_in_parallel(_functions);
  else
    _population.evaluate(_function);
  _population.sort();           // Complete sort is required by get_equivalent_bvs
  update_solution(_population.get_best_bv(), _population.get_best_value());

  // Determinate equivalence class.
  int index = _selection_size - 1;
  bool equivalent_individuals = (_population.get_best_value(index) == _population.get_best_value(index + 1));
  std::pair<int, int> range;    // Equivalence class of index
  double c = 0;
  if (equivalent_individuals) {
    range = _population.get_equivalent_bvs(index);
    c = double(index - range.first + 1) / double(range.second - range.first);
    assert(is_in_interval(c, 0, 1));
  }

  compute_masks(equivalent_individuals, range, c);

  switch (_replacement) {

  case Replacement::elitist:
    if (_population.get_best_value() >= _center.second) { // Favor offspring in case of draw
      _center.first  = _population.get_best_bv();
      _center.second = _population.get_best_value();
    }
    break;

  case Replacement::non_elitist:
    _center.first  = _population.get_best_bv();
    _center.second = _population.get_best_value();
    break;

  case Replacement::ml_update:
    ml_update(equivalent_individuals, range, c);
    break;

  case Replacement::incremental_ml_update:
    incremental_ml_update(equivalent_individuals, range, c);
    break;

  case Replacement::no_replacement:
    break;

  default:
    std::ostringstream stream;
    stream << _replacement;
    throw std::runtime_error("InformationTheoreticEa::iterate: Unknown replacement: " + stream.str());

  }

  igo_update(equivalent_individuals, range, c);
}


void
InformationTheoreticEa::log()
{
  assert(_something_to_log);

  logging::Logger l(_log_context);

  if (_log_mutation_rate)
    l.line() << _mutation_rate << " ";

  if (_log_center_fitness)
    l.line() << _center.second << " ";

}
