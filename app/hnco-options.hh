#ifndef HNCO_OPTIONS_H
#define HNCO_OPTIONS_H

#include <iostream>
#include <string>

/// Command line options
class Options {

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

  /// Cache budget (<= 0 means indefinite)
  int _cache_budget;
  bool _opt_cache_budget;

  /// Offspring population size
  int _ea_lambda;
  bool _opt_ea_lambda;

  /// Parent population size
  int _ea_mu;
  bool _opt_ea_mu;

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

  /// Herding dynamics for binary variables
  int _hea_binary_dynamics;
  bool _opt_hea_binary_dynamics;

  /// Delay before learning rate decay
  int _hea_delay;
  bool _opt_hea_delay;

  /// Number of parallel updates per sample
  int _hea_num_par_updates;
  bool _opt_hea_num_par_updates;

  /// Number of sequential updates per sample
  int _hea_num_seq_updates;
  bool _opt_hea_num_seq_updates;

  /// Rate strategy
  int _hea_rate_strategy;
  bool _opt_hea_rate_strategy;

  /// Reset period (<= 0 means no reset)
  int _hea_reset_period;
  bool _opt_hea_reset_period;

  /// Sampling method for spin variables only
  int _hea_sampling_method;
  bool _opt_hea_sampling_method;

  /// Time constant for exponential decay
  double _hea_time_constant;
  bool _opt_hea_time_constant;

  /// Weight of second moments
  double _hea_weight;
  bool _opt_hea_weight;

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

  /// Expected number of flipped bits (bv_size times mutation probability)
  double _mutation_probability;
  bool _opt_mutation_probability;

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

  /// Expected number of flipped bits (bv_size times mutation probability)
  double _pn_mutation_probability;
  bool _opt_pn_mutation_probability;

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

  /// Number of consecutive rejected moves before throwing LocalMaximum (<= 0 means infinite)
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

  /// Target
  double _target;
  bool _opt_target;

  /// Additive Gaussian noise
  bool _additive_gaussian_noise;

  /// In case no mutation occurs allow the current bit vector to stay unchanged (Bernoulli process)
  bool _allow_stay;

  /// Log infinite norm of the parameters
  bool _bm_log_norm_infinite;

  /// Log L1 norm of the parameters
  bool _bm_log_norm_l1;

  /// Negative and positive selection
  bool _bm_negative_positive_selection;

  /// Cache function evaluations
  bool _cache;

  /// At the end, describe the solution
  bool _describe_solution;

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

  /// Log norm of delta, the moment increment
  bool _hea_log_delta;

  /// Log distance to uniform
  bool _hea_log_dtu;

  /// Log error (moment discrepancy)
  bool _hea_log_error;

  /// Log the distance between the target and the selection moment
  bool _hea_log_selection;

  /// Randomize bit order
  bool _hea_randomize_bit_order;

  /// Incremental evaluation
  bool _incremental_evaluation;

  /// Log improvement
  bool _log_improvement;

  /// Sample a random map
  bool _map_random;

  /// Ensure that the sampled linear or affine map is surjective
  bool _map_surjective;

  /// Strict (>) max-min ant system
  bool _mmas_strict;

  /// Negation (hence minimization) of the function
  bool _negation;

  /// In case no mutation occurs allow the current bit vector to stay unchanged (Bernoulli process)
  bool _pn_allow_stay;

  /// Print the default parameters and exit
  bool _print_defaults;

  /// At the beginning, print the header
  bool _print_header;

  /// At the end, print performance (maximum and number of evaluations needed to reach it)
  bool _print_performance;

  /// At the end, print the solution
  bool _print_solution;

  /// Prior noise
  bool _prior_noise;

  /// Log entropy of probability vector
  bool _pv_log_entropy;

  /// Log probability vector
  bool _pv_log_pv;

  /// Restart any algorithm an indefinite number of times
  bool _restart;

  /// Strict (>) random local search
  bool _rls_strict;

  /// Log bit vector value during random walk
  bool _rw_log_value;

  /// Stop on maximum
  bool _stop_on_maximum;

  /// Stop on target
  bool _stop_on_target;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print help message for section pn
  void print_help_pn(std::ostream& stream) const;

  /// Print help message for section map
  void print_help_map(std::ostream& stream) const;

  /// Print help message for section hea
  void print_help_hea(std::ostream& stream) const;

