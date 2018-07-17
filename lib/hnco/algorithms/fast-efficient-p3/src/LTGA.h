// Brian Goldman

// Implementation of the Linkage Tree Genetic Algorithm
// Designed to match the variant in the paper:
// "Hierarchical problem solving with the linkage tree genetic algorithm"
// by D. Thierens and P. A. N. Bosman

#ifndef LTGA_H_
#define LTGA_H_

#include "Population.h"
#include "Evaluation.h"
#include "Util.h"
#include "HillClimb.h"
#include "Configuration.h"
#include "Optimizer.h"
#include <sstream>

// Inherits and implements the Optimizer interface
class LTGA : public Optimizer {
 public:
  LTGA(Random& _rand, shared_ptr<Evaluator> _evaluator, Configuration& _config);
  bool iterate() override;
  string finalize() override;
  create_optimizer(LTGA);

 private:
  size_t pop_size;
  // Configuration option to use only the winners of a binary tournament
  // during model building. Set by inverting "binary_insert" value
  bool disable_binary_insert;
  // Used to store the population and its entropy table
  Population pop;
  // Keeps track of the fitnesses of solutions
  std::unordered_map<vector<bool>, float> fitnesses;
  // Keeps track of the set of unique solutions in the population
  std::unordered_set<vector<bool>> pop_set;

  // Uses a binary tournament between individuals to determine which
  // solutions are part of the entropy calculations.  All solutions are
  // always added to the next generation.
  void binary_insert(Random& rand, vector<vector<bool>> & solutions,
      Population& next_pop);
  void generation();
  hill_climb::pointer hc;
  // Used for recording purposes
  shared_ptr<Evaluator> local_counter, cross_counter;
  std::ostringstream metadata;
};

#endif /* LTGA_H_ */
