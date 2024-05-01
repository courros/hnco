/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_COLLECTION_MAXSAT_H
#define HNCO_FUNCTIONS_COLLECTION_MAXSAT_H

#include <iosfwd>

#include <fstream>              // std:ifstream, std::ofstream
#include <vector>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/// Abstract class for MaxSat-like functions
class AbstractMaxSat: public Function {

protected:

  /** Expression.

      An expression is represented by a vector of clauses. A clause
      is represented by a vector of literals. A literal is
      represented by a non null integer; if the integer is positive
      then the literal is a variable; if it is negative then it is
      the logical negation of a variable. */
  std::vector<std::vector<int> > _expression;

  /// Number of variables
  int _num_variables;

  /** Load an instance.

      \param stream Input stream
      \throw std::runtime_error
  */
  void load_(std::istream& stream);

  /** Save an instance.

      \param stream Outputstream
  */
  void save_(std::ostream& stream) const;

public:

  /// Default constructor
  AbstractMaxSat():
    _num_variables(0) {}

  /// Get bit vector size
  int get_bv_size() const override { return _num_variables; }

  /// Display the expression
  void display(std::ostream& stream) const override;


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
      throw std::runtime_error("AbstractMaxSat::load: Cannot open " + path);
    load_(stream);
  }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const {
    std::ofstream stream(path);
    if (!stream.good())
      throw std::runtime_error("LinearFunction::save: Cannot open " + path);
    save_(stream);
  }

  ///@}

};


/** MAX-SAT.

    Reference:

    Christos M. Papadimitriou. 1994. Computational
    complexity. Addison-Wesley, Reading, Massachusetts.

*/
class MaxSat: public AbstractMaxSat {

public:

  /// Default constructor
  MaxSat() {}

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

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


/** Max not-all-equal 3SAT.

    Reference:

    Christos M. Papadimitriou. 1994. Computational
    complexity. Addison-Wesley, Reading, Massachusetts.

*/
class MaxNae3Sat: public AbstractMaxSat {

public:

  /// Default constructor
  MaxNae3Sat() {}

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) {
    AbstractMaxSat::load(path);
    for (auto& clause: _expression)
      if (clause.size() != 3)
        throw std::runtime_error("MaxNae3Sat::load: All clauses must have exactly 3 literals");
  }

};


} // end of namespace function
} // end of namespace hnco


#endif