  /// Print help message for section bm
  void print_help_bm(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  Options(int argc, char *argv[]);

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

  /// Get cache_budget
  int get_cache_budget() const { return _cache_budget; }

  /// Set cache_budget
  void set_cache_budget(int x) {
    _cache_budget = x;
    _opt_cache_budget = true;
  }

  /// Get set-flag for cache_budget
  bool set_cache_budget() const { return _opt_cache_budget; }

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

  /// Get hea_binary_dynamics
  int get_hea_binary_dynamics() const { return _hea_binary_dynamics; }

  /// Set hea_binary_dynamics
  void set_hea_binary_dynamics(int x) {
    _hea_binary_dynamics = x;
    _opt_hea_binary_dynamics = true;
  }

  /// Get set-flag for hea_binary_dynamics
  bool set_hea_binary_dynamics() const { return _opt_hea_binary_dynamics; }

  /// Get hea_delay
  int get_hea_delay() const { return _hea_delay; }

  /// Set hea_delay
  void set_hea_delay(int x) {
    _hea_delay = x;
    _opt_hea_delay = true;
  }

  /// Get set-flag for hea_delay
  bool set_hea_delay() const { return _opt_hea_delay; }

  /// Get hea_num_par_updates
  int get_hea_num_par_updates() const { return _hea_num_par_updates; }

  /// Set hea_num_par_updates
  void set_hea_num_par_updates(int x) {
    _hea_num_par_updates = x;
    _opt_hea_num_par_updates = true;
  }

  /// Get set-flag for hea_num_par_updates
  bool set_hea_num_par_updates() const { return _opt_hea_num_par_updates; }

  /// Get hea_num_seq_updates
  int get_hea_num_seq_updates() const { return _hea_num_seq_updates; }

  /// Set hea_num_seq_updates
  void set_hea_num_seq_updates(int x) {
    _hea_num_seq_updates = x;
    _opt_hea_num_seq_updates = true;
  }

  /// Get set-flag for hea_num_seq_updates
  bool set_hea_num_seq_updates() const { return _opt_hea_num_seq_updates; }

  /// Get hea_rate_strategy
  int get_hea_rate_strategy() const { return _hea_rate_strategy; }

  /// Set hea_rate_strategy
  void set_hea_rate_strategy(int x) {
    _hea_rate_strategy = x;
    _opt_hea_rate_strategy = true;
  }

  /// Get set-flag for hea_rate_strategy
  bool set_hea_rate_strategy() const { return _opt_hea_rate_strategy; }

  /// Get hea_reset_period
  int get_hea_reset_period() const { return _hea_reset_period; }

  /// Set hea_reset_period
  void set_hea_reset_period(int x) {
    _hea_reset_period = x;
    _opt_hea_reset_period = true;
  }

  /// Get set-flag for hea_reset_period
  bool set_hea_reset_period() const { return _opt_hea_reset_period; }

  /// Get hea_sampling_method
  int get_hea_sampling_method() const { return _hea_sampling_method; }

  /// Set hea_sampling_method
  void set_hea_sampling_method(int x) {
    _hea_sampling_method = x;
    _opt_hea_sampling_method = true;
  }

  /// Get set-flag for hea_sampling_method
  bool set_hea_sampling_method() const { return _opt_hea_sampling_method; }

  /// Get hea_time_constant
  double get_hea_time_constant() const { return _hea_time_constant; }

  /// Set hea_time_constant
  void set_hea_time_constant(double x) {
    _hea_time_constant = x;
    _opt_hea_time_constant = true;
  }

  /// Get set-flag for hea_time_constant
  bool set_hea_time_constant() const { return _opt_hea_time_constant; }

  /// Get hea_weight
  double get_hea_weight() const { return _hea_weight; }

  /// Set hea_weight
  void set_hea_weight(double x) {
    _hea_weight = x;
    _opt_hea_weight = true;
  }

  /// Get set-flag for hea_weight
  bool set_hea_weight() const { return _opt_hea_weight; }

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

  /// Get mutation_probability
  double get_mutation_probability() const { return _mutation_probability; }

  /// Set mutation_probability
  void set_mutation_probability(double x) {
    _mutation_probability = x;
    _opt_mutation_probability = true;
  }

  /// Get set-flag for mutation_probability
  bool set_mutation_probability() const { return _opt_mutation_probability; }

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

  /// Get pn_mutation_probability
  double get_pn_mutation_probability() const { return _pn_mutation_probability; }

  /// Set pn_mutation_probability
  void set_pn_mutation_probability(double x) {
    _pn_mutation_probability = x;
    _opt_pn_mutation_probability = true;
  }

  /// Get set-flag for pn_mutation_probability
  bool set_pn_mutation_probability() const { return _opt_pn_mutation_probability; }

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
 
  /// Get allow_stay
  bool with_allow_stay() const { return _allow_stay; }

  /// Set allow_stay
  void set_allow_stay() { _allow_stay = true; }
 
  /// Get bm_log_norm_infinite
  bool with_bm_log_norm_infinite() const { return _bm_log_norm_infinite; }

  /// Set bm_log_norm_infinite
  void set_bm_log_norm_infinite() { _bm_log_norm_infinite = true; }
 
  /// Get bm_log_norm_l1
  bool with_bm_log_norm_l1() const { return _bm_log_norm_l1; }

  /// Set bm_log_norm_l1
  void set_bm_log_norm_l1() { _bm_log_norm_l1 = true; }
 
  /// Get bm_negative_positive_selection
  bool with_bm_negative_positive_selection() const { return _bm_negative_positive_selection; }

  /// Set bm_negative_positive_selection
  void set_bm_negative_positive_selection() { _bm_negative_positive_selection = true; }
 
  /// Get cache
  bool with_cache() const { return _cache; }

  /// Set cache
  void set_cache() { _cache = true; }
 
  /// Get describe_solution
  bool with_describe_solution() const { return _describe_solution; }

  /// Set describe_solution
  void set_describe_solution() { _describe_solution = true; }
 
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
 
  /// Get hea_log_delta
  bool with_hea_log_delta() const { return _hea_log_delta; }

  /// Set hea_log_delta
  void set_hea_log_delta() { _hea_log_delta = true; }
 
  /// Get hea_log_dtu
  bool with_hea_log_dtu() const { return _hea_log_dtu; }

  /// Set hea_log_dtu
  void set_hea_log_dtu() { _hea_log_dtu = true; }
 
  /// Get hea_log_error
  bool with_hea_log_error() const { return _hea_log_error; }

  /// Set hea_log_error
  void set_hea_log_error() { _hea_log_error = true; }
 
  /// Get hea_log_selection
  bool with_hea_log_selection() const { return _hea_log_selection; }

  /// Set hea_log_selection
  void set_hea_log_selection() { _hea_log_selection = true; }
 
  /// Get hea_randomize_bit_order
  bool with_hea_randomize_bit_order() const { return _hea_randomize_bit_order; }

  /// Set hea_randomize_bit_order
  void set_hea_randomize_bit_order() { _hea_randomize_bit_order = true; }
 
  /// Get incremental_evaluation
  bool with_incremental_evaluation() const { return _incremental_evaluation; }

  /// Set incremental_evaluation
  void set_incremental_evaluation() { _incremental_evaluation = true; }
 
  /// Get log_improvement
  bool with_log_improvement() const { return _log_improvement; }

  /// Set log_improvement
  void set_log_improvement() { _log_improvement = true; }
 
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
 
  /// Get pn_allow_stay
  bool with_pn_allow_stay() const { return _pn_allow_stay; }

  /// Set pn_allow_stay
  void set_pn_allow_stay() { _pn_allow_stay = true; }
 
  /// Get print_defaults
  bool with_print_defaults() const { return _print_defaults; }

  /// Set print_defaults
  void set_print_defaults() { _print_defaults = true; }
 
  /// Get print_header
  bool with_print_header() const { return _print_header; }

  /// Set print_header
  void set_print_header() { _print_header = true; }
 
  /// Get print_performance
  bool with_print_performance() const { return _print_performance; }

  /// Set print_performance
  void set_print_performance() { _print_performance = true; }
 
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
 
  /// Get stop_on_maximum
  bool with_stop_on_maximum() const { return _stop_on_maximum; }

  /// Set stop_on_maximum
  void set_stop_on_maximum() { _stop_on_maximum = true; }
 
  /// Get stop_on_target
  bool with_stop_on_target() const { return _stop_on_target; }

  /// Set stop_on_target
  void set_stop_on_target() { _stop_on_target = true; }
 
  /// Print a header containing the parameter values
  friend std::ostream& operator<<(std::ostream&, const Options&);
};


#endif
