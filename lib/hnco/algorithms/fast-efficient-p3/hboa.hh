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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_BOA_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_BOA_H

#include "hnco/algorithms/algorithm.hh"
#include "hnco/exception.hh"


/// Fast efficient P3
namespace hnco::algorithm::fast_efficient_p3 {

  /** Hboa.

      Implementation of the Hierarchical Bayesian Optimization
      Algorithm and helper classes based on the publication: Pelikan,
      M. and Goldberg, D. (2006). Hierarchical bayesian optimization
      algorithm. In Scalable Optimization via Probabilistic Modeling,
      volume 33 of Studies in Computational Intelligence, pages
      63–90. Springer Berlin Heidelberg.

      Author: Brian Goldman

      Integrated into HNCO by Arnaud Berny
  */
  class Hboa:
    public Algorithm {

  public:

    /// Constructor
    Hboa(int n):
      Algorithm(n) {}

    /// Maximize
    void maximize();

  };

} // end of namespace hnco::algorithm::fast_efficient_p3


#endif
