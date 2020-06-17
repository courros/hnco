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
#include <math.h>		// fabs, sin

#include <algorithm>		// transform
#include <numeric>		// accumulate, partial_sum

#include "hnco/functions/cancellation.hh"


using namespace hnco::function;


void
SummationCancellation::convert(const bit_vector_t& x)
{
  assert(x.size() >= 9 * _buffer.size());

  for (size_t i = 0; i < _buffer.size(); i++) {
    int offset = 9 * i;
    double tmp;
    if (x[offset] == 0) {	// positive
      int r = 0;
      int base = 1;
      for (int j = offset + 1; j < offset + 9; j++) {
	if (x[j] == 1)
	  r += base;
	base <<= 1;
      }
      assert(0 <= r);
      assert(r <= 255);
      tmp = r / 100.0;
    }
    else {			// negative
      int r = 0;
      int base = 1;
      for (int j = offset + 1; j < offset + 9; j++) {
	if (x[j] == 0)
	  r += base;
	base <<= 1;
      }
      r++;
      assert(1 <= r);
      assert(r <= 256);
      tmp = -r / 100.0;
    }
    _buffer[i] = tmp;
  }
}


double
SummationCancellation::evaluate(const bit_vector_t& x)
{
  convert(x);
  partial_sum(_buffer.begin(), _buffer.end(), _buffer.begin());
  transform(_buffer.begin(), _buffer.end(), _buffer.begin(),
            [](double x){ return fabs(x); });
  return -accumulate(_buffer.begin(), _buffer.end(), 0.0);
}


double
SinusSummationCancellation::evaluate(const bit_vector_t& x)
{
  convert(x);
  for (size_t i = 1; i < _buffer.size(); i++)
    _buffer[i] += sin(_buffer[i - 1]);
  transform(_buffer.begin(), _buffer.end(), _buffer.begin(),
            [](double x){ return fabs(x); });
  return -accumulate(_buffer.begin(), _buffer.end(), 0.0);
}
