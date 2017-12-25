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

#ifndef HNCO_FUNCTIONS_QUBO_H
#define HNCO_FUNCTIONS_QUBO_H

#include <iostream>
#include <vector>
#include <fstream>

#include "function.hh"


namespace hnco {
namespace function {


  /** Quadratic unconstrained binary optimization.

      Its expression is of the form \f$ f(x) = \sum_i Q_{ii} x_i +
      \sum_{i < j} Q_{ij} x_i x_{ij} = x^TQx\f$, where Q is an n x n
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
  class Qubo:
    public Function {

  private:

    /** Matrix.

        n x n upper triangular matrix.
    */
    std::vector<std::vector<double> > _q;

  public:

    /// Constructor
    Qubo() {}

    /** Load an instance.
        \throw Error */
    void load(std::istream& stream);

    /// Get bit vector size
    size_t get_bv_size() { return _q.size(); }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
