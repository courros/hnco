#ifndef FFGEN_OPTION_H
#define FFGEN_OPTION_H

#include <iostream>
#include <string>

namespace hnco::cli {

/// Command line options for ffgen
class FfgenOptions {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version;

  /// Size of bit vectors
  int _bv_size;
  bool _opt_bv_size;

  /// Coupling constant
  double _coupling_constant;
  bool _opt_coupling_constant;

  /// Upper bound of numbers
  double _ep_upper_bound;
  bool _opt_ep_upper_bound;

  /// Field constant
  double _field_constant;
  bool _opt_field_constant;

  /// Type of function
  int _function;
  bool _opt_function;

  /// Common distance of arithmetic progression
  double _lin_distance;
  bool _opt_lin_distance;

  /// Type of LinearFunction generator
  int _lin_generator;
  bool _opt_lin_generator;

  /// Initial weight
  double _lin_initial_weight;
  bool _opt_lin_initial_weight;

  /// Common ratio of geometric progression
  double _lin_ratio;
  bool _opt_lin_ratio;

  /// Number of clauses
  int _ms_num_clauses;
  bool _opt_ms_num_clauses;

  /// Number of literals per clause
  int _ms_num_literals_per_clause;
  bool _opt_ms_num_literals_per_clause;

  /// Each bit is connected to k other bits
  int _nk_k;
  bool _opt_nk_k;

  /// Type of NearestNeighborIsingModel1 generator
  int _nn1_generator;
  bool _opt_nn1_generator;

  /// Type of NearestNeighborIsingModel2 generator
  int _nn2_generator;
  bool _opt_nn2_generator;

  /// Number of columns
  int _nn2_num_columns;
  bool _opt_nn2_num_columns;

  /// Number of rows
  int _nn2_num_rows;
  bool _opt_nn2_num_rows;

  /// Upper bound of numbers
  int _part_upper_bound;
  bool _opt_part_upper_bound;

  /// Path (relative or absolute) of a function file
  std::string _path;
  bool _opt_path;

  /// Seed for the random number generator
  int _seed;
  bool _opt_seed;

  /// Standard deviation
  double _stddev;
  bool _opt_stddev;

  /// Number of empty cells
  int _sudoku_num_empty_cells;
  bool _opt_sudoku_num_empty_cells;

  /// Type of WalshExpansion2 generator
  int _walsh2_generator;
  bool _opt_walsh2_generator;

  /// Dyson-Ising: exponential decay parameter for long range interactions
  double _walsh2_ising_alpha;
  bool _opt_walsh2_ising_alpha;

  /// Number of features
  int _walsh_num_features;
  bool _opt_walsh_num_features;

  /// Generate an instance with a planted solution
  bool _ms_planted_solution;

  /// Periodic boundary conditions
  bool _periodic_boundary_conditions;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Constructor
  FfgenOptions(int argc, char *argv[]);

  /// Get bv_size
  int get_bv_size() const { return _bv_size; }

  /// Set bv_size
  void set_bv_size(int x) {
    _bv_size = x;
    _opt_bv_size = true;
  }

  /// Get set-flag for bv_size
  bool set_bv_size() const { return _opt_bv_size; }

  /// Get coupling_constant
  double get_coupling_constant() const { return _coupling_constant; }

  /// Set coupling_constant
  void set_coupling_constant(double x) {
    _coupling_constant = x;
    _opt_coupling_constant = true;
  }

  /// Get set-flag for coupling_constant
  bool set_coupling_constant() const { return _opt_coupling_constant; }

  /// Get ep_upper_bound
  double get_ep_upper_bound() const { return _ep_upper_bound; }

  /// Set ep_upper_bound
  void set_ep_upper_bound(double x) {
    _ep_upper_bound = x;
    _opt_ep_upper_bound = true;
  }

  /// Get set-flag for ep_upper_bound
  bool set_ep_upper_bound() const { return _opt_ep_upper_bound; }

  /// Get field_constant
  double get_field_constant() const { return _field_constant; }

  /// Set field_constant
  void set_field_constant(double x) {
    _field_constant = x;
    _opt_field_constant = true;
  }

  /// Get set-flag for field_constant
  bool set_field_constant() const { return _opt_field_constant; }

  /// Get function
  int get_function() const { return _function; }

  /// Set function
  void set_function(int x) {
    _function = x;
    _opt_function = true;
  }

  /// Get set-flag for function
  bool set_function() const { return _opt_function; }

  /// Get lin_distance
  double get_lin_distance() const { return _lin_distance; }

  /// Set lin_distance
  void set_lin_distance(double x) {
    _lin_distance = x;
    _opt_lin_distance = true;
  }

  /// Get set-flag for lin_distance
  bool set_lin_distance() const { return _opt_lin_distance; }

  /// Get lin_generator
  int get_lin_generator() const { return _lin_generator; }

  /// Set lin_generator
  void set_lin_generator(int x) {
    _lin_generator = x;
    _opt_lin_generator = true;
  }

