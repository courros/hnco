/* Copyright (C) 2016 Arnaud Berny

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

#include <sstream>
#include <string>

#include "hnco/exception.hh"

#include "four-peaks.hh"

using namespace std;
using namespace hnco;
using namespace hnco::function;
using namespace hnco::exception;


FourPeaks::FourPeaks(int bv_size, int threshold):
  _bv_size(bv_size),
  _threshold(threshold),
  _maximum(2 * bv_size - threshold - 1)
{
  if (threshold > bv_size / 2 - 1) {
    ostringstream stream;
    stream << bv_size / 2 - 1;
    throw Error("FourPeaks::FourPeaks: _threshold must be <= " + stream.str());
  }
}

inline int head(const bit_vector_t& v)
{
  int count = 0;
  bit_vector_t::const_iterator iter = v.begin();
  while (iter != v.end() && *iter == 1) {
    count++;
    iter++;
  }
  return count;
}

inline int tail(const bit_vector_t& v)
{
  int count = 0;
  bit_vector_t::const_reverse_iterator iter = v.rbegin();
  while (iter != v.rend() && *iter == 0) {
    count++;
    iter++;
  }
  return count;
}

double FourPeaks::eval(const bit_vector_t& v)
{
  int h = head(v);
  int t = tail(v);
  int tmp = max(t, h);
  if (t > _threshold && h > _threshold)
    tmp += v.size();
  return tmp;
}
