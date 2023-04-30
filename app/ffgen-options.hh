#ifndef FFGEN_OPTIONS_H
#define FFGEN_OPTIONS_H

#include <iostream>
#include <string>

namespace hnco {
namespace app {

/// Command line options for ffgen
class FfgenOptions {

  /// Name of the executable
  std::string _exec_name;

  /// Name Version
  std::string _version = "0.23";

  enum {
    OPTION_HELP=256,
    OPTION_VERSION,
    OPTION_BV_SIZE,
    OPTION_COUPLING_CONSTANT,
    OPTION_EP_UPPER_BOUND,
    OPTION_FIELD_CONSTANT,
    OPTION_FUNCTION,
    OPTION_LIN_DISTANCE,
    OPTION_LIN_GENERATOR,
    OPTION_LIN_INITIAL_WEIGHT,
    OPTION_LIN_RATIO,
    OPTION_MS_NUM_CLAUSES,
    OPTION_MS_NUM_LITERALS_PER_CLAUSE,
    OPTION_NK_K,
    OPTION_NN1_GENERATOR,
    OPTION_NN2_GENERATOR,
    OPTION_NN2_NUM_COLUMNS,
    OPTION_NN2_NUM_ROWS,
    OPTION_PART_UPPER_BOUND,
    OPTION_PATH,
    OPTION_SEED,
    OPTION_STDDEV,
    OPTION_SUDOKU_NUM_EMPTY_CELLS,
    OPTION_WALSH2_GENERATOR,
    OPTION_WALSH2_ISING_ALPHA,
    OPTION_WALSH_NUM_FEATURES,
    OPTION_MS_PLANTED_SOLUTION,
    OPTION_PERIODIC_BOUNDARY_CONDITIONS
  };

  /// Size of bit vectors
  int _bv_size = 100;
  bool _with_bv_size = false;

  /// Coupling constant
  double _coupling_constant = 1;
  bool _with_coupling_constant = false;

  /// Upper bound of numbers
  double _ep_upper_bound = 1;
  bool _with_ep_upper_bound = false;

  /// Field constant
  double _field_constant = 1;
  bool _with_field_constant = false;

  /// Type of function
  int _function = 1;
  bool _with_function = false;

  /// Common distance of arithmetic progression
  double _lin_distance = 1;
  bool _with_lin_distance = false;

  /// Type of LinearFunction generator
  int _lin_generator = 0;
  bool _with_lin_generator = false;

  /// Initial weight
  double _lin_initial_weight = 1;
  bool _with_lin_initial_weight = false;

  /// Common ratio of geometric progression
  double _lin_ratio = 2;
  bool _with_lin_ratio = false;

  /// Number of clauses
  int _ms_num_clauses = 100;
  bool _with_ms_num_clauses = false;

  /// Number of literals per clause
  int _ms_num_literals_per_clause = 3;
  bool _with_ms_num_literals_per_clause = false;

  /// Each bit is connected to k other bits
  int _nk_k = 3;
  bool _with_nk_k = false;

  /// Type of NearestNeighborIsingModel1 generator
  int _nn1_generator = 0;
  bool _with_nn1_generator = false;

  /// Type of NearestNeighborIsingModel2 generator
  int _nn2_generator = 0;
  bool _with_nn2_generator = false;

  /// Number of columns
  int _nn2_num_columns = 10;
  bool _with_nn2_num_columns = false;

  /// Number of rows
  int _nn2_num_rows = 10;
  bool _with_nn2_num_rows = false;

  /// Upper bound of numbers
  int _part_upper_bound = 100;
  bool _with_part_upper_bound = false;

  /// Path (relative or absolute) of a function file
  std::string _path = "function.txt";
  bool _with_path = false;

  /// Seed for the random number generator
  int _seed = 0;
  bool _with_seed = false;

  /// Standard deviation
  double _stddev = 1;
  bool _with_stddev = false;

  /// Number of empty cells
  int _sudoku_num_empty_cells = 10;
  bool _with_sudoku_num_empty_cells = false;

  /// Type of WalshExpansion2 generator
  int _walsh2_generator = 0;
  bool _with_walsh2_generator = false;

  /// Dyson-Ising: exponential decay parameter for long range interactions
  double _walsh2_ising_alpha = 2;
  bool _with_walsh2_ising_alpha = false;

  /// Number of features
  int _walsh_num_features = 100;
  bool _with_walsh_num_features = false;

  /// Generate an instance with a planted solution
  bool _ms_planted_solution = false;

  /// Periodic boundary conditions
  bool _periodic_boundary_conditions = false;

  /// Print help message
  void print_help(std::ostream& stream) const;

  /// Print version
  void print_version(std::ostream& stream) const;

public:

  /// Default constructor
  FfgenOptions(): _exec_name("ffgen") {}

  /// Constructor
  FfgenOptions(int argc, char *argv[], bool ignore_bad_options = false);

  /// Get the value of bv_size
  int get_bv_size() const { return _bv_size; }

  /// With parameter bv_size
  bool with_bv_size() const { return _with_bv_size; }

  /// Get the value of coupling_constant
  double get_coupling_constant() const { return _coupling_constant; }

