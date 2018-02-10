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

#ifndef HNCO_ALGORITHMS_PV_MMAS_H
#define HNCO_ALGORITHMS_PV_MMAS_H

#include <functional>           // std::function

#include "pv-algorithm.hh"


namespace hnco::algorithm {


  /** Max-min ant system.

      Reference:

      Thomas Stützle and Holger H. Hoos. 2000. MAX–MIN Ant
      System. Future Generation Computer Systems 16, 8 (2000),
      889–914.

  */
  class Mmas:
    public PvAlgorithm {

  protected:

    /// Candidate solution
    bit_vector_t _x;

    /** @name Parameters
     */
    ///@{

    /// Binary operator for comparing evaluations
    std::function<bool(double, double)> _operator = std::greater_equal<double>();

    /// Learning rate
    double _rate = 1e-3;

    ///@}

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    Mmas(int n):
      PvAlgorithm(n),
      _x(n) {}

    /// Initialization
    void init();

    /** @name Setters
     */
    ///@{

    /// Set the binary operator for comparing evaluations
    void set_operator(std::function<bool(double, double)> x) { _operator = x; }

    /// Set the learning rate
    void set_rate(double x) { _rate = x; }

    ///@}

  };


} // end of namespace hnco::algorithm


#endif
