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

#include "hnco/random.hh"

#include "tsp.hh"

using namespace hnco::function;
using namespace hnco::random;


void
Tsp::load_(std::istream& stream)
{
  std::string line;
  std::string key, value;

  while(!stream.eof()) {
    getline(stream, line);
    if (line.empty())
      continue;

    auto pos = line.find(":");
    std::string before = line.substr(0, pos);
    std::istringstream before_stream(before);
    before_stream >> key;

    std::string after = line.substr(pos + 1);
    std::istringstream after_stream(after);

    if (key == "NAME") {
      after_stream >> _name;
    } else if (key == "COMMENT") {
      _comment = line.substr(pos + 1);
      // should eat white at the beginning
    } else if (key == "TYPE") {
      after_stream >> value;
      if (value != "TSP")
        throw std::runtime_error("Tsp::load_: " + _name + " is not a TSP problem.");
    } else if (key == "DIMENSION") {
      after_stream >> _num_cities;
    } else if (key == "EDGE_WEIGHT_TYPE") {
      after_stream >> value;
      if (value == "ATT")
	_edge_weight_type = ATT;
      else if (value == "EUC_2D")
	_edge_weight_type = EUC_2D;
      else
        throw std::runtime_error("Tsp::load_: Unknown edge weight type: " + value);
    } else if (key == "NODE_COORD_SECTION") {
      break;
    }
  }
  load_coordinates(stream);
  compute_distances();
}

void
Tsp::load_coordinates(std::istream& stream)
{
  assert(_num_cities > 0);
  _x.resize(_num_cities);
  _y.resize(_num_cities);
  while (!stream.eof()) {
    int index;
    stream >> index;
    if (stream.fail())
      break;
    assert((index >= 1) && (index <= _num_cities));
    stream >> _x[index - 1];
    stream >> _y[index - 1];
    std::cout << index << ": " << _x[index - 1] << ", " << _y[index - 1] << std::endl;
  }
}

void
Tsp::compute_distances()
{
  assert(_num_cities > 0);
  _x.resize(_num_cities);
  _y.resize(_num_cities);
  _distances.resize(_num_cities);
  for (int i = 0; i < _num_cities; i++)
    _distances[i].resize(_num_cities);

  switch(_edge_weight_type) {

  case ATT:
    compute_distances_att();
    break;

  case EUC_2D:
    compute_distances_euc_2d();
    break;

  default:
    throw std::runtime_error("Tsp::compute_distances: Unknown edge weight type: " + std::to_string(_edge_weight_type));
  }
}

void
Tsp::compute_distances_att()
{
  for (int i = 0; i < _num_cities; i++)
    _distances[i][i] = 0;

  for (int i = 1; i < _num_cities; i++)
    for (int j = 0; j < i; j++) {
      float dx = _x[i] - _x[j];
      float dy = _y[i] - _y[j];
      _distances[i][j] = std::ceil(std::sqrt((dx * dx + dy * dy) / 10.0));
      _distances[j][i] = _distances[i][j];
    }
}

void
Tsp::compute_distances_euc_2d()
{
  for (int i = 0; i < _num_cities; i++)
    _distances[i][i] = 0;

  for (int i = 1; i < _num_cities; i++)
    for (int j = 0; j < i; j++) {
      float dx = _x[i] - _x[j];
      float dy = _y[i] - _y[j];
      _distances[i][j] = std::rint(sqrt(dx * dx + dy * dy));
      _distances[j][i] = _distances[i][j];
    }
}

void
Tsp::save_(std::ostream& stream) const
{
}


void
Tsp::display(std::ostream& stream) const
{
  std::cout << _name << std::endl;
  std::cout << _comment << std::endl;
  std::cout << _num_cities << std::endl;
  std::cout << _edge_weight_type << std::endl;
}


void
Tsp::describe(const hnco::permutation_t& permutation, std::ostream& stream)
{
}


double
Tsp::evaluate(const hnco::permutation_t& permutation)
{
  assert(_distances.size() > 0);
  double result = 0;
  int last = _distances.size() - 1;
  for (int i = 0; i < last; i++)
    result += _distances[permutation[i]][permutation[i+1]];
  result += _distances[permutation[last]][permutation[0]];
  return result;
}
