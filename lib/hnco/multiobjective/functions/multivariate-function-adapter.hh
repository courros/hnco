/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_MULTIOBJECTIVE_FUNCTIONS_MULTIVARIATE_FUNCTION_ADAPTER_H
#define HNCO_MULTIOBJECTIVE_FUNCTIONS_MULTIVARIATE_FUNCTION_ADAPTER_H

#include <assert.h>

#include <type_traits>          // std::is_same

#include "hnco/representations/all.hh"

#include "function.hh"


namespace hnco {
namespace multiobjective {
namespace function {


/** Multivariate function adapter.

    The purpose of this class is to build a regular hnco function from
    an arbitrary multivariate function. This is achieved using a
    composition:
    - Representations (Rep): hypercube -> domain
    - Multivariate function (Fn): product of domains -> product of codomains
    - Converter (Conv): codomain -> double
*/
template<class Fn, class Rep, class Conv>
class MultivariateFunctionAdapter: public Function {
  static_assert(std::is_same<
                typename Fn::domain_type,
                typename Rep::domain_type
                >::value,
                "multiobjective::function::MultivariateFunctionAdapter: domain types do not match");
  static_assert(std::is_same<
                typename Fn::codomain_type,
                typename Conv::codomain_type
                >::value,
                "multiobjective::function::MultivariateFunctionAdapter: codomain types do not match");

  /// Multivariate function
  Fn *_function;

  /// Representations
  std::vector<Rep> _representations;

  /// Variables
  std::vector<typename Rep::domain_type> _variables;

  /// Codomain value
  std::vector<typename Fn::codomain_type> _codomain_value;

  /// Converter from codomain to double
  Conv _converter;

  /// Unpack a bit vector into variables
  void unpack(const bit_vector_t& bv) {
    int start = 0;
    for (size_t i = 0; i < _variables.size(); i++) {
      _variables[i] = _representations[i].unpack(bv, start);
      start += _representations[i].size();
    }
  }

public:

  /** Constructor.

      \param fn Multivariate function
      \param reps Representations
  */
  MultivariateFunctionAdapter(Fn *fn, std::vector<Rep> reps)
    : _function(fn)
    , _representations(reps)
    , _variables(reps.size())
  {
    assert(fn);
    assert(fn->get_num_variables() == int(reps.size()));

    _codomain_value.resize(_function->get_output_size());
  }

  /** @name Information about the function
   */
  ///@{

  /// Get bit vector size
  int get_bv_size() const override {
    int result = 0;
    for (const auto& rep : _representations)
      result += rep.size();
    return result;
  }

  /// Get output size (number of objectives)
  int get_output_size() const override {
    return _function->get_output_size();
  }

  ///@}


  /** @name Evaluation
   */
  ///@{

  /// Evaluate
  void evaluate(const bit_vector_t& bv, value_t& value) override {
    const int output_size = get_output_size();

    assert(get_bv_size() == int(bv.size()));
    assert(output_size == int(value.size()));

    unpack(bv);
    _function->evaluate(_variables, _codomain_value);
    for (int i = 0; i < output_size; i++)
      value[i] = _converter(_codomain_value[i]);
  }

  ///@}


  /** @name Display
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
    stream << get_bv_size() << " bits" << std::endl;
  }

  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override {
    unpack(bv);
    _function->describe(_variables, stream);
  }

  ///@}

};


} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif