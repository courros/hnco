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

#include "human.hh"

using namespace hnco;
using namespace hnco::algorithm;

inline bool string_is_valid(const std::string& str) { return std::all_of(str.begin(), str.end(), [](char c){ return c == '0' || c == '1'; }); }

bool
Human::parse_bit_vector()
{
  while(true) {
    std::string str;
    std::cout << "bv? ";
    std::cin >> str;

    if (str == "quit")
      return false;

    if (!string_is_valid(str)) {
      std::cout << "Warning: invalid string" << std::endl;
      continue;
    }

    _candidate = bv_from_string(str);

    if (int(_candidate.size()) < get_bv_size()) {
      std::cout << "Warning: bit vector is too short" << std::endl;
      continue;
    }

    if (int(_candidate.size()) > get_bv_size()) {
      std::cout << "Warning: bit vector is too long" << std::endl;
      continue;
    }

    return true;
  };
}

void
Human::init()
{
  assert(_function);

  if (parse_bit_vector()) {
    double value = _function->evaluate(_candidate);
    std::cout << "-> " << value << std::endl;
    set_solution(_candidate, value);
  } else
    _last_iteration = true;
}

void
Human::iterate()
{
  assert(_function);

  if (parse_bit_vector()) {
    double value = _function->evaluate(_candidate);
    std::cout << "-> " << value << std::endl;
    update_solution(_candidate, value);
  } else
    _last_iteration = true;
}
