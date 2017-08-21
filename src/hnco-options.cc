#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "hnco-options.hh"

using namespace std;

Options::Options(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.6"),
  _algorithm(100),
  _opt_algorithm(false),
  _bm_mc_reset_strategy(1),
  _opt_bm_mc_reset_strategy(false),
  _bm_num_gs_cycles(1),
  _opt_bm_num_gs_cycles(false),
  _bm_num_gs_steps(100),
  _opt_bm_num_gs_steps(false),
  _bm_sampling(1),
  _opt_bm_sampling(false),
  _budget(10000),
  _opt_budget(false),
  _bv_size(100),
  _opt_bv_size(false),
  _ea_lambda(100),
  _opt_ea_lambda(false),
  _ea_mu(10),
  _opt_ea_mu(false),
  _fun_name("noname"),
  _opt_fun_name(false),
  _fun_num_traps(10),
  _opt_fun_num_traps(false),
  _fun_prefix_length(2),
  _opt_fun_prefix_length(false),
  _fun_threshold(10),
  _opt_fun_threshold(false),
  _function(0),
  _opt_function(false),
  _ga_crossover_probability(0.5),
  _opt_ga_crossover_probability(false),
  _ga_tournament_size(10),
  _opt_ga_tournament_size(false),
  _hea_binary_dynamics(0),
  _opt_hea_binary_dynamics(false),
  _hea_delay(10000),
  _opt_hea_delay(false),
  _hea_num_par_updates(1),
  _opt_hea_num_par_updates(false),
  _hea_num_seq_updates(100),
  _opt_hea_num_seq_updates(false),
  _hea_rate_strategy(0),
  _opt_hea_rate_strategy(false),
  _hea_reset_period(0),
  _opt_hea_reset_period(false),
  _hea_sampling_method(0),
  _opt_hea_sampling_method(false),
  _hea_time_constant(1000),
  _opt_hea_time_constant(false),
  _hea_weight(1),
  _opt_hea_weight(false),
  _learning_rate(0.001),
  _opt_learning_rate(false),
  _map(0),
  _opt_map(false),
  _map_input_size(100),
  _opt_map_input_size(false),
  _map_path("nopath"),
  _opt_map_path(false),
  _neighborhood(0),
  _opt_neighborhood(false),
  _neighborhood_iterator(0),
  _opt_neighborhood_iterator(false),
  _noise_stddev(1),
  _opt_noise_stddev(false),
  _num_iterations(0),
  _opt_num_iterations(false),
  _num_threads(1),
  _opt_num_threads(false),
  _path("nopath"),
  _opt_path(false),
  _population_size(10),
  _opt_population_size(false),
  _pv_log_num_components(5),
  _opt_pv_log_num_components(false),
  _radius(2),
  _opt_radius(false),
  _rls_patience(50),
  _opt_rls_patience(false),
  _sa_initial_acceptance_probability(0.6),
  _opt_sa_initial_acceptance_probability(false),
  _sa_num_transitions(50),
  _opt_sa_num_transitions(false),
  _sa_num_trials(100),
  _opt_sa_num_trials(false),
  _sa_rate(1.2),
  _opt_sa_rate(false),
  _scaled_mutation_probability(1),
  _opt_scaled_mutation_probability(false),
  _seed(0),
  _opt_seed(false),
  _selection_size(1),
  _opt_selection_size(false),
  _target(100),
  _opt_target(false),
  _additive_gaussian_noise(false),
  _bm_log_norm_infinite(false),
  _bm_log_norm_l1(false),
  _bm_negative_positive_selection(false),
  _cache(false),
  _describe_problem(false),
  _describe_solution(false),
  _hea_bound_moment(false),
  _hea_log_delta(false),
  _hea_log_dtu(false),
  _hea_log_error(false),
  _hea_log_selection(false),
  _hea_randomize_bit_order(false),
  _incremental_evaluation(false),
  _log_improvement(false),
  _map_random(false),
  _mmas_strict(false),
  _negation(false),
  _print_default_parameters(false),
  _print_header(false),
  _print_performance(false),
  _print_solution(false),
  _pv_log_entropy(false),
  _pv_log_pv(false),
  _restart(false),
  _rls_strict(false),
  _stop_on_maximum(false),
  _stop_on_target(false)
{
  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_ALGORITHM,
    OPTION_BM_MC_RESET_STRATEGY,
    OPTION_BM_NUM_GS_CYCLES,
    OPTION_BM_NUM_GS_STEPS,
    OPTION_BM_SAMPLING,
    OPTION_BUDGET,
    OPTION_BV_SIZE,
    OPTION_EA_LAMBDA,
    OPTION_EA_MU,
    OPTION_FUN_NAME,
    OPTION_FUN_NUM_TRAPS,
    OPTION_FUN_PREFIX_LENGTH,
    OPTION_FUN_THRESHOLD,
    OPTION_FUNCTION,
    OPTION_GA_CROSSOVER_PROBABILITY,
    OPTION_GA_TOURNAMENT_SIZE,
    OPTION_HEA_BINARY_DYNAMICS,
    OPTION_HEA_DELAY,
    OPTION_HEA_NUM_PAR_UPDATES,
    OPTION_HEA_NUM_SEQ_UPDATES,
    OPTION_HEA_RATE_STRATEGY,
    OPTION_HEA_RESET_PERIOD,
    OPTION_HEA_SAMPLING_METHOD,
    OPTION_HEA_TIME_CONSTANT,
    OPTION_HEA_WEIGHT,
    OPTION_LEARNING_RATE,
    OPTION_MAP,
    OPTION_MAP_INPUT_SIZE,
    OPTION_MAP_PATH,
    OPTION_NEIGHBORHOOD,
    OPTION_NEIGHBORHOOD_ITERATOR,
    OPTION_NOISE_STDDEV,
    OPTION_NUM_ITERATIONS,
    OPTION_NUM_THREADS,
    OPTION_PATH,
    OPTION_POPULATION_SIZE,
    OPTION_PV_LOG_NUM_COMPONENTS,
    OPTION_RADIUS,
    OPTION_RLS_PATIENCE,
    OPTION_SA_INITIAL_ACCEPTANCE_PROBABILITY,
    OPTION_SA_NUM_TRANSITIONS,
    OPTION_SA_NUM_TRIALS,
    OPTION_SA_RATE,
    OPTION_SCALED_MUTATION_PROBABILITY,
    OPTION_SEED,
    OPTION_SELECTION_SIZE,
    OPTION_TARGET,
    OPTION_ADDITIVE_GAUSSIAN_NOISE,
    OPTION_BM_LOG_NORM_INFINITE,
    OPTION_BM_LOG_NORM_L1,
    OPTION_BM_NEGATIVE_POSITIVE_SELECTION,
    OPTION_CACHE,
    OPTION_DESCRIBE_PROBLEM,
    OPTION_DESCRIBE_SOLUTION,
    OPTION_HEA_BOUND_MOMENT,
    OPTION_HEA_LOG_DELTA,
    OPTION_HEA_LOG_DTU,
    OPTION_HEA_LOG_ERROR,
    OPTION_HEA_LOG_SELECTION,
    OPTION_HEA_RANDOMIZE_BIT_ORDER,
    OPTION_INCREMENTAL_EVALUATION,
    OPTION_LOG_IMPROVEMENT,
    OPTION_MAP_RANDOM,
    OPTION_MMAS_STRICT,
    OPTION_NEGATION,
    OPTION_PRINT_DEFAULT_PARAMETERS,
    OPTION_PRINT_HEADER,
    OPTION_PRINT_PERFORMANCE,
    OPTION_PRINT_SOLUTION,
    OPTION_PV_LOG_ENTROPY,
    OPTION_PV_LOG_PV,
    OPTION_RESTART,
    OPTION_RLS_STRICT,
    OPTION_STOP_ON_MAXIMUM,
    OPTION_STOP_ON_TARGET
  };
  const struct option long_options[] = {
    {"algorithm", required_argument, 0, OPTION_ALGORITHM},
    {"bm-mc-reset-strategy", required_argument, 0, OPTION_BM_MC_RESET_STRATEGY},
    {"bm-num-gs-cycles", required_argument, 0, OPTION_BM_NUM_GS_CYCLES},
    {"bm-num-gs-steps", required_argument, 0, OPTION_BM_NUM_GS_STEPS},
    {"bm-sampling", required_argument, 0, OPTION_BM_SAMPLING},
    {"budget", required_argument, 0, OPTION_BUDGET},
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"ea-lambda", required_argument, 0, OPTION_EA_LAMBDA},
    {"ea-mu", required_argument, 0, OPTION_EA_MU},
    {"fun-name", required_argument, 0, OPTION_FUN_NAME},
    {"fun-num-traps", required_argument, 0, OPTION_FUN_NUM_TRAPS},
    {"fun-prefix-length", required_argument, 0, OPTION_FUN_PREFIX_LENGTH},
    {"fun-threshold", required_argument, 0, OPTION_FUN_THRESHOLD},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"ga-crossover-probability", required_argument, 0, OPTION_GA_CROSSOVER_PROBABILITY},
    {"ga-tournament-size", required_argument, 0, OPTION_GA_TOURNAMENT_SIZE},
    {"hea-binary-dynamics", required_argument, 0, OPTION_HEA_BINARY_DYNAMICS},
    {"hea-delay", required_argument, 0, OPTION_HEA_DELAY},
    {"hea-num-par-updates", required_argument, 0, OPTION_HEA_NUM_PAR_UPDATES},
    {"hea-num-seq-updates", required_argument, 0, OPTION_HEA_NUM_SEQ_UPDATES},
    {"hea-rate-strategy", required_argument, 0, OPTION_HEA_RATE_STRATEGY},
    {"hea-reset-period", required_argument, 0, OPTION_HEA_RESET_PERIOD},
    {"hea-sampling-method", required_argument, 0, OPTION_HEA_SAMPLING_METHOD},
    {"hea-time-constant", required_argument, 0, OPTION_HEA_TIME_CONSTANT},
    {"hea-weight", required_argument, 0, OPTION_HEA_WEIGHT},
    {"learning-rate", required_argument, 0, OPTION_LEARNING_RATE},
    {"map", required_argument, 0, OPTION_MAP},
    {"map-input-size", required_argument, 0, OPTION_MAP_INPUT_SIZE},
    {"map-path", required_argument, 0, OPTION_MAP_PATH},
    {"neighborhood", required_argument, 0, OPTION_NEIGHBORHOOD},
    {"neighborhood-iterator", required_argument, 0, OPTION_NEIGHBORHOOD_ITERATOR},
    {"noise-stddev", required_argument, 0, OPTION_NOISE_STDDEV},
    {"num-iterations", required_argument, 0, OPTION_NUM_ITERATIONS},
    {"num-threads", required_argument, 0, OPTION_NUM_THREADS},
    {"path", required_argument, 0, OPTION_PATH},
    {"population-size", required_argument, 0, OPTION_POPULATION_SIZE},
    {"pv-log-num-components", required_argument, 0, OPTION_PV_LOG_NUM_COMPONENTS},
    {"radius", required_argument, 0, OPTION_RADIUS},
    {"rls-patience", required_argument, 0, OPTION_RLS_PATIENCE},
    {"sa-initial-acceptance-probability", required_argument, 0, OPTION_SA_INITIAL_ACCEPTANCE_PROBABILITY},
    {"sa-num-transitions", required_argument, 0, OPTION_SA_NUM_TRANSITIONS},
    {"sa-num-trials", required_argument, 0, OPTION_SA_NUM_TRIALS},
    {"sa-rate", required_argument, 0, OPTION_SA_RATE},
    {"scaled-mutation-probability", required_argument, 0, OPTION_SCALED_MUTATION_PROBABILITY},
    {"seed", required_argument, 0, OPTION_SEED},
    {"selection-size", required_argument, 0, OPTION_SELECTION_SIZE},
    {"target", required_argument, 0, OPTION_TARGET},
    {"additive-gaussian-noise", no_argument, 0, OPTION_ADDITIVE_GAUSSIAN_NOISE},
    {"bm-log-norm-infinite", no_argument, 0, OPTION_BM_LOG_NORM_INFINITE},
    {"bm-log-norm-l1", no_argument, 0, OPTION_BM_LOG_NORM_L1},
    {"bm-negative-positive-selection", no_argument, 0, OPTION_BM_NEGATIVE_POSITIVE_SELECTION},
    {"cache", no_argument, 0, OPTION_CACHE},
    {"describe-problem", no_argument, 0, OPTION_DESCRIBE_PROBLEM},
    {"describe-solution", no_argument, 0, OPTION_DESCRIBE_SOLUTION},
    {"hea-bound-moment", no_argument, 0, OPTION_HEA_BOUND_MOMENT},
    {"hea-log-delta", no_argument, 0, OPTION_HEA_LOG_DELTA},
    {"hea-log-dtu", no_argument, 0, OPTION_HEA_LOG_DTU},
    {"hea-log-error", no_argument, 0, OPTION_HEA_LOG_ERROR},
    {"hea-log-selection", no_argument, 0, OPTION_HEA_LOG_SELECTION},
    {"hea-randomize-bit-order", no_argument, 0, OPTION_HEA_RANDOMIZE_BIT_ORDER},
    {"incremental-evaluation", no_argument, 0, OPTION_INCREMENTAL_EVALUATION},
    {"log-improvement", no_argument, 0, OPTION_LOG_IMPROVEMENT},
    {"map-random", no_argument, 0, OPTION_MAP_RANDOM},
    {"mmas-strict", no_argument, 0, OPTION_MMAS_STRICT},
    {"negation", no_argument, 0, OPTION_NEGATION},
    {"print-default-parameters", no_argument, 0, OPTION_PRINT_DEFAULT_PARAMETERS},
    {"print-header", no_argument, 0, OPTION_PRINT_HEADER},
    {"print-performance", no_argument, 0, OPTION_PRINT_PERFORMANCE},
    {"print-solution", no_argument, 0, OPTION_PRINT_SOLUTION},
    {"pv-log-entropy", no_argument, 0, OPTION_PV_LOG_ENTROPY},
    {"pv-log-pv", no_argument, 0, OPTION_PV_LOG_PV},
    {"restart", no_argument, 0, OPTION_RESTART},
    {"rls-strict", no_argument, 0, OPTION_RLS_STRICT},
    {"stop-on-maximum", no_argument, 0, OPTION_STOP_ON_MAXIMUM},
    {"stop-on-target", no_argument, 0, OPTION_STOP_ON_TARGET},
    {"help", no_argument, 0, OPTION_HELP},
    {"version", no_argument, 0, OPTION_VERSION},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "A:b:s:t:F:r:M:N:i:p:x:m:y:";
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 'A':
    case OPTION_ALGORITHM:
      set_algorithm(atoi(optarg));
      break;

    case OPTION_BM_MC_RESET_STRATEGY:
      set_bm_mc_reset_strategy(atoi(optarg));
      break;

    case OPTION_BM_NUM_GS_CYCLES:
      set_bm_num_gs_cycles(atoi(optarg));
      break;

    case OPTION_BM_NUM_GS_STEPS:
      set_bm_num_gs_steps(atoi(optarg));
      break;

    case OPTION_BM_SAMPLING:
      set_bm_sampling(atoi(optarg));
      break;

    case 'b':
    case OPTION_BUDGET:
      set_budget(atoi(optarg));
      break;

    case 's':
    case OPTION_BV_SIZE:
      set_bv_size(atoi(optarg));
      break;

    case OPTION_EA_LAMBDA:
      set_ea_lambda(atoi(optarg));
      break;

    case OPTION_EA_MU:
      set_ea_mu(atoi(optarg));
      break;

    case OPTION_FUN_NAME:
      set_fun_name(string(optarg));
      break;

    case OPTION_FUN_NUM_TRAPS:
      set_fun_num_traps(atoi(optarg));
      break;

    case OPTION_FUN_PREFIX_LENGTH:
      set_fun_prefix_length(atoi(optarg));
      break;

    case 't':
    case OPTION_FUN_THRESHOLD:
      set_fun_threshold(atoi(optarg));
      break;

    case 'F':
    case OPTION_FUNCTION:
      set_function(atoi(optarg));
      break;

    case OPTION_GA_CROSSOVER_PROBABILITY:
      set_ga_crossover_probability(atof(optarg));
      break;

    case OPTION_GA_TOURNAMENT_SIZE:
      set_ga_tournament_size(atoi(optarg));
      break;

    case OPTION_HEA_BINARY_DYNAMICS:
      set_hea_binary_dynamics(atoi(optarg));
      break;

    case OPTION_HEA_DELAY:
      set_hea_delay(atoi(optarg));
      break;

    case OPTION_HEA_NUM_PAR_UPDATES:
      set_hea_num_par_updates(atoi(optarg));
      break;

    case OPTION_HEA_NUM_SEQ_UPDATES:
      set_hea_num_seq_updates(atoi(optarg));
      break;

    case OPTION_HEA_RATE_STRATEGY:
      set_hea_rate_strategy(atoi(optarg));
      break;

    case OPTION_HEA_RESET_PERIOD:
      set_hea_reset_period(atoi(optarg));
      break;

    case OPTION_HEA_SAMPLING_METHOD:
      set_hea_sampling_method(atoi(optarg));
      break;

    case OPTION_HEA_TIME_CONSTANT:
      set_hea_time_constant(atof(optarg));
      break;

    case OPTION_HEA_WEIGHT:
      set_hea_weight(atof(optarg));
      break;

    case 'r':
    case OPTION_LEARNING_RATE:
      set_learning_rate(atof(optarg));
      break;

    case 'M':
    case OPTION_MAP:
      set_map(atoi(optarg));
      break;

    case OPTION_MAP_INPUT_SIZE:
      set_map_input_size(atoi(optarg));
      break;

    case OPTION_MAP_PATH:
      set_map_path(string(optarg));
      break;

    case 'N':
    case OPTION_NEIGHBORHOOD:
      set_neighborhood(atoi(optarg));
      break;

    case OPTION_NEIGHBORHOOD_ITERATOR:
      set_neighborhood_iterator(atoi(optarg));
      break;

    case OPTION_NOISE_STDDEV:
      set_noise_stddev(atof(optarg));
      break;

    case 'i':
    case OPTION_NUM_ITERATIONS:
      set_num_iterations(atoi(optarg));
      break;

    case OPTION_NUM_THREADS:
      set_num_threads(atoi(optarg));
      break;

    case 'p':
    case OPTION_PATH:
      set_path(string(optarg));
      break;

    case 'x':
    case OPTION_POPULATION_SIZE:
      set_population_size(atoi(optarg));
      break;

    case OPTION_PV_LOG_NUM_COMPONENTS:
      set_pv_log_num_components(atoi(optarg));
      break;

    case OPTION_RADIUS:
      set_radius(atoi(optarg));
      break;

    case OPTION_RLS_PATIENCE:
      set_rls_patience(atoi(optarg));
      break;

    case OPTION_SA_INITIAL_ACCEPTANCE_PROBABILITY:
      set_sa_initial_acceptance_probability(atof(optarg));
      break;

    case OPTION_SA_NUM_TRANSITIONS:
      set_sa_num_transitions(atoi(optarg));
      break;

    case OPTION_SA_NUM_TRIALS:
      set_sa_num_trials(atoi(optarg));
      break;

    case OPTION_SA_RATE:
      set_sa_rate(atof(optarg));
      break;

    case 'm':
    case OPTION_SCALED_MUTATION_PROBABILITY:
      set_scaled_mutation_probability(atof(optarg));
      break;

    case OPTION_SEED:
      set_seed(strtoul(optarg, NULL, 0));
      break;

    case 'y':
    case OPTION_SELECTION_SIZE:
      set_selection_size(atoi(optarg));
      break;

    case OPTION_TARGET:
      set_target(atof(optarg));
      break;

    case OPTION_ADDITIVE_GAUSSIAN_NOISE:
      _additive_gaussian_noise = true;
      break;

    case OPTION_BM_LOG_NORM_INFINITE:
      _bm_log_norm_infinite = true;
      break;

    case OPTION_BM_LOG_NORM_L1:
      _bm_log_norm_l1 = true;
      break;

    case OPTION_BM_NEGATIVE_POSITIVE_SELECTION:
      _bm_negative_positive_selection = true;
      break;

    case OPTION_CACHE:
      _cache = true;
      break;

    case OPTION_DESCRIBE_PROBLEM:
      _describe_problem = true;
      break;

    case OPTION_DESCRIBE_SOLUTION:
      _describe_solution = true;
      break;

    case OPTION_HEA_BOUND_MOMENT:
      _hea_bound_moment = true;
      break;

    case OPTION_HEA_LOG_DELTA:
      _hea_log_delta = true;
      break;

    case OPTION_HEA_LOG_DTU:
      _hea_log_dtu = true;
      break;

    case OPTION_HEA_LOG_ERROR:
      _hea_log_error = true;
      break;

    case OPTION_HEA_LOG_SELECTION:
      _hea_log_selection = true;
      break;

    case OPTION_HEA_RANDOMIZE_BIT_ORDER:
      _hea_randomize_bit_order = true;
      break;

    case OPTION_INCREMENTAL_EVALUATION:
      _incremental_evaluation = true;
      break;

    case OPTION_LOG_IMPROVEMENT:
      _log_improvement = true;
      break;

    case OPTION_MAP_RANDOM:
      _map_random = true;
      break;

    case OPTION_MMAS_STRICT:
      _mmas_strict = true;
      break;

    case OPTION_NEGATION:
      _negation = true;
      break;

    case OPTION_PRINT_DEFAULT_PARAMETERS:
      _print_default_parameters = true;
      break;

    case OPTION_PRINT_HEADER:
      _print_header = true;
      break;

    case OPTION_PRINT_PERFORMANCE:
      _print_performance = true;
      break;

    case OPTION_PRINT_SOLUTION:
      _print_solution = true;
      break;

    case OPTION_PV_LOG_ENTROPY:
      _pv_log_entropy = true;
      break;

    case OPTION_PV_LOG_PV:
      _pv_log_pv = true;
      break;

    case OPTION_RESTART:
      _restart = true;
      break;

    case OPTION_RLS_STRICT:
      _rls_strict = true;
      break;

    case OPTION_STOP_ON_MAXIMUM:
      _stop_on_maximum = true;
      break;

    case OPTION_STOP_ON_TARGET:
      _stop_on_target = true;
      break;

    case OPTION_HELP:
      print_help(cerr);
      exit(0);

    case OPTION_VERSION:
      print_version(cerr);
      exit(0);

    default:
      cerr << "For more information, please enter: " << _exec_name << " --help" << endl;
      exit(1);
    }
  }
}

