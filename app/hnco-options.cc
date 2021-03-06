#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "hnco-options.hh"

using namespace std;

Options::Options(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.16"),
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
  _hea_bit_herding(0),
  _opt_hea_bit_herding(false),
  _hea_num_seq_updates(100),
  _opt_hea_num_seq_updates(false),
  _hea_reset_period(0),
  _opt_hea_reset_period(false),
  _hea_sampling_method(0),
  _opt_hea_sampling_method(false),
  _hea_weight(1),
  _opt_hea_weight(false),
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
  _hea_log_selection(false),
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
    OPTION_FP_UPPER_BOUND,
    OPTION_FUNCTION,
    OPTION_GA_CROSSOVER_BIAS,
    OPTION_GA_CROSSOVER_PROBABILITY,
    OPTION_GA_TOURNAMENT_SIZE,
    OPTION_HEA_BIT_HERDING,
    OPTION_HEA_NUM_SEQ_UPDATES,
    OPTION_HEA_RESET_PERIOD,
    OPTION_HEA_SAMPLING_METHOD,
    OPTION_HEA_WEIGHT,
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
    OPTION_HEA_LOG_SELECTION,
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
    {"fp-upper-bound", required_argument, 0, OPTION_FP_UPPER_BOUND},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"ga-crossover-bias", required_argument, 0, OPTION_GA_CROSSOVER_BIAS},
    {"ga-crossover-probability", required_argument, 0, OPTION_GA_CROSSOVER_PROBABILITY},
    {"ga-tournament-size", required_argument, 0, OPTION_GA_TOURNAMENT_SIZE},
    {"hea-bit-herding", required_argument, 0, OPTION_HEA_BIT_HERDING},
    {"hea-num-seq-updates", required_argument, 0, OPTION_HEA_NUM_SEQ_UPDATES},
    {"hea-reset-period", required_argument, 0, OPTION_HEA_RESET_PERIOD},
    {"hea-sampling-method", required_argument, 0, OPTION_HEA_SAMPLING_METHOD},
    {"hea-weight", required_argument, 0, OPTION_HEA_WEIGHT},
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
    {"hea-log-selection", no_argument, 0, OPTION_HEA_LOG_SELECTION},
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
      set_description_path(string(optarg));
      break;

    case OPTION_EA_LAMBDA:
      set_ea_lambda(atoi(optarg));
      break;

    case OPTION_EA_MU:
      set_ea_mu(atoi(optarg));
      break;

    case OPTION_EXPRESSION:
      set_expression(string(optarg));
      break;

    case OPTION_FN_NAME:
      set_fn_name(string(optarg));
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
      set_fp_expression(string(optarg));
      break;

    case OPTION_FP_LOWER_BOUND:
      set_fp_lower_bound(atof(optarg));
      break;

    case OPTION_FP_NUM_BITS:
      set_fp_num_bits(atoi(optarg));
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

    case OPTION_HEA_BIT_HERDING:
      set_hea_bit_herding(atoi(optarg));
      break;

    case OPTION_HEA_NUM_SEQ_UPDATES:
      set_hea_num_seq_updates(atoi(optarg));
      break;

    case OPTION_HEA_RESET_PERIOD:
      set_hea_reset_period(atoi(optarg));
      break;

    case OPTION_HEA_SAMPLING_METHOD:
      set_hea_sampling_method(atoi(optarg));
      break;

    case OPTION_HEA_WEIGHT:
      set_hea_weight(atof(optarg));
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
      set_map_path(string(optarg));
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
      set_path(string(optarg));
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

    case OPTION_RESULTS_PATH:
      set_results_path(string(optarg));
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
      set_solution_path(string(optarg));
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

    case OPTION_HEA_LOG_SELECTION:
      _hea_log_selection = true;
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
      print_help(cerr);
      exit(0);

    case OPTION_HELP_FP:
      print_help_fp(cerr);
      exit(0);

    case OPTION_HELP_REP:
      print_help_rep(cerr);
      exit(0);

    case OPTION_HELP_PN:
      print_help_pn(cerr);
      exit(0);

    case OPTION_HELP_MAP:
      print_help_map(cerr);
      exit(0);

    case OPTION_HELP_LS:
      print_help_ls(cerr);
      exit(0);

    case OPTION_HELP_SA:
      print_help_sa(cerr);
      exit(0);

    case OPTION_HELP_EA:
      print_help_ea(cerr);
      exit(0);

    case OPTION_HELP_EDA:
      print_help_eda(cerr);
      exit(0);

    case OPTION_HELP_HEA:
      print_help_hea(cerr);
      exit(0);

    case OPTION_HELP_BM:
      print_help_bm(cerr);
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
  stream << "General" << endl;
  stream << "      --concrete-solution" << endl;
  stream << "          At the end, print or save the solution in the domain of the concrete function" << endl;
  stream << "      --description-path (type string, default to \"description.txt\")" << endl;
  stream << "          Path of the description file" << endl;
  stream << "      --load-solution" << endl;
  stream << "          Load a solution from a file" << endl;
  stream << "      --num-threads (type int, default to 1)" << endl;
  stream << "          Number of threads" << endl;
  stream << "      --print-defaults" << endl;
  stream << "          Print the default parameters and exit" << endl;
  stream << "      --print-description" << endl;
  stream << "          Print a description of the solution" << endl;
  stream << "      --print-header" << endl;
  stream << "          At the beginning, print the header" << endl;
  stream << "      --print-results" << endl;
  stream << "          Print results" << endl;
  stream << "      --print-solution" << endl;
  stream << "          Print the solution" << endl;
  stream << "      --results-path (type string, default to \"results.json\")" << endl;
  stream << "          Path of the results file" << endl;
  stream << "      --save-description" << endl;
  stream << "          At the end, save a description of the solution in a file" << endl;
  stream << "      --save-results" << endl;
  stream << "          At the end, save results in a file" << endl;
  stream << "      --save-solution" << endl;
  stream << "          At the end, save the solution in a file" << endl;
  stream << "      --seed (type unsigned, default to 0)" << endl;
  stream << "          Seed for the random number generator" << endl;
  stream << "      --solution-path (type string, default to \"solution.txt\")" << endl;
  stream << "          Path of the solution file" << endl;
  stream << endl;
  stream << "Functions" << endl;
  stream << "  -s, --bv-size (type int, default to 100)" << endl;
  stream << "          Size of bit vectors" << endl;
  stream << "      --fn-display" << endl;
  stream << "          Display the function and exit" << endl;
  stream << "      --fn-get-bv-size" << endl;
  stream << "          Print the size of bit vectors" << endl;
  stream << "      --fn-get-maximum" << endl;
  stream << "          If the maximum is known then print it and exit with status 0 else exit with status 1" << endl;
  stream << "      --fn-has-known-maximum" << endl;
  stream << "          Does the function have a known maximum?" << endl;
  stream << "      --fn-name (type string, default to \"noname\")" << endl;
  stream << "          Name of the function in the dynamic library" << endl;
  stream << "      --fn-num-traps (type int, default to 10)" << endl;
  stream << "          Number of traps" << endl;
  stream << "      --fn-prefix-length (type int, default to 2)" << endl;
  stream << "          Prefix length for long path" << endl;
  stream << "      --fn-provides-incremental-evaluation" << endl;
  stream << "          Does the function provide incremental evaluation?" << endl;
  stream << "  -t, --fn-threshold (type int, default to 10)" << endl;
  stream << "          Threshold (in bits) for Jump, Four Peaks, and Six Peaks" << endl;
  stream << "      --fn-walsh-transform" << endl;
  stream << "          Compute the Walsh transform of the function" << endl;
  stream << "  -F, --function (type int, default to 0)" << endl;
  stream << "          Type of function" << endl;
  stream << "            0: OneMax" << endl;
  stream << "            1: Linear function" << endl;
  stream << "            5: BinVal (linear function with weights in geometric progression 1, 2, 4...)" << endl;
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
  stream << "            71: Max not-all-equal 3SAT" << endl;
  stream << "            80: Low autocorrelation binary sequence" << endl;
  stream << "            81: Low autocorrelation binary sequence merit factor" << endl;
  stream << "            90: Equal products" << endl;
  stream << "            91: Partition" << endl;
  stream << "            100: Summation cancellation" << endl;
  stream << "            101: Summation cancellation with sinus" << endl;
  stream << "            110: Trap" << endl;
  stream << "            120: Hierarchical if and only if (Hiff)" << endl;
  stream << "            130: Plateau" << endl;
  stream << "            140: Long path" << endl;
  stream << "            150: Factorization" << endl;
  stream << "            160: Walsh expansion" << endl;
  stream << "            161: Walsh expansion of degree 1" << endl;
  stream << "            162: Walsh expansion of degree 2" << endl;
  stream << "            171: Nearest neighbor Ising model in one dimension" << endl;
  stream << "            172: Nearest neighbor Ising model in two dimensions" << endl;
  stream << "            180: Real multivariate function" << endl;
  stream << "            181: Integer multivariate function" << endl;
  stream << "            182: Complex multivariate function (squared magnitude of)" << endl;
  stream << "            190: Sudoku" << endl;
  stream << "            1000: Plugin" << endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << endl;
  stream << "          Path of a function file" << endl;
  stream << endl;
  stream << "Function Modifiers" << endl;
  stream << "      --additive-gaussian-noise" << endl;
  stream << "          Additive Gaussian noise" << endl;
  stream << "      --expression (type string, default to \"x\")" << endl;
  stream << "          Expression of the variable x" << endl;
  stream << "      --negation" << endl;
  stream << "          Negation (hence minimization) of the function" << endl;
  stream << "      --noise-stddev (type double, default to 1)" << endl;
  stream << "          Noise standard deviation" << endl;
  stream << "      --parsed-modifier" << endl;
  stream << "          Parsed modifier" << endl;
  stream << endl;
  stream << "Function Controllers" << endl;
  stream << "  -b, --budget (type int, default to 10000)" << endl;
  stream << "          Number of allowed function evaluations (<= 0 means indefinite)" << endl;
  stream << "      --cache" << endl;
  stream << "          Cache function evaluations" << endl;
  stream << "      --cache-budget" << endl;
  stream << "          Set cache on budget" << endl;
  stream << "      --log-improvement" << endl;
  stream << "          Log improvement" << endl;
  stream << "      --record-evaluation-time" << endl;
  stream << "          Record evaluation time" << endl;
  stream << "      --stop-on-maximum" << endl;
  stream << "          Stop on maximum" << endl;
  stream << "      --stop-on-target" << endl;
  stream << "          Stop on target" << endl;
  stream << "      --target (type double, default to 100)" << endl;
  stream << "          Target" << endl;
  stream << endl;
  stream << "Algorithms" << endl;
  stream << "  -A, --algorithm (type int, default to 100)" << endl;
  stream << "          Type of algorithm" << endl;
  stream << "            0: Complete search" << endl;
  stream << "            10: Random search" << endl;
  stream << "            20: Random walk" << endl;
  stream << "            30: Human, so you can try by yourself" << endl;
  stream << "            100: Random local search" << endl;
  stream << "            150: Steepest ascent hill climbing" << endl;
  stream << "            160: First ascent hill climbing" << endl;
  stream << "            200: Simulated annealing" << endl;
  stream << "            300: (1+1) evolutionary algorithm (EA)" << endl;
  stream << "            310: (mu+lambda) evolutionary algorithm (EA)" << endl;
  stream << "            320: (mu,lambda) evolutionary algorithm (EA)" << endl;
  stream << "            400: Genetic algorithm (GA)" << endl;
  stream << "            450: (1+(lambda,lambda)) genetic algorithm (GA)" << endl;
  stream << "            500: Population-based incremental learning (PBIL)" << endl;
  stream << "            501: PBIL with negative and positive selection" << endl;
  stream << "            600: Univariate marginal distribution algorithm (UMDA)" << endl;
  stream << "            700: Compact genetic algorithm (cGA)" << endl;
  stream << "            800: Max-min ant system (MMAS)" << endl;
  stream << "            900: Herding evolutionary algorithm, herding with bit features (HEA)" << endl;
  stream << "            901: Herding evolutionary algorithm, herding with spin features (HEA)" << endl;
  stream << "            1000: Boltzmann machine PBIL" << endl;
  stream << "            1100: Mutual information maximizing input clustering (MIMIC)" << endl;
  stream << "            1110: Hierarchical Bayesian optimization algorithm (hBOA)" << endl;
  stream << "            1200: Linkage tree genetic algorithm (LTGA)" << endl;
  stream << "            1300: Parameter-less population pyramid (P3)" << endl;
  stream << "      --allow-no-mutation" << endl;
  stream << "          Allow no mutation with standard bit mutation" << endl;
  stream << "  -m, --mutation-rate (type double, default to 1)" << endl;
  stream << "          Mutation rate relative to bv_size" << endl;
  stream << "  -i, --num-iterations (type int, default to 0)" << endl;
  stream << "          Number of iterations (<= 0 means indefinite)" << endl;
  stream << "      --restart" << endl;
  stream << "          Restart any algorithm an indefinite number of times" << endl;
  stream << endl;
  stream  << "Additional Sections" << endl;
  stream << "      --help-fp" << endl;
  stream << "          Function parser" << endl;
  stream << "      --help-rep" << endl;
  stream << "          Representations" << endl;
  stream << "      --help-pn" << endl;
  stream << "          Prior Noise" << endl;
  stream << "      --help-map" << endl;
  stream << "          Maps" << endl;
  stream << "      --help-ls" << endl;
  stream << "          Local Search" << endl;
  stream << "      --help-sa" << endl;
  stream << "          Simulated Annealing" << endl;
  stream << "      --help-ea" << endl;
  stream << "          Evolutionary Algorithms" << endl;
  stream << "      --help-eda" << endl;
  stream << "          Estimation of Distribution Algorithms" << endl;
  stream << "      --help-hea" << endl;
  stream << "          Herding Evolutionary Algorithms" << endl;
  stream << "      --help-bm" << endl;
  stream << "          Boltzmann Machine PBIL" << endl;
}

