#include <cstdlib>
#include <getopt.h>

#include "ffgen-options.hh"

using namespace hnco::app;

FfgenOptions::FfgenOptions(int argc, char *argv[], bool ignore_bad_options):
  _exec_name(argv[0])
{
  const struct option long_options[] = {
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"coupling-constant", required_argument, 0, OPTION_COUPLING_CONSTANT},
    {"ep-upper-bound", required_argument, 0, OPTION_EP_UPPER_BOUND},
    {"field-constant", required_argument, 0, OPTION_FIELD_CONSTANT},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"lin-distance", required_argument, 0, OPTION_LIN_DISTANCE},
    {"lin-generator", required_argument, 0, OPTION_LIN_GENERATOR},
    {"lin-initial-weight", required_argument, 0, OPTION_LIN_INITIAL_WEIGHT},
    {"lin-ratio", required_argument, 0, OPTION_LIN_RATIO},
    {"ms-num-clauses", required_argument, 0, OPTION_MS_NUM_CLAUSES},
    {"ms-num-literals-per-clause", required_argument, 0, OPTION_MS_NUM_LITERALS_PER_CLAUSE},
    {"nk-k", required_argument, 0, OPTION_NK_K},
    {"nn1-generator", required_argument, 0, OPTION_NN1_GENERATOR},
    {"nn2-generator", required_argument, 0, OPTION_NN2_GENERATOR},
    {"nn2-num-columns", required_argument, 0, OPTION_NN2_NUM_COLUMNS},
    {"nn2-num-rows", required_argument, 0, OPTION_NN2_NUM_ROWS},
    {"part-upper-bound", required_argument, 0, OPTION_PART_UPPER_BOUND},
    {"path", required_argument, 0, OPTION_PATH},
    {"seed", required_argument, 0, OPTION_SEED},
    {"stddev", required_argument, 0, OPTION_STDDEV},
    {"sudoku-num-empty-cells", required_argument, 0, OPTION_SUDOKU_NUM_EMPTY_CELLS},
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
  optind = 0;
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 's':
    case OPTION_BV_SIZE:
      _with_bv_size = true;
      _bv_size = std::atoi(optarg);
      break;

    case OPTION_COUPLING_CONSTANT:
      _with_coupling_constant = true;
      _coupling_constant = std::atof(optarg);
      break;

    case OPTION_EP_UPPER_BOUND:
      _with_ep_upper_bound = true;
      _ep_upper_bound = std::atof(optarg);
      break;

    case OPTION_FIELD_CONSTANT:
      _with_field_constant = true;
      _field_constant = std::atof(optarg);
      break;

    case 'F':
    case OPTION_FUNCTION:
      _with_function = true;
      _function = std::atoi(optarg);
      break;

    case OPTION_LIN_DISTANCE:
      _with_lin_distance = true;
      _lin_distance = std::atof(optarg);
      break;

    case OPTION_LIN_GENERATOR:
      _with_lin_generator = true;
      _lin_generator = std::atoi(optarg);
      break;

    case OPTION_LIN_INITIAL_WEIGHT:
      _with_lin_initial_weight = true;
      _lin_initial_weight = std::atof(optarg);
      break;

    case OPTION_LIN_RATIO:
      _with_lin_ratio = true;
      _lin_ratio = std::atof(optarg);
      break;

    case OPTION_MS_NUM_CLAUSES:
      _with_ms_num_clauses = true;
      _ms_num_clauses = std::atoi(optarg);
      break;

    case OPTION_MS_NUM_LITERALS_PER_CLAUSE:
      _with_ms_num_literals_per_clause = true;
      _ms_num_literals_per_clause = std::atoi(optarg);
      break;

    case OPTION_NK_K:
      _with_nk_k = true;
      _nk_k = std::atoi(optarg);
      break;

    case OPTION_NN1_GENERATOR:
      _with_nn1_generator = true;
      _nn1_generator = std::atoi(optarg);
      break;

    case OPTION_NN2_GENERATOR:
      _with_nn2_generator = true;
      _nn2_generator = std::atoi(optarg);
      break;

    case OPTION_NN2_NUM_COLUMNS:
      _with_nn2_num_columns = true;
      _nn2_num_columns = std::atoi(optarg);
      break;

    case OPTION_NN2_NUM_ROWS:
      _with_nn2_num_rows = true;
      _nn2_num_rows = std::atoi(optarg);
      break;

    case OPTION_PART_UPPER_BOUND:
      _with_part_upper_bound = true;
      _part_upper_bound = std::atoi(optarg);
      break;

    case 'p':
    case OPTION_PATH:
      _with_path = true;
      _path = std::string(optarg);
      break;

    case OPTION_SEED:
      _with_seed = true;
      _seed = std::atoi(optarg);
      break;

    case OPTION_STDDEV:
      _with_stddev = true;
      _stddev = std::atof(optarg);
      break;

    case OPTION_SUDOKU_NUM_EMPTY_CELLS:
      _with_sudoku_num_empty_cells = true;
      _sudoku_num_empty_cells = std::atoi(optarg);
      break;

    case OPTION_WALSH2_GENERATOR:
      _with_walsh2_generator = true;
      _walsh2_generator = std::atoi(optarg);
      break;

    case OPTION_WALSH2_ISING_ALPHA:
      _with_walsh2_ising_alpha = true;
      _walsh2_ising_alpha = std::atof(optarg);
      break;

    case OPTION_WALSH_NUM_FEATURES:
      _with_walsh_num_features = true;
      _walsh_num_features = std::atoi(optarg);
      break;

    case OPTION_MS_PLANTED_SOLUTION:
      _ms_planted_solution = true;
      break;

    case OPTION_PERIODIC_BOUNDARY_CONDITIONS:
      _periodic_boundary_conditions = true;
      break;

    case OPTION_HELP:
      print_help(std::cerr);
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

