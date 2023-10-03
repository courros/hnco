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
#include <fstream>
#include <sstream>

#include "hnco/functions/function.hh"

#include "make-multivariate-function-adapter.hh"

using namespace hnco::app;
using namespace hnco::function;
using namespace hnco;

std::string
hnco::app::read_file_content(std::string path)
{
  std::ifstream fstream(path);
  if (!fstream)
    throw std::runtime_error("read_file_content: Cannot open " + path);
  std::ostringstream sstream;
  sstream << fstream.rdbuf();
  return sstream.str();
}

std::vector<std::string>
hnco::app::split_string(std::string str, std::string delimiter)
{
  std::vector<std::string> items;
  auto start = 0U;
  auto stop = str.find(delimiter);
  while (stop != std::string::npos) {
    items.push_back(str.substr(start, stop - start));
    start = stop + delimiter.length();
    stop = str.find(delimiter, start);
  }
  items.push_back(str.substr(start));
  return items;
}

IntRepParams
hnco::app::parse_int_rep(std::string expression)
{
  auto parameters = split_string(expression, ",");
  if (parameters.size() != 2)
    throw std::runtime_error("parse_int_rep: Requires 2 parameters");
  int a = std::stoi(parameters[0]);
  int b = std::stoi(parameters[1]);
  return IntRepParams(a, b);
}

LongRepParams
hnco::app::parse_long_rep(std::string expression)
{
  auto parameters = split_string(expression, ",");
  if (parameters.size() != 2)
    throw std::runtime_error("parse_long_rep: Requires 2 parameters");
  long a = std::stol(parameters[0]);
  long b = std::stol(parameters[1]);
  return LongRepParams(a, b);
}