void Options::print_help_fp(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Function parser" << endl;
  stream << "      --fp-expression (type string, default to \"(1-x)^2+100*(y-x^2)^2\")" << endl;
  stream << "          Expression to parse" << endl;
  stream << "      --fp-lower-bound (type double, default to -2)" << endl;
  stream << "          Lower bound" << endl;
  stream << "      --fp-num-bits (type int, default to 8)" << endl;
  stream << "          Number of bits in the dyadic representation of a number" << endl;
  stream << "      --fp-upper-bound (type double, default to 2)" << endl;
  stream << "          Upper bound" << endl;
  stream << endl;
}

void Options::print_help_rep(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Representations" << endl;
  stream << "      --rep-categorical-representation (type int, default to 0)" << endl;
  stream << "          Categorical representation" << endl;
  stream << "            0: Integer" << endl;
  stream << "            1: Linear" << endl;
  stream << endl;
}

void Options::print_help_pn(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Prior Noise" << endl;
  stream << "      --pn-allow-no-mutation" << endl;
  stream << "          Allow no mutation with standard bit mutation" << endl;
  stream << "      --pn-mutation-rate (type double, default to 1)" << endl;
  stream << "          Mutation rate relative to bv_size" << endl;
  stream << "      --pn-neighborhood (type int, default to 0)" << endl;
  stream << "          Type of neighborhood" << endl;
  stream << "            0: Single bit flip" << endl;
  stream << "            1: Standard bit mutation" << endl;
  stream << "            2: Hamming ball" << endl;
  stream << "            3: Hamming sphere" << endl;
  stream << "      --pn-radius (type int, default to 2)" << endl;
  stream << "          Radius of Hamming ball or sphere" << endl;
  stream << "      --prior-noise" << endl;
  stream << "          Prior noise" << endl;
  stream << endl;
}

