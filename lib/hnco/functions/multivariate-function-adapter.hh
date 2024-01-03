/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_MULTIVARIATE_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_MULTIVARIATE_FUNCTION_ADAPTER_H

#include <assert.h>

#include <type_traits>          // std::is_same, std::is_convertible

#include "hnco/representations/all.hh"

#include "function.hh"


namespace hnco {
namespace function {

/**
 * Multivariate function adapter.
 *
 * The purpose of this class is to build a regular hnco function from
 * an arbitrary multivariate function. This is achieved using a
 * composition:
 * - Representations (Rep): hypercube -> domain
 * - Multivariate function (Fn): product of domains -> codomain
 * - Converter (Conv): codomain -> double
 */
template<class Fn, class Rep, class Conv>
class MultivariateFunctionAdapter: public Function {
  static_assert(std::is_convertible<
                typename Rep::domain_type,
                typename Fn::domain_type
                >::value,
                "MultivariateFunctionAdapter: representation domain and function domain types do not match");
  static_assert(std::is_same<
                typename Fn::codomain_type,
                typename Conv::codomain_type
                >::value,
                "MultivariateFunctionAdapter: codomain types do not match");

  /// Multivariate function
  Fn *_function;

  /// Representations
  std::vector<Rep> _representations;

  /// Variables
  std::vector<typename Fn::domain_type> _variables;

  /// Converter from codomain to double
  Conv _converter;

  /// Unpack a bit vector into values
  void unpack(const bit_vector_t& bv) {
    int start = 0;
    for (size_t i = 0; i < _variables.size(); i++) {
      _variables[i] = _representations[i].unpack(bv, start);
      start += _representations[i].size();
    }
  }

public:

  /// %Function type
  using function_type = Fn;

  /// Representation type
  using representation_type = Rep;

  /// Converter type
  using converter_type = Conv;

  /**
   * Constructor.
   * @param fn Multivariate function
   * @param reps Representations
   */
  MultivariateFunctionAdapter(Fn *fn, std::vector<Rep> reps)
    : _function(fn)
    , _representations(reps)
    , _variables(reps.size())
  {
    assert(fn);
    assert(fn->get_num_variables() == int(reps.size()));
  }

  /**
   * @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() const override {
    int result = 0;
    for (const auto& rep : _representations)
      result += rep.size();
    return result;
  }

  ///@}


  /**
   * @name Evaluation
   */
  ///@{

  /// Evaluate
  double evaluate(const bit_vector_t& bv) override {
    assert(int(bv.size()) == get_bv_size());

    unpack(bv);
    return _converter(_function->evaluate(_variables));
  }

  ///@}


  /**
   * @name Display
   */
  ///@{

  /// Display
  void display(std::ostream& stream) const override {
    _function->display(stream);
    stream << "Representations:" << std::endl;
    for (const auto& rep : _representations) {
      rep.display(stream);
      stream << std::endl;
    }
  }

  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override {
    unpack(bv);
    _function->describe(_variables, stream);
  }

  ///@}

};

/**
 * Mixed-integer multivariate function adapter.
 *
 * The purpose of this class is to build a regular hnco function from
 * an arbitrary multivariate function. This is achieved using a
 * composition:
 * - Representations (Rep): hypercube -> domain
 * - Multivariate function (Fn): product of domains -> double
 */
template<typename Fn, typename IntRep, typename FloatRep>
class MixedIntegerMultivariateFunctionAdapter: public Function {
  static_assert(std::is_convertible<
                typename FloatRep::domain_type,
                typename Fn::domain_type
                >::value,
                "MixedIntegerMultivariateFunctionAdapter: representation domain and function domain types do not match");
  static_assert(std::is_convertible<
                typename IntRep::domain_type,
                typename Fn::domain_type
                >::value,
                "MixedIntegerMultivariateFunctionAdapter: representation domain and function domain types do not match");

  /// Multivariate function
  Fn *_function;

  /// Integer representations
  std::vector<IntRep> _int_reps;

  /// Float representations
  std::vector<FloatRep> _float_reps;

  /// Variables
  std::vector<typename Fn::domain_type> _variables;

  /// Lookup table
  std::vector<std::pair<bool, int>> _lut;

  /// Unpack a bit vector into values
  void unpack(const bit_vector_t& bv) {
    int start = 0;
    for (size_t i = 0; i < _variables.size(); i++) {
      auto& p = _lut[i];
      if (p.first) {
        assert(is_in_range(p.second, _int_reps.size()));
        _variables[i] = _int_reps[p.second].unpack(bv, start);
        start += _int_reps[p.second].size();
      } else {
        assert(is_in_range(p.second, _float_reps.size()));
        _variables[i] = _float_reps[p.second].unpack(bv, start);
        start += _float_reps[p.second].size();
      }
    }
  }

public:

  /// %Function type
  using function_type = Fn;

  /// Integer type
  using int_rep_type = IntRep;

  /// Float type
  using float_rep_type = FloatRep;

  /**
   * Constructor.
   * @param fn Multivariate function
   * @param int_reps Integer representations
   * @param float_reps Float representations
   * @param lut Lookup table
   *
   * For each variable, the lookup table tells whether it is an
   * integer or a float, and gives its index in the corresponding
   * representation table, _int_reps or _float_reps.
   */
  MixedIntegerMultivariateFunctionAdapter(Fn *fn,
                                          std::vector<IntRep> int_reps,
                                          std::vector<FloatRep> float_reps,
                                          std::vector<std::pair<bool, int>> lut)
    : _function(fn)
    , _int_reps(int_reps)
    , _float_reps(float_reps)
    , _lut(lut)
  {
    const int total_size = int(int_reps.size() + float_reps.size());

    assert(fn);
    assert(fn->get_num_variables() == total_size);

    _variables.resize(total_size);
    _lut.resize(total_size);
  }

  /**
   * @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() const override {
    int result = 0;
    for (const auto& rep : _int_reps)
      result += rep.size();
    for (const auto& rep : _float_reps)
      result += rep.size();
    return result;
  }

  ///@}


  /**
   * @name Evaluation
   */
  ///@{

  /// Evaluate
  double evaluate(const bit_vector_t& bv) override {
    assert(int(bv.size()) == get_bv_size());

    unpack(bv);
    return _function->evaluate(_variables);
  }

  ///@}


  /**
   * @name Display
   */
  ///@{

  /// Display
  void display(std::ostream& stream) const override {
    _function->display(stream);
    stream << "Representations:" << std::endl;
    for (auto p : _lut) {
      if (p.first)
        _int_reps[p.second].display(stream);
      else
        _float_reps[p.second].display(stream);
      stream << std::endl;
    }
  }

  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override {
    unpack(bv);
    _function->describe(_variables, stream);
  }

  ///@}

};

} // end of namespace function
} // end of namespace hnco


#endif
