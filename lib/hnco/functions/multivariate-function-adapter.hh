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

#ifndef HNCO_FUNCTIONS_MULTIVARIATE_FUNCTION_ADAPTER_H
#define HNCO_FUNCTIONS_MULTIVARIATE_FUNCTION_ADAPTER_H

#include <assert.h>

#include <type_traits>          // std::is_same

#include "function.hh"
#include "representations/representation.hh"


namespace hnco {
namespace function {

/** Multivariate function adapter.

    The purpose of this class is to build a regular hnco function from
    an arbitrary multivariate function. This is achieved using a
    composition:
    - Representations (Rep): hypercube -> domain
    - Multivariate function (Fn): product of domains -> codomain
    - Converter (Conv): codomain -> double
*/
template<class Fn, class Rep, class Conv>
class MultivariateFunctionAdapter: public Function {
  static_assert(std::is_same<
                typename Fn::domain_type,
                typename Rep::domain_type
                >::value,
                "MultivariateFunctionAdapter: domain types do not match");
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
  std::vector<typename Rep::domain_type> _variables;

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

  ///@}


  /** @name Evaluation
   */
  ///@{

  /// Evaluate
  double evaluate(const bit_vector_t& bv) override {
    assert(get_bv_size() == int(bv.size()));
    unpack(bv);
    return _converter(_function->evaluate(_variables));
  }

  ///@}


  /** @name Display
   */
  ///@{

  /// Display
  void display(std::ostream& stream) const override {
    _function->display(stream);
    stream << _function->get_num_variables() << " variables:" << std::endl;
    for (const auto& rep : _representations)
      rep.display(stream);
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
} // end of namespace hnco


#endif
