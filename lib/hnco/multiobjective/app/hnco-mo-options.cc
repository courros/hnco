#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "hnco-mo-options.hh"

using namespace hnco::multiobjective::app;

HncoOptions::HncoOptions(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.20"),
  _algorithm(1400),
  _opt_algorithm(false),
  _bv_size(100),
  _opt_bv_size(false),
  _description_path("description.txt"),
  _opt_description_path(false),
  _ea_crossover_probability(0.8),
  _opt_ea_crossover_probability(false),
  _ea_mu(100),
  _opt_ea_mu(false),
  _ea_mutation_rate(1),
  _opt_ea_mutation_rate(false),
  _ea_tournament_size(2),
  _opt_ea_tournament_size(false),
  _fn_name("noname"),
  _opt_fn_name(false),
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
  _num_iterations(100),
  _opt_num_iterations(false),
  _num_threads(1),
  _opt_num_threads(false),
  _path("function.txt"),
  _opt_path(false),
  _rep_categorical_representation(0),
  _opt_rep_categorical_representation(false),
  _rep_num_additional_bits(2),
  _opt_rep_num_additional_bits(false),
  _results_path("results.json"),
  _opt_results_path(false),
  _seed(0),
  _opt_seed(false),
  _solution_path("solution.txt"),
  _opt_solution_path(false),
  _ea_allow_no_mutation(false),
  _fn_display(false),
  _fn_get_bv_size(false),
  _fn_get_output_size(false),
  _print_defaults(false),
  _print_description(false),
  _print_header(false)
{
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
    OPTION_FP_EXPRESSION,
    OPTION_FP_LOWER_BOUND,
    OPTION_FP_NUM_BITS,
    OPTION_FP_PRECISION,
    OPTION_FP_UPPER_BOUND,
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
    OPTION_PRINT_DEFAULTS,
    OPTION_PRINT_DESCRIPTION,
    OPTION_PRINT_HEADER
  };
  const struct option long_options[] = {
    {"algorithm", required_argument, 0, OPTION_ALGORITHM},
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"description-path", required_argument, 0, OPTION_DESCRIPTION_PATH},
    {"ea-crossover-probability", required_argument, 0, OPTION_EA_CROSSOVER_PROBABILITY},
    {"ea-mu", required_argument, 0, OPTION_EA_MU},
    {"ea-mutation-rate", required_argument, 0, OPTION_EA_MUTATION_RATE},
    {"ea-tournament-size", required_argument, 0, OPTION_EA_TOURNAMENT_SIZE},
    {"fn-name", required_argument, 0, OPTION_FN_NAME},
    {"fp-expression", required_argument, 0, OPTION_FP_EXPRESSION},
    {"fp-lower-bound", required_argument, 0, OPTION_FP_LOWER_BOUND},
    {"fp-num-bits", required_argument, 0, OPTION_FP_NUM_BITS},
    {"fp-precision", required_argument, 0, OPTION_FP_PRECISION},
    {"fp-upper-bound", required_argument, 0, OPTION_FP_UPPER_BOUND},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"num-iterations", required_argument, 0, OPTION_NUM_ITERATIONS},
    {"num-threads", required_argument, 0, OPTION_NUM_THREADS},
    {"path", required_argument, 0, OPTION_PATH},
    {"rep-categorical-representation", required_argument, 0, OPTION_REP_CATEGORICAL_REPRESENTATION},
    {"rep-num-additional-bits", required_argument, 0, OPTION_REP_NUM_ADDITIONAL_BITS},
    {"results-path", required_argument, 0, OPTION_RESULTS_PATH},
    {"seed", required_argument, 0, OPTION_SEED},
    {"solution-path", required_argument, 0, OPTION_SOLUTION_PATH},
    {"ea-allow-no-mutation", no_argument, 0, OPTION_EA_ALLOW_NO_MUTATION},
    {"fn-display", no_argument, 0, OPTION_FN_DISPLAY},
    {"fn-get-bv-size", no_argument, 0, OPTION_FN_GET_BV_SIZE},
    {"fn-get-output-size", no_argument, 0, OPTION_FN_GET_OUTPUT_SIZE},
    {"print-defaults", no_argument, 0, OPTION_PRINT_DEFAULTS},
    {"print-description", no_argument, 0, OPTION_PRINT_DESCRIPTION},
    {"print-header", no_argument, 0, OPTION_PRINT_HEADER},
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"help-fp", no_argument, 0, OPTION_HELP_FP},
    {"help-rep", no_argument, 0, OPTION_HELP_REP},
    {"help-ea", no_argument, 0, OPTION_HELP_EA},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "A:s:m:F:i:p:";
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 'A':
    case OPTION_ALGORITHM:
      set_algorithm(atoi(optarg));
      break;

    case 's':
    case OPTION_BV_SIZE:
      set_bv_size(atoi(optarg));
      break;

    case OPTION_DESCRIPTION_PATH:
      set_description_path(std::string(optarg));
      break;

    case OPTION_EA_CROSSOVER_PROBABILITY:
      set_ea_crossover_probability(atof(optarg));
      break;

    case OPTION_EA_MU:
      set_ea_mu(atoi(optarg));
      break;

    case 'm':
    case OPTION_EA_MUTATION_RATE:
      set_ea_mutation_rate(atof(optarg));
      break;

    case OPTION_EA_TOURNAMENT_SIZE:
      set_ea_tournament_size(atoi(optarg));
      break;

    case OPTION_FN_NAME:
      set_fn_name(std::string(optarg));
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

    case OPTION_REP_CATEGORICAL_REPRESENTATION:
      set_rep_categorical_representation(atoi(optarg));
      break;

    case OPTION_REP_NUM_ADDITIONAL_BITS:
      set_rep_num_additional_bits(atoi(optarg));
      break;

    case OPTION_RESULTS_PATH:
      set_results_path(std::string(optarg));
      break;

    case OPTION_SEED:
      set_seed(strtoul(optarg, NULL, 0));
      break;

    case OPTION_SOLUTION_PATH:
      set_solution_path(std::string(optarg));
      break;

    case OPTION_EA_ALLOW_NO_MUTATION:
      _ea_allow_no_mutation = true;
      break;

    case OPTION_FN_DISPLAY:
      _fn_display = true;
      break;

    case OPTION_FN_GET_BV_SIZE:
      _fn_get_bv_size = true;
      break;

    case OPTION_FN_GET_OUTPUT_SIZE:
      _fn_get_output_size = true;
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

    case OPTION_HELP:
      print_help(std::cerr);
      exit(0);

    case OPTION_HELP_FP:
      print_help_fp(std::cerr);
      exit(0);

    case OPTION_HELP_REP:
      print_help_rep(std::cerr);
      exit(0);

    case OPTION_HELP_EA:
      print_help_ea(std::cerr);
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
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "General" << std::endl;
  stream << "      --description-path (type string, default to \"description.txt\")" << std::endl;
  stream << "          Path of the description file" << std::endl;
  stream << "      --num-threads (type int, default to 1)" << std::endl;
  stream << "          Number of threads" << std::endl;
  stream << "      --print-defaults" << std::endl;
  stream << "          Print the default parameters and exit" << std::endl;
  stream << "      --print-description" << std::endl;
  stream << "          Print a description of the solution" << std::endl;
  stream << "      --print-header" << std::endl;
  stream << "          At the beginning, print the header" << std::endl;
  stream << "      --results-path (type string, default to \"results.json\")" << std::endl;
  stream << "          Path of the results file" << std::endl;
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
  stream << "      --fn-get-output-size" << std::endl;
  stream << "          Print the number of objectives" << std::endl;
  stream << "      --fn-name (type string, default to \"noname\")" << std::endl;
  stream << "          Name of the function in the dynamic library" << std::endl;
  stream << "  -F, --function (type int, default to 0)" << std::endl;
  stream << "          Type of function" << std::endl;
  stream << "            180: Real multivariate function" << std::endl;
  stream << "            181: Integer multivariate function" << std::endl;
  stream << "            182: Complex multivariate function (squared magnitude of)" << std::endl;
  stream << "            1100: Python function (embedded interpreter)" << std::endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << std::endl;
  stream << "          Path of a function file" << std::endl;
  stream << std::endl;
  stream << "Algorithms" << std::endl;
  stream << "  -A, --algorithm (type int, default to 1400)" << std::endl;
  stream << "          Type of algorithm" << std::endl;
  stream << "            1400: NGSA-II" << std::endl;
  stream << "  -i, --num-iterations (type int, default to 100)" << std::endl;
  stream << "          Number of iterations (<= 0 means indefinite)" << std::endl;
  stream << std::endl;
  stream  << "Additional Sections" << std::endl;
  stream << "      --help-fp" << std::endl;
  stream << "          Function parser" << std::endl;
  stream << "      --help-rep" << std::endl;
  stream << "          Representations" << std::endl;
  stream << "      --help-ea" << std::endl;
  stream << "          Evolutionary Algorithms" << std::endl;
}

