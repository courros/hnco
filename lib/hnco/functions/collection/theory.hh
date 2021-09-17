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

#ifndef HNCO_FUNCTIONS_COLLECTION_THEORY_H
#define HNCO_FUNCTIONS_COLLECTION_THEORY_H

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {

/** %OneMax.

    References:

    Heinz Mühlenbein, "How genetic algorithms really work:
    I. mutation and hillclimbing", in Proc. 2nd Int. Conf. on
    Parallel Problem Solving from Nature, 1992

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class OneMax: public Function {

  /// Bit vector size
  int _bv_size;

public:

  /// Constructor
  OneMax(int bv_size):
    _bv_size(bv_size) {}

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /** Get the global maximum.
      \return _bv_size */
  double get_maximum() const override { return _bv_size; }

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Check whether the function provides incremental evaluation.
      \return true
  */
  bool provides_incremental_evaluation() const override { return true; }

  /// Display
  void display(std::ostream& stream) const override { stream << "OneMax" << std::endl; }

  ///@}


  /** @name Evaluation
   */
  ///@{

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /// Incrementally evaluate a bit vector
  double evaluate_incrementally(const bit_vector_t& x, double v, const hnco::sparse_bit_vector_t& flipped_bits) override;

  ///@}

};


/** Leading ones.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class LeadingOnes: public Function {

  /// Bit vector size
  int _bv_size;

public:

  /// Constructor
  LeadingOnes(int bv_size):
    _bv_size(bv_size) {}

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return _bv_size */
  double get_maximum() const override { return _bv_size; }

};


/** %Needle in a haystack.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class Needle: public Function {

  /// Bit vector size
  int _bv_size;

public:

  /// Constructor
  Needle(int bv_size):
    _bv_size(bv_size) {}

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return 1 */
  double get_maximum() const override { return 1; }

};


/** Hierarchical if and only if.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class Hiff: public Function {

  /// Bit vector size
  int _bv_size;

  /// Tree depth
  int _depth;

public:

  /// Constructor
  Hiff(int bv_size);

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return (i + 1) * 2^i where 2^i = _bv_size */
  double get_maximum() const override { return (_depth + 1) * _bv_size; }

};


/** %Ridge.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class Ridge: public Function {

  /// Bit vector size
  int _bv_size;

public:

  /// Constructor
  Ridge(int bv_size):
    _bv_size(bv_size) {}

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return 2 * _bv_size */
  double get_maximum() const override { return 2 * _bv_size; }

};


/** %Plateau.

    Reference:

    Thomas Jansen, Analyzing Evolutionary Algorithms. Springer, 2013.

*/
class Plateau: public Function {

  /// Bit vector size
  int _bv_size;

public:

  /// Constructor
  Plateau(int bv_size):
    _bv_size(bv_size) {}

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /** Check for a known maximum.
      \return true */
  bool has_known_maximum() const override { return true; }

  /** Get the global maximum.
      \return _bv_size + 2 */
  double get_maximum() const override { return _bv_size + 2; }

};


} // end of namespace function
} // end of namespace hnco


#endif
