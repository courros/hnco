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

#include "pv-algorithm.hh"


namespace hnco::algorithm {

  /// Max-min ant system
  class Mmas:
    public PvAlgorithm {
  protected:
    /// Candidate solution
    bit_vector_t _x;

  public:

    /// Constructor
    Mmas(int n):
      PvAlgorithm(n),
      _x(n) {}

    /// Learning rate
    double _rate;

    /// Initialization
    void init();

  };

  /// Strict MMAS
  class StrictMmas:
    public Mmas {
    /// Single iteration
    void iterate();

  public:
    /// Constructor
    StrictMmas(int n):
      Mmas(n) {}
  };

  /// Non strict MMAS
  class NonStrictMmas:
    public Mmas {
    /// Single iteration
    void iterate();
  public:
    /// Constructor
    NonStrictMmas(int n):
      Mmas(n) {}
  };

} // end of namespace hnco::algorithm


#endif
