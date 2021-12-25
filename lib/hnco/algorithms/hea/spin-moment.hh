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

#ifndef HNCO_ALGORITHMS_HEA_SPIN_MOMENT_H
#define HNCO_ALGORITHMS_HEA_SPIN_MOMENT_H

#include <vector>
#include <iostream>

#include "hnco/bit-vector.hh"


namespace hnco {
namespace algorithm {
namespace hea {

struct LowerTriangularWalshMoment2
{
  std::vector<double> first_moment;
  std::vector<std::vector<double>> second_moment;

  LowerTriangularWalshMoment2(int n);

  void display(std::ostream& stream);
  void init();
  void add(const bit_vector_t& bv);
  void average(int count);
  void update(const LowerTriangularWalshMoment2& wm, double rate);
  void bound(double margin);

  /** Compute a scaled difference between two moments.

      In pseudo-code, it computes lambda * wm1 - wm2.
  */
  void scaled_difference(double lambda,
                         const LowerTriangularWalshMoment2& wm1,
                         const LowerTriangularWalshMoment2& wm2);

  double distance(const LowerTriangularWalshMoment2& wm) const;
  double norm_2() const;
  void uniform() { init(); }
};

}
}
}


#endif
