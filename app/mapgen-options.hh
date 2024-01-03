#ifndef MAPGEN_OPTIONS_H
#define MAPGEN_OPTIONS_H

#include <iostream>
#include <string>

namespace hnco {
namespace app {

/// Command line options for mapgen
class MapgenOptions {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version = "0.25";

  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_INPUT_SIZE,
    OPTION_MAP,
    OPTION_OUTPUT_SIZE,
    OPTION_PATH,
    OPTION_SEED,
    OPTION_TS_LENGTH,
    OPTION_TS_SAMPLING_MODE,
    OPTION_SURJECTIVE
  };

  /// Input bit vector size
  int _input_size = 100;
  bool _with_input_size = false;

  /// Type of map
  int _map = 1;
  bool _with_map = false;

  /// Output bit vector size
  int _output_size = 100;
  bool _with_output_size = false;

  /// Path (relative or absolute) of a map file
  std::string _path = "map.txt";
  bool _with_path = false;

  /// Seed for the random number generator
  int _seed;
  bool _with_seed = false;

  /// Transvection sequence length
  int _ts_length = 10;
  bool _with_ts_length = false;

  /// Transvection sequence sampling mode
  int _ts_sampling_mode = 0;
  bool _with_ts_sampling_mode = false;

  /// Ensure that the sampled linear or affine map is surjective
  bool _surjective = false;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Default constructor
  MapgenOptions(): _exec_name("mapgen") {}

  /// Constructor
  MapgenOptions(int argc, char *argv[], bool ignore_bad_options = false);

  /// Get the value of input_size
  int get_input_size() const { return _input_size; }

  /// With parameter input_size
  bool with_input_size() const { return _with_input_size; }

  /// Get the value of map
  int get_map() const { return _map; }

  /// With parameter map
  bool with_map() const { return _with_map; }

  /// Get the value of output_size
  int get_output_size() const { return _output_size; }

  /// With parameter output_size
  bool with_output_size() const { return _with_output_size; }

  /// Get the value of path
  std::string get_path() const { return _path; }

  /// With parameter path
  bool with_path() const { return _with_path; }

  /// Get the value of seed
  int get_seed() const {
    if (_with_seed)
      return _seed;
    else
      throw std::runtime_error("MapgenOptions::get_seed: Parameter seed has no default value and has not been set");
    }

  /// With parameter seed
  bool with_seed() const { return _with_seed; }

  /// Get the value of ts_length
  int get_ts_length() const { return _ts_length; }

  /// With parameter ts_length
  bool with_ts_length() const { return _with_ts_length; }

  /// Get the value of ts_sampling_mode
  int get_ts_sampling_mode() const { return _ts_sampling_mode; }

  /// With parameter ts_sampling_mode
  bool with_ts_sampling_mode() const { return _with_ts_sampling_mode; }

  /// With the flag surjective
  bool with_surjective() const { return _surjective; }

  friend std::ostream& operator<<(std::ostream&, const MapgenOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const MapgenOptions& options);

}
}

#endif