void Options::print_help_map(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Maps" << endl;
  stream << "  -M, --map (type int, default to 0)" << endl;
  stream << "          Type of map" << endl;
  stream << "            0: None" << endl;
  stream << "            1: Translation" << endl;
  stream << "            2: Permutation" << endl;
  stream << "            3: Composition of permutation and translation" << endl;
  stream << "            4: Linear" << endl;
  stream << "            5: Affine" << endl;
  stream << "            6: Affine (transvection sequence)" << endl;
  stream << "      --map-display" << endl;
  stream << "          Display the map and exit" << endl;
  stream << "      --map-input-size (type int, default to 100)" << endl;
  stream << "          Input size of linear and affine maps" << endl;
  stream << "      --map-path (type string, default to \"map.txt\")" << endl;
  stream << "          Path of a map file" << endl;
  stream << "      --map-random" << endl;
  stream << "          Sample a random map" << endl;
  stream << "      --map-surjective" << endl;
  stream << "          Ensure that the sampled linear or affine map is surjective" << endl;
  stream << "      --map-ts-length (type int, default to 10)" << endl;
  stream << "          Transvection sequence length" << endl;
  stream << "      --map-ts-sampling-mode (type int, default to 0)" << endl;
  stream << "          Transvection sequence sampling mode" << endl;
  stream << "            0: Unconstrained" << endl;
  stream << "            1: Commuting transvections" << endl;
  stream << "            2: Unique source" << endl;
  stream << "            3: Unique destination" << endl;
  stream << "            4: Disjoint transvections" << endl;
  stream << "            5: Non commuting transvections" << endl;
  stream << endl;
}

