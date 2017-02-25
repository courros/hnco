#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "mapgen-options.hh"

using namespace std;

Options::Options(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.4"),
  _bv_size(100),
  _opt_bv_size(false),
  _map(1),
  _opt_map(false),
  _map_input_size(100),
  _opt_map_input_size(false),
  _path("nopath"),
  _opt_path(false),
  _seed(0),
  _opt_seed(false)
{
  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_BV_SIZE,
    OPTION_MAP,
    OPTION_MAP_INPUT_SIZE,
    OPTION_PATH,
    OPTION_SEED
  };
  const struct option long_options[] = {
    {"bv-size", required_argument, 0, OPTION_BV_SIZE},
    {"map", required_argument, 0, OPTION_MAP},
    {"map-input-size", required_argument, 0, OPTION_MAP_INPUT_SIZE},
    {"path", required_argument, 0, OPTION_PATH},
    {"seed", required_argument, 0, OPTION_SEED},
    {"help", no_argument, 0, OPTION_HELP},
    {"version", no_argument, 0, OPTION_VERSION},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "s:M:p:";
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 's':
    case OPTION_BV_SIZE:
      set_bv_size(atoi(optarg));
      break;

    case 'M':
    case OPTION_MAP:
      set_map(atoi(optarg));
      break;

    case OPTION_MAP_INPUT_SIZE:
      set_map_input_size(atoi(optarg));
      break;

    case 'p':
    case OPTION_PATH:
      set_path(string(optarg));
      break;

    case OPTION_SEED:
      set_seed(atoi(optarg));
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
  stream << "Map generator for HNCO" << endl << endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << endl << endl;
  stream << "General:" << endl;
  stream << "  -s, --bv-size (type int, default to 100)" << endl;
  stream << "          Size of bit vectors" << endl;
  stream << "  -p, --path (type string, default to \"nopath\")" << endl;
  stream << "          Path (relative or absolute) of a map file" << endl;
  stream << "      --seed (type int, default to 0)" << endl;
  stream << "          Seed for the random number generator" << endl;
  stream << endl;
  stream << "Map:" << endl;
  stream << "  -M, --map (type int, default to 1)" << endl;
  stream << "          Type of map" << endl;
  stream << "            1: Translation" << endl;
  stream << "            2: Permutation" << endl;
  stream << "            3: Composition of permutation and translation" << endl;
  stream << "            4: Linear" << endl;
  stream << "            5: Affine" << endl;
  stream << "      --map-input-size (type int, default to 100)" << endl;
  stream << "          Input size of linear and affine maps" << endl;
  stream << endl;
}

void Options::print_version(ostream& stream) const
{
  stream << _version << endl;
}

ostream& operator<<(ostream& stream, const Options& options)
{
  stream << "# bv_size = " << options._bv_size << endl;
  stream << "# map = " << options._map << endl;
  stream << "# map_input_size = " << options._map_input_size << endl;
  stream << "# path = " << options._path << endl;
  stream << "# seed = " << options._seed << endl;
  stream << "# last_parameter" << endl;
  stream << "# exec_name = " << options._exec_name << endl;
  stream << "# version = " << options._version << endl;
  stream << "# Generated from mapgen.json" << endl;
  return stream;
}
