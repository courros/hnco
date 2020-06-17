/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include <assert.h>

#include "real-multivariate-function-adapter.hh"


using namespace hnco;
using namespace hnco::function;
using namespace hnco::function::real;


void
RealMultivariateFunctionAdapter::convert(const bit_vector_t& x)
{
  assert(int(x.size()) == get_bv_size());

  hnco::bit_vector_t::const_iterator iter = x.begin();
  for (size_t i = 0; i < _rv.size(); i++) {
    _rv[i] = _representation->convert(iter, iter + _representation->size());
    iter += _representation->size();
  }
}


double
RealMultivariateFunctionAdapter::evaluate(const bit_vector_t& x)
{
  convert(x);
  return _function->evaluate(_rv);
}


void
RealMultivariateFunctionAdapter::describe(const bit_vector_t& x, std::ostream& stream)
{
  convert(x);
  for (size_t i = 0; i < _rv.size(); i++)
    stream << _rv[i] << " ";
  stream << std::endl;
}
