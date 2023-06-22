#include <cstdlib>
#include <getopt.h>

#include "hnco-mo-options.hh"

using namespace hnco::multiobjective::app;

HncoOptions::HncoOptions(int argc, char *argv[], bool ignore_bad_options):
  _exec_name(argv[0])
{
  const struct option long_options[] = {
    {"algorithm", required_argument, 0, OPTION_ALGORITHM},
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"description-path", required_argument, 0, OPTION_DESCRIPTION_PATH},
    {"ea-crossover-probability", required_argument, 0, OPTION_EA_CROSSOVER_PROBABILITY},
    {"ea-mu", required_argument, 0, OPTION_EA_MU},
    {"ea-mutation-rate", required_argument, 0, OPTION_EA_MUTATION_RATE},
    {"ea-tournament-size", required_argument, 0, OPTION_EA_TOURNAMENT_SIZE},
    {"fn-name", required_argument, 0, OPTION_FN_NAME},
    {"fp-default-interval", required_argument, 0, OPTION_FP_DEFAULT_INTERVAL},
    {"fp-default-precision", required_argument, 0, OPTION_FP_DEFAULT_PRECISION},
    {"fp-default-size", required_argument, 0, OPTION_FP_DEFAULT_SIZE},
    {"fp-expression", required_argument, 0, OPTION_FP_EXPRESSION},
    {"fp-intervals", required_argument, 0, OPTION_FP_INTERVALS},
    {"fp-precisions", required_argument, 0, OPTION_FP_PRECISIONS},
    {"fp-sizes", required_argument, 0, OPTION_FP_SIZES},
    {"fp-source", required_argument, 0, OPTION_FP_SOURCE},
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
    {"print-default-parameters", no_argument, 0, OPTION_PRINT_DEFAULT_PARAMETERS},
    {"print-description", no_argument, 0, OPTION_PRINT_DESCRIPTION},
    {"print-parameters", no_argument, 0, OPTION_PRINT_PARAMETERS},
    {"print-pareto-front", no_argument, 0, OPTION_PRINT_PARETO_FRONT},
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"help-fn", no_argument, 0, OPTION_HELP_FN},
    {"help-fp", no_argument, 0, OPTION_HELP_FP},
    {"help-rep", no_argument, 0, OPTION_HELP_REP},
    {"help-alg", no_argument, 0, OPTION_HELP_ALG},
    {"help-ea", no_argument, 0, OPTION_HELP_EA},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "A:s:m:F:i:p:";
  optind = 0;
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 'A':
    case OPTION_ALGORITHM:
      _with_algorithm = true;
      _algorithm = std::atoi(optarg);
      break;

    case 's':
    case OPTION_BV_SIZE:
      _with_bv_size = true;
      _bv_size = std::atoi(optarg);
      break;

    case OPTION_DESCRIPTION_PATH:
      _with_description_path = true;
      _description_path = std::string(optarg);
      break;

    case OPTION_EA_CROSSOVER_PROBABILITY:
      _with_ea_crossover_probability = true;
      _ea_crossover_probability = std::atof(optarg);
      break;

    case OPTION_EA_MU:
      _with_ea_mu = true;
      _ea_mu = std::atoi(optarg);
      break;

    case 'm':
    case OPTION_EA_MUTATION_RATE:
      _with_ea_mutation_rate = true;
      _ea_mutation_rate = std::atof(optarg);
      break;

    case OPTION_EA_TOURNAMENT_SIZE:
      _with_ea_tournament_size = true;
      _ea_tournament_size = std::atoi(optarg);
      break;

    case OPTION_FN_NAME:
      _with_fn_name = true;
      _fn_name = std::string(optarg);
      break;

    case OPTION_FP_DEFAULT_INTERVAL:
      _with_fp_default_interval = true;
      _fp_default_interval = std::string(optarg);
      break;

    case OPTION_FP_DEFAULT_PRECISION:
      _with_fp_default_precision = true;
      _fp_default_precision = std::atof(optarg);
      break;

    case OPTION_FP_DEFAULT_SIZE:
      _with_fp_default_size = true;
      _fp_default_size = std::atoi(optarg);
      break;

    case OPTION_FP_EXPRESSION:
      _with_fp_expression = true;
      _fp_expression = std::string(optarg);
      break;

    case OPTION_FP_INTERVALS:
      _with_fp_intervals = true;
      _fp_intervals = std::string(optarg);
      break;

    case OPTION_FP_PRECISIONS:
      _with_fp_precisions = true;
      _fp_precisions = std::string(optarg);
      break;

    case OPTION_FP_SIZES:
      _with_fp_sizes = true;
      _fp_sizes = std::string(optarg);
      break;

    case OPTION_FP_SOURCE:
      _with_fp_source = true;
      _fp_source = std::atoi(optarg);
      break;

    case 'F':
    case OPTION_FUNCTION:
      _with_function = true;
      _function = std::atoi(optarg);
      break;

    case 'i':
    case OPTION_NUM_ITERATIONS:
      _with_num_iterations = true;
      _num_iterations = std::atoi(optarg);
      break;

    case OPTION_NUM_THREADS:
      _with_num_threads = true;
      _num_threads = std::atoi(optarg);
      break;

    case 'p':
    case OPTION_PATH:
      _with_path = true;
      _path = std::string(optarg);
      break;

    case OPTION_REP_CATEGORICAL_REPRESENTATION:
      _with_rep_categorical_representation = true;
      _rep_categorical_representation = std::atoi(optarg);
      break;

    case OPTION_REP_NUM_ADDITIONAL_BITS:
      _with_rep_num_additional_bits = true;
      _rep_num_additional_bits = std::atoi(optarg);
      break;

    case OPTION_RESULTS_PATH:
      _with_results_path = true;
      _results_path = std::string(optarg);
      break;

    case OPTION_SEED:
      _with_seed = true;
      _seed = std::strtoul(optarg, NULL, 0);
      break;

    case OPTION_SOLUTION_PATH:
      _with_solution_path = true;
      _solution_path = std::string(optarg);
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

    case OPTION_PRINT_DEFAULT_PARAMETERS:
      _print_default_parameters = true;
      break;

    case OPTION_PRINT_DESCRIPTION:
      _print_description = true;
      break;

    case OPTION_PRINT_PARAMETERS:
      _print_parameters = true;
      break;

    case OPTION_PRINT_PARETO_FRONT:
      _print_pareto_front = true;
      break;

    case OPTION_HELP:
      print_help(std::cerr);
      exit(0);

    case OPTION_HELP_FN:
      print_help_fn(std::cerr);
      exit(0);

    case OPTION_HELP_FP:
      print_help_fp(std::cerr);
      exit(0);

    case OPTION_HELP_REP:
      print_help_rep(std::cerr);
      exit(0);

    case OPTION_HELP_ALG:
      print_help_alg(std::cerr);
      exit(0);

    case OPTION_HELP_EA:
      print_help_ea(std::cerr);
      exit(0);

    case OPTION_VERSION:
      print_version(std::cerr);
      exit(0);

    default:
      if (!ignore_bad_options) {
        std::cerr << "For more information, please enter: " << _exec_name << " --help" << std::endl;
        exit(1);
      }
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
  stream << "      --print-default-parameters" << std::endl;
  stream << "          Print the parameters and exit" << std::endl;
  stream << "      --print-description" << std::endl;
  stream << "          Print a description of the solution" << std::endl;
  stream << "      --print-parameters" << std::endl;
  stream << "          Print the parameters" << std::endl;
  stream << "      --print-pareto-front" << std::endl;
  stream << "          Print the Pareto front" << std::endl;
  stream << "      --results-path (type string, default to \"results.json\")" << std::endl;
  stream << "          Path of the results file" << std::endl;
  stream << "      --seed (type unsigned, no default)" << std::endl;
  stream << "          Seed for the random number generator" << std::endl;
  stream << "      --solution-path (type string, default to \"solution.txt\")" << std::endl;
  stream << "          Path of the solution file" << std::endl;
  stream << std::endl;
  stream  << "Additional Sections" << std::endl;
  stream << "      --help-fn" << std::endl;
  stream << "          Functions" << std::endl;
  stream << "      --help-fp" << std::endl;
  stream << "          Function parser" << std::endl;
  stream << "      --help-rep" << std::endl;
  stream << "          Representations" << std::endl;
  stream << "      --help-alg" << std::endl;
  stream << "          Algorithms" << std::endl;
  stream << "      --help-ea" << std::endl;
  stream << "          Evolutionary Algorithms" << std::endl;
}

