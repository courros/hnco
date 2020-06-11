/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include <chrono>               // std::chrono::system_clock

#include "random.hh"

using namespace hnco::random;


std::mt19937 Generator::engine;

unsigned Generator::seed = std::mt19937::default_seed;


void Generator::set_seed(unsigned n)
{
  seed = n;
  engine.seed(seed);
}

void Generator::set_seed()
{
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  engine.seed(seed);
}

void Generator::reset()
{
  engine.seed(seed);
}

double Generator::uniform()
{
  std::uniform_real_distribution<double> dist;
  return dist(engine);
}

double Generator::normal()
{
  std::normal_distribution<double> dist;
  return dist(engine);
}

bool Generator::bernoulli()
{
  std::bernoulli_distribution dist;
  return dist(engine);
}
