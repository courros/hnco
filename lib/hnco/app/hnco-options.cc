#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "hnco-options.hh"

using namespace hnco::app;

HncoOptions::HncoOptions(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.19"),
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
  _description_path("description.txt"),
  _opt_description_path(false),
  _ea_lambda(100),
  _opt_ea_lambda(false),
  _ea_mu(10),
  _opt_ea_mu(false),
  _expression("x"),
  _opt_expression(false),
  _fn_name("noname"),
  _opt_fn_name(false),
  _fn_num_traps(10),
  _opt_fn_num_traps(false),
  _fn_prefix_length(2),
  _opt_fn_prefix_length(false),
  _fn_threshold(10),
  _opt_fn_threshold(false),
  _fp_expression("(1-x)^2+100*(y-x^2)^2"),
  _opt_fp_expression(false),
  _fp_lower_bound(-2),
  _opt_fp_lower_bound(false),
  _fp_num_bits(8),
  _opt_fp_num_bits(false),
  _fp_precision(0.01),
  _opt_fp_precision(false),
  _fp_upper_bound(2),
  _opt_fp_upper_bound(false),
  _function(0),
  _opt_function(false),
  _ga_crossover_bias(0.5),
  _opt_ga_crossover_bias(false),
  _ga_crossover_probability(0.5),
  _opt_ga_crossover_probability(false),
  _ga_tournament_size(10),
  _opt_ga_tournament_size(false),
  _hea_reset_period(0),
  _opt_hea_reset_period(false),
  _learning_rate(0.001),
  _opt_learning_rate(false),
  _map(0),
  _opt_map(false),
  _map_input_size(100),
  _opt_map_input_size(false),
  _map_path("map.txt"),
  _opt_map_path(false),
  _map_ts_length(10),
  _opt_map_ts_length(false),
  _map_ts_sampling_mode(0),
  _opt_map_ts_sampling_mode(false),
  _mutation_rate(1),
  _opt_mutation_rate(false),
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
  _path("function.txt"),
  _opt_path(false),
  _pn_mutation_rate(1),
  _opt_pn_mutation_rate(false),
  _pn_neighborhood(0),
  _opt_pn_neighborhood(false),
  _pn_radius(2),
  _opt_pn_radius(false),
  _population_size(10),
  _opt_population_size(false),
  _pv_log_num_components(5),
  _opt_pv_log_num_components(false),
  _radius(2),
  _opt_radius(false),
  _rep_categorical_representation(0),
  _opt_rep_categorical_representation(false),
  _rep_num_additional_bits(2),
  _opt_rep_num_additional_bits(false),
  _results_path("results.json"),
  _opt_results_path(false),
  _rls_patience(50),
  _opt_rls_patience(false),
  _sa_beta_ratio(1.2),
  _opt_sa_beta_ratio(false),
  _sa_initial_acceptance_probability(0.6),
  _opt_sa_initial_acceptance_probability(false),
  _sa_num_transitions(50),
  _opt_sa_num_transitions(false),
  _sa_num_trials(100),
  _opt_sa_num_trials(false),
  _seed(0),
  _opt_seed(false),
  _selection_size(1),
  _opt_selection_size(false),
  _solution_path("solution.txt"),
  _opt_solution_path(false),
  _target(100),
  _opt_target(false),
  _additive_gaussian_noise(false),
  _allow_no_mutation(false),
  _bm_log_norm_infinite(false),
  _bm_log_norm_l1(false),
  _bm_negative_positive_selection(false),
  _cache(false),
  _cache_budget(false),
  _concrete_solution(false),
  _fn_display(false),
  _fn_get_bv_size(false),
  _fn_get_maximum(false),
  _fn_has_known_maximum(false),
  _fn_provides_incremental_evaluation(false),
  _fn_walsh_transform(false),
  _hea_bound_moment(false),
  _hea_log_delta(false),
  _hea_log_dtu(false),
  _hea_log_error(false),
  _hea_log_moment_matrix(false),
  _hea_randomize_bit_order(false),
  _incremental_evaluation(false),
  _load_solution(false),
  _log_improvement(false),
  _map_display(false),
  _map_random(false),
  _map_surjective(false),
  _mmas_strict(false),
  _negation(false),
  _parsed_modifier(false),
  _pn_allow_no_mutation(false),
  _print_defaults(false),
  _print_description(false),
  _print_header(false),
  _print_results(false),
  _print_solution(false),
  _prior_noise(false),
  _pv_log_entropy(false),
  _pv_log_pv(false),
  _record_evaluation_time(false),
  _restart(false),
  _rls_strict(false),
  _rw_log_value(false),
  _save_description(false),
  _save_results(false),
  _save_solution(false),
  _stop_on_maximum(false),
  _stop_on_target(false)
{
  enum {
    OPTION_HELP=256,
    OPTION_HELP_BM,
    OPTION_HELP_EA,
    OPTION_HELP_EDA,
    OPTION_HELP_FP,
    OPTION_HELP_HEA,
    OPTION_HELP_LS,
    OPTION_HELP_MAP,
    OPTION_HELP_PN,
    OPTION_HELP_REP,
    OPTION_HELP_SA,
    OPTION_VERSION,
    OPTION_ALGORITHM,
    OPTION_BM_MC_RESET_STRATEGY,
    OPTION_BM_NUM_GS_CYCLES,
    OPTION_BM_NUM_GS_STEPS,
    OPTION_BM_SAMPLING,
    OPTION_BUDGET,
    OPTION_BV_SIZE,
    OPTION_DESCRIPTION_PATH,
    OPTION_EA_LAMBDA,
    OPTION_EA_MU,
    OPTION_EXPRESSION,
    OPTION_FN_NAME,
    OPTION_FN_NUM_TRAPS,
    OPTION_FN_PREFIX_LENGTH,
    OPTION_FN_THRESHOLD,
    OPTION_FP_EXPRESSION,
    OPTION_FP_LOWER_BOUND,
    OPTION_FP_NUM_BITS,
    OPTION_FP_PRECISION,
    OPTION_FP_UPPER_BOUND,
    OPTION_FUNCTION,
    OPTION_GA_CROSSOVER_BIAS,
    OPTION_GA_CROSSOVER_PROBABILITY,
    OPTION_GA_TOURNAMENT_SIZE,
    OPTION_HEA_RESET_PERIOD,
    OPTION_LEARNING_RATE,
    OPTION_MAP,
    OPTION_MAP_INPUT_SIZE,
    OPTION_MAP_PATH,
    OPTION_MAP_TS_LENGTH,
    OPTION_MAP_TS_SAMPLING_MODE,
    OPTION_MUTATION_RATE,
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
    OPTION_ALLOW_NO_MUTATION,
    OPTION_BM_LOG_NORM_INFINITE,
    OPTION_BM_LOG_NORM_L1,
    OPTION_BM_NEGATIVE_POSITIVE_SELECTION,
    OPTION_CACHE,
    OPTION_CACHE_BUDGET,
    OPTION_CONCRETE_SOLUTION,
    OPTION_FN_DISPLAY,
    OPTION_FN_GET_BV_SIZE,
    OPTION_FN_GET_MAXIMUM,
    OPTION_FN_HAS_KNOWN_MAXIMUM,
    OPTION_FN_PROVIDES_INCREMENTAL_EVALUATION,
    OPTION_FN_WALSH_TRANSFORM,
    OPTION_HEA_BOUND_MOMENT,
    OPTION_HEA_LOG_DELTA,
    OPTION_HEA_LOG_DTU,
    OPTION_HEA_LOG_ERROR,
    OPTION_HEA_LOG_MOMENT_MATRIX,
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
    OPTION_PRINT_DEFAULTS,
    OPTION_PRINT_DESCRIPTION,
    OPTION_PRINT_HEADER,
    OPTION_PRINT_RESULTS,
    OPTION_PRINT_SOLUTION,
    OPTION_PRIOR_NOISE,
    OPTION_PV_LOG_ENTROPY,
    OPTION_PV_LOG_PV,
    OPTION_RECORD_EVALUATION_TIME,
    OPTION_RESTART,
    OPTION_RLS_STRICT,
    OPTION_RW_LOG_VALUE,
    OPTION_SAVE_DESCRIPTION,
    OPTION_SAVE_RESULTS,
    OPTION_SAVE_SOLUTION,
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
    {"description-path", required_argument, 0, OPTION_DESCRIPTION_PATH},
    {"ea-lambda", required_argument, 0, OPTION_EA_LAMBDA},
    {"ea-mu", required_argument, 0, OPTION_EA_MU},
    {"expression", required_argument, 0, OPTION_EXPRESSION},
    {"fn-name", required_argument, 0, OPTION_FN_NAME},
    {"fn-num-traps", required_argument, 0, OPTION_FN_NUM_TRAPS},
    {"fn-prefix-length", required_argument, 0, OPTION_FN_PREFIX_LENGTH},
    {"fn-threshold", required_argument, 0, OPTION_FN_THRESHOLD},
    {"fp-expression", required_argument, 0, OPTION_FP_EXPRESSION},
    {"fp-lower-bound", required_argument, 0, OPTION_FP_LOWER_BOUND},
    {"fp-num-bits", required_argument, 0, OPTION_FP_NUM_BITS},
    {"fp-precision", required_argument, 0, OPTION_FP_PRECISION},
    {"fp-upper-bound", required_argument, 0, OPTION_FP_UPPER_BOUND},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"ga-crossover-bias", required_argument, 0, OPTION_GA_CROSSOVER_BIAS},
    {"ga-crossover-probability", required_argument, 0, OPTION_GA_CROSSOVER_PROBABILITY},
    {"ga-tournament-size", required_argument, 0, OPTION_GA_TOURNAMENT_SIZE},
    {"hea-reset-period", required_argument, 0, OPTION_HEA_RESET_PERIOD},
    {"learning-rate", required_argument, 0, OPTION_LEARNING_RATE},
    {"map", required_argument, 0, OPTION_MAP},
    {"map-input-size", required_argument, 0, OPTION_MAP_INPUT_SIZE},
    {"map-path", required_argument, 0, OPTION_MAP_PATH},
    {"map-ts-length", required_argument, 0, OPTION_MAP_TS_LENGTH},
    {"map-ts-sampling-mode", required_argument, 0, OPTION_MAP_TS_SAMPLING_MODE},
    {"mutation-rate", required_argument, 0, OPTION_MUTATION_RATE},
    {"neighborhood", required_argument, 0, OPTION_NEIGHBORHOOD},
    {"neighborhood-iterator", required_argument, 0, OPTION_NEIGHBORHOOD_ITERATOR},
    {"noise-stddev", required_argument, 0, OPTION_NOISE_STDDEV},
    {"num-iterations", required_argument, 0, OPTION_NUM_ITERATIONS},
    {"num-threads", required_argument, 0, OPTION_NUM_THREADS},
    {"path", required_argument, 0, OPTION_PATH},
    {"pn-mutation-rate", required_argument, 0, OPTION_PN_MUTATION_RATE},
    {"pn-neighborhood", required_argument, 0, OPTION_PN_NEIGHBORHOOD},
    {"pn-radius", required_argument, 0, OPTION_PN_RADIUS},
    {"population-size", required_argument, 0, OPTION_POPULATION_SIZE},
    {"pv-log-num-components", required_argument, 0, OPTION_PV_LOG_NUM_COMPONENTS},
    {"radius", required_argument, 0, OPTION_RADIUS},
    {"rep-categorical-representation", required_argument, 0, OPTION_REP_CATEGORICAL_REPRESENTATION},
    {"rep-num-additional-bits", required_argument, 0, OPTION_REP_NUM_ADDITIONAL_BITS},
    {"results-path", required_argument, 0, OPTION_RESULTS_PATH},
    {"rls-patience", required_argument, 0, OPTION_RLS_PATIENCE},
    {"sa-beta-ratio", required_argument, 0, OPTION_SA_BETA_RATIO},
    {"sa-initial-acceptance-probability", required_argument, 0, OPTION_SA_INITIAL_ACCEPTANCE_PROBABILITY},
    {"sa-num-transitions", required_argument, 0, OPTION_SA_NUM_TRANSITIONS},
    {"sa-num-trials", required_argument, 0, OPTION_SA_NUM_TRIALS},
    {"seed", required_argument, 0, OPTION_SEED},
    {"selection-size", required_argument, 0, OPTION_SELECTION_SIZE},
    {"solution-path", required_argument, 0, OPTION_SOLUTION_PATH},
    {"target", required_argument, 0, OPTION_TARGET},
    {"additive-gaussian-noise", no_argument, 0, OPTION_ADDITIVE_GAUSSIAN_NOISE},
    {"allow-no-mutation", no_argument, 0, OPTION_ALLOW_NO_MUTATION},
    {"bm-log-norm-infinite", no_argument, 0, OPTION_BM_LOG_NORM_INFINITE},
    {"bm-log-norm-l1", no_argument, 0, OPTION_BM_LOG_NORM_L1},
    {"bm-negative-positive-selection", no_argument, 0, OPTION_BM_NEGATIVE_POSITIVE_SELECTION},
    {"cache", no_argument, 0, OPTION_CACHE},
    {"cache-budget", no_argument, 0, OPTION_CACHE_BUDGET},
    {"concrete-solution", no_argument, 0, OPTION_CONCRETE_SOLUTION},
    {"fn-display", no_argument, 0, OPTION_FN_DISPLAY},
    {"fn-get-bv-size", no_argument, 0, OPTION_FN_GET_BV_SIZE},
    {"fn-get-maximum", no_argument, 0, OPTION_FN_GET_MAXIMUM},
    {"fn-has-known-maximum", no_argument, 0, OPTION_FN_HAS_KNOWN_MAXIMUM},
    {"fn-provides-incremental-evaluation", no_argument, 0, OPTION_FN_PROVIDES_INCREMENTAL_EVALUATION},
    {"fn-walsh-transform", no_argument, 0, OPTION_FN_WALSH_TRANSFORM},
    {"hea-bound-moment", no_argument, 0, OPTION_HEA_BOUND_MOMENT},
    {"hea-log-delta", no_argument, 0, OPTION_HEA_LOG_DELTA},
    {"hea-log-dtu", no_argument, 0, OPTION_HEA_LOG_DTU},
    {"hea-log-error", no_argument, 0, OPTION_HEA_LOG_ERROR},
    {"hea-log-moment-matrix", no_argument, 0, OPTION_HEA_LOG_MOMENT_MATRIX},
    {"hea-randomize-bit-order", no_argument, 0, OPTION_HEA_RANDOMIZE_BIT_ORDER},
    {"incremental-evaluation", no_argument, 0, OPTION_INCREMENTAL_EVALUATION},
    {"load-solution", no_argument, 0, OPTION_LOAD_SOLUTION},
    {"log-improvement", no_argument, 0, OPTION_LOG_IMPROVEMENT},
    {"map-display", no_argument, 0, OPTION_MAP_DISPLAY},
    {"map-random", no_argument, 0, OPTION_MAP_RANDOM},
    {"map-surjective", no_argument, 0, OPTION_MAP_SURJECTIVE},
    {"mmas-strict", no_argument, 0, OPTION_MMAS_STRICT},
    {"negation", no_argument, 0, OPTION_NEGATION},
    {"parsed-modifier", no_argument, 0, OPTION_PARSED_MODIFIER},
    {"pn-allow-no-mutation", no_argument, 0, OPTION_PN_ALLOW_NO_MUTATION},
    {"print-defaults", no_argument, 0, OPTION_PRINT_DEFAULTS},
    {"print-description", no_argument, 0, OPTION_PRINT_DESCRIPTION},
    {"print-header", no_argument, 0, OPTION_PRINT_HEADER},
    {"print-results", no_argument, 0, OPTION_PRINT_RESULTS},
    {"print-solution", no_argument, 0, OPTION_PRINT_SOLUTION},
    {"prior-noise", no_argument, 0, OPTION_PRIOR_NOISE},
    {"pv-log-entropy", no_argument, 0, OPTION_PV_LOG_ENTROPY},
    {"pv-log-pv", no_argument, 0, OPTION_PV_LOG_PV},
    {"record-evaluation-time", no_argument, 0, OPTION_RECORD_EVALUATION_TIME},
    {"restart", no_argument, 0, OPTION_RESTART},
    {"rls-strict", no_argument, 0, OPTION_RLS_STRICT},
    {"rw-log-value", no_argument, 0, OPTION_RW_LOG_VALUE},
    {"save-description", no_argument, 0, OPTION_SAVE_DESCRIPTION},
    {"save-results", no_argument, 0, OPTION_SAVE_RESULTS},
    {"save-solution", no_argument, 0, OPTION_SAVE_SOLUTION},
    {"stop-on-maximum", no_argument, 0, OPTION_STOP_ON_MAXIMUM},
    {"stop-on-target", no_argument, 0, OPTION_STOP_ON_TARGET},
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"help-fp", no_argument, 0, OPTION_HELP_FP},
    {"help-rep", no_argument, 0, OPTION_HELP_REP},
    {"help-pn", no_argument, 0, OPTION_HELP_PN},
    {"help-map", no_argument, 0, OPTION_HELP_MAP},
    {"help-ls", no_argument, 0, OPTION_HELP_LS},
    {"help-sa", no_argument, 0, OPTION_HELP_SA},
    {"help-ea", no_argument, 0, OPTION_HELP_EA},
    {"help-eda", no_argument, 0, OPTION_HELP_EDA},
    {"help-hea", no_argument, 0, OPTION_HELP_HEA},
    {"help-bm", no_argument, 0, OPTION_HELP_BM},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "A:b:s:t:F:l:M:m:N:i:p:x:y:";
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

    case OPTION_DESCRIPTION_PATH:
      set_description_path(std::string(optarg));
      break;

    case OPTION_EA_LAMBDA:
      set_ea_lambda(atoi(optarg));
      break;

    case OPTION_EA_MU:
      set_ea_mu(atoi(optarg));
      break;

    case OPTION_EXPRESSION:
      set_expression(std::string(optarg));
      break;

    case OPTION_FN_NAME:
      set_fn_name(std::string(optarg));
      break;

    case OPTION_FN_NUM_TRAPS:
      set_fn_num_traps(atoi(optarg));
      break;

    case OPTION_FN_PREFIX_LENGTH:
      set_fn_prefix_length(atoi(optarg));
      break;

    case 't':
    case OPTION_FN_THRESHOLD:
      set_fn_threshold(atoi(optarg));
      break;

    case OPTION_FP_EXPRESSION:
      set_fp_expression(std::string(optarg));
      break;

    case OPTION_FP_LOWER_BOUND:
      set_fp_lower_bound(atof(optarg));
      break;

    case OPTION_FP_NUM_BITS:
      set_fp_num_bits(atoi(optarg));
      break;

    case OPTION_FP_PRECISION:
      set_fp_precision(atof(optarg));
      break;

    case OPTION_FP_UPPER_BOUND:
      set_fp_upper_bound(atof(optarg));
      break;

    case 'F':
    case OPTION_FUNCTION:
      set_function(atoi(optarg));
      break;

    case OPTION_GA_CROSSOVER_BIAS:
      set_ga_crossover_bias(atof(optarg));
      break;

    case OPTION_GA_CROSSOVER_PROBABILITY:
      set_ga_crossover_probability(atof(optarg));
      break;

    case OPTION_GA_TOURNAMENT_SIZE:
      set_ga_tournament_size(atoi(optarg));
      break;

    case OPTION_HEA_RESET_PERIOD:
      set_hea_reset_period(atoi(optarg));
      break;

    case 'l':
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
      set_map_path(std::string(optarg));
      break;

    case OPTION_MAP_TS_LENGTH:
      set_map_ts_length(atoi(optarg));
      break;

    case OPTION_MAP_TS_SAMPLING_MODE:
      set_map_ts_sampling_mode(atoi(optarg));
      break;

    case 'm':
    case OPTION_MUTATION_RATE:
      set_mutation_rate(atof(optarg));
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
      set_path(std::string(optarg));
      break;

    case OPTION_PN_MUTATION_RATE:
      set_pn_mutation_rate(atof(optarg));
      break;

    case OPTION_PN_NEIGHBORHOOD:
      set_pn_neighborhood(atoi(optarg));
      break;

    case OPTION_PN_RADIUS:
      set_pn_radius(atoi(optarg));
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

    case OPTION_REP_CATEGORICAL_REPRESENTATION:
      set_rep_categorical_representation(atoi(optarg));
      break;

    case OPTION_REP_NUM_ADDITIONAL_BITS:
      set_rep_num_additional_bits(atoi(optarg));
      break;

    case OPTION_RESULTS_PATH:
      set_results_path(std::string(optarg));
      break;

    case OPTION_RLS_PATIENCE:
      set_rls_patience(atoi(optarg));
      break;

    case OPTION_SA_BETA_RATIO:
      set_sa_beta_ratio(atof(optarg));
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

    case OPTION_SEED:
      set_seed(strtoul(optarg, NULL, 0));
      break;

    case 'y':
    case OPTION_SELECTION_SIZE:
      set_selection_size(atoi(optarg));
      break;

    case OPTION_SOLUTION_PATH:
      set_solution_path(std::string(optarg));
      break;

    case OPTION_TARGET:
      set_target(atof(optarg));
      break;

    case OPTION_ADDITIVE_GAUSSIAN_NOISE:
      _additive_gaussian_noise = true;
      break;

    case OPTION_ALLOW_NO_MUTATION:
      _allow_no_mutation = true;
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

    case OPTION_CACHE_BUDGET:
      _cache_budget = true;
      break;

    case OPTION_CONCRETE_SOLUTION:
      _concrete_solution = true;
      break;

    case OPTION_FN_DISPLAY:
      _fn_display = true;
      break;

    case OPTION_FN_GET_BV_SIZE:
      _fn_get_bv_size = true;
      break;

    case OPTION_FN_GET_MAXIMUM:
      _fn_get_maximum = true;
      break;

    case OPTION_FN_HAS_KNOWN_MAXIMUM:
      _fn_has_known_maximum = true;
      break;

    case OPTION_FN_PROVIDES_INCREMENTAL_EVALUATION:
      _fn_provides_incremental_evaluation = true;
      break;

    case OPTION_FN_WALSH_TRANSFORM:
      _fn_walsh_transform = true;
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

    case OPTION_HEA_LOG_MOMENT_MATRIX:
      _hea_log_moment_matrix = true;
      break;

    case OPTION_HEA_RANDOMIZE_BIT_ORDER:
      _hea_randomize_bit_order = true;
      break;

    case OPTION_INCREMENTAL_EVALUATION:
      _incremental_evaluation = true;
      break;

    case OPTION_LOAD_SOLUTION:
      _load_solution = true;
      break;

    case OPTION_LOG_IMPROVEMENT:
      _log_improvement = true;
      break;

    case OPTION_MAP_DISPLAY:
      _map_display = true;
      break;

    case OPTION_MAP_RANDOM:
      _map_random = true;
      break;

    case OPTION_MAP_SURJECTIVE:
      _map_surjective = true;
      break;

    case OPTION_MMAS_STRICT:
      _mmas_strict = true;
      break;

    case OPTION_NEGATION:
      _negation = true;
      break;

    case OPTION_PARSED_MODIFIER:
      _parsed_modifier = true;
      break;

    case OPTION_PN_ALLOW_NO_MUTATION:
      _pn_allow_no_mutation = true;
      break;

    case OPTION_PRINT_DEFAULTS:
      _print_defaults = true;
      break;

    case OPTION_PRINT_DESCRIPTION:
      _print_description = true;
      break;

    case OPTION_PRINT_HEADER:
      _print_header = true;
      break;

    case OPTION_PRINT_RESULTS:
      _print_results = true;
      break;

    case OPTION_PRINT_SOLUTION:
      _print_solution = true;
      break;

    case OPTION_PRIOR_NOISE:
      _prior_noise = true;
      break;

    case OPTION_PV_LOG_ENTROPY:
      _pv_log_entropy = true;
      break;

    case OPTION_PV_LOG_PV:
      _pv_log_pv = true;
      break;

    case OPTION_RECORD_EVALUATION_TIME:
      _record_evaluation_time = true;
      break;

    case OPTION_RESTART:
      _restart = true;
      break;

    case OPTION_RLS_STRICT:
      _rls_strict = true;
      break;

    case OPTION_RW_LOG_VALUE:
      _rw_log_value = true;
      break;

    case OPTION_SAVE_DESCRIPTION:
      _save_description = true;
      break;

    case OPTION_SAVE_RESULTS:
      _save_results = true;
      break;

    case OPTION_SAVE_SOLUTION:
      _save_solution = true;
      break;

    case OPTION_STOP_ON_MAXIMUM:
      _stop_on_maximum = true;
      break;

    case OPTION_STOP_ON_TARGET:
      _stop_on_target = true;
      break;

    case OPTION_HELP:
      print_help(std::cerr);
      exit(0);

    case OPTION_HELP_FP:
      print_help_fp(std::cerr);
      exit(0);

    case OPTION_HELP_REP:
      print_help_rep(std::cerr);
      exit(0);

    case OPTION_HELP_PN:
      print_help_pn(std::cerr);
      exit(0);

    case OPTION_HELP_MAP:
      print_help_map(std::cerr);
      exit(0);

    case OPTION_HELP_LS:
      print_help_ls(std::cerr);
      exit(0);

    case OPTION_HELP_SA:
      print_help_sa(std::cerr);
      exit(0);

    case OPTION_HELP_EA:
      print_help_ea(std::cerr);
      exit(0);

    case OPTION_HELP_EDA:
      print_help_eda(std::cerr);
      exit(0);

    case OPTION_HELP_HEA:
      print_help_hea(std::cerr);
      exit(0);

    case OPTION_HELP_BM:
      print_help_bm(std::cerr);
      exit(0);

    case OPTION_VERSION:
      print_version(std::cerr);
      exit(0);

    default:
      std::cerr << "For more information, please enter: " << _exec_name << " --help" << std::endl;
      exit(1);
    }
  }
}

