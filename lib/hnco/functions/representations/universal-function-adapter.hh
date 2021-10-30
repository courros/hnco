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
#include "hnco/permutation.hh"

#include "permutation-representation.hh"
#include "representation.hh"


namespace hnco {
namespace function {
namespace representation {


template<class Fn>
class UniversalFunctionAdapter: public Function {
  Fn *_function;

  std::vector<DyadicIntegerRepresentation<int>> _integer_reps;
  std::vector<DyadicRealRepresentation<double>> _real_reps;
  std::vector<DyadicComplexRepresentation<double>> _complex_reps;
  std::vector<LinearCategoricalRepresentation> _categorical_reps;
  std::vector<PermutationRepresentation> _permutation_reps;

  bit_vector_t _boolean_vars;
  std::vector<int> _integer_vars;
  std::vector<double> _real_vars;
  std::vector<std::complex<double>> _complex_vars;
  std::vector<int> _categorical_vars;
  std::vector<permutation_t> _permutation_vars;

  int _bv_size;

  void unpack(const bit_vector_t& bv) {
    assert(int(bv.size()) == _bv_size);
    int start = 0;
    std::copy(bv.begin(), bv.begin() + _boolean_vars.size(), _boolean_vars.begin());
    start += _boolean_vars.size();
    for (size_t i = 0; i < _integer_vars.size(); i++) {
      _integer_vars[i] = _integer_reps[i].unpack(bv, start);
      start += _integer_reps[i].size();
    }
    for (size_t i = 0; i < _real_vars.size(); i++) {
      _real_vars[i] = _real_reps[i].unpack(bv, start);
      start += _real_reps[i].size();
    }
    for (size_t i = 0; i < _complex_vars.size(); i++) {
      _complex_vars[i] = _complex_reps[i].unpack(bv, start);
      start += _complex_reps[i].size();
    }
    for (size_t i = 0; i < _categorical_vars.size(); i++) {
      _categorical_vars[i] = _categorical_reps[i].unpack(bv, start);
      start += _categorical_reps[i].size();
    }
    for (size_t i = 0; i < _permutation_vars.size(); i++) {
      _permutation_reps[i].unpack(bv, start, _permutation_vars[i]);
      start += _permutation_reps[i].size();
    }
  }

public:

  /** Constructor.

   */
  UniversalFunctionAdapter(Fn *fn,
                           int num_boolean_vars,
                           std::vector<DyadicIntegerRepresentation<int>> integer_reps,
                           std::vector<DyadicRealRepresentation<double>> real_reps,
                           std::vector<DyadicComplexRepresentation<double>> complex_reps,
                           std::vector<LinearCategoricalRepresentation> categorical_reps,
                           std::vector<PermutationRepresentation> permutation_reps)
    : _function(fn)
    , _integer_reps(integer_reps)
    , _real_reps(real_reps)
    , _complex_reps(complex_reps)
    , _categorical_reps(categorical_reps)
    , _permutation_reps(permutation_reps)
  {
    _boolean_vars.resize(num_boolean_vars);
    _integer_vars.resize(_integer_reps.size());
    _real_vars.resize(_real_reps.size());
    _complex_vars.resize(_complex_reps.size());
    _categorical_vars.resize(_categorical_reps.size());
    _permutation_vars.resize(_permutation_reps.size());
    for (size_t i = 0; i < _permutation_vars.size(); i++) {
      _permutation_vars[i].resize(_permutation_reps[i].get_num_elements());
    }

    // Compute _bv_size.
    _bv_size = _boolean_vars.size();
    for (auto rep : _integer_reps)
      _bv_size += rep.size();
    for (auto rep : _real_reps)
      _bv_size += rep.size();
    for (auto rep : _complex_reps)
      _bv_size += rep.size();
    for (auto rep : _categorical_reps)
      _bv_size += rep.size();
    for (auto rep : _permutation_reps)
      _bv_size += rep.size();
  }

  int get_bv_size() const override { return _bv_size; }
  double evaluate(const bit_vector_t& bv) override {
    unpack(bv);
    return _function->evaluate(_boolean_vars, _integer_vars, _real_vars, _complex_vars, _categorical_vars, _permutation_vars);
  }
  void display(std::ostream& stream) const override {
    _function->display(stream);
  }
  void describe(const bit_vector_t& bv, std::ostream& stream) override {
    unpack(bv);
    stream << "boolean variables:" << std::endl;
    for (size_t i = 0; i < _boolean_vars.size(); i++)
      stream << "boolean[" << i << "] = " << int(_boolean_vars[i]) << std::endl; // does not work without explicit casting
    stream << "--" << std::endl;
    stream << "integer variables:" << std::endl;
    for (size_t i = 0; i < _integer_vars.size(); i++)
      stream << "integer[" << i << "] = " << _integer_vars[i] << std::endl;
    stream << "--" << std::endl;
    stream << "real variables:" << std::endl;
    for (size_t i = 0; i < _real_vars.size(); i++)
      stream << "real[" << i << "] = " << _real_vars[i] << std::endl;
    stream << "--" << std::endl;
    stream << "complex variables:" << std::endl;
    for (size_t i = 0; i < _complex_vars.size(); i++)
      stream << "complex[" << i << "] = " << _complex_vars[i] << std::endl;
    stream << "--" << std::endl;
    stream << "categorical variables:" << std::endl;
    for (size_t i = 0; i < _categorical_vars.size(); i++)
      stream << "categorical[" << i << "] = " << _categorical_vars[i] << std::endl;
    stream << "--" << std::endl;
    stream << "permutation variables:" << std::endl;
    for (size_t i = 0; i < _permutation_vars.size(); i++) {
      stream << "permutation[" << i << "] = ";
      perm_display(_permutation_vars[i], stream);
      stream << std::endl;
    }
    _function->describe(_boolean_vars, _integer_vars, _real_vars, _complex_vars, _categorical_vars, _permutation_vars, stream);
  }
};


} // end of namespace representation
} // end of namespace function
} // end of namespace hnco


#endif
