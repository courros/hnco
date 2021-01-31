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

#include <assert.h>

#include <string>               // std::getline

#include "util.hh"              // hnco::have_same_size
#include "bit-vector.hh"


using namespace hnco;


void check_libhnco(void) {}


void
hnco::bv_display(const bit_vector_t& v, std::ostream& stream)
{
  for (auto c : v)
    if (c == 0)
      stream << 0;
    else
      stream << 1;
}

int
hnco::bv_hamming_weight(const std::vector<bool>& x)
{
  int result = 0;
  for (std::size_t i = 0; i < x.size(); i++)
    if (x[i])
      result++;
  return result;
}

int
hnco::bv_hamming_distance(const bit_vector_t& x, const bit_vector_t& y)
{
  assert(x.size() == y.size());

  int d = 0;
  for (std::size_t i = 0; i < x.size(); i++)
    if (x[i] != y[i])
      d++;
  return d;
}

bit_t
hnco::bv_dot_product(const bit_vector_t& x, const bit_vector_t& y)
{
  assert(x.size() == y.size());

  size_t result = 0;
  for (std::size_t i = 0; i < x.size(); i++)
    if (x[i] && y[i])
      result++;
  return result % 2;
}

bit_t
hnco::bv_dot_product(const bit_vector_t& x, const std::vector<bool>& y)
{
  assert(x.size() == y.size());

  size_t result = 0;
  for (std::size_t i = 0; i < x.size(); i++)
    if (x[i] && y[i])
      result++;
  return result % 2;
}

void
hnco::bv_flip(bit_vector_t& x, const bit_vector_t& mask)
{
  assert(mask.size() == x.size());

  for (std::size_t i = 0; i < x.size(); i++)
    if (mask[i])
      bv_flip(x, i);
}

void
hnco::bv_random(bit_vector_t& x, int k)
{
  assert(k >= 0);
  assert(k <= static_cast<int>(x.size()));

  int n = x.size();
  for (size_t i = 0; i < x.size(); i++) {
    if (k > 0) {
      assert(n > 0);
      double p = double(k) / double(n);
      if (random::Generator::uniform() < p) {
        x[i] = 1;
        k--;
      } else
        x[i] = 0;
      n--;
    } else
      x[i] = 0;
  }
}

void
hnco::bv_add(bit_vector_t& dest, const bit_vector_t& src)
{
  assert(dest.size() == src.size());

  for (std::size_t i = 0; i < dest.size(); i++)
    dest[i] = (src[i] + dest[i]) % 2;
}

void
hnco::bv_add(bit_vector_t& dest, const bit_vector_t& x, const bit_vector_t& y)
{
  assert(dest.size() == x.size());
  assert(dest.size() == y.size());

  for (std::size_t i = 0; i < dest.size(); i++)
    dest[i] = (x[i] + y[i]) % 2;
}

void
hnco::bv_to_vector_bool(std::vector<bool>& y, const bit_vector_t& x)
{
  assert(y.size() == x.size());

  for (size_t i = 0; i < x.size(); i++) {
    if (x[i])
      y[i] = true;
    else
      y[i] = false;
  }
}

void
hnco::bv_from_vector_bool(bit_vector_t& x, const std::vector<bool>& y)
{
  assert(y.size() == x.size());

  for (size_t i = 0; i < x.size(); i++) {
    if (y[i])
      x[i] = 1;
    else
      x[i] = 0;
  }
}

std::size_t
hnco::bv_to_size_type(const bit_vector_t& x)
{
  assert(x.size() <= 8 * sizeof(std::size_t));

  std::size_t result = 0;
  std::size_t mask = 1;
  for (size_t i = 0; i < x.size(); i++) {
    if (x[i])
      result |= mask;
    mask <<= 1;
  }
  return result;
}

std::size_t
hnco::bv_to_size_type(const bit_vector_t& x, int start, int stop)
{
  assert(hnco::is_in_range(start, x.size()));
  assert(hnco::is_in_range(stop, start + 1, x.size() + 1));
  assert((stop - start) <= int(8 * sizeof(std::size_t)));

  std::size_t result = 0;
  std::size_t mask = 1;
  for (int i = start; i < stop; i++) {
    if (x[i])
      result |= mask;
    mask <<= 1;
  }
  return result;
}

void
hnco::bv_from_size_type(bit_vector_t& x, std::size_t u)
{
  assert(x.size() <= 8 * sizeof(std::size_t));

  std::size_t mask = 1;
  for (size_t i = 0; i < x.size(); i++) {
    if (u & mask)
      x[i] = 1;
    else
      x[i] = 0;
    mask <<= 1;
  }
}

bit_vector_t
hnco::bv_from_string(const std::string& str)
{
  bit_vector_t result;

  result.reserve(str.size());
  for (auto c : str)
    if (c == '0')
      result.push_back(0);
    else
      result.push_back(1);
  assert(have_same_size(result, str));

  return result;
}

bit_vector_t
hnco::bv_from_stream(std::istream& stream)
{
  std::string line;

  if (std::getline(stream, line))
    return bv_from_string(line);
  else
    return bit_vector_t();
}
