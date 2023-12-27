#ifndef HNCO_APP_HNCO_OPTIONS_H
#define HNCO_APP_HNCO_OPTIONS_H

#include <iostream>
#include <string>

namespace hnco {
namespace app {

/// Command line options for hnco
class HncoOptions {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version = "0.24";

  enum {
    OPTION_HELP=256,
    OPTION_HELP_ALG,
    OPTION_HELP_BM,
    OPTION_HELP_CTRL,
    OPTION_HELP_EA,
    OPTION_HELP_EDA,
    OPTION_HELP_FN,
    OPTION_HELP_FP,
    OPTION_HELP_HEA,
    OPTION_HELP_LS,
    OPTION_HELP_MAP,
    OPTION_HELP_MOD,
    OPTION_HELP_PN,
    OPTION_HELP_REP,
    OPTION_HELP_SA,
    OPTION_VERSION,
    OPTION_ALGORITHM,
    OPTION_BM_MC_RESET_STRATEGY,
    OPTION_BM_NUM_GS_CYCLES,
    OPTION_BM_NUM_GS_STEPS,
    OPTION_BM_SAMPLING_MODE,
    OPTION_BUDGET,
    OPTION_BV_SIZE,
    OPTION_DESCRIPTION_PATH,
    OPTION_EA_CROSSOVER_BIAS,
    OPTION_EA_CROSSOVER_PROBABILITY,
    OPTION_EA_IT_INITIAL_HAMMING_WEIGHT,
    OPTION_EA_IT_REPLACEMENT,
    OPTION_EA_LAMBDA,
    OPTION_EA_MU,
    OPTION_EA_MUTATION_RATE,
    OPTION_EA_MUTATION_RATE_MAX,
    OPTION_EA_MUTATION_RATE_MIN,
    OPTION_EA_SUCCESS_RATIO,
    OPTION_EA_TOURNAMENT_SIZE,
    OPTION_EA_UPDATE_STRENGTH,
    OPTION_EXPRESSION,
    OPTION_FN_NAME,
    OPTION_FN_NUM_TRAPS,
    OPTION_FN_PREFIX_LENGTH,
    OPTION_FN_THRESHOLD,
    OPTION_FP_DEFAULT_DOUBLE_PRECISION,
    OPTION_FP_DEFAULT_DOUBLE_REP,
    OPTION_FP_DEFAULT_DOUBLE_SIZE,
    OPTION_FP_DEFAULT_INT_REP,
    OPTION_FP_DEFAULT_LONG_REP,
    OPTION_FP_EXPRESSION,
    OPTION_FP_EXPRESSION_SOURCE,
    OPTION_FP_REPRESENTATIONS,
    OPTION_FP_REPRESENTATIONS_PATH,
    OPTION_FP_REPRESENTATIONS_SOURCE,
    OPTION_FUNCTION,
    OPTION_HEA_RESET_PERIOD,
    OPTION_LEARNING_RATE,
    OPTION_MAP,
    OPTION_MAP_INPUT_SIZE,
    OPTION_MAP_PATH,
    OPTION_MAP_TS_LENGTH,
    OPTION_MAP_TS_SAMPLING_MODE,
    OPTION_NEIGHBORHOOD,
    OPTION_NEIGHBORHOOD_ITERATOR,
    OPTION_NOISE_STDDEV,
    OPTION_NUM_ITERATIONS,
    OPTION_NUM_THREADS,
    OPTION_PATH,
    OPTION_PN_MUTATION_RATE,
    OPTION_PN_NEIGHBORHOOD,
    OPTION_PN_RADIUS,
    OPTION_POPULATION_SIZE,
    OPTION_PV_LOG_NUM_COMPONENTS,
    OPTION_RADIUS,
    OPTION_REP_CATEGORICAL_REPRESENTATION,
    OPTION_REP_NUM_ADDITIONAL_BITS,
    OPTION_RESULTS_PATH,
    OPTION_RLS_PATIENCE,
    OPTION_SA_BETA_RATIO,
    OPTION_SA_INITIAL_ACCEPTANCE_PROBABILITY,
    OPTION_SA_NUM_TRANSITIONS,
    OPTION_SA_NUM_TRIALS,
    OPTION_SEED,
    OPTION_SELECTION_SIZE,
    OPTION_SOLUTION_PATH,
    OPTION_TARGET,
    OPTION_ADDITIVE_GAUSSIAN_NOISE,
    OPTION_BM_LOG_NORM_1,
    OPTION_BM_LOG_NORM_INFINITE,
    OPTION_BM_NEGATIVE_POSITIVE_SELECTION,
    OPTION_CACHE,
    OPTION_CACHE_BUDGET,
    OPTION_CONCRETE_SOLUTION,
    OPTION_EA_ALLOW_NO_MUTATION,
    OPTION_EA_IT_LOG_CENTER_FITNESS,
    OPTION_EA_LOG_MUTATION_RATE,
    OPTION_FN_DISPLAY,
    OPTION_FN_GET_BV_SIZE,
    OPTION_FN_GET_MAXIMUM,
    OPTION_FN_HAS_KNOWN_MAXIMUM,
    OPTION_FN_PROVIDES_INCREMENTAL_EVALUATION,
    OPTION_FN_WALSH_TRANSFORM,
    OPTION_HEA_BOUND_MOMENT,
    OPTION_HEA_LOG_DELTA_NORM,
    OPTION_HEA_LOG_HERDING_ERROR,
    OPTION_HEA_LOG_TARGET,
    OPTION_HEA_LOG_TARGET_NORM,
    OPTION_HEA_RANDOMIZE_BIT_ORDER,
    OPTION_INCREMENTAL_EVALUATION,
    OPTION_LOAD_SOLUTION,
    OPTION_LOG_IMPROVEMENT,
    OPTION_MAP_DISPLAY,
    OPTION_MAP_RANDOM,
    OPTION_MAP_SURJECTIVE,
    OPTION_MMAS_STRICT,
    OPTION_NEGATION,
    OPTION_PARSED_MODIFIER,
    OPTION_PN_ALLOW_NO_MUTATION,
    OPTION_PRINT_DEFAULT_PARAMETERS,
    OPTION_PRINT_DESCRIPTION,
    OPTION_PRINT_PARAMETERS,
    OPTION_PRINT_RESULTS,
    OPTION_PRINT_SOLUTION,
    OPTION_PRIOR_NOISE,
    OPTION_PV_LOG_ENTROPY,
    OPTION_PV_LOG_PV,
    OPTION_RECORD_EVALUATION_TIME,
    OPTION_RECORD_TOTAL_TIME,
    OPTION_RESTART,
    OPTION_RLS_STRICT,
    OPTION_RW_LOG_VALUE,
    OPTION_SAVE_DESCRIPTION,
    OPTION_SAVE_RESULTS,
    OPTION_SAVE_SOLUTION,
    OPTION_STOP_ON_MAXIMUM,
    OPTION_STOP_ON_TARGET
  };

