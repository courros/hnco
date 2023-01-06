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

#ifndef HNCO_FUNCTIONS_COLLECTION_LABS_H
#define HNCO_FUNCTIONS_COLLECTION_LABS_H

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** Low autocorrelation binary sequences.

    Reference:

    S Mertens. 1996. Exhaustive search for low-autocorrelation
    binary sequences.  Journal of Physics A: Mathematical and
    General 29, 18 (1996), L473.

    http://stacks.iop.org/0305-4470/29/i=18/a=005

    If _merit_factor_flag is true then the function returns n / (2 *
    autocorrelation) else it returns -autocorrelation.
*/
class Labs: public Function {

protected:

  /// Binary sequence written using 1 and -1
  std::vector<int> _sequence;

  /// Merit factor flag
  bool _merit_factor_flag = false;

  /// Compute autocorrelation
  double compute_autocorrelation(const bit_vector_t&);

public:

  /// Constructor
  Labs(int n):
    _sequence(n) {}

  /// Set merit factor flag
  void set_merit_factor_flag(bool b) { _merit_factor_flag = b; }

  /// Get bit vector size
  int get_bv_size() const override { return _sequence.size(); }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

};


} // end of namespace function
} // end of namespace hnco


#endif
