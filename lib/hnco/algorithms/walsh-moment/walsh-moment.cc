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

#include <iostream>
#include <algorithm>            // std::clamp
#include <cassert>
#include <cmath>		// std::sqrt

#include "hnco/util.hh"         // hnco::have_same_size, hnco::is_in_interval, hnco::square

#include "matrix.hh"           // hnco::algorithm::matrix_is_symmetric
#include "walsh-moment.hh"

using namespace hnco::algorithm::walsh_moment;
using namespace hnco::algorithm;
using namespace hnco;

TriangularMoment::TriangularMoment(int n)
{
  assert(n > 0);

  first_moment.resize(n);
  second_moment.resize(n);
  for (size_t i = 0; i < second_moment.size(); i++)
    second_moment[i].resize(i);
}

void
TriangularMoment::display(std::ostream& stream)
{
  for (size_t i = 0; i < first_moment.size(); i++) {
    for (size_t j = 0; j < i; j++)
      stream << second_moment[i][j] << " ";
    stream << first_moment [i] << " ";
    for (size_t j = i + 1; j < first_moment.size(); j++)
      stream << second_moment[j][i] << " ";
    stream << std::endl;
  }
}

void
TriangularMoment::init()
{
  std::fill(first_moment.begin(), first_moment.end(), 0);
  for (auto& row : second_moment)
    std::fill(row.begin(), row.end(), 0);
}

void
TriangularMoment::add(const bit_vector_t& bv)
{
  assert(hnco::have_same_size(bv, first_moment));

  for (size_t i = 0; i < bv.size(); i++) {
    std::vector<double>& row = second_moment[i];
    if (bv[i]) {
      first_moment[i]--;
      for (size_t j = 0; j < i; j++)
        if (bv[j])
          row[j]++;
        else
          row[j]--;
    } else {
      first_moment[i]++;
      for (size_t j = 0; j < i; j++)
        if (bv[j])
          row[j]--;
        else
          row[j]++;
    }
  }
}

void
TriangularMoment::average(int count)
{
  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] /= count;
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] /= count;
      assert(is_in_interval(row[j], -1, 1));
    }
  }
}

void
TriangularMoment::update(const TriangularMoment& tm, double rate)
{
  assert(hnco::have_same_size(tm.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (tm.first_moment[i] - first_moment[i]);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = tm.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row2[j] - row[j]);
      assert(is_in_interval(row[j], -1, 1));
    }
  }
}

void
TriangularMoment::update(const TriangularMoment& tm1, const TriangularMoment& tm2, double rate)
{
  assert(hnco::have_same_size(tm1.first_moment, first_moment));
  assert(hnco::have_same_size(tm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (tm1.first_moment[i] - tm2.first_moment[i]);

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = tm1.second_moment[i];
    const std::vector<double>& row2 = tm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row1[j] - row2[j]);
    }
  }
}

void
TriangularMoment::scaled_difference(double lambda, const TriangularMoment& tm1, const TriangularMoment& tm2)
{
  assert(hnco::have_same_size(tm1.first_moment, first_moment));
  assert(hnco::have_same_size(tm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = lambda * tm1.first_moment[i] - tm2.first_moment[i];
    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = tm1.second_moment[i];
    const std::vector<double>& row2 = tm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = lambda * row1[j] - row2[j];
    }
  }
}

void
TriangularMoment::bound(double margin)
{
  assert(is_in_interval(margin, 0, 1));

  const double high = 1 - margin;
  const double low = margin - 1;
  assert(low < high);

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = std::clamp(first_moment[i], low, high);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = std::clamp(row[j], low, high);
      assert(is_in_interval(row[j], -1, 1));
    }
  }
}

double
TriangularMoment::norm_1() const
{
  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += std::fabs(first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += std::fabs(row[j]);
  }
  return result;
}

double
TriangularMoment::norm_2() const
{
  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += square(first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += square(row[j]);
  }
  return std::sqrt(result);
}

double
TriangularMoment::norm_infinite() const
{
  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result = std::max(result, first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++)
      result = std::max(result, row[j]);
  }
  return result;
}

double
TriangularMoment::distance(const TriangularMoment& tm) const
{
  assert(hnco::have_same_size(tm.first_moment, first_moment));

  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += square(first_moment[i] - tm.first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = tm.second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += square(row[j] - row2[j]);
  }
  return std::sqrt(result);
}