void FfgenOptions::print_help(std::ostream& stream) const
{
  stream << "Fitness function generator for HNCO" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "General" << std::endl;
  stream << "  -s, --bv-size (type int, default to 100)" << std::endl;
  stream << "          Size of bit vectors" << std::endl;
  stream << "  -F, --function (type int, default to 1)" << std::endl;
  stream << "          Type of function" << std::endl;
  stream << "            1: Linear function" << std::endl;
  stream << "            60: NK landscape" << std::endl;
  stream << "            70: Max-SAT" << std::endl;
  stream << "            90: Equal products" << std::endl;
  stream << "            91: Partition" << std::endl;
  stream << "            160: Walsh expansion" << std::endl;
  stream << "            161: Walsh expansion of degree 1" << std::endl;
  stream << "            162: Walsh expansion of degree 2" << std::endl;
  stream << "            171: Nearest neighbor Ising model in one dimension" << std::endl;
  stream << "            172: Nearest neighbor Ising model in two dimensions" << std::endl;
  stream << "            190: Sudoku" << std::endl;
  stream << "  -p, --path (type string, default to \"function.txt\")" << std::endl;
  stream << "          Path (relative or absolute) of a function file" << std::endl;
  stream << "      --seed (type int, no default)" << std::endl;
  stream << "          Seed for the random number generator" << std::endl;
  stream << std::endl;
  stream << "Parameters" << std::endl;
  stream << "      --coupling-constant (type double, default to 1)" << std::endl;
  stream << "          Coupling constant" << std::endl;
  stream << "      --field-constant (type double, default to 1)" << std::endl;
  stream << "          Field constant" << std::endl;
  stream << "      --periodic-boundary-conditions" << std::endl;
  stream << "          Periodic boundary conditions" << std::endl;
  stream << "      --stddev (type double, default to 1)" << std::endl;
  stream << "          Standard deviation" << std::endl;
  stream << std::endl;
  stream << "Linear function" << std::endl;
  stream << "      --lin-distance (type double, default to 1)" << std::endl;
  stream << "          Common distance of arithmetic progression" << std::endl;
  stream << "      --lin-generator (type int, default to 0)" << std::endl;
  stream << "          Type of LinearFunction generator" << std::endl;
  stream << "            0: Random coefficients" << std::endl;
  stream << "            1: Arithmetic progression" << std::endl;
  stream << "            2: Geometric progression" << std::endl;
  stream << "      --lin-initial-weight (type double, default to 1)" << std::endl;
  stream << "          Initial weight" << std::endl;
  stream << "      --lin-ratio (type double, default to 2)" << std::endl;
  stream << "          Common ratio of geometric progression" << std::endl;
  stream << std::endl;
  stream << "NK Landscape" << std::endl;
  stream << "      --nk-k (type int, default to 3)" << std::endl;
  stream << "          Each bit is connected to k other bits" << std::endl;
  stream << std::endl;
  stream << "Max-SAT" << std::endl;
  stream << "      --ms-num-clauses (type int, default to 100)" << std::endl;
  stream << "          Number of clauses" << std::endl;
  stream << "      --ms-num-literals-per-clause (type int, default to 3)" << std::endl;
  stream << "          Number of literals per clause" << std::endl;
  stream << "      --ms-planted-solution" << std::endl;
  stream << "          Generate an instance with a planted solution" << std::endl;
  stream << std::endl;
  stream << "Equal Products" << std::endl;
  stream << "      --ep-upper-bound (type double, default to 1)" << std::endl;
  stream << "          Upper bound of numbers" << std::endl;
  stream << std::endl;
  stream << "Partition" << std::endl;
  stream << "      --part-upper-bound (type int, default to 100)" << std::endl;
  stream << "          Upper bound of numbers" << std::endl;
  stream << std::endl;
  stream << "Walsh Expansion" << std::endl;
  stream << "      --walsh-num-features (type int, default to 100)" << std::endl;
  stream << "          Number of features" << std::endl;
  stream << std::endl;
  stream << "Walsh expansion of degree 2" << std::endl;
  stream << "      --walsh2-generator (type int, default to 0)" << std::endl;
  stream << "          Type of WalshExpansion2 generator" << std::endl;
  stream << "            0: Random coefficients" << std::endl;
  stream << "            1: One dimensional Dyson Ising model with long range interactions" << std::endl;
  stream << "      --walsh2-ising-alpha (type double, default to 2)" << std::endl;
  stream << "          Dyson-Ising: exponential decay parameter for long range interactions" << std::endl;
  stream << std::endl;
  stream << "Nearest neighbor Ising model in one dimension" << std::endl;
  stream << "      --nn1-generator (type int, default to 0)" << std::endl;
  stream << "          Type of NearestNeighborIsingModel1 generator" << std::endl;
  stream << "            0: Random coupling, random field" << std::endl;
  stream << "            1: Random coupling, constant field" << std::endl;
  stream << "            2: Constant coupling, random field" << std::endl;
  stream << "            3: Constant coupling, constant field" << std::endl;
  stream << std::endl;
  stream << "Nearest neighbor Ising model in two dimensions" << std::endl;
  stream << "      --nn2-generator (type int, default to 0)" << std::endl;
  stream << "          Type of NearestNeighborIsingModel2 generator" << std::endl;
  stream << "            0: Random coupling, random field" << std::endl;
  stream << "            1: Random coupling, constant field" << std::endl;
  stream << "            2: Constant coupling, random field" << std::endl;
  stream << "            3: Constant coupling, constant field" << std::endl;
  stream << "      --nn2-num-columns (type int, default to 10)" << std::endl;
  stream << "          Number of columns" << std::endl;
  stream << "      --nn2-num-rows (type int, default to 10)" << std::endl;
  stream << "          Number of rows" << std::endl;
  stream << std::endl;
  stream << "Sudoku" << std::endl;
  stream << "      --sudoku-num-empty-cells (type int, default to 10)" << std::endl;
  stream << "          Number of empty cells" << std::endl;
  stream << std::endl;
}