  /// Type of algorithm
  int _algorithm = 100;
  bool _with_algorithm = false;

  /// Markov chain reset strategy
  int _bm_mc_reset_strategy = 1;
  bool _with_bm_mc_reset_strategy = false;

  /// Number of Gibbs sampler cycles per bit vector
  int _bm_num_gs_cycles = 1;
  bool _with_bm_num_gs_cycles = false;

  /// Number of Gibbs sampler steps per bit vector
  int _bm_num_gs_steps = 100;
  bool _with_bm_num_gs_steps = false;

  /// Sampling mode for the Boltzmann machine
  int _bm_sampling_mode = 1;
  bool _with_bm_sampling_mode = false;

  /// Number of allowed function evaluations (<= 0 means indefinite)
  int _budget = 10000;
  bool _with_budget = false;

  /// Size of bit vectors
  int _bv_size = 100;
  bool _with_bv_size = false;

  /// Path of the description file
  std::string _description_path = "description.txt";
  bool _with_description_path = false;

  /// Crossover bias
  double _ea_crossover_bias = 0.5;
  bool _with_ea_crossover_bias = false;

  /// Crossover probability
  double _ea_crossover_probability = 0.5;
  bool _with_ea_crossover_probability = false;

  /// Initial Hamming weight
  int _ea_it_initial_hamming_weight = 0;
  bool _with_ea_it_initial_hamming_weight = false;

  /// Selection for replacement in it-EA
  int _ea_it_replacement = 0;
  bool _with_ea_it_replacement = false;

  /// Offspring population size
  int _ea_lambda = 100;
  bool _with_ea_lambda = false;

  /// Parent population size
  int _ea_mu = 10;
  bool _with_ea_mu = false;

  /// Mutation rate relative to bv_size (fixed or initial value)
  double _ea_mutation_rate = 1;
  bool _with_ea_mutation_rate = false;

  /// Maximum mutation rate
  double _ea_mutation_rate_max = 1;
  bool _with_ea_mutation_rate_max = false;

  /// Minimum mutation rate
  double _ea_mutation_rate_min = 0.01;
  bool _with_ea_mutation_rate_min = false;

  /// Success rate for for self-adjusting mutation rate
  double _ea_success_ratio = 4;
  bool _with_ea_success_ratio = false;

  /// Tournament size
  int _ea_tournament_size = 2;
  bool _with_ea_tournament_size = false;

  /// Update strength for self-adjusting mutation rate
  double _ea_update_strength = 1.01;
  bool _with_ea_update_strength = false;

  /// Expression of the variable x
  std::string _expression = "x";
  bool _with_expression = false;

  /// Name of the function in the dynamic library
  std::string _fn_name;
  bool _with_fn_name = false;

  /// Number of traps
  int _fn_num_traps = 10;
  bool _with_fn_num_traps = false;

