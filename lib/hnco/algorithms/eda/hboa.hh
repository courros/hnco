/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_EDA_HBOA_H
#define HNCO_ALGORITHMS_EDA_HBOA_H

#include "hnco/algorithms/algorithm.hh"


namespace hnco {
namespace algorithm {
namespace eda {

  /** Hierarchical Bayesian Optimization %Algorithm.

      Implementation of the Hierarchical Bayesian Optimization
      %Algorithm and helper classes based on the publication: Pelikan,
      M. and Goldberg, D. (2006). Hierarchical bayesian optimization
      algorithm. In Scalable Optimization via Probabilistic Modeling,
      volume 33 of Studies in Computational Intelligence, pages
      63–90. Springer Berlin Heidelberg.

      Author: Brian W. Goldman

      Integrated into HNCO by Arnaud Berny
  */
  class Hboa:
    public Algorithm {

    /// Population size
    int _population_size = 10;

  public:

    /// Constructor
    Hboa(int n):
      Algorithm(n) {}

    /// Maximize
    void maximize();

    /// Set population size
    void set_population_size(int n) { _population_size = n; }

  };

} // end of namespace eda
} // end of namespace algorithm
} // end of namespace hnco


#endif
