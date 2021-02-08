#ifndef MAPGEN_OPTION_H
#define MAPGEN_OPTION_H

#include <iostream>
#include <string>

namespace hnco::cli {

/// Command line options for mapgen
class MapgenOptions {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version;

  /// Input bit vector size
  int _input_size;
  bool _opt_input_size;

  /// Type of map
  int _map;
  bool _opt_map;

  /// Output bit vector size
  int _output_size;
  bool _opt_output_size;

  /// Path (relative or absolute) of a map file
  std::string _path;
  bool _opt_path;

  /// Seed for the random number generator
  int _seed;
  bool _opt_seed;

  /// Transvection sequence length
  int _ts_length;
  bool _opt_ts_length;

  /// Transvection sequence sampling mode
  int _ts_sampling_mode;
  bool _opt_ts_sampling_mode;

  /// Ensure that the sampled linear or affine map is surjective
  bool _surjective;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  MapgenOptions(int argc, char *argv[]);

  /// Get input_size
  int get_input_size() const { return _input_size; }

  /// Set input_size
  void set_input_size(int x) {
    _input_size = x;
    _opt_input_size = true;
  }

  /// Get set-flag for input_size
  bool set_input_size() const { return _opt_input_size; }

  /// Get map
  int get_map() const { return _map; }

  /// Set map
  void set_map(int x) {
    _map = x;
    _opt_map = true;
  }

  /// Get set-flag for map
  bool set_map() const { return _opt_map; }

  /// Get output_size
  int get_output_size() const { return _output_size; }

  /// Set output_size
  void set_output_size(int x) {
    _output_size = x;
    _opt_output_size = true;
  }

  /// Get set-flag for output_size
  bool set_output_size() const { return _opt_output_size; }

  /// Get path
  std::string get_path() const { return _path; }

  /// Set path
  void set_path(std::string x) {
    _path = x;
    _opt_path = true;
  }

  /// Get set-flag for path
  bool set_path() const { return _opt_path; }

  /// Get seed
  int get_seed() const { return _seed; }

  /// Set seed
  void set_seed(int x) {
    _seed = x;
    _opt_seed = true;
  }

  /// Get set-flag for seed
  bool set_seed() const { return _opt_seed; }

  /// Get ts_length
  int get_ts_length() const { return _ts_length; }

  /// Set ts_length
  void set_ts_length(int x) {
    _ts_length = x;
    _opt_ts_length = true;
  }

  /// Get set-flag for ts_length
  bool set_ts_length() const { return _opt_ts_length; }

  /// Get ts_sampling_mode
  int get_ts_sampling_mode() const { return _ts_sampling_mode; }

  /// Set ts_sampling_mode
  void set_ts_sampling_mode(int x) {
    _ts_sampling_mode = x;
    _opt_ts_sampling_mode = true;
  }

  /// Get set-flag for ts_sampling_mode
  bool set_ts_sampling_mode() const { return _opt_ts_sampling_mode; }

  /// Get surjective
  bool with_surjective() const { return _surjective; }

  /// Set surjective
  void set_surjective() { _surjective = true; }

  friend std::ostream& operator<<(std::ostream&, const MapgenOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const MapgenOptions& options);

}

#endif