  /// Prefix length for long path
  int _fn_prefix_length = 2;
  bool _with_fn_prefix_length = false;

  /// Threshold (in bits) for Jump, Four Peaks, and Six Peaks
  int _fn_threshold = 10;
  bool _with_fn_threshold = false;

  /// Default precision of double representations
  double _fp_default_double_precision;
  bool _with_fp_default_double_precision = false;

  /// Default representation for double
  std::string _fp_default_double_rep = "double(0, 1, precision = 1e-3)";
  bool _with_fp_default_double_rep = false;

  /// Default size of double representations
  int _fp_default_double_size;
  bool _with_fp_default_double_size = false;

  /// Default representation for int
  std::string _fp_default_int_rep = "int(1, 100)";
  bool _with_fp_default_int_rep = false;

  /// Default representation for long
  std::string _fp_default_long_rep = "long(1, 100)";
  bool _with_fp_default_long_rep = false;

  /// Mathematical expression
  std::string _fp_expression = "(1-x)^2+100*(y-x^2)^2";
  bool _with_fp_expression = false;

  /// Source for the expression to parse
  int _fp_expression_source = 0;
  bool _with_fp_expression_source = false;

  /// Representations. Example: \"x: double(0, 1); y: double(0, 1, precision = 1e-3); z: double(0, 1, size = 8); u: int(-100, 100); v: long(1, 10000)\"
  std::string _fp_representations;
  bool _with_fp_representations = false;

  /// Path of the representations file
  std::string _fp_representations_path = "representations.txt";
  bool _with_fp_representations_path = false;

  /// Source for the representations
  int _fp_representations_source = 0;
  bool _with_fp_representations_source = false;

  /// Type of function
  int _function = 0;
  bool _with_function = false;

  /// Reset period (<= 0 means no reset)
  int _hea_reset_period = 0;
  bool _with_hea_reset_period = false;

  /// Learning rate
  double _learning_rate = 0.001;
  bool _with_learning_rate = false;

  /// Type of map
  int _map = 0;
  bool _with_map = false;

  /// Input size of linear and affine maps
  int _map_input_size = 100;
  bool _with_map_input_size = false;

  /// Path of the map file
  std::string _map_path = "map.txt";
  bool _with_map_path = false;

  /// Transvection sequence length
  int _map_ts_length = 10;
  bool _with_map_ts_length = false;

  /// Transvection sequence sampling mode
  int _map_ts_sampling_mode = 0;
  bool _with_map_ts_sampling_mode = false;

  /// Type of neighborhood
  int _neighborhood = 0;
  bool _with_neighborhood = false;

  /// Type of neighborhood iterator
  int _neighborhood_iterator = 0;
  bool _with_neighborhood_iterator = false;

  /// Noise standard deviation
  double _noise_stddev = 1;
  bool _with_noise_stddev = false;

  /// Number of iterations (<= 0 means indefinite)
  int _num_iterations = 0;
  bool _with_num_iterations = false;

  /// Number of threads
  int _num_threads = 1;
  bool _with_num_threads = false;

  /// Path of the function file
  std::string _path = "function.txt";
  bool _with_path = false;

  /// Mutation rate relative to bv_size
  double _pn_mutation_rate = 1;
  bool _with_pn_mutation_rate = false;

  /// Type of neighborhood
  int _pn_neighborhood = 0;
  bool _with_pn_neighborhood = false;

  /// Radius of Hamming ball or sphere
  int _pn_radius = 2;
  bool _with_pn_radius = false;

  /// Population size
  int _population_size = 10;
  bool _with_population_size = false;

  /// Number of probability vector components to log
  int _pv_log_num_components = 5;
  bool _with_pv_log_num_components = false;

  /// Radius of Hamming ball or sphere
  int _radius = 2;
  bool _with_radius = false;

  /// Categorical representation
  int _rep_categorical_representation = 0;
  bool _with_rep_categorical_representation = false;

  /// Number of additional bits per element for permutation representation
  int _rep_num_additional_bits = 2;
  bool _with_rep_num_additional_bits = false;

  /// Path of the results file
  std::string _results_path = "results.json";
  bool _with_results_path = false;

  /// Number of consecutive rejected moves before ending the search (<= 0 means infinite)
  int _rls_patience = 50;
  bool _with_rls_patience = false;

  /// Ratio for beta or inverse temperature
  double _sa_beta_ratio = 1.2;
  bool _with_sa_beta_ratio = false;

  /// Initial acceptance probability
  double _sa_initial_acceptance_probability = 0.6;
  bool _with_sa_initial_acceptance_probability = false;

  /// Number of accepted transitions before annealing
  int _sa_num_transitions = 50;
  bool _with_sa_num_transitions = false;

  /// Number of trials to estimate initial inverse temperature
  int _sa_num_trials = 100;
  bool _with_sa_num_trials = false;

