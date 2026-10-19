/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026 Arnaud Berny

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

#include "four-peaks.hh"

#include <sstream>              // std::ostringstream

using namespace hnco::function;
using namespace hnco;

FourPeaks::FourPeaks(int bv_size, int threshold):
  _bv_size(bv_size),
  _threshold(threshold),
  _maximum(2 * bv_size - threshold - 1)
{
  if (threshold > bv_size / 2 - 1) {
    std::ostringstream stream;
    stream << "FourPeaks::FourPeaks: _threshold must be <= " << bv_size / 2 - 1;
    throw std::runtime_error(stream.str());
  }
}

void
FourPeaks::display(std::ostream& stream) const
{
  Function::display(stream);
  stream << "FourPeaks" << std::endl;
}

inline int head(const bit_vector_t& bv, bit_t b)
{
  int count = 0;
  bit_vector_t::const_iterator iter = bv.begin();
  while (iter != bv.end() && *iter == b) {
    count++;
    iter++;
  }
  return count;
}

inline int tail(const bit_vector_t& bv, bit_t b)
{
  int count = 0;
  bit_vector_t::const_reverse_iterator iter = bv.rbegin();
  while (iter != bv.rend() && *iter == b) {
    count++;
    iter++;
  }
  return count;
}

double FourPeaks::evaluate(const bit_vector_t& bv)
{
  int h1 = head(bv, 1);
  int t0 = tail(bv, 0);
  int result = std::max(h1, t0);
  if (h1 > _threshold && t0 > _threshold)
    return result + bv.size();
  else
    return result;
}

SixPeaks::SixPeaks(int bv_size, int threshold):
  _bv_size(bv_size),
  _threshold(threshold),
  _maximum(2 * bv_size - threshold - 1)
{
  if (threshold > bv_size / 2 - 1) {
    std::ostringstream stream;
    stream << "SixPeaks::SixPeaks: _threshold must be <= " << bv_size / 2 - 1;
    throw std::runtime_error(stream.str());
  }
}

void
SixPeaks::display(std::ostream& stream) const
{
  Function::display(stream);
  stream << "SixPeaks" << std::endl;
}

double SixPeaks::evaluate(const bit_vector_t& bv)
{
  int h0 = head(bv, 0);
  int h1 = head(bv, 1);
  int t0 = tail(bv, 0);
  int t1 = tail(bv, 1);
  int result = std::max({h0, h1, t0, t1});
  if ((h0 > _threshold && t1 > _threshold) ||
      (h1 > _threshold && t0 > _threshold))
    return result + bv.size();
  else
    return result;
}
