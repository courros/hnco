/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#include "hnco/random.hh"

#include "nearest-neighbor-ising-model-2.hh"


using namespace hnco::random;
using namespace hnco::function;


void
NearestNeighborIsingModel2::resize(int num_rows, int num_columns)
{
  assert(num_rows > 0);
  assert(num_columns > 0);

  _couplings_right = std::vector<std::vector<double> >(num_rows, std::vector<double>(num_columns));
  _couplings_below = std::vector<std::vector<double> >(num_rows, std::vector<double>(num_columns));
  _field = std::vector<std::vector<double> >(num_rows, std::vector<double>(num_columns));

  _flipped_bits = bit_vector_t(get_bv_size(), 0);
}


double
NearestNeighborIsingModel2::eval(const bit_vector_t& s)
{
  assert(_field.size() > 0);
  assert(_field[0].size() > 0);
  assert(s.size() == get_bv_size());

  const size_t num_rows = _field.size();
  const size_t last_row = num_rows - 1;

  const size_t num_columns = _field[0].size();
  const size_t last_column = num_columns - 1;

  double result = 0;

  for (size_t i = 0, start = 0; i < last_row; i++, start += num_columns) {
    for (size_t j = 0; j < last_column; j++) {
      size_t site = start + j;

      // Right
      assert(site + 1 < s.size());
      if ((s[site] + s[site + 1]) % 2 == 0)
        result += _couplings_right[i][j];
      else
        result -= _couplings_right[i][j];

      // Below
      assert(site + num_columns < s.size());
      if ((s[site] + s[site + num_columns]) % 2 == 0)
        result += _couplings_right[i][j];
      else
        result -= _couplings_right[i][j];

      // External field
      if (s[site])
        result -= _field[i][j];
      else
        result += _field[i][j];

    }
  }

  return result;
}


void
NearestNeighborIsingModel2::display(std::ostream& stream)
{
  assert(_field.size() > 0);

  stream << "NearestNeighborIsingModel2" << std::endl;
  stream << "nrows = " << _field.size() << std::endl;
  stream << "ncols = " << _field[0].size() << std::endl;
  stream << "bv_size = " << get_bv_size() << std::endl;
  stream << "Periodic boundary condition = " << _periodic_boundary_conditions << std::endl;
}
