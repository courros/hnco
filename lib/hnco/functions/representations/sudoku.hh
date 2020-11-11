/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

   This file is part of HNCO.

   HNCO is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   HNCO is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
   Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with HNCO. If not, see
   <http://www.gnu.org/licenses/>.

*/

#ifndef HNCO_FUNCTIONS_REPRESENTATIONS_SUDOKU_H
#define HNCO_FUNCTIONS_REPRESENTATIONS_SUDOKU_H

#include <iostream>
#include <vector>


namespace hnco {
namespace function {
namespace representation {


/// Sudoku
class Sudoku {

public:

  /// Domain type
  typedef std::size_t domain_type;

  /// Codomain type
  typedef double codomain_type;

private:

  /// Problem instance
  std::vector<std::vector<char>> _problem_instance;

  /// Candidate
  std::vector<std::vector<domain_type>> _candidate;

  /// Counts
  std::vector<int> _counts;

  /// Number of variables
  int _num_variables;

  /// Write variables
  void write_variables(const std::vector<domain_type>& x);

public:

  /// Default constructor
  Sudoku()
    : _problem_instance(9, std::vector<char>(9))
    , _candidate(9, std::vector<domain_type>(9))
    , _counts(10)
  {}

  /** Load an instance.
      \throw Error */
  void load(std::istream& stream);

  /// Save an instance
  void save(std::ostream& stream) const;

  /** Random instance.

      \param c Number of empty cells
  */
  void random(int c);

  /// Display the problem
  void display(std::ostream& stream);

  /// Evaluate a solution
  double evaluate(const std::vector<domain_type>& x);

  /// Describe a solution
  void describe(const std::vector<domain_type>& x, std::ostream& stream);

  /// Get the number of variables
  int get_num_variables() { return _num_variables; }

};


} // end of namespace representation
} // end of namespace function
} // end of namespace hnco


#endif
