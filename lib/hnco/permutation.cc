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

#include "permutation.hh"


using namespace hnco;


bool
hnco::perm_is_valid(const permutation_t& permutation)
{
  std::vector<bool> checked(permutation.size(), false);
  for (size_t i = 0; i < permutation.size(); i++) {
    if (permutation[i] < 0)
      return false;
    if (permutation[i] >= int(permutation.size()))
      return false;
    if (checked[permutation[i]])
      return false;
    checked[permutation[i]] = true;
  }
  return true;
}

void
hnco::perm_display(const permutation_t& permutation, std::ostream& stream)
{
  for (auto element : permutation)
    stream << element << ", ";
  stream << "last";
}
