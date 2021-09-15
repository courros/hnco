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

#ifndef HNCO_FUNCTIONS_COLLECTION_QUBO_H
#define HNCO_FUNCTIONS_COLLECTION_QUBO_H

#include <iostream>
#include <fstream>
#include <vector>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** Quadratic unconstrained binary optimization.

    Its expression is of the form \f$ f(x) = \sum_i Q_{ii} x_i +
    \sum_{i < j} Q_{ij} x_i x_j = x^TQx\f$, where Q is an n x n
    upper-triangular matrix.

    Qubo is the problem addressed by qbsolv. Here is its description
    as given on github:

    Qbsolv, a decomposing solver, finds a minimum value of a large
    quadratic unconstrained binary optimization (QUBO) problem by
    splitting it into pieces solved either via a D-Wave system or a
    classical tabu solver.

    There are some differences between WalshExpansion2 and Qubo:

    - WalshExpansion2 maps 0/1 variables into -1/1 variables
    whereas Qubo directly deals with binary variables.

    - Hence, there is a separate linear part in WalshExpansion2
    whereas the linear part in Qubo stems from the diagonal
    elements of the given matrix.

    qbsolv aims at minimizing quadratic functions whereas hnco
    algorithms aim at maximizing them. Hence Qubo::load negates all
    elements so that maximizing the resulting function is equivalent
    to minimizing the original Qubo.

    References:

    Michael Booth, Steven P. Reinhardt, and Aidan
    Roy. 2017. Partitioning Optimization Problems for Hybrid
    Classical/Quantum Execution. Technical Report.  D-Wave.

    https://github.com/dwavesystems/qbsolv

    http://people.brunel.ac.uk/~mastjjb/jeb/orlib/bqpinfo.html

*/
class Qubo: public Function {

private:

  /** Matrix.

      n x n upper triangular matrix.
  */
  std::vector<std::vector<double> > _q;

  /** Load an instance.

      \throw std::runtime_error
  */
  void load(std::istream& stream);

public:

  /// Constructor
  Qubo() {}


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
      throw std::runtime_error("Qubo::load: Cannot open " + path);
    load(stream);
  }

  ///@}

  /// Get bit vector size
  int get_bv_size() const override { return _q.size(); }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


} // end of namespace function
} // end of namespace hnco


#endif
