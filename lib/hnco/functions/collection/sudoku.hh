/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_COLLECTION_SUDOKU_H
#define HNCO_FUNCTIONS_COLLECTION_SUDOKU_H

#include <iostream>
#include <fstream>              // std:ifstream, std::ofstream
#include <vector>


namespace hnco {
namespace function {


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

  /** @name Load and save instance
   */
  ///@{

  /** Load an instance.

      \throw std::runtime_error
  */
  void load_(std::istream& stream);

  /// Save an instance
  void save_(std::ostream& stream) const;

  ///@}

public:

  /// Default constructor
  Sudoku()
    : _problem_instance(9, std::vector<char>(9))
    , _candidate(9, std::vector<domain_type>(9))
    , _counts(10)
  {}

  /** Random instance.

      \param c Number of empty cells
  */
  void random(int c);

  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) {
    std::ifstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Sudoku::load: Cannot open " + path);
    load_(stream);
  }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const {
    std::ofstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Sudoku::save: Cannot open " + path);
    save_(stream);
  }

  ///@}

  /// Get the number of variables
  int get_num_variables() { return _num_variables; }

  /// Display the problem
  void display(std::ostream& stream);

  /// Describe a solution
  void describe(const std::vector<domain_type>& x, std::ostream& stream);

  /// Evaluate a solution
  double evaluate(const std::vector<domain_type>& x);

};


} // end of namespace function
} // end of namespace hnco


#endif
