/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_MAXSAT_H
#define HNCO_FUNCTIONS_MAXSAT_H

#include <iostream>
#include <vector>

#include "function.hh"


namespace hnco {
namespace function {


  /// MAX-SAT
  class MaxSat:
    public Function {

    /** Expression.

        An expression is represented by a vector of clauses. A clause
        is represented by a vector of literals. A literal is
        represented by a non null integer; if the integer is positive
        then the literal is a variable; if it is negative then it is
        the logical negation of a variable. */
    std::vector<std::vector<int> > _expression;

    /// Number of variables
    size_t _num_variables;

  public:

    /// Default constructor
    MaxSat():
      _num_variables(0) {}

    /** Random instance.

        \param n Size of bit vectors
        \param k Number of literals per clause
        \param c Number of clauses
    */
    void random(int n, int k, int c);

    /** Random instance with satisfiable expression.

        \warning Since the expression is satisfiable, the maximum of
        the function is equal to the number of clauses in the
        expression. However, this information is lost in the save and
        load cycle as the archive format only manages the expression
        itself.

        \param solution Solution
        \param k Number of literals per clause
        \param c Number of clauses
    */
    void random(const bit_vector_t& solution, int k, int c);

    /** Load an instance.
        \throw Error */
    void load(std::istream& stream);

    /// Save an instance
    void save(std::ostream& stream);

    /// Get bit vector size
    size_t get_bv_size() { return _num_variables; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Display the expression
    void display(std::ostream& stream);

  };


} // end of namespace function
} // end of namespace hnco


#endif
