#ifndef MAPGEN_OPTIONS_H
#define MAPGEN_OPTIONS_H

#include <iostream>
#include <string>

/// Command line options
class Options {

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

  /// Ensure that the sampled linear or affine map is surjective
  bool _surjective;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  Options(int argc, char *argv[]);

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

  /// Get surjective
  bool with_surjective() const { return _surjective; }

  /// Set surjective
  void set_surjective() { _surjective = true; }
 
  /// Print a header containing the parameter values
  friend std::ostream& operator<<(std::ostream&, const Options&);
};


#endif
