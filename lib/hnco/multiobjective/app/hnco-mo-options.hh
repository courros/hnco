#ifndef HNCO_MULTIOBJECTIVE_APP_HNCO_OPTIONS_H
#define HNCO_MULTIOBJECTIVE_APP_HNCO_OPTIONS_H

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
  std::string _version = "0.23";

  enum {
    OPTION_HELP=256,
    OPTION_HELP_EA,
    OPTION_HELP_FP,
    OPTION_HELP_REP,
    OPTION_VERSION,
    OPTION_ALGORITHM,
    OPTION_BV_SIZE,
    OPTION_DESCRIPTION_PATH,
    OPTION_EA_CROSSOVER_PROBABILITY,
    OPTION_EA_MU,
    OPTION_EA_MUTATION_RATE,
    OPTION_EA_TOURNAMENT_SIZE,
    OPTION_FN_NAME,
    OPTION_FP_DEFAULT_INTERVAL,
    OPTION_FP_EXPRESSION,
    OPTION_FP_INTERVALS,
    OPTION_FP_NUM_BITS,
    OPTION_FP_PRECISION,
    OPTION_FUNCTION,
    OPTION_NUM_ITERATIONS,
    OPTION_NUM_THREADS,
    OPTION_PATH,
    OPTION_REP_CATEGORICAL_REPRESENTATION,
    OPTION_REP_NUM_ADDITIONAL_BITS,
    OPTION_RESULTS_PATH,
    OPTION_SEED,
    OPTION_SOLUTION_PATH,
    OPTION_EA_ALLOW_NO_MUTATION,
    OPTION_FN_DISPLAY,
    OPTION_FN_GET_BV_SIZE,
    OPTION_FN_GET_OUTPUT_SIZE,
    OPTION_PRINT_DEFAULT_PARAMETERS,
    OPTION_PRINT_DESCRIPTION,
    OPTION_PRINT_PARAMETERS,
    OPTION_PRINT_PARETO_FRONT
  };

  /// Type of algorithm
  int _algorithm = 1400;
  bool _with_algorithm = false;

  /// Size of bit vectors
  int _bv_size = 100;
  bool _with_bv_size = false;

  /// Path of the description file
  std::string _description_path = "description.txt";
  bool _with_description_path = false;

  /// Crossover probability
  double _ea_crossover_probability = 0.8;
  bool _with_ea_crossover_probability = false;

  /// Parent population size
  int _ea_mu = 100;
  bool _with_ea_mu = false;

  /// Mutation rate relative to bv_size
  double _ea_mutation_rate = 1;
  bool _with_ea_mutation_rate = false;

  /// Tournament size
  int _ea_tournament_size = 2;
  bool _with_ea_tournament_size = false;

  /// Name of the function in the dynamic library
  std::string _fn_name;
  bool _with_fn_name = false;

  /// Default interval
  std::string _fp_default_interval = "[0, 1]";
  bool _with_fp_default_interval = false;

  /// Expression to parse
  std::string _fp_expression = "(1-x)^2+100*(y-x^2)^2";
  bool _with_fp_expression = false;

  /// Intervals
  std::string _fp_intervals = "x in [0, 1] y in [0, 1]";
  bool _with_fp_intervals = false;

  /// Number of bits in the dyadic representation of a number
  int _fp_num_bits = 8;
  bool _with_fp_num_bits = false;

  /// Precision of the dyadic representation of a number
  double _fp_precision = 0.01;
  bool _with_fp_precision = false;

  /// Type of function
  int _function = 180;
  bool _with_function = false;

  /// Number of iterations
  int _num_iterations = 100;
  bool _with_num_iterations = false;

  /// Number of threads
  int _num_threads = 1;
  bool _with_num_threads = false;

  /// Path of a function file
  std::string _path = "function.dat";
  bool _with_path = false;

  /// Categorical representation
  int _rep_categorical_representation = 0;
  bool _with_rep_categorical_representation = false;

  /// Number of additional bits per element for permutation representation
  int _rep_num_additional_bits = 2;
  bool _with_rep_num_additional_bits = false;

  /// Path of the results file
  std::string _results_path = "results.json";
  bool _with_results_path = false;

  /// Seed for the random number generator
  unsigned _seed;
  bool _with_seed = false;

  /// Path of the solution file
  std::string _solution_path = "solution.dat";
  bool _with_solution_path = false;

  /// Allow no mutation with standard bit mutation
  bool _ea_allow_no_mutation = false;

  /// Display the function and exit
  bool _fn_display = false;

  /// Print the size of bit vectors
  bool _fn_get_bv_size = false;

  /// Print the number of objectives
  bool _fn_get_output_size = false;

  /// Print the parameters and exit
  bool _print_default_parameters = false;

  /// Print a description of the solution
  bool _print_description = false;

  /// Print the parameters
  bool _print_parameters = false;

  /// Print the Pareto front
  bool _print_pareto_front = false;

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

  /// Default constructor
  HncoOptions(): _exec_name("hnco-mo") {}

  /// Constructor
  HncoOptions(int argc, char *argv[], bool ignore_bad_options = false);

  /// Get the value of algorithm
  int get_algorithm() const { return _algorithm; }

  /// With parameter algorithm
  bool with_algorithm() const { return _with_algorithm; }

  /// Get the value of bv_size
  int get_bv_size() const { return _bv_size; }

  /// With parameter bv_size
  bool with_bv_size() const { return _with_bv_size; }

  /// Get the value of description_path
  std::string get_description_path() const { return _description_path; }

  /// With parameter description_path
  bool with_description_path() const { return _with_description_path; }

  /// Get the value of ea_crossover_probability
  double get_ea_crossover_probability() const { return _ea_crossover_probability; }

  /// With parameter ea_crossover_probability
  bool with_ea_crossover_probability() const { return _with_ea_crossover_probability; }

  /// Get the value of ea_mu
  int get_ea_mu() const { return _ea_mu; }

  /// With parameter ea_mu
  bool with_ea_mu() const { return _with_ea_mu; }

  /// Get the value of ea_mutation_rate
  double get_ea_mutation_rate() const { return _ea_mutation_rate; }

  /// With parameter ea_mutation_rate
  bool with_ea_mutation_rate() const { return _with_ea_mutation_rate; }

  /// Get the value of ea_tournament_size
  int get_ea_tournament_size() const { return _ea_tournament_size; }

  /// With parameter ea_tournament_size
  bool with_ea_tournament_size() const { return _with_ea_tournament_size; }

  /// Get the value of fn_name
  std::string get_fn_name() const {
    if (_with_fn_name)
      return _fn_name;
    else
      throw std::runtime_error("HncoOptions::get_fn_name: Parameter fn_name has no default value and has not been set");
    }

  /// With parameter fn_name
  bool with_fn_name() const { return _with_fn_name; }

  /// Get the value of fp_default_interval
  std::string get_fp_default_interval() const { return _fp_default_interval; }

  /// With parameter fp_default_interval
  bool with_fp_default_interval() const { return _with_fp_default_interval; }

  /// Get the value of fp_expression
  std::string get_fp_expression() const { return _fp_expression; }

  /// With parameter fp_expression
  bool with_fp_expression() const { return _with_fp_expression; }

  /// Get the value of fp_intervals
  std::string get_fp_intervals() const { return _fp_intervals; }

  /// With parameter fp_intervals
  bool with_fp_intervals() const { return _with_fp_intervals; }

  /// Get the value of fp_num_bits
  int get_fp_num_bits() const { return _fp_num_bits; }

  /// With parameter fp_num_bits
  bool with_fp_num_bits() const { return _with_fp_num_bits; }

  /// Get the value of fp_precision
  double get_fp_precision() const { return _fp_precision; }

  /// With parameter fp_precision
  bool with_fp_precision() const { return _with_fp_precision; }

  /// Get the value of function
  int get_function() const { return _function; }

  /// With parameter function
  bool with_function() const { return _with_function; }

  /// Get the value of num_iterations
  int get_num_iterations() const { return _num_iterations; }

  /// With parameter num_iterations
  bool with_num_iterations() const { return _with_num_iterations; }

  /// Get the value of num_threads
  int get_num_threads() const { return _num_threads; }

  /// With parameter num_threads
  bool with_num_threads() const { return _with_num_threads; }

  /// Get the value of path
  std::string get_path() const { return _path; }

  /// With parameter path
  bool with_path() const { return _with_path; }

  /// Get the value of rep_categorical_representation
  int get_rep_categorical_representation() const { return _rep_categorical_representation; }

  /// With parameter rep_categorical_representation
  bool with_rep_categorical_representation() const { return _with_rep_categorical_representation; }

  /// Get the value of rep_num_additional_bits
  int get_rep_num_additional_bits() const { return _rep_num_additional_bits; }

  /// With parameter rep_num_additional_bits
  bool with_rep_num_additional_bits() const { return _with_rep_num_additional_bits; }

  /// Get the value of results_path
  std::string get_results_path() const { return _results_path; }

  /// With parameter results_path
  bool with_results_path() const { return _with_results_path; }

  /// Get the value of seed
  unsigned get_seed() const {
    if (_with_seed)
      return _seed;
    else
      throw std::runtime_error("HncoOptions::get_seed: Parameter seed has no default value and has not been set");
    }

  /// With parameter seed
  bool with_seed() const { return _with_seed; }

  /// Get the value of solution_path
  std::string get_solution_path() const { return _solution_path; }

  /// With parameter solution_path
  bool with_solution_path() const { return _with_solution_path; }

  /// With the flag ea_allow_no_mutation
  bool with_ea_allow_no_mutation() const { return _ea_allow_no_mutation; }

  /// With the flag fn_display
  bool with_fn_display() const { return _fn_display; }

  /// With the flag fn_get_bv_size
  bool with_fn_get_bv_size() const { return _fn_get_bv_size; }

  /// With the flag fn_get_output_size
  bool with_fn_get_output_size() const { return _fn_get_output_size; }

  /// With the flag print_default_parameters
  bool with_print_default_parameters() const { return _print_default_parameters; }

  /// With the flag print_description
  bool with_print_description() const { return _print_description; }

  /// With the flag print_parameters
  bool with_print_parameters() const { return _print_parameters; }

  /// With the flag print_pareto_front
  bool with_print_pareto_front() const { return _print_pareto_front; }

  friend std::ostream& operator<<(std::ostream&, const HncoOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const HncoOptions& options);

}
}
}

#endif
