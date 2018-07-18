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

#ifndef HNCO_ALGORITHMS_FAST_EFFICIENT_P3_LTGA_H
#define HNCO_ALGORITHMS_FAST_EFFICIENT_P3_LTGA_H

#include "hnco/algorithms/algorithm.hh"


namespace hnco::algorithm::fast_efficient_p3 {

  /** Ltga.

      Implementation of the Linkage Tree Genetic Algorithm Designed to
      match the variant in the paper: "Hierarchical problem solving
      with the linkage tree genetic algorithm" by D. Thierens and
      P. A. N. Bosman

      Author: Brian Goldman

      Integrated into HNCO by Arnaud Berny
  */
  class Ltga:
    public Algorithm {

    /// Population size
    int _population_size = 10;

  public:

    /// Constructor
    Ltga(int n):
      Algorithm(n) {}

    /// Maximize
    void maximize();

    /// Set population size
    void set_population_size(int n) { _population_size = n; }

  };

} // end of namespace hnco::algorithm::fast_efficient_p3


#endif