void Options::print_help_ls(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Local Search" << endl;
  stream << "      --incremental-evaluation" << endl;
  stream << "          Incremental evaluation" << endl;
  stream << "  -N, --neighborhood (type int, default to 0)" << endl;
  stream << "          Type of neighborhood" << endl;
  stream << "            0: Single bit flip" << endl;
  stream << "            1: Standard bit mutation" << endl;
  stream << "            2: Hamming ball" << endl;
  stream << "            3: Hamming sphere" << endl;
  stream << "      --neighborhood-iterator (type int, default to 0)" << endl;
  stream << "          Type of neighborhood iterator" << endl;
  stream << "            0: Single bit flip iterator" << endl;
  stream << "            1: Hamming sphere iterator" << endl;
  stream << "      --radius (type int, default to 2)" << endl;
  stream << "          Radius of Hamming ball or sphere" << endl;
  stream << "      --rls-patience (type int, default to 50)" << endl;
  stream << "          Number of consecutive rejected moves before throwing LocalMaximumReached (<= 0 means infinite)" << endl;
  stream << "      --rls-strict" << endl;
  stream << "          Strict (>) random local search" << endl;
  stream << "      --rw-log-value" << endl;
  stream << "          Log bit vector value during random walk" << endl;
  stream << endl;
}

