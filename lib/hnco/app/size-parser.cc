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

#include <iostream>
#include <sstream>

#include "size-parser.hh"

namespace hnco {
namespace app {

std::optional<std::pair<std::string, int>> parse_size_declaration(std::string expression)
{
  const std::string delimiter = ":";

  auto start = 0U;
  auto stop = expression.find(delimiter);

  if (stop == std::string::npos) {
    std::cerr << "parse_size_declaration: Missing colon" << std::endl;
    return {};
  }

  std::string name;
  {
    auto before = expression.substr(start, stop);
    std::istringstream stream(before);
    stream >> name;
    if (!stream) {
      std::cerr << "parse_size_declaration: Expected variable name before colon" << std::endl;
      return {};
    }
  }

  start = stop + delimiter.length();
  int size = 0;
  {
    auto after = expression.substr(start);
    std::istringstream stream(after);
    stream >> size;
    if (!stream) {
      std::cerr << "parse_size_declaration: " << name << ": Expected size" << std::endl;
      return {};
    }
  }

  return std::make_pair(name, size);
}

std::unordered_map<std::string, int> parse_sizes(std::string expression)
{
  const std::string delimiter = ";";

  std::unordered_map<std::string, int> sizes;

  if (expression.empty())
    return sizes;

  auto start = 0U;
  auto stop = expression.find(delimiter);

  while (stop != std::string::npos) {
    auto opt = parse_size_declaration(expression.substr(start, stop - start));
    if (opt)
      sizes.insert(opt.value());
    start = stop + delimiter.length();
    stop = expression.find(delimiter, start);
  }

  auto opt = parse_size_declaration(expression.substr(start, stop - start));
  if (opt)
    sizes.insert(opt.value());

  return sizes;
}

}
}
