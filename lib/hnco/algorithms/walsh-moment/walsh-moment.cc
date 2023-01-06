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
#include <math.h>		// std::sqrt

#include "hnco/util.hh"

#include "matrix.hh" // hnco::algorithm::matrix_is_symmetric
#include "walsh-moment.hh"


using namespace hnco::algorithm::walsh_moment;

LowerTriangularWalshMoment2::LowerTriangularWalshMoment2(int n)
{
  assert(n > 0);

  first_moment.resize(n);
  second_moment.resize(n);
  for (size_t i = 0; i < second_moment.size(); i++)
    second_moment[i].resize(i);
}

void
LowerTriangularWalshMoment2::display(std::ostream& stream)
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
LowerTriangularWalshMoment2::init()
{
  std::fill(first_moment.begin(), first_moment.end(), 0);
  for (auto& row : second_moment)
    std::fill(row.begin(), row.end(), 0);
}

void
LowerTriangularWalshMoment2::add(const bit_vector_t& bv)
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
LowerTriangularWalshMoment2::average(int count)
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
LowerTriangularWalshMoment2::update(const LowerTriangularWalshMoment2& wm, double rate)
{
  assert(have_same_size(wm.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (wm.first_moment[i] - first_moment[i]);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = wm.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row2[j] - row[j]);
      assert(is_in_interval(row[j], -1, 1));
    }
  }
}

void
LowerTriangularWalshMoment2::update(const LowerTriangularWalshMoment2& wm1,
                                    const LowerTriangularWalshMoment2& wm2,
                                    double rate)
{
  assert(have_same_size(wm1.first_moment, first_moment));
  assert(have_same_size(wm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (wm1.first_moment[i] - wm2.first_moment[i]);

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = wm1.second_moment[i];
    const std::vector<double>& row2 = wm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row1[j] - row2[j]);
    }
  }
}

void
LowerTriangularWalshMoment2::scaled_difference(double lambda,
                                              const LowerTriangularWalshMoment2& wm1,
                                              const LowerTriangularWalshMoment2& wm2)
{
  assert(have_same_size(wm1.first_moment, first_moment));
  assert(have_same_size(wm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = lambda * wm1.first_moment[i] - wm2.first_moment[i];
    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = wm1.second_moment[i];
    const std::vector<double>& row2 = wm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = lambda * row1[j] - row2[j];
    }
  }
}

void
LowerTriangularWalshMoment2::bound(double margin)
{
  assert(is_in_interval(margin, 0, 1));

  const double high = 1 - margin;
  const double low = margin - 1;
  assert(low < high);

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = clip_value(first_moment[i], low, high);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = clip_value(row[j], low, high);
      assert(is_in_interval(row[j], -1, 1));
    }
  }
}

double
LowerTriangularWalshMoment2::norm_1() const
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
LowerTriangularWalshMoment2::norm_2() const
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
LowerTriangularWalshMoment2::norm_infinite() const
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
LowerTriangularWalshMoment2::distance(const LowerTriangularWalshMoment2& wm) const
{
  assert(have_same_size(wm.first_moment, first_moment));

  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += square(first_moment[i] - wm.first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = wm.second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += square(row[j] - row2[j]);
  }
  return std::sqrt(result);
}

SymmetricWalshMoment2::SymmetricWalshMoment2(int n)
{
  assert(n > 0);

  first_moment.resize(n);
  second_moment.resize(n);
  for (auto& row : second_moment)
    row.resize(n);
}

void
SymmetricWalshMoment2::display(std::ostream& stream)
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
SymmetricWalshMoment2::init()
{
  std::fill(first_moment.begin(), first_moment.end(), 0);
  for (auto& row : second_moment)
    std::fill(row.begin(), row.end(), 0);
}

void
SymmetricWalshMoment2::add(const bit_vector_t& bv)
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
SymmetricWalshMoment2::average(int count)
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
SymmetricWalshMoment2::update(const SymmetricWalshMoment2& wm, double rate)
{
  assert(have_same_size(wm.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (wm.first_moment[i] - first_moment[i]);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = wm.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row2[j] - row[j]);
      assert(is_in_interval(row[j], -1, 1));

      second_moment[j][i] = row[j];
    }
  }
  assert(matrix_is_symmetric(second_moment));
}

void
SymmetricWalshMoment2::update(const SymmetricWalshMoment2& wm1,
                                    const SymmetricWalshMoment2& wm2,
                                    double rate)
{
  assert(have_same_size(wm1.first_moment, first_moment));
  assert(have_same_size(wm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] += rate * (wm1.first_moment[i] - wm2.first_moment[i]);

    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = wm1.second_moment[i];
    const std::vector<double>& row2 = wm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] += rate * (row1[j] - row2[j]);

      second_moment[j][i] = row[j];
    }
  }
}

void
SymmetricWalshMoment2::scaled_difference(double lambda,
                                              const SymmetricWalshMoment2& wm1,
                                              const SymmetricWalshMoment2& wm2)
{
  assert(have_same_size(wm1.first_moment, first_moment));
  assert(have_same_size(wm2.first_moment, first_moment));

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = lambda * wm1.first_moment[i] - wm2.first_moment[i];
    std::vector<double>& row = second_moment[i];
    const std::vector<double>& row1 = wm1.second_moment[i];
    const std::vector<double>& row2 = wm2.second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = lambda * row1[j] - row2[j];

      second_moment[j][i] = row[j];
    }
  }
}

void
SymmetricWalshMoment2::bound(double margin)
{
  assert(is_in_interval(margin, 0, 1));

  const double high = 1 - margin;
  const double low = margin - 1;
  assert(low < high);

  for (size_t i = 0; i < first_moment.size(); i++) {
    first_moment[i] = clip_value(first_moment[i], low, high);
    assert(is_in_interval(first_moment[i], -1, 1));

    std::vector<double>& row = second_moment[i];
    for (size_t j = 0; j < i; j++) {
      row[j] = clip_value(row[j], low, high);
      assert(is_in_interval(row[j], -1, 1));

      second_moment[j][i] = row[j];
    }
  }
}

double
SymmetricWalshMoment2::norm_1() const
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
SymmetricWalshMoment2::norm_2() const
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
SymmetricWalshMoment2::norm_infinite() const
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
SymmetricWalshMoment2::distance(const SymmetricWalshMoment2& wm) const
{
  assert(have_same_size(wm.first_moment, first_moment));

  double result = 0;
  for (size_t i = 0; i < first_moment.size(); i++) {
    result += square(first_moment[i] - wm.first_moment[i]);
    const std::vector<double>& row = second_moment[i];
    const std::vector<double>& row2 = wm.second_moment[i];
    for (size_t j = 0; j < i; j++)
      result += square(row[j] - row2[j]);
  }
  return std::sqrt(result);
}
