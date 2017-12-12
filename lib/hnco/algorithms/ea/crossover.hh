/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_EA_CROSSOVER
#define HNCO_ALGORITHMS_EA_CROSSOVER

#include "hnco/bit-vector.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


  /// Crossover
  class Crossover {

  public:

    /// Destructor
    virtual ~Crossover() {}

    /** Breed.

        The output bit vector is the crossover of parent bit vectors.

        \param p1 First parent bit vector
        \param p2 Second parent bit vector
        \param offspring Offspring bit vector
    */
    virtual void breed(const bit_vector_t& p1, const bit_vector_t& p2, bit_vector_t& offspring) = 0;

  };


  /// Uniform crossover
  class UniformCrossover {

  public:

    /** Breed.

        The output bit vector is the crossover of parent bit vectors.

        \param p1 First parent bit vector
        \param p2 Second parent bit vector
        \param offspring Offspring bit vector
    */
    void breed(const bit_vector_t& parent1, const bit_vector_t& parent2, bit_vector_t& offspring);

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