void HncoOptions::print_help(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "General" << std::endl;
  stream << "      --concrete-solution" << std::endl;
  stream << "          At the end, print or save the solution in the domain of the concrete function" << std::endl;
  stream << "      --description-path (type string, default to \"description.txt\")" << std::endl;
  stream << "          Path of the description file" << std::endl;
  stream << "      --load-solution" << std::endl;
  stream << "          Load a solution from a file" << std::endl;
  stream << "      --num-threads (type int, default to 1)" << std::endl;
  stream << "          Number of threads" << std::endl;
  stream << "      --print-defaults" << std::endl;
  stream << "          Print the default parameters and exit" << std::endl;
  stream << "      --print-description" << std::endl;
  stream << "          Print a description of the solution" << std::endl;
  stream << "      --print-header" << std::endl;
  stream << "          At the beginning, print the header" << std::endl;
  stream << "      --print-results" << std::endl;
  stream << "          Print results" << std::endl;
  stream << "      --print-solution" << std::endl;
  stream << "          Print the solution" << std::endl;
  stream << "      --results-path (type string, default to \"results.json\")" << std::endl;
  stream << "          Path of the results file" << std::endl;
  stream << "      --save-description" << std::endl;
  stream << "          At the end, save a description of the solution in a file" << std::endl;
  stream << "      --save-results" << std::endl;
  stream << "          At the end, save results in a file" << std::endl;
  stream << "      --save-solution" << std::endl;
  stream << "          At the end, save the solution in a file" << std::endl;
  stream << "      --seed (type unsigned, default to 0)" << std::endl;
  stream << "          Seed for the random number generator" << std::endl;
  stream << "      --solution-path (type string, default to \"solution.txt\")" << std::endl;
  stream << "          Path of the solution file" << std::endl;
  stream << std::endl;
  stream << "Functions" << std::endl;
  stream << "  -s, --bv-size (type int, default to 100)" << std::endl;
  stream << "          Size of bit vectors" << std::endl;
  stream << "      --fn-display" << std::endl;
  stream << "          Display the function and exit" << std::endl;
  stream << "      --fn-get-bv-size" << std::endl;
  stream << "          Print the size of bit vectors" << std::endl;
  stream << "      --fn-get-maximum" << std::endl;
  stream << "          If the maximum is known then print it and exit with status 0 else exit with status 1" << std::endl;
  stream << "      --fn-has-known-maximum" << std::endl;
  stream << "          Does the function have a known maximum?" << std::endl;
  stream << "      --fn-name (type string, default to \"noname\")" << std::endl;
  stream << "          Name of the function in the dynamic library" << std::endl;
  stream << "      --fn-num-traps (type int, default to 10)" << std::endl;
  stream << "          Number of traps" << std::endl;
  stream << "      --fn-prefix-length (type int, default to 2)" << std::endl;
  stream << "          Prefix length for long path" << std::endl;
  stream << "      --fn-provides-incremental-evaluation" << std::endl;
  stream << "          Does the function provide incremental evaluation?" << std::endl;
  stream << "  -t, --fn-threshold (type int, default to 10)" << std::endl;
  stream << "          Threshold (in bits) for Jump, Four Peaks, and Six Peaks" << std::endl;
  stream << "      --fn-walsh-transform" << std::endl;
  stream << "          Compute the Walsh transform of the function" << std::endl;
  stream << "  -F, --function (type int, default to 0)" << std::endl;
  stream << "          Type of function" << std::endl;
  stream << "            0: OneMax" << std::endl;
  stream << "            1: Linear function" << std::endl;
  stream << "            5: BinVal (linear function with weights in geometric progression 1, 2, 4...)" << std::endl;
  stream << "            10: Leading ones" << std::endl;
  stream << "            11: Ridge" << std::endl;
  stream << "            20: Needle" << std::endl;
  stream << "            30: Jump" << std::endl;
  stream << "            31: Deceptive jump (aka Jump_k)" << std::endl;
  stream << "            40: Four peaks" << std::endl;
  stream << "            41: Six peaks" << std::endl;
  stream << "            50: Quadratic unconstrained binary optimization (Qubo)" << std::endl;
  stream << "            60: NK landscape" << std::endl;
  stream << "            70: Max-SAT" << std::endl;
  stream << "            71: Max not-all-equal 3SAT" << std::endl;
  stream << "            80: Low autocorrelation binary sequence" << std::endl;
  stream << "            81: Low autocorrelation binary sequence merit factor" << std::endl;
  stream << "            90: Equal products" << std::endl;
  stream << "            91: Partition" << std::endl;
  stream << "            100: Summation cancellation" << std::endl;
  stream << "            101: Summation cancellation with sinus" << std::endl;
  stream << "            110: Trap" << std::endl;
  stream << "            120: Hierarchical if and only if (Hiff)" << std::endl;
  stream << "            130: Plateau" << std::endl;
  stream << "            140: Long path" << std::endl;
  stream << "            150: Factorization" << std::endl;
  stream << "            160: Walsh expansion" << std::endl;
  stream << "            161: Walsh expansion of degree 1" << std::endl;
  stream << "            162: Walsh expansion of degree 2" << std::endl;
  stream << "            171: Nearest neighbor Ising model in one dimension" << std::endl;
  stream << "            172: Nearest neighbor Ising model in two dimensions" << std::endl;
  stream << "            180: Real multivariate function" << std::endl;
  stream << "            181: Integer multivariate function" << std::endl;
  stream << "            182: Complex multivariate function (squared magnitude of)" << std::endl;
  stream << "            190: Sudoku" << std::endl;
  stream << "            200: Travelling salesman problem" << std::endl;
  stream << "            1000: Plugin" << std::endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << std::endl;
  stream << "          Path of a function file" << std::endl;
  stream << std::endl;
  stream << "Function Modifiers" << std::endl;
  stream << "      --additive-gaussian-noise" << std::endl;
  stream << "          Additive Gaussian noise" << std::endl;
  stream << "      --expression (type string, default to \"x\")" << std::endl;
  stream << "          Expression of the variable x" << std::endl;
  stream << "      --negation" << std::endl;
  stream << "          Negation (hence minimization) of the function" << std::endl;
  stream << "      --noise-stddev (type double, default to 1)" << std::endl;
  stream << "          Noise standard deviation" << std::endl;
  stream << "      --parsed-modifier" << std::endl;
  stream << "          Parsed modifier" << std::endl;
  stream << std::endl;
  stream << "Function Controllers" << std::endl;
  stream << "  -b, --budget (type int, default to 10000)" << std::endl;
  stream << "          Number of allowed function evaluations (<= 0 means indefinite)" << std::endl;
  stream << "      --cache" << std::endl;
  stream << "          Cache function evaluations" << std::endl;
  stream << "      --cache-budget" << std::endl;
  stream << "          Set cache on budget" << std::endl;
  stream << "      --log-improvement" << std::endl;
  stream << "          Log improvement" << std::endl;
  stream << "      --record-evaluation-time" << std::endl;
  stream << "          Record evaluation time" << std::endl;
  stream << "      --stop-on-maximum" << std::endl;
  stream << "          Stop on maximum" << std::endl;
  stream << "      --stop-on-target" << std::endl;
  stream << "          Stop on target" << std::endl;
  stream << "      --target (type double, default to 100)" << std::endl;
  stream << "          Target" << std::endl;
  stream << std::endl;
  stream << "Algorithms" << std::endl;
  stream << "  -A, --algorithm (type int, default to 100)" << std::endl;
  stream << "          Type of algorithm" << std::endl;
  stream << "            0: Complete search" << std::endl;
  stream << "            10: Random search" << std::endl;
  stream << "            20: Random walk" << std::endl;
  stream << "            30: Human, so you can try by yourself" << std::endl;
  stream << "            100: Random local search" << std::endl;
  stream << "            150: Steepest ascent hill climbing" << std::endl;
  stream << "            160: First ascent hill climbing" << std::endl;
  stream << "            200: Simulated annealing" << std::endl;
  stream << "            300: (1+1) evolutionary algorithm (EA)" << std::endl;
  stream << "            310: (mu+lambda) evolutionary algorithm (EA)" << std::endl;
  stream << "            320: (mu,lambda) evolutionary algorithm (EA)" << std::endl;
  stream << "            400: Genetic algorithm (GA)" << std::endl;
  stream << "            450: (1+(lambda,lambda)) genetic algorithm (GA)" << std::endl;
  stream << "            500: Population-based incremental learning (PBIL)" << std::endl;
  stream << "            501: PBIL with negative and positive selection" << std::endl;
  stream << "            600: Univariate marginal distribution algorithm (UMDA)" << std::endl;
  stream << "            700: Compact genetic algorithm (cGA)" << std::endl;
  stream << "            800: Max-min ant system (MMAS)" << std::endl;
  stream << "            901: Herding evolutionary algorithm (HEA)" << std::endl;
  stream << "            1000: Boltzmann machine PBIL" << std::endl;
  stream << "            1100: Mutual information maximizing input clustering (MIMIC)" << std::endl;
  stream << "            1110: Hierarchical Bayesian optimization algorithm (hBOA)" << std::endl;
  stream << "            1200: Linkage tree genetic algorithm (LTGA)" << std::endl;
  stream << "            1300: Parameter-less population pyramid (P3)" << std::endl;
  stream << "      --allow-no-mutation" << std::endl;
  stream << "          Allow no mutation with standard bit mutation" << std::endl;
  stream << "  -m, --mutation-rate (type double, default to 1)" << std::endl;
  stream << "          Mutation rate relative to bv_size" << std::endl;
  stream << "  -i, --num-iterations (type int, default to 0)" << std::endl;
  stream << "          Number of iterations (<= 0 means indefinite)" << std::endl;
  stream << "      --restart" << std::endl;
  stream << "          Restart any algorithm an indefinite number of times" << std::endl;
  stream << std::endl;
  stream  << "Additional Sections" << std::endl;
  stream << "      --help-fp" << std::endl;
  stream << "          Function parser" << std::endl;
  stream << "      --help-rep" << std::endl;
  stream << "          Representations" << std::endl;
  stream << "      --help-pn" << std::endl;
  stream << "          Prior Noise" << std::endl;
  stream << "      --help-map" << std::endl;
  stream << "          Maps" << std::endl;
  stream << "      --help-ls" << std::endl;
  stream << "          Local Search" << std::endl;
  stream << "      --help-sa" << std::endl;
  stream << "          Simulated Annealing" << std::endl;
  stream << "      --help-ea" << std::endl;
  stream << "          Evolutionary Algorithms" << std::endl;
  stream << "      --help-eda" << std::endl;
  stream << "          Estimation of Distribution Algorithms" << std::endl;
  stream << "      --help-hea" << std::endl;
  stream << "          Herding Evolutionary Algorithms" << std::endl;
  stream << "      --help-bm" << std::endl;
  stream << "          Boltzmann Machine PBIL" << std::endl;
}

