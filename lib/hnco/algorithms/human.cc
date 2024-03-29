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

#include <string>

#include "hnco/exception.hh"

#include "human.hh"


using namespace hnco;
using namespace hnco::algorithm;
using namespace hnco::exception;


void
Human::parse_bit_vector()
{
  bool again = true;
  do {
    std::string str;
    std::cout << "bv? ";
    std::cin >> str;
    if (str == "quit")
      throw LastEvaluation();
    _candidate = bv_from_string(str);
    if (int(_candidate.size()) < get_bv_size())
      std::cout << "Warning: bit vector is too short" << std::endl;
    else {
      again = false;
      if (int(_candidate.size()) > get_bv_size()) {
        _candidate.resize(get_bv_size());
        std::cout << "Warning: bit vector has been shortened" << std::endl;
      }
    }
  } while (again);
}


void
Human::init()
{
  assert(_function);

  parse_bit_vector();
  double value = _function->evaluate(_candidate);
  std::cout << "value: " << value << std::endl;
  set_solution(_candidate, value);
}


void
Human::iterate()
{
  parse_bit_vector();
  double value = _function->evaluate(_candidate);
  std::cout << "value: " << value << std::endl;
  update_solution(_candidate, value);
}