FullMoment::FullMoment(int n)
{
  assert(n > 0);

  first_moment.resize(n);
  second_moment.resize(n);
  for (auto& row : second_moment)
    row.resize(n);
}

void
FullMoment::display(std::ostream& stream)
{
  const size_t dimension = first_moment.size();
  for (size_t i = 0; i < dimension; i++) {
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < dimension; j++) {
      if (i == j)
        stream << first_moment[i] << " ";
      else
        stream << row[j] << " ";
    }
    stream << std::endl;
  }
}

void
FullMoment::init()
{
  std::fill(first_moment.begin(), first_moment.end(), 0);
  for (auto& row : second_moment)
    std::fill(row.begin(), row.end(), 0);
}

void
FullMoment::add(const bit_vector_t& bv)
{
  assert(have_same_size(bv, first_moment));

  for (size_t i = 0; i < bv.size(); i++) {
    std::vector<double>& row = second_moment[i];
    if (bv[i]) {
      first_moment[i]--;
      for (size_t j = 0; j < i; j++)
        if (bv[j])
          row[j]++;
        else
          row[j]--;
    } else {
      first_moment[i]++;
      for (size_t j = 0; j < i; j++)
        if (bv[j])
          row[j]--;
        else
          row[j]++;
    }
  }
}

void
FullMoment::average(int count)
{
  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] /= count;
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] /= count;
      assert(is_in_interval(row[j], -1, 1));

      second_moment[j][i] = row[j];
    }
  }
  assert(matrix_is_symmetric(second_moment));
}

void
FullMoment::update(const FullMoment& fm, double rate)
{
  assert(hnco::have_same_size(fm.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (fm.first_moment[i] - first_moment[i]);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = fm.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row2[j] - row[j]);
      assert(is_in_interval(row[j], -1, 1));

      second_moment[j][i] = row[j];
    }
  }
  assert(matrix_is_symmetric(second_moment));
}

void
FullMoment::update(const FullMoment& fm1, const FullMoment& fm2, double rate)
{
  assert(hnco::have_same_size(fm1.first_moment, first_moment));
  assert(hnco::have_same_size(fm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (fm1.first_moment[i] - fm2.first_moment[i]);

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = fm1.second_moment[i];
    const std::vector<double>& row2 = fm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row1[j] - row2[j]);

      second_moment[j][i] = row[j];
    }
  }
}

void
FullMoment::scaled_difference(double lambda, const FullMoment& fm1, const FullMoment& fm2)
{
  assert(hnco::have_same_size(fm1.first_moment, first_moment));
  assert(hnco::have_same_size(fm2.first_moment, first_moment));

  const size_t bv_size = first_moment.size();
  for (size_t i = 0; i < bv_size; i++) {
    first_moment[i] = lambda * fm1.first_moment[i] - fm2.first_moment[i];
    auto& row = second_moment[i];
    const auto& row1 = fm1.second_moment[i];
    const auto& row2 = fm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      double tmp = lambda * row1[j] - row2[j];
      row[j] = tmp;
      second_moment[j][i] = tmp;
    }
  }
}

void
FullMoment::bound(double margin)
{
  assert(is_in_interval(margin, 0, 1));

  const double high = 1 - margin;
  const double low = margin - 1;
  assert(low < high);

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = std::clamp(first_moment[i], low, high);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = std::clamp(row[j], low, high);
      assert(is_in_interval(row[j], -1, 1));

      second_moment[j][i] = row[j];
    }
  }
}

double
FullMoment::norm_1() const
{
  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += std::fabs(first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += std::fabs(row[j]);
  }
  return result;
}

double
FullMoment::norm_2() const
{
  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += square(first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += square(row[j]);
  }
  return std::sqrt(result);
}

double
FullMoment::norm_infinite() const
{
  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result = std::max(result, first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++)
      result = std::max(result, row[j]);
  }
  return result;
}

double
FullMoment::distance(const FullMoment& fm) const
{
  assert(hnco::have_same_size(fm.first_moment, first_moment));

  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += square(first_moment[i] - fm.first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = fm.second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += square(row[j] - row2[j]);
  }
  return std::sqrt(result);
}
