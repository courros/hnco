#ifndef HNCO_ALGORITHMS_EA_IT_EA_H
#define HNCO_ALGORITHMS_EA_IT_EA_H

#include <assert.h>

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/population.hh"
#include "hnco/neighborhoods/neighborhood.hh" // neighborhood::StandardBitMutation

namespace hnco {
namespace algorithm {

/**
 * Information-theoretic evolutionary algorithm.
 */
class InformationTheoreticEa: public IterativeAlgorithm {
public:
  /// Selection for replacement
  struct Replacement {
    enum {
      /// Elitist replacement
      elitist               = 0,
      /// Non elitist replacement
      non_elitist           = 1,
      /// Maximum likelihood update
      ml_update             = 2,
      /// Incremental maximum likelihood update
      incremental_ml_update = 3,
      /// No replacement (static search)
      no_replacement        = 4
    };
  };

  /// Constructor
  InformationTheoreticEa(int n, int population_size)
    : IterativeAlgorithm(n)
    , _population(population_size, n)
    , _masks(population_size, bit_vector_t(n))
    , _likelihoods(n)
    , _mutation_operator(n)
    , _center(bit_vector_t(n), 0)
    , _mutation_rate_init(1 / double(n))
    , _mutation_rate_min(1 / double(n))
  {}

  /**
   * @name Setters
   */
  ///@{
  /// Set the selection size
  void set_selection_size(int n) {
    assert(n >= 1);
    assert(n < _population.get_size());
    _selection_size = n;
  }
  /// Set the learning rate
  void set_learning_rate(double rate) { _learning_rate = rate; }
  /// Set the initial mutation rate
  void set_mutation_rate_init(double rate) { _mutation_rate_init = rate; }
  /// Set the minimum mutation rate
  void set_mutation_rate_min(double rate) { _mutation_rate_min = rate; }
  /// Set the maximum mutation rate
  void set_mutation_rate_max(double rate) { _mutation_rate_max = rate; }
  /// Set replacement
  void set_replacement(int replacement) { _replacement = replacement; }
  /// Set the initial Hamming weight
  void set_initial_hamming_weight(int n) { _initial_hamming_weight = n; }
  /// Allow no mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }
  ///@}

  /**
   * @name Setters for logging
   */
  ///@{
  /// Log mutation rate
  void set_log_mutation_rate(bool b) { _log_mutation_rate = b; }
  /// Log center fitness
  void set_log_center_fitness(bool b) { _log_center_fitness = b; }
  ///@}

protected:
  /// %Population
  Population _population;
  /// Mutation masks
  std::vector<bit_vector_t> _masks;
  /// Mutation likelihoods
  std::vector<double> _likelihoods;
  /// Mutation operator
  neighborhood::StandardBitMutation _mutation_operator;
  /// Center of the search distribution
  solution_t _center;
  /// Mutation rate
  double _mutation_rate;

  /**
   * @name Parameters
   */
  ///@{
  /// Selection size
  int _selection_size = 1;
  /// Learning rate
  double _learning_rate = 0.01;
  /// Initial mutation rate
  double _mutation_rate_init;
  /// Minimum mutation rate
  double _mutation_rate_min;
  /// Maximum mutation rate
  double _mutation_rate_max = 0.5;
  /// Initial Hamming weight
  int _initial_hamming_weight = 0;
  /// Replacement
  int _replacement = Replacement::elitist;
  /// Allow no mutation
  bool _allow_no_mutation = false;
  ///@}

  /**
   * @name Logging
   */
  ///@{
  /// Log entropy
  bool _log_mutation_rate = false;
  /// Log center fitness
  bool _log_center_fitness = false;
  ///@}

  /**
   * @name Loop
   */
  ///@{
  /// Initialization
  void init() override;
  /// Single iteration
  void iterate() override;
  /// Log
  void log() override;
  ///@}

  /// Set flag for something to log
  void set_something_to_log() {
    _something_to_log =
         _log_mutation_rate
      || _log_center_fitness;
  }
  /// Compute masks
  void compute_masks(bool equivalent_individuals, std::pair<int, int> range, double c);
  /// ML update
  void ml_update(bool equivalent_individuals, std::pair<int, int> range, double c);
  /// Incremental ML update
  void incremental_ml_update(bool equivalent_individuals, std::pair<int, int> range, double c);
  /// IGO update
  void igo_update(bool equivalent_individuals, std::pair<int, int> range, double c);
};

} // end of namespace algorithm
} // end of namespace hnco

#endif
