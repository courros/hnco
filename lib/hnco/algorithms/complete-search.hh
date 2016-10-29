/* Copyright (C) 2016 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_COMPLETE_H
#define HNCO_ALGORITHMS_COMPLETE_H

#include "hnco/functions/function.hh"

#include "algorithm.hh"


namespace hnco {
namespace algorithm {


  /// Complete search
  class CompleteSearch:
    public Algorithm {

  public:

    /// Constructor
    CompleteSearch(int n):
      Algorithm(n) {}

    /// Maximize
    void maximize();

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