void Options::print_help_sa(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Simulated Annealing" << endl;
  stream << "      --sa-beta-ratio (type double, default to 1.2)" << endl;
  stream << "          Ratio for beta or inverse temperature" << endl;
  stream << "      --sa-initial-acceptance-probability (type double, default to 0.6)" << endl;
  stream << "          Initial acceptance probability" << endl;
  stream << "      --sa-num-transitions (type int, default to 50)" << endl;
  stream << "          Number of accepted transitions before annealing" << endl;
  stream << "      --sa-num-trials (type int, default to 100)" << endl;
  stream << "          Number of trials to estimate initial inverse temperature" << endl;
  stream << endl;
}

void Options::print_help_ea(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Evolutionary Algorithms" << endl;
  stream << "      --ea-lambda (type int, default to 100)" << endl;
  stream << "          Offspring population size" << endl;
  stream << "      --ea-mu (type int, default to 10)" << endl;
  stream << "          Parent population size" << endl;
  stream << "      --ga-crossover-bias (type double, default to 0.5)" << endl;
  stream << "          Crossover bias" << endl;
  stream << "      --ga-crossover-probability (type double, default to 0.5)" << endl;
  stream << "          Crossover probability" << endl;
  stream << "      --ga-tournament-size (type int, default to 10)" << endl;
  stream << "          Tournament size" << endl;
  stream << endl;
}

void Options::print_help_eda(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Estimation of Distribution Algorithms" << endl;
  stream << "  -l, --learning-rate (type double, default to 0.001)" << endl;
  stream << "          Learning rate" << endl;
  stream << "      --mmas-strict" << endl;
  stream << "          Strict (>) max-min ant system" << endl;
  stream << "  -x, --population-size (type int, default to 10)" << endl;
  stream << "          Population size" << endl;
  stream << "      --pv-log-entropy" << endl;
  stream << "          Log entropy of probability vector" << endl;
  stream << "      --pv-log-num-components (type int, default to 5)" << endl;
  stream << "          Number of probability vector components to log" << endl;
  stream << "      --pv-log-pv" << endl;
  stream << "          Log probability vector" << endl;
  stream << "  -y, --selection-size (type int, default to 1)" << endl;
  stream << "          Selection size (number of selected individuals)" << endl;
  stream << endl;
}

