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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_HBOA_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_HBOA_H

#include <memory>               // std::shared_ptr

#include "hnco/algorithms/algorithm.hh"


namespace hnco {
namespace algorithm {
namespace fast_efficient_p3 {

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

    struct Implementation;

    /// Pointer to implementation
    std::unique_ptr<Implementation> _pimpl;;

    /// Population size
    int _population_size = 10;

  public:

    /// Constructor
    Hboa(int n);

    /// Initialization
    void init();

    /// Maximize
    void maximize();

    /// Finalize
    void finalize();

    /// Set population size
    void set_population_size(int n) { _population_size = n; }

  };

} // end of namespace fast_efficient_p3
} // end of namespace algorithm
} // end of namespace hnco


#endif
