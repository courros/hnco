#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "ffgen-options.hh"

using namespace std;

Options::Options(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.6"),
  _bv_size(100),
  _opt_bv_size(false),
  _ep_upper_bound(1),
  _opt_ep_upper_bound(false),
  _function(1),
  _opt_function(false),
  _ms_num_clauses(100),
  _opt_ms_num_clauses(false),
  _ms_num_literals_per_clause(3),
  _opt_ms_num_literals_per_clause(false),
  _nk_k(3),
  _opt_nk_k(false),
  _nk_stddev(1),
  _opt_nk_stddev(false),
  _path("nopath"),
  _opt_path(false),
  _quad_stddev_lin(1),
  _opt_quad_stddev_lin(false),
  _quad_stddev_quad(1),
  _opt_quad_stddev_quad(false),
  _seed(0),
  _opt_seed(false),
  _ms_known_maximum(false)
{
  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_BV_SIZE,
    OPTION_EP_UPPER_BOUND,
    OPTION_FUNCTION,
    OPTION_MS_NUM_CLAUSES,
    OPTION_MS_NUM_LITERALS_PER_CLAUSE,
    OPTION_NK_K,
    OPTION_NK_STDDEV,
    OPTION_PATH,
    OPTION_QUAD_STDDEV_LIN,
    OPTION_QUAD_STDDEV_QUAD,
    OPTION_SEED,
    OPTION_MS_KNOWN_MAXIMUM
  };
  const struct option long_options[] = {
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"ep-upper-bound", required_argument, 0, OPTION_EP_UPPER_BOUND},
    {"function", required_argument, 0, OPTION_FUNCTION},
    {"ms-num-clauses", required_argument, 0, OPTION_MS_NUM_CLAUSES},
    {"ms-num-literals-per-clause", required_argument, 0, OPTION_MS_NUM_LITERALS_PER_CLAUSE},
    {"nk-k", required_argument, 0, OPTION_NK_K},
    {"nk-stddev", required_argument, 0, OPTION_NK_STDDEV},
    {"path", required_argument, 0, OPTION_PATH},
    {"quad-stddev-lin", required_argument, 0, OPTION_QUAD_STDDEV_LIN},
    {"quad-stddev-quad", required_argument, 0, OPTION_QUAD_STDDEV_QUAD},
    {"seed", required_argument, 0, OPTION_SEED},
    {"ms-known-maximum", no_argument, 0, OPTION_MS_KNOWN_MAXIMUM},
    {"help", no_argument, 0, OPTION_HELP},
    {"version", no_argument, 0, OPTION_VERSION},
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

    case OPTION_EP_UPPER_BOUND:
      set_ep_upper_bound(atof(optarg));
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

    case OPTION_NK_STDDEV:
      set_nk_stddev(atof(optarg));
      break;

    case 'p':
    case OPTION_PATH:
      set_path(string(optarg));
      break;

    case OPTION_QUAD_STDDEV_LIN:
      set_quad_stddev_lin(atof(optarg));
      break;

    case OPTION_QUAD_STDDEV_QUAD:
      set_quad_stddev_quad(atof(optarg));
      break;

    case OPTION_SEED:
      set_seed(atoi(optarg));
      break;

    case OPTION_MS_KNOWN_MAXIMUM:
      _ms_known_maximum = true;
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
  stream << "General:" << endl;
  stream << "  -s, --bv-size (type int, default to 100)" << endl;
  stream << "          Size of bit vectors" << endl;
  stream << "  -F, --function (type int, default to 1)" << endl;
  stream << "          Type of function" << endl;
  stream << "            1: Linear function" << endl;
  stream << "            50: Walsh expansion of degree 2" << endl;
  stream << "            60: NK landscape" << endl;
  stream << "            70: Max-SAT" << endl;
  stream << "            90: Equal products" << endl;
  stream << "  -p, --path (type string, default to \"nopath\")" << endl;
  stream << "          Path (relative or absolute) of a function file" << endl;
  stream << "      --seed (type int, default to 0)" << endl;
  stream << "          Seed for the random number generator" << endl;
  stream << endl;
  stream << "WalshExpansion:" << endl;
  stream << "      --quad-stddev-lin (type double, default to 1)" << endl;
  stream << "          Standard deviation of linear part" << endl;
  stream << "      --quad-stddev-quad (type double, default to 1)" << endl;
  stream << "          Standard deviation of quadratic part" << endl;
  stream << endl;
  stream << "NkLandscape:" << endl;
  stream << "      --nk-k (type int, default to 3)" << endl;
  stream << "          Each bit is connected to k other bits" << endl;
  stream << "      --nk-stddev (type double, default to 1)" << endl;
  stream << "          Standard deviation of partial values" << endl;
  stream << endl;
  stream << "MaxSat:" << endl;
  stream << "      --ms-known-maximum" << endl;
  stream << "          Generate an instance with known maximum" << endl;
  stream << "      --ms-num-clauses (type int, default to 100)" << endl;
  stream << "          Number of clauses" << endl;
  stream << "      --ms-num-literals-per-clause (type int, default to 3)" << endl;
  stream << "          Number of literals per clause" << endl;
  stream << endl;
  stream << "EqualProducts:" << endl;
  stream << "      --ep-upper-bound (type double, default to 1)" << endl;
  stream << "          Parameter upper bound" << endl;
  stream << endl;
}

void Options::print_version(ostream& stream) const
{
  stream << _version << endl;
}

ostream& operator<<(ostream& stream, const Options& options)
{
  stream << "# bv_size = " << options._bv_size << endl;
  stream << "# ep_upper_bound = " << options._ep_upper_bound << endl;
  stream << "# function = " << options._function << endl;
  stream << "# ms_num_clauses = " << options._ms_num_clauses << endl;
  stream << "# ms_num_literals_per_clause = " << options._ms_num_literals_per_clause << endl;
  stream << "# nk_k = " << options._nk_k << endl;
  stream << "# nk_stddev = " << options._nk_stddev << endl;
  stream << "# path = " << options._path << endl;
  stream << "# quad_stddev_lin = " << options._quad_stddev_lin << endl;
  stream << "# quad_stddev_quad = " << options._quad_stddev_quad << endl;
  stream << "# seed = " << options._seed << endl;
  if (options._ms_known_maximum)
    stream << "# ms_known_maximum" << endl;
  stream << "# last_parameter" << endl;
  stream << "# exec_name = " << options._exec_name << endl;
  stream << "# version = " << options._version << endl;
  stream << "# Generated from ffgen.json" << endl;
  return stream;
}