  /// Seed for the random number generator
  unsigned _seed;
  bool _with_seed = false;

  /// Selection size (number of selected individuals)
  int _selection_size = 1;
  bool _with_selection_size = false;

  /// Path of the solution file
  std::string _solution_path = "solution.txt";
  bool _with_solution_path = false;

  /// Target
  double _target = 100;
  bool _with_target = false;

  /// Additive Gaussian noise
  bool _additive_gaussian_noise = false;

  /// Log 1-norm of the parameters
  bool _bm_log_norm_1 = false;

  /// Log infinite norm of the parameters
  bool _bm_log_norm_infinite = false;

  /// Negative and positive selection
  bool _bm_negative_positive_selection = false;

  /// Cache function evaluations
  bool _cache = false;

  /// Set cache on budget
  bool _cache_budget = false;

  /// Print or save the solution in the domain of the concrete function
  bool _concrete_solution = false;

  /// Allow no mutation with standard bit mutation
  bool _ea_allow_no_mutation = false;

  /// Log center fitness
  bool _ea_it_log_center_fitness = false;

  /// Log mutation rate
  bool _ea_log_mutation_rate = false;

  /// Display the function and exit
  bool _fn_display = false;

  /// Print the size of bit vectors
  bool _fn_get_bv_size = false;

  /// If the maximum is known then print it and exit with status 0 else exit with status 1
  bool _fn_get_maximum = false;

  /// Check whether the function has a known maximum
  bool _fn_has_known_maximum = false;

  /// Check whether the function provides incremental evaluation
  bool _fn_provides_incremental_evaluation = false;

  /// Compute the Walsh transform of the function
  bool _fn_walsh_transform = false;

  /// Bound moment after update
  bool _hea_bound_moment = false;

  /// Log delta (moment increment) 2-norm
  bool _hea_log_delta_norm = false;

  /// Log herding error (moment discrepancy)
  bool _hea_log_herding_error = false;

  /// Log target moment as a symmetric matrix
  bool _hea_log_target = false;

  /// Log target 2-norm (distance to uniform moment)
  bool _hea_log_target_norm = false;

  /// Randomize bit order
  bool _hea_randomize_bit_order = false;

  /// Incremental evaluation
  bool _incremental_evaluation = false;

  /// Load a solution from a file
  bool _load_solution = false;

  /// Log improvement
  bool _log_improvement = false;

  /// Display the map and exit
  bool _map_display = false;

  /// Sample a random map
  bool _map_random = false;

  /// Ensure that the sampled linear or affine map is surjective
  bool _map_surjective = false;

  /// Strict (>) max-min ant system
  bool _mmas_strict = false;

  /// Negation (hence minimization) of the function
  bool _negation = false;

  /// Parsed modifier
  bool _parsed_modifier = false;

  /// Allow no mutation with standard bit mutation
  bool _pn_allow_no_mutation = false;

  /// Print the default parameters and exit
  bool _print_default_parameters = false;

  /// Print a description of the solution
  bool _print_description = false;

  /// Print the parameters
  bool _print_parameters = false;

  /// Print results
  bool _print_results = false;

  /// Print the solution
  bool _print_solution = false;

  /// Prior noise
  bool _prior_noise = false;

  /// Log entropy of probability vector
  bool _pv_log_entropy = false;

  /// Log probability vector
  bool _pv_log_pv = false;

  /// Record evaluation time
  bool _record_evaluation_time = false;

  /// Record total time
  bool _record_total_time = false;

  /// Restart any algorithm an indefinite number of times
  bool _restart = false;

  /// Strict (>) random local search
  bool _rls_strict = false;

  /// Log bit vector value during random walk
  bool _rw_log_value = false;

  /// Save the description of the solution in a file
  bool _save_description = false;

  /// Save the results in a file
  bool _save_results = false;

  /// Save the solution in a file
  bool _save_solution = false;

  /// Stop on maximum
  bool _stop_on_maximum = false;

  /// Stop on target
  bool _stop_on_target = false;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print help message for section fn
  void print_help_fn(std::ostream& stream) const;

  /// Print help message for section fp
  void print_help_fp(std::ostream& stream) const;

  /// Print help message for section rep
  void print_help_rep(std::ostream& stream) const;

  /// Print help message for section mod
  void print_help_mod(std::ostream& stream) const;

  /// Print help message for section ctrl
  void print_help_ctrl(std::ostream& stream) const;

  /// Print help message for section pn
  void print_help_pn(std::ostream& stream) const;

  /// Print help message for section map
  void print_help_map(std::ostream& stream) const;

  /// Print help message for section alg
  void print_help_alg(std::ostream& stream) const;

  /// Print help message for section ls
  void print_help_ls(std::ostream& stream) const;

  /// Print help message for section sa
  void print_help_sa(std::ostream& stream) const;

  /// Print help message for section ea
  void print_help_ea(std::ostream& stream) const;

