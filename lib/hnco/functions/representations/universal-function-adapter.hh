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

#ifndef HNCO_FUNCTIONS_REPRESENTATIONS_UNVERSAL_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_REPRESENTATIONS_UNVERSAL_FUNCTION_ADAPTER_H

#include <assert.h>

#include <algorithm>            // std::copy

#include "hnco/exception.hh"
#include "hnco/functions/function.hh"

#include "permutation-representation.hh"
#include "representation.hh"


namespace hnco {
namespace function {
namespace representation {


template<class Fn>
class UniversalFunctionAdapter: public Function {
  Fn *_function;

  std::vector<DyadicIntegerRepresentation<int>> _integers_reps;
  std::vector<DyadicRealRepresentation<double>> _real_numbers_reps;
  std::vector<DyadicComplexRepresentation<double>> _complex_numbers_reps;

  bit_vector_t _booleans;
  std::vector<int> _integers;
  std::vector<double> _real_numbers;
  std::vector<std::complex<double>> _complex_numbers;
  std::vector<int> _categorical_values;
  std::vector<permutation_t> _permutations;

  int _bv_size;

  void unpack(const bit_vector_t& bv) {
    assert(int(bv.size()) == _bv_size);
    int start = 0;
    std::copy(bv.begin(), bv.begin() + _booleans.size(), _booleans.begin());
    start += _booleans.size();
    for (size_t i = 0; i < _integers.size(); i++) {
      _integers[i] = _integers_reps[i].unpack(bv, start);
      start += _integers_reps[i].size();
    }
    for (size_t i = 0; i < _real_numbers_reps.size(); i++) {
      _real_numbers[i] = _real_numbers_reps[i].unpack(bv, start);
      start += _real_numbers_reps[i].size();
    }
    for (size_t i = 0; i < _complex_numbers_reps.size(); i++) {
      _complex_numbers[i] = _complex_numbers_reps[i].unpack(bv, start);
      start += _complex_numbers_reps[i].size();
    }
  }

public:
  UniversalFunctionAdapter(Fn *fn, int num_booleans,
                           std::vector<DyadicIntegerRepresentation<int>> integers_reps,
                           std::vector<DyadicRealRepresentation<double>> real_numbers_reps,
                           std::vector<DyadicComplexRepresentation<double>> complex_numbers_reps)
    : _function(fn)
    , _integers_reps(integers_reps)
    , _real_numbers_reps(real_numbers_reps)
    , _complex_numbers_reps(complex_numbers_reps)
  {
    _booleans.resize(num_booleans);
    _integers.resize(_integers_reps.size());
    _real_numbers.resize(_real_numbers_reps.size());
    _complex_numbers.resize(_complex_numbers_reps.size());

    // Compute _bv_size.
    _bv_size = _booleans.size();
    for (auto rep : _integers_reps)
      _bv_size += rep.size();
    for (auto rep : _real_numbers_reps)
      _bv_size += rep.size();
    for (auto rep : _complex_numbers_reps)
      _bv_size += rep.size();
  }
  int get_bv_size() const override { return _bv_size; }
  double evaluate(const bit_vector_t& bv) override {
    unpack(bv);
    return _function->evaluate(_booleans, _integers, _real_numbers, _complex_numbers, _categorical_values, _permutations);
  }
  void display(std::ostream& stream) const override {
    _function->display(stream);
  }
  void describe(const bit_vector_t& bv, std::ostream& stream) override {
    unpack(bv);
    _function->describe(_booleans, _integers, _real_numbers, _complex_numbers, _categorical_values, _permutations, stream);
  }
};


} // end of namespace representation
} // end of namespace function
} // end of namespace hnco


#endif
