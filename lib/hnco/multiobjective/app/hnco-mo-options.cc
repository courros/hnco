#include <cstdlib>
#include <getopt.h>

#include "hnco-mo-options.hh"

using namespace hnco::multiobjective::app;

inline bool check_string_as_bool(std::string str) { return str == "true" || str == "false"; }

HncoOptions::HncoOptions(int argc, char *argv[], bool ignore_bad_options):
  _exec_name(argv[0])
{
  const struct option long_options[] = {
    {"algorithm", required_argument, 0, OPTION_ALGORITHM},
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"ea-crossover-probability", required_argument, 0, OPTION_EA_CROSSOVER_PROBABILITY},
    {"ea-mu", required_argument, 0, OPTION_EA_MU},
    {"ea-mutation-rate", required_argument, 0, OPTION_EA_MUTATION_RATE},
    {"ea-tournament-size", required_argument, 0, OPTION_EA_TOURNAMENT_SIZE},
    {"fn-name", required_argument, 0, OPTION_FN_NAME},
    {"fp-default-double-precision", required_argument, 0, OPTION_FP_DEFAULT_DOUBLE_PRECISION},
    {"fp-default-double-rep", required_argument, 0, OPTION_FP_DEFAULT_DOUBLE_REP},
    {"fp-default-double-size", required_argument, 0, OPTION_FP_DEFAULT_DOUBLE_SIZE},
    {"fp-default-int-rep", required_argument, 0, OPTION_FP_DEFAULT_INT_REP},
    {"fp-default-long-rep", required_argument, 0, OPTION_FP_DEFAULT_LONG_REP},
    {"fp-expression", required_argument, 0, OPTION_FP_EXPRESSION},
    {"fp-representations", required_argument, 0, OPTION_FP_REPRESENTATIONS},
    {"fp-representations-path", required_argument, 0, OPTION_FP_REPRESENTATIONS_PATH},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"num-iterations", required_argument, 0, OPTION_NUM_ITERATIONS},
    {"num-threads", required_argument, 0, OPTION_NUM_THREADS},
    {"path", required_argument, 0, OPTION_PATH},
    {"rep-categorical-representation", required_argument, 0, OPTION_REP_CATEGORICAL_REPRESENTATION},
    {"rep-num-additional-bits", required_argument, 0, OPTION_REP_NUM_ADDITIONAL_BITS},
    {"seed", required_argument, 0, OPTION_SEED},
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

    case OPTION_FP_DEFAULT_DOUBLE_PRECISION:
      _with_fp_default_double_precision = true;
      _fp_default_double_precision = std::atof(optarg);
      break;

    case OPTION_FP_DEFAULT_DOUBLE_REP:
      _with_fp_default_double_rep = true;
      _fp_default_double_rep = std::string(optarg);
      break;

    case OPTION_FP_DEFAULT_DOUBLE_SIZE:
      _with_fp_default_double_size = true;
      _fp_default_double_size = std::atoi(optarg);
      break;

    case OPTION_FP_DEFAULT_INT_REP:
      _with_fp_default_int_rep = true;
      _fp_default_int_rep = std::string(optarg);
      break;

    case OPTION_FP_DEFAULT_LONG_REP:
      _with_fp_default_long_rep = true;
      _fp_default_long_rep = std::string(optarg);
      break;

    case OPTION_FP_EXPRESSION:
      _with_fp_expression = true;
      _fp_expression = std::string(optarg);
      break;

    case OPTION_FP_REPRESENTATIONS:
      _with_fp_representations = true;
      _fp_representations = std::string(optarg);
      break;

    case OPTION_FP_REPRESENTATIONS_PATH:
      _with_fp_representations_path = true;
      _fp_representations_path = std::string(optarg);
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

    case OPTION_SEED:
      _with_seed = true;
      _seed = std::strtoul(optarg, NULL, 0);
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
  stream << "HNCO for multiobjective optimization (minimization)" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "General" << std::endl;
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
  stream << "      --seed (type unsigned, no default)" << std::endl;
  stream << "          Seed for the random number generator" << std::endl;
  stream << std::endl;
  stream << "Additional Sections" << std::endl;
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
  stream << "HNCO for multiobjective optimization (minimization)" << std::endl << std::endl;
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
  stream << "            520: Real multivariate function (rep: bv -> double | parser: [double] -> [double])" << std::endl;
  stream << "            521: Integer multivariate function (rep: bv -> long | parser: [long] -> [long] | cast to double)" << std::endl;
  stream << "            522: Complex multivariate function (rep: bv -> complex | parser: [complex] -> [complex] | z -> std::norm(z))" << std::endl;
  stream << "            523: Integer multivariate function (rep: bv -> int | cast to double | parser: [double] -> [double])" << std::endl;
  stream << "            524: Mixed-representation multivariate function (rep: bv -> long, double, or set | parser: [double] -> [double])" << std::endl;
  stream << "            530: System of real multivariate equations (rep: bv -> double | parser: [double] -> [double] | x -> std::fabs(x))" << std::endl;
  stream << "            531: System of integer multivariate equations (rep: bv -> long | parser: [long] -> [long] | n -> std::fabs(n))" << std::endl;
  stream << "            532: System of complex multivariate equations (rep: bv -> complex | parser: [complex] -> [complex] | z -> std::norm(z))" << std::endl;
  stream << "            533: System of integer multivariate equations (rep: bv -> int | cast to double | parser: [double] -> [double] | x -> std::fabs(x))" << std::endl;
  stream << "            534: System of mixed-representation multivariate equations (rep: bv -> long, double, or set | parser: [double] -> [double] | x -> std::fabs(x))" << std::endl;
  stream << "            1100: Python function (embedded interpreter)" << std::endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << std::endl;
  stream << "          Path of a function file" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_fp(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization (minimization)" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Function parser" << std::endl;
  stream << "      --fp-default-double-precision (type double, no default)" << std::endl;
  stream << "          Default precision of double representations" << std::endl;
  stream << "      --fp-default-double-rep (type string, default to \"double(0, 1, precision = 1e-3)\")" << std::endl;
  stream << "          Default representation for double" << std::endl;
  stream << "      --fp-default-double-size (type int, no default)" << std::endl;
  stream << "          Default size of double representations" << std::endl;
  stream << "      --fp-default-int-rep (type string, default to \"int(-10, 10)\")" << std::endl;
  stream << "          Default representation for int" << std::endl;
  stream << "      --fp-default-long-rep (type string, default to \"long(-100, 100)\")" << std::endl;
  stream << "          Default representation for long" << std::endl;
  stream << "      --fp-expression (type string, no default)" << std::endl;
  stream << "          Mathematical expression (list of objectives separated by ::)" << std::endl;
  stream << "      --fp-representations (type string, no default)" << std::endl;
  stream << "          Representations. Example: \"x: double(0, 1); y: double(0, 1, precision = 1e-3); z: double(0, 1, size = 8); u: int(-10, 10); v: long(-100, 100); w: set(1.1, 2.2, 3.3)\"" << std::endl;
  stream << "      --fp-representations-path (type string, default to \"representations.txt\")" << std::endl;
  stream << "          Path of the representations file" << std::endl;
  stream << std::endl;
}