  /// Print help message for section eda
  void print_help_eda(std::ostream& stream) const;

  /// Print help message for section hea
  void print_help_hea(std::ostream& stream) const;

  /// Print help message for section bm
  void print_help_bm(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Default constructor
  HncoOptions(): _exec_name("hnco") {}

  /// Constructor
  HncoOptions(int argc, char *argv[], bool ignore_bad_options = false);

  /// Get the value of algorithm
  int get_algorithm() const { return _algorithm; }

  /// With parameter algorithm
  bool with_algorithm() const { return _with_algorithm; }

  /// Get the value of bm_mc_reset_strategy
  int get_bm_mc_reset_strategy() const { return _bm_mc_reset_strategy; }

  /// With parameter bm_mc_reset_strategy
  bool with_bm_mc_reset_strategy() const { return _with_bm_mc_reset_strategy; }

  /// Get the value of bm_num_gs_cycles
  int get_bm_num_gs_cycles() const { return _bm_num_gs_cycles; }

  /// With parameter bm_num_gs_cycles
  bool with_bm_num_gs_cycles() const { return _with_bm_num_gs_cycles; }

  /// Get the value of bm_num_gs_steps
  int get_bm_num_gs_steps() const { return _bm_num_gs_steps; }

  /// With parameter bm_num_gs_steps
  bool with_bm_num_gs_steps() const { return _with_bm_num_gs_steps; }

  /// Get the value of bm_sampling_mode
  int get_bm_sampling_mode() const { return _bm_sampling_mode; }

  /// With parameter bm_sampling_mode
  bool with_bm_sampling_mode() const { return _with_bm_sampling_mode; }

  /// Get the value of budget
  int get_budget() const { return _budget; }

  /// With parameter budget
  bool with_budget() const { return _with_budget; }

  /// Get the value of bv_size
  int get_bv_size() const { return _bv_size; }

  /// With parameter bv_size
  bool with_bv_size() const { return _with_bv_size; }

  /// Get the value of description_path
  std::string get_description_path() const { return _description_path; }

  /// With parameter description_path
  bool with_description_path() const { return _with_description_path; }

  /// Get the value of ea_crossover_bias
  double get_ea_crossover_bias() const { return _ea_crossover_bias; }

  /// With parameter ea_crossover_bias
  bool with_ea_crossover_bias() const { return _with_ea_crossover_bias; }

  /// Get the value of ea_crossover_probability
  double get_ea_crossover_probability() const { return _ea_crossover_probability; }

  /// With parameter ea_crossover_probability
  bool with_ea_crossover_probability() const { return _with_ea_crossover_probability; }

  /// Get the value of ea_it_initial_hamming_weight
  int get_ea_it_initial_hamming_weight() const { return _ea_it_initial_hamming_weight; }

  /// With parameter ea_it_initial_hamming_weight
  bool with_ea_it_initial_hamming_weight() const { return _with_ea_it_initial_hamming_weight; }

  /// Get the value of ea_it_replacement
  int get_ea_it_replacement() const { return _ea_it_replacement; }

  /// With parameter ea_it_replacement
  bool with_ea_it_replacement() const { return _with_ea_it_replacement; }

  /// Get the value of ea_lambda
  int get_ea_lambda() const { return _ea_lambda; }

  /// With parameter ea_lambda
  bool with_ea_lambda() const { return _with_ea_lambda; }

  /// Get the value of ea_mu
  int get_ea_mu() const { return _ea_mu; }

  /// With parameter ea_mu
  bool with_ea_mu() const { return _with_ea_mu; }

  /// Get the value of ea_mutation_rate
  double get_ea_mutation_rate() const { return _ea_mutation_rate; }

  /// With parameter ea_mutation_rate
  bool with_ea_mutation_rate() const { return _with_ea_mutation_rate; }

  /// Get the value of ea_mutation_rate_max
  double get_ea_mutation_rate_max() const { return _ea_mutation_rate_max; }

  /// With parameter ea_mutation_rate_max
  bool with_ea_mutation_rate_max() const { return _with_ea_mutation_rate_max; }

  /// Get the value of ea_mutation_rate_min
  double get_ea_mutation_rate_min() const { return _ea_mutation_rate_min; }

  /// With parameter ea_mutation_rate_min
  bool with_ea_mutation_rate_min() const { return _with_ea_mutation_rate_min; }

  /// Get the value of ea_success_ratio
  double get_ea_success_ratio() const { return _ea_success_ratio; }

  /// With parameter ea_success_ratio
  bool with_ea_success_ratio() const { return _with_ea_success_ratio; }

  /// Get the value of ea_tournament_size
  int get_ea_tournament_size() const { return _ea_tournament_size; }

  /// With parameter ea_tournament_size
  bool with_ea_tournament_size() const { return _with_ea_tournament_size; }

  /// Get the value of ea_update_strength
  double get_ea_update_strength() const { return _ea_update_strength; }

  /// With parameter ea_update_strength
  bool with_ea_update_strength() const { return _with_ea_update_strength; }

  /// Get the value of expression
  std::string get_expression() const { return _expression; }

  /// With parameter expression
  bool with_expression() const { return _with_expression; }

  /// Get the value of fn_name
  std::string get_fn_name() const {
    if (_with_fn_name)
      return _fn_name;
    else
      throw std::runtime_error("HncoOptions::get_fn_name: Parameter fn_name has no default value and has not been set");
    }

  /// With parameter fn_name
  bool with_fn_name() const { return _with_fn_name; }

  /// Get the value of fn_num_traps
  int get_fn_num_traps() const { return _fn_num_traps; }

  /// With parameter fn_num_traps
  bool with_fn_num_traps() const { return _with_fn_num_traps; }

  /// Get the value of fn_prefix_length
  int get_fn_prefix_length() const { return _fn_prefix_length; }

  /// With parameter fn_prefix_length
  bool with_fn_prefix_length() const { return _with_fn_prefix_length; }

  /// Get the value of fn_threshold
  int get_fn_threshold() const { return _fn_threshold; }

  /// With parameter fn_threshold
  bool with_fn_threshold() const { return _with_fn_threshold; }

  /// Get the value of fp_default_double_precision
  double get_fp_default_double_precision() const {
    if (_with_fp_default_double_precision)
      return _fp_default_double_precision;
    else
      throw std::runtime_error("HncoOptions::get_fp_default_double_precision: Parameter fp_default_double_precision has no default value and has not been set");
    }

  /// With parameter fp_default_double_precision
  bool with_fp_default_double_precision() const { return _with_fp_default_double_precision; }

  /// Get the value of fp_default_double_rep
  std::string get_fp_default_double_rep() const { return _fp_default_double_rep; }

  /// With parameter fp_default_double_rep
  bool with_fp_default_double_rep() const { return _with_fp_default_double_rep; }

  /// Get the value of fp_default_double_size
  int get_fp_default_double_size() const {
    if (_with_fp_default_double_size)
      return _fp_default_double_size;
    else
      throw std::runtime_error("HncoOptions::get_fp_default_double_size: Parameter fp_default_double_size has no default value and has not been set");
    }

  /// With parameter fp_default_double_size
  bool with_fp_default_double_size() const { return _with_fp_default_double_size; }

  /// Get the value of fp_default_int_rep
  std::string get_fp_default_int_rep() const { return _fp_default_int_rep; }

  /// With parameter fp_default_int_rep
  bool with_fp_default_int_rep() const { return _with_fp_default_int_rep; }

  /// Get the value of fp_default_long_rep
  std::string get_fp_default_long_rep() const { return _fp_default_long_rep; }

  /// With parameter fp_default_long_rep
  bool with_fp_default_long_rep() const { return _with_fp_default_long_rep; }

  /// Get the value of fp_expression
  std::string get_fp_expression() const { return _fp_expression; }

  /// With parameter fp_expression
  bool with_fp_expression() const { return _with_fp_expression; }

  /// Get the value of fp_expression_source
  int get_fp_expression_source() const { return _fp_expression_source; }

  /// With parameter fp_expression_source
  bool with_fp_expression_source() const { return _with_fp_expression_source; }

  /// Get the value of fp_representations
  std::string get_fp_representations() const {
    if (_with_fp_representations)
      return _fp_representations;
    else
      throw std::runtime_error("HncoOptions::get_fp_representations: Parameter fp_representations has no default value and has not been set");
    }

  /// With parameter fp_representations
  bool with_fp_representations() const { return _with_fp_representations; }

  /// Get the value of fp_representations_path
  std::string get_fp_representations_path() const { return _fp_representations_path; }

  /// With parameter fp_representations_path
  bool with_fp_representations_path() const { return _with_fp_representations_path; }

  /// Get the value of fp_representations_source
  int get_fp_representations_source() const { return _fp_representations_source; }

  /// With parameter fp_representations_source
  bool with_fp_representations_source() const { return _with_fp_representations_source; }

  /// Get the value of function
  int get_function() const { return _function; }

  /// With parameter function
  bool with_function() const { return _with_function; }

  /// Get the value of hea_reset_period
  int get_hea_reset_period() const { return _hea_reset_period; }

  /// With parameter hea_reset_period
  bool with_hea_reset_period() const { return _with_hea_reset_period; }

  /// Get the value of learning_rate
  double get_learning_rate() const { return _learning_rate; }

  /// With parameter learning_rate
  bool with_learning_rate() const { return _with_learning_rate; }

  /// Get the value of map
  int get_map() const { return _map; }

  /// With parameter map
  bool with_map() const { return _with_map; }

  /// Get the value of map_input_size
  int get_map_input_size() const { return _map_input_size; }

  /// With parameter map_input_size
  bool with_map_input_size() const { return _with_map_input_size; }

  /// Get the value of map_path
  std::string get_map_path() const { return _map_path; }

  /// With parameter map_path
  bool with_map_path() const { return _with_map_path; }

  /// Get the value of map_ts_length
  int get_map_ts_length() const { return _map_ts_length; }

  /// With parameter map_ts_length
  bool with_map_ts_length() const { return _with_map_ts_length; }

  /// Get the value of map_ts_sampling_mode
  int get_map_ts_sampling_mode() const { return _map_ts_sampling_mode; }

  /// With parameter map_ts_sampling_mode
  bool with_map_ts_sampling_mode() const { return _with_map_ts_sampling_mode; }

  /// Get the value of neighborhood
  int get_neighborhood() const { return _neighborhood; }

  /// With parameter neighborhood
  bool with_neighborhood() const { return _with_neighborhood; }

  /// Get the value of neighborhood_iterator
  int get_neighborhood_iterator() const { return _neighborhood_iterator; }

  /// With parameter neighborhood_iterator
  bool with_neighborhood_iterator() const { return _with_neighborhood_iterator; }

  /// Get the value of noise_stddev
  double get_noise_stddev() const { return _noise_stddev; }

  /// With parameter noise_stddev
  bool with_noise_stddev() const { return _with_noise_stddev; }

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

  /// Get the value of pn_mutation_rate
  double get_pn_mutation_rate() const { return _pn_mutation_rate; }

  /// With parameter pn_mutation_rate
  bool with_pn_mutation_rate() const { return _with_pn_mutation_rate; }

  /// Get the value of pn_neighborhood
  int get_pn_neighborhood() const { return _pn_neighborhood; }

  /// With parameter pn_neighborhood
  bool with_pn_neighborhood() const { return _with_pn_neighborhood; }

  /// Get the value of pn_radius
  int get_pn_radius() const { return _pn_radius; }

  /// With parameter pn_radius
  bool with_pn_radius() const { return _with_pn_radius; }

  /// Get the value of population_size
  int get_population_size() const { return _population_size; }

  /// With parameter population_size
  bool with_population_size() const { return _with_population_size; }

  /// Get the value of pv_log_num_components
  int get_pv_log_num_components() const { return _pv_log_num_components; }

  /// With parameter pv_log_num_components
  bool with_pv_log_num_components() const { return _with_pv_log_num_components; }

  /// Get the value of radius
  int get_radius() const { return _radius; }

  /// With parameter radius
  bool with_radius() const { return _with_radius; }

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

  /// Get the value of rls_patience
  int get_rls_patience() const { return _rls_patience; }

  /// With parameter rls_patience
  bool with_rls_patience() const { return _with_rls_patience; }

  /// Get the value of sa_beta_ratio
  double get_sa_beta_ratio() const { return _sa_beta_ratio; }

  /// With parameter sa_beta_ratio
  bool with_sa_beta_ratio() const { return _with_sa_beta_ratio; }

  /// Get the value of sa_initial_acceptance_probability
  double get_sa_initial_acceptance_probability() const { return _sa_initial_acceptance_probability; }

  /// With parameter sa_initial_acceptance_probability
  bool with_sa_initial_acceptance_probability() const { return _with_sa_initial_acceptance_probability; }

  /// Get the value of sa_num_transitions
  int get_sa_num_transitions() const { return _sa_num_transitions; }

  /// With parameter sa_num_transitions
  bool with_sa_num_transitions() const { return _with_sa_num_transitions; }

  /// Get the value of sa_num_trials
  int get_sa_num_trials() const { return _sa_num_trials; }

  /// With parameter sa_num_trials
  bool with_sa_num_trials() const { return _with_sa_num_trials; }

  /// Get the value of seed
  unsigned get_seed() const {
    if (_with_seed)
      return _seed;
    else
      throw std::runtime_error("HncoOptions::get_seed: Parameter seed has no default value and has not been set");
    }

  /// With parameter seed
  bool with_seed() const { return _with_seed; }

  /// Get the value of selection_size
  int get_selection_size() const { return _selection_size; }

  /// With parameter selection_size
  bool with_selection_size() const { return _with_selection_size; }

  /// Get the value of solution_path
  std::string get_solution_path() const { return _solution_path; }

  /// With parameter solution_path
  bool with_solution_path() const { return _with_solution_path; }

  /// Get the value of target
  double get_target() const { return _target; }

  /// With parameter target
  bool with_target() const { return _with_target; }

  /// With the flag additive_gaussian_noise
  bool with_additive_gaussian_noise() const { return _additive_gaussian_noise; }

  /// With the flag bm_log_norm_1
  bool with_bm_log_norm_1() const { return _bm_log_norm_1; }

  /// With the flag bm_log_norm_infinite
  bool with_bm_log_norm_infinite() const { return _bm_log_norm_infinite; }

  /// With the flag bm_negative_positive_selection
  bool with_bm_negative_positive_selection() const { return _bm_negative_positive_selection; }

  /// With the flag cache
  bool with_cache() const { return _cache; }

  /// With the flag cache_budget
  bool with_cache_budget() const { return _cache_budget; }

  /// With the flag concrete_solution
  bool with_concrete_solution() const { return _concrete_solution; }

  /// With the flag ea_allow_no_mutation
  bool with_ea_allow_no_mutation() const { return _ea_allow_no_mutation; }

  /// With the flag ea_it_log_center_fitness
  bool with_ea_it_log_center_fitness() const { return _ea_it_log_center_fitness; }

  /// With the flag ea_log_mutation_rate
  bool with_ea_log_mutation_rate() const { return _ea_log_mutation_rate; }

  /// With the flag fn_display
  bool with_fn_display() const { return _fn_display; }

  /// With the flag fn_get_bv_size
  bool with_fn_get_bv_size() const { return _fn_get_bv_size; }

  /// With the flag fn_get_maximum
  bool with_fn_get_maximum() const { return _fn_get_maximum; }

  /// With the flag fn_has_known_maximum
  bool with_fn_has_known_maximum() const { return _fn_has_known_maximum; }

  /// With the flag fn_provides_incremental_evaluation
  bool with_fn_provides_incremental_evaluation() const { return _fn_provides_incremental_evaluation; }

  /// With the flag fn_walsh_transform
  bool with_fn_walsh_transform() const { return _fn_walsh_transform; }

  /// With the flag hea_bound_moment
  bool with_hea_bound_moment() const { return _hea_bound_moment; }

  /// With the flag hea_log_delta_norm
  bool with_hea_log_delta_norm() const { return _hea_log_delta_norm; }

  /// With the flag hea_log_herding_error
  bool with_hea_log_herding_error() const { return _hea_log_herding_error; }

  /// With the flag hea_log_target
  bool with_hea_log_target() const { return _hea_log_target; }

  /// With the flag hea_log_target_norm
  bool with_hea_log_target_norm() const { return _hea_log_target_norm; }

  /// With the flag hea_randomize_bit_order
  bool with_hea_randomize_bit_order() const { return _hea_randomize_bit_order; }

  /// With the flag incremental_evaluation
  bool with_incremental_evaluation() const { return _incremental_evaluation; }

  /// With the flag load_solution
  bool with_load_solution() const { return _load_solution; }

  /// With the flag log_improvement
  bool with_log_improvement() const { return _log_improvement; }

  /// With the flag map_display
  bool with_map_display() const { return _map_display; }

  /// With the flag map_random
  bool with_map_random() const { return _map_random; }

  /// With the flag map_surjective
  bool with_map_surjective() const { return _map_surjective; }

  /// With the flag mmas_strict
  bool with_mmas_strict() const { return _mmas_strict; }

  /// With the flag negation
  bool with_negation() const { return _negation; }

  /// With the flag parsed_modifier
  bool with_parsed_modifier() const { return _parsed_modifier; }

  /// With the flag pn_allow_no_mutation
  bool with_pn_allow_no_mutation() const { return _pn_allow_no_mutation; }

  /// With the flag print_default_parameters
  bool with_print_default_parameters() const { return _print_default_parameters; }

  /// With the flag print_description
  bool with_print_description() const { return _print_description; }

  /// With the flag print_parameters
  bool with_print_parameters() const { return _print_parameters; }

  /// With the flag print_results
  bool with_print_results() const { return _print_results; }

  /// With the flag print_solution
  bool with_print_solution() const { return _print_solution; }

  /// With the flag prior_noise
  bool with_prior_noise() const { return _prior_noise; }

  /// With the flag pv_log_entropy
  bool with_pv_log_entropy() const { return _pv_log_entropy; }

  /// With the flag pv_log_pv
  bool with_pv_log_pv() const { return _pv_log_pv; }

  /// With the flag record_evaluation_time
  bool with_record_evaluation_time() const { return _record_evaluation_time; }

  /// With the flag record_total_time
  bool with_record_total_time() const { return _record_total_time; }

  /// With the flag restart
  bool with_restart() const { return _restart; }

  /// With the flag rls_strict
  bool with_rls_strict() const { return _rls_strict; }

  /// With the flag rw_log_value
  bool with_rw_log_value() const { return _rw_log_value; }

  /// With the flag save_description
  bool with_save_description() const { return _save_description; }

  /// With the flag save_results
  bool with_save_results() const { return _save_results; }

  /// With the flag save_solution
  bool with_save_solution() const { return _save_solution; }

  /// With the flag stop_on_maximum
  bool with_stop_on_maximum() const { return _stop_on_maximum; }

  /// With the flag stop_on_target
  bool with_stop_on_target() const { return _stop_on_target; }

  friend std::ostream& operator<<(std::ostream&, const HncoOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const HncoOptions& options);

}
}

#endif
