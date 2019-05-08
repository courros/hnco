#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "ffgen-options.hh"

using namespace std;

Options::Options(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.13"),
  _bv_size(100),
  _opt_bv_size(false),
  _coupling_constant(1),
  _opt_coupling_constant(false),
  _ep_upper_bound(1),
  _opt_ep_upper_bound(false),
  _field_constant(1),
  _opt_field_constant(false),
  _function(1),
  _opt_function(false),
  _ms_num_clauses(100),
  _opt_ms_num_clauses(false),
  _ms_num_literals_per_clause(3),
  _opt_ms_num_literals_per_clause(false),
  _nk_k(3),
  _opt_nk_k(false),
  _nn1_generator(0),
  _opt_nn1_generator(false),
  _nn2_generator(0),
  _opt_nn2_generator(false),
  _nn2_num_columns(10),
  _opt_nn2_num_columns(false),
  _nn2_num_rows(10),
  _opt_nn2_num_rows(false),
  _path("function.txt"),
  _opt_path(false),
  _seed(0),
  _opt_seed(false),
  _stddev(1),
  _opt_stddev(false),
  _walsh2_generator(0),
  _opt_walsh2_generator(false),
  _walsh2_ising_alpha(2),
  _opt_walsh2_ising_alpha(false),
  _walsh_num_features(100),
  _opt_walsh_num_features(false),
  _ms_planted_solution(false),
  _periodic_boundary_conditions(false)
{
  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_BV_SIZE,
    OPTION_COUPLING_CONSTANT,
    OPTION_EP_UPPER_BOUND,
    OPTION_FIELD_CONSTANT,
    OPTION_FUNCTION,
    OPTION_MS_NUM_CLAUSES,
    OPTION_MS_NUM_LITERALS_PER_CLAUSE,
    OPTION_NK_K,
    OPTION_NN1_GENERATOR,
    OPTION_NN2_GENERATOR,
    OPTION_NN2_NUM_COLUMNS,
    OPTION_NN2_NUM_ROWS,
    OPTION_PATH,
    OPTION_SEED,
    OPTION_STDDEV,
    OPTION_WALSH2_GENERATOR,
    OPTION_WALSH2_ISING_ALPHA,
    OPTION_WALSH_NUM_FEATURES,
    OPTION_MS_PLANTED_SOLUTION,
    OPTION_PERIODIC_BOUNDARY_CONDITIONS
  };
  const struct option long_options[] = {
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"coupling-constant", required_argument, 0, OPTION_COUPLING_CONSTANT},
    {"ep-upper-bound", required_argument, 0, OPTION_EP_UPPER_BOUND},
    {"field-constant", required_argument, 0, OPTION_FIELD_CONSTANT},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"ms-num-clauses", required_argument, 0, OPTION_MS_NUM_CLAUSES},
    {"ms-num-literals-per-clause", required_argument, 0, OPTION_MS_NUM_LITERALS_PER_CLAUSE},
    {"nk-k", required_argument, 0, OPTION_NK_K},
    {"nn1-generator", required_argument, 0, OPTION_NN1_GENERATOR},
    {"nn2-generator", required_argument, 0, OPTION_NN2_GENERATOR},
    {"nn2-num-columns", required_argument, 0, OPTION_NN2_NUM_COLUMNS},
    {"nn2-num-rows", required_argument, 0, OPTION_NN2_NUM_ROWS},
    {"path", required_argument, 0, OPTION_PATH},
    {"seed", required_argument, 0, OPTION_SEED},
    {"stddev", required_argument, 0, OPTION_STDDEV},
    {"walsh2-generator", required_argument, 0, OPTION_WALSH2_GENERATOR},
    {"walsh2-ising-alpha", required_argument, 0, OPTION_WALSH2_ISING_ALPHA},
    {"walsh-num-features", required_argument, 0, OPTION_WALSH_NUM_FEATURES},
    {"ms-planted-solution", no_argument, 0, OPTION_MS_PLANTED_SOLUTION},
    {"periodic-boundary-conditions", no_argument, 0, OPTION_PERIODIC_BOUNDARY_CONDITIONS},
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "s:F:p:";
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 's':
    case OPTION_BV_SIZE:
      set_bv_size(atoi(optarg));
      break;

    case OPTION_COUPLING_CONSTANT:
      set_coupling_constant(atof(optarg));
      break;

    case OPTION_EP_UPPER_BOUND:
      set_ep_upper_bound(atof(optarg));
      break;

    case OPTION_FIELD_CONSTANT:
      set_field_constant(atof(optarg));
      break;

    case 'F':
    case OPTION_FUNCTION:
      set_function(atoi(optarg));
      break;

    case OPTION_MS_NUM_CLAUSES:
      set_ms_num_clauses(atoi(optarg));
      break;

    case OPTION_MS_NUM_LITERALS_PER_CLAUSE:
      set_ms_num_literals_per_clause(atoi(optarg));
      break;

    case OPTION_NK_K:
      set_nk_k(atoi(optarg));
      break;

    case OPTION_NN1_GENERATOR:
      set_nn1_generator(atoi(optarg));
      break;

    case OPTION_NN2_GENERATOR:
      set_nn2_generator(atoi(optarg));
      break;

    case OPTION_NN2_NUM_COLUMNS:
      set_nn2_num_columns(atoi(optarg));
      break;

    case OPTION_NN2_NUM_ROWS:
      set_nn2_num_rows(atoi(optarg));
      break;

    case 'p':
    case OPTION_PATH:
      set_path(string(optarg));
      break;

    case OPTION_SEED:
      set_seed(atoi(optarg));
      break;

    case OPTION_STDDEV:
      set_stddev(atof(optarg));
      break;

    case OPTION_WALSH2_GENERATOR:
      set_walsh2_generator(atoi(optarg));
      break;

    case OPTION_WALSH2_ISING_ALPHA:
      set_walsh2_ising_alpha(atof(optarg));
      break;

    case OPTION_WALSH_NUM_FEATURES:
      set_walsh_num_features(atoi(optarg));
      break;

    case OPTION_MS_PLANTED_SOLUTION:
      _ms_planted_solution = true;
      break;

    case OPTION_PERIODIC_BOUNDARY_CONDITIONS:
      _periodic_boundary_conditions = true;
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
  stream << "Fitness function generator for HNCO" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "General" << endl;
  stream << "  -s, --bv-size (type int, default to 100)" << endl;
  stream << "          Size of bit vectors" << endl;
  stream << "  -F, --function (type int, default to 1)" << endl;
  stream << "          Type of function" << endl;
  stream << "            1: Linear function" << endl;
  stream << "            60: NK landscape" << endl;
  stream << "            70: Max-SAT" << endl;
  stream << "            90: Equal products" << endl;
  stream << "            160: Walsh expansion" << endl;
  stream << "            161: Walsh expansion of degree 1" << endl;
  stream << "            162: Walsh expansion of degree 2" << endl;
  stream << "            171: Nearest neighbor Ising model in one dimension" << endl;
  stream << "            172: Nearest neighbor Ising model in two dimensions" << endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << endl;
  stream << "          Path (relative or absolute) of a function file" << endl;
  stream << "      --seed (type int, default to 0)" << endl;
  stream << "          Seed for the random number generator" << endl;
  stream << endl;
  stream << "Parameters" << endl;
  stream << "      --coupling-constant (type double, default to 1)" << endl;
  stream << "          Coupling constant" << endl;
  stream << "      --field-constant (type double, default to 1)" << endl;
  stream << "          Field constant" << endl;
  stream << "      --periodic-boundary-conditions" << endl;
  stream << "          Periodic boundary conditions" << endl;
  stream << "      --stddev (type double, default to 1)" << endl;
  stream << "          Standard deviation" << endl;
  stream << endl;
  stream << "NK Landscape" << endl;
  stream << "      --nk-k (type int, default to 3)" << endl;
  stream << "          Each bit is connected to k other bits" << endl;
  stream << endl;
  stream << "Max-SAT" << endl;
  stream << "      --ms-num-clauses (type int, default to 100)" << endl;
  stream << "          Number of clauses" << endl;
  stream << "      --ms-num-literals-per-clause (type int, default to 3)" << endl;
  stream << "          Number of literals per clause" << endl;
  stream << "      --ms-planted-solution" << endl;
  stream << "          Generate an instance with a planted solution" << endl;
  stream << endl;
  stream << "Equal Products" << endl;
  stream << "      --ep-upper-bound (type double, default to 1)" << endl;
  stream << "          Parameter upper bound" << endl;
  stream << endl;
  stream << "Walsh Expansion" << endl;
  stream << "      --walsh-num-features (type int, default to 100)" << endl;
  stream << "          Number of features" << endl;
  stream << endl;
  stream << "Walsh expansion of degree 2" << endl;
  stream << "      --walsh2-generator (type int, default to 0)" << endl;
  stream << "          Type of WalshExpansion2 generator" << endl;
  stream << "            0: Random coefficients" << endl;
  stream << "            1: One dimensional Dyson Ising model with long range interactions" << endl;
  stream << "      --walsh2-ising-alpha (type double, default to 2)" << endl;
  stream << "          Dyson-Ising: exponential decay parameter for long range interactions" << endl;
  stream << endl;
  stream << "Nearest neighbor Ising model in one dimension" << endl;
  stream << "      --nn1-generator (type int, default to 0)" << endl;
  stream << "          Type of NearestNeighborIsingModel1 generator" << endl;
  stream << "            0: Random coupling, random field" << endl;
  stream << "            1: Random coupling, constant field" << endl;
  stream << "            2: Constant coupling, random field" << endl;
  stream << "            3: Constant coupling, constant field" << endl;
  stream << endl;
  stream << "Nearest neighbor Ising model in two dimensions" << endl;
  stream << "      --nn2-generator (type int, default to 0)" << endl;
  stream << "          Type of NearestNeighborIsingModel2 generator" << endl;
  stream << "            0: Random coupling, random field" << endl;
  stream << "            1: Random coupling, constant field" << endl;
  stream << "            2: Constant coupling, random field" << endl;
  stream << "            3: Constant coupling, constant field" << endl;
  stream << "      --nn2-num-columns (type int, default to 10)" << endl;
  stream << "          Number of columns" << endl;
  stream << "      --nn2-num-rows (type int, default to 10)" << endl;
  stream << "          Number of rows" << endl;
  stream << endl;
}

