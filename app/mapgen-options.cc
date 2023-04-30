#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "mapgen-options.hh"

using namespace hnco::app;

MapgenOptions::MapgenOptions(int argc, char *argv[], bool ignore_bad_options):
  _exec_name(argv[0])
{
  const struct option long_options[] = {
    {"input-size", required_argument, 0, OPTION_INPUT_SIZE},
    {"map", required_argument, 0, OPTION_MAP},
    {"output-size", required_argument, 0, OPTION_OUTPUT_SIZE},
    {"path", required_argument, 0, OPTION_PATH},
    {"seed", required_argument, 0, OPTION_SEED},
    {"ts-length", required_argument, 0, OPTION_TS_LENGTH},
    {"ts-sampling-mode", required_argument, 0, OPTION_TS_SAMPLING_MODE},
    {"surjective", no_argument, 0, OPTION_SURJECTIVE},
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {0, no_argument, 0, 0}
  };
  const char *short_options = "x:M:y:p:";
  optind = 0;
  while (true) {
    int option = getopt_long(argc, argv, short_options, long_options, 0);
    if (option < 0)
      break;
    switch (option) {
    case 'x':
    case OPTION_INPUT_SIZE:
      _with_input_size = true;
      _input_size = atoi(optarg);
      break;

    case 'M':
    case OPTION_MAP:
      _with_map = true;
      _map = atoi(optarg);
      break;

    case 'y':
    case OPTION_OUTPUT_SIZE:
      _with_output_size = true;
      _output_size = atoi(optarg);
      break;

    case 'p':
    case OPTION_PATH:
      _with_path = true;
      _path = std::string(optarg);
      break;

    case OPTION_SEED:
      _with_seed = true;
      _seed = atoi(optarg);
      break;

    case OPTION_TS_LENGTH:
      _with_ts_length = true;
      _ts_length = atoi(optarg);
      break;

    case OPTION_TS_SAMPLING_MODE:
      _with_ts_sampling_mode = true;
      _ts_sampling_mode = atoi(optarg);
      break;

    case OPTION_SURJECTIVE:
      _surjective = true;
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

void MapgenOptions::print_help(std::ostream& stream) const
{
  stream << "Map generator for HNCO" << std::endl << std::endl;
  stream << "usage: " << _exec_name << " [--help] [--version] [options]" << std::endl << std::endl;
  stream << "  -x, --input-size (type int, default to 100)" << std::endl;
  stream << "          Input bit vector size" << std::endl;
  stream << "  -M, --map (type int, default to 1)" << std::endl;
  stream << "          Type of map" << std::endl;
  stream << "            1: Translation" << std::endl;
  stream << "            2: Permutation" << std::endl;
  stream << "            3: Composition of permutation and translation" << std::endl;
  stream << "            4: Linear" << std::endl;
  stream << "            5: Affine" << std::endl;
  stream << "            6: Affine (transvection sequence)" << std::endl;
  stream << "  -y, --output-size (type int, default to 100)" << std::endl;
  stream << "          Output bit vector size" << std::endl;
  stream << "  -p, --path (type string, default to \"map.dat\")" << std::endl;
  stream << "          Path (relative or absolute) of a map file" << std::endl;
  stream << "      --seed (type int, no default)" << std::endl;
  stream << "          Seed for the random number generator" << std::endl;
  stream << "      --surjective" << std::endl;
  stream << "          Ensure that the sampled linear or affine map is surjective" << std::endl;
  stream << "      --ts-length (type int, default to 10)" << std::endl;
  stream << "          Transvection sequence length" << std::endl;
  stream << "      --ts-sampling-mode (type int, default to 0)" << std::endl;
  stream << "          Transvection sequence sampling mode" << std::endl;
  stream << "            0: Unconstrained" << std::endl;
  stream << "            1: Commuting transvections" << std::endl;
  stream << "            2: Unique source" << std::endl;
  stream << "            3: Unique destination" << std::endl;
  stream << "            4: Disjoint transvections" << std::endl;
  stream << "            5: Non commuting transvections" << std::endl;
}

void MapgenOptions::print_version(std::ostream& stream) const
{
  stream << _version << std::endl;
}

std::ostream& hnco::app::operator<<(std::ostream& stream, const MapgenOptions& options)
{
  stream << "# input_size = " << options._input_size << std::endl;
  stream << "# map = " << options._map << std::endl;
  stream << "# output_size = " << options._output_size << std::endl;
  stream << "# path = \"" << options._path << "\"" << std::endl;
  if (options._with_seed)
    stream << "# seed = " << options._seed << std::endl;
  stream << "# ts_length = " << options._ts_length << std::endl;
  stream << "# ts_sampling_mode = " << options._ts_sampling_mode << std::endl;
  if (options._surjective)
    stream << "# surjective " << std::endl;
  stream << "# last_parameter" << std::endl;
  stream << "# exec_name = " << options._exec_name << std::endl;
  stream << "# version = " << options._version << std::endl;
  stream << "# Generated from mapgen.json" << std::endl;
  return stream;
}
