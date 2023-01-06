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

#include <assert.h>

#include "hnco/util.hh"         // square

#include "labs.hh"


using namespace hnco::function;


double
Labs::compute_autocorrelation(const bit_vector_t& x)
{
  assert(x.size() == _sequence.size());

  for (size_t i = 0; i < x.size(); i++)
    if (x[i] == 1)
      _sequence[i] = 1;
    else
      _sequence[i] = -1;

  double E = 0;
  for (size_t k = 1; k < x.size(); k++) {
    double C = 0;
    for (size_t i = 0; i < x.size() - k; i++)
      C += _sequence[i] * _sequence[i + k];
    E += square(C);
  }

  return E;
}


double
Labs::evaluate(const bit_vector_t& x)
{
  assert(x.size() == _sequence.size());

  if (_merit_factor_flag)
    return square(x.size()) / (2 * compute_autocorrelation(x));
  else
    return -compute_autocorrelation(x);
}