void Options::print_help(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "General:" << endl;
  stream << "      --describe-problem" << endl;
  stream << "          Describe the problem and exit" << endl;
  stream << "      --describe-solution" << endl;
  stream << "          At the end, describe the solution" << endl;
  stream << "      --num-threads (type int, default to 1)" << endl;
  stream << "          Number of threads" << endl;
  stream << "      --print-default-parameters" << endl;
  stream << "          Print the default parameters and exit" << endl;
  stream << "      --print-header" << endl;
  stream << "          At the beginning, print the header" << endl;
  stream << "      --print-performance" << endl;
  stream << "          At the end, print performance (maximum and number of evaluations needed to reach it)" << endl;
  stream << "      --print-solution" << endl;
  stream << "          At the end, print the solution" << endl;
  stream << "      --seed (type unsigned, default to 0)" << endl;
  stream << "          Seed for the random number generator" << endl;
  stream << endl;
  stream << "Function:" << endl;
  stream << "  -s, --bv-size (type int, default to 100)" << endl;
  stream << "          Size of bit vectors" << endl;
  stream << "      --fun-name (type string, default to \"noname\")" << endl;
  stream << "          Name of the function in the dynamic library" << endl;
  stream << "      --fun-num-traps (type int, default to 10)" << endl;
  stream << "          Number of traps" << endl;
  stream << "      --fun-prefix-length (type int, default to 2)" << endl;
  stream << "          Prefix length for long path" << endl;
  stream << "  -t, --fun-threshold (type int, default to 10)" << endl;
  stream << "          Threshold (in bits) for Jump, Four Peaks, and Six Peaks" << endl;
  stream << "  -F, --function (type int, default to 0)" << endl;
  stream << "          Type of function" << endl;
  stream << "            0: OneMax" << endl;
  stream << "            1: Linear function" << endl;
  stream << "            10: Leading ones" << endl;
  stream << "            11: Ridge" << endl;
  stream << "            20: Needle" << endl;
  stream << "            30: Jump" << endl;
  stream << "            31: Deceptive jump (aka Jump_k)" << endl;
  stream << "            40: Four peaks" << endl;
  stream << "            41: Six peaks" << endl;
  stream << "            50: Quadratic unconstrained binary optimization (Qubo)" << endl;
  stream << "            60: NK landscape" << endl;
  stream << "            70: Max-SAT" << endl;
  stream << "            80: Low autocorrelation binary sequence" << endl;
  stream << "            90: Equal products" << endl;
  stream << "            100: Cancellation" << endl;
  stream << "            101: Cancellation with sinus" << endl;
  stream << "            110: Trap" << endl;
  stream << "            120: Hierarchical if and only if (Hiff)" << endl;
  stream << "            130: Plateau" << endl;
  stream << "            140: Long path" << endl;
  stream << "            150: Factorization" << endl;
  stream << "            160: Walsh expansion" << endl;
  stream << "            161: Walsh expansion of degree 1" << endl;
  stream << "            162: Walsh expansion of degree 2" << endl;
  stream << "            1000: Plugin" << endl;
  stream << "  -p, --path (type string, default to \"nopath\")" << endl;
  stream << "          Path of a function file" << endl;
  stream << endl;
  stream << "Function decorators:" << endl;
  stream << "      --additive-gaussian-noise" << endl;
  stream << "          Additive Gaussian noise" << endl;
  stream << "  -b, --budget (type int, default to 10000)" << endl;
  stream << "          Number of allowed function evaluations (<= 0 means indefinite)" << endl;
  stream << "      --cache" << endl;
  stream << "          Cache function evaluations" << endl;
  stream << "      --log-improvement" << endl;
  stream << "          Log improvement" << endl;
  stream << "      --negation" << endl;
  stream << "          Negation (hence minimization) of the function" << endl;
  stream << "      --noise-stddev (type double, default to 1)" << endl;
  stream << "          Noise standard deviation" << endl;
  stream << "      --stop-on-maximum" << endl;
  stream << "          Stop on maximum" << endl;
  stream << "      --stop-on-target" << endl;
  stream << "          Stop on target" << endl;
  stream << "      --target (type double, default to 100)" << endl;
  stream << "          Target" << endl;
  stream << endl;
  stream << "Map:" << endl;
  stream << "  -M, --map (type int, default to 0)" << endl;
  stream << "          Type of map" << endl;
  stream << "            0: None" << endl;
  stream << "            1: Translation" << endl;
  stream << "            2: Permutation" << endl;
  stream << "            3: Composition of permutation and translation" << endl;
  stream << "            4: Linear" << endl;
  stream << "            5: Affine" << endl;
  stream << "      --map-input-size (type int, default to 100)" << endl;
  stream << "          Input size of linear and affine maps" << endl;
  stream << "      --map-path (type string, default to \"nopath\")" << endl;
  stream << "          Path of a map file" << endl;
  stream << "      --map-random" << endl;
  stream << "          Sample a random map" << endl;
  stream << endl;
  stream << "Algorithm:" << endl;
  stream << "  -A, --algorithm (type int, default to 100)" << endl;
  stream << "          Type of algorithm" << endl;
  stream << "            0: Complete search" << endl;
  stream << "            10: Random search" << endl;
  stream << "            100: Random local search" << endl;
  stream << "            150: Steepest ascent hill climbing" << endl;
  stream << "            200: Simulated annealing" << endl;
  stream << "            300: (1+1) evolutionary algorithm" << endl;
  stream << "            310: (mu+lambda) evolutionary algorithm" << endl;
  stream << "            320: (mu,lambda) evolutionary algorithm" << endl;
  stream << "            400: Genetic algorithm" << endl;
  stream << "            500: Population-based incremental learning" << endl;
  stream << "            501: PBIL with negative and positive selection" << endl;
  stream << "            600: Univariate marginal distribution algorithm" << endl;
  stream << "            700: Compact genetic algorithm" << endl;
  stream << "            800: Max-min ant system" << endl;
  stream << "            900: Herding evolutionary algorithm, herding with binary variables" << endl;
  stream << "            901: Herding evolutionary algorithm, herding with spin variables" << endl;
  stream << "            1000: Boltzmann machine PBIL" << endl;
  stream << "  -i, --num-iterations (type int, default to 0)" << endl;
  stream << "          Number of iterations (<= 0 means indefinite)" << endl;
  stream << "      --restart" << endl;
  stream << "          Restart any algorithm an indefinite number of times" << endl;
  stream << "  -m, --scaled-mutation-probability (type double, default to 1)" << endl;
  stream << "          Scaled mutation probability m = n * p (p = m / n)" << endl;
  stream << endl;
  stream << "Local Search:" << endl;
  stream << "      --incremental-evaluation" << endl;
  stream << "          Incremental evaluation" << endl;
  stream << "  -N, --neighborhood (type int, default to 0)" << endl;
  stream << "          Type of neighborhood" << endl;
  stream << "            0: One bit flip" << endl;
  stream << "            1: Bernoulli process" << endl;
  stream << "            2: Hamming ball" << endl;
  stream << "            3: Hamming sphere" << endl;
  stream << "      --neighborhood-iterator (type int, default to 0)" << endl;
  stream << "          Type of neighborhood iterator" << endl;
  stream << "            0: One bit flip iterator" << endl;
  stream << "            1: Hamming ball iterator" << endl;
  stream << "      --radius (type int, default to 2)" << endl;
  stream << "          Radius of Hamming ball or sphere" << endl;
  stream << "      --rls-patience (type int, default to 50)" << endl;
  stream << "          Number of consecutive rejected moves before throwing LocalMaximum (<= 0 means infinite)" << endl;
  stream << "      --rls-strict" << endl;
  stream << "          Strict (>) random local search" << endl;
  stream << endl;
  stream << "Simulated Annealing:" << endl;
  stream << "      --sa-initial-acceptance-probability (type double, default to 0.6)" << endl;
  stream << "          Initial acceptance probability" << endl;
  stream << "      --sa-num-transitions (type int, default to 50)" << endl;
  stream << "          Number of accepted transitions before annealing" << endl;
  stream << "      --sa-num-trials (type int, default to 100)" << endl;
  stream << "          Number of trials to estimate initial inverse temperature" << endl;
  stream << "      --sa-rate (type double, default to 1.2)" << endl;
  stream << "          Increase rate for inverse temperature" << endl;
  stream << endl;
  stream << "Evolutionary Algorithms:" << endl;
  stream << "      --ea-lambda (type int, default to 100)" << endl;
  stream << "          Offspring population size" << endl;
  stream << "      --ea-mu (type int, default to 10)" << endl;
  stream << "          Parent population size" << endl;
  stream << "      --ga-crossover-probability (type double, default to 0.5)" << endl;
  stream << "          Crossover probability" << endl;
  stream << "      --ga-tournament-size (type int, default to 10)" << endl;
  stream << "          Tournament size" << endl;
  stream << endl;
  stream << "EDA:" << endl;
  stream << "  -r, --learning-rate (type double, default to 0.001)" << endl;
  stream << "          Learning rate" << endl;
  stream << "      --mmas-strict" << endl;
  stream << "          Strict (>) max-min ant system" << endl;
  stream << "  -x, --population-size (type int, default to 10)" << endl;
  stream << "          Size of the population" << endl;
  stream << "      --pv-log-entropy" << endl;
  stream << "          Log entropy of probability vector" << endl;
  stream << "      --pv-log-num-components (type int, default to 5)" << endl;
  stream << "          Number of probability vector components to log" << endl;
  stream << "      --pv-log-pv" << endl;
  stream << "          Log probability vector" << endl;
  stream << "  -y, --selection-size (type int, default to 1)" << endl;
  stream << "          Size of the selection (number of selected individuals)" << endl;
  stream << endl;
  stream << "HEA:" << endl;
  stream << "      --hea-binary-dynamics (type int, default to 0)" << endl;
  stream << "          Herding dynamics for binary variables" << endl;
  stream << "            0: Minimize distance" << endl;
  stream << "            1: Maximize inner product" << endl;
  stream << "      --hea-bound-moment" << endl;
  stream << "          Bound moment after update" << endl;
  stream << "      --hea-delay (type int, default to 10000)" << endl;
  stream << "          Delay before learning rate decay" << endl;
  stream << "      --hea-log-delta" << endl;
  stream << "          Log norm of delta, the moment increment" << endl;
  stream << "      --hea-log-dtu" << endl;
  stream << "          Log distance to uniform" << endl;
  stream << "      --hea-log-error" << endl;
  stream << "          Log error (moment discrepancy)" << endl;
  stream << "      --hea-log-selection" << endl;
  stream << "          Log the distance between the target and the selection moment" << endl;
  stream << "      --hea-num-par-updates (type int, default to 1)" << endl;
  stream << "          Number of parallel updates per sample" << endl;
  stream << "      --hea-num-seq-updates (type int, default to 100)" << endl;
  stream << "          Number of sequential updates per sample" << endl;
  stream << "      --hea-randomize-bit-order" << endl;
  stream << "          Randomize bit order" << endl;
  stream << "      --hea-rate-strategy (type int, default to 0)" << endl;
  stream << "          Rate strategy" << endl;
  stream << "            0: Constant rate" << endl;
  stream << "            1: Exponential decay" << endl;
  stream << "            2: Inverse decay" << endl;
  stream << "      --hea-reset-period (type int, default to 0)" << endl;
  stream << "          Reset period (<= 0 means no reset)" << endl;
  stream << "      --hea-sampling-method (type int, default to 0)" << endl;
  stream << "          Sampling method for spin variables only" << endl;
  stream << "            0: Greedy algorithm" << endl;
  stream << "            1: Random local search" << endl;
  stream << "            2: Deterministic local search" << endl;
  stream << "            3: Neural network" << endl;
  stream << "      --hea-time-constant (type double, default to 1000)" << endl;
  stream << "          Time constant for exponential decay" << endl;
  stream << "      --hea-weight (type double, default to 1)" << endl;
  stream << "          Weight of second moments" << endl;
  stream << endl;
  stream << "Boltzmann Machine PBIL:" << endl;
  stream << "      --bm-log-norm-infinite" << endl;
  stream << "          Log infinite norm of the parameters" << endl;
  stream << "      --bm-log-norm-l1" << endl;
  stream << "          Log L1 norm of the parameters" << endl;
  stream << "      --bm-mc-reset-strategy (type int, default to 1)" << endl;
  stream << "          Markov chain reset strategy" << endl;
  stream << "            0: No reset" << endl;
  stream << "            1: Reset MC at the beginning of each iteration" << endl;
  stream << "            2: Reset MC before sampling each bit vector" << endl;
  stream << "      --bm-negative-positive-selection" << endl;
  stream << "          Negative and positive selection" << endl;
  stream << "      --bm-num-gs-cycles (type int, default to 1)" << endl;
  stream << "          Number of Gibbs sampler cycles per bit vector" << endl;
  stream << "      --bm-num-gs-steps (type int, default to 100)" << endl;
  stream << "          Number of Gibbs sampler steps per bit vector" << endl;
  stream << "      --bm-sampling (type int, default to 1)" << endl;
  stream << "          Sampling mode for the Boltzmann machine" << endl;
  stream << "            0: Asynchronous sampling" << endl;
  stream << "            1: Asynchronous sampling with full scan" << endl;
  stream << "            2: Synchronous sampling" << endl;
  stream << endl;
}

