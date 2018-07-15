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

#include "hnco/exception.hh"
#include "hnco/iterator.hh"

#include "complete-search.hh"

using namespace hnco::algorithm;
using namespace hnco;


void
CompleteSearch::maximize()
{
  HypercubeIterator iterator(_solution.first.size());

  if (!iterator.has_next())
    throw exception::Error("CompleteSearch::maximize: empty hypercube");

  iterator.next();
  set_solution(iterator.get_current());

  while (iterator.has_next()) {
    iterator.next();
    update_solution(iterator.get_current());
  };

}