void HncoOptions::print_help_fp(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
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
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
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

void HncoOptions::print_help_ea(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Evolutionary Algorithms" << std::endl;
  stream << "      --ea-allow-no-mutation" << std::endl;
  stream << "          Allow no mutation with standard bit mutation" << std::endl;
  stream << "      --ea-crossover-probability (type double, default to 0.8)" << std::endl;
  stream << "          Crossover probability" << std::endl;
  stream << "      --ea-mu (type int, default to 100)" << std::endl;
  stream << "          Parent population size" << std::endl;
  stream << "  -m, --ea-mutation-rate (type double, default to 1)" << std::endl;
  stream << "          Mutation rate relative to bv_size" << std::endl;
  stream << "      --ea-tournament-size (type int, default to 2)" << std::endl;
  stream << "          Tournament size" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_version(std::ostream& stream) const
{
  stream << _version << std::endl;
}

std::ostream& hnco::multiobjective::app::operator<<(std::ostream& stream, const HncoOptions& options)
{
  stream << "# algorithm = " << options._algorithm << std::endl;
  stream << "# bv_size = " << options._bv_size << std::endl;
  stream << "# description_path = " << options._description_path << std::endl;
  stream << "# ea_crossover_probability = " << options._ea_crossover_probability << std::endl;
  stream << "# ea_mu = " << options._ea_mu << std::endl;
  stream << "# ea_mutation_rate = " << options._ea_mutation_rate << std::endl;
  stream << "# ea_tournament_size = " << options._ea_tournament_size << std::endl;
  stream << "# fn_name = " << options._fn_name << std::endl;
  stream << "# fp_expression = " << options._fp_expression << std::endl;
  stream << "# fp_lower_bound = " << options._fp_lower_bound << std::endl;
  stream << "# fp_num_bits = " << options._fp_num_bits << std::endl;
  stream << "# fp_precision = " << options._fp_precision << std::endl;
  stream << "# fp_upper_bound = " << options._fp_upper_bound << std::endl;
  stream << "# function = " << options._function << std::endl;
  stream << "# num_iterations = " << options._num_iterations << std::endl;
  stream << "# num_threads = " << options._num_threads << std::endl;
  stream << "# path = " << options._path << std::endl;
  stream << "# rep_categorical_representation = " << options._rep_categorical_representation << std::endl;
  stream << "# rep_num_additional_bits = " << options._rep_num_additional_bits << std::endl;
  stream << "# results_path = " << options._results_path << std::endl;
  stream << "# seed = " << options._seed << std::endl;
  stream << "# solution_path = " << options._solution_path << std::endl;
  if (options._ea_allow_no_mutation)
    stream << "# ea_allow_no_mutation" << std::endl;
  if (options._fn_display)
    stream << "# fn_display" << std::endl;
  if (options._fn_get_bv_size)
    stream << "# fn_get_bv_size" << std::endl;
  if (options._fn_get_output_size)
    stream << "# fn_get_output_size" << std::endl;
  if (options._print_defaults)
    stream << "# print_defaults" << std::endl;
  if (options._print_description)
    stream << "# print_description" << std::endl;
  if (options._print_header)
    stream << "# print_header" << std::endl;
  stream << "# last_parameter" << std::endl;
  stream << "# exec_name = " << options._exec_name << std::endl;
  stream << "# version = " << options._version << std::endl;
  stream << "# Generated from hnco-mo.json" << std::endl;
  return stream;
}