void Options::print_help_hea(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Herding Evolutionary Algorithms" << endl;
  stream << "      --hea-bit-herding (type int, default to 0)" << endl;
  stream << "          Type of bit herding" << endl;
  stream << "            0: Minimize norm" << endl;
  stream << "            1: Maximize inner product" << endl;
  stream << "      --hea-bound-moment" << endl;
  stream << "          Bound moment after update" << endl;
  stream << "      --hea-log-delta" << endl;
  stream << "          Log norm 2 of delta (in moment space)" << endl;
  stream << "      --hea-log-dtu" << endl;
  stream << "          Log distance to uniform" << endl;
  stream << "      --hea-log-error" << endl;
  stream << "          Log error (moment discrepancy)" << endl;
  stream << "      --hea-log-moment-matrix" << endl;
  stream << "          Log moment matrix" << endl;
  stream << "      --hea-log-selection" << endl;
  stream << "          Log the distance between the target and the selection moment" << endl;
  stream << "      --hea-num-seq-updates (type int, default to 100)" << endl;
  stream << "          Number of sequential updates per sample" << endl;
  stream << "      --hea-randomize-bit-order" << endl;
  stream << "          Randomize bit order" << endl;
  stream << "      --hea-reset-period (type int, default to 0)" << endl;
  stream << "          Reset period (<= 0 means no reset)" << endl;
  stream << "      --hea-sampling-method (type int, default to 0)" << endl;
  stream << "          Sampling method for spin features" << endl;
  stream << "            0: Greedy algorithm" << endl;
  stream << "            1: Random local search" << endl;
  stream << "            2: Deterministic local search" << endl;
  stream << "      --hea-weight (type double, default to 1)" << endl;
  stream << "          Weight of second moments" << endl;
  stream << endl;
}

