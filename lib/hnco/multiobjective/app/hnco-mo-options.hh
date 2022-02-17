#ifndef HNCO_MULTIONJECTIVE_APP_HNCO_OPTIONS_H
#define HNCO_MULTIONJECTIVE_APP_HNCO_OPTIONS_H

#include <iostream>
#include <string>

namespace hnco {
namespace multiobjective {
namespace app {

/// Command line options for hnco-mo
class HncoOptions {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version;

  /// Type of algorithm
  int _algorithm;
  bool _opt_algorithm;

  /// Size of bit vectors
  int _bv_size;
  bool _opt_bv_size;

  /// Path of the description file
  std::string _description_path;
  bool _opt_description_path;

  /// Offspring population size
  int _ea_lambda;
  bool _opt_ea_lambda;

  /// Parent population size
  int _ea_mu;
  bool _opt_ea_mu;

  /// Name of the function in the dynamic library
  std::string _fn_name;
  bool _opt_fn_name;

  /// Expression to parse
  std::string _fp_expression;
  bool _opt_fp_expression;

  /// Lower bound
  double _fp_lower_bound;
  bool _opt_fp_lower_bound;

  /// Number of bits in the dyadic representation of a number
  int _fp_num_bits;
  bool _opt_fp_num_bits;

  /// Precision of the dyadic representation of a number
  double _fp_precision;
  bool _opt_fp_precision;

  /// Upper bound
  double _fp_upper_bound;
  bool _opt_fp_upper_bound;

  /// Type of function
  int _function;
  bool _opt_function;

  /// Crossover bias
  double _ga_crossover_bias;
  bool _opt_ga_crossover_bias;

  /// Crossover probability
  double _ga_crossover_probability;
  bool _opt_ga_crossover_probability;

  /// Tournament size
  int _ga_tournament_size;
  bool _opt_ga_tournament_size;

  /// Mutation rate relative to bv_size
  double _mutation_rate;
  bool _opt_mutation_rate;

  /// Number of iterations (<= 0 means indefinite)
  int _num_iterations;
  bool _opt_num_iterations;

  /// Number of threads
  int _num_threads;
  bool _opt_num_threads;

  /// Path of a function file
  std::string _path;
  bool _opt_path;

  /// Categorical representation
  int _rep_categorical_representation;
  bool _opt_rep_categorical_representation;

  /// Number of additional bits per element for permutation representation
  int _rep_num_additional_bits;
  bool _opt_rep_num_additional_bits;

  /// Path of the results file
  std::string _results_path;
  bool _opt_results_path;

  /// Seed for the random number generator
  unsigned _seed;
  bool _opt_seed;

  /// Path of the solution file
  std::string _solution_path;
  bool _opt_solution_path;

  /// Allow no mutation with standard bit mutation
  bool _allow_no_mutation;

  /// At the end, print or save the solution in the domain of the concrete function
  bool _concrete_solution;

  /// Display the function and exit
  bool _fn_display;

  /// Print the size of bit vectors
  bool _fn_get_bv_size;

  /// Load a solution from a file
  bool _load_solution;

  /// Print the default parameters and exit
  bool _print_defaults;

  /// Print a description of the solution
  bool _print_description;

  /// At the beginning, print the header
  bool _print_header;

  /// Print results
  bool _print_results;

  /// Print the solution
  bool _print_solution;

  /// At the end, save a description of the solution in a file
  bool _save_description;

  /// At the end, save results in a file
  bool _save_results;

  /// At the end, save the solution in a file
  bool _save_solution;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print help message for section fp
  void print_help_fp(std::ostream& stream) const;

  /// Print help message for section rep
  void print_help_rep(std::ostream& stream) const;