void HncoOptions::print_help_fn(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Functions" << std::endl;
  stream << "  -s, --bv-size (type int, default to 100)" << std::endl;
  stream << "          Size of bit vectors" << std::endl;
  stream << "      --fn-display" << std::endl;
  stream << "          Display the function and exit" << std::endl;
  stream << "      --fn-get-bv-size" << std::endl;
  stream << "          Print the size of bit vectors" << std::endl;
  stream << "      --fn-get-output-size" << std::endl;
  stream << "          Print the number of objectives" << std::endl;
  stream << "      --fn-name (type string, no default)" << std::endl;
  stream << "          Name of the function in the dynamic library" << std::endl;
  stream << "  -F, --function (type int, default to 180)" << std::endl;
  stream << "          Type of function" << std::endl;
  stream << "            180: Real multivariate function (parser: double -> double)" << std::endl;
  stream << "            181: Integer multivariate function (parser: long -> long, then cast to double)" << std::endl;
  stream << "            182: Complex multivariate function (parser: complex -> complex, then square of the magnitude)" << std::endl;
  stream << "            1100: Python function (embedded interpreter)" << std::endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << std::endl;
  stream << "          Path of a function file" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_fp(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Function parser" << std::endl;
  stream << "      --fp-default-interval (type string, default to \"[-3, 3]\")" << std::endl;
  stream << "          Default interval" << std::endl;
  stream << "      --fp-default-precision (type double, no default)" << std::endl;
  stream << "          Default precision of dyadic representations of numbers (supersedes fp_default_size)" << std::endl;
  stream << "      --fp-default-size (type int, default to 8)" << std::endl;
  stream << "          Default size of dyadic representations of numbers" << std::endl;
  stream << "      --fp-expression (type string, default to \"sin(x) + cos(y) :: sqrt(x^2 + y^2)\")" << std::endl;
  stream << "          Expression to parse (objectives are separated by ::)" << std::endl;
  stream << "      --fp-intervals (type string, default to \"\")" << std::endl;
  stream << "          Intervals (supersedes fp_default_interval). Example: \"x: [0, 1]; y: [0, 1]\"" << std::endl;
  stream << "      --fp-precisions (type string, default to \"\")" << std::endl;
  stream << "          Per variable precisions of dyadic representations of numbers (supersedes fp_sizes). Example: \"x: 1e-3; y: 1e-4\"" << std::endl;
  stream << "      --fp-sizes (type string, default to \"\")" << std::endl;
  stream << "          Per variable size of dyadic representations of numbers (supersedes fp_default_precision). Example: \"x: 4; y: 6\"" << std::endl;
  stream << "      --fp-source (type int, default to 0)" << std::endl;
  stream << "          Source for the expression to parse" << std::endl;
  stream << "            0: Command-line" << std::endl;
  stream << "            1: Function file" << std::endl;
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

void HncoOptions::print_help_alg(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Algorithms" << std::endl;
  stream << "  -A, --algorithm (type int, default to 100)" << std::endl;
  stream << "          Type of algorithm" << std::endl;
  stream << "            100: NSGA-II" << std::endl;
  stream << "  -i, --num-iterations (type int, default to 100)" << std::endl;
  stream << "          Number of iterations" << std::endl;
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
  stream << "# description_path = \"" << options._description_path << "\"" << std::endl;
  stream << "# ea_crossover_probability = " << options._ea_crossover_probability << std::endl;
  stream << "# ea_mu = " << options._ea_mu << std::endl;
  stream << "# ea_mutation_rate = " << options._ea_mutation_rate << std::endl;
  stream << "# ea_tournament_size = " << options._ea_tournament_size << std::endl;
  if (options._with_fn_name)
    stream << "# fn_name = \"" << options._fn_name << "\"" << std::endl;
  stream << "# fp_default_interval = \"" << options._fp_default_interval << "\"" << std::endl;
  if (options._with_fp_default_precision)
    stream << "# fp_default_precision = " << options._fp_default_precision << std::endl;
  stream << "# fp_default_size = " << options._fp_default_size << std::endl;
  stream << "# fp_expression = \"" << options._fp_expression << "\"" << std::endl;
  stream << "# fp_intervals = \"" << options._fp_intervals << "\"" << std::endl;
  stream << "# fp_precisions = \"" << options._fp_precisions << "\"" << std::endl;
  stream << "# fp_sizes = \"" << options._fp_sizes << "\"" << std::endl;
  stream << "# fp_source = " << options._fp_source << std::endl;
  stream << "# function = " << options._function << std::endl;
  stream << "# num_iterations = " << options._num_iterations << std::endl;
  stream << "# num_threads = " << options._num_threads << std::endl;
  stream << "# path = \"" << options._path << "\"" << std::endl;
  stream << "# rep_categorical_representation = " << options._rep_categorical_representation << std::endl;
  stream << "# rep_num_additional_bits = " << options._rep_num_additional_bits << std::endl;
  stream << "# results_path = \"" << options._results_path << "\"" << std::endl;
  if (options._with_seed)
    stream << "# seed = " << options._seed << std::endl;
  stream << "# solution_path = \"" << options._solution_path << "\"" << std::endl;
  if (options._ea_allow_no_mutation)
    stream << "# ea_allow_no_mutation " << std::endl;
  if (options._fn_display)
    stream << "# fn_display " << std::endl;
  if (options._fn_get_bv_size)
    stream << "# fn_get_bv_size " << std::endl;
  if (options._fn_get_output_size)
    stream << "# fn_get_output_size " << std::endl;
  if (options._print_default_parameters)
    stream << "# print_default_parameters " << std::endl;
  if (options._print_description)
    stream << "# print_description " << std::endl;
  if (options._print_parameters)
    stream << "# print_parameters " << std::endl;
  if (options._print_pareto_front)
    stream << "# print_pareto_front " << std::endl;
  stream << "# last_parameter" << std::endl;
  stream << "# exec_name = " << options._exec_name << std::endl;
  stream << "# version = " << options._version << std::endl;
  stream << "# Generated from hnco-mo.json" << std::endl;
  return stream;
}