void Options::print_help_bm(ostream& stream) const
{
  stream << "HNCO (in Hypercubo Nigrae Capsulae Optimum) -- optimization of black box functions defined on bit vectors" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "Boltzmann Machine PBIL" << endl;
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
  stream << "# description_path = " << options._description_path << endl;
  stream << "# ea_lambda = " << options._ea_lambda << endl;
  stream << "# ea_mu = " << options._ea_mu << endl;
  stream << "# expression = " << options._expression << endl;
  stream << "# fn_name = " << options._fn_name << endl;
  stream << "# fn_num_traps = " << options._fn_num_traps << endl;
  stream << "# fn_prefix_length = " << options._fn_prefix_length << endl;
  stream << "# fn_threshold = " << options._fn_threshold << endl;
  stream << "# fp_expression = " << options._fp_expression << endl;
  stream << "# fp_lower_bound = " << options._fp_lower_bound << endl;
  stream << "# fp_num_bits = " << options._fp_num_bits << endl;
  stream << "# fp_upper_bound = " << options._fp_upper_bound << endl;
  stream << "# function = " << options._function << endl;
  stream << "# ga_crossover_bias = " << options._ga_crossover_bias << endl;
  stream << "# ga_crossover_probability = " << options._ga_crossover_probability << endl;
  stream << "# ga_tournament_size = " << options._ga_tournament_size << endl;
  stream << "# hea_bit_herding = " << options._hea_bit_herding << endl;
  stream << "# hea_num_seq_updates = " << options._hea_num_seq_updates << endl;
  stream << "# hea_reset_period = " << options._hea_reset_period << endl;
  stream << "# hea_sampling_method = " << options._hea_sampling_method << endl;
  stream << "# hea_weight = " << options._hea_weight << endl;
  stream << "# learning_rate = " << options._learning_rate << endl;
  stream << "# map = " << options._map << endl;
  stream << "# map_input_size = " << options._map_input_size << endl;
  stream << "# map_path = " << options._map_path << endl;
  stream << "# map_ts_length = " << options._map_ts_length << endl;
  stream << "# map_ts_sampling_mode = " << options._map_ts_sampling_mode << endl;
  stream << "# mutation_rate = " << options._mutation_rate << endl;
  stream << "# neighborhood = " << options._neighborhood << endl;
  stream << "# neighborhood_iterator = " << options._neighborhood_iterator << endl;
  stream << "# noise_stddev = " << options._noise_stddev << endl;
  stream << "# num_iterations = " << options._num_iterations << endl;
  stream << "# num_threads = " << options._num_threads << endl;
  stream << "# path = " << options._path << endl;
  stream << "# pn_mutation_rate = " << options._pn_mutation_rate << endl;
  stream << "# pn_neighborhood = " << options._pn_neighborhood << endl;
  stream << "# pn_radius = " << options._pn_radius << endl;
  stream << "# population_size = " << options._population_size << endl;
  stream << "# pv_log_num_components = " << options._pv_log_num_components << endl;
  stream << "# radius = " << options._radius << endl;
  stream << "# rep_categorical_representation = " << options._rep_categorical_representation << endl;
  stream << "# results_path = " << options._results_path << endl;
  stream << "# rls_patience = " << options._rls_patience << endl;
  stream << "# sa_beta_ratio = " << options._sa_beta_ratio << endl;
  stream << "# sa_initial_acceptance_probability = " << options._sa_initial_acceptance_probability << endl;
  stream << "# sa_num_transitions = " << options._sa_num_transitions << endl;
  stream << "# sa_num_trials = " << options._sa_num_trials << endl;
  stream << "# seed = " << options._seed << endl;
  stream << "# selection_size = " << options._selection_size << endl;
  stream << "# solution_path = " << options._solution_path << endl;
  stream << "# target = " << options._target << endl;
  if (options._additive_gaussian_noise)
    stream << "# additive_gaussian_noise" << endl;
  if (options._allow_no_mutation)
    stream << "# allow_no_mutation" << endl;
  if (options._bm_log_norm_infinite)
    stream << "# bm_log_norm_infinite" << endl;
  if (options._bm_log_norm_l1)
    stream << "# bm_log_norm_l1" << endl;
  if (options._bm_negative_positive_selection)
    stream << "# bm_negative_positive_selection" << endl;
  if (options._cache)
    stream << "# cache" << endl;
  if (options._cache_budget)
    stream << "# cache_budget" << endl;
  if (options._concrete_solution)
    stream << "# concrete_solution" << endl;
  if (options._fn_display)
    stream << "# fn_display" << endl;
  if (options._fn_get_bv_size)
    stream << "# fn_get_bv_size" << endl;
  if (options._fn_get_maximum)
    stream << "# fn_get_maximum" << endl;
  if (options._fn_has_known_maximum)
    stream << "# fn_has_known_maximum" << endl;
  if (options._fn_provides_incremental_evaluation)
    stream << "# fn_provides_incremental_evaluation" << endl;
  if (options._fn_walsh_transform)
    stream << "# fn_walsh_transform" << endl;
  if (options._hea_bound_moment)
    stream << "# hea_bound_moment" << endl;
  if (options._hea_log_delta)
    stream << "# hea_log_delta" << endl;
  if (options._hea_log_dtu)
    stream << "# hea_log_dtu" << endl;
  if (options._hea_log_error)
    stream << "# hea_log_error" << endl;
  if (options._hea_log_moment_matrix)
    stream << "# hea_log_moment_matrix" << endl;
  if (options._hea_log_selection)
    stream << "# hea_log_selection" << endl;
  if (options._hea_randomize_bit_order)
    stream << "# hea_randomize_bit_order" << endl;
  if (options._incremental_evaluation)
    stream << "# incremental_evaluation" << endl;
  if (options._load_solution)
    stream << "# load_solution" << endl;
  if (options._log_improvement)
    stream << "# log_improvement" << endl;
  if (options._map_display)
    stream << "# map_display" << endl;
  if (options._map_random)
    stream << "# map_random" << endl;
  if (options._map_surjective)
    stream << "# map_surjective" << endl;
  if (options._mmas_strict)
    stream << "# mmas_strict" << endl;
  if (options._negation)
    stream << "# negation" << endl;
  if (options._parsed_modifier)
    stream << "# parsed_modifier" << endl;
  if (options._pn_allow_no_mutation)
    stream << "# pn_allow_no_mutation" << endl;
  if (options._print_defaults)
    stream << "# print_defaults" << endl;
  if (options._print_description)
    stream << "# print_description" << endl;
  if (options._print_header)
    stream << "# print_header" << endl;
  if (options._print_results)
    stream << "# print_results" << endl;
  if (options._print_solution)
    stream << "# print_solution" << endl;
  if (options._prior_noise)
    stream << "# prior_noise" << endl;
  if (options._pv_log_entropy)
    stream << "# pv_log_entropy" << endl;
  if (options._pv_log_pv)
    stream << "# pv_log_pv" << endl;
  if (options._record_evaluation_time)
    stream << "# record_evaluation_time" << endl;
  if (options._restart)
    stream << "# restart" << endl;
  if (options._rls_strict)
    stream << "# rls_strict" << endl;
  if (options._rw_log_value)
    stream << "# rw_log_value" << endl;
  if (options._save_description)
    stream << "# save_description" << endl;
  if (options._save_results)
    stream << "# save_results" << endl;
  if (options._save_solution)
    stream << "# save_solution" << endl;
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