  /// Get set-flag for lin_generator
  bool set_lin_generator() const { return _opt_lin_generator; }

  /// Get lin_initial_weight
  double get_lin_initial_weight() const { return _lin_initial_weight; }

  /// Set lin_initial_weight
  void set_lin_initial_weight(double x) {
    _lin_initial_weight = x;
    _opt_lin_initial_weight = true;
  }

  /// Get set-flag for lin_initial_weight
  bool set_lin_initial_weight() const { return _opt_lin_initial_weight; }

  /// Get lin_ratio
  double get_lin_ratio() const { return _lin_ratio; }

  /// Set lin_ratio
  void set_lin_ratio(double x) {
    _lin_ratio = x;
    _opt_lin_ratio = true;
  }

  /// Get set-flag for lin_ratio
  bool set_lin_ratio() const { return _opt_lin_ratio; }

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

  /// Get nn1_generator
  int get_nn1_generator() const { return _nn1_generator; }

  /// Set nn1_generator
  void set_nn1_generator(int x) {
    _nn1_generator = x;
    _opt_nn1_generator = true;
  }

  /// Get set-flag for nn1_generator
  bool set_nn1_generator() const { return _opt_nn1_generator; }

  /// Get nn2_generator
  int get_nn2_generator() const { return _nn2_generator; }

  /// Set nn2_generator
  void set_nn2_generator(int x) {
    _nn2_generator = x;
    _opt_nn2_generator = true;
  }

  /// Get set-flag for nn2_generator
  bool set_nn2_generator() const { return _opt_nn2_generator; }

  /// Get nn2_num_columns
  int get_nn2_num_columns() const { return _nn2_num_columns; }

  /// Set nn2_num_columns
  void set_nn2_num_columns(int x) {
    _nn2_num_columns = x;
    _opt_nn2_num_columns = true;
  }

  /// Get set-flag for nn2_num_columns
  bool set_nn2_num_columns() const { return _opt_nn2_num_columns; }

  /// Get nn2_num_rows
  int get_nn2_num_rows() const { return _nn2_num_rows; }

  /// Set nn2_num_rows
  void set_nn2_num_rows(int x) {
    _nn2_num_rows = x;
    _opt_nn2_num_rows = true;
  }

  /// Get set-flag for nn2_num_rows
  bool set_nn2_num_rows() const { return _opt_nn2_num_rows; }

  /// Get part_upper_bound
  int get_part_upper_bound() const { return _part_upper_bound; }

  /// Set part_upper_bound
  void set_part_upper_bound(int x) {
    _part_upper_bound = x;
    _opt_part_upper_bound = true;
  }

  /// Get set-flag for part_upper_bound
  bool set_part_upper_bound() const { return _opt_part_upper_bound; }

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

  /// Get sudoku_num_empty_cells
  int get_sudoku_num_empty_cells() const { return _sudoku_num_empty_cells; }

  /// Set sudoku_num_empty_cells
  void set_sudoku_num_empty_cells(int x) {
    _sudoku_num_empty_cells = x;
    _opt_sudoku_num_empty_cells = true;
  }

  /// Get set-flag for sudoku_num_empty_cells
  bool set_sudoku_num_empty_cells() const { return _opt_sudoku_num_empty_cells; }

  /// Get walsh2_generator
  int get_walsh2_generator() const { return _walsh2_generator; }

  /// Set walsh2_generator
  void set_walsh2_generator(int x) {
    _walsh2_generator = x;
    _opt_walsh2_generator = true;
  }

  /// Get set-flag for walsh2_generator
  bool set_walsh2_generator() const { return _opt_walsh2_generator; }

  /// Get walsh2_ising_alpha
  double get_walsh2_ising_alpha() const { return _walsh2_ising_alpha; }

  /// Set walsh2_ising_alpha
  void set_walsh2_ising_alpha(double x) {
    _walsh2_ising_alpha = x;
    _opt_walsh2_ising_alpha = true;
  }

  /// Get set-flag for walsh2_ising_alpha
  bool set_walsh2_ising_alpha() const { return _opt_walsh2_ising_alpha; }

  /// Get walsh_num_features
  int get_walsh_num_features() const { return _walsh_num_features; }

  /// Set walsh_num_features
  void set_walsh_num_features(int x) {
    _walsh_num_features = x;
    _opt_walsh_num_features = true;
  }

  /// Get set-flag for walsh_num_features
  bool set_walsh_num_features() const { return _opt_walsh_num_features; }

  /// Get ms_planted_solution
  bool with_ms_planted_solution() const { return _ms_planted_solution; }

  /// Set ms_planted_solution
  void set_ms_planted_solution() { _ms_planted_solution = true; }

  /// Get periodic_boundary_conditions
  bool with_periodic_boundary_conditions() const { return _periodic_boundary_conditions; }

  /// Set periodic_boundary_conditions
  void set_periodic_boundary_conditions() { _periodic_boundary_conditions = true; }

  friend std::ostream& operator<<(std::ostream&, const FfgenOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const FfgenOptions& options);

}

#endif