void FfgenOptions::print_version(std::ostream& stream) const
{
  stream << _version << std::endl;
}

std::ostream& hnco::app::operator<<(std::ostream& stream, const FfgenOptions& options)
{
  stream << "# bv_size = " << options._bv_size << std::endl;
  stream << "# coupling_constant = " << options._coupling_constant << std::endl;
  stream << "# ep_upper_bound = " << options._ep_upper_bound << std::endl;
  stream << "# field_constant = " << options._field_constant << std::endl;
  stream << "# function = " << options._function << std::endl;
  stream << "# lin_distance = " << options._lin_distance << std::endl;
  stream << "# lin_generator = " << options._lin_generator << std::endl;
  stream << "# lin_initial_weight = " << options._lin_initial_weight << std::endl;
  stream << "# lin_ratio = " << options._lin_ratio << std::endl;
  stream << "# ms_num_clauses = " << options._ms_num_clauses << std::endl;
  stream << "# ms_num_literals_per_clause = " << options._ms_num_literals_per_clause << std::endl;
  stream << "# nk_k = " << options._nk_k << std::endl;
  stream << "# nn1_generator = " << options._nn1_generator << std::endl;
  stream << "# nn2_generator = " << options._nn2_generator << std::endl;
  stream << "# nn2_num_columns = " << options._nn2_num_columns << std::endl;
  stream << "# nn2_num_rows = " << options._nn2_num_rows << std::endl;
  stream << "# part_upper_bound = " << options._part_upper_bound << std::endl;
  stream << "# path = \"" << options._path << "\"" << std::endl;
  if (options._with_seed)
    stream << "# seed = " << options._seed << std::endl;
  stream << "# stddev = " << options._stddev << std::endl;
  stream << "# sudoku_num_empty_cells = " << options._sudoku_num_empty_cells << std::endl;
  stream << "# walsh2_generator = " << options._walsh2_generator << std::endl;
  stream << "# walsh2_ising_alpha = " << options._walsh2_ising_alpha << std::endl;
  stream << "# walsh_num_features = " << options._walsh_num_features << std::endl;
  if (options._ms_planted_solution)
    stream << "# ms_planted_solution " << std::endl;
  if (options._periodic_boundary_conditions)
    stream << "# periodic_boundary_conditions " << std::endl;
  stream << "# last_parameter" << std::endl;
  stream << "# exec_name = " << options._exec_name << std::endl;
  stream << "# version = " << options._version << std::endl;
  stream << "# Generated from ffgen.json" << std::endl;
  return stream;
}
