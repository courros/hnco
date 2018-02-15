/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "permutation.hh"


using namespace hnco;


bool hnco::perm_is_valid(const permutation_t& permutation)
{
  std::vector<bool> checked(permutation.size(), false);
  for (size_t i = 0; i < permutation.size(); i++) {
    assert(permutation[i] >= 0);
    assert(permutation[i] < permutation.size());
    if (checked[permutation[i]])
      return false;
    else
      checked[permutation[i]] = true;
  }
  return true;
}
