#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "mapgen-options.hh"

using namespace std;

Options::Options(int argc, char *argv[]):
  _exec_name(argv[0]),
  _version("0.10"),
  _input_size(100),
  _opt_input_size(false),
  _map(1),
  _opt_map(false),
  _output_size(100),
  _opt_output_size(false),
  _path("nopath"),
  _opt_path(false),
  _seed(0),
  _opt_seed(false),
  _surjective(false)
{
  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_INPUT_SIZE,
    OPTION_MAP,
    OPTION_OUTPUT_SIZE,
    OPTION_PATH,
    OPTION_SEED,
    OPTION_SURJECTIVE
  };
  const struct option long_options[] = {
    {"input-size", required_argument, 0, OPTION_INPUT_SIZE},
    {"map", required_argument, 0, OPTION_MAP},
    {"output-size", required_argument, 0, OPTION_OUTPUT_SIZE},
    {"path", required_argument, 0, OPTION_PATH},
    {"seed", required_argument, 0, OPTION_SEED},
    {"surjective", no_argument, 0, OPTION_SURJECTIVE},
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "x:M:y:p:";
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 'x':
    case OPTION_INPUT_SIZE:
      set_input_size(atoi(optarg));
      break;

    case 'M':
    case OPTION_MAP:
      set_map(atoi(optarg));
      break;

    case 'y':
    case OPTION_OUTPUT_SIZE:
      set_output_size(atoi(optarg));
      break;

    case 'p':
    case OPTION_PATH:
      set_path(string(optarg));
      break;

    case OPTION_SEED:
      set_seed(atoi(optarg));
      break;

    case OPTION_SURJECTIVE:
      _surjective = true;
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
  stream << "  -x, --input-size (type int, default to 100)" << endl;
  stream << "          Input bit vector size" << endl;
  stream << "  -M, --map (type int, default to 1)" << endl;
  stream << "          Type of map" << endl;
  stream << "            1: Translation" << endl;
  stream << "            2: Permutation" << endl;
  stream << "            3: Composition of permutation and translation" << endl;
  stream << "            4: Linear" << endl;
  stream << "            5: Affine" << endl;
  stream << "  -y, --output-size (type int, default to 100)" << endl;
  stream << "          Output bit vector size" << endl;
  stream << "  -p, --path (type string, default to \"nopath\")" << endl;
  stream << "          Path (relative or absolute) of a map file" << endl;
  stream << "      --seed (type int, default to 0)" << endl;
  stream << "          Seed for the random number generator" << endl;
  stream << "      --surjective" << endl;
  stream << "          Ensure that the sampled linear or affine map is surjective" << endl;
}

void Options::print_version(ostream& stream) const
{
  stream << _version << endl;
}

ostream& operator<<(ostream& stream, const Options& options)
{
  stream << "# input_size = " << options._input_size << endl;
  stream << "# map = " << options._map << endl;
  stream << "# output_size = " << options._output_size << endl;
  stream << "# path = " << options._path << endl;
  stream << "# seed = " << options._seed << endl;
  if (options._surjective)
    stream << "# surjective" << endl;
  stream << "# last_parameter" << endl;
  stream << "# exec_name = " << options._exec_name << endl;
  stream << "# version = " << options._version << endl;
  stream << "# Generated from mapgen.json" << endl;
  return stream;
}
