/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_JUMP_H
#define HNCO_FUNCTIONS_JUMP_H

#include "function.hh"


namespace hnco::function {


  /** Jump.

      Reference:

      H. Mühlenbein and T. Mahnig. 2001. Evolutionary Algorithms: From
      Recombination to Search Distributions. In Theoretical Aspects of
      Evolutionary Computing, Leila Kallel, Bart Naudts, and Alex
      Rogers (Eds.). Springer Berlin Heidelberg, 135–174.

  */
  class Jump:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Gap
    int _gap;

  public:

    /// Constructor
    Jump(int bv_size, int gap);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /** Get the global maximum.
        \return _bv_size */
    double get_maximum() { return _bv_size; }

  };


  /** Deceptive jump.

      This is a jump function with a deceptive gap as defined in
      "Analyzing evolutionary algorithms" by Thomas Jansen, where it
      is called Jump_k. Algorithms in the neighborhood of the
      maximizer (which is the all one bit vector) are taken away from
      it.

      Reference:

      Thomas Jansen. 2013. Analyzing Evolutionary Algorithms. Springer.

  */
  class DeceptiveJump:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Gap
    int _gap;

  public:

    /// Constructor
    DeceptiveJump(int bv_size, int gap);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /** Get the global maximum.
        \return _bv_size + _gap */
    double get_maximum() { return _bv_size + _gap; }

  };

} // end of namespace hnco::function


#endif
