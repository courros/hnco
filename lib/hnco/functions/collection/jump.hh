/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_COLLECTION_JUMP_H
#define HNCO_FUNCTIONS_COLLECTION_JUMP_H

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** %Jump.

    Reference:

    H. Mühlenbein and T. Mahnig. 2001. Evolutionary Algorithms: From
    Recombination to Search Distributions. In Theoretical Aspects of
    Evolutionary Computing, Leila Kallel, Bart Naudts, and Alex
    Rogers (Eds.). Springer Berlin Heidelberg, 135–174.

*/
class Jump:
    public Function {

  /// Bit vector size
  int _bv_size;

  /// Gap
  int _gap;

public:

  /// Constructor
  Jump(int bv_size, int gap);

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return _bv_size */
  double get_maximum() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


/** Deceptive jump.

    This is a jump function with a deceptive gap as defined in
    "Analyzing evolutionary algorithms" by Thomas Jansen, where it
    is called Jump_k. Algorithms in the neighborhood of the
    maximizer (which is the all one bit vector) are taken away from
    it.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class DeceptiveJump:
    public Function {

  /// Bit vector size
  int _bv_size;

  /// Gap
  int _gap;

public:

  /// Constructor
  DeceptiveJump(int bv_size, int gap);

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return _bv_size + _gap */
  double get_maximum() const override { return _bv_size + _gap; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


} // end of namespace function
} // end of namespace hnco


#endif