void Options::print_version(ostream& stream) const
{
  stream << _version << endl;
}

ostream& operator<<(ostream& stream, const Options& options)
{
  stream << "# bv_size = " << options._bv_size << endl;
  stream << "# coupling_constant = " << options._coupling_constant << endl;
  stream << "# ep_upper_bound = " << options._ep_upper_bound << endl;
  stream << "# field_constant = " << options._field_constant << endl;
  stream << "# function = " << options._function << endl;
  stream << "# ms_num_clauses = " << options._ms_num_clauses << endl;
  stream << "# ms_num_literals_per_clause = " << options._ms_num_literals_per_clause << endl;
  stream << "# nk_k = " << options._nk_k << endl;
  stream << "# nn1_generator = " << options._nn1_generator << endl;
  stream << "# nn2_generator = " << options._nn2_generator << endl;
  stream << "# nn2_num_columns = " << options._nn2_num_columns << endl;
  stream << "# nn2_num_rows = " << options._nn2_num_rows << endl;
  stream << "# path = " << options._path << endl;
  stream << "# seed = " << options._seed << endl;
  stream << "# stddev = " << options._stddev << endl;
  stream << "# walsh2_generator = " << options._walsh2_generator << endl;
  stream << "# walsh2_ising_alpha = " << options._walsh2_ising_alpha << endl;
  stream << "# walsh_num_features = " << options._walsh_num_features << endl;
  if (options._ms_planted_solution)
    stream << "# ms_planted_solution" << endl;
  if (options._periodic_boundary_conditions)
    stream << "# periodic_boundary_conditions" << endl;
  stream << "# last_parameter" << endl;
  stream << "# exec_name = " << options._exec_name << endl;
  stream << "# version = " << options._version << endl;
  stream << "# Generated from ffgen.json" << endl;
  return stream;
}
