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
  std::string _version;

  /// Type of algorithm
  int _algorithm;
  bool _opt_algorithm;

  /// Markov chain reset strategy
  int _bm_mc_reset_strategy;
  bool _opt_bm_mc_reset_strategy;

  /// Number of Gibbs sampler cycles per bit vector
  int _bm_num_gs_cycles;
  bool _opt_bm_num_gs_cycles;

  /// Number of Gibbs sampler steps per bit vector
  int _bm_num_gs_steps;
  bool _opt_bm_num_gs_steps;

  /// Sampling mode for the Boltzmann machine
  int _bm_sampling;
  bool _opt_bm_sampling;

  /// Number of allowed function evaluations (<= 0 means indefinite)
  int _budget;
  bool _opt_budget;

  /// Size of bit vectors
  int _bv_size;
  bool _opt_bv_size;

  /// Path of the description file
  std::string _description_path;
  bool _opt_description_path;

  /// Crossover bias
  double _ea_crossover_bias;
  bool _opt_ea_crossover_bias;

  /// Crossover probability
  double _ea_crossover_probability;
  bool _opt_ea_crossover_probability;

  /// Offspring population size
  int _ea_lambda;
  bool _opt_ea_lambda;

  /// Parent population size
  int _ea_mu;
  bool _opt_ea_mu;

  /// Mutation rate relative to bv_size (fixed or initial value)
  double _ea_mutation_rate;
  bool _opt_ea_mutation_rate;

  /// Maximum mutation rate
  double _ea_mutation_rate_max;
  bool _opt_ea_mutation_rate_max;

  /// Minimum mutation rate
  double _ea_mutation_rate_min;
  bool _opt_ea_mutation_rate_min;

  /// Success rate for for self-adjusting mutation rate
  double _ea_success_ratio;
  bool _opt_ea_success_ratio;

  /// Tournament size
  int _ea_tournament_size;
  bool _opt_ea_tournament_size;

  /// Update strength for self-adjusting mutation rate
  double _ea_update_strength;
  bool _opt_ea_update_strength;

  /// Expression of the variable x
  std::string _expression;
  bool _opt_expression;

  /// Name of the function in the dynamic library
  std::string _fn_name;
  bool _opt_fn_name;

  /// Number of traps
  int _fn_num_traps;
  bool _opt_fn_num_traps;

  /// Prefix length for long path
  int _fn_prefix_length;
  bool _opt_fn_prefix_length;

  /// Threshold (in bits) for Jump, Four Peaks, and Six Peaks
  int _fn_threshold;
  bool _opt_fn_threshold;

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

  /// Reset period (<= 0 means no reset)
  int _hea_reset_period;
  bool _opt_hea_reset_period;

  /// Learning rate
  double _learning_rate;
  bool _opt_learning_rate;

  /// Type of map
  int _map;
  bool _opt_map;

  /// Input size of linear and affine maps
  int _map_input_size;
  bool _opt_map_input_size;

  /// Path of a map file
  std::string _map_path;
  bool _opt_map_path;

  /// Transvection sequence length
  int _map_ts_length;
  bool _opt_map_ts_length;

  /// Transvection sequence sampling mode
  int _map_ts_sampling_mode;
  bool _opt_map_ts_sampling_mode;

  /// Type of neighborhood
  int _neighborhood;
  bool _opt_neighborhood;

  /// Type of neighborhood iterator
  int _neighborhood_iterator;
  bool _opt_neighborhood_iterator;

  /// Noise standard deviation
  double _noise_stddev;
  bool _opt_noise_stddev;

  /// Number of iterations (<= 0 means indefinite)
  int _num_iterations;
  bool _opt_num_iterations;

  /// Number of threads
  int _num_threads;
  bool _opt_num_threads;

  /// Path of a function file
  std::string _path;
  bool _opt_path;

  /// Mutation rate relative to bv_size
  double _pn_mutation_rate;
  bool _opt_pn_mutation_rate;

  /// Type of neighborhood
  int _pn_neighborhood;
  bool _opt_pn_neighborhood;

  /// Radius of Hamming ball or sphere
  int _pn_radius;
  bool _opt_pn_radius;

  /// Population size
  int _population_size;
  bool _opt_population_size;

  /// Number of probability vector components to log
  int _pv_log_num_components;
  bool _opt_pv_log_num_components;

  /// Radius of Hamming ball or sphere
  int _radius;
  bool _opt_radius;

  /// Categorical representation
  int _rep_categorical_representation;
  bool _opt_rep_categorical_representation;

  /// Number of additional bits per element for permutation representation
  int _rep_num_additional_bits;
  bool _opt_rep_num_additional_bits;

  /// Path of the results file
  std::string _results_path;
  bool _opt_results_path;

  /// Number of consecutive rejected moves before ending the search (<= 0 means infinite)
  int _rls_patience;
  bool _opt_rls_patience;

  /// Ratio for beta or inverse temperature
  double _sa_beta_ratio;
  bool _opt_sa_beta_ratio;

  /// Initial acceptance probability
  double _sa_initial_acceptance_probability;
  bool _opt_sa_initial_acceptance_probability;

  /// Number of accepted transitions before annealing
  int _sa_num_transitions;
  bool _opt_sa_num_transitions;

  /// Number of trials to estimate initial inverse temperature
  int _sa_num_trials;
  bool _opt_sa_num_trials;

  /// Seed for the random number generator
  unsigned _seed;
  bool _opt_seed;

  /// Selection size (number of selected individuals)
  int _selection_size;
  bool _opt_selection_size;

  /// Path of the solution file
  std::string _solution_path;
  bool _opt_solution_path;

  /// Target
  double _target;
  bool _opt_target;

  /// Additive Gaussian noise
  bool _additive_gaussian_noise;

  /// Log 1-norm of the parameters
  bool _bm_log_norm_1;

  /// Log infinite norm of the parameters
  bool _bm_log_norm_infinite;

  /// Negative and positive selection
  bool _bm_negative_positive_selection;

  /// Cache function evaluations
  bool _cache;

  /// Set cache on budget
  bool _cache_budget;

  /// Print or save the solution in the domain of the concrete function
  bool _concrete_solution;

  /// Allow no mutation with standard bit mutation
  bool _ea_allow_no_mutation;

  /// Log mutation rate
  bool _ea_log_mutation_rate;

  /// Display the function and exit
  bool _fn_display;

  /// Print the size of bit vectors
  bool _fn_get_bv_size;

  /// If the maximum is known then print it and exit with status 0 else exit with status 1
  bool _fn_get_maximum;

  /// Does the function have a known maximum?
  bool _fn_has_known_maximum;

  /// Does the function provide incremental evaluation?
  bool _fn_provides_incremental_evaluation;

  /// Compute the Walsh transform of the function
  bool _fn_walsh_transform;

  /// Bound moment after update
  bool _hea_bound_moment;

  /// Log delta (moment increment) 2-norm
  bool _hea_log_delta_norm;

  /// Log herding error (moment discrepancy)
  bool _hea_log_herding_error;

  /// Log target moment as a symmetric matrix
  bool _hea_log_target;

  /// Log target 2-norm (distance to uniform moment)
  bool _hea_log_target_norm;

  /// Randomize bit order
  bool _hea_randomize_bit_order;

  /// Incremental evaluation
  bool _incremental_evaluation;

  /// Load a solution from a file
  bool _load_solution;

  /// Log improvement
  bool _log_improvement;

  /// Display the map and exit
  bool _map_display;

  /// Sample a random map
  bool _map_random;

  /// Ensure that the sampled linear or affine map is surjective
  bool _map_surjective;

  /// Strict (>) max-min ant system
  bool _mmas_strict;

  /// Negation (hence minimization) of the function
  bool _negation;

  /// Parsed modifier
  bool _parsed_modifier;

  /// Allow no mutation with standard bit mutation
  bool _pn_allow_no_mutation;

  /// Print the default parameters and exit
  bool _print_default_parameters;

  /// Print a description of the solution
  bool _print_description;

  /// Print the parameters
  bool _print_parameters;

  /// Print results
  bool _print_results;

  /// Print the solution
  bool _print_solution;

  /// Prior noise
  bool _prior_noise;

  /// Log entropy of probability vector
  bool _pv_log_entropy;

  /// Log probability vector
  bool _pv_log_pv;

  /// Record evaluation time
  bool _record_evaluation_time;

  /// Restart any algorithm an indefinite number of times
  bool _restart;

  /// Strict (>) random local search
  bool _rls_strict;

  /// Log bit vector value during random walk
  bool _rw_log_value;

  /// Save a description of the solution in a file
  bool _save_description;

  /// Save the results in a file
  bool _save_results;

  /// Save the solution in a file
  bool _save_solution;

  /// Stop on maximum
  bool _stop_on_maximum;

  /// Stop on target
  bool _stop_on_target;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print help message for section fp
  void print_help_fp(std::ostream& stream) const;

  /// Print help message for section rep
  void print_help_rep(std::ostream& stream) const;

  /// Print help message for section pn
  void print_help_pn(std::ostream& stream) const;

  /// Print help message for section map
  void print_help_map(std::ostream& stream) const;

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
  HncoOptions();

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

  /// Get bm_mc_reset_strategy
  int get_bm_mc_reset_strategy() const { return _bm_mc_reset_strategy; }

  /// Set bm_mc_reset_strategy
  void set_bm_mc_reset_strategy(int x) {
    _bm_mc_reset_strategy = x;
    _opt_bm_mc_reset_strategy = true;
  }

  /// Get set-flag for bm_mc_reset_strategy
  bool set_bm_mc_reset_strategy() const { return _opt_bm_mc_reset_strategy; }

  /// Get bm_num_gs_cycles
  int get_bm_num_gs_cycles() const { return _bm_num_gs_cycles; }

  /// Set bm_num_gs_cycles
  void set_bm_num_gs_cycles(int x) {
    _bm_num_gs_cycles = x;
    _opt_bm_num_gs_cycles = true;
  }

  /// Get set-flag for bm_num_gs_cycles
  bool set_bm_num_gs_cycles() const { return _opt_bm_num_gs_cycles; }

  /// Get bm_num_gs_steps
  int get_bm_num_gs_steps() const { return _bm_num_gs_steps; }

  /// Set bm_num_gs_steps
  void set_bm_num_gs_steps(int x) {
    _bm_num_gs_steps = x;
    _opt_bm_num_gs_steps = true;
  }

  /// Get set-flag for bm_num_gs_steps
  bool set_bm_num_gs_steps() const { return _opt_bm_num_gs_steps; }

  /// Get bm_sampling
  int get_bm_sampling() const { return _bm_sampling; }

  /// Set bm_sampling
  void set_bm_sampling(int x) {
    _bm_sampling = x;
    _opt_bm_sampling = true;
  }

  /// Get set-flag for bm_sampling
  bool set_bm_sampling() const { return _opt_bm_sampling; }

  /// Get budget
  int get_budget() const { return _budget; }

  /// Set budget
  void set_budget(int x) {
    _budget = x;
    _opt_budget = true;
  }

  /// Get set-flag for budget
  bool set_budget() const { return _opt_budget; }

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

  /// Get ea_crossover_bias
  double get_ea_crossover_bias() const { return _ea_crossover_bias; }

  /// Set ea_crossover_bias
  void set_ea_crossover_bias(double x) {
    _ea_crossover_bias = x;
    _opt_ea_crossover_bias = true;
  }

  /// Get set-flag for ea_crossover_bias
  bool set_ea_crossover_bias() const { return _opt_ea_crossover_bias; }

  /// Get ea_crossover_probability
  double get_ea_crossover_probability() const { return _ea_crossover_probability; }

  /// Set ea_crossover_probability
  void set_ea_crossover_probability(double x) {
    _ea_crossover_probability = x;
    _opt_ea_crossover_probability = true;
  }

  /// Get set-flag for ea_crossover_probability
  bool set_ea_crossover_probability() const { return _opt_ea_crossover_probability; }

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

  /// Get ea_mutation_rate
  double get_ea_mutation_rate() const { return _ea_mutation_rate; }

  /// Set ea_mutation_rate
  void set_ea_mutation_rate(double x) {
    _ea_mutation_rate = x;
    _opt_ea_mutation_rate = true;
  }

  /// Get set-flag for ea_mutation_rate
  bool set_ea_mutation_rate() const { return _opt_ea_mutation_rate; }

  /// Get ea_mutation_rate_max
  double get_ea_mutation_rate_max() const { return _ea_mutation_rate_max; }

  /// Set ea_mutation_rate_max
  void set_ea_mutation_rate_max(double x) {
    _ea_mutation_rate_max = x;
    _opt_ea_mutation_rate_max = true;
  }

  /// Get set-flag for ea_mutation_rate_max
  bool set_ea_mutation_rate_max() const { return _opt_ea_mutation_rate_max; }

  /// Get ea_mutation_rate_min
  double get_ea_mutation_rate_min() const { return _ea_mutation_rate_min; }

  /// Set ea_mutation_rate_min
  void set_ea_mutation_rate_min(double x) {
    _ea_mutation_rate_min = x;
    _opt_ea_mutation_rate_min = true;
  }

  /// Get set-flag for ea_mutation_rate_min
  bool set_ea_mutation_rate_min() const { return _opt_ea_mutation_rate_min; }

  /// Get ea_success_ratio
  double get_ea_success_ratio() const { return _ea_success_ratio; }

  /// Set ea_success_ratio
  void set_ea_success_ratio(double x) {
    _ea_success_ratio = x;
    _opt_ea_success_ratio = true;
  }

  /// Get set-flag for ea_success_ratio
  bool set_ea_success_ratio() const { return _opt_ea_success_ratio; }

  /// Get ea_tournament_size
  int get_ea_tournament_size() const { return _ea_tournament_size; }

  /// Set ea_tournament_size
  void set_ea_tournament_size(int x) {
    _ea_tournament_size = x;
    _opt_ea_tournament_size = true;
  }

  /// Get set-flag for ea_tournament_size
  bool set_ea_tournament_size() const { return _opt_ea_tournament_size; }

  /// Get ea_update_strength
  double get_ea_update_strength() const { return _ea_update_strength; }

  /// Set ea_update_strength
  void set_ea_update_strength(double x) {
    _ea_update_strength = x;
    _opt_ea_update_strength = true;
  }

  /// Get set-flag for ea_update_strength
  bool set_ea_update_strength() const { return _opt_ea_update_strength; }

  /// Get expression
  std::string get_expression() const { return _expression; }

  /// Set expression
  void set_expression(std::string x) {
    _expression = x;
    _opt_expression = true;
  }

  /// Get set-flag for expression
  bool set_expression() const { return _opt_expression; }

  /// Get fn_name
  std::string get_fn_name() const { return _fn_name; }

  /// Set fn_name
  void set_fn_name(std::string x) {
    _fn_name = x;
    _opt_fn_name = true;
  }

  /// Get set-flag for fn_name
  bool set_fn_name() const { return _opt_fn_name; }

  /// Get fn_num_traps
  int get_fn_num_traps() const { return _fn_num_traps; }

  /// Set fn_num_traps
  void set_fn_num_traps(int x) {
    _fn_num_traps = x;
    _opt_fn_num_traps = true;
  }

  /// Get set-flag for fn_num_traps
  bool set_fn_num_traps() const { return _opt_fn_num_traps; }

  /// Get fn_prefix_length
  int get_fn_prefix_length() const { return _fn_prefix_length; }

  /// Set fn_prefix_length
  void set_fn_prefix_length(int x) {
    _fn_prefix_length = x;
    _opt_fn_prefix_length = true;
  }

  /// Get set-flag for fn_prefix_length
  bool set_fn_prefix_length() const { return _opt_fn_prefix_length; }

  /// Get fn_threshold
  int get_fn_threshold() const { return _fn_threshold; }

  /// Set fn_threshold
  void set_fn_threshold(int x) {
    _fn_threshold = x;
    _opt_fn_threshold = true;
  }

  /// Get set-flag for fn_threshold
  bool set_fn_threshold() const { return _opt_fn_threshold; }

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

  /// Get hea_reset_period
  int get_hea_reset_period() const { return _hea_reset_period; }

  /// Set hea_reset_period
  void set_hea_reset_period(int x) {
    _hea_reset_period = x;
    _opt_hea_reset_period = true;
  }

  /// Get set-flag for hea_reset_period
  bool set_hea_reset_period() const { return _opt_hea_reset_period; }

  /// Get learning_rate
  double get_learning_rate() const { return _learning_rate; }

  /// Set learning_rate
  void set_learning_rate(double x) {
    _learning_rate = x;
    _opt_learning_rate = true;
  }

  /// Get set-flag for learning_rate
  bool set_learning_rate() const { return _opt_learning_rate; }

  /// Get map
  int get_map() const { return _map; }

  /// Set map
  void set_map(int x) {
    _map = x;
    _opt_map = true;
  }

  /// Get set-flag for map
  bool set_map() const { return _opt_map; }

  /// Get map_input_size
  int get_map_input_size() const { return _map_input_size; }

  /// Set map_input_size
  void set_map_input_size(int x) {
    _map_input_size = x;
    _opt_map_input_size = true;
  }

  /// Get set-flag for map_input_size
  bool set_map_input_size() const { return _opt_map_input_size; }

  /// Get map_path
  std::string get_map_path() const { return _map_path; }

  /// Set map_path
  void set_map_path(std::string x) {
    _map_path = x;
    _opt_map_path = true;
  }

  /// Get set-flag for map_path
  bool set_map_path() const { return _opt_map_path; }

  /// Get map_ts_length
  int get_map_ts_length() const { return _map_ts_length; }

  /// Set map_ts_length
  void set_map_ts_length(int x) {
    _map_ts_length = x;
    _opt_map_ts_length = true;
  }

  /// Get set-flag for map_ts_length
  bool set_map_ts_length() const { return _opt_map_ts_length; }

  /// Get map_ts_sampling_mode
  int get_map_ts_sampling_mode() const { return _map_ts_sampling_mode; }

  /// Set map_ts_sampling_mode
  void set_map_ts_sampling_mode(int x) {
    _map_ts_sampling_mode = x;
    _opt_map_ts_sampling_mode = true;
  }

  /// Get set-flag for map_ts_sampling_mode
  bool set_map_ts_sampling_mode() const { return _opt_map_ts_sampling_mode; }

  /// Get neighborhood
  int get_neighborhood() const { return _neighborhood; }

  /// Set neighborhood
  void set_neighborhood(int x) {
    _neighborhood = x;
    _opt_neighborhood = true;
  }

  /// Get set-flag for neighborhood
  bool set_neighborhood() const { return _opt_neighborhood; }

  /// Get neighborhood_iterator
  int get_neighborhood_iterator() const { return _neighborhood_iterator; }

  /// Set neighborhood_iterator
  void set_neighborhood_iterator(int x) {
    _neighborhood_iterator = x;
    _opt_neighborhood_iterator = true;
  }

  /// Get set-flag for neighborhood_iterator
  bool set_neighborhood_iterator() const { return _opt_neighborhood_iterator; }

  /// Get noise_stddev
  double get_noise_stddev() const { return _noise_stddev; }

  /// Set noise_stddev
  void set_noise_stddev(double x) {
    _noise_stddev = x;
    _opt_noise_stddev = true;
  }

  /// Get set-flag for noise_stddev
  bool set_noise_stddev() const { return _opt_noise_stddev; }

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

  /// Get pn_mutation_rate
  double get_pn_mutation_rate() const { return _pn_mutation_rate; }

  /// Set pn_mutation_rate
  void set_pn_mutation_rate(double x) {
    _pn_mutation_rate = x;
    _opt_pn_mutation_rate = true;
  }

  /// Get set-flag for pn_mutation_rate
  bool set_pn_mutation_rate() const { return _opt_pn_mutation_rate; }

  /// Get pn_neighborhood
  int get_pn_neighborhood() const { return _pn_neighborhood; }

  /// Set pn_neighborhood
  void set_pn_neighborhood(int x) {
    _pn_neighborhood = x;
    _opt_pn_neighborhood = true;
  }

  /// Get set-flag for pn_neighborhood
  bool set_pn_neighborhood() const { return _opt_pn_neighborhood; }

  /// Get pn_radius
  int get_pn_radius() const { return _pn_radius; }

  /// Set pn_radius
  void set_pn_radius(int x) {
    _pn_radius = x;
    _opt_pn_radius = true;
  }

  /// Get set-flag for pn_radius
  bool set_pn_radius() const { return _opt_pn_radius; }

  /// Get population_size
  int get_population_size() const { return _population_size; }

  /// Set population_size
  void set_population_size(int x) {
    _population_size = x;
    _opt_population_size = true;
  }

  /// Get set-flag for population_size
  bool set_population_size() const { return _opt_population_size; }

  /// Get pv_log_num_components
  int get_pv_log_num_components() const { return _pv_log_num_components; }

  /// Set pv_log_num_components
  void set_pv_log_num_components(int x) {
    _pv_log_num_components = x;
    _opt_pv_log_num_components = true;
  }

  /// Get set-flag for pv_log_num_components
  bool set_pv_log_num_components() const { return _opt_pv_log_num_components; }

  /// Get radius
  int get_radius() const { return _radius; }

  /// Set radius
  void set_radius(int x) {
    _radius = x;
    _opt_radius = true;
  }

  /// Get set-flag for radius
  bool set_radius() const { return _opt_radius; }

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

  /// Get rls_patience
  int get_rls_patience() const { return _rls_patience; }

  /// Set rls_patience
  void set_rls_patience(int x) {
    _rls_patience = x;
    _opt_rls_patience = true;
  }

  /// Get set-flag for rls_patience
  bool set_rls_patience() const { return _opt_rls_patience; }

  /// Get sa_beta_ratio
  double get_sa_beta_ratio() const { return _sa_beta_ratio; }

  /// Set sa_beta_ratio
  void set_sa_beta_ratio(double x) {
    _sa_beta_ratio = x;
    _opt_sa_beta_ratio = true;
  }

  /// Get set-flag for sa_beta_ratio
  bool set_sa_beta_ratio() const { return _opt_sa_beta_ratio; }

  /// Get sa_initial_acceptance_probability
  double get_sa_initial_acceptance_probability() const { return _sa_initial_acceptance_probability; }

  /// Set sa_initial_acceptance_probability
  void set_sa_initial_acceptance_probability(double x) {
    _sa_initial_acceptance_probability = x;
    _opt_sa_initial_acceptance_probability = true;
  }

  /// Get set-flag for sa_initial_acceptance_probability
  bool set_sa_initial_acceptance_probability() const { return _opt_sa_initial_acceptance_probability; }

  /// Get sa_num_transitions
  int get_sa_num_transitions() const { return _sa_num_transitions; }

  /// Set sa_num_transitions
  void set_sa_num_transitions(int x) {
    _sa_num_transitions = x;
    _opt_sa_num_transitions = true;
  }

  /// Get set-flag for sa_num_transitions
  bool set_sa_num_transitions() const { return _opt_sa_num_transitions; }

  /// Get sa_num_trials
  int get_sa_num_trials() const { return _sa_num_trials; }

  /// Set sa_num_trials
  void set_sa_num_trials(int x) {
    _sa_num_trials = x;
    _opt_sa_num_trials = true;
  }

  /// Get set-flag for sa_num_trials
  bool set_sa_num_trials() const { return _opt_sa_num_trials; }

  /// Get seed
  unsigned get_seed() const { return _seed; }

  /// Set seed
  void set_seed(unsigned x) {
    _seed = x;
    _opt_seed = true;
  }

  /// Get set-flag for seed
  bool set_seed() const { return _opt_seed; }

  /// Get selection_size
  int get_selection_size() const { return _selection_size; }

  /// Set selection_size
  void set_selection_size(int x) {
    _selection_size = x;
    _opt_selection_size = true;
  }

  /// Get set-flag for selection_size
  bool set_selection_size() const { return _opt_selection_size; }

  /// Get solution_path
  std::string get_solution_path() const { return _solution_path; }

  /// Set solution_path
  void set_solution_path(std::string x) {
    _solution_path = x;
    _opt_solution_path = true;
  }

  /// Get set-flag for solution_path
  bool set_solution_path() const { return _opt_solution_path; }

  /// Get target
  double get_target() const { return _target; }

  /// Set target
  void set_target(double x) {
    _target = x;
    _opt_target = true;
  }

  /// Get set-flag for target
  bool set_target() const { return _opt_target; }

  /// Get additive_gaussian_noise
  bool with_additive_gaussian_noise() const { return _additive_gaussian_noise; }

  /// Set additive_gaussian_noise
  void set_additive_gaussian_noise() { _additive_gaussian_noise = true; }

  /// Get bm_log_norm_1
  bool with_bm_log_norm_1() const { return _bm_log_norm_1; }

  /// Set bm_log_norm_1
  void set_bm_log_norm_1() { _bm_log_norm_1 = true; }

  /// Get bm_log_norm_infinite
  bool with_bm_log_norm_infinite() const { return _bm_log_norm_infinite; }

  /// Set bm_log_norm_infinite
  void set_bm_log_norm_infinite() { _bm_log_norm_infinite = true; }

  /// Get bm_negative_positive_selection
  bool with_bm_negative_positive_selection() const { return _bm_negative_positive_selection; }

  /// Set bm_negative_positive_selection
  void set_bm_negative_positive_selection() { _bm_negative_positive_selection = true; }

  /// Get cache
  bool with_cache() const { return _cache; }

  /// Set cache
  void set_cache() { _cache = true; }

  /// Get cache_budget
  bool with_cache_budget() const { return _cache_budget; }

  /// Set cache_budget
  void set_cache_budget() { _cache_budget = true; }

  /// Get concrete_solution
  bool with_concrete_solution() const { return _concrete_solution; }

  /// Set concrete_solution
  void set_concrete_solution() { _concrete_solution = true; }

  /// Get ea_allow_no_mutation
  bool with_ea_allow_no_mutation() const { return _ea_allow_no_mutation; }

  /// Set ea_allow_no_mutation
  void set_ea_allow_no_mutation() { _ea_allow_no_mutation = true; }

  /// Get ea_log_mutation_rate
  bool with_ea_log_mutation_rate() const { return _ea_log_mutation_rate; }

  /// Set ea_log_mutation_rate
  void set_ea_log_mutation_rate() { _ea_log_mutation_rate = true; }

  /// Get fn_display
  bool with_fn_display() const { return _fn_display; }

  /// Set fn_display
  void set_fn_display() { _fn_display = true; }

  /// Get fn_get_bv_size
  bool with_fn_get_bv_size() const { return _fn_get_bv_size; }

  /// Set fn_get_bv_size
  void set_fn_get_bv_size() { _fn_get_bv_size = true; }

  /// Get fn_get_maximum
  bool with_fn_get_maximum() const { return _fn_get_maximum; }

  /// Set fn_get_maximum
  void set_fn_get_maximum() { _fn_get_maximum = true; }

  /// Get fn_has_known_maximum
  bool with_fn_has_known_maximum() const { return _fn_has_known_maximum; }

  /// Set fn_has_known_maximum
  void set_fn_has_known_maximum() { _fn_has_known_maximum = true; }

  /// Get fn_provides_incremental_evaluation
  bool with_fn_provides_incremental_evaluation() const { return _fn_provides_incremental_evaluation; }

  /// Set fn_provides_incremental_evaluation
  void set_fn_provides_incremental_evaluation() { _fn_provides_incremental_evaluation = true; }

  /// Get fn_walsh_transform
  bool with_fn_walsh_transform() const { return _fn_walsh_transform; }

  /// Set fn_walsh_transform
  void set_fn_walsh_transform() { _fn_walsh_transform = true; }

  /// Get hea_bound_moment
  bool with_hea_bound_moment() const { return _hea_bound_moment; }

  /// Set hea_bound_moment
  void set_hea_bound_moment() { _hea_bound_moment = true; }

  /// Get hea_log_delta_norm
  bool with_hea_log_delta_norm() const { return _hea_log_delta_norm; }

  /// Set hea_log_delta_norm
  void set_hea_log_delta_norm() { _hea_log_delta_norm = true; }

  /// Get hea_log_herding_error
  bool with_hea_log_herding_error() const { return _hea_log_herding_error; }

  /// Set hea_log_herding_error
  void set_hea_log_herding_error() { _hea_log_herding_error = true; }

  /// Get hea_log_target
  bool with_hea_log_target() const { return _hea_log_target; }

  /// Set hea_log_target
  void set_hea_log_target() { _hea_log_target = true; }

  /// Get hea_log_target_norm
  bool with_hea_log_target_norm() const { return _hea_log_target_norm; }

  /// Set hea_log_target_norm
  void set_hea_log_target_norm() { _hea_log_target_norm = true; }

  /// Get hea_randomize_bit_order
  bool with_hea_randomize_bit_order() const { return _hea_randomize_bit_order; }

  /// Set hea_randomize_bit_order
  void set_hea_randomize_bit_order() { _hea_randomize_bit_order = true; }

  /// Get incremental_evaluation
  bool with_incremental_evaluation() const { return _incremental_evaluation; }

  /// Set incremental_evaluation
  void set_incremental_evaluation() { _incremental_evaluation = true; }

  /// Get load_solution
  bool with_load_solution() const { return _load_solution; }

  /// Set load_solution
  void set_load_solution() { _load_solution = true; }

  /// Get log_improvement
  bool with_log_improvement() const { return _log_improvement; }

  /// Set log_improvement
  void set_log_improvement() { _log_improvement = true; }

  /// Get map_display
  bool with_map_display() const { return _map_display; }

  /// Set map_display
  void set_map_display() { _map_display = true; }

  /// Get map_random
  bool with_map_random() const { return _map_random; }

  /// Set map_random
  void set_map_random() { _map_random = true; }

  /// Get map_surjective
  bool with_map_surjective() const { return _map_surjective; }

  /// Set map_surjective
  void set_map_surjective() { _map_surjective = true; }

  /// Get mmas_strict
  bool with_mmas_strict() const { return _mmas_strict; }

  /// Set mmas_strict
  void set_mmas_strict() { _mmas_strict = true; }

  /// Get negation
  bool with_negation() const { return _negation; }

  /// Set negation
  void set_negation() { _negation = true; }

  /// Get parsed_modifier
  bool with_parsed_modifier() const { return _parsed_modifier; }

  /// Set parsed_modifier
  void set_parsed_modifier() { _parsed_modifier = true; }

  /// Get pn_allow_no_mutation
  bool with_pn_allow_no_mutation() const { return _pn_allow_no_mutation; }

  /// Set pn_allow_no_mutation
  void set_pn_allow_no_mutation() { _pn_allow_no_mutation = true; }

  /// Get print_default_parameters
  bool with_print_default_parameters() const { return _print_default_parameters; }

  /// Set print_default_parameters
  void set_print_default_parameters() { _print_default_parameters = true; }

  /// Get print_description
  bool with_print_description() const { return _print_description; }

  /// Set print_description
  void set_print_description() { _print_description = true; }

  /// Get print_parameters
  bool with_print_parameters() const { return _print_parameters; }

  /// Set print_parameters
  void set_print_parameters() { _print_parameters = true; }

  /// Get print_results
  bool with_print_results() const { return _print_results; }

  /// Set print_results
  void set_print_results() { _print_results = true; }

  /// Get print_solution
  bool with_print_solution() const { return _print_solution; }

  /// Set print_solution
  void set_print_solution() { _print_solution = true; }

  /// Get prior_noise
  bool with_prior_noise() const { return _prior_noise; }

  /// Set prior_noise
  void set_prior_noise() { _prior_noise = true; }

  /// Get pv_log_entropy
  bool with_pv_log_entropy() const { return _pv_log_entropy; }

  /// Set pv_log_entropy
  void set_pv_log_entropy() { _pv_log_entropy = true; }

  /// Get pv_log_pv
  bool with_pv_log_pv() const { return _pv_log_pv; }

  /// Set pv_log_pv
  void set_pv_log_pv() { _pv_log_pv = true; }

  /// Get record_evaluation_time
  bool with_record_evaluation_time() const { return _record_evaluation_time; }

  /// Set record_evaluation_time
  void set_record_evaluation_time() { _record_evaluation_time = true; }

  /// Get restart
  bool with_restart() const { return _restart; }

  /// Set restart
  void set_restart() { _restart = true; }

  /// Get rls_strict
  bool with_rls_strict() const { return _rls_strict; }

  /// Set rls_strict
  void set_rls_strict() { _rls_strict = true; }

  /// Get rw_log_value
  bool with_rw_log_value() const { return _rw_log_value; }

  /// Set rw_log_value
  void set_rw_log_value() { _rw_log_value = true; }

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

  /// Get stop_on_maximum
  bool with_stop_on_maximum() const { return _stop_on_maximum; }

  /// Set stop_on_maximum
  void set_stop_on_maximum() { _stop_on_maximum = true; }

  /// Get stop_on_target
  bool with_stop_on_target() const { return _stop_on_target; }

  /// Set stop_on_target
  void set_stop_on_target() { _stop_on_target = true; }

  friend std::ostream& operator<<(std::ostream&, const HncoOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const HncoOptions& options);

}
}

#endif