void HncoOptions::print_help_rep(std::ostream& stream) const
{
  stream << "HNCO for multiobjective optimization (minimization)" << std::endl << std::endl;
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
  stream << "HNCO for multiobjective optimization (minimization)" << std::endl << std::endl;
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
  stream << "HNCO for multiobjective optimization (minimization)" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "Evolutionary Algorithms" << std::endl;
  stream << "      --ea-allow-no-mutation" << std::endl;
  stream << "          Allow no mutation with standard bit mutation" << std::endl;
  stream << "      --ea-crossover-probability (type double, default to 0.8)" << std::endl;
  stream << "          Crossover probability" << std::endl;
  stream << "      --ea-mu (type int, default to 100)" << std::endl;
  stream << "          Parent population size" << std::endl;
  stream << "  -m, --ea-mutation-rate (type double, no default)" << std::endl;
  stream << "          Mutation rate" << std::endl;
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
  stream << std::boolalpha;
  stream << "# algorithm = " << options._algorithm << std::endl;
  stream << "# bv_size = " << options._bv_size << std::endl;
  stream << "# ea_crossover_probability = " << options._ea_crossover_probability << std::endl;
  stream << "# ea_mu = " << options._ea_mu << std::endl;
  if (options._with_ea_mutation_rate)
    stream << "# ea_mutation_rate = " << options._ea_mutation_rate << std::endl;
  stream << "# ea_tournament_size = " << options._ea_tournament_size << std::endl;
  if (options._with_fn_name)
    stream << "# fn_name = \"" << options._fn_name << "\"" << std::endl;
  if (options._with_fp_default_double_precision)
    stream << "# fp_default_double_precision = " << options._fp_default_double_precision << std::endl;
  stream << "# fp_default_double_rep = \"" << options._fp_default_double_rep << "\"" << std::endl;
  if (options._with_fp_default_double_size)
    stream << "# fp_default_double_size = " << options._fp_default_double_size << std::endl;
  stream << "# fp_default_int_rep = \"" << options._fp_default_int_rep << "\"" << std::endl;
  stream << "# fp_default_long_rep = \"" << options._fp_default_long_rep << "\"" << std::endl;
  if (options._with_fp_expression)
    stream << "# fp_expression = \"" << options._fp_expression << "\"" << std::endl;
  if (options._with_fp_representations)
    stream << "# fp_representations = \"" << options._fp_representations << "\"" << std::endl;
  stream << "# fp_representations_path = \"" << options._fp_representations_path << "\"" << std::endl;
  stream << "# function = " << options._function << std::endl;
  stream << "# num_iterations = " << options._num_iterations << std::endl;
  stream << "# num_threads = " << options._num_threads << std::endl;
  stream << "# path = \"" << options._path << "\"" << std::endl;
  stream << "# rep_categorical_representation = " << options._rep_categorical_representation << std::endl;
  stream << "# rep_num_additional_bits = " << options._rep_num_additional_bits << std::endl;
  if (options._with_seed)
    stream << "# seed = " << options._seed << std::endl;
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
