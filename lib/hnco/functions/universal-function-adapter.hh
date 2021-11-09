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

#ifndef HNCO_FUNCTIONS_UNVERSAL_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_UNVERSAL_FUNCTION_ADAPTER_H

#include <assert.h>

#include <algorithm>            // std::copy

#include "hnco/exception.hh"
#include "hnco/permutation.hh"

#include "function.hh"
#include "representations/all.hh"
#include "universal-function.hh"


namespace hnco {
namespace function {

/** Universal function adapter.

    A universal function adapter turns a universal function into a
    regular hnco function defined on bit vectors.

*/
class UniversalFunctionAdapter: public Function {

  /// Universal function
  UniversalFunction *_function;

  /// Integer representations
  std::vector<representation::DyadicIntegerRepresentation<int>> _integer_reps;

  /// Real representations
  std::vector<representation::DyadicRealRepresentation<double>> _real_reps;

  /// Complex representations
  std::vector<representation::DyadicComplexRepresentation<double>> _complex_reps;

  /// Categorical representations
  std::vector<representation::LinearCategoricalRepresentation> _categorical_reps;

  /// Permuation representations
  std::vector<representation::PermutationRepresentation> _permutation_reps;

  /// Boolean variables
  bit_vector_t _boolean_vars;

  /// Integer variables
  std::vector<int> _integer_vars;

  /// Real variables
  std::vector<double> _real_vars;

  /// Complex variables
  std::vector<std::complex<double>> _complex_vars;

  /// Categorical variables
  std::vector<int> _categorical_vars;

  /// Permutation variables
  std::vector<permutation_t> _permutation_vars;

  /// Bit vector size
  int _bv_size;

  /// Unpack bit vector into variables
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

      \param fn Universal function
      \param num_boolean_vars Number of boolean variables
      \param integer_reps Integer representations
      \param real_reps Real representations
      \param complex_reps Complex representations
      \param categorical_reps Categorical representations
      \param permutation_reps Permutation representations

      Replace reps with {} if there is no corresponding variable. For
      example, if there is no categorical variable,

      UniversalFunctionAdapter(fn, num_boolean_vars, integer_reps, real_reps, complex_reps, {}, permutation_reps)

  */
  UniversalFunctionAdapter(UniversalFunction *fn,
                           int num_boolean_vars,
                           std::vector<representation::DyadicIntegerRepresentation<int>> integer_reps,
                           std::vector<representation::DyadicRealRepresentation<double>> real_reps,
                           std::vector<representation::DyadicComplexRepresentation<double>> complex_reps,
                           std::vector<representation::LinearCategoricalRepresentation> categorical_reps,
                           std::vector<representation::PermutationRepresentation> permutation_reps)
    : _function(fn)
    , _integer_reps(integer_reps)
    , _real_reps(real_reps)
    , _complex_reps(complex_reps)
    , _categorical_reps(categorical_reps)
    , _permutation_reps(permutation_reps)
  {
    assert(num_boolean_vars >= 0);
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

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t& bv) override {
    unpack(bv);
    return _function->evaluate(_boolean_vars, _integer_vars, _real_vars, _complex_vars, _categorical_vars, _permutation_vars);
  }

  /// Display
  void display(std::ostream& stream) const override {
    stream << "boolean variables:" << std::endl;
    stream << bv_domain(_boolean_vars) << std::endl;
    stream << "--" << std::endl;
    stream << "integer variables:" << std::endl;
    for (size_t i = 0; i < _integer_reps.size(); i++) {
      stream << "integer[" << i << "]: ";
      _integer_reps[i].display(stream);
      stream << std::endl;
    }
    stream << "--" << std::endl;
    stream << "real variables:" << std::endl;
    for (size_t i = 0; i < _real_reps.size(); i++) {
      stream << "real[" << i << "]: ";
      _real_reps[i].display(stream);
      stream << std::endl;
    }
    stream << "--" << std::endl;
    stream << "complex variables:" << std::endl;
    for (size_t i = 0; i < _complex_reps.size(); i++) {
      stream << "complex[" << i << "]: ";
      _complex_reps[i].display(stream);
      stream << std::endl;
    }
    stream << "--" << std::endl;
    stream << "categorical variables:" << std::endl;
    for (size_t i = 0; i < _categorical_reps.size(); i++) {
      stream << "categorical[" << i << "]: ";
      _categorical_reps[i].display(stream);
      stream << std::endl;
    }
    stream << "--" << std::endl;
    stream << "permutation variables:" << std::endl;
    for (size_t i = 0; i < _permutation_reps.size(); i++) {
      stream << "permutation[" << i << "]: ";
      _permutation_reps[i].display(stream);
      stream << std::endl;
    }
    _function->display(stream);
  }

  /// Describe a bit vector
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

} // end of namespace function
} // end of namespace hnco


#endif