void Options::print_version(ostream& stream) const
{
  stream << _version << endl;
}

ostream& operator<<(ostream& stream, const Options& options)
{
  stream << "# algorithm = " << options._algorithm << endl;
  stream << "# bm_mc_reset_strategy = " << options._bm_mc_reset_strategy << endl;
  stream << "# bm_num_gs_cycles = " << options._bm_num_gs_cycles << endl;
  stream << "# bm_num_gs_steps = " << options._bm_num_gs_steps << endl;
  stream << "# bm_sampling = " << options._bm_sampling << endl;
  stream << "# budget = " << options._budget << endl;
  stream << "# bv_size = " << options._bv_size << endl;
  stream << "# ea_lambda = " << options._ea_lambda << endl;
  stream << "# ea_mu = " << options._ea_mu << endl;
  stream << "# fun_name = " << options._fun_name << endl;
  stream << "# fun_num_traps = " << options._fun_num_traps << endl;
  stream << "# fun_prefix_length = " << options._fun_prefix_length << endl;
  stream << "# fun_threshold = " << options._fun_threshold << endl;
  stream << "# function = " << options._function << endl;
  stream << "# ga_crossover_probability = " << options._ga_crossover_probability << endl;
  stream << "# ga_tournament_size = " << options._ga_tournament_size << endl;
  stream << "# hea_binary_dynamics = " << options._hea_binary_dynamics << endl;
  stream << "# hea_delay = " << options._hea_delay << endl;
  stream << "# hea_num_par_updates = " << options._hea_num_par_updates << endl;
  stream << "# hea_num_seq_updates = " << options._hea_num_seq_updates << endl;
  stream << "# hea_rate_strategy = " << options._hea_rate_strategy << endl;
  stream << "# hea_reset_period = " << options._hea_reset_period << endl;
  stream << "# hea_sampling_method = " << options._hea_sampling_method << endl;
  stream << "# hea_time_constant = " << options._hea_time_constant << endl;
  stream << "# hea_weight = " << options._hea_weight << endl;
  stream << "# learning_rate = " << options._learning_rate << endl;
  stream << "# map = " << options._map << endl;
  stream << "# map_input_size = " << options._map_input_size << endl;
  stream << "# map_path = " << options._map_path << endl;
  stream << "# neighborhood = " << options._neighborhood << endl;
  stream << "# neighborhood_iterator = " << options._neighborhood_iterator << endl;
  stream << "# noise_stddev = " << options._noise_stddev << endl;
  stream << "# num_iterations = " << options._num_iterations << endl;
  stream << "# num_threads = " << options._num_threads << endl;
  stream << "# path = " << options._path << endl;
  stream << "# population_size = " << options._population_size << endl;
  stream << "# pv_log_num_components = " << options._pv_log_num_components << endl;
  stream << "# radius = " << options._radius << endl;
  stream << "# rls_patience = " << options._rls_patience << endl;
  stream << "# sa_initial_acceptance_probability = " << options._sa_initial_acceptance_probability << endl;
  stream << "# sa_num_transitions = " << options._sa_num_transitions << endl;
  stream << "# sa_num_trials = " << options._sa_num_trials << endl;
  stream << "# sa_rate = " << options._sa_rate << endl;
  stream << "# scaled_mutation_probability = " << options._scaled_mutation_probability << endl;
  stream << "# seed = " << options._seed << endl;
  stream << "# selection_size = " << options._selection_size << endl;
  stream << "# target = " << options._target << endl;
  if (options._additive_gaussian_noise)
    stream << "# additive_gaussian_noise" << endl;
  if (options._bm_log_norm_infinite)
    stream << "# bm_log_norm_infinite" << endl;
  if (options._bm_log_norm_l1)
    stream << "# bm_log_norm_l1" << endl;
  if (options._bm_negative_positive_selection)
    stream << "# bm_negative_positive_selection" << endl;
  if (options._cache)
    stream << "# cache" << endl;
  if (options._describe_problem)
    stream << "# describe_problem" << endl;
  if (options._describe_solution)
    stream << "# describe_solution" << endl;
  if (options._hea_bound_moment)
    stream << "# hea_bound_moment" << endl;
  if (options._hea_log_delta)
    stream << "# hea_log_delta" << endl;
  if (options._hea_log_dtu)
    stream << "# hea_log_dtu" << endl;
  if (options._hea_log_error)
    stream << "# hea_log_error" << endl;
  if (options._hea_log_selection)
    stream << "# hea_log_selection" << endl;
  if (options._hea_randomize_bit_order)
    stream << "# hea_randomize_bit_order" << endl;
  if (options._incremental_evaluation)
    stream << "# incremental_evaluation" << endl;
  if (options._log_improvement)
    stream << "# log_improvement" << endl;
  if (options._map_random)
    stream << "# map_random" << endl;
  if (options._mmas_strict)
    stream << "# mmas_strict" << endl;
  if (options._negation)
    stream << "# negation" << endl;
  if (options._print_default_parameters)
    stream << "# print_default_parameters" << endl;
  if (options._print_header)
    stream << "# print_header" << endl;
  if (options._print_performance)
    stream << "# print_performance" << endl;
  if (options._print_solution)
    stream << "# print_solution" << endl;
  if (options._pv_log_entropy)
    stream << "# pv_log_entropy" << endl;
  if (options._pv_log_pv)
    stream << "# pv_log_pv" << endl;
  if (options._restart)
    stream << "# restart" << endl;
  if (options._rls_strict)
    stream << "# rls_strict" << endl;
  if (options._stop_on_maximum)
    stream << "# stop_on_maximum" << endl;
  if (options._stop_on_target)
    stream << "# stop_on_target" << endl;
  stream << "# last_parameter" << endl;
  stream << "# exec_name = " << options._exec_name << endl;
  stream << "# version = " << options._version << endl;
  stream << "# Generated from hnco.json" << endl;
  return stream;
}