  /// Print help message for section ea
  void print_help_ea(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  HncoOptions(int argc, char *argv[]);

  /// Get algorithm
  int get_algorithm() const { return _algorithm; }

  /// Set algorithm
  void set_algorithm(int x) {
    _algorithm = x;
    _opt_algorithm = true;
  }

  /// Get set-flag for algorithm
  bool set_algorithm() const { return _opt_algorithm; }

  /// Get bv_size
  int get_bv_size() const { return _bv_size; }

  /// Set bv_size
  void set_bv_size(int x) {
    _bv_size = x;
    _opt_bv_size = true;
  }

  /// Get set-flag for bv_size
  bool set_bv_size() const { return _opt_bv_size; }

  /// Get description_path
  std::string get_description_path() const { return _description_path; }

  /// Set description_path
  void set_description_path(std::string x) {
    _description_path = x;
    _opt_description_path = true;
  }

  /// Get set-flag for description_path
  bool set_description_path() const { return _opt_description_path; }

  /// Get ea_lambda
  int get_ea_lambda() const { return _ea_lambda; }

  /// Set ea_lambda
  void set_ea_lambda(int x) {
    _ea_lambda = x;
    _opt_ea_lambda = true;
  }

  /// Get set-flag for ea_lambda
  bool set_ea_lambda() const { return _opt_ea_lambda; }

  /// Get ea_mu
  int get_ea_mu() const { return _ea_mu; }

  /// Set ea_mu
  void set_ea_mu(int x) {
    _ea_mu = x;
    _opt_ea_mu = true;
  }

  /// Get set-flag for ea_mu
  bool set_ea_mu() const { return _opt_ea_mu; }

  /// Get fn_name
  std::string get_fn_name() const { return _fn_name; }

  /// Set fn_name
  void set_fn_name(std::string x) {
    _fn_name = x;
    _opt_fn_name = true;
  }

  /// Get set-flag for fn_name
  bool set_fn_name() const { return _opt_fn_name; }

  /// Get fp_expression
  std::string get_fp_expression() const { return _fp_expression; }

  /// Set fp_expression
  void set_fp_expression(std::string x) {
    _fp_expression = x;
    _opt_fp_expression = true;
  }

  /// Get set-flag for fp_expression
  bool set_fp_expression() const { return _opt_fp_expression; }

  /// Get fp_lower_bound
  double get_fp_lower_bound() const { return _fp_lower_bound; }

  /// Set fp_lower_bound
  void set_fp_lower_bound(double x) {
    _fp_lower_bound = x;
    _opt_fp_lower_bound = true;
  }

  /// Get set-flag for fp_lower_bound
  bool set_fp_lower_bound() const { return _opt_fp_lower_bound; }

  /// Get fp_num_bits
  int get_fp_num_bits() const { return _fp_num_bits; }

  /// Set fp_num_bits
  void set_fp_num_bits(int x) {
    _fp_num_bits = x;
    _opt_fp_num_bits = true;
  }

  /// Get set-flag for fp_num_bits
  bool set_fp_num_bits() const { return _opt_fp_num_bits; }

  /// Get fp_precision
  double get_fp_precision() const { return _fp_precision; }

  /// Set fp_precision
  void set_fp_precision(double x) {
    _fp_precision = x;
    _opt_fp_precision = true;
  }

  /// Get set-flag for fp_precision
  bool set_fp_precision() const { return _opt_fp_precision; }

  /// Get fp_upper_bound
  double get_fp_upper_bound() const { return _fp_upper_bound; }

  /// Set fp_upper_bound
  void set_fp_upper_bound(double x) {
    _fp_upper_bound = x;
    _opt_fp_upper_bound = true;
  }

  /// Get set-flag for fp_upper_bound
  bool set_fp_upper_bound() const { return _opt_fp_upper_bound; }

  /// Get function
  int get_function() const { return _function; }

  /// Set function
  void set_function(int x) {
    _function = x;
    _opt_function = true;
  }

  /// Get set-flag for function
  bool set_function() const { return _opt_function; }

  /// Get ga_crossover_bias
  double get_ga_crossover_bias() const { return _ga_crossover_bias; }

  /// Set ga_crossover_bias
  void set_ga_crossover_bias(double x) {
    _ga_crossover_bias = x;
    _opt_ga_crossover_bias = true;
  }

  /// Get set-flag for ga_crossover_bias
  bool set_ga_crossover_bias() const { return _opt_ga_crossover_bias; }

  /// Get ga_crossover_probability
  double get_ga_crossover_probability() const { return _ga_crossover_probability; }

  /// Set ga_crossover_probability
  void set_ga_crossover_probability(double x) {
    _ga_crossover_probability = x;
    _opt_ga_crossover_probability = true;
  }

  /// Get set-flag for ga_crossover_probability
  bool set_ga_crossover_probability() const { return _opt_ga_crossover_probability; }

  /// Get ga_tournament_size
  int get_ga_tournament_size() const { return _ga_tournament_size; }

  /// Set ga_tournament_size
  void set_ga_tournament_size(int x) {
    _ga_tournament_size = x;
    _opt_ga_tournament_size = true;
  }

  /// Get set-flag for ga_tournament_size
  bool set_ga_tournament_size() const { return _opt_ga_tournament_size; }

  /// Get mutation_rate
  double get_mutation_rate() const { return _mutation_rate; }

  /// Set mutation_rate
  void set_mutation_rate(double x) {
    _mutation_rate = x;
    _opt_mutation_rate = true;
  }

  /// Get set-flag for mutation_rate
  bool set_mutation_rate() const { return _opt_mutation_rate; }

  /// Get num_iterations
  int get_num_iterations() const { return _num_iterations; }

  /// Set num_iterations
  void set_num_iterations(int x) {
    _num_iterations = x;
    _opt_num_iterations = true;
  }

  /// Get set-flag for num_iterations
  bool set_num_iterations() const { return _opt_num_iterations; }

  /// Get num_threads
  int get_num_threads() const { return _num_threads; }

  /// Set num_threads
  void set_num_threads(int x) {
    _num_threads = x;
    _opt_num_threads = true;
  }

  /// Get set-flag for num_threads
  bool set_num_threads() const { return _opt_num_threads; }

  /// Get path
  std::string get_path() const { return _path; }

  /// Set path
  void set_path(std::string x) {
    _path = x;
    _opt_path = true;
  }

  /// Get set-flag for path
  bool set_path() const { return _opt_path; }

  /// Get rep_categorical_representation
  int get_rep_categorical_representation() const { return _rep_categorical_representation; }

  /// Set rep_categorical_representation
  void set_rep_categorical_representation(int x) {
    _rep_categorical_representation = x;
    _opt_rep_categorical_representation = true;
  }

  /// Get set-flag for rep_categorical_representation
  bool set_rep_categorical_representation() const { return _opt_rep_categorical_representation; }

  /// Get rep_num_additional_bits
  int get_rep_num_additional_bits() const { return _rep_num_additional_bits; }

  /// Set rep_num_additional_bits
  void set_rep_num_additional_bits(int x) {
    _rep_num_additional_bits = x;
    _opt_rep_num_additional_bits = true;
  }

  /// Get set-flag for rep_num_additional_bits
  bool set_rep_num_additional_bits() const { return _opt_rep_num_additional_bits; }

  /// Get results_path
  std::string get_results_path() const { return _results_path; }

  /// Set results_path
  void set_results_path(std::string x) {
    _results_path = x;
    _opt_results_path = true;
  }

  /// Get set-flag for results_path
  bool set_results_path() const { return _opt_results_path; }

  /// Get seed
  unsigned get_seed() const { return _seed; }

  /// Set seed
  void set_seed(unsigned x) {
    _seed = x;
    _opt_seed = true;
  }

  /// Get set-flag for seed
  bool set_seed() const { return _opt_seed; }

  /// Get solution_path
  std::string get_solution_path() const { return _solution_path; }

  /// Set solution_path
  void set_solution_path(std::string x) {
    _solution_path = x;
    _opt_solution_path = true;
  }

  /// Get set-flag for solution_path
  bool set_solution_path() const { return _opt_solution_path; }

  /// Get allow_no_mutation
  bool with_allow_no_mutation() const { return _allow_no_mutation; }

  /// Set allow_no_mutation
  void set_allow_no_mutation() { _allow_no_mutation = true; }

  /// Get concrete_solution
  bool with_concrete_solution() const { return _concrete_solution; }

  /// Set concrete_solution
  void set_concrete_solution() { _concrete_solution = true; }

  /// Get fn_display
  bool with_fn_display() const { return _fn_display; }

  /// Set fn_display
  void set_fn_display() { _fn_display = true; }

  /// Get fn_get_bv_size
  bool with_fn_get_bv_size() const { return _fn_get_bv_size; }

  /// Set fn_get_bv_size
  void set_fn_get_bv_size() { _fn_get_bv_size = true; }

  /// Get load_solution
  bool with_load_solution() const { return _load_solution; }

  /// Set load_solution
  void set_load_solution() { _load_solution = true; }

  /// Get print_defaults
  bool with_print_defaults() const { return _print_defaults; }

  /// Set print_defaults
  void set_print_defaults() { _print_defaults = true; }

  /// Get print_description
  bool with_print_description() const { return _print_description; }

  /// Set print_description
  void set_print_description() { _print_description = true; }

  /// Get print_header
  bool with_print_header() const { return _print_header; }

  /// Set print_header
  void set_print_header() { _print_header = true; }

  /// Get print_results
  bool with_print_results() const { return _print_results; }

  /// Set print_results
  void set_print_results() { _print_results = true; }

  /// Get print_solution
  bool with_print_solution() const { return _print_solution; }

  /// Set print_solution
  void set_print_solution() { _print_solution = true; }

  /// Get save_description
  bool with_save_description() const { return _save_description; }

  /// Set save_description
  void set_save_description() { _save_description = true; }

  /// Get save_results
  bool with_save_results() const { return _save_results; }

  /// Set save_results
  void set_save_results() { _save_results = true; }

  /// Get save_solution
  bool with_save_solution() const { return _save_solution; }

  /// Set save_solution
  void set_save_solution() { _save_solution = true; }

  friend std::ostream& operator<<(std::ostream&, const HncoOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const HncoOptions& options);

}
}
}

#endif
