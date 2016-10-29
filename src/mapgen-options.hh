#ifndef MAPGEN_OPTIONS_H
#define MAPGEN_OPTIONS_H

#include <iostream>
#include <string>

/// Command line options
class Options {

  /// Name of the executable
  std::string _exec_name;

  /// Version
  std::string _version;

  /// Size of bit vectors
  int _bv_size;
  bool _opt_bv_size;

  /// Type of map
  int _map;
  bool _opt_map;

  /// Input size of linear and affine maps
  int _map_input_size;
  bool _opt_map_input_size;

  /// Path (relative or absolute) of a map file
  std::string _path;
  bool _opt_path;

  /// Seed for the random number generator
  int _seed;
  bool _opt_seed;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  Options(int argc, char *argv[]);

  /// Get bv_size
  int get_bv_size() const { return _bv_size; }

  /// Set bv_size
  void set_bv_size(int x) {
    _bv_size = x;
    _opt_bv_size = true;
  }

  /// Get set-flag for bv_size
  bool set_bv_size() const { return _opt_bv_size; }

  /// Get map
  int get_map() const { return _map; }

  /// Set map
  void set_map(int x) {
    _map = x;
    _opt_map = true;
  }

  /// Get set-flag for map
  bool set_map() const { return _opt_map; }

  /// Get map_input_size
  int get_map_input_size() const { return _map_input_size; }

  /// Set map_input_size
  void set_map_input_size(int x) {
    _map_input_size = x;
    _opt_map_input_size = true;
  }

  /// Get set-flag for map_input_size
  bool set_map_input_size() const { return _opt_map_input_size; }

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

  /// Print a header containing the parameter values
  friend std::ostream& operator<<(std::ostream&, const Options&);
};


#endif
