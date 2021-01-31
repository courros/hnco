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

#include <iostream>             // std::endl
#include <algorithm>            // std::max_element

#include "hnco/random.hh"

#include "walsh-expansion.hh"


using namespace hnco::random;
using namespace hnco::function;


double
WalshExpansion::evaluate(const bit_vector_t& x)
{
  double result = 0;
  for (size_t i = 0; i < _terms.size(); i++) {
    if (bv_dot_product(x, _terms[i].feature))
      result -= _terms[i].coefficient;
    else
      result += _terms[i].coefficient;
  }
  return result;
}


void
WalshExpansion::display(std::ostream& stream)
{
  std::vector<function::WalshTerm>::iterator it =
    std::max_element(_terms.begin(),
                     _terms.end(),
                     [](const function::WalshTerm& a,
                        const function::WalshTerm& b){ return bv_hamming_weight(a.feature) < bv_hamming_weight(b.feature); });

  stream
    << "Number of terms = " << _terms.size() << std::endl
    << "Order = " << bv_hamming_weight(it->feature) << std::endl;
}