  /// With parameter coupling_constant
  bool with_coupling_constant() const { return _with_coupling_constant; }

  /// Get the value of ep_upper_bound
  double get_ep_upper_bound() const { return _ep_upper_bound; }

  /// With parameter ep_upper_bound
  bool with_ep_upper_bound() const { return _with_ep_upper_bound; }

  /// Get the value of field_constant
  double get_field_constant() const { return _field_constant; }

  /// With parameter field_constant
  bool with_field_constant() const { return _with_field_constant; }

  /// Get the value of function
  int get_function() const { return _function; }

  /// With parameter function
  bool with_function() const { return _with_function; }

  /// Get the value of lin_distance
  double get_lin_distance() const { return _lin_distance; }

  /// With parameter lin_distance
  bool with_lin_distance() const { return _with_lin_distance; }

  /// Get the value of lin_generator
  int get_lin_generator() const { return _lin_generator; }

  /// With parameter lin_generator
  bool with_lin_generator() const { return _with_lin_generator; }

  /// Get the value of lin_initial_weight
  double get_lin_initial_weight() const { return _lin_initial_weight; }

  /// With parameter lin_initial_weight
  bool with_lin_initial_weight() const { return _with_lin_initial_weight; }

  /// Get the value of lin_ratio
  double get_lin_ratio() const { return _lin_ratio; }

  /// With parameter lin_ratio
  bool with_lin_ratio() const { return _with_lin_ratio; }

  /// Get the value of ms_num_clauses
  int get_ms_num_clauses() const { return _ms_num_clauses; }

  /// With parameter ms_num_clauses
  bool with_ms_num_clauses() const { return _with_ms_num_clauses; }

  /// Get the value of ms_num_literals_per_clause
  int get_ms_num_literals_per_clause() const { return _ms_num_literals_per_clause; }

  /// With parameter ms_num_literals_per_clause
  bool with_ms_num_literals_per_clause() const { return _with_ms_num_literals_per_clause; }

  /// Get the value of nk_k
  int get_nk_k() const { return _nk_k; }

  /// With parameter nk_k
  bool with_nk_k() const { return _with_nk_k; }

  /// Get the value of nn1_generator
  int get_nn1_generator() const { return _nn1_generator; }

  /// With parameter nn1_generator
  bool with_nn1_generator() const { return _with_nn1_generator; }

  /// Get the value of nn2_generator
  int get_nn2_generator() const { return _nn2_generator; }

  /// With parameter nn2_generator
  bool with_nn2_generator() const { return _with_nn2_generator; }

  /// Get the value of nn2_num_columns
  int get_nn2_num_columns() const { return _nn2_num_columns; }

  /// With parameter nn2_num_columns
  bool with_nn2_num_columns() const { return _with_nn2_num_columns; }

  /// Get the value of nn2_num_rows
  int get_nn2_num_rows() const { return _nn2_num_rows; }

  /// With parameter nn2_num_rows
  bool with_nn2_num_rows() const { return _with_nn2_num_rows; }

  /// Get the value of part_upper_bound
  int get_part_upper_bound() const { return _part_upper_bound; }

  /// With parameter part_upper_bound
  bool with_part_upper_bound() const { return _with_part_upper_bound; }

  /// Get the value of path
  std::string get_path() const { return _path; }

  /// With parameter path
  bool with_path() const { return _with_path; }

  /// Get the value of seed
  int get_seed() const { return _seed; }

  /// With parameter seed
  bool with_seed() const { return _with_seed; }

  /// Get the value of stddev
  double get_stddev() const { return _stddev; }

  /// With parameter stddev
  bool with_stddev() const { return _with_stddev; }

  /// Get the value of sudoku_num_empty_cells
  int get_sudoku_num_empty_cells() const { return _sudoku_num_empty_cells; }

  /// With parameter sudoku_num_empty_cells
  bool with_sudoku_num_empty_cells() const { return _with_sudoku_num_empty_cells; }

  /// Get the value of walsh2_generator
  int get_walsh2_generator() const { return _walsh2_generator; }

  /// With parameter walsh2_generator
  bool with_walsh2_generator() const { return _with_walsh2_generator; }

  /// Get the value of walsh2_ising_alpha
  double get_walsh2_ising_alpha() const { return _walsh2_ising_alpha; }

  /// With parameter walsh2_ising_alpha
  bool with_walsh2_ising_alpha() const { return _with_walsh2_ising_alpha; }

  /// Get the value of walsh_num_features
  int get_walsh_num_features() const { return _walsh_num_features; }

  /// With parameter walsh_num_features
  bool with_walsh_num_features() const { return _with_walsh_num_features; }

  /// With the flag ms_planted_solution
  bool with_ms_planted_solution() const { return _ms_planted_solution; }

  /// With the flag periodic_boundary_conditions
  bool with_periodic_boundary_conditions() const { return _periodic_boundary_conditions; }

  friend std::ostream& operator<<(std::ostream&, const FfgenOptions&);
};

/// Print a header containing the parameter values
std::ostream& operator<<(std::ostream& stream, const FfgenOptions& options);

}
}

#endif
