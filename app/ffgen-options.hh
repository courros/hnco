#ifndef FFGEN_OPTIONS_H
#define FFGEN_OPTIONS_H

#include <iostream>
#include <string>

/// Command line options
class Options {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version;

  /// Size of bit vectors
  int _bv_size;
  bool _opt_bv_size;

  /// Parameter upper bound
  double _ep_upper_bound;
  bool _opt_ep_upper_bound;

  /// Type of function
  int _function;
  bool _opt_function;

  /// Number of clauses
  int _ms_num_clauses;
  bool _opt_ms_num_clauses;

  /// Number of literals per clause
  int _ms_num_literals_per_clause;
  bool _opt_ms_num_literals_per_clause;

  /// Each bit is connected to k other bits
  int _nk_k;
  bool _opt_nk_k;

  /// Path (relative or absolute) of a function file
  std::string _path;
  bool _opt_path;

  /// Seed for the random number generator
  int _seed;
  bool _opt_seed;

  /// Standard deviation
  double _stddev;
  bool _opt_stddev;

  /// Number of features
  int _walsh_num_features;
  bool _opt_walsh_num_features;

  /// Periodic boundary condition
  bool _ising_periodic_boundary_condition;

  /// Generate an instance with a planted solution
  bool _ms_planted_solution;

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

  /// Get ep_upper_bound
  double get_ep_upper_bound() const { return _ep_upper_bound; }

  /// Set ep_upper_bound
  void set_ep_upper_bound(double x) {
    _ep_upper_bound = x;
    _opt_ep_upper_bound = true;
  }

  /// Get set-flag for ep_upper_bound
  bool set_ep_upper_bound() const { return _opt_ep_upper_bound; }

  /// Get function
  int get_function() const { return _function; }

  /// Set function
  void set_function(int x) {
    _function = x;
    _opt_function = true;
  }

  /// Get set-flag for function
  bool set_function() const { return _opt_function; }

  /// Get ms_num_clauses
  int get_ms_num_clauses() const { return _ms_num_clauses; }

  /// Set ms_num_clauses
  void set_ms_num_clauses(int x) {
    _ms_num_clauses = x;
    _opt_ms_num_clauses = true;
  }

  /// Get set-flag for ms_num_clauses
  bool set_ms_num_clauses() const { return _opt_ms_num_clauses; }

  /// Get ms_num_literals_per_clause
  int get_ms_num_literals_per_clause() const { return _ms_num_literals_per_clause; }

  /// Set ms_num_literals_per_clause
  void set_ms_num_literals_per_clause(int x) {
    _ms_num_literals_per_clause = x;
    _opt_ms_num_literals_per_clause = true;
  }

  /// Get set-flag for ms_num_literals_per_clause
  bool set_ms_num_literals_per_clause() const { return _opt_ms_num_literals_per_clause; }

  /// Get nk_k
  int get_nk_k() const { return _nk_k; }

  /// Set nk_k
  void set_nk_k(int x) {
    _nk_k = x;
    _opt_nk_k = true;
  }

  /// Get set-flag for nk_k
  bool set_nk_k() const { return _opt_nk_k; }

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

  /// Get stddev
  double get_stddev() const { return _stddev; }

  /// Set stddev
  void set_stddev(double x) {
    _stddev = x;
    _opt_stddev = true;
  }

  /// Get set-flag for stddev
  bool set_stddev() const { return _opt_stddev; }

  /// Get walsh_num_features
  int get_walsh_num_features() const { return _walsh_num_features; }

  /// Set walsh_num_features
  void set_walsh_num_features(int x) {
    _walsh_num_features = x;
    _opt_walsh_num_features = true;
  }

  /// Get set-flag for walsh_num_features
  bool set_walsh_num_features() const { return _opt_walsh_num_features; }

  /// Get ising_periodic_boundary_condition
  bool with_ising_periodic_boundary_condition() const { return _ising_periodic_boundary_condition; }

  /// Set ising_periodic_boundary_condition
  void set_ising_periodic_boundary_condition() { _ising_periodic_boundary_condition = true; }
 
  /// Get ms_planted_solution
  bool with_ms_planted_solution() const { return _ms_planted_solution; }

  /// Set ms_planted_solution
  void set_ms_planted_solution() { _ms_planted_solution = true; }
 
  /// Print a header containing the parameter values
  friend std::ostream& operator<<(std::ostream&, const Options&);
};


#endif
