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

#ifndef HNCO_EXCEPTION_H
#define HNCO_EXCEPTION_H

#include <string>

#include "bit-vector.hh"
#include "algorithms/solution.hh"


namespace hnco {

/// Exceptions
namespace exception {


/// Basic exception
class Exception {};

/// Last evaluation
class LastEvaluation: public Exception {};

/// Maximum reached
class MaximumReached: public Exception {};


/// Solution found
class SolutionFound: public Exception {

protected:

  /// Solution
  algorithm::solution_t _solution;

public:

  /// Constructor
  SolutionFound(const algorithm::solution_t& solution):
    _solution(solution) {}

  /// Get solution
  const algorithm::solution_t& get_solution() const { return _solution; }
};


/// Target reached
class TargetReached: public SolutionFound {

public:

  /// Constructor
  TargetReached(const algorithm::solution_t& solution):
    SolutionFound(solution) {}
};


/// %Error
class Error: public Exception {

protected:

  /// Message
  std::string _what;

public:

  /// Constructor
  Error(): _what("Unknown error") {}

  /// Constructor
  Error(const std::string& s): _what(s) {}

  /// Destructor
  virtual ~Error() {}

  /// Get message
  virtual const char* what() const { return _what.c_str(); }

};


} // end of namespace exception
} // end of namespace hnco


#endif