void HncoOptions::print_help_fp(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Function parser" << std::endl;
  stream << "      --fp-expression (type string, default to \"(1-x)^2+100*(y-x^2)^2\")" << std::endl;
  stream << "          Expression to parse" << std::endl;
  stream << "      --fp-lower-bound (type double, default to -2)" << std::endl;
  stream << "          Lower bound" << std::endl;
  stream << "      --fp-num-bits (type int, default to 8)" << std::endl;
  stream << "          Number of bits in the dyadic representation of a number" << std::endl;
  stream << "      --fp-precision (type double, default to 0.01)" << std::endl;
  stream << "          Precision of the dyadic representation of a number" << std::endl;
  stream << "      --fp-upper-bound (type double, default to 2)" << std::endl;
  stream << "          Upper bound" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_rep(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Representations" << std::endl;
  stream << "      --rep-categorical-representation (type int, default to 0)" << std::endl;
  stream << "          Categorical representation" << std::endl;
  stream << "            0: Integer" << std::endl;
  stream << "            1: Linear" << std::endl;
  stream << "      --rep-num-additional-bits (type int, default to 2)" << std::endl;
  stream << "          Number of additional bits per element for permutation representation" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_pn(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Prior Noise" << std::endl;
  stream << "      --pn-allow-no-mutation" << std::endl;
  stream << "          Allow no mutation with standard bit mutation" << std::endl;
  stream << "      --pn-mutation-rate (type double, default to 1)" << std::endl;
  stream << "          Mutation rate relative to bv_size" << std::endl;
  stream << "      --pn-neighborhood (type int, default to 0)" << std::endl;
  stream << "          Type of neighborhood" << std::endl;
  stream << "            0: Single bit flip" << std::endl;
  stream << "            1: Standard bit mutation" << std::endl;
  stream << "            2: Hamming ball" << std::endl;
  stream << "            3: Hamming sphere" << std::endl;
  stream << "      --pn-radius (type int, default to 2)" << std::endl;
  stream << "          Radius of Hamming ball or sphere" << std::endl;
  stream << "      --prior-noise" << std::endl;
  stream << "          Prior noise" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_map(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Maps" << std::endl;
  stream << "  -M, --map (type int, default to 0)" << std::endl;
  stream << "          Type of map" << std::endl;
  stream << "            0: None" << std::endl;
  stream << "            1: Translation" << std::endl;
  stream << "            2: Permutation" << std::endl;
  stream << "            3: Composition of permutation and translation" << std::endl;
  stream << "            4: Linear" << std::endl;
  stream << "            5: Affine" << std::endl;
  stream << "            6: Affine (transvection sequence)" << std::endl;
  stream << "      --map-display" << std::endl;
  stream << "          Display the map and exit" << std::endl;
  stream << "      --map-input-size (type int, default to 100)" << std::endl;
  stream << "          Input size of linear and affine maps" << std::endl;
  stream << "      --map-path (type string, default to \"map.txt\")" << std::endl;
  stream << "          Path of a map file" << std::endl;
  stream << "      --map-random" << std::endl;
  stream << "          Sample a random map" << std::endl;
  stream << "      --map-surjective" << std::endl;
  stream << "          Ensure that the sampled linear or affine map is surjective" << std::endl;
  stream << "      --map-ts-length (type int, default to 10)" << std::endl;
  stream << "          Transvection sequence length" << std::endl;
  stream << "      --map-ts-sampling-mode (type int, default to 0)" << std::endl;
  stream << "          Transvection sequence sampling mode" << std::endl;
  stream << "            0: Unconstrained" << std::endl;
  stream << "            1: Commuting transvections" << std::endl;
  stream << "            2: Unique source" << std::endl;
  stream << "            3: Unique destination" << std::endl;
  stream << "            4: Disjoint transvections" << std::endl;
  stream << "            5: Non commuting transvections" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_ls(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Local Search" << std::endl;
  stream << "      --incremental-evaluation" << std::endl;
  stream << "          Incremental evaluation" << std::endl;
  stream << "  -N, --neighborhood (type int, default to 0)" << std::endl;
  stream << "          Type of neighborhood" << std::endl;
  stream << "            0: Single bit flip" << std::endl;
  stream << "            1: Standard bit mutation" << std::endl;
  stream << "            2: Hamming ball" << std::endl;
  stream << "            3: Hamming sphere" << std::endl;
  stream << "      --neighborhood-iterator (type int, default to 0)" << std::endl;
  stream << "          Type of neighborhood iterator" << std::endl;
  stream << "            0: Single bit flip iterator" << std::endl;
  stream << "            1: Hamming sphere iterator" << std::endl;
  stream << "      --radius (type int, default to 2)" << std::endl;
  stream << "          Radius of Hamming ball or sphere" << std::endl;
  stream << "      --rls-patience (type int, default to 50)" << std::endl;
  stream << "          Number of consecutive rejected moves before ending the search (<= 0 means infinite)" << std::endl;
  stream << "      --rls-strict" << std::endl;
  stream << "          Strict (>) random local search" << std::endl;
  stream << "      --rw-log-value" << std::endl;
  stream << "          Log bit vector value during random walk" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_sa(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Simulated Annealing" << std::endl;
  stream << "      --sa-beta-ratio (type double, default to 1.2)" << std::endl;
  stream << "          Ratio for beta or inverse temperature" << std::endl;
  stream << "      --sa-initial-acceptance-probability (type double, default to 0.6)" << std::endl;
  stream << "          Initial acceptance probability" << std::endl;
  stream << "      --sa-num-transitions (type int, default to 50)" << std::endl;
  stream << "          Number of accepted transitions before annealing" << std::endl;
  stream << "      --sa-num-trials (type int, default to 100)" << std::endl;
  stream << "          Number of trials to estimate initial inverse temperature" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_ea(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Evolutionary Algorithms" << std::endl;
  stream << "      --ea-lambda (type int, default to 100)" << std::endl;
  stream << "          Offspring population size" << std::endl;
  stream << "      --ea-mu (type int, default to 10)" << std::endl;
  stream << "          Parent population size" << std::endl;
  stream << "      --ga-crossover-bias (type double, default to 0.5)" << std::endl;
  stream << "          Crossover bias" << std::endl;
  stream << "      --ga-crossover-probability (type double, default to 0.5)" << std::endl;
  stream << "          Crossover probability" << std::endl;
  stream << "      --ga-tournament-size (type int, default to 10)" << std::endl;
  stream << "          Tournament size" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_eda(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Estimation of Distribution Algorithms" << std::endl;
  stream << "  -l, --learning-rate (type double, default to 0.001)" << std::endl;
  stream << "          Learning rate" << std::endl;
  stream << "      --mmas-strict" << std::endl;
  stream << "          Strict (>) max-min ant system" << std::endl;
  stream << "  -x, --population-size (type int, default to 10)" << std::endl;
  stream << "          Population size" << std::endl;
  stream << "      --pv-log-entropy" << std::endl;
  stream << "          Log entropy of probability vector" << std::endl;
  stream << "      --pv-log-num-components (type int, default to 5)" << std::endl;
  stream << "          Number of probability vector components to log" << std::endl;
  stream << "      --pv-log-pv" << std::endl;
  stream << "          Log probability vector" << std::endl;
  stream << "  -y, --selection-size (type int, default to 1)" << std::endl;
  stream << "          Selection size (number of selected individuals)" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_hea(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Herding Evolutionary Algorithms" << std::endl;
  stream << "      --hea-bound-moment" << std::endl;
  stream << "          Bound moment after update" << std::endl;
  stream << "      --hea-log-delta" << std::endl;
  stream << "          Log norm 2 of delta (in moment space)" << std::endl;
  stream << "      --hea-log-dtu" << std::endl;
  stream << "          Log distance to uniform" << std::endl;
  stream << "      --hea-log-error" << std::endl;
  stream << "          Log error (moment discrepancy)" << std::endl;
  stream << "      --hea-log-moment-matrix" << std::endl;
  stream << "          Log moment matrix" << std::endl;
  stream << "      --hea-randomize-bit-order" << std::endl;
  stream << "          Randomize bit order" << std::endl;
  stream << "      --hea-reset-period (type int, default to 0)" << std::endl;
  stream << "          Reset period (<= 0 means no reset)" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_bm(std::ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Boltzmann Machine PBIL" << std::endl;
  stream << "      --bm-log-norm-infinite" << std::endl;
  stream << "          Log infinite norm of the parameters" << std::endl;
  stream << "      --bm-log-norm-l1" << std::endl;
  stream << "          Log L1 norm of the parameters" << std::endl;
  stream << "      --bm-mc-reset-strategy (type int, default to 1)" << std::endl;
  stream << "          Markov chain reset strategy" << std::endl;
  stream << "            0: No reset" << std::endl;
  stream << "            1: Reset MC at the beginning of each iteration" << std::endl;
  stream << "            2: Reset MC before sampling each bit vector" << std::endl;
  stream << "      --bm-negative-positive-selection" << std::endl;
  stream << "          Negative and positive selection" << std::endl;
  stream << "      --bm-num-gs-cycles (type int, default to 1)" << std::endl;
  stream << "          Number of Gibbs sampler cycles per bit vector" << std::endl;
  stream << "      --bm-num-gs-steps (type int, default to 100)" << std::endl;
  stream << "          Number of Gibbs sampler steps per bit vector" << std::endl;
  stream << "      --bm-sampling (type int, default to 1)" << std::endl;
  stream << "          Sampling mode for the Boltzmann machine" << std::endl;
  stream << "            0: Asynchronous sampling" << std::endl;
  stream << "            1: Asynchronous sampling with full scan" << std::endl;
  stream << "            2: Synchronous sampling" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_version(std::ostream& stream) const
{
  stream << _version << std::endl;
}

std::ostream& hnco::app::operator<<(std::ostream& stream, const HncoOptions& options)
{
  stream << "# algorithm = " << options._algorithm << std::endl;
  stream << "# bm_mc_reset_strategy = " << options._bm_mc_reset_strategy << std::endl;
  stream << "# bm_num_gs_cycles = " << options._bm_num_gs_cycles << std::endl;
  stream << "# bm_num_gs_steps = " << options._bm_num_gs_steps << std::endl;
  stream << "# bm_sampling = " << options._bm_sampling << std::endl;
  stream << "# budget = " << options._budget << std::endl;
  stream << "# bv_size = " << options._bv_size << std::endl;
  stream << "# description_path = " << options._description_path << std::endl;
  stream << "# ea_lambda = " << options._ea_lambda << std::endl;
  stream << "# ea_mu = " << options._ea_mu << std::endl;
  stream << "# expression = " << options._expression << std::endl;
  stream << "# fn_name = " << options._fn_name << std::endl;
  stream << "# fn_num_traps = " << options._fn_num_traps << std::endl;
  stream << "# fn_prefix_length = " << options._fn_prefix_length << std::endl;
  stream << "# fn_threshold = " << options._fn_threshold << std::endl;
  stream << "# fp_expression = " << options._fp_expression << std::endl;
  stream << "# fp_lower_bound = " << options._fp_lower_bound << std::endl;
  stream << "# fp_num_bits = " << options._fp_num_bits << std::endl;
  stream << "# fp_precision = " << options._fp_precision << std::endl;
  stream << "# fp_upper_bound = " << options._fp_upper_bound << std::endl;
  stream << "# function = " << options._function << std::endl;
  stream << "# ga_crossover_bias = " << options._ga_crossover_bias << std::endl;
  stream << "# ga_crossover_probability = " << options._ga_crossover_probability << std::endl;
  stream << "# ga_tournament_size = " << options._ga_tournament_size << std::endl;
  stream << "# hea_reset_period = " << options._hea_reset_period << std::endl;
  stream << "# learning_rate = " << options._learning_rate << std::endl;
  stream << "# map = " << options._map << std::endl;
  stream << "# map_input_size = " << options._map_input_size << std::endl;
  stream << "# map_path = " << options._map_path << std::endl;
  stream << "# map_ts_length = " << options._map_ts_length << std::endl;
  stream << "# map_ts_sampling_mode = " << options._map_ts_sampling_mode << std::endl;
  stream << "# mutation_rate = " << options._mutation_rate << std::endl;
  stream << "# neighborhood = " << options._neighborhood << std::endl;
  stream << "# neighborhood_iterator = " << options._neighborhood_iterator << std::endl;
  stream << "# noise_stddev = " << options._noise_stddev << std::endl;
  stream << "# num_iterations = " << options._num_iterations << std::endl;
  stream << "# num_threads = " << options._num_threads << std::endl;
  stream << "# path = " << options._path << std::endl;
  stream << "# pn_mutation_rate = " << options._pn_mutation_rate << std::endl;
  stream << "# pn_neighborhood = " << options._pn_neighborhood << std::endl;
  stream << "# pn_radius = " << options._pn_radius << std::endl;
  stream << "# population_size = " << options._population_size << std::endl;
  stream << "# pv_log_num_components = " << options._pv_log_num_components << std::endl;
  stream << "# radius = " << options._radius << std::endl;
  stream << "# rep_categorical_representation = " << options._rep_categorical_representation << std::endl;
  stream << "# rep_num_additional_bits = " << options._rep_num_additional_bits << std::endl;
  stream << "# results_path = " << options._results_path << std::endl;
  stream << "# rls_patience = " << options._rls_patience << std::endl;
  stream << "# sa_beta_ratio = " << options._sa_beta_ratio << std::endl;
  stream << "# sa_initial_acceptance_probability = " << options._sa_initial_acceptance_probability << std::endl;
  stream << "# sa_num_transitions = " << options._sa_num_transitions << std::endl;
  stream << "# sa_num_trials = " << options._sa_num_trials << std::endl;
  stream << "# seed = " << options._seed << std::endl;
  stream << "# selection_size = " << options._selection_size << std::endl;
  stream << "# solution_path = " << options._solution_path << std::endl;
  stream << "# target = " << options._target << std::endl;
  if (options._additive_gaussian_noise)
    stream << "# additive_gaussian_noise" << std::endl;
  if (options._allow_no_mutation)
    stream << "# allow_no_mutation" << std::endl;
  if (options._bm_log_norm_infinite)
    stream << "# bm_log_norm_infinite" << std::endl;
  if (options._bm_log_norm_l1)
    stream << "# bm_log_norm_l1" << std::endl;
  if (options._bm_negative_positive_selection)
    stream << "# bm_negative_positive_selection" << std::endl;
  if (options._cache)
    stream << "# cache" << std::endl;
  if (options._cache_budget)
    stream << "# cache_budget" << std::endl;
  if (options._concrete_solution)
    stream << "# concrete_solution" << std::endl;
  if (options._fn_display)
    stream << "# fn_display" << std::endl;
  if (options._fn_get_bv_size)
    stream << "# fn_get_bv_size" << std::endl;
  if (options._fn_get_maximum)
    stream << "# fn_get_maximum" << std::endl;
  if (options._fn_has_known_maximum)
    stream << "# fn_has_known_maximum" << std::endl;
  if (options._fn_provides_incremental_evaluation)
    stream << "# fn_provides_incremental_evaluation" << std::endl;
  if (options._fn_walsh_transform)
    stream << "# fn_walsh_transform" << std::endl;
  if (options._hea_bound_moment)
    stream << "# hea_bound_moment" << std::endl;
  if (options._hea_log_delta)
    stream << "# hea_log_delta" << std::endl;
  if (options._hea_log_dtu)
    stream << "# hea_log_dtu" << std::endl;
  if (options._hea_log_error)
    stream << "# hea_log_error" << std::endl;
  if (options._hea_log_moment_matrix)
    stream << "# hea_log_moment_matrix" << std::endl;
  if (options._hea_randomize_bit_order)
    stream << "# hea_randomize_bit_order" << std::endl;
  if (options._incremental_evaluation)
    stream << "# incremental_evaluation" << std::endl;
  if (options._load_solution)
    stream << "# load_solution" << std::endl;
  if (options._log_improvement)
    stream << "# log_improvement" << std::endl;
  if (options._map_display)
    stream << "# map_display" << std::endl;
  if (options._map_random)
    stream << "# map_random" << std::endl;
  if (options._map_surjective)
    stream << "# map_surjective" << std::endl;
  if (options._mmas_strict)
    stream << "# mmas_strict" << std::endl;
  if (options._negation)
    stream << "# negation" << std::endl;
  if (options._parsed_modifier)
    stream << "# parsed_modifier" << std::endl;
  if (options._pn_allow_no_mutation)
    stream << "# pn_allow_no_mutation" << std::endl;
  if (options._print_defaults)
    stream << "# print_defaults" << std::endl;
  if (options._print_description)
    stream << "# print_description" << std::endl;
  if (options._print_header)
    stream << "# print_header" << std::endl;
  if (options._print_results)
    stream << "# print_results" << std::endl;
  if (options._print_solution)
    stream << "# print_solution" << std::endl;
  if (options._prior_noise)
    stream << "# prior_noise" << std::endl;
  if (options._pv_log_entropy)
    stream << "# pv_log_entropy" << std::endl;
  if (options._pv_log_pv)
    stream << "# pv_log_pv" << std::endl;
  if (options._record_evaluation_time)
    stream << "# record_evaluation_time" << std::endl;
  if (options._restart)
    stream << "# restart" << std::endl;
  if (options._rls_strict)
    stream << "# rls_strict" << std::endl;
  if (options._rw_log_value)
    stream << "# rw_log_value" << std::endl;
  if (options._save_description)
    stream << "# save_description" << std::endl;
  if (options._save_results)
    stream << "# save_results" << std::endl;
  if (options._save_solution)
    stream << "# save_solution" << std::endl;
  if (options._stop_on_maximum)
    stream << "# stop_on_maximum" << std::endl;
  if (options._stop_on_target)
    stream << "# stop_on_target" << std::endl;
  stream << "# last_parameter" << std::endl;
  stream << "# exec_name = " << options._exec_name << std::endl;
  stream << "# version = " << options._version << std::endl;
  stream << "# Generated from hnco.json" << std::endl;
  return stream;
}
